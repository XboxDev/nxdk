#ifndef __PROFILEAPI_H__
#define __PROFILEAPI_H__

#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL QueryPerformanceCounter (LARGE_INTEGER *lpPerformanceCount);
BOOL QueryPerformanceFrequency (LARGE_INTEGER *lpFrequency);

#ifdef __cplusplus
}
#endif

#endif
