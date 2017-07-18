//XBOX low level network interface driver (packet driver)
//=======================================================
//Minimum capabilites : send and receive ethernet packets
//Based on forcedeth Linux nForce driver

//Ring of buffers for received packets is handled by driver.
//Ring of buffers for sent packets must be handled by yourself.
//Driver will only handle the ring of descriptors for them.
//i.e Call Pktdrv_SendPacket with a buffer address taken among
//your ring of n buffers in a circular way and always verify
//that Pktdrv_GetQueuedTxPkts()<n before calling Pktdrv_SendPacket
//In most case, people will just handle n=1 buffer for sending.
//Buffers must be contiguous memory buffers (use Mm fonction).

//int Pktdrv_Callback(unsigned char *packetaddr, unsigned int packetsize)
//has to be defined in your code. Check frequently the number of pkts
//your callback accepted with Pktdrv_ReceivePackets() to trigger calls.
//(NBBUFF pkts will be safely stored until you decide to check for them)
//return 0 if you couldn't receive packet (will be kept for next time).

//In MyPktdrvDpc you can uncomment a line in order to have your callback
//called as soon as a packet arrives (your callback will be called by
//a DPC which means you don't need to be reentrant but you shouldn't
//mess with fpu unless you save and restore fpu processor state)

#include <hal/xbox.h>
#include <xboxrt/debug.h>
#include <xboxkrnl/xboxkrnl.h>

#include "string.h"
#include "stdio.h"
#include <stdlib.h>
//#include <memory.h>

#include "pktdrv.h"

#define DISPLAYMSG

//Defines number of Rx & Tx descriptors, and number of buffers -ring- for received pkts
#define NBBUFF	32

extern unsigned long times(void *);

//temporary dirty interface
extern int something_to_send;
extern unsigned char *packet_to_send;
extern unsigned int size_of_packet_to_send;
extern int Pktdrv_Callback(unsigned char *packetaddr, unsigned int packetsize);

#define MIN(a,b)	(((a)<(b))?(a):(b))

struct s_MyStructures {
	char 	MyContext[1];
	unsigned char NbrRxBuffersWithoutCheck;
	unsigned char Ethaddr[6];		
	unsigned char Ethaddr2[6];		
	unsigned char Ethaddr_reversed[6];
	KDPC 	MyPktdrvDpcObject;
	ULONG	PktdrvIsrCounter;
	KIRQL	IrqLevel;	
	ULONG	Vector;		
	ULONG	Speed;		
	ULONG	OldPhyState;
	ULONG	PhysicalMinusVirtual; // = buffers_physaddr - buffers_addrs;
	ULONG	NbrRxBuffers;	
	ULONG	RxBufferDesc;	//= buffers_addr + 2048; 
	ULONG 	RxBufferTail;	//= buffers_addr + 2048 + g_s->NbrRxBuffers * 8 - 8;
	ULONG	RxBufferNext;	//= buffers_addr + 2048; //Point to next incoming packet entry
	ULONG	NbrTxBuffers;
	ULONG	TxBufferDesc;	//= buffers_addr;
	ULONG 	TxBufferLast;	//= buffers_addr; //Points to last sent packet(s) to check
	ULONG 	TxBufferNext;	//= buffers_addr; //Points to next packet to send entry
	ULONG 	TxBufferTail; 	//= buffers_addr + 8 * g_s->NbrTxBuffers - 8;
	ULONG	QueuedTxPkts;
	};


static int 			g_running=0;
static struct s_MyStructures 	*g_s;
static KINTERRUPT 		s_MyInterruptObject;



//Types and descriptions coming from 
//- ntddk.h (WinXP SP1 DDK)
//- winddk.h (Reactos source)
//- forcedeth.c (Linux NVidia nForce driver)

/*
 * Hardware registers:
 */

enum {
	NvRegIrqStatus 		= 0x000,
#define NVREG_IRQSTAT_BIT0EVENT		0x002
#define NVREG_IRQSTAT_BIT1EVENT		0x004
#define NVREG_IRQSTAT_BIT2EVENT		0x008
#define NVREG_IRQSTAT_MIIEVENT		0x040
#define NVREG_IRQSTAT_UNKEVENT		0x080
#define NVREG_IRQSTAT_MASK		0x1FF

