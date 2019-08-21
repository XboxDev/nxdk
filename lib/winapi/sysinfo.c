#include <sysinfoapi.h>
#include <assert.h>
#include <xboxkrnl/xboxkrnl.h>

void GetSystemTimePreciseAsFileTime (LPFILETIME lpSystemTimeAsFileTime)
{
    assert(lpSystemTimeAsFileTime != NULL);
    LARGE_INTEGER systemTime;
    KeQuerySystemTime(&systemTime);
    lpSystemTimeAsFileTime->dwLowDateTime = systemTime.LowPart;
    lpSystemTimeAsFileTime->dwHighDateTime = systemTime.HighPart;
}
