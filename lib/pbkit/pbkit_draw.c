// pbKit drawing functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021-2025 Erik Abair

#include "pbkit_draw.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "nv_objects.h"
#include "nv_regs.h"
#include "pbkit_pushbuffer.h"

extern unsigned int pb_ColorFmt;

void pb_fill(int x, int y, int w, int h, DWORD color)
{
    uint32_t *p;

    int x1, y1, x2, y2;

    x1 = x;
    y1 = y;
    x2 = x + w;
    y2 = y + h;

    switch (pb_ColorFmt) {
        case NV097_SET_SURFACE_FORMAT_COLOR_LE_X1R5G5B5_Z1R5G5B5:
        case NV097_SET_SURFACE_FORMAT_COLOR_LE_X1R5G5B5_O1R5G5B5:
            color = ((color >> 16) & 0x8000) | ((color >> 7) & 0x7C00) | ((color >> 5) & 0x03E0) | ((color >> 3) & 0x001F);
            break;
        case NV097_SET_SURFACE_FORMAT_COLOR_LE_R5G6B5:
            color = ((color >> 8) & 0xF800) | ((color >> 5) & 0x07E0) | ((color >> 3) & 0x001F);
            break;
        case NV097_SET_SURFACE_FORMAT_COLOR_LE_A8R8G8B8:
            // Nothing to do, input is A8R8G8B8
            break;
        default:
            assert(false);
            break;
    }

    p = pb_begin();
    pb_push(p++, NV20_TCL_PRIMITIVE_3D_CLEAR_VALUE_HORIZ, 2); // sets rectangle coordinates
    *(p++) = ((x2 - 1) << 16) | x1;
    *(p++) = ((y2 - 1) << 16) | y1;
    pb_push(p++, NV20_TCL_PRIMITIVE_3D_CLEAR_VALUE_DEPTH, 3); // sets data used to fill in rectangle
    *(p++) = 0;                                               //(depth<<8)|stencil
    *(p++) = color;                                           // color
    *(p++) = 0xF0;                                            // triggers the HW rectangle fill (0x03 for D&S)
    pb_end(p);
}
