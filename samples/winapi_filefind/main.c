#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <nxdk/mount.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>

int main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    // Create a variable to check if there was an error in the program
    BOOL errored = false;
    // Create a variable for WinAPI error checking
    DWORD error = GetLastError();
    // Mount C:
    BOOL ret = nxMountDrive('C', "\\Device\\Harddisk0\\Partition2\\");
    if (!ret) {
        errored = true;
        // We can get more information about an error from WinAPI code using GetLastError()
        error = GetLastError();
        debugPrint("Failed to mount C: drive! Reason: %x\n", error);
    }

    debugPrint("Content of C:\\\n");

    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    // Like on Windows, "*.*" and "*" will both list all files,
    // no matter whether they contain a dot or not
    hFind = FindFirstFile("C:\\*.*", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        errored = true;
        error = GetLastError();
        debugPrint("FindFirstHandle() failed! Reason: %x\n", error);
    }

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            debugPrint("Directory: ");
        } else {
            debugPrint("File     : ");
        }

        debugPrint("%s\n", findFileData.cFileName);
    } while (FindNextFile(hFind, &findFileData) != 0);

    debugPrint("\n");

    error = GetLastError();
    if (error == ERROR_NO_MORE_FILES) {
        debugPrint("Done! Re-running in 10 seconds...\n");
    } else {
        errored = true;
        debugPrint("error: %x\n", error);
    }

    FindClose(hFind);

    // Give some time to the user to read the files and directories
    Sleep(10000);

    ret = nxUnmountDrive('C');
    // If there was an error while unmounting (nxUnmountDrive() returns false)
    if (!ret) {
      errored = true;
      error = GetLastError();
      debugPrint("Couldn't unmount C: drive! Reason: %x", error);
    }

    if (errored) {
      // Give some time to the user to read any errors
      Sleep(5000);
      return 1;
    }

    return 0;
}
