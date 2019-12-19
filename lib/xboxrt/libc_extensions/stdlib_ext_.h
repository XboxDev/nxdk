/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _CRT_RAND_S
int rand_s (unsigned int *randomValue);
#endif

typedef void (__cdecl *_purecall_handler)(void);

_purecall_handler __cdecl _get_purecall_handler (void);
_purecall_handler __cdecl _set_purecall_handler (_purecall_handler function);

// Win32 extension providing the highest valid errno number
// https://docs.microsoft.com/en-us/cpp/c-runtime-library/errno-doserrno-sys-errlist-and-sys-nerr?view=vs-2019
#define _sys_nerr _PDCLIB_ERRNO_MAX

// Necessary stub for libc++
#include <wchar.h>
int mbtowc (wchar_t *pwc, const char *string, size_t n);

// Defined as on ReactOS - may need further adjustment if we decide to do locales properly
#define MB_CUR_MAX 2

#ifdef __cplusplus
}
#endif
