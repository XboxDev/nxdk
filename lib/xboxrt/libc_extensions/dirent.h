// SPDX-License-Identifier: MIT

#pragma once

/*
 * Provide a minimal dirent surface for code that is compiled as part of a
 * third-party build but does not enumerate Xbox directories at runtime.
 */

#ifdef __cplusplus
extern "C" {
#endif

struct dirent {
    char d_name[256];
};

typedef struct nxdk_dir_stub DIR;

static inline DIR *opendir(const char *path)
{
    (void) path;
    return 0;
}

static inline int closedir(DIR *directory)
{
    (void) directory;
    return -1;
}

static inline struct dirent *readdir(DIR *directory)
{
    (void) directory;
    return 0;
}

#ifdef __cplusplus
}
#endif
