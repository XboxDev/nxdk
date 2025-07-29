// pbKit gamma ramp functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2025 Stefan Schmidt

#ifndef PBKIT_GAMMA_H
#define PBKIT_GAMMA_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct _PB_GAMMA_RAMP
{
    uint8_t red[256];
    uint8_t green[256];
    uint8_t blue[256];
} PB_GAMMA_RAMP;

void pb_set_gamma_ramp (const PB_GAMMA_RAMP *pGammaRamp);

#if defined(__cplusplus)
}
#endif

#endif // PBKIT_GAMMA_H
