#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hal/fileio.h>
#include <xboxkrnl/xboxkrnl.h>
#include <hal/debug.h>
#include <stdbool.h>

// #define DEBUG

static char *currentDirString = NULL;

static char *partitions[] =
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

static int getPartitionIndex(char c)
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

static char *getPartitionString(char c)
{
	int i = getPartitionIndex(c);
	if (i == -1)
		return NULL;
	else
		return partitions[i];
}

static void setPartitionString(char c, char *string)
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
	memcpy(currentDirString, XeImageFileName->Buffer, XeImageFileName->Length);
	currentDirString[XeImageFileName->Length] = '\0';
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
