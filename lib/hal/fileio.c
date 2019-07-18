#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hal/fileio.h>
#include <xboxkrnl/xboxkrnl.h>
#include <xboxrt/debug.h>
#include <stdbool.h>

// #define DEBUG

static char *currentDirString = NULL;

char *partitions[] =
{
	"\\??\\D:\\",
	"\\Device\\Harddisk0\\Partition1\\",
	"\\Device\\Harddisk0\\Partition2\\",
	"\\Device\\Harddisk0\\Partition3\\",
	"\\Device\\Harddisk0\\Partition4\\",
	"\\Device\\Harddisk0\\Partition5\\",
	"\\Device\\Harddisk0\\Partition6\\",
	"\\Device\\Harddisk0\\Partition7\\",
};

int getPartitionIndex(char c)
{
	switch(c)
	{
	case 'c': case 'C': return 2;
	case 'd': case 'D': return 0;
	case 'e': case 'E': return 1;
	case 'f': case 'F': return 6;
	case 'g': case 'G': return 7;
	case 'x': case 'X': return 3;
	case 'y': case 'Y': return 4;
	case 'z': case 'Z': return 5;
	default:            return -1;
	}
}

char *getPartitionString(char c)
{
	int i = getPartitionIndex(c);
	if (i == -1)
		return NULL;
	else
		return partitions[i];
}

void setPartitionString(char c, char *string)
{
	int i = getPartitionIndex(c);
	if (i != -1)
		partitions[i] = string;
}

static char *getCurrentDirString()
{
	char *tmp;
	if (currentDirString) {
		return currentDirString;
	}
	currentDirString = malloc(XeImageFileName->Length + 1);
	strcpy(currentDirString, XeImageFileName->Buffer);
	// Remove XBE name, leaving the path
	tmp = strrchr(currentDirString, '\\');
	if (tmp) {
		*(tmp + 1) = '\0';
	} else {
		free(currentDirString);
		currentDirString = NULL;
	}
	return currentDirString;
}

/**
 * Converts a DOS-style path (eg. "c:/foo/bar.txt") to a XBOX-style
 * path (eg. "\Device\Harddisk0\Partition2\foo\bar.txt").
 * Returns early if the path is alread in XBOX-style - will use the
 * input path as-is.
 *
 * We handle the following scenarios (back/forward slashes are handled
 * in all cases):
 *   c:\foo\bar.txt
 *   d:\foo\bar.txt
 *   .\foo\bar.txt                     ==> d:\foo\bar.txt
 *   \foo\bar.txt                      ==> d:\foo\bar.txt
 *   foo\bar.txt                       ==> d:\foo\bar.txt
 *   \\.\D:\foo\bar.txt                ==> d:\foo\bar.txt
 *   \??\c:\foo\bar.txt                ==> c:\foo\bar.txt
 */
int XConvertDOSFilenameToXBOX(const char *dosFilename, char *xboxFilename)
{
	// Allow Xbox filenames directly
	if (!memcmp(dosFilename, "\\Device\\", 8)) {
		strcpy(xboxFilename, dosFilename);
		return STATUS_SUCCESS;
	}

	// path contains the qualified pathname from the root
	// directory without the leading slash.  eg. "foo\bar.txt"
	const char *path;

	// partition points to a literal string representing
	// the fully qualified device name
	char *partition = NULL;
	if (dosFilename[0] == '.' && (dosFilename[1] == '\\' || dosFilename[1] == '/'))
	{
		//   .\foo\bar.txt
		path = dosFilename+2;
		partition = getCurrentDirString();
	}
	else if (dosFilename[0] == '\\' || dosFilename[0] == '/')
	{
		if (dosFilename[1] == '\\' && dosFilename[2] == '.' && dosFilename[3] == '\\')
		{
			//   \\.\x:\foo\bar.txt
			path = dosFilename+7;
			partition = getPartitionString(dosFilename[4]);
		}
		else if (dosFilename[1] == '?' && dosFilename[2] == '?' && dosFilename[3] == '\\')
		{
			//   \??\x:\foo\bar.txt
			path = dosFilename+7;
			partition = getPartitionString(dosFilename[4]);
		}
		else
		{
			//   \foo\bar.txt
			path = dosFilename+1;
			partition = getCurrentDirString();
		}
	}
	else if (dosFilename[1] == ':')
	{
		//   x:\foo\bar.txt
		path = dosFilename+3;
		partition = getPartitionString(dosFilename[0]);
	}
	else
	{
		//   foo\bar.txt
		path = dosFilename;
		partition = getCurrentDirString();
	}

	if (partition == NULL)
		return ERROR_INVALID_DRIVE;

	strcpy(xboxFilename, partition);
	strcat(xboxFilename, path);

	char *c = xboxFilename;
	while (*c++)
	{
		if (*c == '/')
			*c = '\\';
	}
	return STATUS_SUCCESS;
}

