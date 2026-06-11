#include <SDL.h>
#include <SDL_mixer.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <windows.h>

#define SAMPLE_RATE 22050

static void shutdown_audio(Mix_Chunk *chunk, Mix_Music *music)
{
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    if (music) {
        Mix_FreeMusic(music);
    }
    if (chunk) {
        Mix_FreeChunk(chunk);
    }
    Mix_CloseAudio();
    SDL_Quit();
}

int main(void)
{
    Mix_Chunk *chunk = NULL;
    Mix_Music *music = NULL;
    int channel;

    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    debugPrint("nxdk SDL_mixer sample\n");

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        debugPrint("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(SAMPLE_RATE, AUDIO_S16SYS, 2, 1024) < 0) {
        debugPrint("Mix_OpenAudio failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    chunk = Mix_LoadWAV("D:\\sdl_mixer_tone.wav");
    if (!chunk) {
        debugPrint("Mix_LoadWAV failed: %s\n", Mix_GetError());
        shutdown_audio(chunk, music);
        return 1;
    }

    music = Mix_LoadMUS("D:\\sdl_mixer_music.wav");
    if (!music) {
        debugPrint("Mix_LoadMUS failed: %s\n", Mix_GetError());
        shutdown_audio(chunk, music);
        return 1;
    }

    if (Mix_PlayMusic(music, 0) < 0) {
        debugPrint("Mix_PlayMusic failed: %s\n", Mix_GetError());
        shutdown_audio(chunk, music);
        return 1;
    }

    channel = Mix_PlayChannel(-1, chunk, 0);
    if (channel < 0) {
        debugPrint("Mix_PlayChannel failed: %s\n", Mix_GetError());
        shutdown_audio(chunk, music);
        return 1;
    }

    debugPrint("Playing packaged WAV assets on channel %d\n", channel);
    while (Mix_PlayingMusic() || Mix_Playing(channel)) {
        Sleep(50);
    }

    shutdown_audio(chunk, music);
    debugPrint("SDL_mixer sample complete\n");
    while (1) {
        Sleep(1000);
    }

    return 0;
}
