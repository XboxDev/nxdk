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
DRIVERSRCS := $(NXDK_DIR)/lib/net/pktdrv/pktdrv.c \
              $(NXDK_DIR)/lib/net/nforceif/src/driver.c \
              $(NXDK_DIR)/lib/net/nforceif/src/sys_arch.c

SRCS += $(LWIPSRCS) $(DRIVERSRCS)

# Add includes
CFLAGS += -I $(LWIPDIR)/include
CFLAGS += -I $(NXDK_DIR)/lib/net/nforceif/include
CFLAGS += -I $(NXDK_DIR)/lib/net/pktdrv
CXXFLAGS += -I $(LWIPDIR)/include
CXXFLAGS += -I $(NXDK_DIR)/lib/net/nforceif/include
CXXFLAGS += -I $(NXDK_DIR)/lib/net/pktdrv
