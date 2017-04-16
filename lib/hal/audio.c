#include <string.h>
#include <hal/audio.h>
#include <xboxkrnl/xboxkrnl.h>

// The foundation for this file came from the Cromwell audio driver by 
// Andy (see his comments below).

// Andy@warmcat.com 2003-03-10:
//
// Xbox PC audio is an AC97 compatible audio controller in the MCPX chip
// http://www.amd.com/us-en/assets/content_type/white_papers_and_tech_docs/24467.pdf
// unlike standard AC97 all the regs appear as MMIO from 0xfec00000
// +0 - +7f = Mixer/Codec regs  <-- Wolfson Micro chip
// +100 - +17f = Busmaster regs

// The Wolfson Micro 9709 Codec fitted to the Xbox is a "dumb" codec that
//  has NO PC-controllable registers.  The only regs it has are the
//  manufacturer and device ID ones.

// S/PDIF comes out of the MCPX device and is controlled by an extra set
//  of busmaster DMA registers at +0x170.  These need their own descriptor
//  separate from the PCM Out, although that descriptor can share audio
//  buffers with PCM out successfully.

#define AUDIO_IRQ 6

static KINTERRUPT InterruptObject;
static KDPC DPCObject;

// global reference to the ac97 device
AC97_DEVICE ac97Device;


static void __stdcall DPC(PKDPC Dpc, 
					PVOID DeferredContext, 
					PVOID SystemArgument1, 
					PVOID SystemArgument2)
{
	//DPCs avoid crashes inside non reentrant user callbacks called by nested ISRs.
	//CAUTION : if you use fpu in DPC you have to save & restore yourself fpu state!!!
	//(fpu=floating point unit, i.e the coprocessor executing floating point opcodes)

	volatile AC97_DEVICE *pac97device;

	pac97device = &ac97Device;
	if (pac97device)
			if (pac97device->callback)
				(pac97device->callback)((void *)pac97device, pac97device->callbackData);

	return;
		}
	
// Although we have to explicitly clear the S/PDIF interrupt sources, in fact
// the way we are set up PCM and S/PDIF are in lockstep and we only listen for
// PCM actions, since S/PDIF is always spooling through the same buffer.
static BOOLEAN __stdcall ISR(PKINTERRUPT Interrupt, PVOID ServiceContext)
{
	// Was the interrupt triggered because we were out of data?
	if ((*((char *)0xFEC00116))&8)
		KeInsertQueueDpc(&DPCObject,NULL,NULL); //calls user callback soon

	//KeInsertQueueDpc queues Dpc and returns TRUE if Dpc not already queued.
	//Dpc will be queued only once. So only one Dpc is fired after ISRs cease fire.
	//DPCs avoid crashes inside non reentrant user callbacks called by nested ISRs.
	//CAUTION : if you use fpu in DPC you have to save & restore yourself fpu state!!!
	//(fpu=floating point unit, i.e the coprocessor executing floating point opcodes)

	// Was the interrupt triggered because of FIFO error?
	if ((*((char *)0xFEC00116))&0x10)
		{
			// Fifo underrun - what should I do here?
		// In case of heavy processing, insert a DPC
		}

	*((char *)0xFEC00116)=0xFF; // clear all int sources
	*((char *)0xFEC00176)=0xFF; // clear all int sources

	return TRUE;
}


void XDumpAudioStatus()
{
	volatile AC97_DEVICE *pac97device = &ac97Device;
	if (pac97device)
	{
		volatile unsigned char *pb = (unsigned char *)pac97device->mmio;
		//debugPrint("CIV=%02x LVI=%02x SR=%04x CR=%02x\n", pb[0x114], pb[0x115], pb[0x116], pb[0x11B]);
	}
}

