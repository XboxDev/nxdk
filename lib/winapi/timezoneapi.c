// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2023 Ryan Wendland

#include <timezoneapi.h>
#include <winbase.h>
#include <assert.h>
#include <xboxkrnl/xboxkrnl.h>
#include <string.h>

typedef struct
{
    UCHAR Month;     // 1-12, a 0 indicates there is no timezone information
    UCHAR Day;       // 1 = 1st occurrence of DayOfWeek up to 5 (5th or last)
    UCHAR DayOfWeek; // 0 = Sunday to 6 = Saturday
    UCHAR Hour;
} XBOX_TZ_STRUCT;

// Determine the day of the week given a year, month and day
// https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Methods_in_computer_code
static UCHAR GetDayOfWeek (SHORT year, UCHAR month, UCHAR day)
{
    return (day += month < 3 ? year-- : year - 2, 23 *
            month / 9 + day + 4 + year / 4 - year / 100 + year / 400) % 7;
}

static UCHAR GetDaysInMonth (SHORT year, UCHAR month)
{
    static const UCHAR daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    UCHAR days;

    assert(month >= 1);
    assert(month <= 12);

    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days = 29;
        } else {
            days = 28;
        }
    } else {
        days = daysPerMonth[month - 1];
    }

    return days;
}

// Given a year, month, day of the week, and the n-th occurrence this returns the day in the month
// i.e The 2nd Wednesday in November 2023 is the 8th November. This function would return 8
static UCHAR GetDayOfMonth (SHORT year, XBOX_TZ_STRUCT *tzInfo)
{
    UCHAR dayOfWeek = GetDayOfWeek(year, tzInfo->Month, 1);
    UCHAR daysInMonth = GetDaysInMonth(year, tzInfo->Month);
    UCHAR dayTargetCount = tzInfo->Day - 1;
    UCHAR dayActual = 1;

    // Find first occurrence of the weekday
    dayActual += (tzInfo->DayOfWeek < dayOfWeek) ? (tzInfo->DayOfWeek + 7 - dayOfWeek) : (tzInfo->DayOfWeek - dayOfWeek);

    // Add remaining weeks
    dayActual += dayTargetCount * 7;

    // If we overrun, go back to the last occurrence in the month
    while (dayActual > daysInMonth) {
        dayActual -= 7;
    }
    return dayActual;
}

static void XboxTimeZoneToSystemTime (LPSYSTEMTIME lpSystemTime, XBOX_TZ_STRUCT *tzInfo)
{
    lpSystemTime->wMonth = tzInfo->Month;
    lpSystemTime->wDayOfWeek = tzInfo->DayOfWeek;
    lpSystemTime->wDay = tzInfo->Day;
    lpSystemTime->wHour = tzInfo->Hour;
}

static BOOL XboxTimeZoneValid (XBOX_TZ_STRUCT *tzInfo)
{
    if (tzInfo->Month > 12 || tzInfo->DayOfWeek > 6 || tzInfo->Hour > 23) {
        return FALSE;
    }
    return TRUE;
}

