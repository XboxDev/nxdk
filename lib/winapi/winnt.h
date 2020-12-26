#ifndef __WINNT_H__
#define __WINNT_H__

#include <xboxkrnl/xboxdef.h>

typedef LONG HRESULT;

typedef CHAR *LPSTR;

typedef signed __int64 LONG64, *PLONG64;

LONG64 InterlockedExchange64 (LONG64 volatile *Target, LONG64 Value);
PVOID InterlockedExchangePointer (PVOID volatile *Target, PVOID Value);
PVOID InterlockedCompareExchangePointer (PVOID volatile *Destination, PVOID Exchange, PVOID Comperand);

#ifdef UNICODE
typedef LPCWSTR LPCTSTR;
#else
typedef LPCSTR LPCTSTR;
#endif

#endif
