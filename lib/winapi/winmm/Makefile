WINMM_SRCS := \
	$(NXDK_DIR)/lib/winapi/winmm/timeapi.c

WINMM_OBJS = $(addsuffix .obj, $(basename $(WINMM_SRCS)))

NXDK_CFLAGS += -I$(NXDK_DIR)/lib/winapi/winmm
NXDK_CXXFLAGS += -I$(NXDK_DIR)/lib/winapi/winmm

$(NXDK_DIR)/lib/winmm.lib: $(WINMM_OBJS)

main.exe: $(NXDK_DIR)/lib/winmm.lib

CLEANRULES += clean-winmm

.PHONY: clean-winmm
clean-winmm:
	$(VE)rm -f $(WINMM_OBJS) $(NXDK_DIR)/lib/winmm.lib
