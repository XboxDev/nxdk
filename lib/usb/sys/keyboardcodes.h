#ifndef _KEYBOARD_CODES
#define _KEYBOARD_CODES

static unsigned char ucUSBtoKey[256] = 
{
XKEY_INVALID,
XKEY_INVALID,
XKEY_INVALID,
XKEY_INVALID,
XKEY_A,
XKEY_B,
XKEY_C,
XKEY_D,
XKEY_E,
XKEY_F,
XKEY_G,
XKEY_H,
XKEY_I,
XKEY_J,
XKEY_K,
XKEY_L,
XKEY_M,
XKEY_N,
XKEY_O,
XKEY_P,
XKEY_Q,
XKEY_R,
XKEY_S,
XKEY_T,
XKEY_U,
XKEY_V,
XKEY_W,
XKEY_X,
XKEY_Y,
XKEY_Z,
XKEY_1,
XKEY_2,
XKEY_3,
XKEY_4,
XKEY_5,
XKEY_6,
XKEY_7,
XKEY_8,
XKEY_9,
XKEY_0,
XKEY_RETURN,
XKEY_ESCAPE,
XKEY_BACKSPACE,
XKEY_TAB,
XKEY_SPACE,
XKEY_MINUS,
XKEY_EQUALS,
XKEY_LBRACE,
XKEY_RBRACE,
XKEY_BACKSLASH,
XKEY_GRAVE,
XKEY_SEMICOLON,
XKEY_QUOTE,
XKEY_GRAVE,
XKEY_COMMA,
XKEY_PERIOD,
XKEY_SLASH,
XKEY_CAPSLOCK,
XKEY_F1,
XKEY_F2,
XKEY_F3,
XKEY_F4,
XKEY_F5,
XKEY_F6,
XKEY_F7,
XKEY_F8,
XKEY_F9,
XKEY_F10,
XKEY_F11,
XKEY_F12,
XKEY_PRINTSCREEN,
XKEY_SCROLL_LOCK,
XKEY_PAUSE,
XKEY_INSERT,
XKEY_HOME,
XKEY_PRIOR,
XKEY_DELETE,
XKEY_END,
XKEY_NEXT,
XKEY_RIGHT,
XKEY_LEFT,
XKEY_DOWN,
XKEY_UP,
XKEY_NUMLOCK,
XKEY_DIVIDE,
XKEY_MULTIPLY,
XKEY_SUBTRACT,
XKEY_ADD,
XKEY_NUMPAD_ENTER,
XKEY_NUMPAD1,
XKEY_NUMPAD2,
XKEY_NUMPAD3,
XKEY_NUMPAD4,
XKEY_NUMPAD5,
XKEY_NUMPAD6,
XKEY_NUMPAD7,
XKEY_NUMPAD8,
XKEY_NUMPAD9,
XKEY_NUMPAD0,
XKEY_DECIMAL,
XKEY_BACKSLASH,
XKEY_APPS,
XKEY_SLEEP,
XKEY_NUMPADEQUALS,
XKEY_F13,
XKEY_F14,
XKEY_F15,
XKEY_F16,
XKEY_F17,
XKEY_F18,
XKEY_F19,
XKEY_F20,
XKEY_F21,
XKEY_F22,
XKEY_F23,
XKEY_F24,
XKEY_EXECUTE,
XKEY_HELP,
XKEY_MENU,
XKEY_SELECT,
XKEY_BROWSER_STOP,
XKEY_INVALID, // Keyboard Again
XKEY_INVALID, // Keyboard Undo
XKEY_INVALID, // Keyboard Cut
XKEY_INVALID, // Keyboard Copy
XKEY_INVALID, // Keyboard Paste
XKEY_BROWSER_SEARCH,
XKEY_VOLUME_MUTE,
XKEY_VOLUME_UP,
XKEY_VOLUME_DOWN,
XKEY_INVALID, // Keyboard locking caps lock
XKEY_INVALID, // Keyboard locking num lock
XKEY_INVALID, // Keyboard locking scroll lock
XKEY_DECIMAL,
XKEY_NUMPADEQUALS,
XKEY_INVALID, // Keyboard International 1
XKEY_INVALID, // Keyboard International 2
XKEY_INVALID, // Keyboard International 3
XKEY_INVALID, // Keyboard International 4
XKEY_INVALID, // Keyboard International 5
XKEY_INVALID, // Keyboard International 6
XKEY_INVALID, // Keyboard International 7
XKEY_INVALID, // Keyboard International 8
XKEY_INVALID, // Keyboard International 9
XKEY_INVALID, // Keyboard Lang 1
XKEY_KANJI,
XKEY_KANA,
XKEY_INVALID, // Keyboard Lang 4
XKEY_INVALID, // Keyboard Lang 5
XKEY_INVALID, // Keyboard Lang 6
XKEY_INVALID, // Keyboard Lang 7
XKEY_INVALID, // Keyboard Lang 8
XKEY_INVALID, // Keyboard Lang 9
XKEY_INVALID, // Keyboard Alternate Erase
XKEY_INVALID, // Keyboard SysReq/Attention
XKEY_INVALID, // Keyboard Cancel
XKEY_INVALID, // Keyboard Clear
XKEY_INVALID, // Keyboard Prior
XKEY_INVALID, // Keyboard Return
XKEY_INVALID, // Keyboard Separator
XKEY_INVALID, // Keyboard Out
XKEY_INVALID, // Keyboard Oper
XKEY_INVALID, // Keyboard Clear/Again
XKEY_INVALID, // Keyboard CrSel/Props
XKEY_INVALID, // Keyboard ExSel
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Keypad 00
XKEY_INVALID, // Keypad 000
XKEY_INVALID, // Thousands Separator
XKEY_INVALID, // Decimal Separator
XKEY_INVALID, // Currency Unit
XKEY_INVALID, // Currency Sub-unit
XKEY_INVALID, // Keypad (
XKEY_INVALID, // Keypad )
XKEY_INVALID, // Keypad {
XKEY_INVALID, // Keypad }
XKEY_INVALID, // Keypad Tab
XKEY_INVALID, // Keypad Backspace
XKEY_INVALID, // Keypad A
XKEY_INVALID, // Keypad B
XKEY_INVALID, // Keypad C
XKEY_INVALID, // Keypad D
XKEY_INVALID, // Keypad E
XKEY_INVALID, // Keypad F
XKEY_INVALID, // Keypad XOR
XKEY_INVALID, // Keypad ^
XKEY_INVALID, // Keypad %
XKEY_INVALID, // Keypad <
XKEY_INVALID, // Keypad >
XKEY_INVALID, // Keypad &
XKEY_INVALID, // Keypad &&
XKEY_INVALID, // Keypad |
XKEY_INVALID, // Keypad ||
XKEY_INVALID, // Keypad :
XKEY_INVALID, // Keypad #
XKEY_INVALID, // Keypad Space
XKEY_INVALID, // Keypad @
XKEY_INVALID, // Keypad !
XKEY_INVALID, // Keypad Memory Store
XKEY_INVALID, // Keypad Memory Recall
XKEY_INVALID, // Keypad Memory Clear
XKEY_INVALID, // Keypad Memory Add
XKEY_INVALID, // Keypad Memory Subtract
XKEY_INVALID, // Keypad Memory Multiply
XKEY_INVALID, // Keypad Memory Divide
XKEY_INVALID, // Keypad +/-
XKEY_INVALID, // Keypad Clear
XKEY_INVALID, // Keypad Clear Entry
XKEY_INVALID, // Keypad Binary
XKEY_INVALID, // Keypad Octal
XKEY_INVALID, // Keypad Decimal
XKEY_INVALID, // Keypad Hexadecimal
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_LCONTROL,
XKEY_LSHIFT,
XKEY_LMENU,
XKEY_LWIN,
XKEY_RCONTROL,
XKEY_RSHIFT,
XKEY_RMENU,
XKEY_RWIN,
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
XKEY_INVALID, // Reserved
};

