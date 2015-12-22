#include <xboxkrnl/xboxkrnl.h>
#include <hal/input.h>
#include <hal/io.h>
#include <string.h>

#define XPAD_ANALOG_BUTTON_INTF 0x30
#define USB_IRQ 1

XPAD_INPUT		g_Pads[4];
XPAD_INPUT		g_DefaultPad;

XMOUSE_INPUT	g_Mouse;

/* Has input been inited already? */
static BOOL bInputOK = FALSE;
static BOOL bInputPolling = FALSE;
static KINTERRUPT InterruptObject;
static KDPC DPCObject;

/* Stores time and XPAD state */
extern struct xpad_data XPAD_current[4];
extern struct xpad_data XPAD_last[4];

/* Stores Mouse state */
extern struct xmouse_data XMOUSE_current;

void BootStartUSB(void);
void USBGetEvents(void);
void BootStopUSB(void);

int GetKeyboardStroke(XKEYBOARD_STROKE *pStroke);

static void __stdcall DPC(PKDPC Dpc, 
					PVOID DeferredContext, 
					PVOID SystemArgument1, 
					PVOID SystemArgument2)
{
	//DPCs allow to use non reentrant procedures (called sequentially, FOR SURE).
	//CAUTION : if you use fpu in DPC you have to save & restore yourself fpu state!!!
	//(fpu=floating point unit, i.e the coprocessor executing floating point opcodes)

	USBGetEvents();
	return;
}

void handle_irqs(int);

static BOOLEAN __stdcall ISR(PKINTERRUPT Interrupt, PVOID ServiceContext)
{
	KeInsertQueueDpc(&DPCObject,NULL,NULL); //calls USBGetEvents() very soon
	*((DWORD*)0xFED00014)=0x80000000;
	return TRUE;
}

