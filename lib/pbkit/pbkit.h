//pbKit header
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, see <http://www.gnu.org/licenses/>

#ifndef _PBKIT_H_
#define _PBKIT_H_

#include <xboxkrnl/xboxkrnl.h>

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

//fastest way to write a method for subchannel 3D (0)
#define pb_push(p,command,nparam)       *(p)=((nparam<<18)+command)

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

DWORD   *pb_begin(void);    //start a block with this (avoid more than 128 dwords per block)
void    pb_push1to(DWORD subchannel, DWORD *p, DWORD command, DWORD param1); //slow functions but with debug messages
void    pb_push2to(DWORD subchannel, DWORD *p, DWORD command, DWORD param1, DWORD param2);
void    pb_push3to(DWORD subchannel, DWORD *p, DWORD command, DWORD param1, DWORD param2, DWORD param3);
void    pb_push4to(DWORD subchannel, DWORD *p, DWORD command, DWORD param1, DWORD param2, DWORD param3, DWORD param4);
void    pb_push1(DWORD *p, DWORD command, DWORD param1); //slow functions but with debug messages (targets SUBCH_3D (0))
void    pb_push2(DWORD *p, DWORD command, DWORD param1, DWORD param2);
void    pb_push3(DWORD *p, DWORD command, DWORD param1, DWORD param2, DWORD param3);
void    pb_push4(DWORD *p, DWORD command, DWORD param1, DWORD param2, DWORD param3, DWORD param4);
void    pb_push4f(DWORD *p, DWORD command, float param1, float param2, float param3, float param4);
void    pb_push_transposed_matrix(DWORD *p, DWORD command, float *m);
void    pb_end(DWORD *pEnd);    //end a block with this (triggers the data sending to GPU)

void    pb_extra_buffers(int n);//requests additional back buffers (default is 0) (call it before pb_init)
void    pb_size(DWORD size);    //sets push buffer size (default is 512Kb) (call it before pb_init)
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

int pb_busy(void);

#endif
