// ******************************************************************
// *
// *    .,-:::::    .,::      .::::::::.    .,::      .:
// *  ,;;;'````'    `;;;,  .,;;  ;;;'';;'   `;;;,  .,;; 
// *  [[[             '[[,,[['   [[[__[[\.    '[[,,[['  
// *  $$$              Y$$$P     $$""""Y$$     Y$$$P    
// *  `88bo,__,o,    oP"``"Yo,  _88o,,od8P   oP"``"Yo,  
// *    "YUMMMMMP",m"       "Mm,""YUMMMP" ,m"       "Mm,
// *
// *   Cxbx->Cxbx.h
// *
// *  This file is part of the Cxbx project.
// *
// *  Cxbx and Cxbe are free software; you can redistribute them
// *  and/or modify them under the terms of the GNU General Public
// *  License as published by the Free Software Foundation; either
// *  version 2 of the license, or (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// *  GNU General Public License for more details.
// *
// *  You should have recieved a copy of the GNU General Public License
// *  along with this program; see the file COPYING.
// *  If not, write to the Free Software Foundation, Inc.,
// *  59 Temple Place - Suite 330, Bostom, MA 02111-1307, USA.
// *
// *  (c) 2002-2003 Aaron Robinson <caustik@caustik.com>
// *
// *  All rights reserved
// *
// ******************************************************************
#ifndef CXBX_H
#define CXBX_H

#include <stdint.h>

// CxbxKrnl exports, others import
#ifndef _CXBXKRNL_INTERNAL
#define CXBXKRNL_API __declspec(dllimport)
#else
#define CXBXKRNL_API __declspec(dllexport)
#endif

// Caustik's favorite typedefs
typedef signed int     sint;
typedef unsigned int   uint;
typedef int8_t         int08;
typedef int16_t        int16;
typedef int32_t        int32;
typedef uint8_t        uint08;
typedef uint16_t       uint16;
typedef uint32_t       uint32;
typedef int8_t         sint08;
typedef int16_t        sint16;
typedef int32_t        sint32;

// define this to track resources for debugging purposes
//#define _DEBUG_TRACK_VB // Vertex Buffers
//#define _DEBUG_TRACK_VS // Vertex Shaders
//#define _DEBUG_TRACK_PB // Push Buffers

// define this to track memory allocations
//#define _DEBUG_ALLOC

// define this to trace intercepted function calls
#define _DEBUG_TRACE

// define this to trace warnings
#define _DEBUG_WARNINGS

// define these to dump textures
//#define _DEBUG_DUMP_TEXTURE_SETTEXTURE "C:\\Aaron\\Textures\\"
//#define _DEBUG_DUMP_TEXTURE_REGISTER   "C:\\Aaron\\Textures\\"

// version information
#ifndef _DEBUG_TRACE
#define _CXBX_VERSION "0.8.0-Pre3"
#else
#define _CXBX_VERSION "0.8.0-Pre3-Trace"
#endif

// round dwValue to the nearest multiple of dwMult
static uint32 RoundUp(uint32 dwValue, uint32 dwMult)
{
    if(dwMult == 0)
        return dwValue;

    return dwValue - (dwValue-1)%dwMult + (dwMult - 1);
}

// debug mode choices, either console screen or external file
enum DebugMode
{
    DM_NONE,
    DM_CONSOLE,
    DM_FILE
};

// maximum number of threads cxbx can handle
#define MAXIMUM_XBOX_THREADS 256

extern volatile bool g_bPrintfOn;

// convienance debug output macros
#ifdef _DEBUG_TRACE
#define DbgPrintf if(g_bPrintfOn) printf
#else
inline void null_func(...) { }
#define DbgPrintf null_func
#endif

#endif
