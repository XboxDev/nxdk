ifeq ($(NXDK_DIR),)
NXDK_DIR = $(shell pwd)
endif

ifeq ($(XBE_TITLE),)
XBE_TITLE = nxdk_app
endif

ifeq ($(OUTPUT_DIR),)
OUTPUT_DIR = bin
endif

TARGET       = $(OUTPUT_DIR)/default.xbe
LLDLINK      = /usr/local/opt/llvm/bin/lld -flavor link
CC           = /usr/local/opt/llvm/bin/clang
CXX          = /usr/local/opt/llvm/bin/clang++
CGC          = wine $(NXDK_DIR)/tools/cg/cgc.exe
CXBE         = $(NXDK_DIR)/tools/cxbe/cxbe
VP20COMPILER = $(NXDK_DIR)/tools/vp20compiler/vp20compiler
FP20COMPILER = $(NXDK_DIR)/tools/fp20compiler/fp20compiler
EXTRACT_XISO = $(NXDK_DIR)/tools/extract-xiso/extract-xiso
TOOLS        = cxbe vp20compiler fp20compiler extract-xiso
CFLAGS       = -target i386-pc-win32 -march=pentium3 \
               -ffreestanding -nostdlib -fno-builtin -fno-exceptions \
               -I$(NXDK_DIR)/lib -I$(NXDK_DIR)/lib/xboxrt

include $(NXDK_DIR)/lib/Makefile
OBJS = $(SRCS:.c=.obj)

ifneq ($(GEN_XISO),)
TARGET += $(GEN_XISO)
endif

V = 0
VE_0 := @
VE_1 :=
VE = $(VE_$(V))

ifeq ($(V),1)
STDOUT_TO_NULL=
else
STDOUT_TO_NULL=>/dev/null
endif

all: $(TARGET)

$(OUTPUT_DIR)/default.xbe: $(OUTPUT_DIR) main.exe
	@echo "[ CXBE     ] $@"
	$(VE)$(CXBE) -OUT:$@ -TITLE:$(XBE_TITLE) $^ $(STDOUT_TO_NULL)

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR);

ifneq ($(GEN_XISO),)
$(GEN_XISO): $(OUTPUT_DIR)/default.xbe
	@echo "[ XISO     ] $@"
	$(VE) $(EXTRACT_XISO) -c $(OUTPUT_DIR) $(XISO_FLAGS) $@
endif

main.exe: $(OBJS)
	@echo "[ LLD      ] $@"
	$(VE) $(LLDLINK) -subsystem:windows -dll -out:'$@' \
	-entry:XboxCRT $(NXDK_DIR)/lib/xboxkrnl/libxboxkrnl.lib $(OBJS)

%.obj: %.c $(INCLUDES)
	@echo "[ CC       ] $@"
	$(VE) $(CC) $(CFLAGS) -c -o '$@' '$<'

%.inl: %.vs.cg
	@echo "[ CG       ] $@"
	$(VE) $(CGC) -profile vp20 -o $@.$$$$ $< ; \
	$(VP20COMPILER) $@.$$$$ > $@ ; \
	rm -rf $@.$$$$

%.inl: %.ps.cg
	@echo "[ CG       ] $@"
	$(VE) $(CGC) -profile fp20 -o $@.$$$$ $< ; \
	$(FP20COMPILER) $@.$$$$ > $@ ; \
	rm -rf $@.$$$$

tools: $(TOOLS)
.PHONY: tools $(TOOLS)

cxbe:
	$(MAKE) -C $(NXDK_DIR)/tools/cxbe

vp20compiler:
	$(MAKE) -C $(NXDK_DIR)/tools/vp20compiler

fp20compiler:
	$(MAKE) -C $(NXDK_DIR)/tools/fp20compiler

extract-xiso:
	$(MAKE) -C $(NXDK_DIR)/tools/extract-xiso

.PHONY: clean 
clean:
	$(VE)rm -f $(TARGET) \
	           main.exe main.exe.manifest \
	           $(OBJS) $(SHADEROBJ) \
	           $(GEN_XISO)

.PHONY: distclean 
distclean: clean
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/extract-xiso clean
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/fp20compiler distclean
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/vp20compiler distclean
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/cxbe clean
	$(VE)bash -c "if [ -d $(OUTPUT_DIR) ]; then rmdir $(OUTPUT_DIR); fi"
