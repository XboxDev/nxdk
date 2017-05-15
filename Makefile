ifeq ($(NXDK_DIR),)
NXDK_DIR = $(shell pwd)
endif

ifeq ($(XBE_TITLE),)
XBE_TITLE = nxdk_app
endif

ifeq ($(OUTPUT_DIR),)
OUTPUT_DIR = bin
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LD           = lld-link
CC           = clang
CXX          = clang++
CGC          = $(NXDK_DIR)/tools/cg/linux/cgc
endif
ifeq ($(UNAME_S),Darwin)
LD           = /usr/local/opt/llvm/bin/lld -flavor link
CC           = /usr/local/opt/llvm/bin/clang
CXX          = /usr/local/opt/llvm/bin/clang++
CGC          = $(NXDK_DIR)/tools/cg/mac/cgc
endif

TARGET       = $(OUTPUT_DIR)/default.xbe
CXBE         = $(NXDK_DIR)/tools/cxbe/cxbe
VP20COMPILER = $(NXDK_DIR)/tools/vp20compiler/vp20compiler
FP20COMPILER = $(NXDK_DIR)/tools/fp20compiler/fp20compiler
EXTRACT_XISO = $(NXDK_DIR)/tools/extract-xiso/extract-xiso
TOOLS        = cxbe vp20compiler fp20compiler extract-xiso
CFLAGS       = -target i386-pc-win32 -march=pentium3 \
               -ffreestanding -nostdlib -fno-builtin -fno-exceptions \
               -I$(NXDK_DIR)/lib -I$(NXDK_DIR)/lib/xboxrt \
               -Wno-ignored-attributes

ifeq ($(DEBUG),y)
CFLAGS += -g
endif

include $(NXDK_DIR)/lib/Makefile
OBJS = $(SRCS:.c=.obj)

ifneq ($(GEN_XISO),)
TARGET += $(GEN_XISO)
endif

ifneq ($(NXDK_NET),)
include $(NXDK_DIR)/lib/net/Makefile
endif

ifneq ($(NXDK_SDL),)
include $(NXDK_DIR)/lib/sdl/Makefile
endif

V = 0
VE_0 := @
VE_1 :=
VE = $(VE_$(V))

ifeq ($(V),1)
QUIET=
else
QUIET=>/dev/null
endif

DEPS := $(SRCS:.c=.c.d)

all: $(TARGET)

$(OUTPUT_DIR)/default.xbe: main.exe $(OUTPUT_DIR) $(CXBE)
	@echo "[ CXBE     ] $@"
	$(VE)$(CXBE) -OUT:$@ -TITLE:$(XBE_TITLE) $< $(QUIET)

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR);

ifneq ($(GEN_XISO),)
$(GEN_XISO): $(OUTPUT_DIR)/default.xbe $(EXTRACT_XISO)
	@echo "[ XISO     ] $@"
	$(VE) $(EXTRACT_XISO) -c $(OUTPUT_DIR) $(XISO_FLAGS) $@ $(QUIET)
endif

main.exe: $(OBJS) $(NXDK_DIR)/lib/xboxkrnl/libxboxkrnl.lib
	@echo "[ LD       ] $@"
	$(VE) $(LD) -subsystem:windows -dll -out:'$@' -entry:XboxCRT $^

%.obj: %.c
	@echo "[ CC       ] $@"
	$(VE) $(CC) $(CFLAGS) -c -o '$@' '$<'

%.c.d: %.c
	@echo "[ DEP      ] $@"
	$(VE) set -e; rm -f $@; \
	$(CC) -M -MM -MG -MT '$*.obj' -MF $@ $(CFLAGS) $<; \
	echo "\n$@ : $^\n" >> $@

%.inl: %.vs.cg $(VP20COMPILER)
	@echo "[ CG       ] $@"
	$(VE) $(CGC) -profile vp20 -o $@.$$$$ $< $(QUIET) && \
	$(VP20COMPILER) $@.$$$$ > $@ && \
	rm -rf $@.$$$$

%.inl: %.ps.cg $(FP20COMPILER)
	@echo "[ CG       ] $@"
	$(VE) $(CGC) -profile fp20 -o $@.$$$$ $< $(QUIET) && \
	$(FP20COMPILER) $@.$$$$ > $@ && \
	rm -rf $@.$$$$

tools: $(TOOLS)
.PHONY: tools $(TOOLS)

cxbe: $(CXBE)
$(CXBE):
	@echo "[ BUILD    ] $@"
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/cxbe $(QUIET)

vp20compiler: $(VP20COMPILER)
$(VP20COMPILER):
	@echo "[ BUILD    ] $@"
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/vp20compiler $(QUIET)

fp20compiler: $(FP20COMPILER)
$(FP20COMPILER):
	@echo "[ BUILD    ] $@"
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/fp20compiler $(QUIET)

extract-xiso: $(EXTRACT_XISO)
$(EXTRACT_XISO):
	@echo "[ BUILD    ] $@"
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/extract-xiso $(QUIET)

.PHONY: clean 
clean:
	$(VE)rm -f $(TARGET) \
	           main.exe main.exe.manifest \
	           $(OBJS) $(SHADER_OBJS) \
	           $(GEN_XISO)

.PHONY: distclean 
distclean: clean
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/extract-xiso clean $(QUIET)
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/fp20compiler distclean $(QUIET)
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/vp20compiler distclean $(QUIET)
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/cxbe clean $(QUIET)
	$(VE)bash -c "if [ -d $(OUTPUT_DIR) ]; then rmdir $(OUTPUT_DIR); fi"
	$(VE)rm -f $(DEPS)

-include $(DEPS)
