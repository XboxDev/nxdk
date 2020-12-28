zstd_DIR	= $(NXDK_DIR)/lib/zstd/zstd
zstd_SRCS	= $(sort $(wildcard $(zstd_DIR)/lib/common/*.c))
zstd_SRCS	+= $(sort $(wildcard $(zstd_DIR)/lib/compress/*.c))
zstd_SRCS	+= $(sort $(wildcard $(zstd_DIR)/lib/decompress/*.c))
zstd_SRCS	+= $(sort $(wildcard $(zstd_DIR)/lib/dictBuilder/*.c))

zstd_OBJS = $(addsuffix .obj, $(basename $(zstd_SRCS)))

NXDK_CFLAGS += -I $(zstd_DIR)/lib -D_byteswap_ulong=__builtin_bswap32 -D_byteswap_uint64=__builtin_bswap64

$(NXDK_DIR)/lib/libzstd.lib: $(zstd_OBJS)

main.exe: $(NXDK_DIR)/lib/libzstd.lib

CLEANRULES += clean-zstd

.PHONY: clean-zstd
clean-zstd:
	$(VE)rm -f $(zstd_OBJS) $(NXDK_DIR)/lib/libzstd.lib
