#ifndef _USBLIB_H
#define _USBLIB_H

#include "cromwell_types.h"

void BootStopUSB(void);
void BootStartUSB(void);
void USBGetEvents(void);

void UsbKeyBoardInit(void);
void UsbKeyBoardRemove(void);

void wait_ms(u32 ticks);
void wait_us(u32 ticks);

#endif
