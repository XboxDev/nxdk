/******************************************************************************/
/*																              */
/*  File: hub.h                                                               */
/*  bkenwright@xbdev.net  -  www.xbdev.net                                    */
/*                                                                            */
/******************************************************************************/

#ifndef		__HUB__
#define		__HUB__

#include <usb/ohci.h>



/******************************************************************************/
/*                          USB CONSTANTS                                     */
/******************************************************************************/

/*
 * Device and/or Interface Class codes
 */
#define USB_CLASS_PER_INTERFACE		0	/* for DeviceClass */
#define USB_CLASS_AUDIO			1
#define USB_CLASS_COMM			2
#define USB_CLASS_HID			3
#define USB_CLASS_PRINTER		7
#define USB_CLASS_MASS_STORAGE		8
#define USB_CLASS_HUB			9
#define USB_CLASS_DATA			10
#define USB_CLASS_VENDOR_SPEC		0xff

/*
 * Descriptor types
 */
#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE		0x04
#define USB_DT_ENDPOINT			0x05

#define USB_DT_HUB			0x29
#define USB_DT_HID			0x21
#define USB_DT_REPORT			0x22
#define USB_DT_PHYSICAL			0x23

/*
 * Descriptor sizes per descriptor type
 */
#define USB_DT_DEVICE_SIZE		18
#define USB_DT_CONFIG_SIZE		9
#define USB_DT_INTERFACE_SIZE		9
#define USB_DT_ENDPOINT_SIZE		7
#define USB_DT_ENDPOINT_AUDIO_SIZE	9	/* Audio extension */
#define USB_DT_HUB_NONVAR_SIZE		7

/*
 * USB Request Type and Endpoint Directions
 */
#define USB_DIR_OUT			0
#define USB_DIR_IN			0x80

#define USB_ENDPOINT_NUMBER_MASK	0x0f	/* in bEndpointAddress */
#define USB_ENDPOINT_DIR_MASK		0x80

#define USB_ENDPOINT_XFERTYPE_MASK	0x03	/* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL	0
#define USB_ENDPOINT_XFER_ISOC		1
#define USB_ENDPOINT_XFER_BULK		2
#define USB_ENDPOINT_XFER_INT		3

/*
 * USB Packet IDs (PIDs)
 */
#define USB_PID_OUT			0xe1
#define USB_PID_IN			0x69
#define USB_PID_SETUP			0x2d

/*
 * Standard requests
 */
#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
/* 0x02 is reserved */
#define USB_REQ_SET_FEATURE		0x03
/* 0x04 is reserved */
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B
#define USB_REQ_SYNCH_FRAME		0x0C

/*
 * HIDD requests
 */
#define USB_REQ_GET_REPORT		0x01
#define USB_REQ_GET_IDLE		0x02
#define USB_REQ_GET_PROTOCOL		0x03
#define USB_REQ_SET_REPORT		0x09
#define USB_REQ_SET_IDLE		0x0A
#define USB_REQ_SET_PROTOCOL		0x0B

#define USB_TYPE_STANDARD		(0x00 << 5)
#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_TYPE_VENDOR			(0x02 << 5)
#define USB_TYPE_RESERVED		(0x03 << 5)

#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE		0x01
#define USB_RECIP_ENDPOINT		0x02
#define USB_RECIP_OTHER			0x03

#define USB_HID_RPT_INPUT		0x01
#define USB_HID_RPT_OUTPUT		0x02
#define USB_HID_RPT_FEATURE		0x03

/*
 * Request target types.
 */
#define USB_RT_DEVICE			0x00
#define USB_RT_INTERFACE		0x01
#define USB_RT_ENDPOINT			0x02

#define USB_RT_HUB			(USB_TYPE_CLASS | USB_RECIP_DEVICE)
#define USB_RT_PORT			(USB_TYPE_CLASS | USB_RECIP_OTHER)

#define USB_RT_HIDD			(USB_TYPE_CLASS | USB_RECIP_INTERFACE)


/******************************************************************************/
/*                              HUB CONTANTS                                  */
/******************************************************************************/




/*
 * Hub Class feature numbers
 */
#define C_HUB_LOCAL_POWER	0
#define C_HUB_OVER_CURRENT	1

/*
 * Port feature numbers
 */
#define USB_PORT_FEAT_CONNECTION	0
#define USB_PORT_FEAT_ENABLE		1
#define USB_PORT_FEAT_SUSPEND		2
#define USB_PORT_FEAT_OVER_CURRENT	3
#define USB_PORT_FEAT_RESET		4
#define USB_PORT_FEAT_POWER		8
#define USB_PORT_FEAT_LOWSPEED		9
#define USB_PORT_FEAT_C_CONNECTION	16
#define USB_PORT_FEAT_C_ENABLE		17
#define USB_PORT_FEAT_C_SUSPEND		18
#define USB_PORT_FEAT_C_OVER_CURRENT	19
#define USB_PORT_FEAT_C_RESET		20


