// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Mara (apfel)

#include "log_console.h"
#include "log_console_font.h"

#include <string.h>

#include <hal/video.h>

const static uint16_t margin = 25;

static uint16_t x = margin;
static uint16_t y = margin;

// prints white text on a black backdrop, colors are hardcoded, using an 8x16 font
size_t console_log_function(const char* message, size_t length)
{
    void* framebuffer = XVideoGetFB();
    if (framebuffer == NULL) return 0;

    VIDEO_MODE video_mode = XVideoGetMode();

    unsigned char mask  = 0;
    size_t fb_offset    = 0;
    size_t font_offset  = 0;
    for (size_t i = 0; i < length; i++)
    {
        if (y + 16 >= video_mode.height - margin)
        {
            XVideoFlushFB();

            if (nxLogConsoleAdvance() != nxLogResult_Success) return i;
        }

        if (message[i] == '\n')
        {
            x = margin;
            y += 16 + 1;
            continue;
        }

        fb_offset = (y * video_mode.width + x) * ((video_mode.bpp + 7) / 8);
        for (uint8_t h = 0; h < 16; h++)
        {
            // mask = 0x01; // vmirror
            mask = 0x80;

            for (uint8_t w = 0; w < 8; w++)
            {
                switch (video_mode.bpp)
                {
                case 32:
                    *((uint32_t*)(framebuffer + fb_offset)) = (log_console_font[message[i] * ((8 + 7) / 8) * 16 + font_offset]) & mask ? 0x00FFFFFF : 0x00000000;
                    fb_offset += sizeof(uint32_t);
                    break;

                case 16:
                case 15:
                    *((uint16_t*)(framebuffer + fb_offset)) = (log_console_font[message[i] * ((8 + 7) / 8) * 16 + font_offset]) & mask ? (video_mode.bpp == 16 ? 0xFFFF : 0x7fff) : 0x0000;
                    fb_offset += sizeof(uint16_t);
                    break;
                }

                // mask <<= 1; // vmirror
                mask >>= 1;
            }

            fb_offset += (video_mode.width - 8) * ((video_mode.bpp + 7) / 8);
            font_offset++;
        }

        font_offset = 0;
        x += 8 + 1;

        if (x + 8 > video_mode.width - margin)
        {
            x = margin;
            y += 16 + 1;
        }
    }

    XVideoFlushFB();
    return length;
}

uint8_t nxLogConsoleRegister()
{
    return nxLogRegisterFunction(&console_log_function);
}

void nxLogConsoleUnregister()
{
    nxLogUnregisterFunction(&console_log_function);
}

uint8_t nxLogConsoleMoveCursor(uint16_t _x, uint16_t _y)
{
    if (_x < margin || _y < margin) return nxLogResult_InvalidArguments;

    VIDEO_MODE video_mode = XVideoGetMode();
    if (_x > video_mode.width - margin || _y > video_mode.height - margin) return nxLogResult_InvalidArguments;

    x = _x;
    y = _y;

    return nxLogResult_Success;
}

uint8_t nxLogConsoleClear()
{
    void* framebuffer = XVideoGetFB();
    if (framebuffer == NULL) return nxLogResult_FramebufferRetrievalFailure;

    x = margin;
    y = margin;

    VIDEO_MODE video_mode = XVideoGetMode();
    memset(framebuffer, 0, ((video_mode.bpp + 7) / 8) * (video_mode.width * video_mode.height));

    XVideoFlushFB();
    return nxLogResult_Success;
}

uint8_t nxLogConsoleAdvance()
{
    void* framebuffer = XVideoGetFB();
    if (framebuffer == NULL) return nxLogResult_FramebufferRetrievalFailure;

    VIDEO_MODE video_mode = XVideoGetMode();

    uint32_t base               = ((video_mode.bpp + 7) / 8) * video_mode.width,
             height_one_line    = base * 16,
             height_all_lines   = base * video_mode.height;

    for (uint32_t i = 0; i < height_all_lines - height_one_line; i += 4) *(uint32_t*)(framebuffer + i) = *(uint32_t*)(framebuffer + (height_one_line + i));

    for (uint32_t i = 0; i < height_one_line; i += 4) *(uint32_t*)(framebuffer + ((height_all_lines - height_one_line) + i)) = 0;

    XVideoFlushFB();

    y -= 16 + 1;
    return nxLogResult_Success;
}
