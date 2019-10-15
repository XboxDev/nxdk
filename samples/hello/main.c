#include <hal/debug.h>
#include <pbkit/pbkit.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#include "stdio.h"

int main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    switch(pb_init())
    {
        case 0: break;
        default:
            Sleep(2000);
            return 1;
    }

    pb_show_debug_screen();

    while(1) {
        debugPrint("Hello NXDK!\n");
        Sleep(2000);
    }

    pb_kill();
    return 0;
}
