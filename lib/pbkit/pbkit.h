//pbKit header

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021 Erik Abair

#ifndef _PBKIT_H_
#define _PBKIT_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include <xboxkrnl/xboxkrnl.h>
#include <stdint.h>
#if !defined(__cplusplus)
#include <stdbool.h>
#endif

#include "outer.h"
#include "nv_objects.h"
#include "nv_regs.h"

//4x4 matrices indexes
#define _11                 0
#define _12                 1
#define _13                 2
#define _14                 3
#define _21                 4
#define _22                 5
#define _23                 6
#define _24                 7
#define _31                 8
#define _32                 9
#define _33                 10
#define _34                 11
#define _41                 12
#define _42                 13
#define _43                 14
#define _44                 15

//GPU subchannels
#define SUBCH_3D                0
#define SUBCH_2                 2
#define SUBCH_3                 3
#define SUBCH_4                 4
#define NEXT_SUBCH              5

//DMA and graphics classes
#define DMA_CLASS_2                 0x02
#define DMA_CLASS_3                 0x03
#define DMA_CLASS_3D                0x3D
#define GR_CLASS_19                 0x19
#define GR_CLASS_30                 0x30
#define GR_CLASS_39                 0x39
#define GR_CLASS_62                 0x62
#define GR_CLASS_97                 0x97
#define GR_CLASS_9F                 0x9F

struct s_CtxDma
{
  DWORD               ChannelID;
  DWORD               Inst;   //Addr in PRAMIN area, unit=16 bytes blocks, baseaddr=VIDEO_BASE+NV_PRAMIN
  DWORD               Class;
  DWORD               isGr;
};

void    pb_show_front_screen(void); //shows scene (allows VBL synced screen swapping)
void    pb_show_debug_screen(void); //shows debug screen (default openxdk+SDL buffer)
void    pb_show_depth_screen(void); //shows depth screen (depth stencil buffer)

DWORD   pb_get_vbl_counter(void);
DWORD   pb_wait_for_vbl(void);  //waits for VBL, returns VBL counter value as bonus info
                //you can avoid calling it and try to draw a lot in advance
                //use it if you need strong synchronization with controls

//ALWAYS use this at beginning of frame or you may lose one third of performance because
//automatic compression algorithm for tile #1 can't afford any garbage left behind...
void    pb_erase_depth_stencil_buffer(int x, int y, int w, int h);

void    pb_reset(void); //forces a jump to push buffer head (do it at frame start)
int pb_finished(void);  //prepare screen swapping at VBlank (do it at frame end)
                //if it returns 1 it failed (too early, just wait & retry)
                //that means you can draw more details in your scene

void pb_wait_until_gr_not_busy(void);
DWORD pb_wait_until_tiles_not_busy(void);

uint32_t   *pb_begin(void);    //start a block with this (avoid more than 128 dwords per block)
void    pb_push_to(DWORD subchannel, uint32_t *p, DWORD command, DWORD nparam);
uint32_t   *pb_push1_to(DWORD subchannel, uint32_t *p, DWORD command, DWORD param1);
uint32_t   *pb_push2_to(DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2);
uint32_t   *pb_push3_to(DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3);
uint32_t   *pb_push4_to(DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3, DWORD param4);
uint32_t   *pb_push4f_to(DWORD subchannel, uint32_t *p, DWORD command, float param1, float param2, float param3, float param4);
void    pb_push(uint32_t *p, DWORD command, DWORD nparam);
uint32_t   *pb_push1(uint32_t *p, DWORD command, DWORD param1);
uint32_t   *pb_push2(uint32_t *p, DWORD command, DWORD param1, DWORD param2);
uint32_t   *pb_push3(uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3);
uint32_t   *pb_push4(uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3, DWORD param4);
uint32_t   *pb_push4f(uint32_t *p, DWORD command, float param1, float param2, float param3, float param4);
uint32_t   *pb_push_transposed_matrix(uint32_t *p, DWORD command, float *m);
void    pb_end(uint32_t *pEnd);    //end a block with this (triggers the data sending to GPU)

void    pb_extra_buffers(int n);//requests additional back buffers (default is 0) (call it before pb_init)
void    pb_size(DWORD size);    //sets push buffer size (default is 512Kb) (call it before pb_init)
void    pb_set_color_format(unsigned int fmt, bool swizzled); // sets color surface format (call it before pb_init)
int     pb_init(void);      //returns 0 if everything went well (starts Dma engine)
void    pb_kill(void);      //stops Dma engine and releases push buffer

void    pb_print(const char *format, ...);  //populates a text screen array
void    pb_printat(int row, int col, char *format, ...);    //populates a text screen array
void    pb_erase_text_screen(void); //clears array
void    pb_draw_text_screen(void);  //converts array into drawing sequences

void    pb_target_extra_buffer(int n);  //to have rendering made into a static extra buffer
void    pb_target_back_buffer(void);    //to have rendering made into normal rotating back buffer

DWORD   *pb_extra_buffer(int n);    //returns a static extra buffer address
DWORD   *pb_back_buffer(void);      //returns normal rotating back buffer address
DWORD   pb_back_buffer_width(void);
DWORD   pb_back_buffer_height(void);
DWORD   pb_back_buffer_pitch(void);

void    pb_fill(int x,int y,int w,int h, DWORD color);  //rectangle fill

void    pb_set_viewport(int dwx,int dwy,int width,int height,float zmin,float zmax);

void    pb_set_fb_size_multiplier(unsigned int multiplier);

int pb_busy(void);


void pb_create_dma_ctx(DWORD ChannelID,
                       DWORD Class,
                       DWORD Base,
                       DWORD Limit,
                       struct s_CtxDma *pDmaObject);
void pb_create_gr_ctx(int ChannelID,
                      int Class,
                      struct s_CtxDma *pGrObject);
void pb_bind_channel(struct s_CtxDma *pCtxDmaObject);

uint8_t *pb_depth_stencil_buffer();
DWORD pb_depth_stencil_pitch();
DWORD pb_depth_stencil_size();

DWORD pb_reserve_instance(DWORD size);
void pb_create_gr_instance(int ChannelID,
                        int Class,
                        DWORD instance,
                        DWORD flags,
                        DWORD flags3D,
                        struct s_CtxDma *pGrObject);

void pb_print_char(char c);
#ifdef __cplusplus
}
#endif

#endif
