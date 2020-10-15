#include <hal/debug.h>
#include <hal/video.h>
#include <SDL.h>
#include <windows.h>
#include <pbkit/pbkit.h>
#include <stdbool.h>

int main(void)
{
  SDL_GameController *pad = NULL;
  bool pbk_init = false, sdl_init = false;

  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  sdl_init = SDL_Init(SDL_INIT_GAMECONTROLLER) == 0;
  if (!sdl_init) {
    debugPrint("SDL_Init failed: %s\n", SDL_GetError());
    goto wait_then_cleanup;
  }

  if (SDL_NumJoysticks() < 1) {
    debugPrint("Please connect gamepad\n");
    goto wait_then_cleanup;
  }

  pad = SDL_GameControllerOpen(0);
  if (pad == NULL) {
    debugPrint("Failed to open gamecontroller 0\n");
    goto wait_then_cleanup;
  }

  pbk_init = pb_init() == 0;
  if (!pbk_init) {
    debugPrint("pbkit init failed\n");
    goto wait_then_cleanup;
  }

  pb_show_front_screen();

  while (1) {
    pb_wait_for_vbl();
    pb_target_back_buffer();
    pb_reset();
    pb_fill(0, 0, 640, 480, 0);
    pb_erase_text_screen();

    SDL_GameControllerUpdate();
    pb_print(
      "Axis:\n"
      "- Lstick: x=%d, y=%d\n"
      "- Rstick: x=%d, y=%d\n"
      "- Ltrig: %d\n"
      "- Rtrig: %d\n"
      "Buttons:\n"
      "- A:%d B:%d X:%d Y:%d\n"
      "- Back:%d Start:%d White:%d Black:%d\n"
      "- Up:%d Down:%d Left:%d Right:%d\n"
      "- Lstick:%d, Rstick:%d\n",
      SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX),
      SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY),
      SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTX),
      SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTY),
      SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT),
      SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_A),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_B),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_X),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_Y),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_BACK),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_START),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_UP),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSTICK),
      SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSTICK));

    pb_draw_text_screen();
    while (pb_busy());
    while (pb_finished());
  }

wait_then_cleanup:
  Sleep(5000);

cleanup:
  if (pbk_init) {
    pb_kill();
  }
  if (pad != NULL) {
    SDL_GameControllerClose(pad);
  }
  if (sdl_init) {
    SDL_Quit();
  }
  
  return 0;
}
