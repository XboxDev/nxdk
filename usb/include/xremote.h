#ifndef _XREMOTE_H_
#define _XREMOTE_H_

extern u16 current_remote_key;
extern u8 remotekeyIsRepeat;

#define RC_KEY_SELECT	0x0b0a
#define RC_KEY_UP	0xa60a
#define RC_KEY_DOWN	0xa70a
#define RC_KEY_RIGHT	0xa80a
#define RC_KEY_LEFT	0xa90a
#define RC_KEY_INFO	0xc30a
#define RC_KEY_9	0xc60a
#define RC_KEY_8	0xc70a
#define RC_KEY_7	0xc80a
#define RC_KEY_6	0xc90a
#define RC_KEY_5	0xca0a
#define RC_KEY_4	0xcb0a
#define RC_KEY_3	0xcc0a
#define RC_KEY_2	0xcd0a
#define RC_KEY_1	0xce0a
#define RC_KEY_0	0xcf0a
#define RC_KEY_DISPLAY	0xd50a
#define RC_KEY_BACK	0xd80a
#define RC_KEY_SKIPF	0xdd0a
#define RC_KEY_SKIPB	0xdf0a
#define RC_KEY_STOP	0xe00a
#define RC_KEY_REW	0xe20a
#define RC_KEY_FWD	0xe30a
#define RC_KEY_TITLE	0xe50a
#define RC_KEY_PAUSE	0xe60a
#define RC_KEY_PLAY	0xea0a
#define RC_KEY_MENU	0xf70a

#endif
