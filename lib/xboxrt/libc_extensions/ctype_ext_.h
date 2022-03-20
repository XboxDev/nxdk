// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#ifdef __cplusplus
extern "C" {
#endif

// (obsolete) POSIX function, but required by libc++
static int isascii (int c)
{
    return c >= 0 && c < 128;
}

// Defined as on Win32
#define _UPPER   0x01
#define _LOWER   0x02
#define _DIGIT   0x04
#define _SPACE   0x08
#define _PUNCT   0x10
#define _CONTROL 0x20
#define _BLANK   0x40
#define _HEX     0x80
#define _ALPHA   (0x0100 | _UPPER | _LOWER)

#ifdef __cplusplus
}
#endif
