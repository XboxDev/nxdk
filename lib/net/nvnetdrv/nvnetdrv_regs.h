enum {
    NvRegIrqStatus = 0x000,
    NvRegIrqMask = 0x004,
#define NVREG_IRQ_RX_ERROR       0x0001
#define NVREG_IRQ_RX             0x0002
#define NVREG_IRQ_RX_NOBUF       0x0004
#define NVREG_IRQ_TX_ERROR       0x0008
#define NVREG_IRQ_TX_OK          0x0010
#define NVREG_IRQ_TIMER          0x0020
#define NVREG_IRQ_LINK           0x0040
#define NVREG_IRQ_RX_FORCED      0x0080
#define NVREG_IRQ_TX_FORCED      0x0100
#define NVREG_IRQMASK_THROUGHPUT (NVREG_IRQ_RX_ERROR | NVREG_IRQ_RX | NVREG_IRQ_RX_NOBUF | \
                                  NVREG_IRQ_TX_ERROR | NVREG_IRQ_TX_OK | NVREG_IRQ_LINK | NVREG_IRQ_RX_FORCED)
#define NVREG_IRQMASK_CPU        (NVREG_IRQ_TIMER | NVREG_IRQ_LINK)
#define NVREG_IRQ_TX_ALL         (NVREG_IRQ_TX_ERROR | NVREG_IRQ_TX_OK | NVREG_IRQ_TX_FORCED)
#define NVREG_IRQ_RX_ALL         (NVREG_IRQ_RX_ERROR | NVREG_IRQ_RX | NVREG_IRQ_RX_NOBUF | NVREG_IRQ_RX_FORCED)
#define NVREG_IRQ_OTHER          (NVREG_IRQ_TIMER | NVREG_IRQ_LINK)
#define NVREG_IRQ_MASK           0x1FF

    NvRegPollingControl = 0x008,
#define NVREG_POLL_ENABLED 3
#define NVREG_POLL_DISABLED 0

/*
 * NVREG_POLL_DEFAULT is the interval length of the timer source on the nic
 * NVREG_POLL_DEFAULT=97 would result in an interval length of 1 ms
 */
    NvRegPollingInterval = 0x00c,
#define NVREG_POLL_DEFAULT_THROUGHPUT 970
#define NVREG_POLL_DEFAULT_CPU        013

    NvRegDuplexMode = 0x080,
#define NVREG_DUPLEX_MODE_HDFLAG 0x00000002
#define NVREG_DUPLEX_MODE_FORCEF 0x003B0F3C
#define NVREG_DUPLEX_MODE_FORCEH 0x003B0F3E
#define NVREG_DUPLEX_MODE_FDMASK 0xFFFFFFFD

    NvRegTransmitterControl = 0x084,
#define NVREG_XMITCTL_START 0x01

    NvRegTransmitterStatus = 0x088,
#define NVREG_XMITSTAT_BUSY 0x01

// FIXME:  This could use some investigating
    NvRegPacketFilterFlags = 0x08c,
#define NVREG_PFF_ALWAYS        0x7F0008
#define NVREG_PFF_PROMISC       0x000080
#define NVREG_PFF_MYADDR        0x000020
#define NVREG_PFF_ALWAYS_MYADDR 0x7F0020

// FIXME: xnic disagrees?
    NvRegOffloadConfig = 0x090,
#define NVREG_OFFLOAD_HOMEPHY 0x601
#define NVREG_OFFLOAD_NORMAL  0x5EE

    NvRegReceiverControl = 0x094,
#define NVREG_RCVCTL_START 0x01

    NvRegReceiverStatus = 0x098,
#define NVREG_RCVSTAT_BUSY 0x01

// FIXME: check if better data available
	NvRegSlotTime = 0x9c,
