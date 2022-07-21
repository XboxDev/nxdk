// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2022 Ryan Wendland

#include "nvnetdrv.h"
#include "nvnetdrv_regs.h"
#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <xboxkrnl/xboxkrnl.h>

#define NVNET_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define NVNET_RX_EMPTY (NULL)

struct __attribute__((packed)) descriptor_t
{
    uint32_t paddr;
    uint16_t length;
    uint16_t flags;
};

// Struct to hold user TX callback data
struct tx_misc_t
{
    void *bufAddr;
    size_t length;
    nvnetdrv_tx_callback_t callback;
    void *userdata;
};

// Struct to hold RX callback data
struct rx_misc_t
{
    void *bufAddr;
    size_t length;
};

#ifdef NVNETDRV_ENABLE_STATS
struct nvnetdrv_stats_t
{
    uint32_t rx_interrupts;
    uint32_t rx_extraByteErrors;
    uint32_t tx_interrupts;
    uint32_t phy_interrupts;
    uint32_t rx_receivedPackets;
    uint32_t rx_framingError;
    uint32_t rx_overflowError;
    uint32_t rx_crcError;
    uint32_t rx_error4;
    uint32_t rx_error3;
    uint32_t rx_error2;
    uint32_t rx_error1;
    uint32_t rx_missedFrameError;
};
static struct nvnetdrv_stats_t nvnetdrv_stats;
#define INC_STAT(statname, val) do {nvnetdrv_stats.statname += (val);} while(0);
#else
#define INC_STAT(statname, val)
#endif

// FIXME
#define BASE ((void *)0xFEF00000)
#define reg32(offset) (*((volatile uint32_t *)((uintptr_t)BASE + (offset))))

// Manage NIC
static atomic_bool g_running = false;
static uint8_t g_ethAddr[6];
static uint32_t g_linkSpeed;
static ULONG g_irq;
static KIRQL g_irql;
static KDPC g_dpcObj;
static KINTERRUPT g_interrupt;
static HANDLE g_irqThread;
static KEVENT g_irqEvent;

// Manage RX ring
static volatile struct descriptor_t *g_rxRingDescriptors;
static KSEMAPHORE g_rxRingFreeDescriptors;
static HANDLE g_rxRingRequeueThread;
static size_t g_rxRingBeginIndex;
static size_t g_rxRingEndIndex;
static uint8_t *g_rxRingBuffers;
static uint32_t g_rxRingBufferVtoP;

// Manage RX buffer callbacks to user network stack
static nvnetdrv_rx_callback_t g_rxCallback;
static KSEMAPHORE g_rxCallbackQueued;
static HANDLE g_rxCallbackThread;
struct rx_misc_t *g_rxCallbackQueue;
static size_t g_rxCallbackEndIndex;

// Manage RX buffer pool to supply RX ring
static void **g_rxBuffPool;
static size_t g_rxBuffPoolHead;
static size_t g_rxBuffCnt;
static RTL_CRITICAL_SECTION g_rxBuffPoolLock;
static KSEMAPHORE g_rxFreeBuffers;

// Manage TX ring
static volatile struct descriptor_t *g_txRingDescriptors;
static size_t g_txRingBeginIndex;
static atomic_size_t g_txRingEndIndex;
static atomic_size_t g_txRingDescriptorsInUseCount;
static KSEMAPHORE g_txRingFreeDescriptors;
struct tx_misc_t tx_misc[TX_RING_SIZE];

// Time constants used in nvnetdrv
static LARGE_INTEGER no_sleep = {.QuadPart = 0};
static LARGE_INTEGER tenmicros = {.QuadPart = -100};
static LARGE_INTEGER fiftymicros = {.QuadPart = -500};

static void nvnetdrv_rx_push (void *buffer_virt)
{
    RtlEnterCriticalSection(&g_rxBuffPoolLock);
    g_rxBuffPool[++g_rxBuffPoolHead] = buffer_virt;
    RtlLeaveCriticalSection(&g_rxBuffPoolLock);
    KeReleaseSemaphore(&g_rxFreeBuffers, IO_NETWORK_INCREMENT, 1, FALSE);
}

