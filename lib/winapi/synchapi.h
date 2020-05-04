#ifndef __SYNCHAPI_H__
#define __SYNCHAPI_H__

#include <windef.h>
#include <winbase.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

VOID InitializeCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
BOOL InitializeCriticalSectionAndSpinCount (LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount);
VOID DeleteCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
VOID EnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
BOOL TryEnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
VOID LeaveCriticalSection (LPCRITICAL_SECTION lpCriticalSection);

#define WAIT_ABANDONED ((DWORD)0x00000080L) // same as STATUS_ABANDONED_WAIT_0
#define WAIT_IO_COMPLETION ((DWORD)0x000000C0L) // same as STATUS_USER_APC
#define WAIT_OBJECT_0 ((DWORD)0x00000000L) // same as STATUS_WAIT_0
#define WAIT_TIMEOUT ((DWORD)0x00000102L) // same as STATUS_TIMEOUT
#define WAIT_FAILED ((DWORD)0xFFFFFFFFL)

VOID Sleep (DWORD dwMilliseconds);
DWORD SleepEx (DWORD dwMilliseconds, BOOL bAlertable);

DWORD WaitForSingleObjectEx (HANDLE hHandle, DWORD dwMilliseconds, BOOL bAlertable);
DWORD WaitForSingleObject (HANDLE hHandle, DWORD dwMilliseconds);
DWORD WaitForMultipleObjectsEx (DWORD nCount, const HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds, BOOL bAlertable);
DWORD WaitForMultipleObjects (DWORD nCount, const HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds);

HANDLE CreateSemaphore (LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName);
BOOL ReleaseSemaphore (HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount);

#ifdef __cplusplus
}
#endif

#endif
