// ******************************************************************
// * 
// * proj : OpenXDK
// *
// * desc : Open Source XBox Development Kit
// *
// * file : io.h
// *
// * note : XBox Kernel *I/O Manager* Declarations
// *
// ******************************************************************
#ifndef XBOXKRNL_IO_H
#define XBOXKRNL_IO_H

#include "xboxkrnl.h"

XBSYSAPI VOID *IoAllocateIrp;
XBSYSAPI VOID *IoBuildAsynchronousFsdRequest;
XBSYSAPI VOID *IoBuildDeviceIoControlRequest;
XBSYSAPI VOID *IoBuildSynchronousFsdRequest;
XBSYSAPI VOID *IoCheckShareAccess;
XBSYSAPI VOID *IoCompletionObjectType;
XBSYSAPI VOID *IoCreateDevice;

// ******************************************************************
// * IoCreateFile
// ******************************************************************
XBSYSAPI EXPORTNUM(66) NTSTATUS NTAPI IoCreateFile
(
    OUT PHANDLE             FileHandle,
    IN  ACCESS_MASK         DesiredAccess,
    IN  POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    IN  PLARGE_INTEGER      AllocationSize,
    IN  ULONG               FileAttributes,
    IN  ULONG               ShareAccess,
    IN  ULONG               Disposition,
    IN  ULONG               CreateOptions,
    IN  ULONG               Options
);

// ******************************************************************
// * IoCreateSymbolicLink
// ******************************************************************
XBSYSAPI EXPORTNUM(67) NTSTATUS NTAPI IoCreateSymbolicLink
(
    IN PSTRING SymbolicLinkName,
    IN PSTRING DeviceName
);

XBSYSAPI VOID *IoDeleteDevice;

// ******************************************************************
// * IoDeleteSymbolicLink
// ******************************************************************
XBSYSAPI EXPORTNUM(69) NTSTATUS NTAPI IoDeleteSymbolicLink
(
    IN PSTRING SymbolicLinkName
);

XBSYSAPI VOID *IoDeviceObjectType;
XBSYSAPI VOID *IoFileObjectType;
XBSYSAPI VOID *IoFreeIrp;
XBSYSAPI VOID *IoInitializeIrp;
XBSYSAPI VOID *IoInvalidDeviceRequest;
XBSYSAPI VOID *IoQueryFileInformation;
XBSYSAPI VOID *IoQueryVolumeInformation;
XBSYSAPI VOID *IoQueueThreadIrp;
XBSYSAPI VOID *IoRemoveShareAccess;
XBSYSAPI VOID *IoSetIoCompletion;
XBSYSAPI VOID *IoSetShareAccess;
XBSYSAPI VOID *IoStartNextPacket;
XBSYSAPI VOID *IoStartNextPacketByKey;
XBSYSAPI VOID *IoStartPacket;
XBSYSAPI VOID *IoSynchronousDeviceIoControlRequest;
XBSYSAPI VOID *IoSynchronousFsdRequest;
XBSYSAPI VOID *IofCallDriver;
XBSYSAPI VOID *IofCompleteRequest;
XBSYSAPI VOID *IoDismountVolume;
XBSYSAPI VOID *IoDismountVolumeByName;
XBSYSAPI VOID *IoMarkIrpMustComplete;

#endif


