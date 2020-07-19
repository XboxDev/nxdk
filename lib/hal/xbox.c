#include <string.h>
// #include <xboxrt/stat.h>
#include <hal/xbox.h>
#include <hal/fileio.h>
#include <xboxkrnl/xboxkrnl.h>

#include <stdio.h>
#include <hal/debug.h>

#define KernelMode 0

void XReboot()
{
	HalReturnToFirmware(HalRebootRoutine);
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
