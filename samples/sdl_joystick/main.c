#define _GNU_SOURCE 1     /* See feature_test_macros(7) */
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define NKEYS 6

//#ifndef NXDK
//#define debugPrint printf
//#endif

static const char *event_to_str(Uint32 value)
{
    if(value == SDL_AUDIODEVICEADDED) return "SDL_AUDIODEVICEADDED";
    if(value == SDL_AUDIODEVICEREMOVED) return "SDL_AUDIODEVICEREMOVED";
    if(value == SDL_CONTROLLERAXISMOTION) return "SDL_CONTROLLERAXISMOTION";
    if(value == SDL_CONTROLLERBUTTONDOWN) return "SDL_CONTROLLERBUTTONDOWN";
    if(value == SDL_CONTROLLERBUTTONUP) return "SDL_CONTROLLERBUTTONUP";
    if(value == SDL_CONTROLLERDEVICEADDED) return "SDL_CONTROLLERDEVICEADDED";
    if(value == SDL_CONTROLLERDEVICEREMOVED) return "SDL_CONTROLLERDEVICEREMOVED";
    if(value == SDL_CONTROLLERDEVICEREMAPPED) return "SDL_CONTROLLERDEVICEREMAPPED";
    if(value == SDL_DOLLARGESTURE) return "SDL_DOLLARGESTURE";
    if(value == SDL_DOLLARRECORD) return "SDL_DOLLARRECORD";
    if(value == SDL_DROPFILE) return "SDL_DROPFILE";
    if(value == SDL_DROPTEXT) return "SDL_DROPTEXT";
    if(value == SDL_DROPBEGIN) return "SDL_DROPBEGIN";
    if(value == SDL_DROPCOMPLETE) return "SDL_DROPCOMPLETE";
    if(value == SDL_FINGERMOTION) return "SDL_FINGERMOTION";
    if(value == SDL_FINGERDOWN) return "SDL_FINGERDOWN";
    if(value == SDL_FINGERUP) return "SDL_FINGERUP";
    if(value == SDL_KEYDOWN) return "SDL_KEYDOWN";
    if(value == SDL_KEYUP) return "SDL_KEYUP";
    if(value == SDL_JOYAXISMOTION) return "SDL_JOYAXISMOTION";
    if(value == SDL_JOYBALLMOTION) return "SDL_JOYBALLMOTION";
    if(value == SDL_JOYHATMOTION) return "SDL_JOYHATMOTION";
    if(value == SDL_JOYBUTTONDOWN) return "SDL_JOYBUTTONDOWN";
    if(value == SDL_JOYBUTTONUP) return "SDL_JOYBUTTONUP";
    if(value == SDL_JOYDEVICEADDED) return "SDL_JOYDEVICEADDED";
    if(value == SDL_JOYDEVICEREMOVED) return "SDL_JOYDEVICEREMOVED";
    if(value == SDL_MOUSEMOTION) return "SDL_MOUSEMOTION";
    if(value == SDL_MOUSEBUTTONDOWN) return "SDL_MOUSEBUTTONDOWN";
    if(value == SDL_MOUSEBUTTONUP) return "SDL_MOUSEBUTTONUP";
    if(value == SDL_MOUSEWHEEL) return "SDL_MOUSEWHEEL";
    if(value == SDL_MULTIGESTURE) return "SDL_MULTIGESTURE";
    if(value == SDL_QUIT) return "SDL_QUIT";
    if(value == SDL_SYSWMEVENT) return "SDL_SYSWMEVENT";
    if(value == SDL_TEXTEDITING) return "SDL_TEXTEDITING";
    if(value == SDL_TEXTINPUT) return "SDL_TEXTINPUT";
    if(value == SDL_USEREVENT) return "SDL_USEREVENT";
    if(value == SDL_WINDOWEVENT) return "SDL_WINDOWEVENT";

    return "Unknown";
}

static const char *button_to_str(Uint8 value)
{
    if(value == SDL_CONTROLLER_BUTTON_A) return "SDL_CONTROLLER_BUTTON_A";
    if(value == SDL_CONTROLLER_BUTTON_B) return "SDL_CONTROLLER_BUTTON_B";
    if(value == SDL_CONTROLLER_BUTTON_X) return "SDL_CONTROLLER_BUTTON_X";
    if(value == SDL_CONTROLLER_BUTTON_Y) return "SDL_CONTROLLER_BUTTON_Y";
    if(value == SDL_CONTROLLER_BUTTON_BACK) return "SDL_CONTROLLER_BUTTON_BACK";
    if(value == SDL_CONTROLLER_BUTTON_GUIDE) return "SDL_CONTROLLER_BUTTON_GUIDE";
    if(value == SDL_CONTROLLER_BUTTON_START) return "SDL_CONTROLLER_BUTTON_START";
    if(value == SDL_CONTROLLER_BUTTON_LEFTSTICK) return "SDL_CONTROLLER_BUTTON_LEFTSTICK";
    if(value == SDL_CONTROLLER_BUTTON_RIGHTSTICK) return "SDL_CONTROLLER_BUTTON_RIGHTSTICK";
    if(value == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) return "SDL_CONTROLLER_BUTTON_LEFTSHOULDER";
    if(value == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) return "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER";
    if(value == SDL_CONTROLLER_BUTTON_DPAD_UP) return "SDL_CONTROLLER_BUTTON_DPAD_UP";
    if(value == SDL_CONTROLLER_BUTTON_DPAD_DOWN) return "SDL_CONTROLLER_BUTTON_DPAD_DOWN";
    if(value == SDL_CONTROLLER_BUTTON_DPAD_LEFT) return "SDL_CONTROLLER_BUTTON_DPAD_LEFT";
    if(value == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) return "SDL_CONTROLLER_BUTTON_DPAD_RIGHT";
    if(value == SDL_CONTROLLER_BUTTON_MAX) return "SDL_CONTROLLER_BUTTON_MAX";

    /*value can't be SDL_CONTROLLER_BUTTON_INVALID because it's -1 and value is unsigned*/
    return "SDL_CONTROLLER_BUTTON_INVALID";
}