	NvRegIrqMask 		= 0x004,
#define NVREG_IRQ_RX_ERROR		0x0001
#define NVREG_IRQ_RX			0x0002
#define NVREG_IRQ_RX_NOBUF		0x0004
#define NVREG_IRQ_TX_ERROR		0x0008
#define NVREG_IRQ_TX_OK			0x0010
#define NVREG_IRQ_TIMER			0x0020
#define NVREG_IRQ_LINK			0x0040
#define NVREG_IRQ_RX_FORCED		0x0080
#define NVREG_IRQ_TX_FORCED		0x0100
#define NVREG_IRQMASK_THROUGHPUT	0x00DF
#define NVREG_IRQMASK_CPU		0x0040
#define NVREG_IRQ_TX_ALL		0x0118
//=(NVREG_IRQ_TX_ERROR|NVREG_IRQ_TX_OK|NVREG_IRQ_TX_FORCED)
#define NVREG_IRQ_RX_ALL		0x0087
//=(NVREG_IRQ_RX_ERROR|NVREG_IRQ_RX|NVREG_IRQ_RX_NOBUF|NVREG_IRQ_RX_FORCED)
#define NVREG_IRQ_OTHER			0x0060
//=(NVREG_IRQ_TIMER|NVREG_IRQ_LINK)
#define NVREG_IRQ_UNKNOWN		0x01FF
//=(~
//		(
//			NVREG_IRQ_RX_ERROR|NVREG_IRQ_RX|NVREG_IRQ_RX_NOBUF|
//			NVREG_IRQ_TX_ERROR|NVREG_IRQ_TX_OK|NVREG_IRQ_TIMER|
//			NVREG_IRQ_LINK|NVREG_IRQ_RX_FORCED|NVREG_IRQ_TX_FORCED
//		)
// )

	NvRegUnknownSetupReg6 	= 0x008,
#define NVREG_UNKSETUP6_VAL		3

/*
 * NVREG_POLL_DEFAULT is the interval length of the timer source on the Pktdrv
 * NVREG_POLL_DEFAULT=97 would result in an interval length of 1 ms
 */
	NvRegPollingInterval 	= 0x00c,
#define NVREG_POLL_DEFAULT_THROUGHPUT	970
#define NVREG_POLL_DEFAULT_CPU		013

	NvRegMSIMap0 		= 0x020,
	NvRegMSIMap1 		= 0x024,
	NvRegMSIIrqMask 	= 0x030,
#define NVREG_MSI_VECTOR_0_ENABLED 	0x001

	NvRegMacReset 		= 0x03c,
#define NVREG_MAC_RESET_ASSERT		0x0F3

	NvRegDuplexMode		= 0x080,
#define NVREG_DUPLEX_MODE_HDFLAG	0x00000002
#define NVREG_DUPLEX_MODE_FORCEF	0x003B0F3C
#define NVREG_DUPLEX_MODE_FORCEH	0x003B0F3E
#define NVREG_DUPLEX_MODE_FDMASK	0xFFFFFFFD

	NvRegTransmitterControl = 0x084,
#define NVREG_SendCTL_START		0x01

	NvRegTransmitterStatus 	= 0x088,
#define NVREG_SendSTAT_BUSY		0x01

	NvRegPacketFilterFlags 	= 0x08c,
#define NVREG_PFF_ALWAYS		0x7F0008
#define NVREG_PFF_PROMISC		0x000080
#define NVREG_PFF_MYADDR		0x000020
#define NVREG_PFF_ALWAYS_MYADDR		0x7F0020

	NvRegOffloadConfig 	= 0x090,
#define NVREG_OFFLOAD_HOMEPHY		0x601
#define NVREG_OFFLOAD_NORMAL		0x5EE

	NvRegReceiverControl 	= 0x094,
#define NVREG_RCVCTL_START		0x01

	NvRegReceiverStatus 	= 0x098,
#define NVREG_RCVSTAT_BUSY		0x01

	NvRegRandomSeed 	= 0x09c,
#define NVREG_RNDSEED_MASK		0x00FF
#define NVREG_RNDSEED_FORCE		0x7F00
#define NVREG_RNDSEED_FORCE2		0x2D00
#define NVREG_RNDSEED_FORCE3		0x7400

	NvRegUnknownSetupReg1 	= 0x0A0,
#define NVREG_UNKSETUP1_VAL		0x16070F
	NvRegUnknownSetupReg2 	= 0x0A4,
#define NVREG_UNKSETUP2_VAL		0x16

	NvRegMacAddrA 		= 0x0A8,
	NvRegMacAddrB 		= 0x0AC,
	NvRegMulticastAddrA 	= 0x0B0,
#define NVREG_MCASTADDRA_FORCE		0x01
	NvRegMulticastAddrB 	= 0x0B4,
	NvRegMulticastMaskA 	= 0x0B8,
	NvRegMulticastMaskB 	= 0x0BC,

	NvRegPhyInterface 	= 0x0C0,
#define PHY_RGMII			0x10000000

	NvRegTxRingPhysAddr 	= 0x100,
	NvRegRxRingPhysAddr 	= 0x104,
	NvRegRingSizes 		= 0x108,
#define NVREG_RINGSZ_TXSHIFT 		0
#define NVREG_RINGSZ_RXSHIFT 		16

	NvRegUnkTransmitterReg 	= 0x10c,

	NvRegLinkSpeed 		= 0x110,
#define NVREG_LINKSPEED_FORCE 		0x10000
#define NVREG_LINKSPEED_10MBPS		1000
#define NVREG_LINKSPEED_100MBPS		100
#define NVREG_LINKSPEED_1000MBPS	50
#define NVREG_LINKSPEED_MASK		0xFFF

