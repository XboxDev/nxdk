#include "../usb_wrapper.h"

unsigned int current_keyboard_key;

// Stuff for handling a keyboard queue
typedef struct _XKEYBOARD_DATA
{
	unsigned char	ucModifiers;
	unsigned char	ucPad;
	unsigned char	ucScanCodes[6];
} XKEYBOARD_DATA;

XKEYBOARD_DATA XKEYBOARD_Current;
XKEYBOARD_DATA XKEYBOARD_Last;

XKEYBOARD_STROKE *pKeyQueue = NULL;
int iKeyWritePos = 0;
int iKeyReadPos = 0;
int iKeyQueueSize = 0;

BOOL bShift = FALSE;
BOOL bCtrl = FALSE;
BOOL bAlt = FALSE;

int InitKeyboardQueue(size_t size);
void FreeKeyboardQueue(void);
void UpdateKeyboardQueue(XKEYBOARD_DATA* pData);

// End of keyboard queue stuff

struct usb_kbd_info {
	struct urb *urb;
	unsigned char kbd_pkt[8];
	unsigned char old[8];

	/*
	struct input_dev dev;
	struct usb_device *usbdev;
	struct urb irq, led;
	struct usb_ctrlrequest dr;
	unsigned char leds, newleds;
	char name[128];
	int open;
	*/
};

static void usb_kbd_irq(struct urb *urb, struct pt_regs *regs)
{
	struct usb_kbd_info *kbd = urb->context;
	int i;

	if (urb->status) return;
	
	memcpy(kbd->kbd_pkt, urb->transfer_buffer, 8);
	
	current_keyboard_key = kbd->kbd_pkt[2];

	UpdateKeyboardQueue((XKEYBOARD_DATA*)kbd->kbd_pkt);
	
		
	usb_submit_urb(urb,GFP_ATOMIC);
		
}

static int usb_kbd_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct urb *urb;
	struct usb_device *udev = interface_to_usbdev (intf);
	struct usb_endpoint_descriptor *ep_irq_in;
	struct usb_endpoint_descriptor *ep_irq_out;
	struct usb_kbd_info *usbk;

	int i, pipe, maxp;
	char *buf;

	usbk=(struct usb_kbd_info *)kmalloc(sizeof(struct usb_kbd_info),0);
	if (!usbk) return -1;

	urb=usb_alloc_urb(0,0);
	if (!urb) return -1;

	usbk->urb=urb;

	ep_irq_in = &intf->altsetting[0].endpoint[0].desc;
	usb_fill_int_urb(urb, udev,
                         usb_rcvintpipe(udev, ep_irq_in->bEndpointAddress),
                         usbk->kbd_pkt, 8, usb_kbd_irq,
                         usbk, 8);

	usb_submit_urb(urb,GFP_ATOMIC);
	usb_set_intfdata(intf,usbk);

	return 0;
}


static void usb_kbd_disconnect(struct usb_interface *intf)
{
	struct usb_kbd_info *usbk = usb_get_intfdata (intf);
	usbprintk("Keyboard disconnected\n ");
	usb_unlink_urb(usbk->urb);
	usb_free_urb(usbk->urb);
	kfree(usbk);
}

static struct usb_device_id usb_kbd_id_table [] = {
	{ USB_INTERFACE_INFO(3, 1, 1) },
	{ }						/* Terminating entry */
};


static struct usb_driver usb_kbd_driver = {
	.owner =		THIS_MODULE,
	.name =			"keyboard",
	.probe =		usb_kbd_probe,
	.disconnect =		usb_kbd_disconnect,
	.id_table =		usb_kbd_id_table,
};

void UsbKeyBoardInit(void)
{
	if(InitKeyboardQueue(40) < 0)
		return;

	if (usb_register(&usb_kbd_driver) < 0) {
		usbprintk("Unable to register Keyboard driver");
		return;
	}       
}

