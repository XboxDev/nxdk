// clang-format off

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
#include "pbkit_dma.h"
#include "pbkit_draw.h"
#include "pbkit_framebuffer.h"
#include "pbkit_print.h"
#include "pbkit_pushbuffer.h"

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

void    pb_extra_buffers(int n);//requests additional back buffers (default is 0) (call it before pb_init)
void    pb_size(DWORD size);    //sets push buffer size (default is 512Kb) (call it before pb_init)
int     pb_init(void);      //returns 0 if everything went well (starts Dma engine)
void    pb_kill(void);      //stops Dma engine and releases push buffer

void    pb_target_extra_buffer(int n);  //to have rendering made into a static extra buffer
void    pb_target_back_buffer(void);    //to have rendering made into normal rotating back buffer

DWORD   *pb_extra_buffer(int n);    //returns a static extra buffer address
DWORD   *pb_back_buffer(void);      //returns normal rotating back buffer address
DWORD   pb_back_buffer_width(void);
DWORD   pb_back_buffer_height(void);
DWORD   pb_back_buffer_pitch(void);

void    pb_set_viewport(int dwx,int dwy,int width,int height,float zmin,float zmax);

int pb_busy(void);

#ifdef __cplusplus
}
#endif

#endif
