#include <stdio.h>
#include <fileapi.h>
#include <hal/winerror.h>
#include <xboxrt/debug.h>
#include <pbkit/pbkit.h>
#include <hal/xbox.h>

int main()
{
    int ret = pb_init();
    if (ret != 0) {
        XSleep(2000);
        return -1;
    }

    pb_show_debug_screen();

    debugPrint("Content of C:\\\n");

    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    // Like on Windows, "*.*" and "*" will both list all files,
    // no matter whether they contain a dot or not
    hFind = FindFirstFile("C:\\*.*", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        debugPrint("FindFirstHandle() failed!\n");
        XSleep(5000);
        return -1;
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
        debugPrint("error: %x\n", error);
    }

    FindClose(hFind);

    while (1) {
        XSleep(2000);
    }

    pb_kill();
    XReboot();
}
