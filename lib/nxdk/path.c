// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include "path.h"
#include <string.h>
#include <xboxkrnl/xboxkrnl.h>

void nxGetCurrentXbeNtPath (char *path)
{
    // Retrieve the XBE path
    strncpy(path, XeImageFileName->Buffer, XeImageFileName->Length);
    path[XeImageFileName->Length] = '\0';
}
