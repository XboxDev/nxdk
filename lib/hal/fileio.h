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
