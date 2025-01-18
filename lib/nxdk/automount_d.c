// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2022 Stefan Schmidt

#include <nxdk/mount.h>
#include <nxdk/path.h>
#include <assert.h>
#include <string.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>

__cdecl int automount_d_drive (void)
{
    if (nxIsDriveMounted('D')) {
        return 0;
    }

    // D: doesn't exist yet, so we create it
    CHAR targetPath[MAX_PATH];
    nxGetCurrentXbeNtPath(targetPath);

    // Cut off the XBE file name by inserting a null-terminator
    char *filenameStr;
    filenameStr = strrchr(targetPath, '\\');
    assert(filenameStr != NULL);
    *(filenameStr + 1) = '\0';

    // Mount the obtained path as D:
    BOOL success;
    success = nxMountDrive('D', targetPath);
    assert(success);

    return 0;
}
__attribute__((section(".CRT$XIT"), used)) int (__cdecl *const automount_d_drive_p)(void) = automount_d_drive;