static void *nvnetdrv_rx_pop (void)
{
    void *p;
    KeWaitForSingleObject(&g_rxFreeBuffers, Executive, KernelMode, FALSE, NULL);
    RtlEnterCriticalSection(&g_rxBuffPoolLock);
    p = g_rxBuffPool[g_rxBuffPoolHead--];
    RtlLeaveCriticalSection(&g_rxBuffPoolLock);
    return p;
}

static inline void nvnetdrv_irq_disable (void)
{
    reg32(NvRegIrqMask) = 0;
}

static inline void nvnetdrv_irq_enable (void)
{
    reg32(NvRegIrqMask) = NVREG_IRQMASK_THROUGHPUT;
}

static BOOLEAN NTAPI nvnetdrv_isr (PKINTERRUPT Interrupt, PVOID ServiceContext)
{
    nvnetdrv_irq_disable();
    KeInsertQueueDpc(&g_dpcObj, NULL, NULL);
    return TRUE;
}

static void NTAPI nvnetdrv_dpc (PKDPC Dpc, PVOID DeferredContext, PVOID arg1, PVOID arg2)
{
    KeSetEvent(&g_irqEvent, IO_NETWORK_INCREMENT, FALSE);
}

static inline uint32_t nvnetdrv_rx_ptov (uint32_t phys_address)
{
    return (phys_address == 0) ? 0 : (phys_address + g_rxRingBufferVtoP);
}

static inline uint32_t nvnetdrv_rx_vtop (uint32_t virt_address)
{
    return (virt_address == 0) ? 0 : (virt_address - g_rxRingBufferVtoP);
}

static void nvnetdrv_rx_requeue (size_t buffer_index)
{
    assert(buffer_index < RX_RING_SIZE);
    void *rx_buff = nvnetdrv_rx_pop();
    assert (rx_buff != NULL);
    g_rxRingDescriptors[buffer_index].paddr = nvnetdrv_rx_vtop((uint32_t)rx_buff);
    g_rxRingDescriptors[buffer_index].length = NVNET_RX_BUFF_LEN;
    g_rxRingDescriptors[buffer_index].flags = NV_RX_AVAIL;
}

static void nvnetdrv_handle_rx_irq (void)
{
    LONG freed_descriptors = 0;

    while (g_rxRingDescriptors[g_rxRingBeginIndex].paddr != NVNET_RX_EMPTY) {

        volatile struct descriptor_t *rx_packet = &g_rxRingDescriptors[g_rxRingBeginIndex];
        uint16_t flags = rx_packet->flags;

        if (flags & NV_RX_AVAIL) {
            // Reached a descriptor that still belongs to the NIC
            break;
        }

        if ((flags & NV_RX_DESCRIPTORVALID) == 0) {
            goto release_packet;
        }

        if (!(flags & NV_RX_ERROR) || (flags & NV_RX_FRAMINGERR)) {
            uint16_t packet_length = rx_packet->length;

            if (flags & NV_RX_SUBTRACT1) {
                INC_STAT(rx_extraByteErrors, 1);
                if (packet_length > 0)
                    packet_length--;
            }

            if (flags & NV_RX_FRAMINGERR) {
                INC_STAT(rx_framingError, 1);
            }

            INC_STAT(rx_receivedPackets, 1);

            // Queue a pending RX callback. We dont want to call it directly from the IRQ thread
            // incase the users network stack blocks in the callback.
            g_rxCallbackQueue[g_rxCallbackEndIndex].length = packet_length;
            g_rxCallbackQueue[g_rxCallbackEndIndex].bufAddr = (void *)nvnetdrv_rx_ptov(rx_packet->paddr);
            g_rxCallbackEndIndex = (g_rxCallbackEndIndex + 1) % g_rxBuffCnt;
            KeReleaseSemaphore(&g_rxCallbackQueued, IO_NETWORK_INCREMENT, 1, FALSE);
            goto next_packet;
        }
        else
        {
            if (flags & NV_RX_MISSEDFRAME) INC_STAT(rx_missedFrameError, 1);
            if (flags & NV_RX_OVERFLOW) INC_STAT(rx_overflowError, 1);
            if (flags & NV_RX_CRCERR) INC_STAT(rx_crcError, 1);
            if (flags & NV_RX_ERROR4) INC_STAT(rx_error4, 1);
            if (flags & NV_RX_ERROR3) INC_STAT(rx_error3, 1);
            if (flags & NV_RX_ERROR2) INC_STAT(rx_error2, 1);
            if (flags & NV_RX_ERROR1) INC_STAT(rx_error1, 1);
            goto release_packet;
        }

    //On error drop packet and release buffer
    release_packet:
        nvnetdrv_rx_release((void *)nvnetdrv_rx_ptov(rx_packet->paddr));
        // Fallthrough
    //A successful RX the packet is passed to user but we clear it from the RX ring which will be repopulated with
    //a spare RX buffer if available to keep data flowing while the user holds their buffer.
    next_packet:
        rx_packet->paddr = NVNET_RX_EMPTY;
        freed_descriptors++;
        g_rxRingBeginIndex = (g_rxRingBeginIndex + 1) % RX_RING_SIZE;
    }
    KeReleaseSemaphore(&g_rxRingFreeDescriptors, IO_NETWORK_INCREMENT, freed_descriptors, FALSE);
    INC_STAT(rx_interrupts, 1);
}

