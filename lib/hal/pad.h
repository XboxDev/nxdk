#ifndef HAL_PAD_H
#define HAL_PAD_H

struct xpad_data
{
	unsigned char hPresent;
	int timestamp;
	short stick_left_x;
	short stick_left_y;
	short stick_right_x;
	short stick_right_y;
	short trig_left;
	short trig_right;
	char pad; /* 1 up 2 down 4 left 8 right */
	char state; /* 1 start 2 back 4 stick_left 8 stick_right */
	unsigned char keys[6]; /* A B X Y Black White */
	
};

#define XPAD_DPAD_UP           0x01
#define XPAD_DPAD_DOWN         0x02
#define XPAD_DPAD_LEFT         0x04
#define XPAD_DPAD_RIGHT        0x08
#define XPAD_START             0x10
#define XPAD_BACK              0x20
#define XPAD_LEFT_THUMB        0x40
#define XPAD_RIGHT_THUMB       0x80

#define XPAD_A                0
#define XPAD_B                1
#define XPAD_X                2
#define XPAD_Y                3
#define XPAD_BLACK            4
#define XPAD_WHITE            5
#define XPAD_LEFT_TRIGGER     6
#define XPAD_RIGHT_TRIGGER    7

typedef struct _XPAD_BUTTONS
{
	unsigned short	usDigitalButtons;
	unsigned char	ucAnalogButtons[8];
} XPAD_BUTTONS;

typedef struct _XPAD_INPUT
{
	unsigned char	hPresent;
	
	short			sLThumbX;
	short			sLThumbY;
	short			sRThumbX;
	short			sRThumbY;

	XPAD_BUTTONS	CurrentButtons;
	XPAD_BUTTONS	LastButtons;
	XPAD_BUTTONS	PressedButtons;

} XPAD_INPUT;

extern XPAD_INPUT g_Pads[4];
extern XPAD_INPUT g_DefaultPad;

#endif
