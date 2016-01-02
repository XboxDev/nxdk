LLDLINK      = /usr/local/opt/llvm/bin/lld -flavor link
CC           = /usr/local/opt/llvm/bin/clang
CXX          = /usr/local/opt/llvm/bin/clang++
CGC          = wine tools/cg/cgc.exe
CXBE         = tools/cxbe/cxbe
VP20COMPILER = tools/vp20compiler/vp20compiler
FP20COMPILER = tools/fp20compiler/fp20compiler
EXTRACT_XISO = tools/extract-xiso/extract-xiso

CFLAGS = -target i386-pc-win32 -march=pentium3 \
    -ffreestanding -nostdlib -fno-builtin -fno-exceptions \
    -I. -Ixboxrt

SHADERINT = \
	app/vs.vp \
	app/ps.fp

SHADEROBJ = \
	app/vs.inl \
	app/ps.inl

USB_SRCS = \
    usb/host/ohci-hcd.c \
    usb/core/message.c \
    usb/core/hcd.c \
    usb/core/hcd-pci.c \
    usb/core/hub.c \
    usb/core/usb.c \
    usb/core/config.c \
    usb/core/urb.c \
    usb/core/buffer_simple.c \
    usb/core/usb-debug.c \
    usb/sys/BootUSB.c \
    usb/sys/linuxwrapper.c \
    usb/sys/xpad.c \
    usb/sys/xremote.c \
    usb/sys/usbkey.c \
    usb/sys/usbmouse.c \
    usb/misc/misc.c \
    usb/misc/pci.c \
    usb/misc/malloc.c

INCLUDES := $(wildcard xboxkrnl/*.h) \
            $(wildcard xboxrt/*.h) \
            $(wildcard hal/*.h) \
            $(wildcard pbkit/*.h) \
            $(wildcard app/*.h) \
            $(SHADEROBJ)

SRCS := $(wildcard xboxrt/*.c) \
        $(wildcard hal/*.c) \
        $(wildcard pbkit/*.c) \
        $(wildcard app/*.c) \
        $(USB_SRCS)
OBJS = $(SRCS:.c=.obj)

all: default.iso

default.iso: bin/default.xbe
	$(EXTRACT_XISO) -c bin $@

bin/default.xbe: app/main.exe
	mkdir -p bin
	$(CXBE) -OUT:bin/default.xbe -TITLE:0ldskoo1 app/main.exe

app/main.exe: $(OBJS)
	$(LLDLINK) -subsystem:windows -dll -out:'$@' -entry:XboxCRT xboxkrnl/libxboxkrnl.lib $(OBJS)

%.obj: %.c ${INCLUDES}
	$(CC) $(CFLAGS) -c -o '$@' '$<'

app/vs.vp: app/vs.cg
	$(CGC) -profile vp20 -o '$@' '$<'

app/ps.fp: app/ps.cg
	$(CGC) -profile fp20 -o '$@' '$<'

%.inl: %.vp
	$(VP20COMPILER) '$<' > '$@'

%.inl: %.fp
	$(FP20COMPILER) '$<' > '$@'

cxbe:
	$(MAKE) -C tools/cxbe

vp20compiler:
	$(MAKE) -C tools/vp20compiler

fp20compiler:
	$(MAKE) -C tools/fp20compiler

extract-xiso:
	$(MAKE) -C tools/extract-xiso

tools: cxbe vp20compiler fp20compiler extract-xiso

.PHONY: clean tools cxbe vp20compiler fp20compiler extract-xiso
clean:
	rm -f default.iso bin/default.xbe app/main.exe $(OBJS) $(SHADERINT) $(SHADEROBJ)