int XCreateFile(
	int *handle,
	const char *filename,
	unsigned int desiredAccess,
	unsigned int sharedMode,
	unsigned int creationDisposition,
	unsigned int flagsAndAttributes)
{
	OBJECT_ATTRIBUTES objectAttributes;
	ANSI_STRING       xboxFilename;
	IO_STATUS_BLOCK   ioStatusBlock;
	NTSTATUS          status;

#ifdef DEBUG
	debugPrint("XCreateFile filename=%s access=%08x shared=%08x creation=%08x flags=%08x ", filename, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes);
#endif

	// first thing we do is invalidate the handle
	*handle = INVALID_HANDLE_VALUE;

	// Fix the given filename and convert into an ANSI_STRING
	char tmp[200];
	int rc = XConvertDOSFilenameToXBOX(filename, tmp);
	if (rc != STATUS_SUCCESS)
		return rc;
	RtlInitAnsiString(&xboxFilename, tmp);

	// Convert creationDisposition flags
	switch (creationDisposition)
	{
	case    CREATE_NEW:
		creationDisposition = FILE_CREATE;
		break;
	case    CREATE_ALWAYS:
		creationDisposition = FILE_OVERWRITE_IF;
		break;
	case    OPEN_EXISTING:
		creationDisposition = FILE_OPEN;
		break;
	case    OPEN_ALWAYS:
		creationDisposition = FILE_OPEN_IF;
		break;
	case    TRUNCATE_EXISTING:
		// This one is special in Win32.  CreateFile errors if write
		// access is not requested.
		creationDisposition = FILE_OVERWRITE;
		if (!(desiredAccess & GENERIC_WRITE))
			return ERROR_INVALID_PARAMETER;
		break;
	default:
		// error out immediately if it doesn't recognize the disposition
		return ERROR_INVALID_PARAMETER;
	}

	DWORD flags = 0;

	// FILE_FLAG_BACKUP_SEMANTICS just allows opening directories on XBOX
	if (!(flagsAndAttributes & FILE_FLAG_BACKUP_SEMANTICS))
		flags |= FILE_NON_DIRECTORY_FILE;

	// If we're going to use blocking mode, we need to add this flag
	if (!(flagsAndAttributes & FILE_FLAG_OVERLAPPED))
		flags |= FILE_SYNCHRONOUS_IO_NONALERT;

	// Mimic Win32 by automatically adding DELETE access with this flag
	if (flagsAndAttributes & FILE_FLAG_DELETE_ON_CLOSE)
	{
		desiredAccess |= DELETE;
		flags |= FILE_DELETE_ON_CLOSE;
	}

	// The rest are simple translations
	if (flagsAndAttributes & FILE_FLAG_WRITE_THROUGH)
		flags |= FILE_WRITE_THROUGH;
	if (flagsAndAttributes & FILE_FLAG_NO_BUFFERING)
		flags |= FILE_NO_INTERMEDIATE_BUFFERING;
	if (flagsAndAttributes & FILE_FLAG_RANDOM_ACCESS)
		flags |= FILE_RANDOM_ACCESS;
	if (flagsAndAttributes & FILE_FLAG_SEQUENTIAL_SCAN)
		flags |= FILE_SEQUENTIAL_ONLY;
	// Eliminate flags from dwFlagsAndAttributes, leaving only attributes
	flagsAndAttributes &= (0xFFFF & ~FILE_ATTRIBUTE_DIRECTORY);

	objectAttributes.RootDirectory = NULL;
	objectAttributes.Attributes = ((flagsAndAttributes & FILE_FLAG_POSIX_SEMANTICS) ? 0 : OBJ_CASE_INSENSITIVE);
	objectAttributes.ObjectName = &xboxFilename;

	status = NtCreateFile(
		(PHANDLE)handle,
		desiredAccess,
		&objectAttributes,
		&ioStatusBlock,
		NULL,
		flagsAndAttributes,
		sharedMode,
		creationDisposition,
		flags);

	if (!NT_SUCCESS(status))
		return RtlNtStatusToDosError(status);

	// In CREATE_ALWAYS and OPEN_ALWAYS mode, if the file opened was
	// overwritten (we know from Information), an error code is set even
	// though the function succeeds.  This is so the application can know
	// the difference.
	if (((creationDisposition == FILE_OVERWRITE_IF) && ((unsigned int)ioStatusBlock.Information == FILE_OVERWRITTEN)) ||
	    ((creationDisposition == FILE_OPEN_IF) && ((unsigned int)ioStatusBlock.Information == FILE_OPENED)))
	{
		status = ERROR_ALREADY_EXISTS;
	}
	else
	{
		status = STATUS_SUCCESS;
	}

#ifdef DEBUG
	debugPrint(" ==>  handle=%08x status=%08x\n", *handle, status);
#endif

	return status;
}

