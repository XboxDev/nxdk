// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file lib/xboxrt/libc_extensions/sys/types.h
 * @brief Provide the small POSIX sys/types.h surface that nxdk consumers expect when
 * they follow the Unix/BSD socket path instead of the WinSock path.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NXDK_PID_T_DEFINED
#define NXDK_PID_T_DEFINED
/** @brief Integer type for application identifiers used by portable code. */
typedef int pid_t;
#endif

#ifndef NXDK_UID_T_DEFINED
#define NXDK_UID_T_DEFINED
/** @brief Integer type for user identifiers in portable data structures. */
typedef unsigned int uid_t;
#endif

#ifndef NXDK_GID_T_DEFINED
#define NXDK_GID_T_DEFINED
/** @brief Integer type for group identifiers in portable data structures. */
typedef unsigned int gid_t;
#endif

#ifndef NXDK_MODE_T_DEFINED
#define NXDK_MODE_T_DEFINED
/** @brief File mode representation shared with the existing stat.h header. */
typedef uint32_t mode_t;
#endif

#ifndef NXDK_OFF_T_DEFINED
#define NXDK_OFF_T_DEFINED
/** @brief Signed 64-bit file size and offset representation. */
typedef int64_t off_t;
#endif

#ifndef NXDK_SSIZE_T_DEFINED
#define NXDK_SSIZE_T_DEFINED
/** @brief Signed byte count compatible with lwIP socket operations. */
typedef int ssize_t;
#endif

/** @brief Filesystem volume identifier. */
typedef uint32_t dev_t;
/** @brief Native 64-bit filesystem file index. */
typedef uint64_t ino_t;
/** @brief Number of filesystem links. */
typedef uint32_t nlink_t;

#ifdef __cplusplus
}
#endif
