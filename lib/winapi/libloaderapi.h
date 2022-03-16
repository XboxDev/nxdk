// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Jannik Vogel

#ifndef __LIBLOADERAPI_H__
#define __LIBLOADERAPI_H__

#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

HMODULE LoadLibraryExA (LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
HMODULE LoadLibraryA (LPCSTR lpLibFileName);
BOOL FreeLibrary (HMODULE hLibModule);
FARPROC GetProcAddress (HMODULE hModule, LPCSTR lpProcName);

#ifndef UNICODE
#define LoadLibraryEx LoadLibraryExA
#define LoadLibrary LoadLibraryA
#else
#error nxdk does not support the Unicode API
#endif

#ifdef __cplusplus
}
#endif

#endif
