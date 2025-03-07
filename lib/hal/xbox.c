// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2004 Craig Edwards
// SPDX-FileCopyrightText: 2006 Richard Osborne
// SPDX-FileCopyrightText: 2017-2020 Stefan Schmidt
// SPDX-FileCopyrightText: 2022-2025 Erik Abair

#include <string.h>
// #include <xboxrt/stat.h>
#include <hal/xbox.h>
#include <hal/fileio.h>
#include <xboxkrnl/xboxkrnl.h>

#include <stdio.h>
#include <hal/debug.h>

#define KernelMode 0

#define LaunchDataPageSize 0x1000

static const char *xonline_dash_path = "\\Device\\Harddisk0\\Partition2\\XODash\\xonlinedash.xbe";

void XReboot()
{
    HalReturnToFirmware(HalRebootRoutine);
}

int XGetLaunchInfo(unsigned long *launchDataType, const unsigned char **launchData)
{
    *launchDataType = LDT_NONE;
    *launchData = NULL;

    if (LaunchDataPage == NULL) {
        LaunchDataPage = MmAllocateContiguousMemory(LaunchDataPageSize);
        if (LaunchDataPage == NULL) {
            return -1;
        }
        memset(LaunchDataPage, 0, LaunchDataPageSize);
        LaunchDataPage->Header.dwLaunchDataType = LDT_FROM_DASHBOARD;
    }

    *launchDataType = LaunchDataPage->Header.dwLaunchDataType;
    *launchData = LaunchDataPage->LaunchData;
    return 0;
}

void XLaunchXBE(const char *xbePath)
{
    XLaunchXBEEx(xbePath, NULL);
}

static BOOL DoesFileExist(const char *lpPathName)
{
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;

    RtlInitAnsiString(&path, lpPathName);
    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, NULL, NULL);

    HANDLE handle;
    IO_STATUS_BLOCK statusBlock;
    NTSTATUS status = NtOpenFile(&handle,
                                 GENERIC_READ,
                                 &objectAttributes,
                                 &statusBlock,
                                 FILE_SHARE_READ,
                                 FILE_NON_DIRECTORY_FILE);
    if (!NT_SUCCESS(status)) {
        return FALSE;
    }

    NtClose(handle);
    return TRUE;
}

void XLaunchXBEEx(const char *xbePath, const void *launchData)
{
    if (LaunchDataPage == NULL) {
        LaunchDataPage = MmAllocateContiguousMemory(LaunchDataPageSize);
        if (LaunchDataPage == NULL) {
            return;
        }
    }

    // For ease of debugging.
    PLAUNCH_DATA_PAGE launchDataPage = LaunchDataPage;

    MmPersistContiguousMemory(launchDataPage, LaunchDataPageSize, TRUE);
    memset((void*)launchDataPage, 0, LaunchDataPageSize);

    launchDataPage->Header.dwLaunchDataType = LDT_TITLE;
    launchDataPage->Header.dwTitleId = CURRENT_XBE_HEADER->CertificateHeader->TitleID;
    launchDataPage->Header.dwFlags = 0x0000;

    if (!xbePath) {
        launchDataPage->Header.dwLaunchDataType = LDT_LAUNCH_DASHBOARD;

        if (launchData && *(DWORD*)launchData >= LDT_LAUNCH_DASHBOARD_REASON_NETWORK) {
            xbePath = xonline_dash_path;
            if (!DoesFileExist(xbePath)) {
                xbePath = NULL;
            }
        }
    }

    if (xbePath) {
        XConvertDOSFilenameToXBOX(xbePath, launchDataPage->Header.szLaunchPath);

        // one last thing... xbePath now looks like:
        //   \Device\Harddisk0\Partition2\blah\doom.xbe
        // but it has to look like:
        //   \Device\Harddisk0\Partition2\blah;doom.xbe
        char *lastSlash = strrchr(launchDataPage->Header.szLaunchPath, '\\');
        if (!lastSlash) {
            // if we couldn't find a trailing slash, the conversion to
            // the xbox path mustn't have worked, so we will return
            return;
        }

        *lastSlash = ';';
    }

    if (launchData) {
        memcpy(launchDataPage->LaunchData, launchData, sizeof(launchDataPage->LaunchData));
    }

    HalReturnToFirmware(HalQuickRebootRoutine);
}
