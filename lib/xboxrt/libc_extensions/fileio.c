// SPDX-License-Identifier: MIT
/**
 * @file lib/xboxrt/libc_extensions/fileio.c
 * @brief Native file descriptors with serialized offsets, append, and lifetime.
 */
#include "fd_internal.h"
#include "fs_internal.h"

#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <unistd.h>
#include <xboxkrnl/xboxkrnl.h>

/** @brief One native file description and the lock protecting its offset. */
struct file_descriptor
{
    HANDLE handle; ///< NULL marks an unused slot.
    int flags;     ///< Access mode and mutable file-status flags.
    int reserved;  ///< A pending open owns this otherwise empty slot.
    SRWLOCK lock;  ///< Serializes I/O and closure of this slot.
};

static struct file_descriptor files[NXDK_SOCKET_FD_OFFSET]; ///< File slots; 0 to 2 are reserved.
static SRWLOCK table_lock = SRWLOCK_INIT;                   ///< Protects allocation and lookup.
static const struct nxdk_socket_io *socket_io;              ///< Optional immutable network dispatch.

void nxdk_fd_set_socket_io (const struct nxdk_socket_io *operations)
{
    __atomic_store_n(&socket_io, operations, __ATOMIC_RELEASE);
}

/**
 * @brief Look up networking operations without requiring a network library dependency.
 * @return Installed operations, or NULL with EBADF when networking is unavailable.
 */
static const struct nxdk_socket_io *get_socket_io (void)
{
    const struct nxdk_socket_io *operations = __atomic_load_n(&socket_io, __ATOMIC_ACQUIRE);
    if (operations == NULL) {
        errno = EBADF;
    }
    return operations;
}

HANDLE nxdk_fd_acquire (int fd)
{
    if (fd < 3 || fd >= NXDK_SOCKET_FD_OFFSET) {
        errno = EBADF;
        return INVALID_HANDLE_VALUE;
    }
    AcquireSRWLockExclusive(&table_lock);
    if (files[fd].handle == NULL) {
        ReleaseSRWLockExclusive(&table_lock);
        errno = EBADF;
        return INVALID_HANDLE_VALUE;
    }
    AcquireSRWLockExclusive(&files[fd].lock);
    HANDLE handle = files[fd].handle;
    ReleaseSRWLockExclusive(&table_lock);
    return handle;
}

void nxdk_fd_release (int fd)
{
    ReleaseSRWLockExclusive(&files[fd].lock);
}

