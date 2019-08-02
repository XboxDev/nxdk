#include <stdio.h>
#include <string.h>
#include <fileapi.h>
#include <winerror.h>
#include <xboxrt/debug.h>
#include <pbkit/pbkit.h>
#include <hal/xbox.h>

int mount_drive_c ()
{
    OBJECT_STRING MountPath = RTL_CONSTANT_STRING("\\??\\C:");
    OBJECT_STRING DrivePath = RTL_CONSTANT_STRING("\\Device\\Harddisk0\\Partition2\\");
    NTSTATUS status;

    status = IoCreateSymbolicLink(&MountPath, &DrivePath);
    if (!NT_SUCCESS(status)) {
        debugPrint("Failed to mount C: drive!\n");
        XSleep(5000);
        return -1;
    }

    return 0;
}

int main()
{
    int ret = pb_init();
    if (ret != 0) {
        XSleep(2000);
        return -1;
    }

    pb_show_debug_screen();

    // Mount C:
    ret = mount_drive_c();
    if (ret != 0) {
        return ret;
    }

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
