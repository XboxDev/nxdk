// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned (__stdcall *_beginthreadex_proc_type)(void *);

uintptr_t __cdecl _beginthreadex (void *_Security, unsigned _StackSize, _beginthreadex_proc_type _StartAddress, void *_ArgList, unsigned _InitFlag, unsigned *_ThrdAddr);
void __cdecl _endthreadex (unsigned _ReturnCode);

#ifdef __cplusplus
}
#endif

#endif
