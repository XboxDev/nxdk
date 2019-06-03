/*
 * Simple XPAD driver for XBOX
 *
 * (c) 2003-07-04, Georg Acher (georg@acher.org)
 *
 * Inspired by linux/drivers/usb/input/xpad.c
 * by Marko Friedemann <mfr@bmx-chemnitz.de>
 *
 */



#include "../usb_wrapper.h"
#include "usb/include/config.h"

#include <xboxkrnl/xboxkrnl.h>

// history for the Rising - falling events
unsigned char xpad_button_history[7];

/* Stores time and XPAD state */
struct xpad_data XPAD_current[4];
struct xpad_data XPAD_last[4];

struct xpad_info
{
	struct urb *urb;
	int num;
	unsigned char data[32];
};

int xpad_num=0;
/*------------------------------------------------------------------------*/ 
static void xpad_irq(struct urb *urb, struct pt_regs *regs)
{
	struct xpad_info *xpi = urb->context;
	unsigned char* data= urb->transfer_buffer;
	
	struct xpad_data *xp=&XPAD_current[xpi->num];
	struct xpad_data *xpo=&XPAD_last[xpi->num];

	/* This hack means the xpad event always gets posted to the
	 * first xpad - avoids problems iterating over multiple xpads
	 * as the xpi->num entries are not reused when xpads are
	 * connected, then removed */

	//struct xpad_data *xp=&XPAD_current[0];
	//struct xpad_data *xpo=&XPAD_last[0];
	
	if (xpi->num<0 || xpi->num>3)
		return;
	
	memcpy(xpo,xp,sizeof(struct xpad_data));
	
	xp->stick_left_x=(short) (((short)data[13] << 8) | data[12]);
	xp->stick_left_y=(short) (((short)data[15] << 8) | data[14]);
	xp->stick_right_x=(short) (((short)data[17] << 8) | data[16]);
	xp->stick_right_y=(short) (((short)data[19] << 8) | data[18]);
	xp->trig_left= data[10];
	xp->trig_right= data[11];
	xp->pad = data[2]&0xf;
	xp->state = (data[2]>>4)&0xf;
	xp->keys[0] = data[4]; // a
	xp->keys[1] = data[5]; // b
	xp->keys[2] = data[6]; // x
	xp->keys[3] = data[7]; // y
	xp->keys[4] = data[8]; // black
	xp->keys[5] = data[9]; // white
	xp->timestamp=jiffies; // FIXME: A more uniform flowing time would be better... 	
	usb_submit_urb(urb,GFP_ATOMIC);

}
/*------------------------------------------------------------------------*/ 

static int GetPadNum(const char* szDevPath)
{
	int pos = (XboxHardwareInfo.Flags & XBOX_HW_FLAG_INTERNAL_USB_HUB) ? 2 : 0;
	int portchar = szDevPath[pos];

	switch(portchar)
	{
	case '3': 
		return 0; break;
	case '4': 
		return 1; break;
	case '1': 
		return 2; break;
	case '2': 
		return 3; break;
	}

	return -1;
}

static int xpad_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct urb *urb;
	struct usb_device *udev = interface_to_usbdev (intf);
	struct usb_endpoint_descriptor *ep_irq_in;
	struct usb_endpoint_descriptor *ep_irq_out;
	struct xpad_info *xpi;

	int padnum = GetPadNum(udev->devpath);
	if(padnum < 0) return -1;


	xpi=kmalloc(sizeof(struct xpad_info),GFP_KERNEL);
	if (!xpi) return -1;

	urb=usb_alloc_urb(0,0);
	if (!urb) return -1;

	xpi->urb=urb;
	xpi->num=padnum; //xpad_num;
	ep_irq_in = &intf->altsetting[0].endpoint[0].desc;
	usb_fill_int_urb(urb, udev,
                         usb_rcvintpipe(udev, ep_irq_in->bEndpointAddress),
                         xpi->data, 32, xpad_irq,
                         xpi, 32);

	usb_submit_urb(urb,GFP_ATOMIC);

	usb_set_intfdata(intf,xpi);
	usbprintk("XPAD #%i connected\n",xpad_num);
	#ifdef XPAD_VIBRA_STARTUP
	{
		// Brum Brum
		char data1[6]={0,6,0,120,0,120};
		char data2[6]={0,6,0,0,0,0};
		int dummy;			

		usb_bulk_msg(udev, usb_sndbulkpipe(udev,2),
			data1, 6, &dummy, 500);
		wait_ms(500);
		usb_bulk_msg(udev, usb_sndbulkpipe(udev,2),
			data2, 6, &dummy, 500);		
	}
	#endif

	XPAD_current[xpi->num].hPresent = 1;

	xpad_num++;

	return 0;
}
/*------------------------------------------------------------------------*/ 
static void xpad_disconnect(struct usb_interface *intf)
{
	struct xpad_info *xpi=usb_get_intfdata (intf);
	usb_unlink_urb(xpi->urb);
	usb_free_urb(xpi->urb);

	memset(&XPAD_current[xpi->num], 0x00, sizeof(struct xpad_data));
	memset(&XPAD_last[xpi->num], 0x00, sizeof(struct xpad_data));

	kfree(xpi);
	xpad_num--;
}
/*------------------------------------------------------------------------*/ 
static struct usb_device_id xpad_ids [] = {
	{ USB_INTERFACE_INFO(88, 66, 0) }, /* Xbox Controller Interfaces */
        { }                            /* Terminating entry */   
};


static struct usb_driver xpad_driver = {
	.owner =	THIS_MODULE,
	.name =		"XPAD",
	.probe =	xpad_probe,
	.disconnect =	xpad_disconnect,
	.id_table =	xpad_ids,
};

/*------------------------------------------------------------------------*/ 
void XPADInit(void)
{
	int n;
	for(n=0;n<4;n++)
	{
		memset(&XPAD_current[n], 0, sizeof(struct xpad_data));
		memset(&XPAD_last[n], 0, sizeof(struct xpad_data));
	}
	memset(&xpad_button_history, 0, sizeof(xpad_button_history));
	
	usbprintk("XPAD probe %p ",xpad_probe);
	if (usb_register(&xpad_driver) < 0) {
		err("Unable to register XPAD driver");
		return;
	}       
}
/*------------------------------------------------------------------------*/ 
void XPADRemove(void) {
	usb_deregister(&xpad_driver);
}

/*------------------------------------------------------------------------*/ 