	NvRegUnknownSetupReg5 	= 0x130,
#define NVREG_UNKSETUP5_BIT31		(1<<31)
	NvRegUnknownSetupReg3 	= 0x13C,
#define NVREG_UNKSETUP3_VAL1		0x200010
	NvRegUnknownSetupReg7 	= 0x140,
#define NVREG_UNKSETUP7_VAL1		0x300010

	NvRegTxRxControl 	= 0x144,
#define NVREG_TXRXCTL_KICK		0x0001
#define NVREG_TXRXCTL_BIT1		0x0002
#define NVREG_TXRXCTL_BIT2		0x0004
#define NVREG_TXRXCTL_IDLE		0x0008
#define NVREG_TXRXCTL_RESET		0x0010
#define NVREG_TXRXCTL_RXCHECK		0x0400
#define NVREG_TXRXCTL_DESC_1		0x0000
#define NVREG_TXRXCTL_DESC_2		0x2100
#define NVREG_TXRXCTL_DESC_3		0x2200
#define NVREG_TXRXCTL_VLANSTRIP 	0x0040
#define NVREG_TXRXCTL_VLANINS		0x0080

	NvRegTxRingPhysAddrHigh = 0x148,
	NvRegRxRingPhysAddrHigh = 0x14C,
	NvRegMIIStatus 		= 0x180,
#define NVREG_MIISTAT_ERROR		0x0001
#define NVREG_MIISTAT_LINKCHANGE	0x0008
#define NVREG_MIISTAT_MASK		0x000F
#define NVREG_MIISTAT_MASK2		0x000F

	NvRegUnknownSetupReg4 	= 0x184,
#define NVREG_UNKSETUP4_VAL		8

	NvRegAdapterControl 	= 0x188,
#define NVREG_ADAPTCTL_START		0x02
#define NVREG_ADAPTCTL_LINKUP		0x04
#define NVREG_ADAPTCTL_PHYVALID		0x40000
#define NVREG_ADAPTCTL_RUNNING		0x100000
#define NVREG_ADAPTCTL_PHYSHIFT		24

	NvRegMIISpeed 		= 0x18c,
#define NVREG_MIISPEED_BIT8		(1<<8)
#define NVREG_MIIDELAY	5
	NvRegMIIControl 	= 0x190,
#define NVREG_MIICTL_INUSE		0x08000
#define NVREG_MIICTL_WRITE		0x00400
#define NVREG_MIICTL_ADDRSHIFT		5
	NvRegMIIData 		= 0x194,

	NvRegWakeUpFlags 	= 0x200,
#define NVREG_WAKEUPFLAGS_VAL		0x7770
#define NVREG_WAKEUPFLAGS_BUSYSHIFT	24
#define NVREG_WAKEUPFLAGS_ENABLESHIFT	16
#define NVREG_WAKEUPFLAGS_D3SHIFT	12
#define NVREG_WAKEUPFLAGS_D2SHIFT	8
#define NVREG_WAKEUPFLAGS_D1SHIFT	4
#define NVREG_WAKEUPFLAGS_D0SHIFT	0
#define NVREG_WAKEUPFLAGS_ACCEPT_MAGPAT		0x01
#define NVREG_WAKEUPFLAGS_ACCEPT_WAKEUPPAT	0x02
#define NVREG_WAKEUPFLAGS_ACCEPT_LINKCHANGE	0x04
#define NVREG_WAKEUPFLAGS_ENABLE	0x1111

	NvRegPatternCRC 	= 0x204,
	NvRegPatternMask 	= 0x208,

	NvRegPowerCap 		= 0x268,
#define NVREG_POWERCAP_D3SUPP		(1<<30)
#define NVREG_POWERCAP_D2SUPP		(1<<26)
#define NVREG_POWERCAP_D1SUPP		(1<<25)
	NvRegPowerState 	= 0x26c,
#define NVREG_POWERSTATE_POWEREDUP	0x8000
#define NVREG_POWERSTATE_VALID		0x0100
#define NVREG_POWERSTATE_MASK		0x0003
#define NVREG_POWERSTATE_D0		0x0000
#define NVREG_POWERSTATE_D1		0x0001
#define NVREG_POWERSTATE_D2		0x0002
#define NVREG_POWERSTATE_D3		0x0003

	NvRegVlanControl 	= 0x300,
#define NVREG_VLANCONTROL_ENABLE	0x2000

	NvRegMSIXMap0 		= 0x3E0,
	NvRegMSIXMap1 		= 0x3E4,
	NvRegMSIXIrqStatus 	= 0x3F0,

	NvRegPowerState2 	= 0x600,
#define NVREG_POWERSTATE2_POWERUP_MASK		0x0F11
#define NVREG_POWERSTATE2_POWERUP_REV_A3	0x0001
};



#define	EEPROM_INDEX_MACADDR	0x101



#define FLAG_MASK_V1 0xffff0000
#define LEN_MASK_V1 (0xffffffff ^ FLAG_MASK_V1)

