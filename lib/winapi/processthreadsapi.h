#ifndef __PROCESSTHREADSAPI_H__
#define __PROCESSTHREADSAPI_H__

#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

HANDLE GetCurrentThread (VOID);
DWORD GetCurrentThreadId (VOID);
DWORD GetThreadId (HANDLE Thread);
BOOL SwitchToThread (VOID);

BOOL SetThreadPriority (HANDLE hThread, int nPriority);

#ifdef __cplusplus
}
#endif

#endif
