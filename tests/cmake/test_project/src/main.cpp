
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <nxdk/net.h>
#include <pbkit/pbkit.h>

#include "c_file.h"

extern "C" int add_ten(int val);

//! WARNING: This file is only intended to verify that a full-featured build is
//! achievable. Do not use this as an example for building real apps, no error
//! checking is performed.
int main() {
  verify_c_file_builds();

  pb_init();

  SDL_Init(SDL_INIT_GAMECONTROLLER);
  TTF_Init();

  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

  int foo = add_ten(100);

  nx_net_parameters_t network_config{NX_NET_AUTO, NX_NET_AUTO, 0, 0, 0, 0, 0};
  nxNetInit(&network_config);

  pb_kill();
  return 0;
}
