// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2021 Stefan Schmidt

#ifndef __PROCESSTHREADSAPI_H__
#define __PROCESSTHREADSAPI_H__

#include <winbase.h>
#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef DWORD(__stdcall *LPTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);
HANDLE CreateThread (LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
VOID ExitThread (DWORD dwExitCode);
BOOL GetExitCodeThread (HANDLE hThread, LPDWORD lpExitCode);

HANDLE GetCurrentThread (VOID);
DWORD GetCurrentThreadId (VOID);
DWORD GetThreadId (HANDLE Thread);
BOOL SwitchToThread (VOID);

BOOL SetThreadPriority (HANDLE hThread, int nPriority);

DWORD TlsAlloc (void);
BOOL TlsFree (DWORD dwTlsIndex);
LPVOID TlsGetValue (DWORD dwTlsIndex);
BOOL TlsSetValue (DWORD dwTlsIndex, LPVOID lpTlsValue);

#ifdef __cplusplus
}
#endif

#endif
