NXDK_SRCS := \
	$(NXDK_DIR)/lib/nxdk/mount.c

NXDK_OBJS = $(addsuffix .obj, $(basename $(NXDK_SRCS)))

$(NXDK_DIR)/lib/libnxdk.lib: $(NXDK_OBJS)

main.exe: $(NXDK_DIR)/lib/libnxdk.lib

CLEANRULES += clean-nxdk

.PHONY: clean-nxdk
clean-nxdk:
	$(VE)rm -f $(NXDK_OBJS) $(NXDK_DIR)/lib/libnxdk.lib
