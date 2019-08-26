USB_SRCS := \
	$(NXDK_DIR)/lib/usb/host/ohci-hcd.c \
	$(NXDK_DIR)/lib/usb/core/message.c \
	$(NXDK_DIR)/lib/usb/core/hcd.c \
	$(NXDK_DIR)/lib/usb/core/hcd-pci.c \
	$(NXDK_DIR)/lib/usb/core/hub.c \
	$(NXDK_DIR)/lib/usb/core/usb.c \
	$(NXDK_DIR)/lib/usb/core/config.c \
	$(NXDK_DIR)/lib/usb/core/urb.c \
	$(NXDK_DIR)/lib/usb/core/buffer_simple.c \
	$(NXDK_DIR)/lib/usb/core/usb-debug.c \
	$(NXDK_DIR)/lib/usb/sys/BootUSB.c \
	$(NXDK_DIR)/lib/usb/sys/linuxwrapper.c \
	$(NXDK_DIR)/lib/usb/sys/xpad.c \
	$(NXDK_DIR)/lib/usb/sys/xremote.c \
	$(NXDK_DIR)/lib/usb/sys/usbkey.c \
	$(NXDK_DIR)/lib/usb/sys/usbmouse.c \
	$(NXDK_DIR)/lib/usb/misc/misc.c \
	$(NXDK_DIR)/lib/usb/misc/pci.c \
	$(NXDK_DIR)/lib/usb/misc/malloc.c

USB_OBJS = $(addsuffix .obj, $(basename $(USB_SRCS)))

$(NXDK_DIR)/lib/libnxdk_usb.lib: $(USB_OBJS)

main.exe: $(NXDK_DIR)/lib/libnxdk_usb.lib

CLEANRULES += clean-usb

.PHONY: clean-usb
clean-usb:
	$(VE)rm -f $(USB_OBJS) $(NXDK_DIR)/lib/libnxdk_usb.lib
