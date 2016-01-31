/******************************************************************************/
/*																              */
/*  File: ohci.h                                                              */
/*                                                                            */
/*  Details: Original File & Design:  Thomas Frei                             */
/*  (modifications for xbox openxdk by bkenwright)                            */
/*  (bkenwright@xbdev.net)  www.xbdev.net                                     */
/*                                                                            */
/******************************************************************************/


#ifndef		__OHCI_X__
#define		__OHCI_X__

#include <stddef.h>

typedef		unsigned char	__u8;
typedef		char			__s8;
typedef		unsigned short	__u16;
typedef		short			__s16;
typedef		unsigned long	__u32;
typedef		int				__s32;


// Example sippet of possible later improvment on structure aligment in vs
// __declspec(align(32)) struct Str1
// {
//   int a, b, c, d, e;
// };

/*
typedef		unsigned char	__u8;
typedef		char			__s8;
typedef		unsigned short	__u16;
typedef		short			__s16;
typedef		unsigned long	__u32;
typedef		int				__s32;

#define     NULL  0
*/



#define SIZEOF_HCCA 0x100



/*
 * The HCCA (Host Controller Communications Area) is a 256 byte
 * structure defined in the OHCI spec. that the host controller is
 * told the base address of.  It must be 256-byte aligned.
 */

#define NUM_INTS 32     /* part of the OHCI standard */



#pragma pack( push, 1 )
struct ohci_hcca
{
        __u32   int_table[NUM_INTS];    /* Interrupt ED table */
        __u16   frame_no;               /* current frame number */
        __u16   pad1;                   /* set to 0 on each frame_no change */
        __u32   done_head;             /* info returned for an interrupt */
        __u8    reserved_for_hc[116];
};
#pragma pack( pop )
typedef struct ohci_hcca ohci_hcca_t;


/*
 * Maximum number of root hub ports.
 */

#define MAX_ROOT_PORTS 15


#pragma pack(1)
typedef volatile struct 
{
        /* control and status registers */
        __u32   revision;
        __u32   control;
        __u32   cmdstatus;
        __u32   intrstatus;
        __u32   intrenable;
        __u32   intrdisable;
        /* memory pointers */
        __u32   hcca;
        __u32   ed_periodcurrent;
        __u32   ed_controlhead;
        __u32   ed_controlcurrent;
        __u32   ed_bulkhead;
        __u32   ed_bulkcurrent;
        __u32   donehead;
        /* frame counters */
        __u32   fminterval;
		__u32   fmremaining;
		__u32   fmnumber;
        __u32   periodicstart;
        __u32   lsthresh;
        /* Root hub ports */
	volatile struct  ohci_roothub_regs 
	{
		__u32 a;
		__u32 b;
		__u32 status;
		__u32 portstatus[MAX_ROOT_PORTS];
	} roothub;
} ohci_regs_t;
#pragma pack()




#pragma pack(1)
typedef struct ohci 
{
        volatile ohci_hcca_t *hcca;         /* hcca */
		
        volatile ohci_regs_t * regs;        /* OHCI controller's memory */

} ohci_t;
#pragma pack()


/******************************************************************************/


#define SKIP   0x4000L
#define OUTV    0x0800L
#define INV     0x1000L
#define AUTOIO 0x1800L
#define LOWS   0x2000L



//#pragma block (push,1)
#pragma pack(1)
typedef struct {
__u32 Format;
__u32 Tailptr;
__u32 Headptr;
__u32 NextED;
} s_Endpointdescripor;
#pragma pack()

#pragma pack(1)
typedef struct {
__u32 Format;
__u32 Buffer;
__u32 NextTD;
__u32 BufferEnd;
} s_Transferdescriptor;
#pragma pack()

#pragma pack(1)
typedef struct {
__u8  Length;
__u8  DescriptorType;
__u16 USB;
__u8  DeviceClass;
__u8  DeviceSubClass;
__u8  DeviceProtocol;
__u8  MaxPacketSize;
__u16 Vendor;
__u16 ProductID;
__u16 Device;
__u8  Manufacturer;
__u8  ProductIndex;
__u8  SerialNumber;
__u8  ConfigNumber;
} s_USB_Devicedescriptor;
#pragma pack()

#pragma pack(1)
typedef struct {
__u8  Length;
__u8  DescriptorType;
__u16 TotalLength;
__u8  NumberofInterfaces;
__u8  ConfigValue;
__u8  Configuration;
__u8  Attributes;
__u8  MaxPower;
} s_USB_Configurationdescriptor;

typedef struct {
__u8  Length;
__u8  DescriptorType;
__u8  Interfacenumber;
__u8  AlternateSetting;
__u8  NumberofEndpoints;
__u8  InterfaceClass;
__u8  InterfaceSubClass;
__u8  InterfaceProtocol;
__u8  InterfaceIndex;
} s_USB_Interfacedescriptor;

typedef struct {
__u8  Length;
__u8  DescriptorType;
__u8  EndpointAddress;
__u8  Attributes;
__u16 MaxPacketSize;
__u8  Interval;
} s_USB_Endpointdescriptor;

typedef struct {
__u8  Length;
__u8  DescriptorType;
__u16 *LANGID; 
} s_USB_Languagedescriptor;

typedef struct {
__u8  Length;
__u8  DescriptorType;
__u8  *String;
} s_USB_Stringdescriptor;
//#pragma block (pop)
#pragma pack()


/******************************************************************************/

// Main Functions
int FindOHC(ohci_t * ohci, void *regbase);
int FindDev(ohci_t * ohci, int Port);


int ResetPort(ohci_t * ohci, int Port);
int SetAddres(ohci_t * ohci, int Port, __u8 AddrNew);
int SetConfigur(ohci_t * ohci, __u8 Addr, __u8 Config);
int GetDesc(ohci_t * ohci, __u8 Addr, __u8 DescrType, __u8 Index, __u8 Count, __u8 *DBuffer);



// Debug Functions
void DebugFile(ohci_t * ohci);
void DebugDescriptor( s_USB_Devicedescriptor * pDes );
void DebugConfigDescriptor( s_USB_Configurationdescriptor * pDes );
void DebugInterfaceDescriptor( s_USB_Interfacedescriptor * pDes );
void DebugEndPointDescriptor( s_USB_Endpointdescriptor * pDes );



#endif // __OHCI_X__
