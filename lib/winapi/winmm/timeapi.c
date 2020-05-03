#include <timeapi.h>
#include <winerror.h>
#include <xboxkrnl/xboxkrnl.h>

MMRESULT timeBeginPeriod (UINT uPeriod)
{
    return TIMERR_NOERROR;
}

MMRESULT timeEndPeriod (UINT uPeriod)
{
    return TIMERR_NOERROR;
}

DWORD timeGetTime ()
{
    return KeTickCount;
}