static void nvnetdrv_handle_tx_irq (void)
{
    LONG freed_descriptors = 0;

    while (g_txRingDescriptorsInUseCount > 0) {
        uint16_t flags = g_txRingDescriptors[g_txRingBeginIndex].flags;

        if (flags & NV_TX_VALID) {
            // We reached a descriptor that wasn't processed by hw yet
            // Re-init the transfer to ensure the NIC sends it
            reg32(NvRegTxRxControl) = NVREG_TXRXCTL_KICK;
            break;
        }

        // Buffers get locked before sending and unlocked after sending
        MmLockUnlockBufferPages(tx_misc[g_txRingBeginIndex].bufAddr, tx_misc[g_txRingBeginIndex].length, TRUE);

        // If registered, call the users tx complete callback funciton.
        if (tx_misc[g_txRingBeginIndex].callback) {
            tx_misc[g_txRingBeginIndex].callback(tx_misc[g_txRingBeginIndex].userdata);
        }

        freed_descriptors++;
        g_txRingBeginIndex = (g_txRingBeginIndex + 1) % TX_RING_SIZE;
        g_txRingDescriptorsInUseCount--;
    }

    KeReleaseSemaphore(&g_txRingFreeDescriptors, IO_NETWORK_INCREMENT, freed_descriptors, FALSE);
    INC_STAT(tx_interrupts, 1);
}

static void nvnetdrv_handle_mii_irq (uint32_t miiStatus, bool init)
{
    uint32_t adapterControl = reg32(NvRegAdapterControl);
    uint32_t linkState = PhyGetLinkState(!init);

    if (miiStatus & NVREG_MIISTAT_LINKCHANGE) {
        nvnetdrv_stop_txrx();
    }

    if (linkState & XNET_ETHERNET_LINK_10MBPS) {
        g_linkSpeed = NVREG_LINKSPEED_10MBPS;
    } else {
        g_linkSpeed = NVREG_LINKSPEED_100MBPS;
    }

    if (linkState & XNET_ETHERNET_LINK_FULL_DUPLEX) {
        reg32(NvRegDuplexMode) &= NVREG_DUPLEX_MODE_FDMASK;
    } else {
        reg32(NvRegDuplexMode) |= NVREG_DUPLEX_MODE_HDFLAG;
    }

    if (miiStatus & NVREG_MIISTAT_LINKCHANGE) {
        nvnetdrv_start_txrx();
    }

    INC_STAT(phy_interrupts, 1);
}

static void nvnetdrv_handle_irq (void)
{
    while (true)
    {
        uint32_t irq = reg32(NvRegIrqStatus);
        uint32_t mii = reg32(NvRegMIIStatus);

        //No interrupts left to handle. Leave
        if (!irq && !mii)
            break;

        // Acknowledge interrupts
        reg32(NvRegMIIStatus) = mii;
        reg32(NvRegIrqStatus) = irq;

        // Handle interrupts
        if (irq & NVREG_IRQ_RX_ALL) {
            nvnetdrv_handle_rx_irq();
        }
        if (irq & NVREG_IRQ_TX_ALL) {
            nvnetdrv_handle_tx_irq();
        }
        if (irq & NVREG_IRQ_LINK) {
            nvnetdrv_handle_mii_irq(mii, false);
        }
    }
}

