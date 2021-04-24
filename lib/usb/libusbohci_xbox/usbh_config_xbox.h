/**************************************************************************//**
 * @file     usbh_config_xbox.h
 * @version  V1.00
 * @brief    This header file defines the configuration of USB Host library.
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 * Copyright (C) 2021 Ryan Wendland (Xbox port)
 *****************************************************************************/

#ifndef  _USBH_CONFIG_H_
#define  _USBH_CONFIG_H_

/*----------------------------------------------------------------------------------------*/
/*   Hardware settings                                                                    */
/*----------------------------------------------------------------------------------------*/
#define NON_CACHE_MASK 0
#define USB_IRQ 1
#define ENABLE_OHCI_IRQ()      _ohci->HcInterruptEnable |= USBH_HcInterruptEnable_MIE_Msk
#define DISABLE_OHCI_IRQ()     _ohci->HcInterruptDisable = USBH_HcInterruptDisable_MIE_Msk
#define IS_OHCI_IRQ_ENABLED()  ((_ohci->HcInterruptEnable & USBH_HcInterruptEnable_MIE_Msk) !=0)
#define ENABLE_EHCI_IRQ()
#define DISABLE_EHCI_IRQ()
#define IS_EHCI_IRQ_ENABLED()  0

#define ENABLE_OHCI                         /* Enable OHCI host controller                */
#define OHCI_PORT_CNT          4            /* Number of OHCI roothub ports               */
//#define OHCI_PER_PORT_POWER               /* OHCI root hub per port powered             */

#define OHCI_ISO_DELAY         4            /* preserved number frames while scheduling 
                                               OHCI isochronous transfer                  */

#define MAX_DESC_BUFF_SIZE     4096         /* To hold the configuration descriptor, USB 
                                               core will allocate a buffer with this size
                                               for each connected device. USB core does 
                                               not release it until device disconnected.  */

/*----------------------------------------------------------------------------------------*/
/*   Memory allocation settings                                                           */
/*----------------------------------------------------------------------------------------*/
#ifndef MAX_UDEV_DRIVER
#define MAX_UDEV_DRIVER        8       /*!< Maximum number of registered drivers                      */
#endif
#ifndef MAX_ALT_PER_IFACE
#define MAX_ALT_PER_IFACE      12      /*!< maximum number of alternative interfaces per interface    */
#endif
#ifndef MAX_EP_PER_IFACE
#define MAX_EP_PER_IFACE       8       /*!< maximum number of endpoints per interface                 */
#endif
#ifndef MAX_HUB_DEVICE
#define MAX_HUB_DEVICE         8       /*!< Maximum number of hub devices                             */
#endif

/* Host controller hardware transfer descriptors memory pool. ED/TD/ITD of OHCI
   are all allocated from this pool. Allocated unit size is determined by MEM_POOL_UNIT_SIZE.
   May allocate one or more units depend on hardware descriptor type.                                 */
#define MEM_POOL_UNIT_SIZE     128     /*!< A fixed hard coding setting. Do not change it!            */
#ifndef MEM_POOL_UNIT_NUM
//Four S controllers with their internal hub use about 50 'units'. Roughly double it for safe measure
#define MEM_POOL_UNIT_NUM      96      /*!< Increase this or heap size if memory allocate failed.     */
#endif

/* USB library memory pool for device descriptors, pipe tranfer buffers etc.                          */
#ifndef USB_MEMORY_POOL_SIZE
#define USB_MEMORY_POOL_SIZE   (512*1024)
#endif

/*----------------------------------------------------------------------------------------*/
/*   Debug settings                                                                       */
/*----------------------------------------------------------------------------------------*/
//#define ENABLE_ERROR_MSG                  /* enable debug messages                      */
//#define ENABLE_DEBUG_MSG                  /* enable debug messages                      */
//#define ENABLE_VERBOSE_DEBUG              /* verbos debug messages                      */
//#define DUMP_DESCRIPTOR                   /* dump descriptors                           */

#ifdef ENABLE_ERROR_MSG
#define USB_error            sysprintf
#else
#define USB_error(...)
#endif

#ifdef ENABLE_DEBUG_MSG
#define USB_debug            sysprintf
#ifdef ENABLE_VERBOSE_DEBUG
#define USB_vdebug           sysprintf
#else
#define USB_vdebug(...)
#endif
#else
#define USB_debug(...)
#define USB_vdebug(...)
#endif

#define USBH                 ((USBH_T *)0xFED00000)
#define HSUSBH               ((HSUSBH_T *)NULL)
#endif  /* _USBH_CONFIG_H_ */
