// pbKit DMA channel utility functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021-2025 Erik Abair

#ifndef PBKIT_DMA_H
#define PBKIT_DMA_H

#include <stdint.h>

#include <xboxkrnl/xboxkrnl.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define MAXRAM 0x03FFAFFF

// DMA classes
#define DMA_CLASS_2  0x02
#define DMA_CLASS_3  0x03
#define DMA_CLASS_3D 0x3D

// Graphics classes
#define GR_CLASS_12 0x12 // "Beta" class for blending operations (see xf86-video-nouveau).
#define GR_CLASS_19 0x19
#define GR_CLASS_30 0x30
#define GR_CLASS_39 0x39
#define GR_CLASS_62 0x62
#define GR_CLASS_72 0x72 // "Beta4" class for blending operations (see xf86-video-nouveau).
#define GR_CLASS_97 0x97
#define GR_CLASS_9F 0x9F

// Enumerated DMA channel destinations
#define DMA_CHANNEL_3D_3                   3
#define DMA_CHANNEL_PIXEL_RENDERER         9
#define DMA_CHANNEL_DEPTH_STENCIL_RENDERER 10
#define DMA_CHANNEL_BITBLT_IMAGES          11

struct s_CtxDma
{
    DWORD ChannelID;
    DWORD Inst; // Addr in PRAMIN area, unit=16 bytes blocks, baseaddr=VIDEO_BASE+NV_PRAMIN
    DWORD Class;
    DWORD isGr;
};

// Points an existing DMA context object at a new address.
void pb_set_dma_address(const struct s_CtxDma *context, const void *address, uint32_t limit);

// Binds a subchannel to the given context.
void pb_bind_subchannel(uint32_t subchannel, const struct s_CtxDma *context);

// Returns an AGP version of the given nv2a tiled memory address.
void *pb_agp_access(void *fb_memory_pointer);

// Creates a DMA context for the given channel, class, and address space.
void pb_create_dma_ctx(DWORD ChannelID,
                       DWORD Class,
                       DWORD Base,
                       DWORD Limit,
                       struct s_CtxDma *pDmaObject);

// Binds the given DMA context.
void pb_bind_channel(struct s_CtxDma *pCtxDmaObject);

// Creates the context object for the given DMA object.
void pb_create_gr_ctx(int ChannelID, int Class, struct s_CtxDma *pCtxDmaObject);

#ifdef __cplusplus
}
#endif

#endif // PBKIT_DMA_H
