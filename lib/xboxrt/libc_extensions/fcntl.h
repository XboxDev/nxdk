// SPDX-License-Identifier: MIT

#pragma once

/*
 * Provide the subset of fcntl.h constants that nxdk consumers currently need.
 *
 * Some libraries only need this header to exist, while others also depend on
 * the open mode constants below.
 */

#ifndef O_RDONLY
#define O_RDONLY 0x0000
#endif
#ifndef O_WRONLY
#define O_WRONLY 0x0001
#endif
#ifndef O_RDWR
#define O_RDWR 0x0002
#endif
#ifndef O_ACCMODE
#define O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)
#endif
#ifndef O_CREAT
#define O_CREAT 0x0100
#endif
#ifndef O_EXCL
#define O_EXCL 0x0200
#endif
#ifndef O_TRUNC
#define O_TRUNC 0x0400
#endif
#ifndef O_APPEND
#define O_APPEND 0x0800
#endif
#ifndef O_BINARY
#define O_BINARY 0x0000
#endif
#ifndef O_TEXT
#define O_TEXT 0x0000
#endif
