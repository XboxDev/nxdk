#include <stdint.h>
#include <string.h>
#include <math.h>
#include <hal/video.h>
#include <hal/debug.h>
#include <windows.h>

static void draw_title(const char* title)
{
    extern int nextCol;
    extern int nextRow;
    nextCol = 25;
    nextRow = 25;
    debugPrint("%s", title);
    for(int i = strlen(title); i < 60; i++) {
        debugPrint(" ");
    }
}

int main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    /* Create test image in framebuffer */
    uint32_t *rgbx = (uint32_t*)XVideoGetFB();
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
        GAMMA_RAMP_ENTRY entries[256];

        draw_title("Linear");
        for(int i = 0; i < 256; i++) {
            entries[i].red   = i;
            entries[i].green = i;
            entries[i].blue  = i;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

        draw_title("Gamma correction (Simplified 2.2)");
        for(int i = 0; i < 256; i++) {
            float f = i / (float)0xFF;

            f = powf(f, 1.0f/2.2f);

            entries[i].red   = 0xFF * f;
            entries[i].green = 0xFF * f;
            entries[i].blue  = 0xFF * f;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

        draw_title("Gamma correction (Accurate sRGB)");
        for(int i = 0; i < 256; i++) {
            float f = i / (float)0xFF;

            /* https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf (Listing 30) */
            float flo = f * 12.92f;
            float fhi = powf(f, 1.0f/2.4f) * 1.055f - 0.055f;
            f = (f <= 0.0031308f) ? flo : fhi;

            entries[i].red   = 0xFF * f;
            entries[i].green = 0xFF * f;
            entries[i].blue  = 0xFF * f;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

        draw_title("Tonemap (Reinhard) + Gamma correction (Simplified 2.2)");
        for(int i = 0; i < 256; i++) {
            float f = i / (float)0xFF;

            /* http://filmicworlds.com/blog/why-reinhard-desaturates-your-blacks/ */
            f = f / (f + 1.0f);
            f = powf(f, 1.0f/2.2f);

            entries[i].red   = 0xFF * f;
            entries[i].green = 0xFF * f;
            entries[i].blue  = 0xFF * f;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

        draw_title("Tonemap (Filmic) + Gamma correction (Simplified 2.2)");
        for(int i = 0; i < 256; i++) {
            float f = i / (float)0xFF;

            /* http://filmicworlds.com/blog/why-a-filmic-curve-saturates-your-blacks/ */
            f = f - 0.004;
            if (f < 0.0f) { f = 0.0f; }
            f = (f * (6.2f * f + 0.5f)) / (f * (6.2f * f + 1.7f) + 0.06f);

            entries[i].red   = 0xFF * f;
            entries[i].green = 0xFF * f;
            entries[i].blue  = 0xFF * f;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

        draw_title("Inverse");
        for(int i = 0; i < 256; i++) {
            entries[i].red   = 0xFF - i;
            entries[i].green = 0xFF - i;
            entries[i].blue  = 0xFF - i;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

        draw_title("Red only");
        for(int i = 0; i < 256; i++) {
            entries[i].red   = i;
            entries[i].green = 0;
            entries[i].blue  = 0;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

        draw_title("Green only");
        for(int i = 0; i < 256; i++) {
            entries[i].red   = 0;
            entries[i].green = i;
            entries[i].blue  = 0;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

        draw_title("Blue only");
        for(int i = 0; i < 256; i++) {
            entries[i].red   = 0;
            entries[i].green = 0;
            entries[i].blue  = i;
        }
        XVideoSetGammaRamp(0, entries, 256);
        Sleep(1000);

    }

    /* Unreachable */

    return 0;
}