static void NTAPI irq_thread(PKSTART_ROUTINE StartRoutine, PVOID StartContext)
{
    (void)StartRoutine;
    (void)StartContext;

    while (true) {
        KeWaitForSingleObject(&g_irqEvent, Executive, KernelMode, FALSE, NULL);
        if (!g_running) break;

        nvnetdrv_handle_irq();
        nvnetdrv_irq_enable();
    }

    PsTerminateSystemThread(0);
}

static void NTAPI rxrequeue_thread(PKSTART_ROUTINE StartRoutine, PVOID StartContext)
{
    (void)StartRoutine;
    (void)StartContext;

    while (true) {
        //Sleep until there is an empty descriptor in the RX ring
        if (!g_running) break;
        KeWaitForSingleObject(&g_rxRingFreeDescriptors, Executive, KernelMode, FALSE, NULL);
        if (!g_running) break;

        do {
            nvnetdrv_rx_requeue(g_rxRingEndIndex);
            g_rxRingEndIndex = (g_rxRingEndIndex + 1) % RX_RING_SIZE;
        } while (g_running &&
                 KeWaitForSingleObject(&g_rxRingFreeDescriptors, Executive,
                                       KernelMode, FALSE, &no_sleep) == STATUS_SUCCESS);
    }
    PsTerminateSystemThread(0);
}

static void NTAPI rxcallback_thread(PKSTART_ROUTINE StartRoutine, PVOID StartContext)
{
    (void)StartRoutine;
    (void)StartContext;

    size_t idx = 0;

    while (true) {
        //Sleep until there is an RX callback that needs processing
        if (!g_running) break;
        KeWaitForSingleObject(&g_rxCallbackQueued, Executive, KernelMode, FALSE, NULL);
        if (!g_running) break;

        do {
            g_rxCallback(g_rxCallbackQueue[idx].bufAddr, g_rxCallbackQueue[idx].length);
            idx = (idx + 1) % g_rxBuffCnt;
        } while (g_running &&
                 KeWaitForSingleObject(&g_rxCallbackQueued, Executive,
                                       KernelMode, FALSE, &no_sleep) == STATUS_SUCCESS);
    }
    PsTerminateSystemThread(0);
}

const uint8_t *nvnetdrv_get_ethernet_addr ()
{
    return g_ethAddr;
}

