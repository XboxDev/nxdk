#include <hal/debug.h>
#include <hal/video.h>
#include <windows.h>

int main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    while(1) {
        debugPrint("Hello nxdk!\n");
        Sleep(2000);
    }

    return 0;
}
