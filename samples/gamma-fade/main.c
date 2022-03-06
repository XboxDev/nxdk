#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <nxdk/log_console.h>
#include <hal/video.h>
#include <windows.h>

static float remap(float in_low,  float in_high,
                   float out_low, float out_high,
                   float value)
{
                                 /* [in_low; in_high]       */
    value -= in_low;             /* [0.0; in_high-in_low]   */
    value /= in_high - in_low;   /* [0.0; 1.0]              */
    value *= out_high - out_low; /* [0.0; out_high-out_low] */
    value += out_low;            /* [out_low; out_high]     */
    return value;
}

/* This is also known as `lerp(a, b, t)` */
static float mix(float a, float b, float t)
{
    return a * (1.0f - t) + b * t;
}

/* This is also known as `mix(0.0, 1.0, x)` */
static float linear(float x)
{
    return x;
}

/* This is also known as `smoothstep(0.0, 1.0, x)` */
static float smooth(float x)
{
    return x * x * (3.0f - 2.0f * x);
}

static void set_mix_fade(float red, float green, float blue,
                         float t)
{
    GAMMA_RAMP_ENTRY entries[256];
    for(int i = 0; i < 256; i++) {
        float f = i / (float)0xFF;

        entries[i].red   = 0xFF * mix(f, red,   t);
        entries[i].green = 0xFF * mix(f, green, t);
        entries[i].blue  = 0xFF * mix(f, blue,  t);
    }
    XVideoSetGammaRamp(0, entries, 256);
}

static void set_cinematic_fade(float a_black_input,  float a_white_input,
                               float a_black_output, float a_white_output,
                               float b_black_input,  float b_white_input,
                               float b_black_output, float b_white_output,
                               float t)
{
    GAMMA_RAMP_ENTRY entries[256];
    for(int i = 0; i < 256; i++) {
        float f = i / (float)0xFF;

        /* Interpolate the input and output levels */
        float black_input  = mix(a_black_input,  b_black_input,  t);
        float white_input  = mix(a_white_input,  b_white_input,  t);
        float black_output = mix(a_black_output, b_black_output, t);
        float white_output = mix(a_white_output, b_white_output, t);

        /* Remap our brightness */
        if (f < black_input) { f = black_input; }
        if (f > white_input) { f = white_input; }
        f = remap(black_input, white_input, black_output, white_output, f);
        if (f < black_output) { f = black_output; }
        if (f > white_output) { f = white_output; }

        entries[i].red   = 0xFF * f;
        entries[i].green = 0xFF * f;
        entries[i].blue  = 0xFF * f;
    }
    XVideoSetGammaRamp(0, entries, 256);
}

static float animate_frame(DWORD ts_start, DWORD ts, float duration)
{
    /* Wait for the next frame */
    XVideoWaitForVBlank();

    /* Update timestamp */
    DWORD ts_delta = ts - ts_start;
    float t = remap(0.0f, duration, 0.0f, 1.0f, ts_delta);

    /* Signal completion */
    if (t >= 1.0f) {
        return 1.0f;
    }

    /* Return time in range [0.0; 1.0[ */
    return t;
}

static void do_mix_fade(float red, float green, float blue,
                        float(*interpolator)(float), float duration)
{
    DWORD ts_start;
    float t;

    /* Fade to color */
    ts_start = GetTickCount();
    do {
        t = animate_frame(ts_start, GetTickCount(), duration / 2.0f);
        set_mix_fade(red, green, blue,
                     interpolator(t));
    } while(t < 1.0f);

    /* Fade to image */
    ts_start = GetTickCount();
    do {
        t = animate_frame(ts_start, GetTickCount(), duration / 2.0f);
        set_mix_fade(red, green, blue,
                     interpolator(1.0f - t));
    } while(t < 1.0f);
}

