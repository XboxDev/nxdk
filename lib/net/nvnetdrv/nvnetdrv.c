// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2022 Ryan Wendland

#include "nvnetdrv.h"
#include "nvnetdrv_regs.h"
#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <xboxkrnl/xboxkrnl.h>

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
#define INC_STAT(statname, val)           \
    do {                                  \
        nvnetdrv_stats.statname += (val); \
    } while (0);
#else
#define INC_STAT(statname, val)
#endif

// FIXME
#define BASE          ((void *)0xFEF00000)
#define reg32(offset) (*((volatile uint32_t *)((uintptr_t)BASE + (offset))))

// Manage NIC
static atomic_bool g_running = false;
static uint8_t g_ethAddr[6];
static uint32_t g_linkSpeed;
static ULONG g_irq;
static KIRQL g_irql;
static KDPC g_dpcObj;
static KINTERRUPT g_interrupt;

// Manage RX and TX rings
static volatile struct descriptor_t *g_rxRing;
static volatile struct descriptor_t *g_txRing;
static size_t g_rxRingSize;
static size_t g_txRingSize;
static size_t g_rxRingHead;
static size_t g_txRingHead;
static atomic_size_t g_txPendingCount;
static atomic_size_t g_rxRingTail;
static atomic_size_t g_txRingTail;
static KSEMAPHORE g_txRingFreeCount;
struct tx_misc_t *g_txData;
static uint8_t *g_rxRingUserBuffers;
static uint32_t g_rxRingBufferVtoP;

// Manage RX buffers
static nvnetdrv_rx_callback_t g_rxCallback;

// Time constants used in nvnetdrv
#define NO_SLEEP      \
    &(LARGE_INTEGER)  \
    {                 \
        .QuadPart = 0 \
    }
#define TEN_MICRO        \
    &(LARGE_INTEGER)     \
    {                    \
        .QuadPart = -100 \
    }
