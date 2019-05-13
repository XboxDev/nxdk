#ifndef HAL_LED_H
#define HAL_LED_H

typedef enum _XLEDColor {
    XLED_OFF = 0x00,
    XLED_GREEN = 0x01,
    XLED_RED = 0x10,
    XLED_ORANGE = 0x11
} XLEDColor;

/*
  Restore system LED control.
*/
void XResetLED(void);

/*
  Set the LED color manually
  t1-t4 are used to set the color for each segment
  in a four step loop. For more details about front LED characteristics,
  please see https://xboxdevwiki.net/LED
*/
void XSetCustomLED(XLEDColor t1, XLEDColor t2, XLEDColor t3, XLEDColor t4);

#endif