int open (const char *path, int flags, ...)
{
    const int access = flags & O_ACCMODE;
    if ((access != O_RDONLY && access != O_WRONLY && access != O_RDWR) ||
        (flags & ~(O_ACCMODE | O_APPEND | O_CREAT | O_TRUNC | O_EXCL | O_CLOEXEC)) ||
        ((flags & O_TRUNC) && access == O_RDONLY)) {
        errno = EINVAL;
        return -1;
    }
    if (flags & O_CREAT) {
        va_list arguments;
        va_start(arguments, flags);
        (void)va_arg(arguments, mode_t);
        va_end(arguments);
    }
    char normalized[MAX_PATH];
    if (nxdk_fs_path(path, normalized) != 0) {
        return -1;
    }
    DWORD desired = access == O_RDONLY ? GENERIC_READ : access == O_WRONLY ? GENERIC_WRITE
                                                                           : GENERIC_READ | GENERIC_WRITE;
    DWORD creation = OPEN_EXISTING;
    if (flags & O_CREAT) {
        creation = flags & O_EXCL ? CREATE_NEW : flags & O_TRUNC ? CREATE_ALWAYS
                                                                 : OPEN_ALWAYS;
    } else if (flags & O_TRUNC) {
        creation = TRUNCATE_EXISTING;
    }
    /* Reserve before creating or truncating, so EMFILE has no filesystem side effects. */
    int fd;
    AcquireSRWLockExclusive(&table_lock);
    for (fd = 3; fd < NXDK_SOCKET_FD_OFFSET; ++fd) {
        if (files[fd].handle == NULL && !files[fd].reserved) {
            files[fd].reserved = 1;
            break;
        }
    }
    ReleaseSRWLockExclusive(&table_lock);
    if (fd == NXDK_SOCKET_FD_OFFSET) {
        errno = EMFILE;
        return -1;
    }
    HANDLE handle = CreateFileA(normalized, desired, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    const DWORD error = handle == INVALID_HANDLE_VALUE ? GetLastError() : 0;
    AcquireSRWLockExclusive(&table_lock);
    AcquireSRWLockExclusive(&files[fd].lock);
    files[fd].reserved = 0;
    if (handle != INVALID_HANDLE_VALUE) {
        files[fd].handle = handle;
        files[fd].flags = flags & (O_ACCMODE | O_APPEND);
    }
    nxdk_fd_release(fd);
    ReleaseSRWLockExclusive(&table_lock);
    if (handle == INVALID_HANDLE_VALUE) {
        errno = _PDCLIB_w32errno(error);
        return -1;
    }
    return fd;
}

int close (int fd)
{
    if (fd >= NXDK_SOCKET_FD_OFFSET) {
        const struct nxdk_socket_io *operations = get_socket_io();
        return operations != NULL ? operations->close(fd) : -1;
    }
    if (fd < 3) {
        errno = EBADF;
        return -1;
    }
    AcquireSRWLockExclusive(&table_lock);
    if (files[fd].handle == NULL) {
        ReleaseSRWLockExclusive(&table_lock);
        errno = EBADF;
        return -1;
    }
    AcquireSRWLockExclusive(&files[fd].lock);
    HANDLE handle = files[fd].handle;
    files[fd].handle = NULL;
    ReleaseSRWLockExclusive(&table_lock);
    BOOL success = CloseHandle(handle);
    if (!success) {
        errno = _PDCLIB_w32errno(GetLastError());
    }
    nxdk_fd_release(fd);
    return success ? 0 : -1;
}

ssize_t read (int fd, void *buffer, size_t count)
{
    if (fd >= NXDK_SOCKET_FD_OFFSET) {
        const struct nxdk_socket_io *operations = get_socket_io();
        return operations != NULL ? operations->read(fd, buffer, count > INT_MAX ? INT_MAX : count) : -1;
    }
    HANDLE handle = nxdk_fd_acquire(fd);
    if (handle == INVALID_HANDLE_VALUE) {
        return -1;
    }
    ssize_t result = -1;
    if (!(files[fd].flags & O_RDONLY)) {
        errno = EBADF;
    } else if (buffer == NULL && count != 0) {
        errno = EFAULT;
    } else if (count == 0) {
        result = 0;
    } else {
        DWORD amount;
        if (ReadFile(handle, buffer, count > INT_MAX ? INT_MAX : count, &amount, NULL)) {
            result = (ssize_t)amount;
        } else {
            errno = _PDCLIB_w32errno(GetLastError());
        }
    }
    nxdk_fd_release(fd);
    return result;
}

ssize_t write (int fd, const void *buffer, size_t count)
{
    if (fd >= NXDK_SOCKET_FD_OFFSET) {
        const struct nxdk_socket_io *operations = get_socket_io();
        return operations != NULL ? operations->write(fd, buffer, count > INT_MAX ? INT_MAX : count) : -1;
    }
    HANDLE handle = nxdk_fd_acquire(fd);
    if (handle == INVALID_HANDLE_VALUE) {
        return -1;
    }
    ssize_t result = -1;
    if (!(files[fd].flags & O_WRONLY)) {
        errno = EBADF;
    } else if (buffer == NULL && count != 0) {
        errno = EFAULT;
    } else if (count == 0) {
        result = 0;
    } else if (files[fd].flags & O_APPEND) {
        /* The native EOF sentinel makes append atomic across different handles. */
        LARGE_INTEGER end = {.QuadPart = -1};
        IO_STATUS_BLOCK io;
        NTSTATUS status = NtWriteFile(handle, NULL, NULL, NULL, &io, (void *)buffer, count > INT_MAX ? INT_MAX : count, &end);
        if (status == STATUS_PENDING) {
            status = NtWaitForSingleObject(handle, FALSE, NULL);
            if (NT_SUCCESS(status)) {
                status = io.Status;
            }
        }
        if (NT_SUCCESS(status)) {
            result = (ssize_t)io.Information;
        } else {
            errno = _PDCLIB_w32errno(RtlNtStatusToDosError(status));
        }
    } else {
        DWORD amount;
        if (WriteFile(handle, buffer, count > INT_MAX ? INT_MAX : count, &amount, NULL)) {
            result = (ssize_t)amount;
        } else {
            errno = _PDCLIB_w32errno(GetLastError());
        }
    }
    nxdk_fd_release(fd);
    return result;
}

off_t lseek (int fd, off_t offset, int whence)
{
    if (fd >= NXDK_SOCKET_FD_OFFSET) {
        if (fcntl(fd, F_GETFL) < 0) {
            return -1;
        }
        errno = ESPIPE;
        return -1;
    }
    HANDLE handle = nxdk_fd_acquire(fd);
    if (handle == INVALID_HANDLE_VALUE) {
        return -1;
    }
    LARGE_INTEGER position = {.QuadPart = offset};
    off_t result = -1;
    if (whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END) {
        errno = EINVAL;
    } else if (SetFilePointerEx(handle, position, &position, whence)) {
        result = position.QuadPart;
    } else {
        errno = _PDCLIB_w32errno(GetLastError());
    }
    nxdk_fd_release(fd);
    return result;
}

int fcntl (int fd, int command, ...)
{
    int argument = 0;
    if (command == F_SETFL || command == F_SETFD) {
        va_list arguments;
        va_start(arguments, command);
        argument = va_arg(arguments, int);
        va_end(arguments);
    }
    if (fd >= NXDK_SOCKET_FD_OFFSET) {
        const struct nxdk_socket_io *operations = get_socket_io();
        return operations != NULL ? operations->fcntl(fd, command, argument) : -1;
    }
    if (nxdk_fd_acquire(fd) == INVALID_HANDLE_VALUE) {
        return -1;
    }
    int result = -1;
    switch (command) {
    case F_GETFL:
        result = files[fd].flags;
        break;
    case F_SETFL:
        if (argument & ~(O_ACCMODE | O_APPEND | O_CREAT | O_TRUNC | O_EXCL | O_CLOEXEC)) {
            errno = EINVAL;
        } else {
            files[fd].flags = (files[fd].flags & O_ACCMODE) | (argument & O_APPEND);
            result = 0;
        }
        break;
    case F_GETFD:
        result = FD_CLOEXEC;
        break;
    case F_SETFD:
        if (argument == FD_CLOEXEC) {
            result = 0;
        } else {
            errno = argument == 0 ? ENOTSUP : EINVAL;
        }
        break;
    default:
        errno = EINVAL;
        break;
    }
    nxdk_fd_release(fd);
    return result;
}
