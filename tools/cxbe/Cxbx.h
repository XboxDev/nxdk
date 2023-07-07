// This file is part of Cxbe
// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2019 Jannik Vogel
// SPDX-FileCopyrightText: 2021 Stefan Schmidt

#ifndef CXBX_H
#define CXBX_H

#include <stdint.h>

// Caustik's favorite typedefs
typedef signed int sint;
typedef unsigned int uint;
typedef int8_t int08;
typedef int16_t int16;
typedef int32_t int32;
typedef uint8_t uint08;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int8_t sint08;
typedef int16_t sint16;
typedef int32_t sint32;
typedef intptr_t sintptr;

#define VERSION "unknown"

// round dwValue to the nearest multiple of dwMult
static uint32 RoundUp(uint32 dwValue, uint32 dwMult)
{
    if(dwMult == 0)
        return dwValue;

    return dwValue - (dwValue - 1) % dwMult + (dwMult - 1);
}

#endif
