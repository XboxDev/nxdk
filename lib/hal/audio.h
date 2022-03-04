// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2003 Andy Green
// SPDX-FileCopyrightText: 2004 Craig Edwards
// SPDX-FileCopyrightText: 2019-2021 Stefan Schmidt
// SPDX-FileCopyrightText: 2020 Jannik Vogel
// SPDX-FileCopyrightText: 2021 Bennet Blischke

#ifndef HAL_AUDIO_H
#define HAL_AUDIO_H

#if defined(__cplusplus)
extern "C"
{
#endif

// This is the signature for the function that the audio
// subsystem will call back to when it has run out of data
typedef void (*XAudioCallback)(void *pac97Device, void *data);

// buffer descriptor from AC97 specification
typedef struct 
{  
	unsigned int   bufferStartAddress;
	unsigned short bufferLengthInSamples;  // 0=no smaples
	unsigned short bufferControl;          // b15=1=issue IRQ on completion, b14=1=last in stream
} AC97_DESCRIPTOR __attribute__ ((aligned (8)));

// represents the current ac97 device
typedef struct 
{
	volatile AC97_DESCRIPTOR pcmSpdifDescriptor[32];
	volatile AC97_DESCRIPTOR pcmOutDescriptor[32];
	volatile unsigned int   *mmio;
	unsigned char            nextDescriptor;
	XAudioCallback           callback;
	void                    *callbackData;
	int                      sampleSizeInBits;
	int                      numChannels;
} AC97_DEVICE  __attribute__ ((aligned (8)));

// The XAudio API is only supposed to be used as a backend. Using SDL2 for 
// audio playback should be preferred for applications.
// Note that currently `sampleSizeInBits` and `numChannels` are ignored. Both
// are provided to cope with future enhancements. Currently supported samples
// are 16 bit, 2 channels (stereo)
void XAudioInit(int sampleSizeInBits, int numChannels, XAudioCallback callback, void *data);
void XAudioPlay(void);
void XAudioPause(void);
void XAudioProvideSamples(unsigned char *buffer, unsigned short bufferLength, int isFinal);

#ifdef __cplusplus
}
#endif

#endif
