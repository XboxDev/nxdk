PBKIT_SRCS := \
	$(NXDK_DIR)/lib/pbkit/pbkit.c

PBKIT_OBJS = $(addsuffix .obj, $(basename $(PBKIT_SRCS)))

$(NXDK_DIR)/lib/libpbkit.lib: $(PBKIT_OBJS)

main.exe: $(NXDK_DIR)/lib/libpbkit.lib

CLEANRULES += clean-pbkit

.PHONY: clean-pbkit
clean-pbkit:
	$(VE)rm -f $(PBKIT_OBJS) $(NXDK_DIR)/lib/libpbkit.lib
