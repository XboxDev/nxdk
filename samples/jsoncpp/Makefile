XBE_TITLE = nxdk\ sample\ -\ jsoncpp
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.cpp
NXDK_DIR ?= $(CURDIR)/../..
NXDK_CXX = y
NXDK_JSONCPP = y

include $(NXDK_DIR)/Makefile

TARGET += $(OUTPUT_DIR)/message.json
$(GEN_XISO): $(OUTPUT_DIR)/message.json
$(OUTPUT_DIR)/message.json: $(CURDIR)/message.json $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'