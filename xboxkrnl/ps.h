// ******************************************************************
// * 
// * proj : OpenXDK
// *
// * desc : Open Source XBox Development Kit
// *
// * file : ps.h
// *
// * note : XBox Kernel *Process Structure* Declarations
// *
// ******************************************************************
#ifndef XBOXKRNL_PS_H
#define XBOXKRNL_PS_H

#include "xboxkrnl.h"

//XBSYSAPI VOID *PsCreateSystemThread;

XBSYSAPI EXPORTNUM(254) NTSTATUS NTAPI PsCreateSystemThread
(
   PULONG lpThreadAttributes,               // SD
   DWORD dwStackSize,                       // initial stack size
   PKSTART_ROUTINE lpStartAddress,          // thread function
   VOID* lpParameter,                       // thread argument
   DWORD dwCreationFlags,                   // creation option
   DWORD* lpThreadId                        // thread identifier

);

// ******************************************************************
// * PsCreateSystemThreadEx
// ******************************************************************
XBSYSAPI EXPORTNUM(255) NTSTATUS NTAPI PsCreateSystemThreadEx
(
    OUT PHANDLE         ThreadHandle,
    IN  ULONG           ThreadExtraSize,
    IN  ULONG           KernelStackSize,
    IN  ULONG           TlsDataSize,
    OUT PULONG          ThreadId OPTIONAL,
    IN  PVOID           StartContext1,
    IN  PVOID           StartContext2,
    IN  BOOLEAN         CreateSuspended,
    IN  BOOLEAN         DebugStack,
    IN  PKSTART_ROUTINE StartRoutine
);

XBSYSAPI VOID *PsQueryStatistics;
XBSYSAPI VOID *PsSetCreateThreadNotifyRoutine;

// ******************************************************************
// * PsTerminateSystemThread
// ******************************************************************
XBSYSAPI EXPORTNUM(258) VOID NTAPI PsTerminateSystemThread(IN NTSTATUS ExitStatus);

//XBSYSAPI VOID *PsThreadObjectType;
XBSYSAPI EXPORTNUM(259) volatile DWORD PsThreadObjectType;

#endif


