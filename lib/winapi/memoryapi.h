// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#ifndef __MEMORYAPI_H__
#define __MEMORYAPI_H__

#include <windef.h>
#include <xboxkrnl/xboxkrnl.h>

#ifdef __cplusplus
extern "C" {
#endif

LPVOID VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
BOOL VirtualFree (LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
SIZE_T VirtualQuery (LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);

#ifdef __cplusplus
}
#endif

#endif