#pragma pack( push, 1 )
typedef struct 
{
	__u16 wPortStatus;
	__u16 wPortChange;	
} usb_port_status;
#pragma pack( pop )


/* wPortStatus bits */
#define USB_PORT_STAT_CONNECTION	0x0001
#define USB_PORT_STAT_ENABLE		0x0002
#define USB_PORT_STAT_SUSPEND		0x0004
#define USB_PORT_STAT_OVERCURRENT	0x0008
#define USB_PORT_STAT_RESET		0x0010
#define USB_PORT_STAT_POWER		0x0100
#define USB_PORT_STAT_LOW_SPEED		0x0200

/* wPortChange bits */
#define USB_PORT_STAT_C_CONNECTION	0x0001
#define USB_PORT_STAT_C_ENABLE		0x0002
#define USB_PORT_STAT_C_SUSPEND		0x0004
#define USB_PORT_STAT_C_OVERCURRENT	0x0008
#define USB_PORT_STAT_C_RESET		0x0010

/* wHubCharacteristics (masks) */
#define HUB_CHAR_LPSM		0x0003
#define HUB_CHAR_COMPOUND	0x0004
#define HUB_CHAR_OCPM		0x0018

#pragma pack( push, 1 )
typedef struct 
{
	__u16 wHubStatus;
	__u16 wHubChange;
} usb_hub_status ;
#pragma pack( pop )

/*
 *Hub Status & Hub Change bit masks
 */
#define HUB_STATUS_LOCAL_POWER	0x0001
#define HUB_STATUS_OVERCURRENT	0x0002

#define HUB_CHANGE_LOCAL_POWER	0x0001
#define HUB_CHANGE_OVERCURRENT	0x0002

/* Hub descriptor */
#pragma pack( push, 1 )
typedef struct 
{
	__u8  bLength;
	__u8  bDescriptorType;
	__u8  bNbrPorts;
	__u16 wHubCharacteristics;
	__u8  bPwrOn2PwrGood;
	__u8  bHubContrCurrent;
	__u8  DeviceRemovable;
	__u8  PortPowerCtrlMask;
} usb_hub_descriptor ;
#pragma pack( pop )

/*
struct usb_hub_descriptor_t
{
	__u8           bDescLength;
	__u8           bDescriptorType;
	__u8           bNbrPorts;
	__u16          wHubCharacteristics;
	#define  UHD_PWR                0x0003
	#define  UHD_PWR_GANGED         0x0000
	#define  UHD_PWR_INDIVIDUAL     0x0001
	#define  UHD_PWR_NO_SWITCH      0x0002
	#define  UHD_COMPOUND           0x0004
	#define  UHD_OC                 0x0018
	#define  UHD_OC_GLOBAL          0x0000
	#define  UHD_OC_INDIVIDUAL      0x0008
	#define  UHD_OC_NONE            0x0010
	#define  UHD_TT_THINK           0x0060
	#define  UHD_TT_THINK_8         0x0000
	#define  UHD_TT_THINK_16        0x0020
	#define  UHD_TT_THINK_24        0x0040
	#define  UHD_TT_THINK_32        0x0060
	#define  UHD_PORT_IND           0x0080
	
	__u8           bPwrOn2PwrGood; // delay in 2 ms units
	#define  UHD_PWRON_FACTOR 2
	
	__u8           bHubContrCurrent;
	__u8           DeviceRemovable[32]; //max 255 ports
	
	#define  UHD_NOT_REMOV(desc, i) \
	(((desc)->DeviceRemovable[(i)/8] >> ((i) % 8)) & 1)
	__u8          PortPowerCtrlMask[1]; // deprecated
	};
*/
#define USB_HUB_DESCRIPTOR_SIZE 9 /* includes deprecated PortPowerCtrlMask */


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

int get_control_msg(ohci_t  * ohci, 
					  __u8  Addr,
				  	  __u8  request,     // 0x06
					  __u8  requesttype, // 0x80
					  __u16 value,
					  __u16 index,
					  __u16 size,
					  __u8  * data );




int set_control_msg(ohci_t  * ohci, 
					  __u8  Addr,
					  __u8  request,     
					  __u8  requesttype, 
					  __u16 value,
					  __u16 index,
					  __u16 size,
					  __u8 * data );



//int usb_get_device_descriptor(ohci_t * ohci, __u8 Addr, int size, void *buf);


//int usb_get_hub_descriptor(ohci_t * ohci, __u8 Addr, int size, void *buf);




int usb_set_configuration(ohci_t * ohci, __u8 Addr, int configuration);


void DebugHubDescriptor( usb_hub_descriptor * pDes );



/***********************************************************************************/


#define UT_WRITE                0x00
#define UT_READ                 0x80
#define UT_STANDARD             0x00
#define UT_CLASS                0x20
#define UT_VENDOR               0x40
#define UT_DEVICE               0x00
#define UT_INTERFACE            0x01
#define UT_ENDPOINT             0x02
#define UT_OTHER                0x03

