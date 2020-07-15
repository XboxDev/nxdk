#include <hal/debug.h>
#include <hal/video.h>
#include <nxdk/mount.h>
#include <windows.h>

int main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    // Create a variable to check if there was any error in the program
    BOOL errored = false;
    // Mount some drives for demonstration purposes
    BOOL ret;
    ret = nxMountDrive('C', "\\Device\\Harddisk0\\Partition2\\");
    if (!ret) {
        errored = true;
        // Additional error info can be retrieved with GetLastError()
        DWORD error = GetLastError();
        debugPrint("Failed to mount C: drive! Reason: %x\n", error);
    }
    ret = nxMountDrive('E', "\\Device\\Harddisk0\\Partition1\\");
    if (!ret) {
        errored = true;
        DWORD error = GetLastError();
        debugPrint("Failed to mount E: drive! Reason: %x\n", error);
    }

    // Retrieve drive bitmaks. Every bit represents one drive letter
    DWORD driveBits = GetLogicalDrives();
    if (driveBits == 0 && GetLastError() != ERROR_SUCCESS) {
        errored = true;
        DWORD error = GetLastError();
        debugPrint("Failed to retrieve drive bitmask! Reason: %x\n", error);
    }
    debugPrint("Drive bitmask: 0x%x\n\n", driveBits);


    // Reserve buffer long enough for all possible drive strings plus null-terminator
    char buffer[26 * 4 + 1];
    // IMPORTANT: The size passed to GetLogicalDriveStringsA is WITHOUT the null-terminator, even though it gets written
    DWORD charsWritten = GetLogicalDriveStringsA(sizeof(buffer)-1, buffer);
    if (charsWritten == 0) {
        errored = true;
        DWORD error = GetLastError();
        debugPrint("Failed to retrieve drive strings! Reason: %x\n", error);
    }

    if (charsWritten > sizeof(buffer) - 1) {
        // Can't happen here as our buffer is large enough to cover all possibilities
        debugPrint("Buffer for GetLogicalDriveStringsA too small!\n");
        Sleep(5000);
        return 1;
    }

    debugPrint("Drives found:\n");
    char *drive = buffer;
    while (drive < buffer + charsWritten) {
        debugPrint("%s\n", drive);
        while(*drive++);
    }
    debugPrint("\nDone! Re-running in 10 seconds...");

    // Give some time to the user to read the drives
    Sleep(10000);

    ret = nxUnmountDrive('C');
    // If there was an error while unmounting (nxUnmountDrive() returns false)
    if (!ret) {
      errored = true;
      DWORD error = GetLastError();
      debugPrint("Couldn't unmount C: drive! Reason: %x\n. Trying to unmount E: drive...\n", error);
    }

    ret = nxUnmountDrive('E');
    if (!ret) {
      errored = true;
      DWORD error = GetLastError();
      debugPrint("Couldn't unmount E: drive! Reason: %s\n", error);
    }

    if (errored) {
      // Give some time to the user to read any errors
      Sleep(5000);
    	return 1;
    }

    return 0;
}
