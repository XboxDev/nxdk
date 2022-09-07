// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2004 Craig Edwards
// SPDX-FileCopyrightText: 2005 Robin Mulloy
// SPDX-FileCopyrightText: 2005 Tom Burns
// SPDX-FileCopyrightText: 2006 Richard Osborne

// SPDX-FileCopyrightText: 2019-2021 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Jannik Vogel
// SPDX-FileCopyrightText: 2020 Bennet Blischke
// SPDX-FileCopyrightText: 2020 Matthew Lawson


//
//	Debug output scrolling code submitted by Robin Mulloy
//
//
#ifndef HAL_DEBUG_H
#define HAL_DEBUG_H

#if defined(__cplusplus)
extern "C"
{
#endif

#define WHITE   0x00FFFFFF
#define BLACK   0x00000000

#define WHITE_16BPP   0xFFFF
#define BLACK_16BPP  0x0000

#define WHITE_15BPP   0x7FFF
#define BLACK_15BPP  0x0000

/**
 * Prints a message to whatever debug facilities might
 * be available.
 */
void debugPrint(const char *format, ...) __attribute__((format(printf, 1, 2)));
void debugPrintNum(int i);
void debugPrintBinary( int num );
void debugPrintHex(const char *buffer, int length);
void debugClearScreen( void );
void debugAdvanceScreen( void );
void debugMoveCursor(int x, int y);
void debugResetCursor( void );

#ifdef __cplusplus
}
#endif

#endif
