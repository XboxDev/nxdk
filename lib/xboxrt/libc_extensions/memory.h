// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file lib/xboxrt/libc_extensions/memory.h
 * @brief Older third-party sources may still include memory.h for the standard string
 * and memory routines. Route that legacy include to string.h.
 */

#include <string.h>
