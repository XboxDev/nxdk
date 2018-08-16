#include <hal/xbox.h>
#include <xboxkrnl/xboxkrnl.h>

#define SMC_REG_LEDMODE             0x07
#define SMC_REG_LEDSEQ              0x08

void main(void)
{
    // Set LED to be user controlled
    HalWriteSMBusValue(0x20, SMC_REG_LEDMODE, FALSE, 0x01);

    // LED pattern is:
    //   Red:   0xC = 1100 (ON,  ON,  OFF, OFF;   ON,  ON,  OFF, OFF;   ...)
    //   Green: 0x0 = 0000 (OFF, OFF, OFF, OFF;   OFF, OFF, OFF, OFF;   ...)
    // This means the red LED will blink slowly and the green LED is off
    HalWriteSMBusValue(0x20, SMC_REG_LEDSEQ, FALSE, 0xC0);

    // If we quit, the LED might get reset, so we loop forever
    while(1) {
        XSleep(1000);
    }
}