#define NV_TX_LASTPACKET	(1<<16)
#define NV_TX_RETRYERROR	(1<<19)
#define NV_TX_FORCED_INTERRUPT	(1<<24)
#define NV_TX_DEFERRED		(1<<26)
#define NV_TX_CARRIERLOST	(1<<27)
#define NV_TX_LATECOLLISION	(1<<28)
#define NV_TX_UNDERFLOW		(1<<29)
#define NV_TX_ERROR		(1<<30)
#define NV_TX_VALID		(1<<31)

#define NV_RX_DESCRIPTORVALID	(1<<16)
#define NV_RX_MISSEDFRAME	(1<<17)
#define NV_RX_SUBSTRACT1	(1<<18)
#define NV_RX_ERROR1		(1<<23)
#define NV_RX_ERROR2		(1<<24)
#define NV_RX_ERROR3		(1<<25)
#define NV_RX_ERROR4		(1<<26)
#define NV_RX_CRCERR		(1<<27)
#define NV_RX_OVERFLOW		(1<<28)
#define NV_RX_FRAMINGERR	(1<<29)
#define NV_RX_ERROR		(1<<30)
#define NV_RX_AVAIL		(1<<31)


//PhyGetLinkState
#define PHY_LINK_RUNNING	0x01
#define PHY_LINK_100MBPS	0x02
#define PHY_LINK_10MBPS		0x04
#define PHY_LINK_FULL_DUPLEX	0x08
#define PHY_LINK_HALF_DUPLEX	0x10




//Register access macros for XBOX
#define	BASE	0xFEF00000
#define REG(x)	(*((DWORD *)(BASE+(x))))
#define REGW(x)	(*((WORD *)(BASE+(x))))
#define REGB(x)	(*((BYTE *)(BASE+(x))))




typedef enum _MEMORY_CACHING_TYPE_ORIG {
    MmFrameBufferCached = 2
} MEMORY_CACHING_TYPE_ORIG;

typedef enum _MEMORY_CACHING_TYPE {
    MmNonCached = 0,
    MmCached = 1,
    MmWriteCombined = MmFrameBufferCached,
    MmHardwareCoherentCached,
    MmNonCachedUnordered,       // IA64
    MmUSWCCached,
    MmMaximumCacheType
} MEMORY_CACHING_TYPE;
/*
	MmNonCached : The requested memory should not be cached by the processor. 
	MmCached : The processor should cache the requested memory. 
	MmWriteCombined : The requested memory should not be cached by the processor, 
	 but writes to the memory can be combined by the processor. 
*/




//Checks for possible received packets
static int PktdrvRecvInterrupt(void)
{
	ULONG 	p;
	ULONG	flag;
	BOOLEAN fatal;
	int	handled;
	int	n=0;

	//Look for next entry in Rx ring and read its flag
	while(1)
	{
		p=g_s->RxBufferNext;
		flag=*((ULONG *)(p+4));

		if (flag & NV_RX_AVAIL) return n; //we received nothing!

		if ((flag & NV_RX_DESCRIPTORVALID) == 0)
		{
			//Not a received packet
		}
		else
		{	
			fatal=FALSE;

			if (flag & NV_RX_ERROR)
			{
				if (flag & NV_RX_FRAMINGERR) { /* not fatal */ }
				if (flag & NV_RX_OVERFLOW) { fatal=TRUE; }
				if (flag & NV_RX_CRCERR) { fatal=TRUE; }
				if (flag & NV_RX_ERROR4) { fatal=TRUE; }
				if (flag & NV_RX_ERROR3) { fatal=TRUE; }
				if (flag & NV_RX_ERROR2) { fatal=TRUE; }
				if (flag & NV_RX_ERROR1) { fatal=TRUE; }
			}

			if (!fatal)
			{
				//Call user callback and warn that a packet has been received
				//Phys Addr of packet is *p
				//Length of packet is 1 up to 2046 bytes
				//Length = ( (*((ULONG *)(p+4))) & 0x7FF ) + 1
				handled=Pktdrv_Callback(
				(unsigned char *)( (*((ULONG *)p))-g_s->PhysicalMinusVirtual),
				(unsigned int)( ( (*((ULONG *)(p+4))) & 0x7FF ) + 1 )	);
				if (!handled) 
					return n; //We probably lack space up there
				else
					n++;
			}
		}

		//Empty the entry
		*((ULONG *)(p+4))=NV_RX_AVAIL | 2045;
			
		//Have RxBufferNext Point to next entry in ring
		if (g_s->RxBufferNext==g_s->RxBufferTail) //return to start of ring?
			g_s->RxBufferNext=g_s->RxBufferDesc;
		else
			g_s->RxBufferNext+=8;
	};

	return n;
}


//Detects if Tx ring is full or not
static BOOLEAN PktdrvSendReady(void)
{
	return (g_s->QueuedTxPkts!=g_s->NbrTxBuffers);
}

