#ifndef HAL_INPUT_H
#define HAL_INPUT_H

#include "pad.h"
#include "mouse.h"
#include "keyboard.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/* General input functions */
__attribute__((deprecated))
void XInput_Init(void);
void XInput_Init_Polling(void);
void XInput_Quit(void);
void XInput_GetEvents(void);

/* Pad specific functions */
int XInputGetPadCount(void);

/* Keyboard specific functions */
int XInputGetKeystroke(XKEYBOARD_STROKE *pStroke);

/* Mouse specific functions */
XMOUSE_INPUT XInputGetMouseData(void);

#ifdef __cplusplus
}
#endif

#endif
