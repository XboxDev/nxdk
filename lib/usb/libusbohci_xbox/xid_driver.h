
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

//XINPUT defines and struct format from
//https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
#define XINPUT_GAMEPAD_DPAD_UP 0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN 0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT 0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT 0x0008
#define XINPUT_GAMEPAD_START 0x0010
#define XINPUT_GAMEPAD_BACK 0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB 0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB 0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER 0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER 0x0200
#define XINPUT_GAMEPAD_A 0x1000
#define XINPUT_GAMEPAD_B 0x2000
#define XINPUT_GAMEPAD_X 0x4000
#define XINPUT_GAMEPAD_Y 0x8000

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
    uint8_t leftTrigger;
    uint8_t rightTrigger;
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

typedef struct __attribute__((packed))
{
    uint8_t  startByte;
    uint8_t  bLength;
    uint16_t buttons[3];
    uint16_t aimingLeverX; // 0 = Left, 0xFFFF = Right
    uint16_t aimingLeverY; // 0 = Top,  0xFFFF = Bottom
    int16_t  turningLever;
    int16_t  sightChangeX;
    int16_t  sightChangeY;
    uint16_t slidePedal;
    uint16_t brakePedal;
    uint16_t accelPedal;
    uint8_t  tuner;   // 0-15 is from 9oclock, around clockwise
    int8_t   shifter; // -2 = R, -1 = N, 0 = Error, 1 = 1st, 2 = 2nd, 3 = 3rnd, 4 = 4th, 5 = 5th
} xid_steelbattalion_in;

#define STEELBATTALION_LIGHT_COUNT 40
#define STEELBATTALION_LIGHT_BYTES (STEELBATTALION_LIGHT_COUNT / 2) // Each nibble is 1 light
typedef struct __attribute__((packed))
{
   uint8_t startByte;
   uint8_t bLength;
   uint8_t lights[STEELBATTALION_LIGHT_BYTES];
} xid_steelbattalion_out;

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
int32_t usbh_xid_sbc_lights(xid_dev_t *xid_dev, uint8_t lights[STEELBATTALION_LIGHT_BYTES]);

#ifdef __cplusplus
}
#endif

#endif
