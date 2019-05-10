#include <debug.h>
#include <pbkit/pbkit.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <assert.h>
#include <SDL.h>
#include <stdbool.h>

int main() {
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    int ret = pb_init();
    if (ret != 0) {
        XSleep(2000);
        return -1;
    }

    pb_show_debug_screen();

    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    int gameControllerIndex = 0;

    SDL_GameController *gameController = SDL_GameControllerOpen(gameControllerIndex);
    if (gameController == NULL) {
        debugPrint("Could not open GameController %i, please plug in a\n", gameControllerIndex);
        debugPrint("compatible controller into port 1. Exiting...\n");
        SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
        XSleep(2000);
        pb_kill();
        return -1;
    }
    
    debugPrint("GameController name: %s\n", SDL_GameControllerName(gameController));
    debugPrint("GameController Map : %s\n", SDL_GameControllerMapping(gameController));
    debugPrint("GameControllerEvent: %i\n", SDL_GameControllerEventState(SDL_QUERY));
    debugPrint("\n");

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_CONTROLLERDEVICEADDED) {
                debugPrint("Controller %i added. (NOT opened yet)\n", event.cdevice.which);
            } else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
                debugPrint("Controller %i removed, exiting...\n", event.cdevice.which);
                SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
                XSleep(2000);
                pb_kill();
                return -1;
            } else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
                debugPrint("Button \"%s\" pressed\n",
                           SDL_GameControllerGetStringForButton((SDL_GameControllerButton)event.cbutton.button));
            } else if (event.type == SDL_CONTROLLERAXISMOTION) {
                debugPrint("Axis \"%s\" moved, new value: %i\n",
                           SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)event.caxis.axis),
                           event.caxis.value);
            }
        }
    }

    SDL_GameControllerClose(gameController);
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    pb_kill();
    return 0;
}
