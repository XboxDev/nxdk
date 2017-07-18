#include <string.h>
// #include <xboxrt/stat.h>
#include <hal/xbox.h>
#include <hal/fileio.h>
#include <xboxkrnl/xboxkrnl.h>

#include <stdio.h>
#include <xboxrt/debug.h>

#define KernelMode 0

void XReboot()
{
	HalReturnToFirmware(HalRebootRoutine);
}

int XGetTickCount()
{
	return KeTickCount;
}

void XSleep(int milliseconds)
{
	LARGE_INTEGER interval;
        interval.QuadPart = (long long) milliseconds * -10000;
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
}

/**
 * Launches an XBE.  Examples of xbePath might be:
 *   c:\\blah.xbe
 *   c:/foo/bar.xbe
 * If the XBE is able to be launched, this method will
 * not return.  If there is a problem, then it return.
 */
void XLaunchXBE(char *xbePath)
{
#if 0
	struct stat statbuf;
	if (stat(xbePath, &statbuf) < 0)
		return;
#endif

    if (LaunchDataPage == NULL)
        LaunchDataPage = MmAllocateContiguousMemory(0x1000);

    if (LaunchDataPage == NULL)
		return;

    MmPersistContiguousMemory(LaunchDataPage, 0x1000, TRUE);

	memset((void*)LaunchDataPage, 0, 0x1000);

	LaunchDataPage->Header.dwLaunchDataType = 0xFFFFFFFF;
	LaunchDataPage->Header.dwTitleId = 0;
	XConvertDOSFilenameToXBOX(xbePath, LaunchDataPage->Header.szLaunchPath);

	// one last thing... xbePath now looks like:
	//   \Device\Harddisk0\Partition2\blah\doom.xbe
	// but it has to look like:
	//   \Device\Harddisk0\Partition2\blah;doom.xbe
	char *lastSlash = strrchr(LaunchDataPage->Header.szLaunchPath, '\\');
	if (lastSlash != NULL)
	{
		*lastSlash = ';';
		HalReturnToFirmware(HalQuickRebootRoutine);
	}

	// if we couldn't find a trailing slash, the conversion to
	// the xbox path mustn't have worked, so we will return
}

int XCreateThread(XThreadCallback callback, void *args1, void *args2)
{
	HANDLE id;
	HANDLE handle;

	NTSTATUS status = PsCreateSystemThreadEx(
		(HANDLE)&handle,
		0,
		65536,
		0,
		&id,
		args1,
		args2,
		FALSE,
		FALSE,
		(PKSYSTEM_ROUTINE)callback);

	if (handle == 0) {
		return -1;
	}

	return (unsigned int)handle;
}