int XReadFile(
	int handle,
	void *buffer,
	unsigned int numberOfBytesToRead,
	unsigned int *numberOfBytesRead)
{
	IO_STATUS_BLOCK ioStatusBlock;
	NTSTATUS        status;

#ifdef DEBUG
	debugPrint("XReadFile handle=%08x numberOfBytesToRead=%08x\n", handle, numberOfBytesToRead);
#endif

	if (numberOfBytesRead)
		*numberOfBytesRead = 0;

	status = NtReadFile(
		(void*)handle,
		NULL,
		NULL,
		NULL,
		(void*)&ioStatusBlock,
		(void*)buffer,
		numberOfBytesToRead,
		NULL);

	if (status == STATUS_PENDING)
		status = NtWaitForSingleObject((void*)handle, FALSE, (void*)NULL);

    if (NT_SUCCESS(status)) {
        if (numberOfBytesRead)
            *numberOfBytesRead = (unsigned int)ioStatusBlock.Information;
        return TRUE;
    }

    return FALSE;
}

int XWriteFile(
	int handle,
	void *buffer,
	unsigned int numberOfBytesToWrite,
	unsigned int *numberOfBytesWritten)
{
	IO_STATUS_BLOCK ioStatusBlock;
	NTSTATUS        status;

#ifdef DEBUG
	debugPrint("XWriteFile handle=%08x numberOfBytesToWrite=%08x\n", handle, numberOfBytesToWrite);
#endif

	if(numberOfBytesWritten)
		*numberOfBytesWritten = 0;

	status = NtWriteFile(
		(void*)handle,
		NULL,
		NULL,
		NULL,
		&ioStatusBlock,
		buffer,
		numberOfBytesToWrite,
		NULL);

	if (status == STATUS_PENDING)
		status = NtWaitForSingleObject((void*)handle, FALSE, NULL);

    if (NT_SUCCESS(status)) {
        if (numberOfBytesWritten){
            *numberOfBytesWritten = (unsigned int)ioStatusBlock.Information;
        }
        return TRUE;
    }

    return FALSE;
}

int XCloseHandle(int handle)
{
	NTSTATUS status = NtClose((void*)handle);

#ifdef DEBUG
	debugPrint("XCloseHandle handle=%08x\n", handle);
#endif

    if (NT_SUCCESS(status)) {
        return TRUE;
    }

    return FALSE;
}

int XGetFileSize(int handle, unsigned int *filesize)
{
	NTSTATUS                      status;
	IO_STATUS_BLOCK               ioStatusBlock;
	FILE_NETWORK_OPEN_INFORMATION openInfo;

#ifdef DEBUG
	debugPrint("XGetFileSize handle=%08x\n", handle);
#endif

	// We use FileNetworkOpenInformation from NtQueryInformationFile to get
	// the file size.  This trick I got from Windows XP's implementation,
	// which seems to work on XBOX.
	status = NtQueryInformationFile(
		(void*)handle,
		&ioStatusBlock,
		&openInfo,
		sizeof(openInfo),
		FileNetworkOpenInformation);

    if (NT_SUCCESS(status)) {
		if (filesize)
			*filesize = (unsigned int)openInfo.EndOfFile.u.LowPart;
        return TRUE;
    }

    return FALSE;
}

