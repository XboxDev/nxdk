#include <hal/debug.h>
#include <hal/video.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <windows.h>
#include <stdbool.h>

const extern int SCREEN_WIDTH;
const extern int SCREEN_HEIGHT;

int main(void) {
  int initialized_SDL   = -1;
  int initialized_TTF   = -1;
  SDL_Window   *window   = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture  *texture  = NULL;

  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
  
  initialized_SDL = SDL_VideoInit(NULL);
  if (initialized_SDL != 0) {
    debugPrint("SDL_VideoInit failed: %s", SDL_GetError());
    Sleep(2000);
    goto cleanup;
  }

  initialized_TTF = TTF_Init();
  if (initialized_TTF != 0) {
    debugPrint("TTF_Init failed: %s", TTF_GetError());
    Sleep(2000);
    goto cleanup;
  }

  SDL_Surface  *surface;
  TTF_Font     *font;

  window = SDL_CreateWindow("NXDK SDL_ttf sample",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);
  if (window == NULL) {
    debugPrint("Window creation failed: %s", SDL_GetError());
    Sleep(2000);
    goto cleanup;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    debugPrint("CreateRenderer failed: %s\n", SDL_GetError());
    Sleep(2000);
    goto cleanup;
  }

  const char* font_path = "vegur-regular.ttf";
  const int font_size = 96;
  font = TTF_OpenFont(font_path, font_size);
  if (font == NULL) {
    debugPrint("Couldn't load font: %s", TTF_GetError());
    Sleep(2000);
    goto cleanup;
  }

  SDL_Color font_color = {0x7F, 0xFF, 0x7F, 0xFF};
  surface = TTF_RenderText_Blended(font, "NXDK", font_color);
  TTF_CloseFont(font);
  if (surface == NULL) {
    debugPrint("TTF_RenderText failed: %s", TTF_GetError());
    Sleep(2000);
    goto cleanup;
  }

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (texture == NULL) {
    debugPrint("Couldn't create texture: %s\n", SDL_GetError());
    Sleep(2000);
    goto cleanup;
  }

  SDL_Rect outputPosition;
  int window_height, window_width;
  SDL_GetWindowSize(window, &window_width, &window_height);
  SDL_QueryTexture(texture, NULL, NULL, &outputPosition.w, &outputPosition.h);
  outputPosition.x = window_width / 2 - outputPosition.w / 2;
  outputPosition.y = window_height / 2 - outputPosition.h / 2;

  /* Main render loop */
  bool running = true;
  while (running) {
    /* Check for events */
   SDL_Event event;
   while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
      default:
        break;
      }
    }
 
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &outputPosition);
    SDL_RenderPresent(renderer);
  }

cleanup:
  if (texture != NULL) {
    SDL_DestroyTexture(texture);
  }
  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);
  }
  if (window != NULL) {
    SDL_DestroyWindow(window);
  }
  if (initialized_TTF == 0) {
    TTF_Quit();
  }
  if (initialized_SDL == 0) {
    SDL_Quit();
  }
  
  return 0;
}
