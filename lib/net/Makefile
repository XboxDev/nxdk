# Include LWIP sources
LWIPDIR = $(NXDK_DIR)/lib/net/lwip/src
include $(LWIPDIR)/Filelists.mk
NETIFFILES := $(filter-out $(LWIPDIR)/netif/slipif.c,$(NETIFFILES))
LWIPSRCS := $(COREFILES) \
            $(CORE4FILES) \
            $(CORE6FILES) \
            $(NETIFFILES) \
            $(APIFILES)

# Include driver sources
DRIVERSRCS := $(wildcard $(NXDK_DIR)/lib/net/pktdrv/*.c) \
              $(wildcard $(NXDK_DIR)/lib/net/nforceif/src/*.c)

SRCS += $(LWIPSRCS) $(DRIVERSRCS)

# Add includes
CFLAGS += -I $(LWIPDIR)/include
CFLAGS += -I $(NXDK_DIR)/lib/net/nforceif/include
CFLAGS += -I $(NXDK_DIR)/lib/net/pktdrv