void XInput_Init(void)
{
	int i;
	ULONG tmp;
	KIRQL irql;
	ULONG vector;
	if(bInputOK) {
		return;
	}

	bInputOK = TRUE;
	bInputPolling = FALSE;

	// Zero our pad structures
	for(i=0; i<4; i++)
		memset(&g_Pads[i], 0x00, sizeof(XPAD_INPUT));

	// Zero the mouse data
	memset(&g_Mouse, 0x00, sizeof(XMOUSE_INPUT));
	
	// Startup the cromwell usb code
	BootStartUSB();
	
	// Get the current state of our devices
	XInput_GetEvents();
	
	vector = HalGetInterruptVector(USB_IRQ, &irql);
	
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

void XInput_Init_Polling(void)
{
	int i;

	if(bInputOK) {
		return;
	}
	bInputOK = TRUE;
	bInputPolling = TRUE;
	
	// Zero our pad structures
	for(i=0; i<4; i++)
		memset(&g_Pads[i], 0x00, sizeof(XPAD_INPUT));

	// Zero the mouse data
	memset(&g_Mouse, 0x00, sizeof(XMOUSE_INPUT));

	// Startup the cromwell usb code
	BootStartUSB();

	// Get the current state of our devices
	XInput_GetEvents();
}

void XInput_Quit(void)
{
	if(!bInputOK)
		return;

	int i;

	// Stop the cromwell usb code
	BootStopUSB();

	// Zero our pad structures
	for(i=0; i<4; i++)
		memset(&g_Pads[i], 0x00, sizeof(XPAD_INPUT));

	// Zero the mouse data
	memset(&g_Mouse, 0x00, sizeof(XMOUSE_INPUT));
	
	if (!bInputPolling) {
		KeDisconnectInterrupt(&InterruptObject);
	}

	bInputOK = FALSE;
}

void XInput_GetEvents(void)
{
	int pad, button;
	int iLThumbX, iLThumbY, iRThumbX, iRThumbY;
	if (bInputPolling) {
		USBGetEvents();
	}

	for(pad=0; pad<4; pad++)
	{
		g_Pads[pad].hPresent = XPAD_current[pad].hPresent;

		if(!g_Pads[pad].hPresent)
		{
			memset(&g_Pads[pad], 0x00, sizeof(XPAD_INPUT));
			continue;
		}

		// Digital buttons - dpad (up, down, left, right), start, back, left thumb, right thumb
		g_Pads[pad].CurrentButtons.usDigitalButtons	= XPAD_current[pad].pad | (XPAD_current[pad].state << 4);

		// Analog buttons
		g_Pads[pad].CurrentButtons.ucAnalogButtons[0]	
			= (XPAD_current[pad].keys[0] > XPAD_ANALOG_BUTTON_INTF) ? XPAD_current[pad].keys[0] : 0;		// A
		g_Pads[pad].CurrentButtons.ucAnalogButtons[1]	
			= (XPAD_current[pad].keys[1] > XPAD_ANALOG_BUTTON_INTF) ? XPAD_current[pad].keys[1] : 0;		// B
		g_Pads[pad].CurrentButtons.ucAnalogButtons[2]	
			= (XPAD_current[pad].keys[2] > XPAD_ANALOG_BUTTON_INTF) ? XPAD_current[pad].keys[2] : 0;		// X
		g_Pads[pad].CurrentButtons.ucAnalogButtons[3]	
			= (XPAD_current[pad].keys[3] > XPAD_ANALOG_BUTTON_INTF) ? XPAD_current[pad].keys[3] : 0;		// Y
		g_Pads[pad].CurrentButtons.ucAnalogButtons[4]	
			= (XPAD_current[pad].keys[4] > XPAD_ANALOG_BUTTON_INTF) ? XPAD_current[pad].keys[4] : 0;		// Black
		g_Pads[pad].CurrentButtons.ucAnalogButtons[5]	
			= (XPAD_current[pad].keys[5] > XPAD_ANALOG_BUTTON_INTF) ? XPAD_current[pad].keys[5] : 0;		// White


		g_Pads[pad].CurrentButtons.ucAnalogButtons[6]	
			= (XPAD_current[pad].trig_left > XPAD_ANALOG_BUTTON_INTF) ? XPAD_current[pad].trig_left : 0;	// Left trigger
		g_Pads[pad].CurrentButtons.ucAnalogButtons[7]	
			= (XPAD_current[pad].trig_right > XPAD_ANALOG_BUTTON_INTF) ? XPAD_current[pad].trig_right : 0;	// Right trigger

		// Thumbsticks
		g_Pads[pad].sLThumbX			= XPAD_current[pad].stick_left_x;		// Left stick X
		g_Pads[pad].sLThumbY			= XPAD_current[pad].stick_left_y;		// Left stick Y
		g_Pads[pad].sRThumbX			= XPAD_current[pad].stick_right_x;		// Right stick X
		g_Pads[pad].sRThumbY			= XPAD_current[pad].stick_right_y;		// Right stick Y


		// Track changes for buttons
		g_Pads[pad].PressedButtons.usDigitalButtons =  
			g_Pads[pad].CurrentButtons.usDigitalButtons & (g_Pads[pad].LastButtons.usDigitalButtons ^ g_Pads[pad].CurrentButtons.usDigitalButtons);

		g_Pads[pad].LastButtons.usDigitalButtons =  g_Pads[pad].CurrentButtons.usDigitalButtons;

		for(button=0; button<8; button++)
		{
			g_Pads[pad].PressedButtons.ucAnalogButtons[button] = 
				(g_Pads[pad].CurrentButtons.ucAnalogButtons[button] > XPAD_ANALOG_BUTTON_INTF) & 
				(g_Pads[pad].LastButtons.ucAnalogButtons[button] ^ 
				(g_Pads[pad].CurrentButtons.ucAnalogButtons[button] > XPAD_ANALOG_BUTTON_INTF));

			g_Pads[pad].LastButtons.ucAnalogButtons[button] = (g_Pads[pad].CurrentButtons.ucAnalogButtons[button] > XPAD_ANALOG_BUTTON_INTF);
		}
	}

	//Fill combination gamepad
	memset(&g_DefaultPad, 0x00, sizeof(XPAD_INPUT));

	iLThumbX = iLThumbY = iRThumbX = iRThumbY = 0;

	for(pad=0; pad<4; pad++)
	{
		g_DefaultPad.CurrentButtons.usDigitalButtons |= g_Pads[pad].CurrentButtons.usDigitalButtons;
		g_DefaultPad.LastButtons.usDigitalButtons |= g_Pads[pad].LastButtons.usDigitalButtons;
		g_DefaultPad.PressedButtons.usDigitalButtons |= g_Pads[pad].PressedButtons.usDigitalButtons;

		iLThumbX += g_Pads[pad].sLThumbX;
		iLThumbY += g_Pads[pad].sLThumbY;
		iRThumbX += g_Pads[pad].sRThumbX;
		iRThumbY += g_Pads[pad].sRThumbY;

		for(button=0; button<8; button++)
		{
			if(g_Pads[pad].CurrentButtons.ucAnalogButtons[button] >= g_DefaultPad.CurrentButtons.ucAnalogButtons[button])
			{
				g_DefaultPad.CurrentButtons.ucAnalogButtons[button] = g_Pads[pad].CurrentButtons.ucAnalogButtons[button];
			}

			g_DefaultPad.LastButtons.ucAnalogButtons[button] |= g_Pads[pad].LastButtons.ucAnalogButtons[button];
			g_DefaultPad.PressedButtons.ucAnalogButtons[button] |= g_Pads[pad].PressedButtons.ucAnalogButtons[button];
		}
	}

	g_DefaultPad.sLThumbX = (iLThumbX & 0xffff);
	g_DefaultPad.sLThumbY = (iLThumbY & 0xffff);
	g_DefaultPad.sRThumbX = (iRThumbX & 0xffff);
	g_DefaultPad.sRThumbY = (iRThumbY & 0xffff);

	// Mouse status
	memcpy(&g_Mouse, &XMOUSE_current, sizeof(XMOUSE_INPUT));
}

int XInputGetPadCount(void)
{
	int pad;
	int numpads = 0;

	for(pad=0; pad<4; pad++)
	{
		if(XPAD_current[pad].hPresent)
			numpads++;
	}

	return numpads;
}

int XInputGetKeystroke(XKEYBOARD_STROKE *pStroke)
{
	return GetKeyboardStroke(pStroke);
}

XMOUSE_INPUT XInputGetMouseData(void)
{
	return g_Mouse;
}
