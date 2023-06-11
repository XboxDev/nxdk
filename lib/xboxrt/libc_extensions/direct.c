// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2023 ExoSkye

// Part of Microsoft CRT

#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

// Made referencing https://www.digitalmars.com/rtl/direct.html, retrieved 2023-06-10, copyrighted 1999-2018 by Digital Mars

#define MAXPATH 80
#define MAXDRIVE 3

/*
 * These silently succeed, since they wouldn't have any effect on the program at all
 */

int _chdir(char* path) {
    return 0;
}

int _chdrive(int drive) {
    return 0;
}

/*
 * The Xbox has no concept of current working directory, so these can't work
 */

char* _getcwd(char* buffer, size_t length) {
    errno = -EINVAL;
    return NULL;
}

char* _getwd(char* path_name) {
    errno = -EINVAL;
    return NULL;
}

int _getdrive(void) {
    errno = -EINVAL;
    return 0;
}

/*
 * There's no path variable on the Xbox, so this can't work
 */

char* _searchpath(const char* file) {
    errno = -EINVAL;
    return NULL;
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
        DWORD err = GetLastError();

        if (err == ERROR_ALREADY_EXISTS) {
            errno = EACCES;
        } else if (err == ERROR_PATH_NOT_FOUND) {
            errno = ENOENT;
        }

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
        DWORD err = GetLastError();

        if (err == ERROR_ALREADY_EXISTS) {
            errno = EACCES;
        } else if (err == ERROR_PATH_NOT_FOUND) {
            errno = ENOENT;
        }

        return -1;
    }
}

void fnmerge(char* path, const char* drive, const char* dir, const char* name, const char* ext) {

}

void fnsplit(const char* path, char* drive, char* dir, char* name, char* ext) {

}
