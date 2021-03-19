#include <nxdk/format.h>
#include <nxdk/fatx.h>

#include <stdlib.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>

bool nxFormatVolume (const char *volumePath, uint32_t bytesPerCluster)
{
    ANSI_STRING volumePathStr;
    OBJECT_ATTRIBUTES objattr;
    IO_STATUS_BLOCK ioStatus;
    HANDLE handle;
    NTSTATUS status;

    // Default cluster size is 16KiB (32 sectors)
    if (bytesPerCluster == 0) {
        bytesPerCluster = 16384;
    }

    RtlInitAnsiString(&volumePathStr, volumePath);
    InitializeObjectAttributes(&objattr, &volumePathStr, OBJ_CASE_INSENSITIVE, NULL, NULL);

    status = NtOpenFile(&handle, SYNCHRONIZE | FILE_READ_DATA | FILE_WRITE_DATA, &objattr, &ioStatus, 0, FILE_SYNCHRONOUS_IO_NONALERT | FILE_NO_INTERMEDIATE_BUFFERING);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return false;
    }

    // Get geometry of the entire disk, mainly to get the sector size
    DISK_GEOMETRY diskGeometry;
    status = NtDeviceIoControlFile(handle, NULL, NULL, NULL, &ioStatus, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &diskGeometry, sizeof(diskGeometry));
    if (!NT_SUCCESS(status)) {
        NtClose(handle);
        SetLastError(RtlNtStatusToDosError(status));
        return false;
    }

    // Get information about the partition we format, for size checks
    PARTITION_INFORMATION partInfo;
    status = NtDeviceIoControlFile(handle, NULL, NULL, NULL, &ioStatus, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &partInfo, sizeof(partInfo));
    if (!NT_SUCCESS(status)) {
        NtClose(handle);
        SetLastError(RtlNtStatusToDosError(status));
        return false;
    }

    uint32_t alignment = ROUND_TO_PAGES(diskGeometry.BytesPerSector);
    if (bytesPerCluster < alignment) {
        bytesPerCluster = alignment;
    }

    uint32_t superblockBytes = PAGE_SIZE;
    if (superblockBytes < alignment) {
        superblockBytes = alignment;
    }

    uint32_t numclusters = partInfo.PartitionLength.QuadPart / bytesPerCluster + 1;
    uint32_t clusterTableSize;
    bool fatx16;
    if (numclusters < 65520) {
        fatx16 = true;
        clusterTableSize = numclusters * sizeof(uint16_t);
    } else {
        fatx16 = false;
        clusterTableSize = numclusters * sizeof(uint32_t);
    }

    // Round up to nearest multiple of alignment
    clusterTableSize = (clusterTableSize + alignment - 1) & ~(alignment - 1);

    // Check if the partition is large enough to contain the absolute minimum (superblock, cluster table, root directory)
    if (superblockBytes + clusterTableSize + bytesPerCluster >= partInfo.PartitionLength.QuadPart) {
        NtClose(handle);
        SetLastError(ERROR_DISK_FULL);
        return false;
    }

    // Allocate the buffer for our upcoming disk writes
    uint8_t *buffer = malloc(alignment);
    if (!buffer) {
        NtClose(handle);
        SetLastError(RtlNtStatusToDosError(STATUS_INSUFFICIENT_RESOURCES));
        return false;
    }

    RtlFillMemory(buffer, alignment, 0xFF);
    FATX_SUPERBLOCK *superblock = (FATX_SUPERBLOCK *)buffer;
    superblock->Signature = FATX_SIGNATURE;
    superblock->SectorsPerCluster = bytesPerCluster / diskGeometry.BytesPerSector;
    superblock->FirstClusterOffset = 1;
    superblock->Unknown = 0;

    // Unclear why, but this is how games do it
    LARGE_INTEGER systemTime;
    KeQuerySystemTime(&systemTime);
    superblock->VolumeId = systemTime.LowPart;

    // Write the whole superblock
    LARGE_INTEGER offset;
    offset.QuadPart = 0;
    for (size_t remainingBytes = superblockBytes; remainingBytes > 0; remainingBytes -= alignment, offset.QuadPart += alignment) {
        status = NtWriteFile(handle, NULL, NULL, NULL, &ioStatus, buffer, alignment, &offset);
        if (!NT_SUCCESS(status)) {
            goto close_and_return;
        }

        RtlZeroMemory(buffer, alignment);
    }

    // Following the superblock, write the cluster table
    if (fatx16) {
        ((uint16_t *)buffer)[0] = 0xfff8;
        ((uint16_t *)buffer)[1] = 0xffff;
    } else {
        ((uint32_t *)buffer)[0] = 0xfffffff8;
        ((uint32_t *)buffer)[1] = 0xffffffff;
    }
    offset.QuadPart = superblockBytes;
    for (size_t remainingBytes = clusterTableSize; remainingBytes > 0; remainingBytes -= alignment, offset.QuadPart += alignment) {
        status = NtWriteFile(handle, NULL, NULL, NULL, &ioStatus, buffer, alignment, &offset);
        if (!NT_SUCCESS(status)) {
            goto close_and_return;
        }

        RtlZeroMemory(buffer, sizeof(uint32_t) * 2);
    }

    // Following the cluster table, write an empty root directory cluster
    RtlFillMemory(buffer, alignment, 0xFF);
    for (size_t remainingBytes = bytesPerCluster; remainingBytes > 0; remainingBytes -= alignment, offset.QuadPart += alignment) {
        status = NtWriteFile(handle, NULL, NULL, NULL, &ioStatus, buffer, alignment, &offset);
        if (!NT_SUCCESS(status)) {
            goto close_and_return;
        }
    }

close_and_return:
    free(buffer);
    NtFsControlFile(handle, NULL, NULL, NULL, &ioStatus, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0);
    NtClose(handle);
    if (NT_SUCCESS(status)) {
        return true;
    } else {
        SetLastError(RtlNtStatusToDosError(status));
        return false;
    }
}
