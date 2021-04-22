#ifndef __WINMM_TIMEAPI_H__
#define __WINMM_TIMEAPI_H__

#include <windef.h>
#include <mmsystem.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define TIMERR_NOERROR 0
#define TIMERR_NOCANDO 97

MMRESULT timeBeginPeriod (UINT uPeriod);
MMRESULT timeEndPeriod (UINT uPeriod);
DWORD timeGetTime (void);

#ifdef __cplusplus
}
#endif

#endif

