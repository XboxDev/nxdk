#ifndef _KEYBOARD_H
#define _KEYBOARD_H

typedef struct _XKEYBOARD_STROKE
{
	unsigned char	ucFlags;
	unsigned char	ucKeyCode;
	unsigned char	ucAsciiValue;
} XKEYBOARD_STROKE;

// Modifier key flag values
#define XKEYBOARD_LCONTROL_FLAG			0x01
#define XKEYBOARD_LSHIFT_FLAG			0x02
#define XKEYBOARD_LALT_FLAG				0x04
#define XKEYBOARD_LWINDOWS_FLAG			0x08
#define XKEYBOARD_RCONTROL_FLAG			0x10
#define XKEYBOARD_RSHIFT_FLAG			0x20
#define XKEYBOARD_RALT_FLAG				0x40
#define XKEYBOARD_RWINDOWS_FLAG			0x80

// usb keyboard usage values for key modifiers
#define USB_KEYBOARD_USAGE_LCONTROL		0xE0
#define USB_KEYBOARD_USAGE_LSHIFT		0xE1
#define USB_KEYBOARD_USAGE_LALT			0xE2
#define USB_KEYBOARD_USAGE_LWINDOWS		0xE3
#define USB_KEYBOARD_USAGE_RCONTROL		0xE4
#define USB_KEYBOARD_USAGE_RSHIFT		0xE5
#define USB_KEYBOARD_USAGE_RALT			0xE6
#define USB_KEYBOARD_USAGE_RWINDOWS		0xE7

// keyboard state flags - keyup, shift, ctrl, alt etc
#define XKEYBOARD_KEYUP					0x01
#define XKEYBOARD_SHIFT					0x02
#define XKEYBOARD_CONTROL				0x04
#define XKEYBOARD_ALT					0x08

// key code constants
#define XKEY_INVALID		0x00

#define XKEY_LBUTTON        0x01
#define XKEY_RBUTTON        0x02
#define XKEY_CANCEL         0x03
#define XKEY_MBUTTON        0x04
#define XKEY_XBUTTON1       0x05
#define XKEY_XBUTTON2       0x06

#define XKEY_BACKSPACE      0x08
#define XKEY_TAB            0x09


#define XKEY_CLEAR          0x0C
#define XKEY_RETURN         0x0D


#define XKEY_SHIFT          0x10
#define XKEY_CONTROL        0x11
#define XKEY_MENU           0x12
#define XKEY_PAUSE          0x13
#define XKEY_CAPSLOCK       0x14
#define XKEY_KANA           0x15
#define XKEY_JUNJA          0x17
#define XKEY_FINAL          0x18
#define XKEY_KANJI          0x19

#define XKEY_ESCAPE         0x1B
#define XKEY_CONVERT        0x1C
#define XKEY_NONCONVERT     0x1D
#define XKEY_ACCEPT         0x1E
#define XKEY_MODECHANGE     0x1F
#define XKEY_SPACE          0x20
#define XKEY_PRIOR          0x21
#define XKEY_NEXT           0x22
#define XKEY_END            0x23
#define XKEY_HOME           0x24
#define XKEY_LEFT           0x25
#define XKEY_UP             0x26
#define XKEY_RIGHT          0x27
#define XKEY_DOWN           0x28
#define XKEY_SELECT         0x29
#define XKEY_PRINT          0x2A
#define XKEY_EXECUTE        0x2B
#define XKEY_PRINTSCREEN    0x2C
#define XKEY_INSERT         0x2D
#define XKEY_DELETE         0x2E
#define XKEY_HELP           0x2F
#define XKEY_0				0x30
#define XKEY_1				0x31
#define XKEY_2				0x32
#define XKEY_3				0x33
#define XKEY_4				0x34
#define XKEY_5				0x35
#define XKEY_6				0x36
#define XKEY_7				0x37
#define XKEY_8				0x38
#define XKEY_9				0x39

#define XKEY_A				0x41
#define XKEY_B				0x42
#define XKEY_C				0x43
#define XKEY_D				0x44
#define XKEY_E				0x45
#define XKEY_F				0x46
#define XKEY_G				0x47
#define XKEY_H				0x48
#define XKEY_I				0x49
#define XKEY_J				0x4A
#define XKEY_K				0x4B
#define XKEY_L				0x4C
#define XKEY_M				0x4D
#define XKEY_N				0x4E
#define XKEY_O				0x4F
#define XKEY_P				0x50
#define XKEY_Q				0x51
#define XKEY_R				0x52
#define XKEY_S				0x53
#define XKEY_T				0x54
#define XKEY_U				0x55
#define XKEY_V				0x56
#define XKEY_W				0x57
#define XKEY_X				0x58
#define XKEY_Y				0x59
#define XKEY_Z				0x5A
#define XKEY_LWIN           0x5B
#define XKEY_RWIN           0x5C
#define XKEY_APPS           0x5D

