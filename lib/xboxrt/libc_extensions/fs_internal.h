// SPDX-License-Identifier: MIT
/**
 * @file lib/xboxrt/libc_extensions/fs_internal.h
 * @brief Share path normalization and error conversion between filesystem APIs.
 */
#pragma once
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <windows.h>

/**
 * @brief Reuse the Xbox C runtime's existing Win32-to-errno mapping.
 * @param error Win32 error code.
 * @return Corresponding errno value.
 */
int _PDCLIB_w32errno (DWORD error);

/**
 * @brief Fill metadata for an already-open handle, leaving output unchanged on error.
 * @param handle Native file or directory handle.
 * @param status Destination metadata.
 * @return Zero on success, or -1 with errno set.
 */
struct stat;
int nxdk_fs_stat_handle (HANDLE handle, struct stat *status);

/**
 * @brief Recognize the case-insensitive native device namespace.
 * @param path Nonempty source path.
 * @return Nonzero for a native Device path.
 */
static inline int nxdk_fs_device_path (const char *path)
{
    const char prefix[] = "\\device\\";
    for (size_t index = 0; index < sizeof(prefix) - 1; ++index) {
        unsigned char value = (unsigned char)path[index];
        if (value >= 'A' && value <= 'Z') {
            value += 'a' - 'A';
        }
        if (value == '/') {
            value = '\\';
        }
        if (value != (unsigned char)prefix[index]) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Normalize separators and dot components using nxdk's fixed D:\\ cwd.
 * @param path Source path.
 * @param result Destination with MAX_PATH bytes of storage.
 * @return Zero on success, or -1 with errno set on failure.
 */
static inline int nxdk_fs_path (const char *path, char *result)
{
    if (path == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (*path == '\0') {
        errno = ENOENT;
        return -1;
    }
    size_t used;
    if (path[1] == ':') {
        result[0] = path[0];
        result[1] = ':';
        result[2] = '\\';
        used = 3;
        path += 2;
    } else if (nxdk_fs_device_path(path)) {
        result[0] = '\\';
        used = 1;
        ++path;
    } else {
        memcpy(result, "D:\\", 3);
        used = 3;
    }
    const size_t root = used;
    while (*path != '\0') {
        while (*path == '/' || *path == '\\') {
            ++path;
        }
        const char *component = path;
        while (*path != '\0' && *path != '/' && *path != '\\') {
            ++path;
        }
        const size_t length = (size_t)(path - component);
        if (length == 0 || (length == 1 && component[0] == '.')) {
            continue;
        }
        if (length == 2 && component[0] == '.' && component[1] == '.') {
            while (used > root && result[used - 1] != '\\') {
                --used;
            }
            if (used > root) {
                --used;
            }
            continue;
        }
        const size_t separator = result[used - 1] != '\\';
        if (used + separator + length >= MAX_PATH) {
            errno = ENAMETOOLONG;
            return -1;
        }
        if (separator) {
            result[used++] = '\\';
        }
        memcpy(result + used, component, length);
        used += length;
    }
    result[used] = '\0';
    return 0;
}
