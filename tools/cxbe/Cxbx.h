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
// *  You should have received a copy of the GNU General Public License
// *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
// *
// *  (c) 2002-2003 Aaron Robinson <caustik@caustik.com>
// *
// *  All rights reserved
// *
// ******************************************************************
#ifndef CXBX_H
#define CXBX_H

#include <stdint.h>

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
typedef intptr_t       sintptr;

#define VERSION "unknown"

// round dwValue to the nearest multiple of dwMult
static uint32 RoundUp(uint32 dwValue, uint32 dwMult)
{
    if(dwMult == 0)
        return dwValue;

    return dwValue - (dwValue-1)%dwMult + (dwMult - 1);
}

#endif