int nvnetdrv_init(size_t rx_buffer_count, nvnetdrv_rx_callback_t rx_callback)
{
    assert(!g_running);
    assert(rx_callback);
    assert(rx_buffer_count > 1);

    g_rxCallback = rx_callback;
    g_rxBuffCnt = rx_buffer_count;

    // Get Mac Address from EEPROM
    ULONG type;
    NTSTATUS status = ExQueryNonVolatileSetting(XC_FACTORY_ETHERNET_ADDR, &type, &g_ethAddr, 6, NULL);
    if (!NT_SUCCESS(status)) {
        return NVNET_NO_MAC;
    }

    // Allocate memory for TX and RX ring descriptors.
    void *descriptors = MmAllocateContiguousMemoryEx((RX_RING_SIZE + TX_RING_SIZE) * sizeof(struct descriptor_t),
                                                     0, 0xFFFFFFFF, 0, PAGE_READWRITE);
    if (!descriptors) {
        return NVNET_NO_MEM;
    }

    // Allocate memory for RX buffers. TX buffers are supplied by the user.
    g_rxRingBuffers = MmAllocateContiguousMemoryEx(g_rxBuffCnt * NVNET_RX_BUFF_LEN,
                                               0, 0xFFFFFFFF, 0, PAGE_READWRITE);
    if (!g_rxRingBuffers) {
        MmFreeContiguousMemory(descriptors);
        return NVNET_NO_MEM;
    }

    // Allocate memory for storing our push/pop stack for spare RX buffer
    g_rxBuffPool = (void **)malloc(g_rxBuffCnt * sizeof(void *));
    if (!g_rxBuffPool) {
        MmFreeContiguousMemory(descriptors);
        MmFreeContiguousMemory(g_rxRingBuffers);
        return NVNET_NO_MEM;
    }

    // Allocate memory for storing complete RX transfers, ready for callbacks.
    g_rxCallbackQueue = malloc(g_rxBuffCnt * sizeof(struct rx_misc_t));
    if (!g_rxCallbackQueue) {
        MmFreeContiguousMemory(descriptors);
        MmFreeContiguousMemory(g_rxRingBuffers);
        free(g_rxBuffPool);
        return NVNET_NO_MEM;
    }

    RtlZeroMemory(descriptors, (RX_RING_SIZE + TX_RING_SIZE) * sizeof(struct descriptor_t));
    RtlZeroMemory(g_rxCallbackQueue, g_rxBuffCnt * sizeof(struct rx_misc_t));
    RtlZeroMemory(g_rxBuffPool, g_rxBuffCnt * sizeof(void *));

    // Reset NIC. MSDash delays 10us here
    nvnetdrv_stop_txrx();
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_RESET;
    KeDelayExecutionThread(KernelMode, FALSE, &tenmicros);
    reg32(NvRegTxRxControl) = 0;
    KeDelayExecutionThread(KernelMode, FALSE, &tenmicros);

    // Disable interrupts while we initialise the NIC
    reg32(NvRegIrqMask) = 0;
    reg32(NvRegMIIMask) = 0;

    // Acknowledge any existing interrupts status bits
    reg32(NvRegTransmitterStatus) = reg32(NvRegTransmitterStatus);
    reg32(NvRegReceiverStatus) = reg32(NvRegReceiverStatus);
    reg32(NvRegIrqStatus) = reg32(NvRegIrqStatus);
    reg32(NvRegMIIStatus) = reg32(NvRegMIIStatus);

    // Clear latent registers
    reg32(NvRegWakeUpFlags) = 0;
    reg32(NvRegPollingControl) = 0;
    reg32(NvRegTransmitPoll) = 0;
    reg32(NvRegLinkSpeed) = 0;

    // Reset local ring tracking variables
    g_rxRingBeginIndex = 0;
    g_txRingBeginIndex = 0;
    g_txRingEndIndex = 0;
    g_txRingDescriptorsInUseCount = 0;
    g_rxCallbackEndIndex = 0;
    RtlZeroMemory(tx_misc, sizeof(tx_misc));

    // Setup the TX and RX ring descriptor pointers
    g_rxRingDescriptors = (struct descriptor_t *)descriptors;
    g_txRingDescriptors = (struct descriptor_t *)descriptors + RX_RING_SIZE;

    // Remember the offset between virtual and physical address
    g_rxRingBufferVtoP = ((uint32_t)g_rxRingBuffers) - (uint32_t)MmGetPhysicalAddress(g_rxRingBuffers);

    // Setup some fixed registers for the NIC
    reg32(NvRegMacAddrA) = (g_ethAddr[0] << 0) | (g_ethAddr[1] << 8) | (g_ethAddr[2] << 16) | (g_ethAddr[3] << 24);
    reg32(NvRegMacAddrB) = (g_ethAddr[4] << 0) | (g_ethAddr[5] << 8);
    reg32(NvRegMulticastAddrA) = NVREG_MCASTMASKA_NONE;
    reg32(NvRegMulticastAddrB) = NVREG_MCASTMASKB_NONE;
    reg32(NvRegMulticastMaskA) = NVREG_MCASTMASKA_NONE;
    reg32(NvRegMulticastMaskB) = NVREG_MCASTMASKB_NONE;
    reg32(NvRegOffloadConfig) = NVREG_OFFLOAD_NORMAL;
    reg32(NvRegPacketFilterFlags) = NVREG_PFF_ALWAYS_MYADDR;
    reg32(NvRegDuplexMode) = NVREG_DUPLEX_MODE_FORCEH;

    //Pseudo random slot time to minimise collisions
    reg32(NvRegSlotTime) = ((rand() % 0xFF) & NVREG_SLOTTIME_MASK) | NVREG_SLOTTIME_10_100_FULL;
    reg32(NvRegTxDeferral) = NVREG_TX_DEFERRAL_RGMII_10_100;
    reg32(NvRegRxDeferral) = NVREG_RX_DEFERRAL_DEFAULT;

    // MS Dash does this and sets up both these registers with 0x300010)
    reg32(NvRegUnknownSetupReg7) = NVREG_UNKSETUP7_VAL1; //RxWatermark?
    reg32(NvRegTxWatermark) = NVREG_UNKSETUP7_VAL1;

    // Point the NIC to our TX and RX ring buffers. NIC expects Ring size as size-1.
    reg32(NvRegTxRingPhysAddr) = MmGetPhysicalAddress((void *)g_txRingDescriptors);
    reg32(NvRegRxRingPhysAddr) = MmGetPhysicalAddress((void *)g_rxRingDescriptors);
    reg32(NvRegRingSizes) = ((RX_RING_SIZE - 1) << NVREG_RINGSZ_RXSHIFT) |
                            ((TX_RING_SIZE - 1) << NVREG_RINGSZ_TXSHIFT);

    // Prepare for Phy Init
    reg32(NvRegAdapterControl) = (1 << NVREG_ADAPTCTL_PHYSHIFT) | NVREG_ADAPTCTL_PHYVALID;
    reg32(NvRegMIISpeed) = NVREG_MIISPEED_BIT8 | NVREG_MIIDELAY;
    reg32(NvRegMIIMask) = NVREG_MII_LINKCHANGE;
    KeDelayExecutionThread(KernelMode, FALSE, &fiftymicros);

    // Initialise the transceiver
    if (PhyInitialize(FALSE, NULL) != STATUS_SUCCESS) {
        MmFreeContiguousMemory(descriptors);
        MmFreeContiguousMemory(g_rxRingBuffers);
        free(g_rxCallbackQueue);
        return NVNET_PHY_ERR;
    }

    // Short delay to allow the phy to startup. MSDash delays 50us
    reg32(NvRegAdapterControl) |= NVREG_ADAPTCTL_RUNNING;
    KeDelayExecutionThread(KernelMode, FALSE, &fiftymicros);

    // The NIC hardware IRQ queues a DPC. The DPC then sets g_irqEvent.
    // g_irqEvent is monitored by irqthread to handle the IRQ
    g_irq = HalGetInterruptVector(4, &g_irql);
    KeInitializeInterrupt(&g_interrupt, &nvnetdrv_isr, NULL, g_irq, g_irql, LevelSensitive, TRUE);
    KeInitializeDpc(&g_dpcObj, nvnetdrv_dpc, NULL);
    KeInitializeEvent(&g_irqEvent, SynchronizationEvent, FALSE);

    // We use semaphores to track the number of free TX and RX ring descriptors, the number of free RX buffers
    // available to be queued in the RX ring and the number of pending rx callbacks.
    KeInitializeSemaphore(&g_txRingFreeDescriptors, TX_RING_SIZE, TX_RING_SIZE);
    KeInitializeSemaphore(&g_rxRingFreeDescriptors, RX_RING_SIZE, RX_RING_SIZE);
    KeInitializeSemaphore(&g_rxFreeBuffers, 0, g_rxBuffCnt);
    KeInitializeSemaphore(&g_rxCallbackQueued, 0, g_rxBuffCnt);

    // Setup the push/pop stack for all our RX buffers. RX buffers are stored here until they can
    // be pushed into the RX ring. We need to handle the case where the user supplies less buffers
    // than can fit in the RX ring, also if excess (spare) buffers are supplied.
    g_rxBuffPoolHead = -1;
    RtlInitializeCriticalSection(&g_rxBuffPoolLock);
    for (uint32_t i = 0; i < g_rxBuffCnt; i++) {
        nvnetdrv_rx_push(g_rxRingBuffers + (i * NVNET_RX_BUFF_LEN));
    }

    // Fill our rx ring descriptor. g_rxBuffCnt may be less than the ring size, so only fill what we can.
    for (uint32_t i = 0; i < NVNET_MIN(g_rxBuffCnt, RX_RING_SIZE); i++) {
        KeWaitForSingleObject(&g_rxRingFreeDescriptors, Executive, KernelMode, FALSE, NULL);
        nvnetdrv_rx_requeue(i);
    }

    // g_rxRingEndIndex stores the position in the RX ring that is empty or about to be empty on the next packet.
    g_rxRingEndIndex = NVNET_MIN(g_rxBuffCnt, RX_RING_SIZE) % RX_RING_SIZE;

    // Get link speed settings from Phy
    nvnetdrv_handle_mii_irq(0, true);

    // Set running flag. This needs to happen before we connect the irq and start threads.
    bool prev_value = atomic_exchange(&g_running, true);
    assert(!prev_value);

    // Create a minimal stack, no TLS thread to handle NIC events
    PsCreateSystemThreadEx(&g_irqThread, 0, 4096, 0, NULL, NULL, NULL, FALSE, FALSE, irq_thread);

    // Create a minimal stack, no TLS thread to handle NIC RX ring buffer re-queing
    PsCreateSystemThreadEx(&g_rxRingRequeueThread, 0, 4096, 0, NULL, NULL, NULL, FALSE, FALSE, rxrequeue_thread);

    // Create a minimal stack, no TLS thread to handle NIC RX callbacks to user application
    PsCreateSystemThreadEx(&g_rxCallbackThread, 0, 4096, 0, NULL, NULL, NULL, FALSE, FALSE, rxcallback_thread);

    // Connect the NIC IRQ to the ISR
    status = KeConnectInterrupt(&g_interrupt);
    if (!NT_SUCCESS(status)) {
        nvnetdrv_stop();
        return NVNET_SYS_ERR;
    }

    // Start getting and sending data
    nvnetdrv_irq_enable();
    nvnetdrv_start_txrx();

    return NVNET_OK;
}

