// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2023 Ryan Wendland

#ifndef __TIMEZONEAPI_H__
#define __TIMEZONEAPI_H__

#include <minwinbase.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIME_ZONE_ID_UNKNOWN  0
#define TIME_ZONE_ID_STANDARD 1
#define TIME_ZONE_ID_DAYLIGHT 2
#define TIME_ZONE_ID_INVALID  ((DWORD)0xFFFFFFFF)

typedef struct _TIME_ZONE_INFORMATION
{
    LONG Bias;
    WCHAR StandardName[32];
    SYSTEMTIME StandardDate;
    LONG StandardBias;
    WCHAR DaylightName[32];
    SYSTEMTIME DaylightDate;
    LONG DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;

DWORD GetTimeZoneInformation (LPTIME_ZONE_INFORMATION lpTimeZoneInformation);

#ifdef __cplusplus
}
#endif

#endif
