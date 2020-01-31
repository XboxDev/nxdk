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


#include "pcms162441.h"

#ifndef NXDK
#define debugPrint printf
void Sleep(int duration){}
#else
#include <hal/video.h>
#include <hal/debug.h>
#define SDL_LogError(facility, fmt, ...) debugPrint(fmt, ##__VA_ARGS__)
#endif

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


static void SoundDumpSpec(SDL_AudioSpec *spec)
{
    debugPrint("Spec %p:\n", spec);
    debugPrint("\tfreq: %d\n",spec->freq);
    debugPrint("\tformat: %d\n",spec->format);
    debugPrint("\tchannels: %d\n",spec->channels);
    debugPrint("\tsilence: %d\n",spec->silence);
    debugPrint("\tsamples: %d\n",spec->samples);
    debugPrint("\tsize: %d\n",spec->size);
    debugPrint("\tcallback: %p\n",spec->callback);
    debugPrint("\tuserdata: %p\n",spec->userdata);
}



int
main(void)
{
    /*Doesn't work on the xbox: output is scrambled*/

#ifdef NXDK
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
#endif

    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Load the SDL library */
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        Sleep(8000);
        return (1);
    }


    wave.spec.freq = 44100; 
	wave.spec.format = AUDIO_S16; 
	wave.spec.channels = 2; 
	wave.spec.samples = 4096; 
	wave.spec.callback = NULL;
    wave.spec.userdata = NULL;

    wave.sound = s162441_raw;
    wave.soundlen = s162441_raw_len;

    /* Initialize fillerup() variables */
    if (SDL_OpenAudio(&wave.spec, NULL) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open audio: %s\n", SDL_GetError());
        Sleep(8000);
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
    SDL_Quit();
    return 0;
}

/* vi: set ts=4 sw=4 expandtab: */