#define NVREG_SLOTTIME_LEGBF_ENABLED 0x80000000
#define NVREG_SLOTTIME_10_100_FULL   0x00007f00
#define NVREG_SLOTTIME_1000_FULL     0x0003ff00
#define NVREG_SLOTTIME_HALF          0x0000ff00
#define NVREG_SLOTTIME_DEFAULT       0x00007f00
#define NVREG_SLOTTIME_MASK          0x000000ff

    NvRegTxDeferral = 0x0A0,
#define NVREG_TX_DEFERRAL_RGMII_10_100 0x16070F

    NvRegRxDeferral = 0x0A4,
#define NVREG_RX_DEFERRAL_DEFAULT 0x16

    NvRegMacAddrA = 0x0A8,
    NvRegMacAddrB = 0x0AC,
    NvRegMulticastAddrA = 0x0B0,
#define NVREG_MCASTADDRA_FORCE 0x01
    NvRegMulticastAddrB = 0x0B4,
    NvRegMulticastMaskA = 0x0B8,
#define NVREG_MCASTMASKA_NONE 0xffffffff
    NvRegMulticastMaskB = 0x0BC,
#define NVREG_MCASTMASKB_NONE 0xffff

    NvRegPhyInterface = 0x0C0,
#define PHY_RGMII 0x10000000

    NvRegTxRingPhysAddr = 0x100,
    NvRegRxRingPhysAddr = 0x104,
    NvRegRingSizes = 0x108, // NOTE: these are size-1!
#define NVREG_RINGSZ_TXSHIFT 0
#define NVREG_RINGSZ_RXSHIFT 16

    NvRegTransmitPoll = 0x10c,
#define NVREG_TRANSMITPOLL_MAC_ADDR_REV	0x00008000

    NvRegLinkSpeed = 0x110,
#define NVREG_LINKSPEED_FORCE    0x10000
#define NVREG_LINKSPEED_10MBPS   1000
#define NVREG_LINKSPEED_100MBPS  100
#define NVREG_LINKSPEED_1000MBPS 50
#define NVREG_LINKSPEED_MASK     0xFFF

    NvRegUnknownSetupReg5 = 0x130,
#define NVREG_UNKSETUP5_BIT31 (1 << 31)

    NvRegTxWatermark = 0x13c,
#define NVREG_TX_WM_DESC1_DEFAULT	0x0200010
#define NVREG_TX_WM_DESC2_3_DEFAULT	0x1e08000
#define NVREG_TX_WM_DESC2_3_1000	0xfe08000

// FIXME: used in pktdrv, but not forcedeth
    NvRegUnknownSetupReg7 = 0x140,
#define NVREG_UNKSETUP7_VAL1 0x300010

    NvRegTxRxControl = 0x144,
#define NVREG_TXRXCTL_KICK      0x0001
#define NVREG_TXRXCTL_GET       0x0002 // NOTE: changed it
#define NVREG_TXRXCTL_DISABLE   0x0004 // NOTE: changed it
#define NVREG_TXRXCTL_IDLE      0x0008
#define NVREG_TXRXCTL_RESET     0x0010
#define NVREG_TXRXCTL_RXCHECK   0x0400
#define NVREG_TXRXCTL_DESC_1    0x0000
#define NVREG_TXRXCTL_DESC_2    0x2100
#define NVREG_TXRXCTL_DESC_3    0x2200
#define NVREG_TXRXCTL_VLANSTRIP 0x0040
#define NVREG_TXRXCTL_VLANINS   0x0080

    NvRegMIIStatus          = 0x180,
#define NVREG_MIISTAT_ERROR      0x0001
#define NVREG_MIISTAT_LINKCHANGE 0x0008
#define NVREG_MIISTAT_MASK       0x000F
#define NVREG_MIISTAT_MASK2      0x000F

    NvRegMIIMask = 0x184,
#define NVREG_MII_LINKCHANGE 0x0008

    NvRegAdapterControl = 0x188,