static unsigned char ucUSBtoAscii[256] = 
{
0, //XKEY_INVALID,
0, //XKEY_INVALID,
0, //XKEY_INVALID,
0, //XKEY_INVALID,
'a', //XKEY_A,
'b', //XKEY_B,
'c', //XKEY_C,
'd', //XKEY_D,
'e', //XKEY_E,
'f', //XKEY_F,
'g', //XKEY_G,
'h', //XKEY_H,
'i', //XKEY_I,
'j', //XKEY_J,
'k', //XKEY_K,
'l', //XKEY_L,
'm', //XKEY_M,
'n', //XKEY_N,
'o', //XKEY_O,
'p', //XKEY_P,
'q', //XKEY_Q,
'r', //XKEY_R,
's', //XKEY_S,
't', //XKEY_T,
'u', //XKEY_U,
'v', //XKEY_V,
'w', //XKEY_W,
'x', //XKEY_X,
'y', //XKEY_Y,
'z', //XKEY_Z,
'1', //XKEY_1,
'2', //XKEY_2,
'3', //XKEY_3,
'4', //XKEY_4,
'5', //XKEY_5,
'6', //XKEY_6,
'7', //XKEY_7,
'8', //XKEY_8,
'9', //XKEY_9,
'0', //XKEY_0,
'\n', //XKEY_RETURN,
27, //XKEY_ESCAPE,
8, //XKEY_BACKSPACE,
9, //XKEY_TAB,
' ', //XKEY_SPACE,
'-', //XKEY_MINUS,
'=', //XKEY_EQUALS,
'[', //XKEY_LBRACE,
']', //XKEY_RBRACE,
'\\', //XKEY_BACKSLASH,
'`', //XKEY_GRAVE,
';', //XKEY_SEMICOLON,
'\'', //XKEY_QUOTE,
'`', //XKEY_GRAVE,
',', //XKEY_COMMA,
'.', //XKEY_PERIOD,
'/', //XKEY_SLASH,
0, //XKEY_CAPSLOCK,
0, //XKEY_F1,
0, //XKEY_F2,
0, //XKEY_F3,
0, //XKEY_F4,
0, //XKEY_F5,
0, //XKEY_F6,
0, //XKEY_F7,
0, //XKEY_F8,
0, //XKEY_F9,
0, //XKEY_F10,
0, //XKEY_F11,
0, //XKEY_F12,
0, //XKEY_PRINTSCREEN,
0, //XKEY_SCROLL_LOCK,
0, //XKEY_PAUSE,
0, //XKEY_INSERT,
0, //XKEY_HOME,
0, //XKEY_PRIOR,
0, //XKEY_DELETE,
0, //XKEY_END,
0, //XKEY_NEXT,
0, //XKEY_RIGHT,
0, //XKEY_LEFT,
0, //XKEY_DOWN,
0, //XKEY_UP,
0, //XKEY_NUMLOCK,
'/', //XKEY_DIVIDE,
'*', //XKEY_MULTIPLY,
'-', //XKEY_SUBTRACT,
'+', //XKEY_ADD,
'\n', //XKEY_NUMPAD_ENTER,
'1', //XKEY_NUMPAD1,
'2', //XKEY_NUMPAD2,
'3', //XKEY_NUMPAD3,
'4', //XKEY_NUMPAD4,
'5', //XKEY_NUMPAD5,
'6', //XKEY_NUMPAD6,
'7', //XKEY_NUMPAD7,
'8', //XKEY_NUMPAD8,
'9', //XKEY_NUMPAD9,
'0', //XKEY_NUMPAD0,
'.', //XKEY_DECIMAL,
'\\', //XKEY_BACKSLASH,
0, //XKEY_APPS,
0, //XKEY_SLEEP,
'=', //XKEY_NUMPADEQUALS,
0, //XKEY_F13,
0, //XKEY_F14,
0, //XKEY_F15,
0, //XKEY_F16,
0, //XKEY_F17,
0, //XKEY_F18,
0, //XKEY_F19,
0, //XKEY_F20,
0, //XKEY_F21,
0, //XKEY_F22,
0, //XKEY_F23,
0, //XKEY_F24,
0, //XKEY_EXECUTE,
0, //XKEY_HELP,
0, //XKEY_MENU,
0, //XKEY_SELECT,
0, //XKEY_BROWSER_STOP,
0, //XKEY_INVALID, // Keyboard Again
0, //XKEY_INVALID, // Keyboard Undo
0, //XKEY_INVALID, // Keyboard Cut
0, //XKEY_INVALID, // Keyboard Copy
0, //XKEY_INVALID, // Keyboard Paste
0, //XKEY_BROWSER_SEARCH,
0, //XKEY_VOLUME_MUTE,
0, //XKEY_VOLUME_UP,
0, //XKEY_VOLUME_DOWN,
0, //XKEY_INVALID, // Keyboard locking caps lock
0, //XKEY_INVALID, // Keyboard locking num lock
0, //XKEY_INVALID, // Keyboard locking scroll lock
'.', //XKEY_DECIMAL,
'=', //XKEY_NUMPADEQUALS,
0, //XKEY_INVALID, // Keyboard International 1
0, //XKEY_INVALID, // Keyboard International 2
0, //XKEY_INVALID, // Keyboard International 3
0, //XKEY_INVALID, // Keyboard International 4
0, //XKEY_INVALID, // Keyboard International 5
0, //XKEY_INVALID, // Keyboard International 6
0, //XKEY_INVALID, // Keyboard International 7
0, //XKEY_INVALID, // Keyboard International 8
0, //XKEY_INVALID, // Keyboard International 9
0, //XKEY_INVALID, // Keyboard Lang 1
0, //XKEY_KANJI,
0, //XKEY_KANA,
0, //XKEY_INVALID, // Keyboard Lang 4
0, //XKEY_INVALID, // Keyboard Lang 5
0, //XKEY_INVALID, // Keyboard Lang 6
0, //XKEY_INVALID, // Keyboard Lang 7
0, //XKEY_INVALID, // Keyboard Lang 8
0, //XKEY_INVALID, // Keyboard Lang 9
0, //XKEY_INVALID, // Keyboard Alternate Erase
0, //XKEY_INVALID, // Keyboard SysReq/Attention
0, //XKEY_INVALID, // Keyboard Cancel
0, //XKEY_INVALID, // Keyboard Clear
0, //XKEY_INVALID, // Keyboard Prior
0, //XKEY_INVALID, // Keyboard Return
0, //XKEY_INVALID, // Keyboard Separator
0, //XKEY_INVALID, // Keyboard Out
0, //XKEY_INVALID, // Keyboard Oper
0, //XKEY_INVALID, // Keyboard Clear/Again
0, //XKEY_INVALID, // Keyboard CrSel/Props
0, //XKEY_INVALID, // Keyboard ExSel
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Keypad 00
0, //XKEY_INVALID, // Keypad 000
0, //XKEY_INVALID, // Thousands Separator
0, //XKEY_INVALID, // Decimal Separator
0, //XKEY_INVALID, // Currency Unit
0, //XKEY_INVALID, // Currency Sub-unit
0, //XKEY_INVALID, // Keypad (
0, //XKEY_INVALID, // Keypad )
0, //XKEY_INVALID, // Keypad {
0, //XKEY_INVALID, // Keypad }
0, //XKEY_INVALID, // Keypad Tab
0, //XKEY_INVALID, // Keypad Backspace
0, //XKEY_INVALID, // Keypad A
0, //XKEY_INVALID, // Keypad B
0, //XKEY_INVALID, // Keypad C
0, //XKEY_INVALID, // Keypad D
0, //XKEY_INVALID, // Keypad E
0, //XKEY_INVALID, // Keypad F
0, //XKEY_INVALID, // Keypad XOR
0, //XKEY_INVALID, // Keypad ^
0, //XKEY_INVALID, // Keypad %
0, //XKEY_INVALID, // Keypad <
0, //XKEY_INVALID, // Keypad >
0, //XKEY_INVALID, // Keypad &
0, //XKEY_INVALID, // Keypad &&
0, //XKEY_INVALID, // Keypad |
0, //XKEY_INVALID, // Keypad ||
0, //XKEY_INVALID, // Keypad :
0, //XKEY_INVALID, // Keypad #
0, //XKEY_INVALID, // Keypad Space
0, //XKEY_INVALID, // Keypad @
0, //XKEY_INVALID, // Keypad !
0, //XKEY_INVALID, // Keypad Memory Store
0, //XKEY_INVALID, // Keypad Memory Recall
0, //XKEY_INVALID, // Keypad Memory Clear
0, //XKEY_INVALID, // Keypad Memory Add
0, //XKEY_INVALID, // Keypad Memory Subtract
0, //XKEY_INVALID, // Keypad Memory Multiply
0, //XKEY_INVALID, // Keypad Memory Divide
0, //XKEY_INVALID, // Keypad +/-
0, //XKEY_INVALID, // Keypad Clear
0, //XKEY_INVALID, // Keypad Clear Entry
0, //XKEY_INVALID, // Keypad Binary
0, //XKEY_INVALID, // Keypad Octal
0, //XKEY_INVALID, // Keypad Decimal
0, //XKEY_INVALID, // Keypad Hexadecimal
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_LCONTROL,
0, //XKEY_LSHIFT,
0, //XKEY_LMENU,
0, //XKEY_LWIN,
0, //XKEY_RCONTROL,
0, //XKEY_RSHIFT,
0, //XKEY_RMENU,
0, //XKEY_RWIN,
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
};

