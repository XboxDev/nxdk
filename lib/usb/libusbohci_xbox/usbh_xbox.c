#include <stdint.h>
#include <stdarg.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>
#include <hal/debug.h>
#include "usbh_config_xbox.h"

extern void OHCI_IRQHandler(void);
static KINTERRUPT InterruptObject;
static BOOLEAN __stdcall ISR(PKINTERRUPT Interrupt, PVOID ServiceContext)
{
    OHCI_IRQHandler();
    return TRUE;
}

//Initialise the systems ohci irq and irq handler.
//The USB stacks handler function (OHCI_IRQHandler) should be called
void usbh_ohci_irq_init() {
    KIRQL irql;
    ULONG vector = HalGetInterruptVector(USB_IRQ, &irql);
    KeInitializeInterrupt(&InterruptObject,
                          &ISR,
                          NULL,
                          vector,
                          irql,
                          LevelSensitive,
                          FALSE);
    KeConnectInterrupt(&InterruptObject);
}

void usbh_ohci_irq_deinit() {
    KeDisconnectInterrupt(&InterruptObject);
}

//Allocate a contiguous memory pool that the ohci hardware can access.
void *usbh_allocate_memory_pool(uint32_t size, uint32_t boundary) {
    return MmAllocateContiguousMemoryEx(size,
                                        0,
                                        64 * 1024 * 1024,
                                        boundary,
                                        PAGE_READWRITE | PAGE_NOCACHE);
}

void usbh_free_memory_pool(void *memory_pool) {
    MmFreeContiguousMemory(memory_pool);
}

//Return the system tick count in 10ms blocks
uint32_t usbh_get_ticks(void) {
    return GetTickCount() / 10;
}

void usbh_delay_us(int usec) {
    LARGE_INTEGER duration;
    duration.QuadPart = ((LONGLONG)usec) * -10;
    while (1)
    {
        NTSTATUS status = KeDelayExecutionThread(UserMode, FALSE, &duration);
        if (status != STATUS_ALERTED)
        {
            return;
        }
    }
}

//Convert a virtual address from the memory pool to a DMAable physical address.
void *usbh_dma_to_virt(void *physical_address) {
    if (physical_address == NULL)
        return NULL;

    //Already is a virtual address
    if (((uint32_t)physical_address & 0x80000000) != 0)
        return physical_address;

    return (void *)(0x80000000 | (uint32_t)physical_address);
}

//Convert a physical address from the ohci hardware to a virtual address.
void *usbh_virt_to_dma(void *virtual_address) {
    if ((uint32_t)virtual_address == 0x80000000)
        return NULL;

    //Already is a physical address
    if (((uint32_t)virtual_address & 0x80000000) == 0)
        return virtual_address;

    return (void *)MmGetPhysicalAddress(virtual_address);
}

void usbh_sysprintf(const char *buffer)
{
    debugPrint(buffer);
}