int XSetFilePointer(
	int handle,
	int distanceToMove,
	int *newFilePointer,
	int moveMethod)
{
	FILE_POSITION_INFORMATION positionInfo;
	LARGE_INTEGER             targetPointer;
	IO_STATUS_BLOCK           ioStatusBlock;
	NTSTATUS                  status;
	unsigned int              filesize;

#ifdef DEBUG
	debugPrint("XSetFilePointer handle=%08x distance=%08x method=%02x\n", handle, distanceToMove, moveMethod);
#endif

	// Calculate the target pointer
	switch (moveMethod)
	{
	case FILE_BEGIN:      // From the beginning of the file
		targetPointer.u.HighPart = 0;
		targetPointer.u.LowPart = distanceToMove;
	  break;
	case FILE_CURRENT:    // From the current position
		status = NtQueryInformationFile((void*)handle, &ioStatusBlock, &positionInfo, sizeof(positionInfo), FilePositionInformation);
		if (!NT_SUCCESS(status))
			return RtlNtStatusToDosError(status);
		targetPointer.u.HighPart = 0;
		targetPointer.u.LowPart = positionInfo.CurrentByteOffset.u.LowPart + distanceToMove;
		break;
	case FILE_END:       // From the end of the file
		status = XGetFileSize(handle, &filesize);
		if (!NT_SUCCESS(status))
			return RtlNtStatusToDosError(status);
		targetPointer.QuadPart = (unsigned long long)filesize-distanceToMove;
		break;
	default:
		return ERROR_INVALID_PARAMETER;
	}

	// Don't allow a negative seek
	if ((targetPointer.u.LowPart & 0x80000000) != 0)
		return ERROR_NEGATIVE_SEEK;

	// Fill in the new position information
	positionInfo.CurrentByteOffset.u.HighPart = targetPointer.u.HighPart;
	positionInfo.CurrentByteOffset.u.LowPart= targetPointer.u.LowPart;

	// Set the new position
	status = NtSetInformationFile((void*)handle, &ioStatusBlock, &positionInfo, sizeof(positionInfo), FilePositionInformation);

    if (NT_SUCCESS(status)) {
		if (newFilePointer)
			*newFilePointer = targetPointer.u.LowPart;
        return TRUE;
    }

	return FALSE;
}

int XRenameFile(
	const char *oldFilename,
	const char *newFilename)
{
	ANSI_STRING             xboxFilename;
	IO_STATUS_BLOCK         ioStatusBlock;
	OBJECT_ATTRIBUTES       objectAttributes;
	FILE_RENAME_INFORMATION renameInfo;
	NTSTATUS                status;
	int                     handle;

#ifdef DEBUG
	debugPrint("XRenameFile oldFilename=%s newFilename=%s\n", oldFilename, newFilename);
#endif

	char tmp[200];
	int rc = XConvertDOSFilenameToXBOX(oldFilename, tmp);
	if (rc != STATUS_SUCCESS)
		return rc;
	RtlInitAnsiString(&xboxFilename, tmp);

	objectAttributes.RootDirectory = NULL;
	objectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
	objectAttributes.ObjectName = &xboxFilename;

	// Try to open the file
	// I'm not sure why we have to use these particular flags, but we do.
	status = NtCreateFile(
		(PHANDLE)handle,
		DELETE | SYNCHRONIZE,
		&objectAttributes,
		&ioStatusBlock,
		NULL,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT | FILE_NON_DIRECTORY_FILE);

	if (!NT_SUCCESS(status))
		return RtlNtStatusToDosError(status);

	// Set up the rename information
	rc = XConvertDOSFilenameToXBOX(newFilename, tmp);
	if (rc != STATUS_SUCCESS)
		return rc;
	RtlInitAnsiString(&renameInfo.FileName, tmp);
	renameInfo.ReplaceIfExists = TRUE;
	renameInfo.RootDirectory = NULL;

	// Try to rename the file
	status = NtSetInformationFile(
		(HANDLE)handle,
		&ioStatusBlock,
		&renameInfo,
		sizeof(renameInfo),
		FileRenameInformation);

	if (!NT_SUCCESS(status))
	{
		NtClose((HANDLE)handle);
		return RtlNtStatusToDosError(status);
	}
	else
	{
		NtClose((HANDLE)handle);
		return STATUS_SUCCESS;
	}
}

int XCreateDirectory(char *directoryName)
{
	ANSI_STRING       xboxFilename;
	IO_STATUS_BLOCK   ioStatusBlock;
	OBJECT_ATTRIBUTES objectAttributes;
	NTSTATUS          status;
	int               handle;

#ifdef DEBUG
	debugPrint("XCreateDirectory directoryName=%s\n", directoryName);
#endif

	char tmp[200];
	int rc = XConvertDOSFilenameToXBOX(directoryName, tmp);
	if (rc != STATUS_SUCCESS)
		return rc;
	RtlInitAnsiString(&xboxFilename, tmp);

	objectAttributes.RootDirectory = NULL;
	objectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
	objectAttributes.ObjectName = &xboxFilename;

	// Create the directory
	status = NtCreateFile(
		(PHANDLE)&handle,
		GENERIC_READ | SYNCHRONIZE | DELETE,
		&objectAttributes,
		&ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_SUPERSEDE,
		FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);

	if (!NT_SUCCESS(status))
		return RtlNtStatusToDosError(status);
	else
	{
		NtClose((HANDLE)handle);
		return STATUS_SUCCESS;
	}
}

