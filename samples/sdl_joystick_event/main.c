#include <debug.h>
#include <pbkit/pbkit.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <assert.h>
#include <SDL.h>
#include <stdbool.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

int main() {
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    int ret = pb_init();
    if (ret != 0) {
        XSleep(2000);
        return -1;
    }

    pb_show_debug_screen();

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    int joystickIndex = 0;
    SDL_Joystick *joy = SDL_JoystickOpen(joystickIndex);
    if (joy == NULL) {
        debugPrint("Could not open Joystick %i, exiting...", joystickIndex);
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
        XSleep(2000);
        pb_kill();
        return -1;
    }

    debugPrint("Number of axes   : %2i\n", SDL_JoystickNumAxes(joy));
    debugPrint("Number of buttons: %2i\n", SDL_JoystickNumButtons(joy));
    debugPrint("Number of balls  : %2i\n", SDL_JoystickNumBalls(joy));
    debugPrint("Number of hats   : %2i\n", SDL_JoystickNumHats(joy));
    debugPrint("\n");

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_JOYBUTTONDOWN) {
                debugPrint("Button %i pressed\n", event.jbutton.button);
            } else if (event.type == SDL_JOYAXISMOTION) {
                debugPrint("Axis %i moved, new value: %i\n", event.jaxis.axis, event.jaxis.value);
            } else if (event.type == SDL_JOYHATMOTION) {
                debugPrint("Hat %i moved, new value: %i\n", event.jhat.hat, event.jhat.value);
            } else if (event.type == SDL_JOYDEVICEREMOVED) {
                if (event.jdevice.which == joystickIndex) {
                    debugPrint("Joystick %i removed, exiting...", event.jdevice.which);
                    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
                    XSleep(2000);
                    pb_kill();
                    break;
                }
            }
        }
    }

    SDL_JoystickClose(joy);
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    pb_kill();

    return 0;
}
