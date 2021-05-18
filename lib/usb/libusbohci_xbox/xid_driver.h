
//Copyright 2020, Ryan Wendland
//SPDX-License-Identifier: MIT
#ifndef _USBH_XID_H_
#define _USBH_XID_H_

#include "usb.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CONFIG_XID_MAX_DEV
#define CONFIG_XID_MAX_DEV 4
#endif
#ifndef XID_MAX_TRANSFER_QUEUE
#define XID_MAX_TRANSFER_QUEUE 4
#endif
//Ref https://xboxdevwiki.net/index.php?title=Xbox_Input_Devices
#define XID_INTERFACE_CLASS 0x58
#define XID_INTERFACE_SUBCLASS 0x42

struct xid_dev;
typedef void(XID_CONN_FUNC)(struct xid_dev *hdev, int param);

typedef struct __attribute__((packed)) 
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdXid;
    uint8_t bType;
    uint8_t bSubType;
    uint8_t bMaxInputReportSize;
    uint8_t bMaxOutputReportSize;
    uint16_t wAlternateProductIds[4];
} xid_descriptor;

typedef struct __attribute__((packed)) 
{
    uint8_t startByte;
    uint8_t bLength;
    uint16_t dButtons;
    uint8_t a;
    uint8_t b;
    uint8_t x;
    uint8_t y;
    uint8_t black;
    uint8_t white;
    uint8_t l;
    uint8_t r;
    int16_t leftStickX;
    int16_t leftStickY;
    int16_t rightStickX;
    int16_t rightStickY;
} xid_gamepad_in;

typedef struct __attribute__((packed))
{
    uint8_t startByte;
    uint8_t bLength;
    uint16_t lValue;
    uint16_t hValue;
} xid_gamepad_out;

//Ref https://xboxdevwiki.net/Xbox_DVD_Movie_Playback_Kit
typedef struct __attribute__((packed))
{
    uint8_t startByte;
    uint8_t bLength;
    uint16_t buttonCode;
    uint16_t timeElapsed; //ms since last button press
} xid_xremote_in;

typedef struct xid_dev
{
    uint16_t idVendor;
    uint16_t idProduct;
    xid_descriptor xid_desc;                 //See https://xboxdevwiki.net/index.php?title=Xbox_Input_Devices
    UTR_T *utr_list[XID_MAX_TRANSFER_QUEUE]; //UTR list of queued transfers
    IFACE_T *iface;                          //This xid interface
    uint32_t uid;                            //A unique ID to identify this device
    struct xid_dev *next;                    //Pointer to the next xid in the linked list.
    void *user_data;                         //Pointer to an optional user struct
} xid_dev_t;

//Ref https://xboxdevwiki.net/index.php?title=Xbox_Input_Devices
#define XID_TYPE_GAMECONTROLLER 0x01
#define XID_TYPE_XREMOTE 0x03
#define XID_TYPE_STEELBATTALION 0x80
typedef enum _xid_type
{
    XID_UNKNOWN                = 0x0000,
    GAMECONTROLLER_S           = XID_TYPE_GAMECONTROLLER << 8 | 0x01,
    GAMECONTROLLER_DUKE        = XID_TYPE_GAMECONTROLLER << 8 | 0x02,
    GAMECONTROLLER_WHEEL       = XID_TYPE_GAMECONTROLLER << 8 | 0x10,
    GAMECONTROLLER_ARCADESTICK = XID_TYPE_GAMECONTROLLER << 8 | 0x20,
    XREMOTE                    = XID_TYPE_XREMOTE << 8 | 0x00,
    STEEL_BATTALION            = XID_TYPE_STEELBATTALION << 8 | 0x01,
} xid_type;

void usbh_xid_init(void);
void usbh_install_xid_conn_callback(XID_CONN_FUNC *conn_func, XID_CONN_FUNC *disconn_func);
xid_dev_t *usbh_xid_get_device_list(void);
int32_t usbh_xid_read(xid_dev_t *xid_dev, uint8_t ep_addr, void *rx_complete_callback);
int32_t usbh_xid_write(xid_dev_t *xid_dev, uint8_t ep_addr, uint8_t *txbuff, uint32_t len, void *tx_complete_callback);
xid_type usbh_xid_get_type(xid_dev_t *xid_dev);
int32_t usbh_xid_rumble(xid_dev_t *xid_dev, uint16_t l_value, uint16_t h_value);

#ifdef __cplusplus
}
#endif

#endif
