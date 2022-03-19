// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Stefan Schmidt

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