#define NVREG_ADAPTCTL_START    0x02
#define NVREG_ADAPTCTL_LINKUP   0x04
#define NVREG_ADAPTCTL_PHYVALID 0x40000
#define NVREG_ADAPTCTL_RUNNING  0x100000
#define NVREG_ADAPTCTL_PHYSHIFT 24

    NvRegMIISpeed = 0x18c,
#define NVREG_MIISPEED_BIT8 (1 << 8)
#define NVREG_MIIDELAY 5
    NvRegMIIControl = 0x190,
#define NVREG_MIICTL_INUSE     0x08000
#define NVREG_MIICTL_WRITE     0x00400
#define NVREG_MIICTL_ADDRSHIFT 5
    NvRegMIIData = 0x194,

    NvRegWakeUpFlags = 0x200,
#define NVREG_WAKEUPFLAGS_VAL               0x7770
#define NVREG_WAKEUPFLAGS_BUSYSHIFT         24
#define NVREG_WAKEUPFLAGS_ENABLESHIFT       16
#define NVREG_WAKEUPFLAGS_D3SHIFT           12
#define NVREG_WAKEUPFLAGS_D2SHIFT           8
#define NVREG_WAKEUPFLAGS_D1SHIFT           4
#define NVREG_WAKEUPFLAGS_D0SHIFT           0
#define NVREG_WAKEUPFLAGS_ACCEPT_MAGPAT     0x01
#define NVREG_WAKEUPFLAGS_ACCEPT_WAKEUPPAT  0x02
#define NVREG_WAKEUPFLAGS_ACCEPT_LINKCHANGE 0x04
#define NVREG_WAKEUPFLAGS_ENABLE            0x1111

    NvRegPatternCRC = 0x204,
    NvRegPatternMask = 0x208,

    //Additional NVRegs obtained from monitoring Xbox HW:
    NvRegTxCount        = 0x114, //Counts up with transmits?
    NvRegRxCount        = 0x118, //Counts up with receives
    NvRegCurrentTxRdesc = 0x11C, //Pointer to the current TX ring descriptor (struct descriptor_t)
    NvRegCurrentRxRdesc = 0x120, //Pointer to the current RX ring descriptor (struct descriptor_t)
    NvRegCurrentTxBuff  = 0x124, //Pointer to the current TX data buffer.
    NvRegCurrentRxBuff  = 0x12C, //Pointer to the current RX data buffer.
    NvRegNextTxRdesc    = 0x134, //Pointer to the next TX ring descriptor (struct descriptor_t)
    NvRegNextRxRdesc    = 0x138, //Pointer to the next RX ring descriptor (struct descriptor_t)
};

#define NV_TX_LASTPACKET       (1 <<  0)
#define NV_TX_RETRYERROR       (1 <<  3)
#define NV_TX_FORCED_INTERRUPT (1 <<  8)
#define NV_TX_DEFERRED         (1 << 10)
#define NV_TX_CARRIERLOST      (1 << 11)
#define NV_TX_LATECOLLISION    (1 << 12)
#define NV_TX_UNDERFLOW        (1 << 13)
#define NV_TX_ERROR            (1 << 14)
#define NV_TX_VALID            (1 << 15)

#define NV_RX_DESCRIPTORVALID (1 <<  0)
#define NV_RX_MISSEDFRAME     (1 <<  1)
#define NV_RX_SUBTRACT1       (1 <<  2)
#define NV_RX_ERROR1          (1 <<  7)
#define NV_RX_ERROR2          (1 <<  8)
#define NV_RX_ERROR3          (1 <<  9)
#define NV_RX_ERROR4          (1 << 10)
#define NV_RX_CRCERR          (1 << 11)
#define NV_RX_OVERFLOW        (1 << 12)
#define NV_RX_FRAMINGERR      (1 << 13)
#define NV_RX_ERROR           (1 << 14)
#define NV_RX_AVAIL           (1 << 15)