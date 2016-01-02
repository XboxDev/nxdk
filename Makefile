LLDLINK      = /usr/local/opt/llvm/bin/lld -flavor link
CC           = /usr/local/opt/llvm/bin/clang
CXX          = /usr/local/opt/llvm/bin/clang++
CGC          = wine $(NXDK_DIR)/tools/cg/cgc.exe
CXBE         = $(NXDK_DIR)/tools/cxbe/cxbe
VP20COMPILER = $(NXDK_DIR)/tools/vp20compiler/vp20compiler
FP20COMPILER = $(NXDK_DIR)/tools/fp20compiler/fp20compiler
EXTRACT_XISO = $(NXDK_DIR)/tools/extract-xiso/extract-xiso
TOOLS        = cxbe vp20compiler fp20compiler extract-xiso

CFLAGS = -target i386-pc-win32 -march=pentium3 \
         -ffreestanding -nostdlib -fno-builtin -fno-exceptions \
         -I$(NXDK_DIR)/lib -I$(NXDK_DIR)/lib/xboxrt

ifeq ($(NXDK_DIR),)
	NXDK_DIR = $(shell pwd)
endif

include $(NXDK_DIR)/lib/Makefile

OBJS = $(SRCS:.c=.obj)

all: default.iso

default.iso: bin/default.xbe
	$(EXTRACT_XISO) -c bin $@

bin/default.xbe: main.exe
	mkdir -p bin
	$(CXBE) -OUT:bin/default.xbe -TITLE:0ldskoo1 main.exe

main.exe: $(OBJS)
	$(LLDLINK) -subsystem:windows -dll -out:'$@' \
	-entry:XboxCRT $(NXDK_DIR)/lib/xboxkrnl/libxboxkrnl.lib $(OBJS)

%.obj: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c -o '$@' '$<'

%.inl: %.vs.cg
	$(CGC) -profile vp20 -o $@.$$$$ $< ; \
	$(VP20COMPILER) $@.$$$$ > $@ ; \
	rm -rf $@.$$$$

%.inl: %.ps.cg
	$(CGC) -profile fp20 -o $@.$$$$ $< ; \
	$(FP20COMPILER) $@.$$$$ > $@ ; \
	rm -rf $@.$$$$

#
# Tools
#

tools: $(TOOLS)

cxbe:
	$(MAKE) -C $(NXDK_DIR)/tools/cxbe

vp20compiler:
	$(MAKE) -C $(NXDK_DIR)/tools/vp20compiler

fp20compiler:
	$(MAKE) -C $(NXDK_DIR)/tools/fp20compiler

extract-xiso:
	$(MAKE) -C $(NXDK_DIR)/tools/extract-xiso

.PHONY: clean tools cxbe vp20compiler fp20compiler extract-xiso
clean:
	rm -f default.iso bin/default.xbe main.exe main.exe.manifest $(OBJS) $(SHADEROBJ)
