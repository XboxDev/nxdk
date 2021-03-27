HAL_SRCS := \
	$(NXDK_DIR)/lib/hal/audio.c \
	$(NXDK_DIR)/lib/hal/debug.c \
	$(NXDK_DIR)/lib/hal/fileio.c \
	$(NXDK_DIR)/lib/hal/io.c \
	$(NXDK_DIR)/lib/hal/led.c \
	$(NXDK_DIR)/lib/hal/video.c \
	$(NXDK_DIR)/lib/hal/xbox.c

HAL_OBJS = $(addsuffix .obj, $(basename $(HAL_SRCS)))

$(NXDK_DIR)/lib/libnxdk_hal.lib: $(HAL_OBJS)

main.exe: $(NXDK_DIR)/lib/libnxdk_hal.lib

CLEANRULES += clean-hal

.PHONY: clean-hal
clean-hal:
	$(VE)rm -f $(HAL_OBJS) $(NXDK_DIR)/lib/libnxdk_hal.lib