void nvnetdrv_stop(void)
{
    assert(g_running);

    KeDisconnectInterrupt(&g_interrupt);

    // Disable DMA and wait for it to idle, re-checking every 50 microseconds
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_DISABLE;
    for (int i = 0; i < 10000; i++) {
        if (reg32(NvRegTxRxControl) & NVREG_TXRXCTL_IDLE) {
            break;
        }
        KeDelayExecutionThread(KernelMode, FALSE, &fiftymicros);
    }

    //Stop NIC processing rings
    nvnetdrv_stop_txrx();

    //Clear the nvnet running flag so threads know to end
    bool prev_value = atomic_exchange(&g_running, false);
    assert(prev_value);

    // End the IRQ event-handling thread
    KeSetEvent(&g_irqEvent, IO_NETWORK_INCREMENT, FALSE);
    NtWaitForSingleObject(g_irqThread, FALSE, NULL);
    NtClose(g_irqThread);

    // Pass back all TX buffers to user.
    for (int i = 0; i < TX_RING_SIZE; i++) {
        if (tx_misc[i].callback) {
            tx_misc[i].callback(tx_misc[i].userdata);
        }
    }

    // Free all TX descriptors g_txRingFreeDescriptors so nvnetdrv_acquire_tx_descriptors will return.
    KeReleaseSemaphore(&g_txRingFreeDescriptors, IO_NETWORK_INCREMENT, g_txRingDescriptorsInUseCount, NULL);

    // End rxrequeue_thread
    nvnetdrv_rx_push(g_rxRingBuffers); //Just push a buffer into stack so we dont get stuck waiting for one
    KeReleaseSemaphore(&g_rxRingFreeDescriptors, IO_NETWORK_INCREMENT, 1, NULL);
    NtWaitForSingleObject(g_rxRingRequeueThread, FALSE, NULL);
    NtClose(g_rxRingRequeueThread);

    // End rxcallback_thread
    KeReleaseSemaphore(&g_rxCallbackQueued, IO_NETWORK_INCREMENT, 1, NULL);
    NtWaitForSingleObject(g_rxCallbackThread, FALSE, NULL);
    NtClose(g_rxCallbackThread);

    // Reset TX & RX control
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_DISABLE | NVREG_TXRXCTL_RESET;
    KeDelayExecutionThread(KernelMode, FALSE, &tenmicros);
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_DISABLE;

    // Free all memory allocated by nvnetdrv
    RtlDeleteCriticalSection(&g_rxBuffPoolLock);
    MmFreeContiguousMemory((void *)g_rxRingDescriptors);
    MmFreeContiguousMemory((void *)g_rxRingBuffers);
    free(g_rxCallbackQueue);
    free(g_rxBuffPool);
}

