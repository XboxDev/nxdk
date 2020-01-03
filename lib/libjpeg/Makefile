LIBJPEG_TURBO_DIR = $(NXDK_DIR)/lib/libjpeg/libjpeg-turbo

LIBJPEG_TURBO_SRCS = $(LIBJPEG_TURBO_DIR)/jcapimin.c \
                     $(LIBJPEG_TURBO_DIR)/jcapistd.c \
                     $(LIBJPEG_TURBO_DIR)/jccoefct.c \
                     $(LIBJPEG_TURBO_DIR)/jccolor.c \
                     $(LIBJPEG_TURBO_DIR)/jcdctmgr.c \
                     $(LIBJPEG_TURBO_DIR)/jchuff.c \
                     $(LIBJPEG_TURBO_DIR)/jcicc.c \
                     $(LIBJPEG_TURBO_DIR)/jcinit.c \
                     $(LIBJPEG_TURBO_DIR)/jcmainct.c \
                     $(LIBJPEG_TURBO_DIR)/jcmarker.c \
                     $(LIBJPEG_TURBO_DIR)/jcmaster.c \
                     $(LIBJPEG_TURBO_DIR)/jcomapi.c \
                     $(LIBJPEG_TURBO_DIR)/jcparam.c \
                     $(LIBJPEG_TURBO_DIR)/jcphuff.c \
                     $(LIBJPEG_TURBO_DIR)/jcprepct.c \
                     $(LIBJPEG_TURBO_DIR)/jcsample.c \
                     $(LIBJPEG_TURBO_DIR)/jctrans.c \
                     $(LIBJPEG_TURBO_DIR)/jdapimin.c \
                     $(LIBJPEG_TURBO_DIR)/jdapistd.c \
                     $(LIBJPEG_TURBO_DIR)/jdatadst.c \
                     $(LIBJPEG_TURBO_DIR)/jdatasrc.c \
                     $(LIBJPEG_TURBO_DIR)/jdcoefct.c \
                     $(LIBJPEG_TURBO_DIR)/jdcolor.c \
                     $(LIBJPEG_TURBO_DIR)/jddctmgr.c \
                     $(LIBJPEG_TURBO_DIR)/jdhuff.c \
                     $(LIBJPEG_TURBO_DIR)/jdicc.c \
                     $(LIBJPEG_TURBO_DIR)/jdinput.c \
                     $(LIBJPEG_TURBO_DIR)/jdmainct.c \
                     $(LIBJPEG_TURBO_DIR)/jdmarker.c \
                     $(LIBJPEG_TURBO_DIR)/jdmaster.c \
                     $(LIBJPEG_TURBO_DIR)/jdmerge.c \
                     $(LIBJPEG_TURBO_DIR)/jdphuff.c \
                     $(LIBJPEG_TURBO_DIR)/jdpostct.c \
                     $(LIBJPEG_TURBO_DIR)/jdsample.c \
                     $(LIBJPEG_TURBO_DIR)/jdtrans.c \
                     $(LIBJPEG_TURBO_DIR)/jerror.c \
                     $(LIBJPEG_TURBO_DIR)/jfdctflt.c \
                     $(LIBJPEG_TURBO_DIR)/jfdctfst.c \
                     $(LIBJPEG_TURBO_DIR)/jfdctint.c \
                     $(LIBJPEG_TURBO_DIR)/jidctflt.c \
                     $(LIBJPEG_TURBO_DIR)/jidctfst.c \
                     $(LIBJPEG_TURBO_DIR)/jidctint.c \
                     $(LIBJPEG_TURBO_DIR)/jidctred.c \
                     $(LIBJPEG_TURBO_DIR)/jquant1.c \
                     $(LIBJPEG_TURBO_DIR)/jquant2.c \
                     $(LIBJPEG_TURBO_DIR)/jutils.c \
                     $(LIBJPEG_TURBO_DIR)/jmemmgr.c \
                     $(LIBJPEG_TURBO_DIR)/jmemnobs.c \
                     $(LIBJPEG_TURBO_DIR)/jaricom.c \
                     $(LIBJPEG_TURBO_DIR)/jcarith.c \
                     $(LIBJPEG_TURBO_DIR)/jdarith.c

# FIXME: MMX/SSE is currently disabled due to requiring nasm
LIBJPEG_TURBO_SRCS += $(LIBJPEG_TURBO_DIR)/jsimd_none.c

LIBJPEG_TURBO_OBJS = $(addsuffix .obj, $(basename $(LIBJPEG_TURBO_SRCS)))

NXDK_CFLAGS += -isystem $(LIBJPEG_TURBO_DIR) -I$(NXDK_DIR)/lib/libjpeg
NXDK_CXXFLAGS += -isystem $(LIBJPEG_TURBO_DIR) -I$(NXDK_DIR)/lib/libjpeg

$(NXDK_DIR)/lib/libjpeg.lib: $(LIBJPEG_TURBO_OBJS)

main.exe: $(NXDK_DIR)/lib/libjpeg.lib

CLEANRULES += clean-libjpeg

.PHONY: clean-libjpeg
clean-libjpeg:
	$(VE)rm -f $(LIBJPEG_OBJS) $(NXDK_DIR)/lib/libjpeg.lib