static const char *axis_to_str(Uint8 value)
{
    if(value == SDL_CONTROLLER_AXIS_LEFTX) return "SDL_CONTROLLER_AXIS_LEFTX";
    if(value == SDL_CONTROLLER_AXIS_LEFTY) return "SDL_CONTROLLER_AXIS_LEFTY";
    if(value == SDL_CONTROLLER_AXIS_RIGHTX) return "SDL_CONTROLLER_AXIS_RIGHTX";
    if(value == SDL_CONTROLLER_AXIS_RIGHTY) return "SDL_CONTROLLER_AXIS_RIGHTY";
    if(value == SDL_CONTROLLER_AXIS_TRIGGERLEFT) return "SDL_CONTROLLER_AXIS_TRIGGERLEFT";
    if(value == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) return "SDL_CONTROLLER_AXIS_TRIGGERRIGHT";
    if(value == SDL_CONTROLLER_AXIS_MAX) return "SDL_CONTROLLER_AXIS_MAX";

    /*see button_to_str*/
    return "SDL_CONTROLLER_AXIS_INVALID";
}

static const char *hat_to_str(Uint8 value)
{
    if(value == SDL_HAT_LEFTUP) return "SDL_HAT_LEFTUP";
    if(value == SDL_HAT_LEFTUP) return "SDL_HAT_LEFTUP";
    if(value == SDL_HAT_LEFT) return "SDL_HAT_LEFT";
    if(value == SDL_HAT_LEFTDOWN) return "SDL_HAT_LEFTDOWN";
    if(value == SDL_HAT_UP) return "SDL_HAT_UP";
    if(value == SDL_HAT_CENTERED) return "SDL_HAT_CENTERED";
    if(value == SDL_HAT_DOWN) return "SDL_HAT_DOWN";
    if(value == SDL_HAT_RIGHTUP) return "SDL_HAT_RIGHTUP";
    if(value == SDL_HAT_RIGHT) return "SDL_HAT_RIGHT";
    if(value == SDL_HAT_RIGHTDOWN) return "SDL_HAT_RIGHTDOWN";

    return "Unknown hat value";
}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Event event;
    char **tokey;
    SDL_Renderer *renderer;
    char *tmp;
    int njoy;

    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) < 0) {
        debugPrint("could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");


    debugPrint("Vidoe inited !\n");
    Sleep(2000);
    window = SDL_CreateWindow(
                "joystick-test",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                0
                );
    if (window == NULL) {
        debugPrint("could not create window: %s\n", SDL_GetError());
        return 1;
    }
    screenSurface = SDL_GetWindowSurface(window);
    
    njoy = SDL_NumJoysticks();
    if(!njoy){
        debugPrint("No(%d) joystick(s) detected, bailing out\n", njoy);
        exit(0);
    }
    debugPrint("%d joystick(s) detected\n", njoy);

#if 0
    SDL_GameController *sgc = SDL_GameControllerOpen(0);
    if (sgc == NULL) {
      debugPrint("Joystick Error: %s", SDL_GetError());
      Sleep(2000);
    }else{
        debugPrint("Gamepad detected: %p\n",sgc);
        Sleep(2000);
    }

#else
    SDL_Joystick* joy = SDL_JoystickOpen(0);
//    int joy=0;
    int num_axes    = SDL_JoystickNumAxes(joy);
    int num_buttons = SDL_JoystickNumButtons(joy);
    int num_hats    = SDL_JoystickNumHats(joy);
    int num_balls   = SDL_JoystickNumBalls(joy);

    debugPrint("Joystick %d has: %d axes, %d buttons, %d hats and %d balls\n",
               0, num_axes,num_buttons,num_hats,num_balls);
#endif

//    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
    debugPrint("Waiting for events\n");
    do{
        while(SDL_WaitEvent(&event)){
//            debugPrint("Got event %d: %s\n",event.type, event_to_str(event.type));
    //        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
    //        SDL_UpdateWindowSurface(window);
            if(event.type == SDL_QUIT)
                exit(0);

            if(event.type == SDL_JOYAXISMOTION){
                debugPrint("Joystick %d axis %d value changed to: %d\n", 
                       event.jaxis.which, 
                       event.jaxis.axis,
                       event.jaxis.value);
    //            continue;
            }

            if(event.type == SDL_JOYBUTTONDOWN){
                debugPrint("Joystick %d button %d pressed\n", 
                       event.jbutton.which, 
                       event.jbutton.button);

    //            continue;
            }

            if(event.type == SDL_JOYBUTTONUP){
                debugPrint("Joystick %d button %d released\n", 
                       event.jbutton.which, 
                       event.jbutton.button);
    //            continue;
            }

            if(event.type == SDL_JOYHATMOTION){
                debugPrint("Joystick %d hat %d: %s\n", event.jhat.which, event.jhat.hat, hat_to_str(event.jhat.value));
            }
#if 0
            if(event.type == SDL_CONTROLLERAXISMOTION){
                debugPrint("Controller %d axis %s: %d\n", event.caxis.which, axis_to_str(event.caxis.axis), event.caxis.value);
            }

            if(event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP){
                debugPrint("Controller %d button %s: %s\n", 
                       event.cbutton.which,
                       button_to_str(event.cbutton.button),
                       (event.cbutton.state == SDL_PRESSED) ? "Pressed" : "Released");
            }
#endif
        }
    }while(1);

    SDL_Quit();
    return 0;
}

