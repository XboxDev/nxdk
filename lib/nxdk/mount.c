/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#include "mount.h"
#include <stdio.h>
#include <winbase.h>
#include <xboxkrnl/xboxkrnl.h>

bool nxIsDriveMounted (char driveLetter)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING drivePath;
    CHAR drivePathBuffer[7];
    OBJECT_ATTRIBUTES objattr;

    sprintf(drivePathBuffer, "\\??\\%c:", driveLetter);
    RtlInitAnsiString(&drivePath, drivePathBuffer);

    InitializeObjectAttributes(&objattr, &drivePath, OBJ_CASE_INSENSITIVE, NULL, NULL);
    status = NtOpenSymbolicLinkObject(&handle, &objattr);
    if (NT_SUCCESS(status)) {
        NtClose(handle);
        return true;
    }

    return false;
}

bool nxMountDrive (char driveLetter, const char *path)
{
    NTSTATUS status;
    ANSI_STRING drivePath;
    CHAR drivePathBuffer[7];
    ANSI_STRING mountPath;

    sprintf(drivePathBuffer, "\\??\\%c:", driveLetter);
    RtlInitAnsiString(&drivePath, drivePathBuffer);

    RtlInitAnsiString(&mountPath, path);

    status = IoCreateSymbolicLink(&drivePath, &mountPath);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return false;
    }

    return true;
}

bool nxUnmountDrive (char driveLetter)
{
    NTSTATUS status;
    ANSI_STRING drivePath;
    CHAR drivePathBuffer[7];

    sprintf(drivePathBuffer, "\\??\\%c:", driveLetter);
    RtlInitAnsiString(&drivePath, drivePathBuffer);

    status = IoDeleteSymbolicLink(&drivePath);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return true;
    }

    return true;
}
