// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2023 ExoSkye

// Part of Microsoft CRT

#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <assert.h>
#include <hal/debug.h>

// Made referencing https://www.digitalmars.com/rtl/direct.html, retrieved 2023-06-10, copyrighted 1999-2018 by Digital Mars

// Only call this function in the case there actually is an error
static int convert_error(DWORD winerror)
{
    switch (winerror) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_FILENAME_EXCED_RANGE:
        case ERROR_BAD_PATHNAME:
            return ENOENT;
            break;
        case ERROR_ALREADY_EXISTS:
        case ERROR_FILE_EXISTS:
            return EEXIST;
            break;
        case ERROR_NOT_LOCKED:
        case ERROR_ACCESS_DENIED:
            return EACCES;
            break;
        case ERROR_INVALID_PARAMETER:
        case ERROR_INVALID_FUNCTION:
            return EINVAL;
            break;
        case ERROR_NOT_ENOUGH_MEMORY:
            return ENOMEM;
            break;
        case ERROR_DIR_NOT_EMPTY:
            return ENOTEMPTY;
            break;
        default:
            debugPrint("WARNING: Unknown win32 error code 0x%08lx, returning EINVAL", winerror);
            return EINVAL;
    }
}

/*
 * TODO: Make a working directory system?
 */

int _chdir(char* path) {
    assert(0);
    return -1; // Unreachable
}

int _chdrive(int drive) {
    assert(0);
    return -1; // Unreachable
}

char* _getcwd(char* buffer, size_t length) {
    assert(0);
    return NULL; // Unreachable
}

char* _getwd(char* path_name) {
    assert(0);
    return NULL; // Unreachable
}

int _getdrive(void) {
    assert(0);
    return -1; // Unreachable
}

/*
 * Below are the only things that can work
 */

int _mkdir(const char* pathname) {
    BOOL result = CreateDirectoryA(
            pathname,
            NULL
    );

    if (result == true) {
        return 0;
    } else {
        errno = convert_error(GetLastError());

        return -1;
    }
}

int _rmdir(const char* pathname) {
    BOOL result = RemoveDirectoryA(
            pathname
    );

    if (result == true) {
        return 0;
    } else {
        errno = convert_error(GetLastError());

        return -1;
    }
}
