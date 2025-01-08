// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt
// SPDX-FileCopyrightText: 2020 Samuel Cuella
// SPDX-FileCopyrightText: 2023 Ryan Wendland

#include <assert.h>
#include <sysinfoapi.h>
#include <timezoneapi.h>
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

void GetSystemTimeAsFileTime (LPFILETIME lpSystemTimeAsFileTime)
{
    GetSystemTimePreciseAsFileTime(lpSystemTimeAsFileTime);
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

void GetLocalTime (LPSYSTEMTIME lpSystemTime)
{
    assert(lpSystemTime != NULL);

    LARGE_INTEGER kTime;
    TIME_FIELDS timeFields;
    TIME_ZONE_INFORMATION tzInfo;
    LONG bias;

    switch (GetTimeZoneInformation(&tzInfo)) {
        case TIME_ZONE_ID_UNKNOWN:
            bias = tzInfo.Bias;
            break;
        case TIME_ZONE_ID_STANDARD:
            bias = tzInfo.Bias + tzInfo.StandardBias;
            break;
        case TIME_ZONE_ID_DAYLIGHT:
            bias = tzInfo.Bias + tzInfo.DaylightBias;
            break;
        default:
            bias = 0;
            break;
    }

    KeQuerySystemTime(&kTime);
    kTime.QuadPart -= (bias * 60LL * 10000000LL);

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