//Checks for a patcket to send
static void PktdrvSendInterrupt(void) 
{
	ULONG p,flag;

	//Before we send any packet, let's check if last packets have been sent

	while (g_s->TxBufferLast!=g_s->TxBufferNext)
	{
		p=g_s->TxBufferLast;
		flag=*((ULONG *)(p+4));

		if ((flag & NV_TX_VALID) == 0)
		{
			//Packet is gone, reduce counter and check next one.
			//Note that errors and actual number of bytes sent
			//can be read from flag right now and right here!
			//Actual number is higher because of padding...
			g_s->QueuedTxPkts--;
			//Let's cleanup
			*((ULONG *)p)=0;
			*((ULONG *)(p+4))=0;
			//Have TxBufferLast point to next entry
			if (g_s->TxBufferLast==g_s->TxBufferTail)
				g_s->TxBufferLast=g_s->TxBufferDesc;
			else
				g_s->TxBufferLast+=8;
		}
		else
			break; //packet not sent already, we will check later
	}
}


static void PktdrvSendPacket(unsigned char *buffer, int length)
{
	ULONG p;

	if (PktdrvSendReady()) //Do we have room in the Tx ring?
	{
		//p points to next free entry of Tx ring descriptor
		p=g_s->TxBufferNext;
		MmLockUnlockBufferPages(
				(PVOID)buffer,
				length,
				0);

		*((ULONG *)p) = MmGetPhysicalAddress(buffer);
		*((ULONG *)(p+4)) = (length-1) | 
				NV_TX_VALID | 
				NV_TX_LASTPACKET;

		g_s->QueuedTxPkts++;

		//Have TxBufferNext point to next entry
		if (g_s->TxBufferNext==g_s->TxBufferTail) //return to start of ring?
			g_s->TxBufferNext=g_s->TxBufferDesc;
		else
			g_s->TxBufferNext+=8;

		REG(NvRegTxRxControl)=NVREG_TXRXCTL_KICK;
	}
	else
	{
		//Tx ring is full
		//User should do : while(Xnet_GetQueuedPkts()>=n) { /*wait*/ }; then send pkt
		//That will prevent the loss of sent packet right here
		//Where n is the number of buffers (ring) where pending outcoming packets are
		//stored. In most case n=1 (just one buffer is used to send 1 packet at a time)
	}
}


//Starts Pktdrv
static void PktdrvStartSendRecv(void)
{
	REG(NvRegLinkSpeed) = NVREG_LINKSPEED_FORCE | g_s->Speed;

	REG(NvRegTransmitterControl) = NVREG_SendCTL_START;
	REG(NvRegReceiverControl) = NVREG_RCVCTL_START;

	REG(NvRegTxRxControl)= NVREG_TXRXCTL_KICK | NVREG_TXRXCTL_BIT1;
}

//Stops Pktdrv
static void PktdrvStopSendRecv(void)
{
	int 	i;

	REG(NvRegLinkSpeed)=0;
	REG(NvRegReceiverControl)=0;
	REG(NvRegTransmitterControl)=0;

	for(i=0;i<500;i++)
	{
		if (	((REGB(NvRegReceiverStatus) & NVREG_RCVSTAT_BUSY)==0)	
			&&
			((REGB(NvRegTransmitterStatus) & NVREG_SendSTAT_BUSY)==0)	)
			break;
		KeStallExecutionProcessor(10); //Wait 10 microseconds
	}

	REG(NvRegTxRxControl)=NVREG_TXRXCTL_BIT2;

	for(i=0;i<100000;i++)
	{
		if (REGB(NvRegTxRxControl) & NVREG_TXRXCTL_IDLE) break;
		KeStallExecutionProcessor(10);	
	}

	REG(NvRegTxRxControl)=0;
}

//Resets Pktdrv
static void PktdrvReset(void)
{
	PktdrvStopSendRecv();

	REG(NvRegTxRxControl)=NVREG_TXRXCTL_RESET;
	KeStallExecutionProcessor(10); //10 microseconds of busy-wait

	REG(NvRegTxRxControl)=0;
	KeStallExecutionProcessor(10);	

	REG(NvRegUnknownSetupReg4)=0;
	REG(NvRegIrqMask)=0;
	REG(NvRegWakeUpFlags)=0;
	REG(NvRegUnknownSetupReg6)=0;
	REG(NvRegTxRingPhysAddr)=0;
	REG(NvRegRxRingPhysAddr)=0;
	REG(NvRegUnkTransmitterReg)=0;
	REG(NvRegLinkSpeed)=0;

	REG(NvRegTransmitterStatus)=REG(NvRegTransmitterStatus);
	REG(NvRegReceiverStatus)=REG(NvRegReceiverStatus);
	REG(NvRegMIIStatus)=REG(NvRegMIIStatus);
	REG(NvRegIrqStatus)=REG(NvRegIrqStatus);
}


