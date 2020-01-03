ZLIB_DIR = $(NXDK_DIR)/lib/zlib/zlib
ZLIB_SRCS = $(ZLIB_DIR)/adler32.c \
            $(ZLIB_DIR)/crc32.c \
            $(ZLIB_DIR)/deflate.c \
            $(ZLIB_DIR)/infback.c \
            $(ZLIB_DIR)/inffast.c \
            $(ZLIB_DIR)/inflate.c \
            $(ZLIB_DIR)/inftrees.c \
            $(ZLIB_DIR)/trees.c \
            $(ZLIB_DIR)/zutil.c

ZLIB_OBJS = $(addsuffix .obj, $(basename $(ZLIB_SRCS)))

NXDK_CFLAGS += -I $(ZLIB_DIR) -DZ_SOLO

$(NXDK_DIR)/lib/libzlib.lib: $(ZLIB_OBJS)

main.exe: $(NXDK_DIR)/lib/libzlib.lib

CLEANRULES += clean-zlib

.PHONY: clean-zlib
clean-zlib:
	$(VE)rm -f $(ZLIB_OBJS) $(NXDK_DIR)/lib/libzlib.lib
