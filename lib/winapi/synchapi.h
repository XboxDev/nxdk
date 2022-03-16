// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2020 Stefan Schmidt
// SPDX-FileCopyrightText: 2020 Jannik Vogel

#ifndef __SYNCHAPI_H__
#define __SYNCHAPI_H__

#include <stdint.h>
#include <windef.h>
#include <winbase.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;

VOID InitializeCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
BOOL InitializeCriticalSectionAndSpinCount (LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount);
VOID DeleteCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
VOID EnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
BOOL TryEnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
VOID LeaveCriticalSection (LPCRITICAL_SECTION lpCriticalSection);

VOID InitializeConditionVariable (PCONDITION_VARIABLE ConditionVariable);
BOOL SleepConditionVariableCS (PCONDITION_VARIABLE ConditionVariable, PCRITICAL_SECTION CriticalSection, DWORD dwMilliseconds);
BOOL SleepConditionVariableSRW (PCONDITION_VARIABLE ConditionVariable, PSRWLOCK SRWLock, DWORD dwMilliseconds, ULONG Flags);
VOID WakeConditionVariable (PCONDITION_VARIABLE ConditionVariable);
VOID WakeAllConditionVariable (PCONDITION_VARIABLE ConditionVariable);
// UninitializeConditionVariable is an nxdk extension to free associated system resources
VOID UninitializeConditionVariable (PCONDITION_VARIABLE ConditionVariable);


void AcquireSRWLockExclusive (PSRWLOCK SRWLock);
void AcquireSRWLockShared (PSRWLOCK SRWLock);
void InitializeSRWLock (PSRWLOCK SRWLock);
void ReleaseSRWLockExclusive (PSRWLOCK SRWLock);
void ReleaseSRWLockShared (PSRWLOCK SRWLock);
BOOLEAN TryAcquireSRWLockExclusive (PSRWLOCK SRWLock);
BOOLEAN TryAcquireSRWLockShared (PSRWLOCK SRWLock);

BOOL InitOnceExecuteOnce (PINIT_ONCE InitOnce, PINIT_ONCE_FN InitFn, PVOID Context, LPVOID *Parameter);
BOOL InitOnceBeginInitialize (LPINIT_ONCE lpInitOnce, DWORD dwFlags, PBOOL fPending, LPVOID *lpContext);
BOOL InitOnceComplete (LPINIT_ONCE lpInitOnce, DWORD dwFlags, LPVOID lpContext);

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
