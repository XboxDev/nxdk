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

    DWORD error;

    // Mount C:
    BOOL ret = nxMountDrive('C', "\\Device\\Harddisk0\\Partition2\\");

    if (!ret) {
        // There was an error. We can get more information about an error from WinAPI code using GetLastError()
        error = GetLastError();
        debugPrint("Failed to mount C: drive! Error code: %x\n", error);
        goto sleepForever;
    }

    debugPrint("Content of C:\\\n");

    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    // Like on Windows, "*.*" and "*" will both list all files,
    // no matter whether they contain a dot or not
    hFind = FindFirstFile("C:\\*.*", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        error = GetLastError();
        debugPrint("FindFirstHandle() failed! Error code: %x\n", error);
        goto cleanup;
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
        debugPrint("Done!\n");
    } else {
        debugPrint("Error: %lx\n", error);
    }

    FindClose(hFind);

cleanup:
    ret = nxUnmountDrive('C');
    // If there was an error while unmounting
    if (!ret) {
        error = GetLastError();
        debugPrint("Failed to unmount C: drive! Error code: %x", error);
    }
sleepForever:
    while (1) {
        Sleep(2000);
    }

    return 0;
}
