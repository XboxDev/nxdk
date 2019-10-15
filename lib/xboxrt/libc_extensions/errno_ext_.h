// Make sure errno_t is always defined when including errno.h, mimicking the
// behavior of Win32 headers as expected by libc++.
#ifndef _PDCLIB_ERRNO_T_DEFINED
#define _PDCLIB_ERRNO_T_DEFINED _PDCLIB_ERRNO_T_DEFINED
typedef int errno_t;
#endif
