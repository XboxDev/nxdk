#include <stdbool.h>
#include <hal/video.h>
#include <nxdk/log_console.h>
#include <windows.h>

int main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    nxLogConsoleRegister();

    while (true)
    {
        nxLogPrint("Hello nxdk!\n");
        Sleep(1000);
    }

    return 0;
}
