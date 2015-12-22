/******************************************************************************/
/*																              */
/*  File: pad.h                                                               */
/*  bkenwright@xbdev.net  -  www.xbdev.net                                    */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*
    What on earth!  Whats this file for?
	Well once we have our usb device address set...for the xbox gamepad :)
	This file will do some simple commands...sending bulk test messages
	...an example would be to send a rumble message?
	Also probe for some button presses?

    In Development though.
*/
/******************************************************************************/


#ifndef		__PAD__
#define		__PAD__


#include <usb/ohci.h>
#include <usb/hub.h>




/******************************************************************************/
/*                                                                            */
/* Functions                                                                  */
/*                                                                            */
/******************************************************************************/


void usb_bulk_msg( usbd_device * dev, int size, void * data );    // Send data to the USB

void usb_bulk_msg_in( usbd_device * dev, int size, void * data ); // Get data from the USB




#endif // __PAD__
