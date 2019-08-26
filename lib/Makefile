SRCS += \
	$(shell find $(NXDK_DIR)/lib/pdclib/functions/ -name "*.c") \
	$(shell find $(NXDK_DIR)/lib/pdclib/platform/xbox/ -name "*.c") \
	$(shell find $(NXDK_DIR)/lib/pdclib/platform/xbox/ -name "*.s") \
	$(wildcard $(NXDK_DIR)/lib/winapi/*.c) \
	$(wildcard $(NXDK_DIR)/lib/pbkit/*.c)

include $(NXDK_DIR)/lib/hal/Makefile
include $(NXDK_DIR)/lib/usb/Makefile
include $(NXDK_DIR)/lib/xboxrt/Makefile