void UsbKeyBoardRemove(void) {
	usb_deregister(&usb_kbd_driver);

	FreeKeyboardQueue();
}

//
// Keyboard queue handling functions
//
#include "keyboardcodes.h"

int InitKeyboardQueue(size_t size)
{
	pKeyQueue = (XKEYBOARD_STROKE*)kmalloc(size * sizeof(XKEYBOARD_STROKE), 0);
	if(!pKeyQueue)
		return -1;

	memset(pKeyQueue, 0x00, size * sizeof(XKEYBOARD_STROKE));

	memset(&XKEYBOARD_Current, 0x00, sizeof(XKEYBOARD_DATA));
	memset(&XKEYBOARD_Last, 0x00, sizeof(XKEYBOARD_DATA));

	iKeyWritePos = 0;
	iKeyReadPos = 0;
	iKeyQueueSize = size;

	return 0;
}

void FreeKeyboardQueue(void)
{
	if(pKeyQueue)
	{
		kfree(pKeyQueue);
		pKeyQueue = NULL;
	}

	iKeyWritePos = 0;
	iKeyReadPos = 0;
	iKeyQueueSize = 0;
}

void UpdateKeyboardQueue(XKEYBOARD_DATA* pData)
{
	XKEYBOARD_DATA keysUp;
	XKEYBOARD_DATA keysDown;
	BOOL bKeyToAdd;

	if(!pKeyQueue)
		return;

	memcpy(&XKEYBOARD_Last, &XKEYBOARD_Current, sizeof(XKEYBOARD_DATA));
	memcpy(&XKEYBOARD_Current, pData, sizeof(XKEYBOARD_DATA));

	keysDown.ucModifiers = XKEYBOARD_Current.ucModifiers; 
	keysDown.ucModifiers &= ~XKEYBOARD_Last.ucModifiers;

	keysUp.ucModifiers = XKEYBOARD_Last.ucModifiers;
	keysUp.ucModifiers &= ~XKEYBOARD_Current.ucModifiers;

	// Keys Pressed
	for(int iCurKey=0; iCurKey<6; iCurKey++)
	{
		bKeyToAdd = TRUE;
		if(XKEYBOARD_Current.ucScanCodes[iCurKey] > 0x03)
		{
			for(int iLastKey=0; iLastKey<6; iLastKey++)
			{
				if(XKEYBOARD_Current.ucScanCodes[iCurKey] == XKEYBOARD_Last.ucScanCodes[iLastKey])
				{
					bKeyToAdd = FALSE;
					break;
				}
			}
		}
		else
			bKeyToAdd = FALSE;

		keysDown.ucScanCodes[iCurKey] = bKeyToAdd ? XKEYBOARD_Current.ucScanCodes[iCurKey] : 0;
	}

	//Keys Released
	for(int iLastKey=0; iLastKey<6; iLastKey++)
	{
		bKeyToAdd = TRUE;
		if(XKEYBOARD_Last.ucScanCodes[iLastKey] > 0x03)
		{
			for(int iCurKey=0; iCurKey<6; iCurKey++)
			{
				if(XKEYBOARD_Last.ucScanCodes[iLastKey] == XKEYBOARD_Current.ucScanCodes[iCurKey])
				{
					bKeyToAdd = FALSE;
					break;
				}
			}
		}
		else
			bKeyToAdd = FALSE;

		keysUp.ucScanCodes[iLastKey] = bKeyToAdd ? XKEYBOARD_Last.ucScanCodes[iLastKey] : 0;
	}

	// Add keys to the queue
	for(int i=0; i<8; i++)
	{
		bKeyToAdd = FALSE;

		if( (keysUp.ucModifiers >> i) & 1 )
		{
			if(i==0 || i==4)
				bCtrl = FALSE;
			if(i==1 || i==5)
				bShift = FALSE;
			if(i==2 || i==6)
				bAlt = FALSE;

			pKeyQueue[iKeyWritePos].ucFlags = XKEYBOARD_KEYUP;
			bKeyToAdd = TRUE;
		}
		if( (keysDown.ucModifiers >> i) & 1 )
		{
			if(i==0 || i==4)
				bCtrl = TRUE;
			if(i==1 || i==5)
				bShift = TRUE;
			if(i==2 || i==6)
				bAlt = TRUE;

			pKeyQueue[iKeyWritePos].ucFlags = 0;
			bKeyToAdd = TRUE;
		}
		if(bKeyToAdd)
		{
			pKeyQueue[iKeyWritePos].ucKeyCode = ucUSBtoKey[i + USB_KEYBOARD_USAGE_LCONTROL];
			pKeyQueue[iKeyWritePos].ucAsciiValue = 0;

			iKeyWritePos++;
			if(iKeyWritePos >= iKeyQueueSize)
				iKeyWritePos = 0;
		}
	}

	for(int i=0; i<6; i++)
	{
		if(keysUp.ucScanCodes[i] > 0x03)
		{
			pKeyQueue[iKeyWritePos].ucKeyCode = ucUSBtoKey[keysUp.ucScanCodes[i]];
			pKeyQueue[iKeyWritePos].ucFlags = XKEYBOARD_KEYUP;

			if( bShift )
				pKeyQueue[iKeyWritePos].ucAsciiValue = ucUSBtoAsciiShifted[keysUp.ucScanCodes[i]];
			else
				pKeyQueue[iKeyWritePos].ucAsciiValue = ucUSBtoAscii[keysUp.ucScanCodes[i]];

			//Need to add flags for shift, ctrl etc here
			if(bShift)
				pKeyQueue[iKeyWritePos].ucFlags |= XKEYBOARD_SHIFT;
			
			if(bCtrl)
				pKeyQueue[iKeyWritePos].ucFlags |= XKEYBOARD_CONTROL;

			if(bAlt)
				pKeyQueue[iKeyWritePos].ucFlags |= XKEYBOARD_ALT;

			iKeyWritePos++;
			if(iKeyWritePos >= iKeyQueueSize)
				iKeyWritePos = 0;
		}
	}

	for(int i=0; i<6; i++)
	{
		if(keysDown.ucScanCodes[i] > 0x03)
		{
			pKeyQueue[iKeyWritePos].ucKeyCode = ucUSBtoKey[keysDown.ucScanCodes[i]];
			pKeyQueue[iKeyWritePos].ucFlags = 0;

			if( bShift )
				pKeyQueue[iKeyWritePos].ucAsciiValue = ucUSBtoAsciiShifted[keysDown.ucScanCodes[i]];
			else
				pKeyQueue[iKeyWritePos].ucAsciiValue = ucUSBtoAscii[keysDown.ucScanCodes[i]];

			//Need to add flags for shift, ctrl etc here
			if(bShift)
				pKeyQueue[iKeyWritePos].ucFlags |= XKEYBOARD_SHIFT;
			
			if(bCtrl)
				pKeyQueue[iKeyWritePos].ucFlags |= XKEYBOARD_CONTROL;

			if(bAlt)
				pKeyQueue[iKeyWritePos].ucFlags |= XKEYBOARD_ALT;

			iKeyWritePos++;
			if(iKeyWritePos >= iKeyQueueSize)
				iKeyWritePos = 0;
		}
	}

}

int GetKeyboardStroke(XKEYBOARD_STROKE *pStroke)
{
	if(!pStroke)
		return -1;

	memset(pStroke, 0x00, sizeof(XKEYBOARD_STROKE));

	if(!pKeyQueue)
		return -1;

	if(iKeyReadPos == iKeyWritePos)
		return -1;

	memcpy(pStroke, &pKeyQueue[iKeyReadPos], sizeof(XKEYBOARD_STROKE));

	iKeyReadPos++;
	if(iKeyReadPos >= iKeyQueueSize)
		iKeyReadPos = 0;

	return 0;
}