int XDeleteFile(const char *fileName)
{
	ANSI_STRING                  xboxFilename;
	IO_STATUS_BLOCK              ioStatusBlock;
	OBJECT_ATTRIBUTES            objectAttributes;
	FILE_DISPOSITION_INFORMATION deleteInfo;
	NTSTATUS                     status;
	int                          handle;

#ifdef DEBUG
	debugPrint("XDeleteFile fileName=%s\n", fileName);
#endif

	char tmp[200];
	int rc = XConvertDOSFilenameToXBOX(fileName, tmp);
	if (rc != STATUS_SUCCESS)
		return rc;
	RtlInitAnsiString(&xboxFilename, tmp);

	objectAttributes.RootDirectory = NULL;
	objectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
	objectAttributes.ObjectName = &xboxFilename;

	// Open the file for deletion
	status = NtCreateFile(
		(PHANDLE)&handle,
		DELETE | SYNCHRONIZE,
		&objectAttributes,
		&ioStatusBlock,
		NULL,
		0,
		FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE);

	if (!NT_SUCCESS(status))
		return RtlNtStatusToDosError(status);

	// Delete the file
	deleteInfo.DeleteFile = TRUE;
	status = NtSetInformationFile(
		(HANDLE)handle,
		&ioStatusBlock,
		&deleteInfo,
		sizeof(deleteInfo),
		FileDispositionInformation);

	if (!NT_SUCCESS(status))
		return RtlNtStatusToDosError(status);
	else
	{
		// Close the dead handle and return
		NtClose((HANDLE)handle);
		return STATUS_SUCCESS;
	}
}

int XDeleteDirectory(const char *directoryName)
{
	ANSI_STRING                  xboxFilename;
	IO_STATUS_BLOCK              ioStatusBlock;
	OBJECT_ATTRIBUTES            objectAttributes;
	FILE_DISPOSITION_INFORMATION deleteInfo;
	NTSTATUS                     status;
	int                          handle;

#ifdef DEBUG
	debugPrint("XDeleteDirectory directoryName=%s\n", directoryName);
#endif

	char tmp[200];
	int rc = XConvertDOSFilenameToXBOX(directoryName, tmp);
	if (rc != STATUS_SUCCESS)
		return rc;
	RtlInitAnsiString(&xboxFilename, tmp);

	objectAttributes.RootDirectory = NULL;
	objectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
	objectAttributes.ObjectName = &xboxFilename;

	// Open the file for deletion
	status = NtCreateFile(
		(PHANDLE)&handle,
		DELETE | SYNCHRONIZE,
		&objectAttributes,
		&ioStatusBlock,
		NULL,
		0,
		FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE);

	if (!NT_SUCCESS(status))
		return RtlNtStatusToDosError(status);

	// Delete the file
	deleteInfo.DeleteFile = TRUE;
	status = NtSetInformationFile(
		(HANDLE)handle,
		&ioStatusBlock,
		&deleteInfo,
		sizeof(deleteInfo),
		FileDispositionInformation);

	if (!NT_SUCCESS(status))
		return RtlNtStatusToDosError(status);
	else
	{
		// Close the dead handle and return
		NtClose((HANDLE)handle);
		return STATUS_SUCCESS;
	}
}

int XMountDrive(char driveLetter, char *directoryName)
{
#ifdef DEBUG
	debugPrint("XMountDrive driveLetter=%c directoryName=%s\n", driveLetter, directoryName);
#endif

	ANSI_STRING drive, device;

	char driveBuffer[10];
	sprintf(driveBuffer, "\\??\\%c:", driveLetter);

	char *deviceBuffer;

	// we allocate some memory here that never gets deallocated.  Hopefully
	// that won't be a problem because it only small and shouldn't happen often
	deviceBuffer = (char *)malloc(200);
	int rc = XConvertDOSFilenameToXBOX(directoryName, deviceBuffer);
	if (rc != STATUS_SUCCESS)
		return rc;

	// we need to make sure it has a trailing slash
	int len = strlen(deviceBuffer);
	if (deviceBuffer[len-1] != '\\')
	{
		deviceBuffer[len] = '\\';
		deviceBuffer[len+1] = 0;
	}

	RtlInitAnsiString(&drive, driveBuffer);
	RtlInitAnsiString(&device, deviceBuffer);

	IoDeleteSymbolicLink(&drive);

	NTSTATUS status = IoCreateSymbolicLink(&drive, &device);
	if (!NT_SUCCESS(status))
		return RtlNtStatusToDosError(status);
	else
	{
		setPartitionString(driveLetter, deviceBuffer);
		return STATUS_SUCCESS;
	}
}
