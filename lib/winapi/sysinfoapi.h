#ifndef __SYSINFOAPI_H__
#define __SYSINFOAPI_H__

#include <minwinbase.h>

#ifdef __cplusplus
extern "C" {
#endif

void GetSystemTimePreciseAsFileTime (LPFILETIME lpSystemTimeAsFileTime);

#ifdef __cplusplus
}
#endif

#endif