void nvnetdrv_start_txrx (void)
{
    reg32(NvRegLinkSpeed) = g_linkSpeed | NVREG_LINKSPEED_FORCE;
    reg32(NvRegTransmitterControl) |= NVREG_XMITCTL_START;
    reg32(NvRegReceiverControl) |= NVREG_RCVCTL_START;
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_KICK | NVREG_TXRXCTL_GET;
}

void nvnetdrv_stop_txrx (void)
{
    reg32(NvRegReceiverControl) &= ~NVREG_RCVCTL_START;
    reg32(NvRegTransmitterControl) &= ~NVREG_XMITCTL_START;

    //Wait for active TX and RX descriptors to finish
    for (int i = 0; i < 50000; i++) {
        if (!((reg32(NvRegReceiverStatus) & NVREG_RCVSTAT_BUSY) ||
              (reg32(NvRegTransmitterStatus) & NVREG_XMITSTAT_BUSY))) {
            break;
        }
        KeDelayExecutionThread(KernelMode, FALSE, &tenmicros);
    }

    reg32(NvRegLinkSpeed) = 0;
    reg32(NvRegTransmitPoll) = 0;
}

int nvnetdrv_acquire_tx_descriptors (size_t count)
{
    NTSTATUS status;

    // Sanity check
    assert(count > 0);
    // Avoid excessive requests
    assert(count <= 4);

    if (!g_running)
        return false;

    while (true) {
        // Wait for TX descriptors to become available
        KeWaitForSingleObject(&g_txRingFreeDescriptors, Executive, KernelMode, FALSE, NULL);

        if (!g_running) return false;

        // We want to try claim all tx descriptors at once without sleeping.
        size_t i = 0;
        for (i = 0; i < count - 1; i++) {
            // Try to acquire remaining descriptors without sleeping
            status = KeWaitForSingleObject(&g_txRingFreeDescriptors, Executive, KernelMode, FALSE, &no_sleep);
            if (!NT_SUCCESS(status) || status == STATUS_TIMEOUT) {
                // Couldn't acquire all at once, back off
                KeReleaseSemaphore(&g_txRingFreeDescriptors, IO_NETWORK_INCREMENT, i + 1, NULL);
                if (status == STATUS_TIMEOUT) {
                    // Sleep for 10 microseconds to avoid immediate re-locking
                    KeDelayExecutionThread(UserMode, FALSE, &tenmicros);
                    // Retry
                    break;
                } else {
                    return false;
                }
            }
        }

        if (!g_running) return false;

        //If we have claimed all the tx descriptors. We are done.
        if (i == (count - 1))
            break;
    }
    return true;
}

