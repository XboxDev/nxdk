// ******************************************************************
// * 
// * proj : OpenXDK
// *
// * desc : Open Source XBox Development Kit
// *
// * file : mm.h
// *
// * note : XBox Kernel *Memory Manager* Declarations
// *
// ******************************************************************
#ifndef XBOXKRNL_MM_H
#define XBOXKRNL_MM_H

#include "xboxkrnl.h"

XBSYSAPI VOID *MmGlobalData;

// ******************************************************************
// * MmAllocateContiguousMemory
// ******************************************************************
// *
// * Allocates a range of physically contiguous, cache-aligned 
// * memory from nonpaged pool (main pool on xbox).
// *
// ******************************************************************
XBSYSAPI EXPORTNUM(165) PVOID NTAPI MmAllocateContiguousMemory
(
	IN ULONG NumberOfBytes
);

// ******************************************************************
// * MmAllocateContiguousMemoryEx
// ******************************************************************
XBSYSAPI EXPORTNUM(166) PVOID NTAPI MmAllocateContiguousMemoryEx
(
	IN ULONG			NumberOfBytes,
	IN PHYSICAL_ADDRESS LowestAcceptableAddress,
	IN PHYSICAL_ADDRESS HighestAcceptableAddress,
	IN ULONG			Alignment OPTIONAL,
	IN ULONG			ProtectionType
);

// ******************************************************************
// * MmAllocateSystemMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(167) PVOID NTAPI MmAllocateSystemMemory
(
    ULONG NumberOfBytes,
    ULONG Protect
);

// ******************************************************************
// * MmClaimGpuInstanceMemory;
// ******************************************************************
XBSYSAPI EXPORTNUM(168) PVOID NTAPI MmClaimGpuInstanceMemory
(
    ULONG Mask,
    IN PVOID pBaseAddr
);

XBSYSAPI VOID *MmCreateKernelStack;
XBSYSAPI VOID *MmDeleteKernelStack;

// ******************************************************************
// * MmFreeContiguousMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(171) VOID NTAPI MmFreeContiguousMemory
(
	IN PVOID BaseAddress
);

// ******************************************************************
// * MmFreeSystemMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(172) NTSTATUS NTAPI MmFreeSystemMemory
(
    PVOID BaseAddress,
    ULONG NumberOfBytes
);

// ******************************************************************
// * MmGetPhysicalAddress
// ******************************************************************
XBSYSAPI EXPORTNUM(173) PHYSICAL_ADDRESS NTAPI MmGetPhysicalAddress
(
    IN PVOID   BaseAddress
);

XBSYSAPI VOID *MmIsAddressValid;

//XBSYSAPI VOID *MmLockUnlockBufferPages;
XBSYSAPI EXPORTNUM(175) PHYSICAL_ADDRESS NTAPI MmLockUnlockBufferPages
(
    IN PHYSICAL_ADDRESS   BaseAddress,
	IN ULONG            NumberOfBytes,
    IN ULONG            Protect
);


XBSYSAPI VOID *MmLockUnlockPhysicalPage;

// ******************************************************************
// * MmMapIoSpace
// ******************************************************************
XBSYSAPI EXPORTNUM(177) PVOID NTAPI MmMapIoSpace
(
    IN PHYSICAL_ADDRESS PhysicalAddress,
    IN ULONG            NumberOfBytes,
    IN ULONG            Protect
);

// ******************************************************************
// * MmPersistContiguousMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(178) VOID NTAPI MmPersistContiguousMemory
(
    IN PVOID   BaseAddress,
    IN ULONG   NumberOfBytes,
    IN BOOLEAN Persist
);

XBSYSAPI VOID *MmQueryAddressProtect;

// ******************************************************************
// * MmQueryAllocationSize
// ******************************************************************
XBSYSAPI EXPORTNUM(180) ULONG NTAPI MmQueryAllocationSize
(
    IN PVOID   BaseAddress
);

// ******************************************************************
// * MmQueryStatistics
// ******************************************************************
XBSYSAPI EXPORTNUM(181) NTSTATUS NTAPI MmQueryStatistics
(
    OUT PMM_STATISTICS MemoryStatistics
);

// ******************************************************************
// * MmSetAddressProtect
// ******************************************************************
XBSYSAPI EXPORTNUM(182) VOID NTAPI MmSetAddressProtect
(
    IN PVOID BaseAddress,
    IN ULONG NumberOfBytes,
    IN ULONG NewProtect
);

XBSYSAPI VOID *MmUnmapIoSpace;

#endif



