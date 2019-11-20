/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#include <nxdk/mount.h>
#include <nxdk/path.h>
#include <assert.h>
#include <string.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>

__attribute__((constructor)) void automount_d_drive (void)
{
    if (nxIsDriveMounted('D')) {
        return;
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
}
