// pbKit framebuffer management functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021-2025 Erik Abair

#ifndef PBKIT_FRAMEBUFFER_H
#define PBKIT_FRAMEBUFFER_H

#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

// Sets the framebuffer's color surface format (call it before pb_init).
void pb_set_color_format (unsigned int fmt, bool swizzled);

#if defined(__cplusplus)
}
#endif

#endif // PBKIT_FRAMEBUFFER_H
