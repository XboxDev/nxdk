#ifndef HAL_AUDIO_H
#define HAL_AUDIO_H

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
	AC97_DESCRIPTOR        pcmSpdifDescriptor[32];
	AC97_DESCRIPTOR        pcmOutDescriptor[32];
	volatile unsigned int *mmio;
	volatile unsigned int  nextDescriptorMod31;
	XAudioCallback         callback;
	void                  *callbackData;
	int                    sampleSizeInBits;
	int                    numChannels;
} AC97_DEVICE  __attribute__ ((aligned (8)));

// note that I currently ignore sampleSizeInBits and numChannels.  They
// are provided to cope with future enhancements. Currently supported samples
// are 16 bit, 2 channels (stereo)
void XAudioInit(int sampleSizeInBits, int numChannels, XAudioCallback callback, void *data);
void XAudioPlay();
void XAudioPause();
void XAudioProvideSamples(unsigned char *buffer, unsigned short bufferLength, int isFinal);

#endif
