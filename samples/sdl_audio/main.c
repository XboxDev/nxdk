/*
  Based on SDL test/loopwavequeue.c (a7609d9e2ab6c547022a60a35a440ee6cb515ac9)

  Copyright (C) 1997-2019 Sam Lantinga <slouken@libsdl.org>
  Copyright (C) 2020 Jannik Vogel

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

/* Program to load a wave file and loop playing it using SDL sound queueing */

#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include <hal/video.h>
#include <hal/debug.h>

static struct
{
    SDL_AudioSpec spec;
    Uint8 *sound;               /* Pointer to wave data */
    Uint32 soundlen;            /* Length of wave data */
} wave;


/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    SDL_Quit();
    exit(rc);
}

void
loop()
{
    {
        /* The device from SDL_OpenAudio() is always device #1. */
        const Uint32 queued = SDL_GetQueuedAudioSize(1);
        debugPrint("Device has %u bytes queued. Playing: %d\n", (unsigned int) queued, SDL_GetAudioStatus() == SDL_AUDIO_PLAYING);
        if (queued <= 8192) {  /* time to requeue the whole thing? */
            if (SDL_QueueAudio(1, wave.sound, wave.soundlen) == 0) {
                debugPrint("Device queued %u more bytes.\n", (unsigned int) wave.soundlen);
            } else {
                debugPrint("Device FAILED to queue %u more bytes: %s\n", (unsigned int) wave.soundlen, SDL_GetError());
            }
        }
    }
}

int
main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    char filename[4096];

    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Load the SDL library */
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return (1);
    }

    SDL_strlcpy(filename, "nxdk.wav", sizeof(filename));

    /* Load the wave file into memory */
    if (SDL_LoadWAV(filename, &wave.spec, &wave.sound, &wave.soundlen) == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s\n", filename, SDL_GetError());
        quit(1);
    }

    wave.spec.callback = NULL;  /* we'll push audio. */

    /* Initialize fillerup() variables */
    if (SDL_OpenAudio(&wave.spec, NULL) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open audio: %s\n", SDL_GetError());
        SDL_FreeWAV(wave.sound);
        quit(2);
    }

    /*static x[99999]; SDL_QueueAudio(1, x, sizeof (x));*/

    /* Let the audio run */
    SDL_PauseAudio(0);

    /* Note that we stuff the entire audio buffer into the queue in one
       shot. Most apps would want to feed it a little at a time, as it
       plays, but we're going for simplicity here. */
    
    while (1)
    {
        loop();

        SDL_Delay(100);  /* let it play for awhile. */
    }

    /* Clean up on signal */
    SDL_CloseAudio();
    SDL_FreeWAV(wave.sound);
    SDL_Quit();
    return 0;
}

/* vi: set ts=4 sw=4 expandtab: */

