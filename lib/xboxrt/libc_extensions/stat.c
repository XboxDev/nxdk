// SPDX-License-Identifier: MIT
/**
 * @file lib/xboxrt/libc_extensions/stat.c
 * @brief Query Xbox filesystem metadata by path or open descriptor.
 */
#include "fd_internal.h"
#include "fs_internal.h"

#include <limits.h>
#include <sys/stat.h>
#include <xboxkrnl/xboxkrnl.h>

/**
 * @brief Convert native file timestamps with checked Unix seconds.
 * @param ticks Native 100-nanosecond timestamp since 1601.
 * @param seconds Destination Unix timestamp.
 * @return Zero on success, or -1 with EOVERFLOW on failure.
 */
static int stat_time (long long ticks, time_t *seconds)
{
    long long value = ticks / 10000000LL - 11644473600LL;
    if (ticks % 10000000LL < 0) {
        --value;
    }
    if (value < LONG_MIN || value > LONG_MAX) {
        errno = EOVERFLOW;
        return -1;
    }
    *seconds = (time_t)value;
    return 0;
}

int stat (const char *path, struct stat *status)
{
    if (status == NULL) {
        errno = EINVAL;
        return -1;
    }
    char normalized[MAX_PATH];
    if (nxdk_fs_path(path, normalized) != 0) {
        return -1;
    }
    HANDLE handle = CreateFileA(normalized, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        errno = _PDCLIB_w32errno(GetLastError());
        return -1;
    }
    struct stat value;
    const int result = nxdk_fs_stat_handle(handle, &value);
    if (!CloseHandle(handle) && result == 0) {
        errno = _PDCLIB_w32errno(GetLastError());
        return -1;
    }
    if (result == 0) {
        *status = value;
    }
    return result;
}

int fstat (int fd, struct stat *status)
{
    if (status == NULL) {
        errno = EINVAL;
        return -1;
    }
    HANDLE handle = nxdk_fd_acquire(fd);
    if (handle == INVALID_HANDLE_VALUE) {
        return -1;
    }
    const int result = nxdk_fs_stat_handle(handle, status);
    nxdk_fd_release(fd);
    return result;
}

int nxdk_fs_stat_handle (HANDLE handle, struct stat *status)
{
    BY_HANDLE_FILE_INFORMATION identity;
    if (!GetFileInformationByHandle(handle, &identity)) {
        const DWORD error = GetLastError();
        errno = _PDCLIB_w32errno(error);
        return -1;
    }
    IO_STATUS_BLOCK io;
    FILE_NETWORK_OPEN_INFORMATION metadata;
    const NTSTATUS result = NtQueryInformationFile(handle, &io, &metadata, sizeof(metadata), FileNetworkOpenInformation);
    if (!NT_SUCCESS(result)) {
        errno = _PDCLIB_w32errno(RtlNtStatusToDosError(result));
        return -1;
    }
    struct stat value = {0};
    if (metadata.EndOfFile.QuadPart < 0 || stat_time(metadata.LastAccessTime.QuadPart, &value.st_atime) != 0 || stat_time(metadata.LastWriteTime.QuadPart, &value.st_mtime) != 0 || stat_time(metadata.ChangeTime.QuadPart, &value.st_ctime) != 0) {
        errno = EOVERFLOW;
        return -1;
    }
    value.st_dev = identity.dwVolumeSerialNumber;
    value.st_ino = ((uint64_t)identity.nFileIndexHigh << 32) | identity.nFileIndexLow;
    value.st_nlink = identity.nNumberOfLinks;
    value.st_size = metadata.EndOfFile.QuadPart;
    value.st_mode = S_IRUSR | S_IRGRP | S_IROTH;
    if ((metadata.FileAttributes & FILE_ATTRIBUTE_READONLY) == 0) {
        value.st_mode |= S_IWUSR | S_IWGRP | S_IWOTH;
    }
    if ((metadata.FileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        value.st_mode |= S_IFDIR | S_IXUSR | S_IXGRP | S_IXOTH;
    } else {
        value.st_mode |= S_IFREG;
    }
    *status = value;
    return 0;
}
