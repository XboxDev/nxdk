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

    while (true) {
        // Fetch current GameController state
        SDL_GameControllerUpdate();
        debugClearScreen();
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_A),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_B),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_X),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_X));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_Y),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_Y));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_BACK),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_BACK));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_START),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_START));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_LEFTSTICK),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_LEFTSTICK));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_RIGHTSTICK),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_RIGHTSTICK));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
        debugPrint("\n");
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_DPAD_UP),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_UP));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT),
                   SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
        debugPrint("\n");
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForAxis(SDL_CONTROLLER_AXIS_LEFTX),
                   SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForAxis(SDL_CONTROLLER_AXIS_LEFTY),
                   SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForAxis(SDL_CONTROLLER_AXIS_RIGHTX),
                   SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForAxis(SDL_CONTROLLER_AXIS_RIGHTY),
                   SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT),
                   SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
        debugPrint("%s:%i\n",
                   SDL_GameControllerGetStringForAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT),
                   SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
        XSleep(100);
    }

    SDL_GameControllerClose(gameController);
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    pb_kill();
    return 0;
}
