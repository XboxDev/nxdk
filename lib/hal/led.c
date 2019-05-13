/*
  Copyright and related rights waived by Lucas Eriksson via CC0
  https://creativecommons.org/publicdomain/zero/1.0/
*/

#include <hal/led.h>
#include <xboxkrnl/xboxkrnl.h>
#include <assert.h>

#define LED_MODE_AUTO   0x00
#define LED_MODE_MANUAL 0x01

#define SMC_REG_LEDMODE 0x07
#define SMC_REG_LEDSEQ  0x08

void XResetLED(void) {
    HalWriteSMBusValue(0x20, SMC_REG_LEDMODE, FALSE, LED_MODE_AUTO);
}

void XSetCustomLED(XLEDColor t1, XLEDColor t2, XLEDColor t3, XLEDColor t4) {
    HalWriteSMBusValue(0x20, SMC_REG_LEDMODE, FALSE, LED_MODE_MANUAL);

    ULONG colorScheme =
        ((t1 & 0x11) << 3) |
        ((t2 & 0x11) << 2) |
        ((t3 & 0x11) << 1) |
        ((t4 & 0x11) << 0);

    HalWriteSMBusValue(0x20, SMC_REG_LEDSEQ, FALSE, colorScheme);
}
