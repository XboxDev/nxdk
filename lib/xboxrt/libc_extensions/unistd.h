// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2017 Matt Borgerson

#pragma once

/*
 * Provide the small POSIX unistd.h surface that nxdk consumers currently need.
 *
 * The read(), write(), and lseek() stubs exist so cross-built third-party
 * utility code can compile even though the shipping Xbox binary does not use
 * those file descriptor paths at runtime.
 */

#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline pid_t getpid(void)
{
	return 1;
}

static inline ssize_t read(int fd, void *buffer, size_t count)
{
	(void) fd;
	(void) buffer;
	(void) count;
	return -1;
}

static inline ssize_t write(int fd, const void *buffer, size_t count)
{
	(void) fd;
	(void) buffer;
	(void) count;
	return -1;
}

static inline off_t lseek(int fd, off_t offset, int whence)
{
	(void) fd;
	(void) offset;
	(void) whence;
	return -1;
}

#ifdef __cplusplus
}
#endif