//Checks possible speed or duplex mode change
static void PktdrvMiiInterrupt(int mode) 
{
	//Verifies if speed or duplex mode changed
	//mode=1 -> startup mode (calls PhyGetLinkState(0))
	//(used at startup, before Pktdrv driver is started)
	//mode=0 -> running mode (calls PhyGetLinkState(1))
	//(used in interrupt, while Pktdrv driver is running)
	ULONG state,dummy;

	mode&=1; //only 0 or 1 is accepted
	dummy=REG(NvRegAdapterControl); //just read it
	state=PhyGetLinkState(1-mode);
	if ((mode==0)&&(state==g_s->OldPhyState)) return; //All is ok, remain silent

	//We want details (startup) or state changed (both modes)
#ifdef DISPLAYMSG
	debugPrint("Transceiver link state :\n");
	if (state & PHY_LINK_RUNNING) 
		debugPrint(" Running at "); 
	else 
		debugPrint(" NOT running at ");
	if (state & PHY_LINK_100MBPS) 
		debugPrint("100 Mbps ");
	else 
		if (state & PHY_LINK_10MBPS) 
			debugPrint("10 Mbps ");
		else 
			debugPrint("unknown speed ");
	if (state & PHY_LINK_FULL_DUPLEX)
		debugPrint("in full duplex mode\n");
	else
		if (state & PHY_LINK_HALF_DUPLEX)
			debugPrint("in half duplex mode\n");
		else
			debugPrint("in unknown duplex mode\n");
#endif
	if (mode==0) PktdrvStopSendRecv(); //Pktdrv is running. Stop it.

	if (state & PHY_LINK_10MBPS) //update Speed member in structure
		g_s->Speed=NVREG_LINKSPEED_10MBPS; //decimal value 1000!!!
	else
		g_s->Speed=NVREG_LINKSPEED_100MBPS; //decimal value 100

	if (state & PHY_LINK_FULL_DUPLEX) //update mode in Pktdrv register
		REG(NvRegDuplexMode) &= NVREG_DUPLEX_MODE_FDMASK;
	else	
		REG(NvRegDuplexMode) |= NVREG_DUPLEX_MODE_HDFLAG;

	if (mode==0) PktdrvStartSendRecv(); //Mode changed. Restart Pktdrv.
	//This function will read g_s->Speed and program speed link register.

	g_s->OldPhyState=state;
}



static BOOLEAN __stdcall MyPktdrvIsr(PKINTERRUPT Interrupt, PVOID ServiceContext)
{
	REG(NvRegIrqMask)=0;
	if (g_running)
	{
		KeInsertQueueDpc(&g_s->MyPktdrvDpcObject,NULL,NULL);
		g_s->PktdrvIsrCounter++;
	}
	return TRUE;
}



static void __stdcall MyPktdrvDpc(		PKDPC Dpc, 
					PVOID DeferredContext, 
					PVOID SystemArgument1, 
					PVOID SystemArgument2	)
{
	//DPCs allow to use non reentrant procedures (called sequentially, FOR SURE).
	//CAUTION : if you use fpu in DPC you have to save & restore yourself fpu state!!!
	//(fpu=floating point unit, i.e the coprocessor executing floating point opcodes)

	ULONG irq_status;
	ULONG mii_status;

	if (g_running==0) return;

	mii_status=0;
	irq_status=	NVREG_IRQSTAT_BIT0EVENT |	
			NVREG_IRQSTAT_BIT1EVENT	|
			NVREG_IRQSTAT_BIT2EVENT	|
			NVREG_IRQSTAT_UNKEVENT;

	while (irq_status)
	{
		if (irq_status & NVREG_IRQSTAT_MIIEVENT) PktdrvMiiInterrupt(0);
		REG(NvRegMIIStatus)=mii_status;
		REG(NvRegIrqStatus)=irq_status;
//uncomment this line if you want your callback to be called as soon as packet arrived
//		PktdrvRecvInterrupt(); //Check if we received packets // (let them stock up)
		PktdrvSendInterrupt(); //Check if we have a packet to send

		if (irq_status & NVREG_IRQSTAT_BIT1EVENT)
			REG(NvRegTxRxControl)=NVREG_TXRXCTL_BIT1;
		mii_status=REG(NvRegMIIStatus);
		irq_status=REG(NvRegIrqStatus);
	};
	REG(NvRegIrqMask)=	NVREG_IRQ_LINK | 	
				NVREG_IRQ_TX_OK | 
				NVREG_IRQ_TX_ERROR |
				NVREG_IRQ_RX_NOBUF |
				NVREG_IRQ_RX |
				NVREG_IRQ_RX_ERROR;

	return;
}


void Pktdrv_Quit(void)
{
	if (g_running==0) return;

	g_running=0;

	PktdrvStopSendRecv();
	PktdrvReset();
	KeDisconnectInterrupt(&s_MyInterruptObject);

	MmFreeContiguousMemory((void *)g_s->TxBufferDesc);
	free(g_s);
}





