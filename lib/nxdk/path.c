/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#include "path.h"
#include <string.h>
#include <xboxkrnl/xboxkrnl.h>

void nxGetCurrentXbeNtPath (char *path)
{
    // Retrieve the XBE path
    strncpy(path, XeImageFileName->Buffer, XeImageFileName->Length);
    path[XeImageFileName->Length] = '\0';
}
