// pbKit gamma ramp functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2025 Stefan Schmidt

#include "pbkit_gamma.h"
#include "outer.h"

void pb_set_gamma_ramp (const PB_GAMMA_RAMP *pGammaRamp)
{
    VIDEOREG8(NV_USER_DAC_WRITE_MODE_ADDRESS) = 0;

    for (int i = 0; i < 256; i++) {
        VIDEOREG8(NV_USER_DAC_PALETTE_DATA) = pGammaRamp->red[i];
        VIDEOREG8(NV_USER_DAC_PALETTE_DATA) = pGammaRamp->green[i];
        VIDEOREG8(NV_USER_DAC_PALETTE_DATA) = pGammaRamp->blue[i];
    }
}
