// ******************************************************************
// * 
// * proj : OpenXDK
// *
// * desc : Open Source XBox Development Kit
// *
// * file : ex.h
// *
// * note : XBox Kernel *Executive* Declarations
// *
// ******************************************************************
#ifndef XBOXKRNL_EX_H
#define XBOXKRNL_EX_H

#include "xboxkrnl.h"

XBSYSAPI VOID *ExAcquireReadWriteLockExclusive;
XBSYSAPI VOID *ExAcquireReadWriteLockShared;

// ******************************************************************
// * ExAllocatePool
// ******************************************************************
// *
// * Allocates pool memory
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(14) PVOID NTAPI ExAllocatePool
(
	IN ULONG NumberOfBytes
);

XBSYSAPI VOID *ExAllocatePoolWithTag;
XBSYSAPI VOID *ExEventObjectType;

// ******************************************************************
// * ExFreePool
// ******************************************************************
// *
// * Deallocates a block  of pool memory
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(17) PVOID NTAPI ExFreePool
(
    IN PVOID P
);

XBSYSAPI VOID *ExInitializeReadWriteLock;
XBSYSAPI VOID *ExInterlockedAddLargeInteger;
XBSYSAPI VOID *ExInterlockedAddLargeStatistic;
XBSYSAPI VOID *ExInterlockedCompareExchange64;
XBSYSAPI VOID *ExMutantObjectType;
XBSYSAPI VOID *ExQueryPoolBlockSize;

// ******************************************************************
// * ExQueryNonVolatileSetting
// ******************************************************************
XBSYSAPI EXPORTNUM(24) NTSTATUS NTAPI ExQueryNonVolatileSetting
(
	IN  DWORD               ValueIndex,
	OUT DWORD              *Type,
	OUT PUCHAR              Value,
	IN  SIZE_T              ValueLength,
	OUT PSIZE_T             ResultLength OPTIONAL
);

XBSYSAPI VOID *ExReadWriteRefurbInfo;
XBSYSAPI VOID *ExRaiseException;
XBSYSAPI VOID *ExRaiseStatus;
XBSYSAPI VOID *ExReleaseReadWriteLock;

// ******************************************************************
// * ExSaveNonVolatileSetting
// ******************************************************************
XBSYSAPI EXPORTNUM(29) NTSTATUS NTAPI ExSaveNonVolatileSetting
(
    IN  DWORD               ValueIndex,
    OUT DWORD              *Type,
    IN  PUCHAR              Value,
    IN  SIZE_T              ValueLength
);

XBSYSAPI VOID *ExSemaphoreObjectType;
XBSYSAPI VOID *ExTimerObjectType;
XBSYSAPI VOID *ExfInterlockedInsertHeadList;
XBSYSAPI VOID *ExfInterlockedInsertTailList;
XBSYSAPI VOID *ExfInterlockedRemoveHeadList;
XBSYSAPI VOID *InterlockedCompareExchange;
XBSYSAPI VOID *InterlockedDecrement;
XBSYSAPI VOID *InterlockedIncrement;
XBSYSAPI VOID *InterlockedExchange;
XBSYSAPI VOID *InterlockedExchangeAdd;
XBSYSAPI VOID *InterlockedFlushSList;
XBSYSAPI VOID *InterlockedPopEntrySList;
XBSYSAPI VOID *InterlockedPushEntrySList;

#endif


