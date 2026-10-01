// SPDX-License-Identifier: MIT

#pragma once

/*
 * Provide the limited sys/stat.h surface currently needed by nxdk consumers.
 *
 * This keeps the declarations close to the rest of nxdk's libc extension layer
 * while the underlying file metadata support remains intentionally small.
 */

#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NXDK_DEV_T_DEFINED
#define NXDK_DEV_T_DEFINED
typedef unsigned int dev_t;
#endif

#ifndef NXDK_INO_T_DEFINED
#define NXDK_INO_T_DEFINED
typedef unsigned int ino_t;
#endif

#ifndef NXDK_NLINK_T_DEFINED
#define NXDK_NLINK_T_DEFINED
typedef unsigned int nlink_t;
#endif

#ifndef S_IFMT
#define S_IFMT 0170000
#endif
#ifndef S_IFDIR
#define S_IFDIR 0040000
#endif
#ifndef S_IFCHR
#define S_IFCHR 0020000
#endif
#ifndef S_IFBLK
#define S_IFBLK 0060000
#endif
#ifndef S_IFREG
#define S_IFREG 0100000
#endif
#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
#endif

struct stat {
    dev_t st_dev;
    mode_t st_mode;
    ino_t st_ino;
    nlink_t st_nlink;
    uid_t st_uid;
    gid_t st_gid;
    dev_t st_rdev;
    unsigned int st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
};

int fstat(int fd, struct stat *status);
int stat(const char *path, struct stat *status);
int _fstat(int fd, struct stat *status);
int _stat(const char *path, struct stat *status);

#ifdef __cplusplus
}
#endif
