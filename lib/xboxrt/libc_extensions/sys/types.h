// SPDX-License-Identifier: MIT

#pragma once

/*
 * Provide the small POSIX sys/types.h surface that nxdk consumers expect when
 * they follow the Unix/BSD socket path instead of the WinSock path.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NXDK_PID_T_DEFINED
#define NXDK_PID_T_DEFINED
typedef int pid_t;
#endif

#ifndef NXDK_UID_T_DEFINED
#define NXDK_UID_T_DEFINED
typedef unsigned int uid_t;
#endif

#ifndef NXDK_GID_T_DEFINED
#define NXDK_GID_T_DEFINED
typedef unsigned int gid_t;
#endif

#ifndef NXDK_MODE_T_DEFINED
#define NXDK_MODE_T_DEFINED
typedef uint32_t mode_t;
#endif

#ifndef NXDK_OFF_T_DEFINED
#define NXDK_OFF_T_DEFINED
typedef long off_t;
#endif

#ifndef NXDK_SSIZE_T_DEFINED
#define NXDK_SSIZE_T_DEFINED
typedef int ssize_t;
#endif

#ifdef __cplusplus
}
#endif
