// pbKit drawing functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021-2025 Erik Abair

#ifndef PBKIT_DRAW_H
#define PBKIT_DRAW_H

#include <stdint.h>
#include <xboxkrnl/xboxkrnl.h>

#if defined(__cplusplus)
extern "C" {
#endif

// Fills the rectangular region described by (x, y, x + w, y + h) with the given
// color.
void pb_fill (int x, int y, int w, int h, DWORD color);

void pb_set_depth_stencil_buffer_region (uint32_t depth_buffer_format, uint32_t depth_value, uint8_t stencil_value,
                                         uint32_t left, uint32_t top, uint32_t width, uint32_t height);

#if defined(__cplusplus)
}
#endif

#endif // PBKIT_DRAW_H
