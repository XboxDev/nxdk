// SPDX-License-Identifier: MIT
/**
 * @file lib/xboxrt/libc_extensions/dirent.h
 * @brief Declare directory streams backed by Xbox filesystem searches.
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
/** @brief One entry in a directory stream. */
struct dirent
{
    char d_name[260]; ///< Null-terminated filename, valid until the next readdir call.
};
/** @brief Opaque, independently owned directory enumeration. */
typedef struct nxdk_directory DIR;
/**
 * @brief Open a directory using nxdk paths, resolving relative paths against D:\\.
 * @param path Directory to enumerate.
 * @return Owned stream, or NULL with errno set on failure.
 */
DIR *opendir (const char *path);
/**
 * @brief Read the next directory entry.
 * @param directory Open directory stream.
 * @return Entry, or NULL at end or on failure; errno is unchanged at normal end.
 */
struct dirent *readdir (DIR *directory);
/**
 * @brief Release a directory stream and its search handle.
 * @param directory Stream to close.
 * @return Zero on success, or -1 with errno set on failure.
 */
int closedir (DIR *directory);
#ifdef __cplusplus
}
#endif
