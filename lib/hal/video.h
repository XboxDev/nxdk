// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2005 Robin Mulloy
// SPDX-FileCopyrightText: 2006 Tom Burns
// SPDX-FileCopyrightText: 2006 Richard Osborne
// SPDX-FileCopyrightText: 2017-2021 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2020 Jannik Vogel
// SPDX-FileCopyrightText: 2022 Ryan Wendland

#ifndef HAL_VIDEO_H
#define HAL_VIDEO_H

#include <xboxkrnl/xboxkrnl.h>

#if defined(__cplusplus)
extern "C"
{
#endif

// Defines for frame buffer
#define VIDEO_BASE				0xFD000000

// Hardware access macros
#define VIDEOREG(x) (*(volatile unsigned int*)(VIDEO_BASE + (x)))
#define VIDEOREG16(x) (*(volatile unsigned short*)(VIDEO_BASE + (x)))
#define VIDEOREG8(x) (*(volatile unsigned char*)(VIDEO_BASE + (x)))

// Defines for video encoder options
#define VIDEO_ENC_GET_SETTINGS      6
#define VIDEO_ENC_VIDEOENABLE       9
#define VIDEO_ENC_FLICKERFILTER     11
#define VIDEO_ENC_SOFTEN_FILTER     14

// Flags for video settings
#define VIDEO_MODE_720P   0x20000
#define VIDEO_MODE_1080I  0x40000
#define VIDEO_MODE_480P   0x80000
#define VIDEO_60Hz   0x400000
#define VIDEO_50Hz   0x800000
#define VIDEO_WIDESCREEN  0x010000
#define VIDEO_LETTERBOX   0x100000

#define VIDEO_ADAPTER_MASK  0x000000FF
#define VIDEO_STANDARD_MASK 0x0000FF00

//Defines for refresh rates
#define REFRESH_50HZ                50
#define REFRESH_60HZ                60
#define REFRESH_DEFAULT             0

// Structure to hold details about a video mode;
typedef struct _VIDEO_MODE
{
	int		width;
	int		height;
	int		bpp;
	int		refresh;
} VIDEO_MODE;

typedef struct _GAMMA_RAMP_ENTRY
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} GAMMA_RAMP_ENTRY;


DWORD XVideoGetEncoderSettings(void);
unsigned char* XVideoGetFB(void);
void XVideoSetFB(unsigned char *fb);
VIDEO_MODE XVideoGetMode(void);
void XVideoFlushFB(void);

void XVideoSetFlickerFilter(int level);
BOOL XVideoSetMode(int width, int height, int bpp, int refresh);
void XVideoSetSoftenFilter(BOOL enable);
void XVideoSetVideoEnable(BOOL enable);
void XVideoSetGammaRamp(unsigned int offset, const GAMMA_RAMP_ENTRY *entries, unsigned int count);

/*
Lists the video modes available at the given colour depth and refresh rate.
On the first call the argument p must be a pointer to a void pointer
initialized to NULL. This is used internally to maintain context between
successive calls. If a matching video mode is found then XVideoListModes will
return TRUE and the video mode details will be written to the VIDEO_MODE vm
structure. If no matching video mode is found the function will return FALSE.
Repeatedly calling the function allows all matching video modes to be read out.
If a value of 0 is provided for the refresh rate the current refresh rate is used.
If a value of 0 is provided for the bpp a default value of 32bpp is used.
*/
BOOLEAN XVideoListModes(VIDEO_MODE *vm, int bpp, int refresh, void **p);

void XVideoWaitForVBlank(void);
unsigned char* XVideoGetVideoBase(void);
int XVideoVideoMemorySize(void);

#ifdef __cplusplus
}
#endif

#endif
