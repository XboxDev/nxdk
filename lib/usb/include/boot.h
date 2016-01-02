#ifndef _Boot_H_
#define _Boot_H_

/***************************************************************************
      Includes used by XBox boot code
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/////////////////////////////////
// configuration


///////// BootUSB.c

//#include "sys/types.h"
//#include "sys/nttypes.h"
 #include <stddef.h>
// #include <openxdk/openxdk.h>
// #include <memory.h>
#include <string.h>
#include <stdlib.h>
// #include <malloc.h>

static inline double min (double a, double b)
{
        if (a < b) return a; else return b;
}

static inline double max (double a, double b)
{
        if (a > b) return a; else return b;
}

#endif // _Boot_H_

