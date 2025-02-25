// pbKit DMA channel utility functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021-2025 Erik Abair

#include "pbkit_dma.h"

#include "nv_objects.h"
#include "outer.h"
#include "pbkit_pushbuffer.h"

// Value that may be added to contiguous memory addresses to access as ADDR_AGPMEM, which is guaranteed to be linear
// (and thus may be slower than tiled ADDR_FBMEM but can be manipulated directly).
#define AGP_MEMORY_REMAP 0xF0000000

#define PB_SETOUTER 0xB2A

void pb_set_dma_address (const struct s_CtxDma *context, const void *address, uint32_t limit)
{
    uint32_t dma_addr = ((uint32_t)address) & 0x03FFFFFF;
    uint32_t dma_flags = DMA_CLASS_3D | 0x0000B000;
    dma_addr |= 3;

    uint32_t *p = pb_begin();
    p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_WAIT_MAKESPACE, 0);

    // set params addr,data
    p = pb_push2(p, NV20_TCL_PRIMITIVE_3D_PARAMETER_A, NV_PRAMIN + (context->Inst << 4) + 0x08, dma_addr);

    // calls subprogID PB_SETOUTER: does VIDEOREG(addr)=data
    p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_FIRE_INTERRUPT, PB_SETOUTER);
    p = pb_push2(p, NV20_TCL_PRIMITIVE_3D_PARAMETER_A, NV_PRAMIN + (context->Inst << 4) + 0x0C, dma_addr);
    p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_FIRE_INTERRUPT, PB_SETOUTER);
    p = pb_push2(p, NV20_TCL_PRIMITIVE_3D_PARAMETER_A, NV_PRAMIN + (context->Inst << 4) + 0x00, dma_flags);
    p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_FIRE_INTERRUPT, PB_SETOUTER);
    p = pb_push2(p, NV20_TCL_PRIMITIVE_3D_PARAMETER_A, NV_PRAMIN + (context->Inst << 4) + 0x04, limit);
    p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_FIRE_INTERRUPT, PB_SETOUTER);

    pb_end(p);
}

void pb_bind_subchannel (uint32_t subchannel, const struct s_CtxDma *context)
{
    uint32_t *p = pb_begin();
    p = pb_push1_to(subchannel, p, NV20_TCL_PRIMITIVE_SET_MAIN_OBJECT, context->ChannelID);
    pb_end(p);
}

void *pb_agp_access (void *fb_memory_pointer)
{
    return (void *)(((uint32_t)fb_memory_pointer) | AGP_MEMORY_REMAP);
}
