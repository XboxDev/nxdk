PROJECT_NAME = rustnxdk
XISO_NAME = rusttest
XBE_TITLE = rusttest
NXDK_DIR = /home/$(USER)/nxdk
OUTPUT_DIR = bin
RUSTFLAGS := --target xbox.json -Zbuild-std=core

CXBE = $(NXDK_DIR)/tools/cxbe/cxbe
XISO = $(NXDK_DIR)/tools/extract-xiso/build/extract-xiso

EXEC_DIR = target/xbox/release

ifeq ($(DEBUG),y)
    EXEC_DIR = target/xbox/debug
else
    RUSTFLAGS += --release
endif

all:
	@mkdir -p $(OUTPUT_DIR)
	@cargo +nightly build $(RUSTFLAGS)
	@cp $(EXEC_DIR)/$(PROJECT_NAME).exe ./main.exe
	$(CXBE) -OUT:default.xbe -TITLE:$(XBE_TITLE) ./main.exe
	@mv default.xbe $(OUTPUT_DIR)
	$(XISO) -c $(OUTPUT_DIR) $(XISO_NAME).iso