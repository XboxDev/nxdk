NXDK_SRCS := \
	$(NXDK_DIR)/lib/nxdk/format.c \
	$(NXDK_DIR)/lib/nxdk/mount.c \
	$(NXDK_DIR)/lib/nxdk/path.c

NXDK_OBJS = $(addsuffix .obj, $(basename $(NXDK_SRCS)))

$(NXDK_DIR)/lib/libnxdk.lib: $(NXDK_OBJS)

main.exe: $(NXDK_DIR)/lib/libnxdk.lib

CLEANRULES += clean-nxdk

.PHONY: clean-nxdk
clean-nxdk:
	$(VE)rm -f $(NXDK_OBJS) $(NXDK_DIR)/lib/libnxdk.lib

NXDK_AUTOMOUNT_D_SRCS := \
	$(NXDK_DIR)/lib/nxdk/automount_d.c

NXDK_AUTOMOUNT_D_OBJS = $(addsuffix .obj, $(basename $(NXDK_AUTOMOUNT_D_SRCS)))

$(NXDK_DIR)/lib/libnxdk_automount_d.lib: $(NXDK_AUTOMOUNT_D_OBJS)

ifneq ($(NXDK_DISABLE_AUTOMOUNT_D),y)
main.exe: $(NXDK_DIR)/lib/libnxdk_automount_d.lib
NXDK_LDFLAGS += -include:_automount_d_drive
endif

CLEANRULES += clean-nxdk-automount-d

.PHONY: clean-nxdk-automount-d
clean-nxdk-automount-d:
	$(VE)rm -f $(NXDK_AUTOMOUNT_D_OBJS) $(NXDK_DIR)/lib/libnxdk_automount_d.lib
