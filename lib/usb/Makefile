USBH_DIR=$(NXDK_DIR)/lib/usb

# USB Host core
USB_CORE_SRCS := \
	$(USBH_DIR)/libusbohci/src_core/hub.c \
	$(USBH_DIR)/libusbohci/src_core/mem_alloc.c \
	$(USBH_DIR)/libusbohci/src_core/ohci.c \
	$(USBH_DIR)/libusbohci/src_core/support.c \
	$(USBH_DIR)/libusbohci/src_core/usb_core.c

USBH_OBJS = $(addsuffix .obj, $(basename $(USB_CORE_SRCS)))

# Xbox specific files
USB_XBOX_SRCS := \
	$(USBH_DIR)/libusbohci_xbox/usbh_xbox.c \
	$(USBH_DIR)/libusbohci_xbox/xid_driver.c

USBH_OBJS += $(addsuffix .obj, $(basename $(USB_XBOX_SRCS)))

# USB Human Input Device class driver
ifneq ($(NXDK_USB_ENABLE_HID),)
USB_HID_SRCS := \
	$(USBH_DIR)/libusbohci/src_hid/hid_core.c \
	$(USBH_DIR)/libusbohci/src_hid/hid_driver.c

USBH_OBJS += $(addsuffix .obj, $(basename $(USB_HID_SRCS)))
endif

# USB Communication Device class driver
ifneq ($(NXDK_USB_ENABLE_CDC),)
USB_CDC_SRCS := \
	$(USBH_DIR)/libusbohci/src_cdc/cdc_core.c \
	$(USBH_DIR)/libusbohci/src_cdc/cdc_driver.c \
	$(USBH_DIR)/libusbohci/src_cdc/cdc_parser.c

USBH_OBJS += $(addsuffix .obj, $(basename $(USB_CDC_SRCS)))
endif

# USB Audio class driver
ifneq ($(NXDK_USB_ENABLE_UAC),)
USB_UAC_SRCS := \
	$(USBH_DIR)/libusbohci/src_uac/uac_core.c \
	$(USBH_DIR)/libusbohci/src_uac/uac_driver.c \
	$(USBH_DIR)/libusbohci/src_uac/uac_parser.c

USBH_OBJS += $(addsuffix .obj, $(basename $(USB_UAC_SRCS)))
endif

# USB Video class driver
ifneq ($(NXDK_USB_ENABLE_UVC),)
USB_UVC_SRCS := \
	$(USBH_DIR)/libusbohci/src_uvc/uvc_core.c \
	$(USBH_DIR)/libusbohci/src_uvc/uvc_driver.c \
	$(USBH_DIR)/libusbohci/src_uvc/uvc_parser.c

USBH_OBJS += $(addsuffix .obj, $(basename $(USB_UVC_SRCS)))
endif

# Mass Storage Class driver
ifneq ($(NXDK_USB_ENABLE_MSC),)
USB_MSC_SRCS := \
	$(USBH_DIR)/libusbohci/src_msc/msc_driver.c \
	$(USBH_DIR)/libusbohci/src_msc/msc_xfer.c
USBH_OBJS += $(addsuffix .obj, $(basename $(USB_MSC_SRCS)))
endif

$(NXDK_DIR)/lib/nxdk_usb.lib: $(USBH_OBJS)

NXDK_CFLAGS   += -I$(USBH_DIR)/libusbohci/inc -I$(USBH_DIR)/libusbohci_xbox/ -DUSBH_USE_EXTERNAL_CONFIG=\"usbh_config_xbox.h\"
NXDK_CXXFLAGS += -I$(USBH_DIR)/libusbohci/inc -I$(USBH_DIR)/libusbohci_xbox/ -DUSBH_USE_EXTERNAL_CONFIG=\"usbh_config_xbox.h\"

main.exe: $(NXDK_DIR)/lib/nxdk_usb.lib

CLEANRULES += clean-usb

.PHONY: clean-usb
clean-usb:
	$(VE)rm -f $(USBH_OBJS) $(NXDK_DIR)/lib/nxdk_usb.lib
