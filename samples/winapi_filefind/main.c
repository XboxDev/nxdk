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

    // Mount C:
    BOOL ret = nxMountDrive('C', "\\Device\\Harddisk0\\Partition2\\");
    if (!ret) {
        debugPrint("Failed to mount C: drive!\n");
        Sleep(5000);
        return 1;
    }

    debugPrint("Content of C:\\\n");

    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    // Like on Windows, "*.*" and "*" will both list all files,
    // no matter whether they contain a dot or not
    hFind = FindFirstFile("C:\\*.*", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        debugPrint("FindFirstHandle() failed!\n");
        Sleep(5000);
        return 1;
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

    DWORD error = GetLastError();
    if (error == ERROR_NO_MORE_FILES) {
        debugPrint("Done!\n");
    } else {
        debugPrint("error: %lx\n", error);
    }

    FindClose(hFind);

    while (1) {
        Sleep(2000);
    }

    return 0;
}
