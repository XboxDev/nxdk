#include "../usb_wrapper.h"


/* Stores Mouse state */
struct xmouse_data XMOUSE_current;

struct usb_mouse_info {
	struct urb *urb;
	unsigned char data[8];
	//unsigned char old[8];
};

static void usb_mouse_irq(struct urb *urb, struct pt_regs *regs)
{
	struct usb_mouse_info *mouse = urb->context;
	int i;

	if (urb->status) return;
	
	memcpy(mouse->data, urb->transfer_buffer, 8);

	XMOUSE_current.buttons	= mouse->data[0];
	XMOUSE_current.x		= mouse->data[1];
	XMOUSE_current.y		= mouse->data[2];
	XMOUSE_current.wheel	= mouse->data[3];
		
	usb_submit_urb(urb,GFP_ATOMIC);
		
}

static int usb_mouse_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct urb *urb;
	struct usb_device *udev = interface_to_usbdev (intf);
	struct usb_endpoint_descriptor *ep_irq_in;
	struct usb_endpoint_descriptor *ep_irq_out;
	struct usb_mouse_info *usbmouse;

	int i, pipe, maxp;
	char *buf;

	usbmouse=(struct usb_mouse_info *)kmalloc(sizeof(struct usb_mouse_info),0);
	if (!usbmouse) return -1;

	urb=usb_alloc_urb(0,0);
	if (!urb) return -1;

	usbmouse->urb=urb;

	ep_irq_in = &intf->altsetting[0].endpoint[0].desc;
	usb_fill_int_urb(urb, udev,
                         usb_rcvintpipe(udev, ep_irq_in->bEndpointAddress),
                         usbmouse->data, 8, usb_mouse_irq,
                         usbmouse, 8);

	usb_submit_urb(urb,GFP_ATOMIC);
	usb_set_intfdata(intf,usbmouse);
}


static void usb_mouse_disconnect(struct usb_interface *intf)
{
	struct usb_mouse_info *usbmouse = usb_get_intfdata (intf);
	usbprintk("Mouse disconnected\n ");
	usb_unlink_urb(usbmouse->urb);
	usb_free_urb(usbmouse->urb);
	kfree(usbmouse);
}

static struct usb_device_id usb_mouse_id_table [] = {
	{ USB_INTERFACE_INFO(3, 1, 2) },
	{ }						/* Terminating entry */
};


static struct usb_driver usb_mouse_driver = {
	.owner =		THIS_MODULE,
	.name =			"mouse",
	.probe =		usb_mouse_probe,
	.disconnect =		usb_mouse_disconnect,
	.id_table =		usb_mouse_id_table,
};

void UsbMouseInit(void)
{
	memset(&XMOUSE_current, 0, sizeof(struct xmouse_data));

	if (usb_register(&usb_mouse_driver) < 0) {
		usbprintk("Unable to register Mouse driver");
		return;
	}       
}

void UsbMouseRemove(void) {
	usb_deregister(&usb_mouse_driver);
}
