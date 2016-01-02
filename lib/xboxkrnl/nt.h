// ******************************************************************
// * 
// * proj : OpenXDK
// *
// * desc : Open Source XBox Development Kit
// *
// * file : nt.h
// *
// * note : XBox Kernel *NT* Declarations
// *
// ******************************************************************
#ifndef XBOXKRNL_NT_H
#define XBOXKRNL_NT_H

#include "xboxkrnl.h"

// ******************************************************************
// * NtAllocateVirtualMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(184) NTSTATUS NTAPI NtAllocateVirtualMemory
(
    IN OUT PVOID    *BaseAddress,
    IN ULONG         ZeroBits,
    IN OUT PULONG    AllocationSize,
    IN DWORD         AllocationType,
    IN DWORD         Protect
);

XBSYSAPI VOID *NtCancelTimer;

// ******************************************************************
// * NtClearEvent
// ******************************************************************
XBSYSAPI EXPORTNUM(186) NTSTATUS NTAPI NtClearEvent
(
    IN HANDLE EventHandle
);

// ******************************************************************
// * NtClose
// ******************************************************************
// *
// * Closes an object handle
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(187) NTSTATUS NTAPI NtClose
(
	IN HANDLE Handle
);

XBSYSAPI VOID *NtCreateDirectoryObject;

// ******************************************************************
// * NtCreateEvent
// ******************************************************************
XBSYSAPI EXPORTNUM(189) NTSTATUS NTAPI NtCreateEvent
(
    OUT PHANDLE             EventHandle,
    IN  POBJECT_ATTRIBUTES  ObjectAttributes OPTIONAL,
    IN  EVENT_TYPE          EventType,
    IN  BOOLEAN             InitialState
);

// ******************************************************************
// * NtCreateFile
// ******************************************************************
// *
// * Causes a new file or directory to be created, or opens and
// * existing file, device, directory, or volume, giving the caller
// * a handle for the file object.
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(190) NTSTATUS NTAPI NtCreateFile
(
	OUT PHANDLE             FileHandle, 
	IN  ACCESS_MASK         DesiredAccess,
	IN  POBJECT_ATTRIBUTES	ObjectAttributes,
	OUT PIO_STATUS_BLOCK	IoStatusBlock,
	IN  PLARGE_INTEGER	    AllocationSize OPTIONAL, 
	IN  ULONG	            FileAttributes, 
	IN  ULONG	            ShareAccess, 
	IN  ULONG	            CreateDisposition, 
	IN  ULONG	            CreateOptions 
);

XBSYSAPI VOID *NtCreateIoCompletion;

// ******************************************************************
// * NtCreateMutant
// ******************************************************************
XBSYSAPI EXPORTNUM(192) NTSTATUS NTAPI NtCreateMutant
(
    OUT PHANDLE             MutantHandle,
    IN  POBJECT_ATTRIBUTES  ObjectAttributes,
    IN  BOOLEAN             InitialOwner
);

XBSYSAPI EXPORTNUM(193) NTSTATUS NTAPI NtCreateSemaphore
(
    OUT PHANDLE           SemaphoreHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN LONG               InitialCount,
    IN LONG               MaximumCount
);

XBSYSAPI VOID *NtCreateTimer;
XBSYSAPI VOID *NtDeleteFile;
XBSYSAPI VOID *NtDeviceIoControlFile;

// ******************************************************************
// * NtDuplicateObject
// ******************************************************************
XBSYSAPI EXPORTNUM(197) NTSTATUS NTAPI NtDuplicateObject
(
    PVOID   SourceHandle,
    PVOID  *TargetHandle,
    DWORD   Options
);

// ******************************************************************
// * NtFlushBuffersFile
// ******************************************************************
XBSYSAPI EXPORTNUM(198) NTSTATUS NTAPI NtFlushBuffersFile
(
    PVOID                FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock
);

// ******************************************************************
// * NtFreeVirtualMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(199) NTSTATUS NTAPI NtFreeVirtualMemory
(
    IN OUT PVOID *BaseAddress,
    IN OUT PULONG FreeSize,
    IN ULONG      FreeType
);

XBSYSAPI VOID *NtFsControlFile;
XBSYSAPI VOID *NtOpenDirectoryObject;

