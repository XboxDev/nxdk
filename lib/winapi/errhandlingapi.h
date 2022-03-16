// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#ifndef __ERRHANDLINGAPI_H__
#define __ERRHANDLINGAPI_H__

#include <winbase.h>

#ifdef __cplusplus
extern "C"
{
#endif

VOID RaiseException (DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR *lpArguments);

#ifdef __cplusplus
}
#endif

#endif
