// SPDX-License-Identifier: MIT
/**
 * @file lib/xboxrt/libc_extensions/dirent.c
 * @brief Implement independently owned directory streams using Win32 searches.
 */
#include "fs_internal.h"

#include <dirent.h>
#include <stdlib.h>

/** @brief Per-stream search state. */
struct nxdk_directory
{
    HANDLE handle;         ///< Search handle, invalid for empty streams.
    WIN32_FIND_DATAA data; ///< Last native result.
    struct dirent entry;   ///< Caller-visible entry storage.
    int first;             ///< Whether the first native result is pending.
    int ended;             ///< Whether the stream reached end-of-directory.
};

DIR *opendir (const char *path)
{
    char pattern[MAX_PATH];
    if (nxdk_fs_path(path, pattern) != 0) {
        return NULL;
    }
    const DWORD attributes = GetFileAttributesA(pattern);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        errno = _PDCLIB_w32errno(GetLastError());
        return NULL;
    }
    if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        errno = ENOTDIR;
        return NULL;
    }
    size_t length = strlen(pattern);
    const size_t separator = pattern[length - 1] != '\\';
    if (length + separator + 1 >= MAX_PATH) {
        errno = ENAMETOOLONG;
        return NULL;
    }
    if (separator) {
        pattern[length++] = '\\';
    }
    pattern[length++] = '*';
    pattern[length] = '\0';
    DIR *directory = calloc(1, sizeof(*directory));
    if (directory == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    directory->handle = FindFirstFileA(pattern, &directory->data);
    directory->first = 1;
    if (directory->handle == INVALID_HANDLE_VALUE) {
        const DWORD error = GetLastError();
        if (error != ERROR_FILE_NOT_FOUND && error != ERROR_NO_MORE_FILES) {
            free(directory);
            errno = _PDCLIB_w32errno(error);
            return NULL;
        }
        directory->ended = 1;
    }
    return directory;
}

struct dirent *readdir (DIR *directory)
{
    if (directory == NULL) {
        errno = EBADF;
        return NULL;
    }
    if (directory->ended) {
        return NULL;
    }
    if (!directory->first && !FindNextFileA(directory->handle, &directory->data)) {
        const DWORD error = GetLastError();
        directory->ended = 1;
        if (error != ERROR_NO_MORE_FILES) {
            errno = _PDCLIB_w32errno(error);
        }
        return NULL;
    }
    directory->first = 0;
    memcpy(directory->entry.d_name, directory->data.cFileName, sizeof(directory->entry.d_name));
    directory->entry.d_name[sizeof(directory->entry.d_name) - 1] = '\0';
    return &directory->entry;
}

int closedir (DIR *directory)
{
    if (directory == NULL) {
        errno = EBADF;
        return -1;
    }
    const int failed = directory->handle != INVALID_HANDLE_VALUE && !FindClose(directory->handle);
    const DWORD error = failed ? GetLastError() : 0;
    free(directory);
    if (failed) {
        errno = _PDCLIB_w32errno(error);
        return -1;
    }
    return 0;
}
