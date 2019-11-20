/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#ifndef __NXDK_MOUNT_H__
#define __NXDK_MOUNT_H__

#ifdef __cplusplus
{
#endif

#ifndef __cplusplus
#include <stdbool.h>
#endif

bool nxIsDriveMounted (char driveLetter);
bool nxMountDrive (char driveLetter, char *path);
bool nxUnmountDrive (char driveLetter);

#ifdef __cplusplus
}
#endif

#endif
