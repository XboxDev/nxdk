// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2023 ExoSkye
// SPDX-FileCopyrightText: 2025 Ryan Wendland

// Part of Microsoft CRT

#include <assert.h>
#include <errno.h>
#include <hal/debug.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Made referencing https://www.digitalmars.com/rtl/direct.html, retrieved 2023-06-10, copyrighted 1999-2018 by Digital Mars

// Only call this function in the case there actually is an error
static int win32_error_to_errno (DWORD winerror)
{
    switch (winerror) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_FILENAME_EXCED_RANGE:
        case ERROR_BAD_PATHNAME:
            return ENOENT;
        case ERROR_ALREADY_EXISTS:
        case ERROR_FILE_EXISTS:
            return EEXIST;
        case ERROR_NOT_LOCKED:
        case ERROR_ACCESS_DENIED:
            return EACCES;
        case ERROR_INVALID_PARAMETER:
        case ERROR_INVALID_FUNCTION:
            return EINVAL;
        case ERROR_NOT_ENOUGH_MEMORY:
            return ENOMEM;
        case ERROR_DIR_NOT_EMPTY:
            return ENOTEMPTY;
        case ERROR_CALL_NOT_IMPLEMENTED:
            return ENOSYS;
        default:
            debugPrint("WARNING: Unknown win32 error code 0x%08lx, returning EINVAL", winerror);
            return EINVAL;
    }
}

int _mkdir (const char *pathname)
{
    BOOL result = CreateDirectoryA(pathname, NULL);

    if (result == false) {
        errno = win32_error_to_errno(GetLastError());
        return -1;
    }

    return 0;
}

int _rmdir (const char *pathname)
{
    BOOL result = RemoveDirectoryA(pathname);

    if (result == false) {
        errno = win32_error_to_errno(GetLastError());
        return -1;
    }

    return 0;
}

/*
 * TODO: Make a working directory system?
 * nxdk mounts the running xbe directory at D:\, so we will return that as a hardcoded current working directory and drive.
 */

int _chdir (char *path)
{
    if (strcmp(path, "D:\\") == 0 || strcmp(path, "D:") == 0) {
        return 0;
    }

    assert(0);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    errno = win32_error_to_errno(GetLastError());
    return -1;
}

int _chdrive (int drive)
{
    if (drive == 4) {
        return 0;
    }

    assert(0);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    errno = win32_error_to_errno(GetLastError());
    return -1;
}

char *_getcwd (char *buffer, size_t length)
{
    const char *fixed_cwd = "D:\\";
    const int required_length = strlen(fixed_cwd) + 1;

    // If buffer is specified as NULL, _getcwd will allocate, using the malloc function, at least length bytes of memory.
    if (buffer == NULL) {
        buffer = (char *)malloc(required_length < length ? length : required_length);
        if (buffer == NULL) {
            errno = ENOMEM;
            return NULL;
        }
    } else if (length < required_length) {
        errno = ERANGE;
        return NULL;
    }

    strcpy(buffer, fixed_cwd);
    return buffer;
}

char *_getwd (char *path_name)
{
    if (path_name == NULL) {
        errno = EINVAL;
        return NULL;
    }
    // This is a legacy unsafe function. It assumes that the buffer is at least MAX_PATH characters long.
    return _getcwd(path_name, MAX_PATH);
}

int _getdrive (void)
{
    return 4;
}
