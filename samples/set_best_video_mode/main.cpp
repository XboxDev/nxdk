#include <hal/debug.h>
#include <hal/video.h>
#include <windows.h>
#include <vector>

// Screen dimension globals
static int SCREEN_WIDTH = 640;
static int SCREEN_HEIGHT = 480;

int main(void)
{
    // set the default video mode
    XVideoSetMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, REFRESH_DEFAULT);

    // create an empty list for the available video modes
    std::vector<VIDEO_MODE> availableVideoModes;

    // save the best video mode here
    VIDEO_MODE bestVideoMode = {0, 0, 0, 0};

    VIDEO_MODE vm;
    int bpp = 32;  // Bits per pixel
    void *p = NULL;  // Initialize to NULL for the first call

    // get the available video modes
    while (XVideoListModes(&vm, bpp, REFRESH_DEFAULT, &p)) {
        availableVideoModes.push_back(vm);

        // ensure height is equal to or better than the current best video mode
        if (vm.height < bestVideoMode.height) {
            continue;
        }

        // ensure width is equal to or better than the current best video mode
        if (vm.width < bestVideoMode.width) {
            continue;
        }

        // ensure bpp is equal to or better than the current best video mode
        if (vm.bpp < bestVideoMode.bpp) {
            continue;
        }

        // ensure refresh is equal to or better than the current best video mode
        if (vm.refresh < bestVideoMode.refresh) {
            continue;
        }

        // save the best video mode
        bestVideoMode = vm;
    }

    SCREEN_WIDTH = bestVideoMode.width;
    SCREEN_HEIGHT = bestVideoMode.height;

    debugPrint("Available video modes:\n");
    for (VIDEO_MODE vm : availableVideoModes) {
        debugPrint("Width: %d, Height: %d, BPP: %d, Refresh: %d\n", vm.width, vm.height, vm.bpp, vm.refresh);
    }
    Sleep(2000);

    // set the best video mode
    XVideoSetMode(SCREEN_WIDTH, SCREEN_HEIGHT, bestVideoMode.bpp, bestVideoMode.refresh);

    debugPrint("Best video mode:\n");
    debugPrint("Width: %d, Height: %d, BPP: %d, Refresh: %d\n", bestVideoMode.width, bestVideoMode.height, bestVideoMode.bpp, bestVideoMode.refresh);

    while (1) {
        Sleep(1000);
    }

    return 0;
}
