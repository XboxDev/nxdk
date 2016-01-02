#ifndef HAL_MOUSE_H
#define HAL_MOUSE_H

struct xmouse_data
{
	unsigned char	buttons;
	char			x;
	char			y;
	char			wheel;
};

#define XMOUSE_BUTTON_1		0x01
#define XMOUSE_BUTTON_2		0x02
#define XMOUSE_BUTTON_3		0x04
#define XMOUSE_BUTTON_4		0x08
#define XMOUSE_BUTTON_5		0x10


typedef struct _XMOUSE_INPUT
{
	unsigned char	ucButtons;
	char			cX;
	char			cY;
	char			cWheel;
} XMOUSE_INPUT;

extern XMOUSE_INPUT g_Mouse;

#endif