// Initialises the audio subsystem.  This *must* be done before
// audio will work.  You can pass NULL as the callback, but if you
// do that, it is your responsibility to keep feeding the data to
// XAudioProvideSamples() manually.
//
// note that I currently ignore sampleSizeInBits and numChannels.  They
// are provided to cope with future enhancements. Currently supported samples
// are 16 bits, 2 channels (stereo)
void XAudioInit(int sampleSizeInBits, int numChannels, XAudioCallback callback, void *data)
{
	volatile AC97_DEVICE * pac97device = &ac97Device;
	KIRQL irql;
	ULONG vector;

	pac97device->mmio = (unsigned int *)0xfec00000;
	pac97device->nextDescriptorMod31 = 0;
	pac97device->callback = callback;
	pac97device->callbackData = data;
	pac97device->sampleSizeInBits = sampleSizeInBits;
	pac97device->numChannels = numChannels;

	// initialise descriptors to all 0x00 (no samples)        
	memset((void *)&pac97device->pcmSpdifDescriptor[0], 0, sizeof(pac97device->pcmSpdifDescriptor));
	memset((void *)&pac97device->pcmOutDescriptor[0], 0, sizeof(pac97device->pcmOutDescriptor));

	// perform a cold reset
	pac97device->mmio[0x12C>>2] |= 2;
	
	// wait until the chip is finished resetting...
	while(!(pac97device->mmio[0x130>>2]&0x100))
		;

	// clear all interrupts
	((unsigned char *)pac97device->mmio)[0x116] = 0xFF;
	((unsigned char *)pac97device->mmio)[0x176] = 0xFF;

	// tell the audio chip where it should look for the descriptors
	unsigned int pcmAddress = (unsigned int)&pac97device->pcmOutDescriptor[0];
	unsigned int spdifAddress = (unsigned int)&pac97device->pcmSpdifDescriptor[0];
	pac97device->mmio[0x100>>2] = 0;  // no PCM input
	pac97device->mmio[0x110>>2] = MmGetPhysicalAddress((void *)pcmAddress);
	pac97device->mmio[0x170>>2] = MmGetPhysicalAddress((void *)spdifAddress);

	// default to being silent...
	XAudioPause(pac97device);
	
	// Register our ISR
	vector = HalGetInterruptVector(AUDIO_IRQ, &irql);

	KeInitializeDpc(&DPCObject,&DPC,NULL);

	KeInitializeInterrupt(&InterruptObject,
				&ISR,
				NULL,
				vector,
				irql,
				LevelSensitive,
				FALSE);
	
	KeConnectInterrupt(&InterruptObject);
}

// tell the chip it is OK to play...
void XAudioPlay()
{
	volatile AC97_DEVICE *pac97device = &ac97Device;
	pac97device->mmio[0x118>>2] = (unsigned int)0x1d000000; // PCM out - run, allow interrupts
	pac97device->mmio[0x178>>2] = (unsigned int)0x1d000000; // PCM out - run, allow interrupts
}

// tell the chip it is paused.
void XAudioPause()
{
	volatile AC97_DEVICE *pac97device = &ac97Device;
	pac97device->mmio[0x118>>2] = (unsigned int)0x1c000000; // PCM out - PAUSE, allow interrupts
	pac97device->mmio[0x178>>2] = (unsigned int)0x1c000000; // PCM out - PAUSE, allow interrupts
}

// This is the function you should call when you want to give the
// audio chip some more data.  If you have registered a callback, it
// should call this method.  If you are providing the samples manually,
// you need to make sure you call this function often enough so the
// chip doesn't run out of data
void XAudioProvideSamples(unsigned char *buffer, unsigned short bufferLength, int isFinal)
{
	volatile AC97_DEVICE *pac97device = &ac97Device;
	volatile unsigned char *pb = (unsigned char *)pac97device->mmio;

	unsigned short bufferControl = 0x8000;
	if (isFinal) 
		bufferControl |= 0x4000;

	unsigned int address = (unsigned int)buffer;

	pac97device->pcmOutDescriptor[pac97device->nextDescriptorMod31].bufferStartAddress    = MmGetPhysicalAddress((PVOID)address);
	pac97device->pcmOutDescriptor[pac97device->nextDescriptorMod31].bufferLengthInSamples = bufferLength / (pac97device->sampleSizeInBits / 8);
	pac97device->pcmOutDescriptor[pac97device->nextDescriptorMod31].bufferControl         = bufferControl;
	pb[0x115] = (unsigned char)pac97device->nextDescriptorMod31; // set last active descriptor

	pac97device->pcmSpdifDescriptor[pac97device->nextDescriptorMod31].bufferStartAddress    = MmGetPhysicalAddress((PVOID)address);
	pac97device->pcmSpdifDescriptor[pac97device->nextDescriptorMod31].bufferLengthInSamples = bufferLength / (pac97device->sampleSizeInBits / 8);
	pac97device->pcmSpdifDescriptor[pac97device->nextDescriptorMod31].bufferControl         = bufferControl;
	pb[0x175] = (unsigned char)pac97device->nextDescriptorMod31; // set last active descriptor

	// increment to the next buffer descriptor (rolling around to 0 once you get to 31)
	pac97device->nextDescriptorMod31 = (pac97device->nextDescriptorMod31 +1 ) & 0x1f;
}
