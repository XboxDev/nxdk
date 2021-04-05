XBE_TITLE = nxdk\ sample\ -\ SDL2_ttf
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.c
NXDK_DIR ?= $(CURDIR)/../..
NXDK_SDL = y

all_local: cp_font all

include $(NXDK_DIR)/Makefile

cp_font:
	@mkdir -p $(OUTPUT_DIR)
	cp vegur-regular.ttf $(OUTPUT_DIR)/
