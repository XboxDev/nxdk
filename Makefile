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
UNAME_M := $(shell uname -m)

LD  = nxdk-link
LIB = nxdk-lib
AS  = nxdk-as
CC  = nxdk-cc
CXX = nxdk-cxx

ifeq ($(UNAME_S),Linux)
ifneq ($(UNAME_M),x86_64)
CGC          = $(NXDK_DIR)/tools/cg/linux/cgc.i386
else
CGC          = $(NXDK_DIR)/tools/cg/linux/cgc
endif #UNAME_M != x86_64
endif
ifeq ($(UNAME_S),Darwin)
CGC          = $(NXDK_DIR)/tools/cg/mac/cgc
endif
ifneq (,$(findstring MSYS_NT,$(UNAME_S)))
$(error Please use a MinGW64 shell)
endif
ifneq (,$(findstring MINGW,$(UNAME_S)))
CGC          = $(NXDK_DIR)/tools/cg/win/cgc
endif

TARGET       = $(OUTPUT_DIR)/default.xbe
CXBE         = $(NXDK_DIR)/tools/cxbe/cxbe
VP20COMPILER = $(NXDK_DIR)/tools/vp20compiler/vp20compiler
FP20COMPILER = $(NXDK_DIR)/tools/fp20compiler/fp20compiler
EXTRACT_XISO = $(NXDK_DIR)/tools/extract-xiso/build/extract-xiso
TOOLS        = cxbe vp20compiler fp20compiler extract-xiso

ifeq ($(DEBUG),y)
NXDK_ASFLAGS += -g -gdwarf-4
NXDK_CFLAGS += -g -gdwarf-4
NXDK_CXXFLAGS += -g -gdwarf-4
NXDK_LDFLAGS += -debug
endif

ifeq ($(LTO),y)
NXDK_ASFLAGS += -flto
NXDK_CFLAGS += -flto
NXDK_CXXFLAGS += -flto
endif

ifneq ($(GEN_XISO),)
TARGET += $(GEN_XISO)
endif

ifneq ($(NXDK_ONLY),)
NXDK_CXX = y
NXDK_SDL = y
TARGET = main.exe
endif

all: $(TARGET)

include $(NXDK_DIR)/lib/Makefile
OBJS = $(addsuffix .obj, $(basename $(SRCS)))

ifneq ($(NXDK_CXX),)
include $(NXDK_DIR)/lib/libcxx/Makefile.nxdk
endif

include $(NXDK_DIR)/lib/net/Makefile

ifneq ($(NXDK_SDL),)
include $(NXDK_DIR)/lib/sdl/SDL2/Makefile.xbox
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

DEPS := $(filter %.c.d, $(SRCS:.c=.c.d))
DEPS += $(filter %.cpp.d, $(SRCS:.cpp=.cpp.d))

$(OUTPUT_DIR)/default.xbe: main.exe $(OUTPUT_DIR) $(CXBE)
	@echo "[ CXBE     ] $@"
	$(VE)$(CXBE) -OUT:$@ -TITLE:$(XBE_TITLE) -TITLEID:$(XBE_TITLEID) \
	    -REGION:$(XBE_REGION) -VERSION:$(XBE_VERSION) $< $(QUIET)

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR);

ifneq ($(GEN_XISO),)
$(GEN_XISO): $(OUTPUT_DIR)/default.xbe $(EXTRACT_XISO)
	@echo "[ XISO     ] $@"
	$(VE) $(EXTRACT_XISO) -c $(OUTPUT_DIR) $(XISO_FLAGS) "$@" $(QUIET)
endif

$(SRCS): $(SHADER_OBJS)

ifneq ($(NXDK_ONLY),)
.PHONY: main.exe
main.exe: $(OBJS)
else
main.exe: $(OBJS) $(NXDK_DIR)/lib/xboxkrnl/libxboxkrnl.lib
	@echo "[ LD       ] $@"
	$(VE) $(LD) $(NXDK_LDFLAGS) $(LDFLAGS) -out:'$@' $^
endif

%.lib:
	@echo "[ LIB      ] $@"
	$(VE) $(LIB) -out:'$@' $^

%.obj: %.cpp
	@echo "[ CXX      ] $@"
	$(VE) $(CXX) $(NXDK_CXXFLAGS) $(CXXFLAGS) -MD -MP -MT '$@' -MF '$(patsubst %.obj,%.cpp.d,$@)' -c -o '$@' '$<'

%.obj: %.c
	@echo "[ CC       ] $@"
	$(VE) $(CC) $(NXDK_CFLAGS) $(CFLAGS) -MD -MP -MT '$@' -MF '$(patsubst %.obj,%.c.d,$@)' -c -o '$@' '$<'

%.obj: %.s
	@echo "[ AS       ] $@"
	$(VE) $(AS) $(NXDK_ASFLAGS) $(ASFLAGS) -c -o '$@' '$<'

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
	$(VE)(mkdir $(NXDK_DIR)/tools/extract-xiso/build; \
	cd $(NXDK_DIR)/tools/extract-xiso/build && \
	cmake -G "Unix Makefiles" .. $(QUIET) && \
	$(MAKE) $(QUIET))

.PHONY: clean
clean: $(CLEANRULES)
	$(VE)rm -f $(TARGET) \
	           main.exe main.exe.manifest main.lib \
	           $(OBJS) $(SHADER_OBJS) $(DEPS) \
	           $(GEN_XISO)

.PHONY: distclean
distclean: clean
	$(VE)rm -rf $(NXDK_DIR)/tools/extract-xiso/build
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/fp20compiler distclean $(QUIET)
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/vp20compiler distclean $(QUIET)
	$(VE)$(MAKE) -C $(NXDK_DIR)/tools/cxbe clean $(QUIET)
	$(VE)bash -c "if [ -d $(OUTPUT_DIR) ]; then rmdir $(OUTPUT_DIR); fi"

-include $(DEPS)
