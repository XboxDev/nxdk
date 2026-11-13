// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file lib/net/poll.h
 * @brief Provide the POSIX poll.h include path expected by Unix-oriented third-party
 * libraries while reusing nxdk's socket compatibility layer.
 */

#include <sys/socket.h>
