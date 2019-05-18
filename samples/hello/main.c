#include <xboxrt/debug.h>
#include <pbkit/pbkit.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include "stdio.h"

void main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    switch(pb_init())
    {
        case 0: break;
        default:
            XSleep(2000);
            XReboot();
            return;
    }

    pb_show_debug_screen();

    while(1) {
        debugPrint("Hello NXDK!\n");
        XSleep(2000);
    }

    pb_kill();
    XReboot();
}