// ******************************************************************
// * NtOpenFile
// ******************************************************************
XBSYSAPI EXPORTNUM(202) NTSTATUS NTAPI NtOpenFile
(
    OUT PHANDLE             FileHandle,
    IN  ACCESS_MASK         DesiredAccess,
    IN  POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    IN  ULONG               ShareAccess,
    IN  ULONG               OpenOptions
);

XBSYSAPI VOID *NtOpenSymbolicLinkObject;
XBSYSAPI VOID *NtProtectVirtualMemory;

// ******************************************************************
// * NtPulseEvent
// ******************************************************************
XBSYSAPI EXPORTNUM(205) NTSTATUS NTAPI NtPulseEvent
(
    IN HANDLE                       EventHandle,
    OUT PLONG                       PreviousState OPTIONAL
);

XBSYSAPI VOID *NtQueueApcThread;

// ******************************************************************
// * NtQueryDirectoryFile
// ******************************************************************
XBSYSAPI EXPORTNUM(207) NTSTATUS NTAPI NtQueryDirectoryFile
(
    IN  HANDLE                      FileHandle,
    IN  HANDLE                      Event OPTIONAL,
    IN  PVOID                       ApcRoutine, // Todo: define this routine's prototype
    IN  PVOID                       ApcContext,
    OUT PIO_STATUS_BLOCK            IoStatusBlock,
    OUT PVOID                       FileInformation,
    IN  ULONG                       Length,
    IN  FILE_INFORMATION_CLASS      FileInformationClass,
    IN  PSTRING                     FileMask,
    IN  BOOLEAN                     RestartScan
);

XBSYSAPI VOID *NtQueryDirectoryObject;
XBSYSAPI VOID *NtQueryEvent;

// ******************************************************************
// * NtQueryFullAttributesFile
// ******************************************************************
XBSYSAPI EXPORTNUM(210) NTSTATUS NTAPI NtQueryFullAttributesFile
(   
    IN  POBJECT_ATTRIBUTES          ObjectAttributes,
    OUT PVOID                       Attributes
);

// ******************************************************************
// * NtQueryInformationFile
// ******************************************************************
// *
// * Return various kinds of information about a given file object.
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(211) NTSTATUS NTAPI NtQueryInformationFile
(   
    IN  HANDLE                      FileHandle,
    OUT PIO_STATUS_BLOCK            IoStatusBlock,
    OUT PVOID                       FileInformation, 
    IN  ULONG                       Length, 
    IN  FILE_INFORMATION_CLASS      FileInfo
);

XBSYSAPI VOID *NtQueryIoCompletion;
XBSYSAPI VOID *NtQueryMutant;
XBSYSAPI VOID *NtQuerySemaphore;
XBSYSAPI VOID *NtQuerySymbolicLinkObject;
XBSYSAPI VOID *NtQueryTimer;

// ******************************************************************
// * NtQueryVirtualMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(217) NTSTATUS NTAPI NtQueryVirtualMemory
(
    IN  PVOID                       BaseAddress,
    OUT PMEMORY_BASIC_INFORMATION   MemoryInformation
);

// ******************************************************************
// * NtQueryVolumeInformationFile
// ******************************************************************
XBSYSAPI EXPORTNUM(218) NTSTATUS NTAPI NtQueryVolumeInformationFile
(
    IN  HANDLE                      FileHandle,
    OUT PIO_STATUS_BLOCK            IoStatusBlock,
    OUT PFILE_FS_SIZE_INFORMATION   FileInformation,
    IN  ULONG                       Length,
    IN  FS_INFORMATION_CLASS        FileInformationClass
);

// ******************************************************************
// * NtReadFile
// ******************************************************************
// *
// * Read data from an opened file.
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(219) NTSTATUS NTAPI NtReadFile
(
	IN  HANDLE          FileHandle,            // TODO: correct paramters
	IN  HANDLE          Event OPTIONAL,
	IN  PVOID           ApcRoutine OPTIONAL,
	IN  PVOID           ApcContext,
	OUT PVOID           IoStatusBlock,
	OUT PVOID           Buffer,
	IN  ULONG           Length,
	IN  PLARGE_INTEGER  ByteOffset OPTIONAL
);

XBSYSAPI VOID *NtReadFileScatter;

// ******************************************************************
// * NtReleaseMutant
// ******************************************************************
XBSYSAPI EXPORTNUM(221) NTSTATUS NTAPI NtReleaseMutant
(
    IN  HANDLE  MutantHandle,
    OUT PLONG   PreviousCount
);

