// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2017 Matt Borgerson

/**
 * @file lib/xboxrt/libc_extensions/unistd.h
 * @brief Descriptor-based file I/O, sharing ownership and offsets with stdio.
 */
#pragma once
#include <stddef.h>
#include <sys/types.h>

#ifndef SEEK_SET
#define SEEK_SET 0 ///< Seek relative to the beginning.
#define SEEK_CUR 1 ///< Seek relative to the current offset.
#define SEEK_END 2 ///< Seek relative to the end.
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Read bytes from a file or socket, advancing its offset where applicable.
 * @param fd File or socket descriptor.
 * @param buffer Destination buffer.
 * @param count Maximum byte count; one call transfers at most INT_MAX bytes.
 * @return Bytes read, zero at EOF, or -1 with errno set on failure.
 */
ssize_t read (int fd, void *buffer, size_t count);
/**
 * @brief Write bytes to a file or socket, honoring O_APPEND for every file write.
 * @param fd File or socket descriptor.
 * @param buffer Source buffer.
 * @param count Maximum byte count; one call transfers at most INT_MAX bytes.
 * @return Bytes written, which may be less than count, or -1 with errno set.
 */
ssize_t write (int fd, const void *buffer, size_t count);
/**
 * @brief Close a descriptor and its native handle after pending file I/O completes.
 * @param fd File or socket descriptor; a successful fdopen transfers closure to fclose.
 * @return Zero on success, or -1 with errno set on failure.
 */
int close (int fd);
/**
 * @brief Reposition a regular file using a signed 64-bit offset.
 * @param fd File descriptor.
 * @param offset Offset in bytes.
 * @param whence SEEK_SET, SEEK_CUR, or SEEK_END.
 * @return New offset, or -1 with errno set; sockets fail with ESPIPE.
 */
off_t lseek (int fd, off_t offset, int whence);
#ifdef __cplusplus
}
#endif
