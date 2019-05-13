#include <hal/led.h>
#include <hal/xbox.h>

void main(void)
{
    // Set front LEDs to shine green, red and then turned off in a repeating pattern.
    XSetCustomLED(XLED_GREEN, XLED_RED, XLED_OFF, XLED_OFF);

    // XResetLED() can be used to restore system control of the front LEDs

    // If we quit, the LED might get reset, so we loop forever
    while(1) {
        XSleep(1000);
    }
}
