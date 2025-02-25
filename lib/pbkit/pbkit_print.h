// pbKit text overlay functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021-2025 Erik Abair

#ifndef PBKIT_PRINT_H
#define PBKIT_PRINT_H

#if defined(__cplusplus)
extern "C" {
#endif

// Adds the given printf-style string to the text overlay. advancing the cursor
// past the end of the string.
void pb_print(const char *format, ...);

// Sets the cursor to the given row and column, then adds the given
// printf-style string to the text overlay, advancing the cursor past the end of
// the string.
//
// Note: If the provided row or column are invalid, they are ignored and the
// current cursor row and/or column are used instead.
void pb_printat(int row, int col, const char *format, ...);

// Clears the text overlay.
void pb_erase_text_screen(void);

// Renders the text overlay to the current framebuffer.
void pb_draw_text_screen(void);

// Adds the given character to the text overlay at the current cursor position,
// then shifts the cursor position by one column, wrapping if needed.
void pb_print_char(char c);

#if defined(__cplusplus)
}
#endif

#endif // PBKIT_PRINT_H
