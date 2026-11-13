// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file lib/net/sys/ioctl.h
 * @brief Provide the POSIX sys/ioctl.h include path expected by Unix-oriented
 * third-party libraries while reusing nxdk's socket compatibility layer.
 */

#include <sys/socket.h>

/** @brief Socket-only I/O controls, retaining the existing lwIP interface. */
#define ioctl(s, command, argument) lwip_ioctl(s, command, argument)