static void do_cinematic_fade(float a_black_input,  float a_white_input,
                              float a_black_output, float a_white_output,
                              float b_black_input,  float b_white_input,
                              float b_black_output, float b_white_output,
                              float(*interpolator)(float), float duration)
{
    DWORD ts_start;
    float t;

    /* Fade to color */
    ts_start = GetTickCount();
    do {
        t = animate_frame(ts_start, GetTickCount(), duration / 2.0f);
        set_cinematic_fade(a_black_input,  a_white_input,
                           a_black_output, a_white_output,
                           b_black_input,  b_white_input,
                           b_black_output, b_white_output,
                           interpolator(t));
    } while(t < 1.0f);

    /* Fade to image */
    ts_start = GetTickCount();
    do {
        t = animate_frame(ts_start, GetTickCount(), duration / 2.0f);
        set_cinematic_fade(a_black_input,  a_white_input,
                           a_black_output, a_white_output,
                           b_black_input,  b_white_input,
                           b_black_output, b_white_output,
                           interpolator(1.0f - t));
    } while(t < 1.0f);
}

static void draw_title(const char* title)
{
    nxLogConsoleMoveCursor(25, 25);
    nxLogPrint(title);

    for (int i = strlen(title); i < 60; i++) nxLogPrint(" ");
}

int main(void)
{
    float duration = 2000.0f;

    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    nxLogConsoleRegister();

    /* Create test image in framebuffer */
    volatile uint32_t *rgbx = (uint32_t*)XVideoGetFB();
    for(unsigned int y = 0; y < 480; y++) {
        unsigned int stripe = y / (480/3);

        /* Border left */
        for(unsigned int x = 0; x < (640-512)/2; x++) {
            *rgbx++ = 0x808080;
        }
        /* 512 pixels gradient */
        for(unsigned int x = 0; x < 256; x++) {
            *rgbx++ = x << (16 - stripe * 8);
            *rgbx++ = x << (16 - stripe * 8);
        }
        /* Border right */
        for(unsigned int x = 0; x < (640-512)/2; x++) {
            *rgbx++ = 0x808080;
        }
    }

    while(1) {

        draw_title("Mix fade to black (Linear)");
        do_mix_fade(0.0f, 0.0f, 0.0f,
                    linear, duration);
        Sleep(1000);

        draw_title("Mix fade to black (Smooth)");
        do_mix_fade(0.0f, 0.0f, 0.0f,
                    smooth, duration);
        Sleep(1000);

        draw_title("Mix fade to white (Linear)");
        do_mix_fade(1.0f, 1.0f, 1.0f,
                    linear, duration);
        Sleep(1000);

        draw_title("Mix fade to white (Smooth)");
        do_mix_fade(1.0f, 1.0f, 1.0f,
                    smooth, duration);
        Sleep(1000);

        draw_title("Mix fade to pink (Linear)");
        do_mix_fade(1.0f, 0.0f, 1.0f,
                    linear, duration);
        Sleep(1000);

        draw_title("Mix fade to pink (Smooth)");
        do_mix_fade(1.0f, 0.0f, 1.0f,
                    smooth, duration);
        Sleep(1000);

        draw_title("Cinematic fade to black (Linear)");
        do_cinematic_fade(0.0f, 1.0f, 0.0f, 1.0f,
                          0.7f, 1.0f, 0.0f, 0.0f,
                          linear, duration);
        Sleep(1000);

        draw_title("Cinematic fade to black (Smooth)");
        do_cinematic_fade(0.0f, 1.0f, 0.0f, 1.0f,
                          0.7f, 1.0f, 0.0f, 0.0f,
                          smooth, duration);
        Sleep(1000);

        draw_title("Cinematic fade to white (Linear)");
        do_cinematic_fade(0.0f, 1.0f, 0.0f, 1.0f,
                          0.0f, 0.3f, 1.0f, 1.0f,
                          linear, duration);
        Sleep(1000);

        draw_title("Cinematic fade to white (Smooth)");
        do_cinematic_fade(0.0f, 1.0f, 0.0f, 1.0f,
                          0.0f, 0.3f, 1.0f, 1.0f,
                          smooth, duration);
        Sleep(1000);

    }

    /* Unreachable */

    return 0;
}
