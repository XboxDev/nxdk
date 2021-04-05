XBE_TITLE = nxdk\ sample\ -\ SDL2_image
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.c
NXDK_DIR ?= $(CURDIR)/../..
NXDK_SDL = y

include $(NXDK_DIR)/Makefile

TARGET += $(OUTPUT_DIR)/testimg.jpg
$(GEN_XISO): $(OUTPUT_DIR)/testimg.jpg
$(OUTPUT_DIR)/testimg.jpg: $(CURDIR)/testimg.jpg $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'
