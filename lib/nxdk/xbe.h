/*
 * Copyright (c) 2022 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#ifndef __NXDK_XBE_H__
#define __NXDK_XBE_H__

#include <xboxkrnl/xboxdef.h>

#ifdef __cplusplus
extern "C" {
#endif

PXBE_SECTION_HEADER nxXbeGetSectionByName (const char *name);

#ifdef __cplusplus
}
#endif

#endif