#define UT_READ_DEVICE          (UT_READ  | UT_STANDARD | UT_DEVICE)
#define UT_READ_INTERFACE       (UT_READ  | UT_STANDARD | UT_INTERFACE)
#define UT_READ_ENDPOINT        (UT_READ  | UT_STANDARD | UT_ENDPOINT)
#define UT_WRITE_DEVICE         (UT_WRITE | UT_STANDARD | UT_DEVICE)
#define UT_WRITE_INTERFACE      (UT_WRITE | UT_STANDARD | UT_INTERFACE)
#define UT_WRITE_ENDPOINT       (UT_WRITE | UT_STANDARD | UT_ENDPOINT)
#define UT_READ_CLASS_DEVICE    (UT_READ  | UT_CLASS | UT_DEVICE)
#define UT_READ_CLASS_INTERFACE (UT_READ  | UT_CLASS | UT_INTERFACE)
#define UT_READ_CLASS_OTHER     (UT_READ  | UT_CLASS | UT_OTHER)
#define UT_READ_CLASS_ENDPOINT  (UT_READ  | UT_CLASS | UT_ENDPOINT)
#define UT_WRITE_CLASS_DEVICE   (UT_WRITE | UT_CLASS | UT_DEVICE)
#define UT_WRITE_CLASS_INTERFACE (UT_WRITE | UT_CLASS | UT_INTERFACE)
#define UT_WRITE_CLASS_OTHER    (UT_WRITE | UT_CLASS | UT_OTHER)
#define UT_WRITE_CLASS_ENDPOINT (UT_WRITE | UT_CLASS | UT_ENDPOINT)
#define UT_READ_VENDOR_DEVICE   (UT_READ  | UT_VENDOR | UT_DEVICE)
#define UT_READ_VENDOR_INTERFACE (UT_READ  | UT_VENDOR | UT_INTERFACE)
#define UT_READ_VENDOR_OTHER    (UT_READ  | UT_VENDOR | UT_OTHER)
#define UT_READ_VENDOR_ENDPOINT (UT_READ  | UT_VENDOR | UT_ENDPOINT)
#define UT_WRITE_VENDOR_DEVICE  (UT_WRITE | UT_VENDOR | UT_DEVICE)
#define UT_WRITE_VENDOR_INTERFACE (UT_WRITE | UT_VENDOR | UT_INTERFACE)
#define UT_WRITE_VENDOR_OTHER   (UT_WRITE | UT_VENDOR | UT_OTHER)
#define UT_WRITE_VENDOR_ENDPOINT (UT_WRITE | UT_VENDOR | UT_ENDPOINT)

/* Requests */
#define UR_GET_STATUS           0x00
#define UR_CLEAR_FEATURE        0x01
#define UR_SET_FEATURE          0x03
#define UR_SET_ADDRESS          0x05
#define UR_GET_DESCRIPTOR       0x06
#define  UDESC_DEVICE           0x01
#define  UDESC_CONFIG           0x02
#define  UDESC_STRING           0x03
#define  UDESC_INTERFACE        0x04
#define  UDESC_ENDPOINT         0x05
#define  UDESC_DEVICE_QUALIFIER 0x06
#define  UDESC_OTHER_SPEED_CONFIGURATION 0x07
#define  UDESC_INTERFACE_POWER  0x08
#define  UDESC_OTG              0x09
#define  UDESC_CS_DEVICE        0x21    /* class specific */
#define  UDESC_CS_CONFIG        0x22
#define  UDESC_CS_STRING        0x23
#define  UDESC_CS_INTERFACE     0x24
#define  UDESC_CS_ENDPOINT      0x25
#define  UDESC_HUB              0x29
#define UR_SET_DESCRIPTOR       0x07
#define UR_GET_CONFIG           0x08
#define UR_SET_CONFIG           0x09
#define UR_GET_INTERFACE        0x0a
#define UR_SET_INTERFACE        0x0b
#define UR_SYNCH_FRAME          0x0c



/***********************************************************************************/

typedef struct 
{
	unsigned char  requesttype; // 1 byte
	unsigned char  request;     // 1 byte
	unsigned short value;       // 2 bytes
	unsigned short index;       // 2 bytes
	unsigned short length;      // 2 bytes
} devrequest;// end devrequest             //   Total = 8 bytes


typedef struct 
{
         ohci_t     * p_ohci;      /* our ohci stuff */
         __u8       address;       /* device addess */
}usbd_device;// end usbd_device



int usbd_get_desc(usbd_device * dev, int type, int index, int len, void *desc);

int usbd_do_request(usbd_device * dev, devrequest *req, void *data);


int usbd_get_hub_descriptor(usbd_device * dev, void * data);


int do_hub_work(usbd_device * dev);


int usbd_do_request_big_packet(usbd_device * dev, devrequest *req, void *data);




#endif // __HUB__