void nvnetdrv_submit_tx_descriptors (nvnetdrv_descriptor_t *buffers, size_t count)
{
    // Sanity check
    assert(count > 0);
    // Avoid excessive requests
    assert(count <= 4);

    if (!g_running)
        return;

    // Check that no buffer crosses a page boundary
    for (size_t i = 0; i < count; i++) {
        // For 4KiB pages, the least significant 12 bits are intra-page offsets, so shift them away and compare the rest
        assert(((uint32_t)buffers[i].addr >> 12) == (((uint32_t)buffers[i].addr + buffers[i].length - 1) >> 12));
    }

    // We don't check for buffer overrun here, because the Semaphore already protects us
    size_t descriptors_index = g_txRingEndIndex;
    while (!atomic_compare_exchange_weak(&g_txRingEndIndex, &descriptors_index,
                                         (descriptors_index + count) % TX_RING_SIZE));

    for (size_t i = 0; i < count; i++) {
        size_t current_descriptor_index = (descriptors_index + i) % TX_RING_SIZE;

        tx_misc[current_descriptor_index].bufAddr = buffers[i].addr;
        tx_misc[current_descriptor_index].length = buffers[i].length;
        tx_misc[current_descriptor_index].userdata = buffers[i].userdata;
        tx_misc[current_descriptor_index].callback = buffers[i].callback;

        // Buffers get locked before sending and unlocked after sending
        MmLockUnlockBufferPages(buffers[i].addr, buffers[i].length, FALSE);
        g_txRingDescriptors[current_descriptor_index].paddr = MmGetPhysicalAddress(buffers[i].addr);
        g_txRingDescriptors[current_descriptor_index].length = buffers[i].length - 1;
        g_txRingDescriptors[current_descriptor_index].flags = (i != 0 ? NV_TX_VALID : 0);
    }

    // Terminate descriptor chain
    g_txRingDescriptors[(descriptors_index + count - 1) % TX_RING_SIZE].flags |= NV_TX_LASTPACKET;

    // Enable first descriptor last to keep the NIC from sending incomplete packets
    g_txRingDescriptors[descriptors_index].flags |= NV_TX_VALID;

    // Keep track of how many descriptors are in use
    g_txRingDescriptorsInUseCount += count;

    // Inform that NIC that we have TX packet waiting
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_KICK;
}

void nvnetdrv_rx_release (void *buffer_virt)
{
    assert(buffer_virt != NULL);

    if (!g_running)
        return;

    nvnetdrv_rx_push(buffer_virt);
}