static unsigned char ucUSBtoAsciiShifted[256] = 
{
0, //XKEY_INVALID,
0, //XKEY_INVALID,
0, //XKEY_INVALID,
0, //XKEY_INVALID,
'A', //XKEY_A,
'B', //XKEY_B,
'C', //XKEY_C,
'D', //XKEY_D,
'E', //XKEY_E,
'F', //XKEY_F,
'G', //XKEY_G,
'H', //XKEY_H,
'I', //XKEY_I,
'J', //XKEY_J,
'K', //XKEY_K,
'L', //XKEY_L,
'M', //XKEY_M,
'N', //XKEY_N,
'O', //XKEY_O,
'P', //XKEY_P,
'Q', //XKEY_Q,
'R', //XKEY_R,
'S', //XKEY_S,
'T', //XKEY_T,
'U', //XKEY_U,
'V', //XKEY_V,
'W', //XKEY_W,
'X', //XKEY_X,
'Y', //XKEY_Y,
'Z', //XKEY_Z,
'!', //XKEY_1,
'"', //XKEY_2,
'£', //XKEY_3,
'$', //XKEY_4,
'%', //XKEY_5,
'^', //XKEY_6,
'&', //XKEY_7,
'*', //XKEY_8,
'(', //XKEY_9,
')', //XKEY_0,
'\n', //XKEY_RETURN,
27, //XKEY_ESCAPE,
8, //XKEY_BACKSPACE,
9, //XKEY_TAB,
' ', //XKEY_SPACE,
'_', //XKEY_MINUS,
'+', //XKEY_EQUALS,
'{', //XKEY_LBRACE,
'}', //XKEY_RBRACE,
'|', //XKEY_BACKSLASH,
'¬', //XKEY_GRAVE,
':', //XKEY_SEMICOLON,
'@', //XKEY_QUOTE,
'¬', //XKEY_GRAVE,
'<', //XKEY_COMMA,
'>', //XKEY_PERIOD,
'?', //XKEY_SLASH,
0, //XKEY_CAPSLOCK,
0, //XKEY_F1,
0, //XKEY_F2,
0, //XKEY_F3,
0, //XKEY_F4,
0, //XKEY_F5,
0, //XKEY_F6,
0, //XKEY_F7,
0, //XKEY_F8,
0, //XKEY_F9,
0, //XKEY_F10,
0, //XKEY_F11,
0, //XKEY_F12,
0, //XKEY_PRINTSCREEN,
0, //XKEY_SCROLL_LOCK,
0, //XKEY_PAUSE,
0, //XKEY_INSERT,
0, //XKEY_HOME,
0, //XKEY_PRIOR,
0, //XKEY_DELETE,
0, //XKEY_END,
0, //XKEY_NEXT,
0, //XKEY_RIGHT,
0, //XKEY_LEFT,
0, //XKEY_DOWN,
0, //XKEY_UP,
0, //XKEY_NUMLOCK,
'/', //XKEY_DIVIDE,
'*', //XKEY_MULTIPLY,
'-', //XKEY_SUBTRACT,
'+', //XKEY_ADD,
'\n', //XKEY_NUMPAD_ENTER,
0, //XKEY_NUMPAD1,
0, //XKEY_NUMPAD2,
0, //XKEY_NUMPAD3,
0, //XKEY_NUMPAD4,
0, //XKEY_NUMPAD5,
0, //XKEY_NUMPAD6,
0, //XKEY_NUMPAD7,
0, //XKEY_NUMPAD8,
0, //XKEY_NUMPAD9,
0, //XKEY_NUMPAD0,
0, //XKEY_DECIMAL,
'|', //XKEY_BACKSLASH,
0, //XKEY_APPS,
0, //XKEY_SLEEP,
'=', //XKEY_NUMPADEQUALS,
0, //XKEY_F13,
0, //XKEY_F14,
0, //XKEY_F15,
0, //XKEY_F16,
0, //XKEY_F17,
0, //XKEY_F18,
0, //XKEY_F19,
0, //XKEY_F20,
0, //XKEY_F21,
0, //XKEY_F22,
0, //XKEY_F23,
0, //XKEY_F24,
0, //XKEY_EXECUTE,
0, //XKEY_HELP,
0, //XKEY_MENU,
0, //XKEY_SELECT,
0, //XKEY_BROWSER_STOP,
0, //XKEY_INVALID, // Keyboard Again
0, //XKEY_INVALID, // Keyboard Undo
0, //XKEY_INVALID, // Keyboard Cut
0, //XKEY_INVALID, // Keyboard Copy
0, //XKEY_INVALID, // Keyboard Paste
0, //XKEY_BROWSER_SEARCH,
0, //XKEY_VOLUME_MUTE,
0, //XKEY_VOLUME_UP,
0, //XKEY_VOLUME_DOWN,
0, //XKEY_INVALID, // Keyboard locking caps lock
0, //XKEY_INVALID, // Keyboard locking num lock
0, //XKEY_INVALID, // Keyboard locking scroll lock
0, //XKEY_DECIMAL,
'=', //XKEY_NUMPADEQUALS,
0, //XKEY_INVALID, // Keyboard International 1
0, //XKEY_INVALID, // Keyboard International 2
0, //XKEY_INVALID, // Keyboard International 3
0, //XKEY_INVALID, // Keyboard International 4
0, //XKEY_INVALID, // Keyboard International 5
0, //XKEY_INVALID, // Keyboard International 6
0, //XKEY_INVALID, // Keyboard International 7
0, //XKEY_INVALID, // Keyboard International 8
0, //XKEY_INVALID, // Keyboard International 9
0, //XKEY_INVALID, // Keyboard Lang 1
0, //XKEY_KANJI,
0, //XKEY_KANA,
0, //XKEY_INVALID, // Keyboard Lang 4
0, //XKEY_INVALID, // Keyboard Lang 5
0, //XKEY_INVALID, // Keyboard Lang 6
0, //XKEY_INVALID, // Keyboard Lang 7
0, //XKEY_INVALID, // Keyboard Lang 8
0, //XKEY_INVALID, // Keyboard Lang 9
0, //XKEY_INVALID, // Keyboard Alternate Erase
0, //XKEY_INVALID, // Keyboard SysReq/Attention
0, //XKEY_INVALID, // Keyboard Cancel
0, //XKEY_INVALID, // Keyboard Clear
0, //XKEY_INVALID, // Keyboard Prior
0, //XKEY_INVALID, // Keyboard Return
0, //XKEY_INVALID, // Keyboard Separator
0, //XKEY_INVALID, // Keyboard Out
0, //XKEY_INVALID, // Keyboard Oper
0, //XKEY_INVALID, // Keyboard Clear/Again
0, //XKEY_INVALID, // Keyboard CrSel/Props
0, //XKEY_INVALID, // Keyboard ExSel
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Keypad 00
0, //XKEY_INVALID, // Keypad 000
0, //XKEY_INVALID, // Thousands Separator
0, //XKEY_INVALID, // Decimal Separator
0, //XKEY_INVALID, // Currency Unit
0, //XKEY_INVALID, // Currency Sub-unit
0, //XKEY_INVALID, // Keypad (
0, //XKEY_INVALID, // Keypad )
0, //XKEY_INVALID, // Keypad {
0, //XKEY_INVALID, // Keypad }
0, //XKEY_INVALID, // Keypad Tab
0, //XKEY_INVALID, // Keypad Backspace
0, //XKEY_INVALID, // Keypad A
0, //XKEY_INVALID, // Keypad B
0, //XKEY_INVALID, // Keypad C
0, //XKEY_INVALID, // Keypad D
0, //XKEY_INVALID, // Keypad E
0, //XKEY_INVALID, // Keypad F
0, //XKEY_INVALID, // Keypad XOR
0, //XKEY_INVALID, // Keypad ^
0, //XKEY_INVALID, // Keypad %
0, //XKEY_INVALID, // Keypad <
0, //XKEY_INVALID, // Keypad >
0, //XKEY_INVALID, // Keypad &
0, //XKEY_INVALID, // Keypad &&
0, //XKEY_INVALID, // Keypad |
0, //XKEY_INVALID, // Keypad ||
0, //XKEY_INVALID, // Keypad :
0, //XKEY_INVALID, // Keypad #
0, //XKEY_INVALID, // Keypad Space
0, //XKEY_INVALID, // Keypad @
0, //XKEY_INVALID, // Keypad !
0, //XKEY_INVALID, // Keypad Memory Store
0, //XKEY_INVALID, // Keypad Memory Recall
0, //XKEY_INVALID, // Keypad Memory Clear
0, //XKEY_INVALID, // Keypad Memory Add
0, //XKEY_INVALID, // Keypad Memory Subtract
0, //XKEY_INVALID, // Keypad Memory Multiply
0, //XKEY_INVALID, // Keypad Memory Divide
0, //XKEY_INVALID, // Keypad +/-
0, //XKEY_INVALID, // Keypad Clear
0, //XKEY_INVALID, // Keypad Clear Entry
0, //XKEY_INVALID, // Keypad Binary
0, //XKEY_INVALID, // Keypad Octal
0, //XKEY_INVALID, // Keypad Decimal
0, //XKEY_INVALID, // Keypad Hexadecimal
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_LCONTROL,
0, //XKEY_LSHIFT,
0, //XKEY_LMENU,
0, //XKEY_LWIN,
0, //XKEY_RCONTROL,
0, //XKEY_RSHIFT,
0, //XKEY_RMENU,
0, //XKEY_RWIN,
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
0, //XKEY_INVALID, // Reserved
};

#endif
