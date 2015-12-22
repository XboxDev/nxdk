// ******************************************************************
// * 
// * proj : OpenXDK
// *
// * desc : Open Source XBox Development Kit
// *
// * file : xbox.h
// *
// * note : XBox Kernel Declarations
// *
// ******************************************************************
#ifndef XBOX_H
#define XBOX_H

#include "xboxkrnl.h"

// ******************************************************************
// * AvGetSavedDataAddress
// ******************************************************************
XBSYSAPI EXPORTNUM(1) PVOID NTAPI AvGetSavedDataAddress(void);

// ******************************************************************
// * AvSendTVEncoderOption
// ******************************************************************
XBSYSAPI EXPORTNUM(2) VOID NTAPI AvSendTVEncoderOption
(
	IN	PVOID	RegisterBase, 
	IN	ULONG	Option, 
	IN	ULONG	Param, 
	OUT	ULONG	*Result
);

// ******************************************************************
// * AvSetDisplayMode
// ******************************************************************
XBSYSAPI EXPORTNUM(4) ULONG NTAPI AvSetDisplayMode
(
	IN PVOID	RegisterBase,
	IN ULONG	Step,
	IN ULONG	Mode,
	IN ULONG	Format,
	IN ULONG	Pitch,
	IN ULONG	FrameBuffer
);

// ******************************************************************
// * AvSetSavedDataAddress
// ******************************************************************
XBSYSAPI EXPORTNUM(4) VOID NTAPI AvSetSavedDataAddress
(
	IN PVOID	Address
);

XBSYSAPI VOID *FscGetCacheSize;
XBSYSAPI VOID *FscInvalidateIdleBlocks;

// ******************************************************************
// * FscSetCacheSize
// ******************************************************************
XBSYSAPI EXPORTNUM(37) LONG NTAPI FscSetCacheSize(ULONG uCachePages);

XBSYSAPI VOID *KdDebuggerEnabled;
XBSYSAPI VOID *KdDebuggerNotPresent;
XBSYSAPI VOID *KfRaiseIrql;
XBSYSAPI VOID *KfLowerIrql;
XBSYSAPI VOID *KiBugCheckData;
XBSYSAPI VOID *KiUnlockDispatcherDatabase;

// ******************************************************************
// * LaunchDataPage (actually a pointer)
// ******************************************************************
XBSYSAPI EXPORTNUM(164) PLAUNCH_DATA_PAGE LaunchDataPage;

// ******************************************************************
// * PhyGetLinkState
// ******************************************************************
XBSYSAPI EXPORTNUM(252) ULONG NTAPI PhyGetLinkState
(
	IN ULONG	Mode
);

// ******************************************************************
// * PhyInitialize
// ******************************************************************
XBSYSAPI EXPORTNUM(253) ULONG NTAPI PhyInitialize
(
	IN ULONG	Parameter1,
	IN ULONG	Parameter2
);

// ******************************************************************
// * XboxEEPROMKey
// ******************************************************************
XBSYSAPI EXPORTNUM(321) UCHAR XboxEEPROMKey[16];

// ******************************************************************
// * XboxHardwareInfo
// ******************************************************************
XBSYSAPI EXPORTNUM(322) XBOX_HARDWARE_INFO XboxHardwareInfo;

// ******************************************************************
// * XboxHDKey
// ******************************************************************
XBSYSAPI EXPORTNUM(323) UCHAR XboxHDKey[16];

XBSYSAPI VOID *XboxKrnlVersion;

// ******************************************************************
// * XboxSignatureKey
// ******************************************************************
XBSYSAPI EXPORTNUM(325) BYTE XboxSignatureKey[16];

// ******************************************************************
// * XeImageFileName
// ******************************************************************

XBSYSAPI EXPORTNUM(326) ANSI_STRING XeImageFileName[1];

XBSYSAPI VOID *XeLoadSection;
XBSYSAPI VOID *XeUnloadSection;

// ******************************************************************
// * XcSHAInit
// ******************************************************************
XBSYSAPI EXPORTNUM(335) VOID NTAPI XcSHAInit(UCHAR *pbSHAContext);

// ******************************************************************
// * XcSHAUpdate
// ******************************************************************
XBSYSAPI EXPORTNUM(336) VOID NTAPI XcSHAUpdate(UCHAR *pbSHAContext, UCHAR *pbInput, ULONG dwInputLength);

// ******************************************************************
// * XcSHAFinal
// ******************************************************************
XBSYSAPI EXPORTNUM(337) VOID NTAPI XcSHAFinal(UCHAR *pbSHAContext, UCHAR *pbDigest);

XBSYSAPI VOID *XcRC4Key;
XBSYSAPI VOID *XcRC4Crypt;
XBSYSAPI VOID *XcHMAC;
XBSYSAPI VOID *XcPKEncPublic;
XBSYSAPI VOID *XcPKDecPrivate;
XBSYSAPI VOID *XcPKGetKeyLen;
XBSYSAPI VOID *XcVerifyPKCS1Signature;
XBSYSAPI VOID *XcModExp;
XBSYSAPI VOID *XcDESKeyParity;
XBSYSAPI VOID *XcKeyTable;
XBSYSAPI VOID *XcBlockCrypt;
XBSYSAPI VOID *XcBlockCryptCBC;
XBSYSAPI VOID *XcCryptService;
XBSYSAPI VOID *XcUpdateCrypto;
XBSYSAPI VOID *XboxLANKey;
XBSYSAPI VOID *XboxAlternateSignatureKeys;
XBSYSAPI EXPORTNUM(355) BYTE XePublicKeyData[276];
XBSYSAPI VOID *IdexChannelObject;

#endif


