#ifndef _XLIBC_TIME_H
#define _XLIBC_TIME_H

#include <_restrict.h>
#include <_unimplemented.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NULL 0

#define CLOCKS_PER_SEC
#define TIME_UTC 1

typedef unsigned int size_t;

typedef struct {} clock_t; // FIXME
typedef long long time_t;

struct timespec
{
    time_t tv_sec; // Whole seconds
    long tv_nsec; // Nanoseconds
};

struct tm
{
    int tm_sec; // Seconds after the minute [0,60]
    int tm_min; // Minutes after the hour [0, 59]
    int tm_hour; // Hours since midnight [0,23]
    int tm_mday; // Day of the month [1,31]
    int tm_mon; // Months since January [0,11]
    int tm_year; // Years since 1900
    int tm_wday; // Days since Sunday [0,6]
    int tm_yday; // Days since January 1 [0,365]
    int tm_isdst; // Daylight Saving Time flag
};

clock_t clock (void) __unimplemented;

double difftime (time_t time1, time_t time0) __unimplemented;
time_t mktime (struct tm *timeptr) __unimplemented;
time_t time (time_t *timer) __unimplemented;

int timespec_get (struct timespec *ts, int base);

char *asctime (const struct tm *timeptr) __unimplemented;
char *ctime (const time_t *timer) __unimplemented;
struct tm *gmtime (const time_t *timer) __unimplemented;
struct tm *localtime (const time_t *timer) __unimplemented;
size_t strftime (char * XLIBC_RESTRICT s, size_t maxsize, const char * XLIBC_RESTRICT format, const struct tm * XLIBC_RESTRICT timeptr) __unimplemented;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _XLIBC_TIME_H */
