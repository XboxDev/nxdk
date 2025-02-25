PBKIT_SRCS := \
	$(NXDK_DIR)/lib/pbkit/pbkit.c \
	$(NXDK_DIR)/lib/pbkit/pbkit_dma.c \
	$(NXDK_DIR)/lib/pbkit/pbkit_draw.c \
	$(NXDK_DIR)/lib/pbkit/pbkit_print.c \
	$(NXDK_DIR)/lib/pbkit/pbkit_pushbuffer.c

PBKIT_OBJS = $(addsuffix .obj, $(basename $(PBKIT_SRCS)))

$(NXDK_DIR)/lib/libpbkit.lib: $(PBKIT_OBJS)

main.exe: $(NXDK_DIR)/lib/libpbkit.lib

CLEANRULES += clean-pbkit

.PHONY: clean-pbkit
clean-pbkit:
	$(VE)rm -f $(PBKIT_OBJS) $(NXDK_DIR)/lib/libpbkit.lib
