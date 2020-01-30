#include <sysinfoapi.h>
#include <assert.h>
#include <xboxkrnl/xboxkrnl.h>

void GetSystemTime (LPSYSTEMTIME lpSystemTime)
{
    assert(lpSystemTime != NULL);

    LARGE_INTEGER KTime;
    TIME_FIELDS TimeFields;

    KeQuerySystemTime(&KTime);

    RtlTimeToTimeFields(&KTime, &TimeFields);

    lpSystemTime->wYear = TimeFields.Year;
    lpSystemTime->wMonth = TimeFields.Month;
    lpSystemTime->wDay = TimeFields.Day;
    lpSystemTime->wHour = TimeFields.Hour;
    lpSystemTime->wMinute = TimeFields.Minute;
    lpSystemTime->wSecond = TimeFields.Second;
    lpSystemTime->wMilliseconds = TimeFields.Millisecond;
    lpSystemTime->wDayOfWeek = TimeFields.Weekday;
}


void GetSystemTimePreciseAsFileTime (LPFILETIME lpSystemTimeAsFileTime)
{
    assert(lpSystemTimeAsFileTime != NULL);
    LARGE_INTEGER systemTime;
    KeQuerySystemTime(&systemTime);
    lpSystemTimeAsFileTime->dwLowDateTime = systemTime.LowPart;
    lpSystemTimeAsFileTime->dwHighDateTime = systemTime.HighPart;
}

DWORD GetTickCount (void)
{
    return KeTickCount;
}

void GetSystemInfo (LPSYSTEM_INFO lpSystemInfo)
{
    assert(lpSystemInfo != NULL);

    lpSystemInfo->DUMMYUNIONNAME.DUMMYSTRUCTNAME.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_INTEL;
    lpSystemInfo->dwPageSize = 4096;
    lpSystemInfo->dwActiveProcessorMask = 1;
    lpSystemInfo->dwNumberOfProcessors = 1;
    lpSystemInfo->dwAllocationGranularity = 4096;
}