DWORD GetTimeZoneInformation (LPTIME_ZONE_INFORMATION lpTimeZoneInformation)
{
    assert(lpTimeZoneInformation != NULL);

    LONG timeZoneBias, daylightBias, standardBias, daylightDisableFlag;
    LONG timeNow, daylightStart, daylightEnd;
    XBOX_TZ_STRUCT daylightStartDate, daylightEndDate;
    CHAR daylightName[4], standardName[4];
    NTSTATUS queryStatus;
    TIME_FIELDS dateNow;
    LARGE_INTEGER kTime;
    ULONG type;

    memset(lpTimeZoneInformation, 0, sizeof(TIME_ZONE_INFORMATION));

    queryStatus = ExQueryNonVolatileSetting(XC_TIMEZONE_BIAS, &type, &timeZoneBias, sizeof(timeZoneBias), NULL);
    if (!NT_SUCCESS(queryStatus)) {
        SetLastError(RtlNtStatusToDosError(queryStatus));
        return TIME_ZONE_ID_INVALID;
    }

    lpTimeZoneInformation->Bias = timeZoneBias;

    // Check if daylight savings time (DST) adjustment is enabled
    queryStatus = ExQueryNonVolatileSetting(XC_MISC, &type, &daylightDisableFlag, sizeof(daylightDisableFlag), NULL);
    if (!NT_SUCCESS(queryStatus)) {
        SetLastError(RtlNtStatusToDosError(queryStatus));
        return TIME_ZONE_ID_INVALID;
    }

    // DST adjustment is disabled
    if (daylightDisableFlag & XC_MISC_FLAG_DISABLE_DST) {
        return TIME_ZONE_ID_UNKNOWN;
    }

    // Query all DST info from EEPROM
    // FIXME: One large EEPROM access could be better
    queryStatus = ExQueryNonVolatileSetting(XC_TZ_DLT_DATE, &type, &daylightStartDate, sizeof(daylightStartDate), NULL);
    if (!NT_SUCCESS(queryStatus)) {
        SetLastError(RtlNtStatusToDosError(queryStatus));
        return TIME_ZONE_ID_INVALID;
    }
    queryStatus = ExQueryNonVolatileSetting(XC_TZ_STD_DATE, &type, &daylightEndDate, sizeof(daylightEndDate), NULL);
    if (!NT_SUCCESS(queryStatus)) {
        SetLastError(RtlNtStatusToDosError(queryStatus));
        return TIME_ZONE_ID_INVALID;
    }
    queryStatus = ExQueryNonVolatileSetting(XC_TZ_DLT_BIAS, &type, &daylightBias, sizeof(daylightBias), NULL);
    if (!NT_SUCCESS(queryStatus)) {
        SetLastError(RtlNtStatusToDosError(queryStatus));
        return TIME_ZONE_ID_INVALID;
    }
    queryStatus = ExQueryNonVolatileSetting(XC_TZ_STD_BIAS, &type, &standardBias, sizeof(standardBias), NULL);
    if (!NT_SUCCESS(queryStatus)) {
        SetLastError(RtlNtStatusToDosError(queryStatus));
        return TIME_ZONE_ID_INVALID;
    }
    queryStatus = ExQueryNonVolatileSetting(XC_TZ_DLT_NAME, &type, daylightName, sizeof(daylightName), NULL);
    if (!NT_SUCCESS(queryStatus)) {
        SetLastError(RtlNtStatusToDosError(queryStatus));
        return TIME_ZONE_ID_INVALID;
    }
    queryStatus = ExQueryNonVolatileSetting(XC_TZ_STD_NAME, &type, standardName, sizeof(standardName), NULL);
    if (!NT_SUCCESS(queryStatus)) {
        SetLastError(RtlNtStatusToDosError(queryStatus));
        return TIME_ZONE_ID_INVALID;
    }

    if ((XboxTimeZoneValid(&daylightStartDate) == FALSE) || (XboxTimeZoneValid(&daylightEndDate) == FALSE)) {
        return TIME_ZONE_ID_INVALID;
    }

    // Xbox stores up to 4 characters here
    for (UCHAR i = 0; i < 4; i++) {
        lpTimeZoneInformation->StandardName[i] = standardName[i];
        lpTimeZoneInformation->DaylightName[i] = daylightName[i];
    }

    // There was no DST info
    if (daylightEndDate.Month == 0 || daylightStartDate.Month == 0) {
        return TIME_ZONE_ID_UNKNOWN;
    }

    XboxTimeZoneToSystemTime(&lpTimeZoneInformation->DaylightDate, &daylightStartDate);
    XboxTimeZoneToSystemTime(&lpTimeZoneInformation->StandardDate, &daylightEndDate);
    lpTimeZoneInformation->DaylightBias = daylightBias;
    lpTimeZoneInformation->StandardBias = standardBias;

    // Now we need to determine if the current time is within the DST range
    KeQuerySystemTime(&kTime);
    RtlTimeToTimeFields(&kTime, &dateNow);

    // Determine what day in the month DST starts and ends
    UCHAR daylightStartDay = GetDayOfMonth(dateNow.Year, &daylightStartDate);
    UCHAR daylightEndDay = GetDayOfMonth(dateNow.Year, &daylightEndDate);

    // Fix month wrapping. i.e if DST is from Oct(10) to Feb(2) and the current month is Jan(1) we should be in DST
    // Applying a simple check would result is 10 < 1 < 2 = false and incorrect
    // To account for this, we offset the months in the following year by 12
    // This results in the comparison 10 < 13(12+1) < 14(12+2) = true and is correct
    if (daylightEndDate.Month < daylightStartDate.Month) {
        if (dateNow.Month <= daylightEndDate.Month) {
            dateNow.Month += 12;
        }
        daylightEndDate.Month += 12;
    }

    // Now that month wrapping is fixed, we convert the dates to minutes from the start of the year corresponding to daylightStartDate
    // The conversion to minutes allows for easier comparison. The minute resolution allows for timezones with 0.5 hour offsets.
    const LONG mpd = 24 * 60;  // Minutes per day
    const LONG mpm = 31 * mpd; // Minutes per month. 31 is ok providing it's consistent
    timeNow = (dateNow.Month * mpm) + (dateNow.Day * mpd) + (dateNow.Hour * 60) + dateNow.Minute;
    // DST change overs happen in local time, so we need to add the respective biases too
    daylightStart = (daylightStartDate.Month * mpm) + (daylightStartDay * mpd) + (daylightStartDate.Hour * 60) + timeZoneBias + standardBias;
    daylightEnd = (daylightEndDate.Month * mpm) + (daylightEndDay * mpd) + (daylightEndDate.Hour * 60) + timeZoneBias + daylightBias;

    if (timeNow >= daylightStart && timeNow < daylightEnd) {
        return TIME_ZONE_ID_DAYLIGHT;
    } else {
        return TIME_ZONE_ID_STANDARD;
    }
}
