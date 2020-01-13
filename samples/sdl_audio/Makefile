XBE_TITLE = sdl_audio
GEN_XISO = $(XBE_TITLE).iso
SRCS += $(CURDIR)/main.c
NXDK_DIR = $(CURDIR)/../..
NXDK_SDL = y

all_local: cp_sound all

include $(NXDK_DIR)/Makefile

cp_sound:
	@mkdir -p $(OUTPUT_DIR)
	cp nxdk.wav $(OUTPUT_DIR)/
