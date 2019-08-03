#ifndef __SYNCHAPI_H__
#define __SYNCHAPI_H__

#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

VOID InitializeCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
VOID DeleteCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
VOID EnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
BOOL TryEnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
VOID LeaveCriticalSection (LPCRITICAL_SECTION lpCriticalSection);

#define WAIT_IO_COMPLETION 0x000000C0L

VOID Sleep (DWORD dwMilliseconds);
DWORD SleepEx (DWORD dwMilliseconds, BOOL bAlertable);

#ifdef __cplusplus
}
#endif

#endif
