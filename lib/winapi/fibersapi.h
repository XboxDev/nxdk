// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#ifndef __FIBERSAPI_H__
#define __FIBERSAPI_H__

#include <winbase.h>

#ifdef __cplusplus
extern "C"
{
#endif

DWORD FlsAlloc (PFLS_CALLBACK_FUNCTION lpCallback);
BOOL FlsFree (DWORD dwFlsIndex);
PVOID FlsGetValue (DWORD dwFlsIndex);
BOOL FlsSetValue (DWORD dwFlsIndex, PVOID lpFlsData);

#ifdef __cplusplus
}
#endif

#endif
