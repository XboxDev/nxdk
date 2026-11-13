// SPDX-License-Identifier: MIT
/**
 * @file lib/xboxrt/libc_extensions/fcntl.h
 * @brief File open flags and descriptor control, compatible with lwIP's flags.
 */
#pragma once

#include <sys/types.h>

#define O_NONBLOCK 1        ///< Nonblocking socket I/O; unsupported for regular files.
#define O_NDELAY O_NONBLOCK ///< Alias for nonblocking I/O.
#define O_RDONLY 2          ///< Open for reading.
#define O_WRONLY 4          ///< Open for writing.
#define O_RDWR 6            ///< Open for reading and writing.
#define O_ACCMODE 6         ///< Mask of access-mode bits.
#define O_APPEND 8          ///< Append every write atomically at end of file.
#define O_CREAT 0x100       ///< Create the file if absent; permissions are not stored by FATX.
#define O_TRUNC 0x200       ///< Truncate an existing writable file.
#define O_EXCL 0x400        ///< With O_CREAT, fail if the file already exists.
#define O_BINARY 0          ///< Native I/O does not translate text.
#define O_CLOEXEC 0x800     ///< Handles are never inherited by another process.
#define F_GETFD 1           ///< Get descriptor flags, always FD_CLOEXEC.
#define F_SETFD 2           ///< Accept FD_CLOEXEC; inheritance is unsupported.
#define F_GETFL 3           ///< Get access and file-status flags.
#define F_SETFL 4           ///< Set O_APPEND for files or O_NONBLOCK for sockets.
#define FD_CLOEXEC 1        ///< Descriptor is not inherited.

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Open a regular file, allocating the lowest free descriptor from 3 to 255.
 * @param path File path, relative to D:\\ when no drive is specified.
 * @param flags Access mode and optional creation and status flags.
 * @param ... Creation mode when O_CREAT is set; FATX has no Unix permissions.
 * @return Descriptor on success, or -1 with errno set on failure.
 */
int open (const char *path, int flags, ...);
/**
 * @brief Query or change descriptor flags, dispatching sockets to lwIP.
 * @param fd File or socket descriptor.
 * @param command F_GETFD, F_SETFD, F_GETFL, or F_SETFL.
 * @param ... Integer flags for a set command.
 * @return Flags for a get command, zero for a set command, or -1 with errno set.
 */
int fcntl (int fd, int command, ...);
#ifdef __cplusplus
}
#endif