XBSYSAPI EXPORTNUM(222) NTSTATUS NTAPI NtReleaseSemaphore
(
    IN HANDLE   SemaphoreHandle,
    IN ULONG    ReleaseCount,
    OUT PULONG  PreviousCount OPTIONAL
);

XBSYSAPI VOID *NtRemoveIoCompletion;

// ******************************************************************
// * NtResumeThread
// ******************************************************************
XBSYSAPI EXPORTNUM(224) NTSTATUS NTAPI NtResumeThread
(
    IN  HANDLE ThreadHandle,
    OUT PULONG PreviousSuspendCount
);

// ******************************************************************
// * NtSetEvent
// ******************************************************************
XBSYSAPI EXPORTNUM(225) NTSTATUS NTAPI NtSetEvent
(
    IN  HANDLE  EventHandle,
    OUT PLONG   PreviousState
);

// ******************************************************************
// * NtSetInformationFile
// ******************************************************************
// *
// * Changes various kinds of information about a given file object.
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(226) NTSTATUS NTAPI NtSetInformationFile
(	
	IN  HANDLE  FileHandle,            // TODO: correct paramters
	OUT	PVOID	IoStatusBlock,
	IN	PVOID	FileInformation,
	IN	ULONG	Length,
	IN	ULONG	FileInformationClass
);

XBSYSAPI VOID *NtSetIoCompletion;
XBSYSAPI VOID *NtSetSystemTime;
XBSYSAPI VOID *NtSetTimerEx;
XBSYSAPI VOID *NtSignalAndWaitForSingleObjectEx;

// ******************************************************************
// * NtSuspendThread
// ******************************************************************
XBSYSAPI EXPORTNUM(231) NTSTATUS NTAPI NtSuspendThread
(
    IN  HANDLE  ThreadHandle,
    OUT PULONG  PreviousSuspendCount OPTIONAL
);

// ******************************************************************
// * NtUserIoApcDispatcher
// ******************************************************************
XBSYSAPI EXPORTNUM(232) VOID NTAPI NtUserIoApcDispatcher
(
    PVOID            ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG            Reserved
);

// ******************************************************************
// * NtWaitForSingleObject
// ******************************************************************
// *
// * Put the current thread into a wait state until the given 
// * dispatcher object is set to a signaled state or (optionally) 
// * until the wait times out.
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(233) NTSTATUS NTAPI NtWaitForSingleObject
(	
	IN  HANDLE         Handle,
	IN	BOOLEAN	       Alertable,
	IN	PLARGE_INTEGER Timeout
);

// ******************************************************************
// * NtWaitForSingleObjectEx
// ******************************************************************
XBSYSAPI EXPORTNUM(234) NTSTATUS NTAPI NtWaitForSingleObjectEx
(
    IN  HANDLE          Handle,
    IN  CHAR            WaitMode,
    IN  BOOLEAN         Alertable,
    IN  PLARGE_INTEGER  Timeout
);

// ******************************************************************
// * NtWaitForMultipleObjectsEx
// ******************************************************************
XBSYSAPI EXPORTNUM(235) NTSTATUS NTAPI NtWaitForMultipleObjectsEx
(
    IN  ULONG           Count,
    IN  HANDLE         *Handles,
    IN  WAIT_TYPE       WaitType,
    IN  CHAR            WaitMode,
    IN  BOOLEAN         Alertable,
    IN  PLARGE_INTEGER  Timeout
);

// ******************************************************************
// * NtWriteFile
// ******************************************************************
// *
// * Write data to an opened file.
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(236) NTSTATUS NTAPI NtWriteFile
(	
	IN  HANDLE          FileHandle,            // TODO: correct paramters
	IN	PVOID	        Event,
	IN	PVOID	        ApcRoutine,
	IN	PVOID	        ApcContext,
	OUT	PVOID	        IoStatusBlock,
	IN	PVOID	        Buffer,
	IN	ULONG	        Length,
	IN	PLARGE_INTEGER  ByteOffset
);

XBSYSAPI VOID *NtWriteFileGather;

// ******************************************************************
// * NtYieldExecution
// ******************************************************************
XBSYSAPI EXPORTNUM(238) VOID NTAPI NtYieldExecution(void);

#endif


