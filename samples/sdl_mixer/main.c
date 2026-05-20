#include <SDL.h>
#include <SDL_mixer.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <math.h>
#include <stdint.h>
#include <windows.h>

#define SAMPLE_RATE 22050
#define CHANNELS 1
#define BITS_PER_SAMPLE 16
#define TONE_HZ 440
#define TONE_MS 400

static void write_le16(uint8_t *dst, uint16_t value)
{
    dst[0] = (uint8_t)(value & 0xff);
    dst[1] = (uint8_t)(value >> 8);
}

static void write_le32(uint8_t *dst, uint32_t value)
{
    dst[0] = (uint8_t)(value & 0xff);
    dst[1] = (uint8_t)((value >> 8) & 0xff);
    dst[2] = (uint8_t)((value >> 16) & 0xff);
    dst[3] = (uint8_t)(value >> 24);
}

static uint8_t *create_test_wav(size_t *size)
{
    const uint32_t samples = (SAMPLE_RATE * TONE_MS) / 1000;
    const uint16_t block_align = (CHANNELS * BITS_PER_SAMPLE) / 8;
    const uint32_t data_size = samples * block_align;
    const uint32_t byte_rate = SAMPLE_RATE * block_align;
    uint8_t *wav = (uint8_t *)SDL_malloc(44 + data_size);
    uint32_t i;

    if (!wav) {
        return NULL;
    }

    SDL_memcpy(wav + 0, "RIFF", 4);
    write_le32(wav + 4, 36 + data_size);
    SDL_memcpy(wav + 8, "WAVE", 4);
    SDL_memcpy(wav + 12, "fmt ", 4);
    write_le32(wav + 16, 16);
    write_le16(wav + 20, 1);
    write_le16(wav + 22, CHANNELS);
    write_le32(wav + 24, SAMPLE_RATE);
    write_le32(wav + 28, byte_rate);
    write_le16(wav + 32, block_align);
    write_le16(wav + 34, BITS_PER_SAMPLE);
    SDL_memcpy(wav + 36, "data", 4);
    write_le32(wav + 40, data_size);

    for (i = 0; i < samples; ++i) {
        const double phase = (2.0 * 3.14159265358979323846 * TONE_HZ * i) / SAMPLE_RATE;
        const int16_t sample = (int16_t)(sin(phase) * 12000.0);
        write_le16(wav + 44 + (i * block_align), (uint16_t)sample);
    }

    *size = 44 + data_size;
    return wav;
}

int main(void)
{
    uint8_t *wav_data;
    size_t wav_size;
    SDL_RWops *rw;
    Mix_Chunk *chunk;
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

    wav_data = create_test_wav(&wav_size);
    if (!wav_data) {
        debugPrint("Failed to allocate test WAV\n");
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    rw = SDL_RWFromMem(wav_data, (int)wav_size);
    chunk = rw ? Mix_LoadWAV_RW(rw, 1) : NULL;
    if (!chunk) {
        debugPrint("Mix_LoadWAV_RW failed: %s\n", Mix_GetError());
        SDL_free(wav_data);
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    channel = Mix_PlayChannel(-1, chunk, 0);
    debugPrint("Playing generated WAV on channel %d\n", channel);

    while (Mix_Playing(channel)) {
        Sleep(50);
    }

    Mix_FreeChunk(chunk);
    SDL_free(wav_data);
    Mix_CloseAudio();
    SDL_Quit();

    debugPrint("SDL_mixer sample complete\n");
    while (1) {
        Sleep(1000);
    }

    return 0;
}