//Returns 1 if everything is ok
int Pktdrv_Init(void)
{
	int 	n;
	ULONG	len;
	ULONG	type;
	ULONG 	buffers_addr;
	ULONG	buffers_physaddr;
	ULONG	status;
	ULONG	buffers_total_size;
	ULONG	p,p2;
	ULONG	RandomValue;

	if (g_running==1) return 1;

	g_s=(struct s_MyStructures *)malloc(sizeof(struct s_MyStructures));
	//g_s holds the various needed structures
	if (!g_s)
	{
		debugPrint("Can't allocate global structure.\n");
		return 0;
	}
	

	g_s->Vector=HalGetInterruptVector(4,&g_s->IrqLevel); 
	
	KeInitializeDpc(&g_s->MyPktdrvDpcObject,&MyPktdrvDpc,&g_s->MyContext); 

	KeInitializeInterrupt(	&s_MyInterruptObject,
				&MyPktdrvIsr,
				&g_s->MyContext,
				g_s->Vector,
				g_s->IrqLevel,
				LevelSensitive,	
				TRUE	);		

	PktdrvReset();

	g_s->NbrRxBuffersWithoutCheck=NBBUFF; //Total buffers = NBBUFF+2 (Tx&Rx Descriptors)

	n = g_s->NbrRxBuffersWithoutCheck; 	
	g_s->NbrRxBuffers = MIN(n,256); 	
	g_s->NbrTxBuffers = MIN(n,256); 	

	//Rx ring will point to the pool of n allocated buffers
	//Tx ring is empty at startup may point to any contiguous buffer physical address

	buffers_total_size = ((n+1+1)<<11);

	//allocates n+1+1 DMA buffers 2048 bytes each
	buffers_addr=(ULONG)MmAllocateContiguousMemoryEx(
		buffers_total_size,
		0,		//lowest acceptable
		0x10000,	//highest acceptable
		0,  		//no need to align to specific boundaries multiple
		MmNonCachedUnordered);	//4
	if (!buffers_addr)
		buffers_addr=(ULONG)MmAllocateContiguousMemoryEx(
			buffers_total_size,
			0,		//lowest acceptable
			0xFFFFFFFF,	//highest acceptable
			0,  		//no need to align to specific boundaries multiple
			MmNonCachedUnordered);	//4
	if (!buffers_addr) 
	{
		debugPrint("Can't allocate DMA reception buffers\n");
		free(g_s);
		return 0;
	}

	//Write zeroes in first buffer and second buffer (descriptors)
	memset((void *)buffers_addr, 0, 4096); 

	buffers_physaddr=MmGetPhysicalAddress((void *)buffers_addr);

	g_s->PhysicalMinusVirtual = buffers_physaddr - buffers_addr; 

	g_s->RxBufferDesc = buffers_addr + 2048;
	g_s->RxBufferNext = buffers_addr + 2048;
	g_s->RxBufferTail = buffers_addr + 2048 + g_s->NbrRxBuffers * 8 - 8;

	g_s->TxBufferDesc = buffers_addr; 
	g_s->TxBufferLast = buffers_addr; 
	g_s->TxBufferNext = buffers_addr;
	g_s->TxBufferTail = buffers_addr + 8 * g_s->NbrTxBuffers - 8;

	p=buffers_addr + 4096 + 2 + g_s->PhysicalMinusVirtual; //Points 1st buffer at offset 2
	p2=buffers_addr + 2048;

	while(p2 <= g_s->RxBufferTail)	
	{
		*((DWORD *)p2)=p; //Physical address of offset 2 of buffer
		*((DWORD *)(p2+4))=NV_RX_AVAIL | 2045; //Makes all Rx buffers available
		//2046 bytes available for incoming packet at offset 2
		p2+=8;
		p+=2048;
	};

	//Buffers description :
	//1st buffer is a list of n pointers+flags (every 8 bytes) and is Tx ring descriptor
	//2nd buffer is a list of n pointers+flags (every 8 bytes) and is Rx ring descriptor
	//3rd buffer and following ones are pointed by the Rx ring pointers (at offset 2)
	//(n buffers used for packet receiving at startup while Tx ring is all zeroed)
	//Total : 1+1+n buffers
	//Descriptor is a list of 8 bytes values (a 32 bits physical address + a 32 bits flag)
	//The flag has the length (minus one) of available room/received packet/to send packet
	//in the lower 11 bits of the 32 bits flag

	len=0;
	ExQueryNonVolatileSetting(EEPROM_INDEX_MACADDR,&type,g_s->Ethaddr,6,&len);
	if (len!=6) 
		debugPrint("Can't read ethernet address from EEPROM\n");
#ifdef DISPLAYMSG
	else
		debugPrint("EEPROM MacAddress = %02x %02x %02x %02x %02x %02x\n",
				g_s->Ethaddr[0],
				g_s->Ethaddr[1],
				g_s->Ethaddr[2],
				g_s->Ethaddr[3],
				g_s->Ethaddr[4],
				g_s->Ethaddr[5]);
#endif

	g_s->Speed=NVREG_LINKSPEED_100MBPS;

	g_s->Ethaddr_reversed[0]=g_s->Ethaddr[5];
	g_s->Ethaddr_reversed[1]=g_s->Ethaddr[4];
	g_s->Ethaddr_reversed[2]=g_s->Ethaddr[3];
	g_s->Ethaddr_reversed[3]=g_s->Ethaddr[2];
	g_s->Ethaddr_reversed[4]=g_s->Ethaddr[1];
	g_s->Ethaddr_reversed[5]=g_s->Ethaddr[0];

	//Writing the MAC address (6 bytes ethernet address)
	REG(NvRegMacAddrA)=*((DWORD *)&g_s->Ethaddr_reversed[0]);
	REG(NvRegMacAddrB)=(ULONG)(*((WORD *)&g_s->Ethaddr_reversed[4]));

	REG(NvRegMulticastMaskA)=0xFFFFFFFF;
	REG(NvRegMulticastMaskB)=0x0000FFFF;
	REG(NvRegMulticastAddrA)=*((DWORD *)&g_s->Ethaddr[0]); //Yes, not reversed!
	REG(NvRegMulticastAddrB)=(ULONG)(*((WORD *)&g_s->Ethaddr[4]));

	RandomValue=0x1234+*((DWORD *)g_s->Ethaddr)+*((WORD *)&g_s->Ethaddr[4]);

	//In case of ethernet colision, random duration pauses are used before retry
	REG(NvRegRandomSeed) = (RandomValue & NVREG_RNDSEED_MASK) | NVREG_RNDSEED_FORCE;

	REG(NvRegOffloadConfig)=NVREG_OFFLOAD_NORMAL; //1518 bytes
	REG(NvRegPacketFilterFlags)=NVREG_PFF_ALWAYS_MYADDR;
	REG(NvRegDuplexMode)=NVREG_DUPLEX_MODE_FORCEH;

	REG(NvRegUnknownSetupReg1)=NVREG_UNKSETUP1_VAL;
	REG(NvRegUnknownSetupReg2)=NVREG_UNKSETUP2_VAL;

	//Writing the DMA buffers addresses and sizes
	REG(NvRegTxRingPhysAddr)= g_s->TxBufferDesc + g_s->PhysicalMinusVirtual; //1st buf phys
	REG(NvRegRxRingPhysAddr)= g_s->RxBufferDesc + g_s->PhysicalMinusVirtual; //2nd buf phys
	REG(NvRegRingSizes)= ((g_s->NbrRxBuffers-1)<<16) | (g_s->NbrTxBuffers-1);

	REG(NvRegUnknownSetupReg7)=NVREG_UNKSETUP7_VAL1;
	REG(NvRegUnknownSetupReg3)=NVREG_UNKSETUP7_VAL1;  //Yes, Val7 into Reg3!

	REG(NvRegAdapterControl) = (1 << NVREG_ADAPTCTL_PHYSHIFT) | NVREG_ADAPTCTL_PHYVALID;
	REG(NvRegMIISpeed)= NVREG_MIISPEED_BIT8 | NVREG_MIIDELAY;

	KeStallExecutionProcessor(50); //50 micro seconds of busy-wait

	g_running=1;

	//XBOX specific (nForce specific)
	if (PhyInitialize(0,0)!=0) //Initialize transceiver
	{
		debugPrint("PhyInitialize error\n");
		Pktdrv_Quit();
		return 0;
	}

	REG(NvRegAdapterControl)=NVREG_ADAPTCTL_RUNNING;
			
	KeStallExecutionProcessor(50); 

	PktdrvMiiInterrupt(1); //force display/update of current speed and duplex mode

	PktdrvStartSendRecv();

	REG(NvRegMIIStatus)=REG(NvRegMIIStatus);
	REG(NvRegIrqStatus)=REG(NvRegIrqStatus);

	REG(NvRegUnknownSetupReg4)=NVREG_UNKSETUP4_VAL;

	REG(NvRegIrqMask)=	NVREG_IRQ_LINK | 	
				NVREG_IRQ_TX_OK | 
				NVREG_IRQ_TX_ERROR |
				NVREG_IRQ_RX_NOBUF |
				NVREG_IRQ_RX |
				NVREG_IRQ_RX_ERROR;

	status=KeConnectInterrupt(&s_MyInterruptObject);
	status = 1; // KeConnectInterrupt is returning 0... but it seems to work
	
	if (status==0) 
	{
		debugPrint("KeConnectInterrupt error\n");
		Pktdrv_Quit();
		return 0;
	}
#ifdef DISPLAYMSG
	else
		debugPrint("Network interruption initialized successfully\n");
#endif
	return 1;
}


int Pktdrv_ReceivePackets(void)
{
	if (g_running) return PktdrvRecvInterrupt(); //returns nbr of packets accepted by callback
	return 0;
}

void Pktdrv_SendPacket(unsigned char *buffer,int length)
{
	if (g_running) 
	{
		PktdrvSendInterrupt();
		//if QueuedTxPkts>=n (n=number of outgoing buffers -ring- in calling app)
		//then packet will not be sent (app has to wait until QueuedTxPkts<n)
		PktdrvSendPacket(buffer,length);
	}
}


void Pktdrv_GetEthernetAddr(unsigned char *address)
{
	if ((address)&&(g_running))
		memcpy(address,g_s->Ethaddr,6);
}

int Pktdrv_GetQueuedTxPkts(void)
{
	if (g_running)
	{
		PktdrvSendInterrupt(); //detects any sent packet and updates QueuedTxPkts
		return g_s->QueuedTxPkts;
	}
	else
		return 0;
}
