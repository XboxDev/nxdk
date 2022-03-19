// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2020 Stefan Schmidt

#ifndef __NXDK_MOUNT_H__
#define __NXDK_MOUNT_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#include <stdbool.h>
#endif

bool nxIsDriveMounted (char driveLetter);
bool nxMountDrive (char driveLetter, const char *path);
bool nxUnmountDrive (char driveLetter);

#ifdef __cplusplus
}
#endif

#endif
