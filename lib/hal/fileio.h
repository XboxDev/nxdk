// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2004 Craig Edwards
// SPDX-FileCopyrightText: 2017-2020 Stefan Schmidt

#ifndef HAL_FILEIO_H
#define HAL_FILEIO_H

#include "xboxkrnl/xboxkrnl.h"
#include "winerror.h"
#include <winbase.h>

#if defined(__cplusplus)
extern "C"
{
#endif

int XConvertDOSFilenameToXBOX(
	const char *dosFilename,
	char *xboxFilename);

#ifdef __cplusplus
}
#endif

#endif
