// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Stefan Schmidt

#ifndef _XBOXRT_TCHAR_H
#define _XBOXRT_TCHAR_H

/**
 * @file tchar.h
 * @author Stefan Schmidt
 *
 * tchar.h is an MS extension to provide mappings for building applications that use
 * multi-byte character strings, Unicode or single-byte character strings, from
 * the same sourcecode.
 * This provides a (not necessarily complete) implementation for nxdk to
 * hopefully help with porting Windows-specific software.
 * For more information, see the following link:
 * https://docs.microsoft.com/en-us/cpp/c-runtime-library/generic-text-mappings?view=vs-2019
 */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(UNICODE) || defined(_MBCS)
#error nxdk does not support the Unicode API
#endif

#define _TEOF EOF

#ifndef __TCHAR_DEFINED
#define __TCHAR_DEFINED
typedef char _TCHAR;
typedef int _TINT;
typedef signed char _TSCHAR;
typedef unsigned char _TUCHAR;
typedef char _TXCHAR;
#endif

#define __T(x) x
#define _T __T
#define _TEXT __T

#define _tmain main

#define _tprintf printf
#define _ftprintf fprintf
#define _stprintf sprintf
#define _sntprintf _snprintf
#define _vtprintf vprintf
#define _vftprintf vfprintf
#define _vstprintf vsprintf
#define _vsntprintf vsnprintf
#define _tscanf scanf
#define _ftscanf fscanf
#define _stscanf sscanf

#define _fgettc fgetc
#define _fgetts fgets
#define _fputtc fputc
#define _fputts fputs
#define _gettc getc
#define _gettchar getchar
#define _puttc putc
#define _puttchar putchar
#define _putts puts

#define _tcstof strtof
#define _tcstod strtod
#define _tcstol strtol
#define _tcstoll strtoll
#define _tcstoul strtoul
#define _tcstoull strtoull
#define _tstof atof
#define _tstol atol
#define _tstoi atoi
#define _ttol atol
#define _ttoi atoi

#define _tcscat strcat
#define _tcscpy strcpy
#define _tcsdup strdup
#define _tcslen strlen
#define _tcsxfrm strxfrm
#define _tcserror strerror

#define _tsystem system

#define _tasctime asctime
#define _tctime ctime
#define _tcsftime strftime

#define _tgetenv getenv

#define _tfopen fopen
#define _tfreopen freopen
#define _ttmpnam tmpnam

#define _istcntrl iscntrl
#define _istxdigit isxdigit
#define _istalnum isalnum
#define _istalpha isalpha
#define _istdigit isdigit
#define _istgraph isgraph
#define _istlower islower
#define _istprint isprint
#define _istpunct ispunct
#define _istspace isspace
#define _istupper isupper
#define _totupper toupper
#define _totlower tolower
#define _istlegal(c) (1)

#ifdef __cplusplus
}
#endif

#endif