#define XKEY_SLEEP          0x5F
#define XKEY_NUMPAD0        0x60
#define XKEY_NUMPAD1        0x61
#define XKEY_NUMPAD2        0x62
#define XKEY_NUMPAD3        0x63
#define XKEY_NUMPAD4        0x64
#define XKEY_NUMPAD5        0x65
#define XKEY_NUMPAD6        0x66
#define XKEY_NUMPAD7        0x67
#define XKEY_NUMPAD8        0x68
#define XKEY_NUMPAD9        0x69
#define XKEY_MULTIPLY       0x6A
#define XKEY_ADD            0x6B
#define XKEY_NUMPAD_ENTER   0x6C
#define XKEY_SUBTRACT       0x6D
#define XKEY_DECIMAL        0x6E
#define XKEY_DIVIDE         0x6F
#define XKEY_F1             0x70
#define XKEY_F2             0x71
#define XKEY_F3             0x72
#define XKEY_F4             0x73
#define XKEY_F5             0x74
#define XKEY_F6             0x75
#define XKEY_F7             0x76
#define XKEY_F8             0x77
#define XKEY_F9             0x78
#define XKEY_F10            0x79
#define XKEY_F11            0x7A
#define XKEY_F12            0x7B
#define XKEY_F13            0x7C
#define XKEY_F14            0x7D
#define XKEY_F15            0x7E
#define XKEY_F16            0x7F
#define XKEY_F17            0x80
#define XKEY_F18            0x81
#define XKEY_F19            0x82
#define XKEY_F20            0x83
#define XKEY_F21            0x84
#define XKEY_F22            0x85
#define XKEY_F23            0x86
#define XKEY_F24            0x87








#define XKEY_NUMLOCK        0x90
#define XKEY_SCROLL_LOCK    0x91
#define XKEY_NUMPADEQUALS	0x92

/*
 * Fujitsu/OASYS kbd definitions
 */
#define XKEY_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define XKEY_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define XKEY_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define XKEY_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define XKEY_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key








#define XKEY_LSHIFT					0xA0
#define XKEY_RSHIFT					0xA1
#define XKEY_LCONTROL				0xA2
#define XKEY_RCONTROL				0xA3
#define XKEY_LMENU					0xA4
#define XKEY_RMENU					0xA5
#define XKEY_BROWSER_BACK			0xA6
#define XKEY_BROWSER_FORWARD		0xA7
#define XKEY_BROWSER_REFRESH		0xA8
#define XKEY_BROWSER_STOP			0xA9
#define XKEY_BROWSER_SEARCH			0xAA
#define XKEY_BROWSER_FAVORITES		0xAB
#define XKEY_BROWSER_HOME			0xAC
#define XKEY_VOLUME_MUTE			0xAD
#define XKEY_VOLUME_DOWN			0xAE
#define XKEY_VOLUME_UP				0xAF
#define XKEY_MEDIA_NEXT_TRACK		0xB0
#define XKEY_MEDIA_PREV_TRACK		0xB1
#define XKEY_MEDIA_STOP				0xB2
#define XKEY_MEDIA_PLAY_PAUSE		0xB3
#define XKEY_LAUNCH_MAIL			0xB4
#define XKEY_LAUNCH_MEDIA_SELECT	0xB5
#define XKEY_LAUNCH_APP1			0xB6
#define XKEY_LAUNCH_APP2			0xB7


#define XKEY_SEMICOLON				0xBA
#define XKEY_EQUALS					0xBB
#define XKEY_COMMA				    0xBC
#define XKEY_MINUS					0xBD
#define XKEY_PERIOD					0xBE
#define XKEY_SLASH					0xBF
#define XKEY_GRAVE					0xC0






#define XKEY_LBRACE					0xDB
#define XKEY_BACKSLASH				0xDC
#define XKEY_RBRACE					0xDD
#define XKEY_QUOTE					0xDE
#define XKEY_MISC					0xDF

#define XKEY_OEM_AX					0xE1
#define XKEY_OEM_102				0xE2
#define XKEY_ICO_HELP				0xE3
#define XKEY_ICO_00					0xE4
#define XKEY_PROCESSKEY				0xE5
#define XKEY_ICO_CLEAR				0xE6
#define XKEY_PACKET					0xE7

#define XKEY_OEM_RESET				0xE9
#define XKEY_OEM_JUMP				0xEA
#define XKEY_OEM_PA1				0xEB
#define XKEY_OEM_PA2				0xEC
#define XKEY_OEM_PA3				0xED
#define XKEY_OEM_WSCTRL				0xEE
#define XKEY_OEM_CUSEL				0xEF
#define XKEY_OEM_ATTN				0xF0
#define XKEY_OEM_FINISH				0xF1
#define XKEY_OEM_COPY				0xF2
#define XKEY_OEM_AUTO				0xF3
#define XKEY_OEM_ENLW				0xF4
#define XKEY_OEM_BACKTAB			0xF5

#define XKEY_ATTN					0xF6
#define XKEY_CRSEL					0xF7
#define XKEY_EXSEL					0xF8
#define XKEY_EREOF					0xF9
#define XKEY_PLAY					0xFA
#define XKEY_ZOOM					0xFB
#define XKEY_NONAME					0xFC
#define XKEY_PA1					0xFD
#define XKEY_OEM_CLEAR				0xFE


#endif
