#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#include <string.h>
#include <hal/audio.h>
#include <xboxkrnl/xboxkrnl.h>
#include <assert.h>

#define MIN(x,y) ((x)<(y)?(x):(y))

/* The WAV file used is signed 16 bit (LE), 2-channel, 48kHz PCM. The WAV has
 * been converted to a simple header file of raw PCM data for convenience of
 * loading. See Makefile for how to update this file. */
#include "nxdk_wav.h"

volatile unsigned int callback_count;

unsigned char *voice_data;
unsigned int voice_len;
unsigned int voice_pos;

#define NUM_BUFFERS 2
const unsigned short buffer_size = 48*1024;
unsigned char *buffers[NUM_BUFFERS];
unsigned int current_buf;

/* FIXME: Move this constant to a common header file */
#define MAXRAM 0x03FFAFFF

/* Called from DPC. Be careful to save and restore FPU state if floats are
 * used in this callback.
 */
static void provide_samples_callback(void *pac97Device, void *data)
{
    int is_final = (voice_pos+buffer_size) >= voice_len;
    int chunk_size = MIN(voice_len-voice_pos, buffer_size);

    memcpy(buffers[current_buf], voice_data+voice_pos, chunk_size);
    XAudioProvideSamples(buffers[current_buf], chunk_size, is_final);

    if (is_final) {
        voice_pos = 0;
    } else {
        voice_pos = voice_pos+chunk_size;
    }

    current_buf = (current_buf+1) % NUM_BUFFERS;
    callback_count++;
}

int main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    for (int i = 0; i < NUM_BUFFERS; i++) {
        buffers[i] = MmAllocateContiguousMemoryEx(buffer_size, 0, MAXRAM, 0,
            (PAGE_READWRITE | PAGE_WRITECOMBINE));
        assert(buffers[i] != NULL);
    }

    voice_data = nxdk_wav_h_bin;
    voice_len = nxdk_wav_h_bin_len;
    voice_pos = 0;
    current_buf = 0;
    callback_count = 0;

    /* Initialize audio subsystem to begin playback */
    XAudioInit(16, 2, &provide_samples_callback, NULL);

    /* Provide initial samples to begin playing */
    for (int i = 0; i < NUM_BUFFERS; i++) {
        provide_samples_callback(NULL, NULL);
    }

    /* Enable playback */
    debugPrint("Playing voice...\n");
    XAudioPlay();

    /* Audio will be played by hardware, we can loop here (or handle game logic)
     * while it plays in the background. The callback will be called when more
     * samples are required.
     */
    while (1) {
        debugPrint("Number of callbacks: %d\n", callback_count);
        Sleep(500);
    }

    return 0;
}
