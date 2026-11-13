// SPDX-License-Identifier: MIT
/**
 * @file lib/xboxrt/libc_extensions/fd_internal.h
 * @brief Internal descriptor lifetime and optional socket dispatch interfaces.
 */
#pragma once
#include <sys/types.h>
#include <windows.h>

#define NXDK_SOCKET_FD_OFFSET 256 ///< Reserve descriptors below this for files.

/** @brief Socket operations installed once networking is initialized. */
struct nxdk_socket_io
{
    ssize_t (*read)(int, void *, size_t);        ///< Receive socket data.
    ssize_t (*write)(int, const void *, size_t); ///< Send socket data.
    int (*close)(int);                           ///< Close a socket.
    int (*fcntl)(int, int, int);                 ///< Query or set socket flags.
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Install an immutable socket dispatch table with static lifetime.
 * @param operations Operations provided by nxdk's network initialization.
 */
void nxdk_fd_set_socket_io (const struct nxdk_socket_io *operations);
/**
 * @brief Lock a file descriptor against concurrent close, seek, and I/O.
 * @param fd File descriptor.
 * @return Native handle with its lock held, or INVALID_HANDLE_VALUE with errno set.
 */
HANDLE nxdk_fd_acquire (int fd);
/**
 * @brief Release a successful nxdk_fd_acquire lock.
 * @param fd Locked file descriptor.
 */
void nxdk_fd_release (int fd);
#ifdef __cplusplus
}
#endif
