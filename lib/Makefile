USB_SRCS := \
	lib/usb/host/ohci-hcd.c \
	lib/usb/core/message.c \
	lib/usb/core/hcd.c \
	lib/usb/core/hcd-pci.c \
	lib/usb/core/hub.c \
	lib/usb/core/usb.c \
	lib/usb/core/config.c \
	lib/usb/core/urb.c \
	lib/usb/core/buffer_simple.c \
	lib/usb/core/usb-debug.c \
	lib/usb/sys/BootUSB.c \
	lib/usb/sys/linuxwrapper.c \
	lib/usb/sys/xpad.c \
	lib/usb/sys/xremote.c \
	lib/usb/sys/usbkey.c \
	lib/usb/sys/usbmouse.c \
	lib/usb/misc/misc.c \
	lib/usb/misc/pci.c \
	lib/usb/misc/malloc.c

INCLUDES += \
	$(wildcard lib/xboxkrnl/*.h) \
	$(wildcard lib/xboxrt/*.h) \
	$(wildcard lib/hal/*.h) \
	$(wildcard lib/pbkit/*.h)

SRCS += \
	$(wildcard lib/xboxrt/*.c) \
	$(wildcard lib/hal/*.c) \
	$(wildcard lib/pbkit/*.c) \
	$(USB_SRCS)
