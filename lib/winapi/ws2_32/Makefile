NXDK_NET=y

WS2_32_SRCS := \
	$(wildcard $(NXDK_DIR)/lib/winapi/ws2_32/*.c)

WS2_32_OBJS = $(addsuffix .obj, $(basename $(WS2_32_SRCS)))

NXDK_CFLAGS += -I$(NXDK_DIR)/lib/winapi/ws2_32 -I$(NXDK_DIR)/lib/net/pktdrv
NXDK_CXXFLAGS += -I$(NXDK_DIR)/lib/winapi/ws2_32 -I$(NXDK_DIR)/lib/net/pktdrv

$(NXDK_DIR)/lib/ws2_32.lib: $(WS2_32_OBJS)

main.exe: $(NXDK_DIR)/lib/ws2_32.lib

CLEANRULES += clean-ws2_32

.PHONY: clean-ws2_32
clean-ws2_32:
	$(VE)rm -f $(WS2_32_OBJS) $(NXDK_DIR)/lib/ws2_32.lib
