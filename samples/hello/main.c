#include <hal/debug.h>
#include <pbkit/pbkit.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#include "stdio.h"

void main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    switch(pb_init())
    {
        case 0: break;
        default:
            Sleep(2000);
            XReboot();
            return;
    }

    pb_show_debug_screen();

    while(1) {
        debugPrint("Hello NXDK!\n");
        Sleep(2000);
    }

    pb_kill();
    XReboot();
}
