// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Jannik Vogel

#ifndef __SHLOBJ_CORE_H__
#define __SHLOBJ_CORE_H__

#include <minwinbase.h>
#include <windef.h>
#include <winnt.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CSIDL_APPDATA
#define CSIDL_APPDATA 0x001A
#endif

BOOL SHGetSpecialFolderPathA (HWND hwnd, LPSTR pszPath, int csidl, BOOL fCreate);

#ifdef __cplusplus
}
#endif

#endif