#define FIFTY_MICRO      \
    &(LARGE_INTEGER)     \
    {                    \
        .QuadPart = -500 \
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

static void nvnetdrv_handle_irq (void);

static void NTAPI nvnetdrv_dpc (PKDPC Dpc, PVOID DeferredContext, PVOID arg1, PVOID arg2)
{
    nvnetdrv_handle_irq();
    nvnetdrv_irq_enable();
}

static inline uint32_t nvnetdrv_rx_ptov (uint32_t phys_address)
{
    return (phys_address == 0) ? 0 : (phys_address + g_rxRingBufferVtoP);
}

static inline uint32_t nvnetdrv_rx_vtop (uint32_t virt_address)
{
    return (virt_address == 0) ? 0 : (virt_address - g_rxRingBufferVtoP);
}

static void nvnetdrv_handle_rx_irq (void)
{
    while (g_rxRing[g_rxRingHead].paddr != NVNET_RX_EMPTY) {
        volatile struct descriptor_t *rx_packet = &g_rxRing[g_rxRingHead];
        uint16_t flags = rx_packet->flags;

        if (flags & NV_RX_AVAIL) {
            // Reached a descriptor that still belongs to the NIC
            break;
        }

        if ((flags & NV_RX_DESCRIPTORVALID) == 0) {
            goto release_packet;
        }

        if ((flags & NV_RX_ERROR_MASK) == 0) {
            uint16_t packet_length = rx_packet->length;

            if (flags & NV_RX_SUBTRACT1) {
                INC_STAT(rx_extraByteErrors, 1);
                if (packet_length > 0) {
                    packet_length--;
                }
            }

            if (flags & NV_RX_FRAMINGERR) {
                INC_STAT(rx_framingError, 1);
            }

            INC_STAT(rx_receivedPackets, 1);

            void *vaddr = (void *)nvnetdrv_rx_ptov(rx_packet->paddr);
            rx_packet->paddr = NVNET_RX_EMPTY;
            g_rxCallback(vaddr, packet_length);
            goto next_packet;
        } else {
            if (flags & NV_RX_MISSEDFRAME) {
                INC_STAT(rx_missedFrameError, 1);
            }
            if (flags & NV_RX_OVERFLOW) {
                INC_STAT(rx_overflowError, 1);
            }
            if (flags & NV_RX_CRCERR) {
                INC_STAT(rx_crcError, 1);
            }
            if (flags & NV_RX_ERROR4) {
                INC_STAT(rx_error4, 1);
            }
            if (flags & NV_RX_ERROR3) {
                INC_STAT(rx_error3, 1);
            }
            if (flags & NV_RX_ERROR2) {
                INC_STAT(rx_error2, 1);
            }
            if (flags & NV_RX_ERROR1) {
                INC_STAT(rx_error1, 1);
            }
            goto release_packet;
        }

    // On error drop packet and release buffer
    release_packet:
        nvnetdrv_rx_release((void *)nvnetdrv_rx_ptov(rx_packet->paddr));
        // Fallthrough
    next_packet:
        g_rxRingHead = (g_rxRingHead + 1) % g_rxRingSize;
    }
    INC_STAT(rx_interrupts, 1);
}

static void nvnetdrv_handle_tx_irq (void)
{
    LONG freed_descriptors = 0;

    while (g_txPendingCount > 0) {
        uint16_t flags = g_txRing[g_txRingHead].flags;

        if (flags & NV_TX_VALID) {
            // We reached a descriptor that wasn't processed by hw yet
            // Re-init the transfer to ensure the NIC sends it
            reg32(NvRegTxRxControl) = NVREG_TXRXCTL_KICK;
            break;
        }

        // Buffers get locked before sending and unlocked after sending
        MmLockUnlockBufferPages(g_txData[g_txRingHead].bufAddr, g_txData[g_txRingHead].length, TRUE);

        // If registered, call the users tx complete callback funciton.
        if (g_txData[g_txRingHead].callback) {
            g_txData[g_txRingHead].callback(g_txData[g_txRingHead].userdata);
            g_txData[g_txRingHead].callback = NULL;
        }

        freed_descriptors++;
        g_txRingHead = (g_txRingHead + 1) % g_txRingSize;
        g_txPendingCount--;
    }

    KeReleaseSemaphore(&g_txRingFreeCount, IO_NETWORK_INCREMENT, freed_descriptors, FALSE);
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
    while (true) {
        uint32_t irq = reg32(NvRegIrqStatus);
        uint32_t mii = reg32(NvRegMIIStatus);

        // No interrupts left to handle. Leave
        if (!irq && !mii) {
            break;
        }

        // We need to handle MII irq before acknowledging it to prevent link state IRQ occurring
        // during polling of the link state register
        if (irq & NVREG_IRQ_LINK) {
            nvnetdrv_handle_mii_irq(mii, false);
        }

        // Acknowledge interrupts
        reg32(NvRegMIIStatus) = mii;
        reg32(NvRegIrqStatus) = irq;

        // Handle TX/RX interrupts
        if (irq & NVREG_IRQ_RX_ALL) {
            nvnetdrv_handle_rx_irq();
        }
        if (irq & NVREG_IRQ_TX_ALL) {
            nvnetdrv_handle_tx_irq();
        }
        if (irq & NVREG_IRQ_RX_NOBUF) {
            reg32(NvRegTxRxControl) = NVREG_TXRXCTL_GET;
        }
    }
}


const uint8_t *nvnetdrv_get_ethernet_addr (void)
{
    return g_ethAddr;
}

int nvnetdrv_init (size_t rx_buffer_count, nvnetdrv_rx_callback_t rx_callback, size_t tx_queue_size)
{
    assert(!g_running);
    assert(rx_callback);
    assert(rx_buffer_count > 1);

    g_rxCallback = rx_callback;
    g_rxRingSize = rx_buffer_count;
    g_txRingSize = tx_queue_size;

    // Get Mac Address from EEPROM
    ULONG type;
    NTSTATUS status = ExQueryNonVolatileSetting(XC_FACTORY_ETHERNET_ADDR, &type, &g_ethAddr, 6, NULL);
    if (!NT_SUCCESS(status)) {
        return NVNET_NO_MAC;
    }

    // Allocate memory for TX and RX ring descriptors.
    void *descriptors = MmAllocateContiguousMemoryEx((g_rxRingSize + g_txRingSize) * sizeof(struct descriptor_t), 0,
                                                     0xFFFFFFFF, 0, PAGE_READWRITE);
    if (!descriptors) {
        return NVNET_NO_MEM;
    }

    // Allocate memory to store data associated with outgoing transfers
    g_txData = malloc(g_txRingSize * sizeof(struct tx_misc_t));
    if (!g_txData) {
        MmFreeContiguousMemory(descriptors);
        return NVNET_NO_MEM;
    }

    // Allocate memory for RX buffers. TX buffers are supplied by the user.
    g_rxRingUserBuffers =
        MmAllocateContiguousMemoryEx(g_rxRingSize * NVNET_RX_BUFF_LEN, 0, 0xFFFFFFFF, 0, PAGE_READWRITE);
    if (!g_rxRingUserBuffers) {
        MmFreeContiguousMemory(descriptors);
        free(g_txData);
        return NVNET_NO_MEM;
    }

    RtlZeroMemory(descriptors, (g_rxRingSize + g_txRingSize) * sizeof(struct descriptor_t));

    // Reset NIC. MSDash delays 10us here
    nvnetdrv_stop_txrx();
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_RESET;
    KeDelayExecutionThread(KernelMode, FALSE, TEN_MICRO);
    reg32(NvRegTxRxControl) = 0;
    KeDelayExecutionThread(KernelMode, FALSE, TEN_MICRO);
    reg32(NvRegMIIMask) = 0;
    reg32(NvRegIrqMask) = 0;
    reg32(NvRegWakeUpFlags) = 0;
    reg32(NvRegPollingControl) = 0;
    reg32(NvRegTxRingPhysAddr) = 0;
    reg32(NvRegRxRingPhysAddr) = 0;
    reg32(NvRegTransmitPoll) = 0;
    reg32(NvRegLinkSpeed) = 0;

    // Acknowledge any existing interrupts status bits
    reg32(NvRegTransmitterStatus) = reg32(NvRegTransmitterStatus);
    reg32(NvRegReceiverStatus) = reg32(NvRegReceiverStatus);
    reg32(NvRegIrqStatus) = reg32(NvRegIrqStatus);
    reg32(NvRegMIIStatus) = reg32(NvRegMIIStatus);

    // Reset local ring tracking variables
    g_rxRingHead = 0;
    g_rxRingTail = 0;
    g_txRingHead = 0;
    g_txRingTail = 0;
    g_txPendingCount = 0;
    RtlZeroMemory(g_txData, g_txRingSize * sizeof(struct tx_misc_t));

    // Setup the TX and RX ring descriptor pointers
    g_rxRing = (volatile struct descriptor_t *)descriptors;
    g_txRing = (volatile struct descriptor_t *)descriptors + g_rxRingSize;

    // Remember the offset between virtual and physical address
    g_rxRingBufferVtoP = ((uint32_t)g_rxRingUserBuffers) - (uint32_t)MmGetPhysicalAddress(g_rxRingUserBuffers);

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

    // Pseudo random slot time to minimise collisions
    reg32(NvRegSlotTime) = ((rand() % 0xFF) & NVREG_SLOTTIME_MASK) | NVREG_SLOTTIME_10_100_FULL;
    reg32(NvRegTxDeferral) = NVREG_TX_DEFERRAL_RGMII_10_100;
    reg32(NvRegRxDeferral) = NVREG_RX_DEFERRAL_DEFAULT;

    // MS Dash does this and sets up both these registers with 0x300010)
    reg32(NvRegUnknownSetupReg7) = NVREG_UNKSETUP7_VAL1; // RxWatermark?
    reg32(NvRegTxWatermark) = NVREG_UNKSETUP7_VAL1;

    // Point the NIC to our TX and RX ring buffers. NIC expects Ring size as size-1.
    reg32(NvRegTxRingPhysAddr) = MmGetPhysicalAddress((void *)g_txRing);
    reg32(NvRegRxRingPhysAddr) = MmGetPhysicalAddress((void *)g_rxRing);
    reg32(NvRegRingSizes) = ((g_rxRingSize - 1) << NVREG_RINGSZ_RXSHIFT) | ((g_txRingSize - 1) << NVREG_RINGSZ_TXSHIFT);

    // Prepare for Phy Init
    reg32(NvRegAdapterControl) = (1 << NVREG_ADAPTCTL_PHYSHIFT) | NVREG_ADAPTCTL_PHYVALID;
    reg32(NvRegMIISpeed) = NVREG_MIISPEED_BIT8 | NVREG_MIIDELAY;
    reg32(NvRegMIIMask) = NVREG_MII_LINKCHANGE;
    KeDelayExecutionThread(KernelMode, FALSE, FIFTY_MICRO);

    // Initialise the transceiver
    if (PhyInitialize(FALSE, NULL) != STATUS_SUCCESS) {
        MmFreeContiguousMemory(descriptors);
        MmFreeContiguousMemory(g_rxRingUserBuffers);
        return NVNET_PHY_ERR;
    }

    // Short delay to allow the phy to startup. MSDash delays 50us
    reg32(NvRegAdapterControl) |= NVREG_ADAPTCTL_RUNNING;
    KeDelayExecutionThread(KernelMode, FALSE, FIFTY_MICRO);

    // The NIC hardware IRQ queues a DPC. The DPC then sets g_irqEvent.
    // g_irqEvent is monitored by irqthread to handle the IRQ
    g_irq = HalGetInterruptVector(4, &g_irql);
    KeInitializeInterrupt(&g_interrupt, &nvnetdrv_isr, NULL, g_irq, g_irql, LevelSensitive, TRUE);
    KeInitializeDpc(&g_dpcObj, nvnetdrv_dpc, NULL);

    // We use semaphores to track the number of free TX ring descriptors.
    KeInitializeSemaphore(&g_txRingFreeCount, g_txRingSize, g_txRingSize);

    // Get link speed settings from Phy
    nvnetdrv_handle_mii_irq(0, true);

    // Set running flag. This needs to happen before we connect the irq and start threads.
    bool prev_value = atomic_exchange(&g_running, true);
    assert(!prev_value);

    // Fill up our RX ring with buffers
    for (uint32_t i = 0; i < g_rxRingSize; i++) {
        nvnetdrv_rx_release(g_rxRingUserBuffers + (i * NVNET_RX_BUFF_LEN));
    }

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

void nvnetdrv_stop (void)
{
    assert(g_running);

    KeDisconnectInterrupt(&g_interrupt);

    // Stop NIC processing rings
    nvnetdrv_stop_txrx();

    // Clear the nvnet running flag so threads know to end
    bool prev_value = atomic_exchange(&g_running, false);
    assert(prev_value);

    // Pass back all TX buffers to user.
    for (size_t i = g_txRingTail; i != g_txRingHead; i = (i + 1) % g_txRingSize) {
        if (g_txData[i].callback) {
            g_txData[i].callback(g_txData[i].userdata);
        }
    }

    // Free all TX descriptors g_txRingFreeCount so nvnetdrv_acquire_tx_descriptors will return.
    KeReleaseSemaphore(&g_txRingFreeCount, IO_NETWORK_INCREMENT, g_txPendingCount, FALSE);

    // Reset TX & RX control
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_DISABLE | NVREG_TXRXCTL_RESET;
    KeDelayExecutionThread(KernelMode, FALSE, TEN_MICRO);
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_DISABLE;

    // Free all memory allocated by nvnetdrv
    MmFreeContiguousMemory((void *)g_rxRing);
    MmFreeContiguousMemory((void *)g_rxRingUserBuffers);
    free(g_txData);
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

    // Wait for active TX and RX descriptors to finish
    for (int i = 0; i < 50000; i++) {
        if (!((reg32(NvRegReceiverStatus) & NVREG_RCVSTAT_BUSY) ||
              (reg32(NvRegTransmitterStatus) & NVREG_XMITSTAT_BUSY))) {
            break;
        }
        KeDelayExecutionThread(KernelMode, FALSE, TEN_MICRO);
    }

    // Disable DMA and wait for it to idle, re-checking every 50 microseconds
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_DISABLE;
    for (int i = 0; i < 10000; i++) {
        if (reg32(NvRegTxRxControl) & NVREG_TXRXCTL_IDLE) {
            break;
        }
        KeDelayExecutionThread(KernelMode, FALSE, FIFTY_MICRO);
    }

    reg32(NvRegTxRxControl) = 0;
}

int nvnetdrv_acquire_tx_descriptors (size_t count)
{
    NTSTATUS status;

    // Sanity check
    assert(count > 0);
    // Avoid excessive requests
    assert(count <= 4);

    if (!g_running) {
        return false;
    }

    while (true) {
        // Wait for TX descriptors to become available
        KeWaitForSingleObject(&g_txRingFreeCount, Executive, KernelMode, FALSE, NULL);

        if (!g_running) {
            return false;
        }

        // We want to try claim all tx descriptors at once without sleeping.
        size_t i = 0;
        for (i = 0; i < count - 1; i++) {
            // Try to acquire remaining descriptors without sleeping
            status = KeWaitForSingleObject(&g_txRingFreeCount, Executive, KernelMode, FALSE, NO_SLEEP);
            if (!NT_SUCCESS(status) || status == STATUS_TIMEOUT) {
                // Couldn't acquire all at once, back off
                KeReleaseSemaphore(&g_txRingFreeCount, IO_NETWORK_INCREMENT, i + 1, NULL);
                if (status == STATUS_TIMEOUT) {
                    // Sleep for 10 microseconds to avoid immediate re-locking
                    KeDelayExecutionThread(UserMode, FALSE, TEN_MICRO);
                    // Retry
                    break;
                } else {
                    return false;
                }
            }
        }

        if (!g_running) {
            return false;
        }

        // If we have claimed all the tx descriptors. We are done.
        if (i == (count - 1)) {
            break;
        }
    }
    return true;
}

void nvnetdrv_submit_tx_descriptors (nvnetdrv_descriptor_t *buffers, size_t count)
{
    // Sanity check
    assert(count > 0);
    // Avoid excessive requests
    assert(count <= 4);

    if (!g_running) {
        return;
    }

    // Check that no buffer crosses a page boundary
    for (size_t i = 0; i < count; i++) {
        // For 4KiB pages, the least significant 12 bits are intra-page offsets, so shift them away and compare the rest
        assert(((uint32_t)buffers[i].addr >> 12) == (((uint32_t)buffers[i].addr + buffers[i].length - 1) >> 12));
    }

    // We don't check for buffer overrun here, because the Semaphore already protects us
    size_t descriptors_index = g_txRingTail;
    while (
        !atomic_compare_exchange_weak(&g_txRingTail, &descriptors_index, (descriptors_index + count) % g_txRingSize)) {
        descriptors_index = g_txRingTail;
    }

    for (size_t i = 0; i < count; i++) {
        size_t current_descriptor_index = (descriptors_index + i) % g_txRingSize;

        g_txData[current_descriptor_index].bufAddr = buffers[i].addr;
        g_txData[current_descriptor_index].length = buffers[i].length;
        g_txData[current_descriptor_index].userdata = buffers[i].userdata;
        g_txData[current_descriptor_index].callback = buffers[i].callback;

        // Buffers get locked before sending and unlocked after sending
        MmLockUnlockBufferPages(buffers[i].addr, buffers[i].length, FALSE);
        g_txRing[current_descriptor_index].paddr = MmGetPhysicalAddress(buffers[i].addr);
        g_txRing[current_descriptor_index].length = buffers[i].length - 1;
        g_txRing[current_descriptor_index].flags = (i != 0 ? NV_TX_VALID : 0);
    }

    // Terminate descriptor chain
    g_txRing[(descriptors_index + count - 1) % g_txRingSize].flags |= NV_TX_LASTPACKET;

    // Keep track of how many descriptors are in use
    g_txPendingCount += count;

    // Enable first descriptor last to keep the NIC from sending incomplete packets
    g_txRing[descriptors_index].flags |= NV_TX_VALID;

    // Inform that NIC that we have TX packet waiting
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_KICK;
}

void nvnetdrv_rx_release (void *buffer_virt)
{
    assert(buffer_virt != NULL);

    if (!g_running) {
        return;
    }

    size_t index = g_rxRingTail;
    while (!atomic_compare_exchange_weak(&g_rxRingTail, &index, (index + 1) % g_rxRingSize)) {
        index = g_rxRingTail;
    }
    g_rxRing[index].flags = 0;
    g_rxRing[index].paddr = nvnetdrv_rx_vtop((uint32_t)buffer_virt);
    g_rxRing[index].length = NVNET_RX_BUFF_LEN;
    g_rxRing[index].flags = NV_RX_AVAIL;
    reg32(NvRegTxRxControl) = NVREG_TXRXCTL_GET;
}
