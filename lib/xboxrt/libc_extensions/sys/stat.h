// SPDX-License-Identifier: MIT
/**
 * @file lib/xboxrt/libc_extensions/sys/stat.h
 * @brief Declare path-based file metadata for Xbox files and directories.
 */
#pragma once
#include <sys/types.h>
#include <time.h>

#define S_IFMT 0170000                               ///< File-type mask.
#define S_IFDIR 0040000                              ///< Directory file type.
#define S_IFREG 0100000                              ///< Regular-file type.
#define S_IRUSR 00400                                ///< Owner read permission.
#define S_IWUSR 00200                                ///< Owner write permission.
#define S_IXUSR 00100                                ///< Owner search permission for directories.
#define S_IRGRP 00040                                ///< Group read permission.
#define S_IWGRP 00020                                ///< Group write permission.
#define S_IXGRP 00010                                ///< Group search permission for directories.
#define S_IROTH 00004                                ///< Other read permission.
#define S_IWOTH 00002                                ///< Other write permission.
#define S_IXOTH 00001                                ///< Other search permission for directories.
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR) ///< Test for a directory.
#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG) ///< Test for a regular file.

/**
 * @brief File identity, size, and timestamps queried from the filesystem.
 *
 * Xbox has one privileged identity. Permission bits describe the read-only
 * attribute and directory searchability, rather than Unix access-control lists.
 */
struct stat
{
    dev_t st_dev;     ///< Filesystem volume serial number.
    ino_t st_ino;     ///< Native filesystem file index.
    mode_t st_mode;   ///< File type and mapped read/write/search permissions.
    nlink_t st_nlink; ///< Link count reported by the native filesystem API.
    uid_t st_uid;     ///< Sole privileged user, zero.
    gid_t st_gid;     ///< Sole privileged group, zero.
    dev_t st_rdev;    ///< Zero: this API supports regular files and directories.
    off_t st_size;    ///< Signed 64-bit file length in bytes.
    time_t st_atime;  ///< Last-access Unix timestamp.
    time_t st_mtime;  ///< Last-write Unix timestamp.
    time_t st_ctime;  ///< Last metadata-change Unix timestamp.
};

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Query actual file metadata using a temporary native file handle.
 * @param path File or directory, relative to nxdk's fixed D:\\ cwd if needed.
 * @param status Destination, unchanged on error.
 * @return Zero on success, or -1 with errno set, including EOVERFLOW when a
 * timestamp is not representable by nxdk's signed 32-bit time_t.
 */
int stat (const char *path, struct stat *status);
/**
 * @brief Query metadata for an open regular file without changing its offset.
 * @param fd Open file descriptor.
 * @param status Destination, unchanged on error.
 * @return Zero on success, or -1 with errno set.
 */
int fstat (int fd, struct stat *status);
#ifdef __cplusplus
}
#endif
