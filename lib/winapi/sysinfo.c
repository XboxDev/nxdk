#include <sysinfoapi.h>
#include <assert.h>
#include <xboxkrnl/xboxkrnl.h>

void GetSystemTime (LPSYSTEMTIME lpSystemTime)
{
    assert(lpSystemTime != NULL);

    LARGE_INTEGER kTime;
    TIME_FIELDS timeFields;

    KeQuerySystemTime(&kTime);
    RtlTimeToTimeFields(&kTime, &timeFields);
    lpSystemTime->wYear = timeFields.Year;
    lpSystemTime->wMonth = timeFields.Month;
    lpSystemTime->wDay = timeFields.Day;
    lpSystemTime->wHour = timeFields.Hour;
    lpSystemTime->wMinute = timeFields.Minute;
    lpSystemTime->wSecond = timeFields.Second;
    lpSystemTime->wMilliseconds = timeFields.Millisecond;
    lpSystemTime->wDayOfWeek = timeFields.Weekday;
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
