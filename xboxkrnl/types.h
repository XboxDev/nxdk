#ifndef TYPES_H
#define TYPES_H

#ifndef HAVE_BOOLEAN
typedef int boolean;
#endif
#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif

#define true 1
#define false 0

#ifndef NULL
#define NULL 0
#endif

#define CONST const

typedef unsigned char BYTE;
typedef unsigned short WORD, WCHAR, *PWSTR;
typedef unsigned int DWORD, *PDWORD, *LPDWORD;

typedef unsigned int SIZE_T, *PSIZE_T;

typedef void VOID, *PVOID, *LPVOID;
typedef const void *PCVOID,*LPCVOID;

typedef BYTE* PBYTE;

typedef char CHAR, CCHAR, *PCHAR, *PSTR, *LPSTR, BOOL, *PBOOL, BOOLEAN, *PBOOLEAN, *LPCTSTR;
typedef const char *PCSZ, *PCSTR, *LPCSTR;
typedef unsigned char UCHAR;
typedef UCHAR* PUCHAR;

typedef unsigned short CSHORT, USHORT, *PUSHORT;

typedef unsigned int UINT;

typedef long LONG, *PLONG;
typedef unsigned long ULONG, *PULONG;
typedef PULONG ULONG_PTR; /* TODO: NOT SURE!! */


typedef long long           LONGLONG;
typedef unsigned long long  ULONGLONG;


typedef PVOID HANDLE;
typedef HANDLE* PHANDLE;
typedef ULONG PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;


#define OBJ_INHERIT            2L
#define OBJ_PERMANENT         16L
#define OBJ_EXCLUSIVE         32L
#define OBJ_CASE_INSENSITIVE  64L
#define OBJ_OPENIF           128L
#define OBJ_OPENLINK         256L
#define OBJ_VALID_ATTRIBUTES 498L

#define PAGE_READWRITE 0x4
#define PAGE_EXECUTE_READWRITE 0x40

#define NT_SUCCESS(Status)              ((NTSTATUS) (Status) >= 0)
#define STATUS_PENDING   0x00000103L
#define STATUS_NO_MEMORY 0xc0000017
#define MEM_RESERVE 0x00002000
#define MEM_COMMIT 0x00001000

#define ObDosDevices                    ((HANDLE) 0xFFFFFFFD)


#define STATUS_SUCCESS   0x00000000
#define STATUS_ALERTED                   ((DWORD   )0x00000101L)
#define STATUS_INVALID_PARAMETER         ((DWORD   )0xC000000DL)
#define STATUS_USER_APC                  ((DWORD   )0x000000C0L)
// The SCSI input buffer was too large (not necessarily an error!)
#define STATUS_DATA_OVERRUN              ((DWORD   )0xC000003CL)
#define STATUS_INVALID_IMAGE_FORMAT      ((DWORD   )0xC000007BL)
#define STATUS_INSUFFICIENT_RESOURCES    ((DWORD   )0xC000009AL)
#define STATUS_TOO_MANY_SECRETS          ((DWORD   )0xC0000156L)
#define STATUS_XBE_REGION_MISMATCH       ((DWORD   )0xC0050001L)
#define STATUS_XBE_MEDIA_MISMATCH        ((DWORD   )0xC0050002L)
#define STATUS_OBJECT_NAME_NOT_FOUND     ((DWORD   )0xC0000034L)
#define STATUS_OBJECT_NAME_COLLISION     ((DWORD   )0xC0000035L)

// CreateOption values for NtCreateFile()
// FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT is what CreateFile
// uses for most things when translating to NtCreateFile.
#define FILE_LIST_DIRECTORY                     0x00000001
#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008
#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080
#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
#define FILE_OPEN_FOR_RECOVERY                  0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800
#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000
#define FILE_RESERVE_OPFILTER                   0x00100000
#define FILE_OPEN_REPARSE_POINT                 0x00200000
#define FILE_OPEN_NO_RECALL                     0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x00800000
#define FILE_COPY_STRUCTURED_STORAGE            0x00000041
#define FILE_STRUCTURED_STORAGE                 0x00000441
#define FILE_VALID_OPTION_FLAGS                 0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS            0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS        0x00000032
#define FILE_VALID_SET_FLAGS                    0x00000036

#define FILE_SUPERSEDE                          0x00000000
#define FILE_OPEN                               0x00000001
#define FILE_CREATE                             0x00000002
#define FILE_OPEN_IF                            0x00000003
#define FILE_OVERWRITE                          0x00000004
#define FILE_OVERWRITE_IF                       0x00000005
#define FILE_MAXIMUM_DISPOSITION                0x00000005

// NtCreateFile/NtOpenFile stuff
#define FILE_SUPERSEDED                         0x00000000
#define FILE_OPENED                             0x00000001
#define FILE_CREATED                            0x00000002
#define FILE_OVERWRITTEN                        0x00000003
#define FILE_EXISTS                             0x00000004
#define FILE_DOES_NOT_EXIST                     0x00000005

// ******************************************************************
// * STRING
// ******************************************************************
typedef struct _ANSI_STRING 
{
	USHORT Length;
	USHORT MaximumLength;
	PSTR   Buffer;
} ANSI_STRING, *PANSI_STRING, STRING, *PSTRING;

// ******************************************************************
// * UNICODE_STRING
// ******************************************************************
typedef struct _UNICODE_STRING
{
	USHORT  Length;
	USHORT  MaximumLength;
	PWSTR   Buffer;
}
UNICODE_STRING, *PUNICODE_STRING;

// ******************************************************************
// * LARGE_INTEGER
// ******************************************************************
typedef union _LARGE_INTEGER
{
	struct
	{
		DWORD   LowPart;
		LONG    HighPart;
	} u;
	LONGLONG QuadPart;
}
LARGE_INTEGER, *PLARGE_INTEGER;

// ******************************************************************
// * ULARGE_INTEGER
// ******************************************************************
typedef union _ULARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	} u1;
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ULONGLONG QuadPart;
}
ULARGE_INTEGER, *PULARGE_INTEGER;

typedef ULONG ACCESS_MASK;

typedef LONG NTSTATUS;

#define OPTIONAL

typedef struct _OBJECT_ATTRIBUTES
{
        HANDLE RootDirectory;
        PANSI_STRING ObjectName;
        ULONG Attributes;
} 
OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _SECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} 
SECURITY_ATTRIBUTES,*PSECURITY_ATTRIBUTES,*LPSECURITY_ATTRIBUTES;

typedef struct _OVERLAPPED {
	ULONG_PTR		Internal;
	ULONG_PTR		InternalHigh;
	DWORD		Offset;
	DWORD		OffsetHigh;
	HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef struct _IO_STATUS_BLOCK
{
    union
    {
        NTSTATUS Status;
        PVOID    Pointer;
    }
    u1;

    ULONG_PTR Information;
} 
IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;


// Structure of an RC4 key
typedef struct _RC4_SBOX {
// Opaque, but copiable, structure
BYTE Data[256 + 4];
} RC4_SBOX, *PRC4_SBOX;

// ******************************************************************
// * RETURN_FIRMWARE
// ******************************************************************
typedef enum _RETURN_FIRMWARE
{
	ReturnFirmwareHalt          = 0x00,
	ReturnFirmwareReboot        = 0x01,
	ReturnFirmwareQuickReboot   = 0x02,
	ReturnFirmwareHard          = 0x03,
	ReturnFirmwareFatal         = 0x04,
	ReturnFirmwareAll           = 0x05
}
RETURN_FIRMWARE, *LPRETURN_FIRMWARE;

// ******************************************************************
// * KOBJECTS
// ******************************************************************
typedef enum _KOBJECTS
{
    DpcObject = 0x13,
}
KOBJECTS, *PKOBJECTS;

// ******************************************************************
// * KPROCESSOR_MODE
// ******************************************************************
typedef CCHAR KPROCESSOR_MODE;

// ******************************************************************
// * LIST_ENTRY
// ******************************************************************
typedef struct _LIST_ENTRY
{
	struct _LIST_ENTRY *Flink;
	struct _LIST_ENTRY *Blink;
}
LIST_ENTRY, *PLIST_ENTRY;

// ******************************************************************
// * DISPATCHER_HEADER
// ******************************************************************
typedef struct _DISPATCHER_HEADER
{
	UCHAR       Type;           // 0x00
	UCHAR       Absolute;       // 0x01
	UCHAR       Size;           // 0x02
	UCHAR       Inserted;       // 0x03
	LONG        SignalState;    // 0x04
	LIST_ENTRY  WaitListHead;   // 0x08
}
DISPATCHER_HEADER;

// ******************************************************************
// * TIMER_TYPE
// ******************************************************************
typedef enum _TIMER_TYPE
{
	NotificationTimer     = 0,
	SynchronizationTimer  = 1
}
TIMER_TYPE;

// ******************************************************************
// * KTIMER (Timer Object)
// ******************************************************************
typedef struct _KTIMER
{
	DISPATCHER_HEADER   Header;           // 0x00
	ULARGE_INTEGER      DueTime;          // 0x10
	LIST_ENTRY          TimerListEntry;   // 0x18
	struct _KDPC       *Dpc;              // 0x20
	LONG                Period;           // 0x24
}
KTIMER, *PKTIMER;

// ******************************************************************
// * KDPC (Deferred Procedure Call (DPC) Object)
// ******************************************************************
typedef struct _KDPC
{
	CSHORT              Type;               // 0x00
	UCHAR               Number;             // 0x02
	UCHAR               Importance;         // 0x03
	LIST_ENTRY          DpcListEntry;       // 0x04
	PVOID               DeferredRoutine;    // 0x0C
	PVOID               DeferredContext;	// 0x10
	PVOID               SystemArgument1;	// 0x14
	PVOID               SystemArgument2;	// 0x18
}
KDPC, *PKDPC;

// ******************************************************************
// * PKDEFERRED_ROUTINE
// ******************************************************************
typedef VOID __stdcall (*PKDEFERRED_ROUTINE) (PKDPC Dpc, 
						PVOID DeferredContext, 
						PVOID SystemArgument1,
						PVOID SystemArgument2);

// ******************************************************************
// * KINTERRUPT
// ******************************************************************
// *
// * NOTE: INCOMPLETE!!
// *
// ******************************************************************
typedef struct _KINTERRUPT
{
	UCHAR             UnknownA[0x0C];
	UCHAR             KIRQL;
	UCHAR             PaddingA[0x03];
	UCHAR             UnknownB[0x08];
	UCHAR             ISR[0x58];
}
KINTERRUPT, *PKINTERRUPT;

// ******************************************************************
// * PKSERVICE_ROUTINE
// ******************************************************************
typedef BOOLEAN __stdcall (*PKSERVICE_ROUTINE) (PKINTERRUPT Interrupt, PVOID ServiceContext);

// ******************************************************************
// * KIRQL
// ******************************************************************
typedef UCHAR KIRQL, *PKIRQL;

// ******************************************************************
// * KINTERRUPT_MODE
// ******************************************************************
typedef enum _KINTERRUPT_MODE
{
	LevelSensitive,
	Latched,
}
KINTERRUPT_MODE;

// ******************************************************************
// * KTHREAD
// ******************************************************************
// *
// * NOTE: INCOMPLETE!!
// *
// ******************************************************************
typedef struct _KTHREAD
{
	UCHAR           UnknownA[0x28];
	PVOID           TlsData;        // 0x28
	UCHAR           UnknownB[0xE4]; // 0x2C
}
KTHREAD, *PKTHREAD;

// ******************************************************************
// * MM_STATISTICS
// ******************************************************************
typedef struct _MM_STATISTICS
{
	ULONG   Length;
	ULONG   TotalPhysicalPages;
	ULONG   AvailablePages;
	ULONG   VirtualMemoryBytesCommitted;
	ULONG   VirtualMemoryBytesReserved;
	ULONG   CachePagesCommitted;
	ULONG   PoolPagesCommitted;
	ULONG   StackPagesCommitted;
	ULONG   ImagePagesCommitted;
}
MM_STATISTICS, *PMM_STATISTICS;

// ******************************************************************
// * EVENT_TYPE
// ******************************************************************
typedef enum _EVENT_TYPE
{
	NotificationEvent = 0,
	SynchronizationEvent
}
EVENT_TYPE;


// ******************************************************************
// * FSINFOCLASS
// ******************************************************************
typedef enum _FSINFOCLASS
{
	FileFsVolumeInformation       = 1,
	FileFsLabelInformation,      // 2
	FileFsSizeInformation,       // 3
	FileFsDeviceInformation,     // 4
	FileFsAttributeInformation,  // 5
	FileFsControlInformation,    // 6
	FileFsFullSizeInformation,   // 7
	FileFsObjectIdInformation,   // 8
	FileFsMaximumInformation
}
FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

// ******************************************************************
// * FILE_INFORMATION_CLASS
// ******************************************************************
typedef enum _FILE_INFORMATION_CLASS
{
	FileDirectoryInformation        = 1,
	FileFullDirectoryInformation,
	FileBothDirectoryInformation,
	FileBasicInformation,
	FileStandardInformation,
	FileInternalInformation,
	FileEaInformation,
	FileAccessInformation,
	FileNameInformation,
	FileRenameInformation,
	FileLinkInformation,
	FileNamesInformation,
	FileDispositionInformation,
	FilePositionInformation,
	FileFullEaInformation,
	FileModeInformation,
	FileAlignmentInformation,
	FileAllInformation,
	FileAllocationInformation,
	FileEndOfFileInformation,
	FileAlternateNameInformation,
	FileStreamInformation,
	FilePipeInformation,
	FilePipeLocalInformation,
	FilePipeRemoteInformation,
	FileMailslotQueryInformation,
	FileMailslotSetInformation,
	FileCompressionInformation,
	FileCopyOnWriteInformation,
	FileCompletionInformation,
	FileMoveClusterInformation,
	FileQuotaInformation,
	FileReparsePointInformation,
	FileNetworkOpenInformation,
	FileObjectIdInformation,
	FileTrackingInformation,
	FileOleDirectoryInformation,
	FileContentIndexInformation,
	FileInheritContentIndexInformation,
	FileOleInformation,
	FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef struct _FILE_POSITION_INFORMATION 
{
	LARGE_INTEGER   CurrentByteOffset;
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;

// ******************************************************************
// * FILE_NETWORK_OPEN_INFORMATION
// ******************************************************************
typedef struct _FILE_NETWORK_OPEN_INFORMATION 
{
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER AllocationSize;
	LARGE_INTEGER EndOfFile;
	ULONG FileAttributes;
	ULONG FixLinuxGccDummy;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

// ******************************************************************
// * FILE_DIRECTORY_INFORMATION
// ******************************************************************
typedef struct _FILE_DIRECTORY_INFORMATION 
{
	ULONG           NextEntryOffset;
	ULONG           FileIndex;
	LARGE_INTEGER   CreationTime;
	LARGE_INTEGER   LastAccessTime;
	LARGE_INTEGER   LastWriteTime;
	LARGE_INTEGER   ChangeTime;
	LARGE_INTEGER   EndOfFile;
	LARGE_INTEGER   AllocationSize;
	ULONG           FileAttributes;
	ULONG           FileNameLength;
	CHAR            FileName[0x100];        // Offset: 0x40
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

// ******************************************************************
// * FILE_STANDARD_INFORMATION
// ******************************************************************
typedef struct _FILE_STANDARD_INFORMATION
{
	LARGE_INTEGER  AllocationSize;
	LARGE_INTEGER  EndOfFile;
	ULONG  NumberOfLinks;
	BOOLEAN  DeletePending;
	BOOLEAN  Directory;
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;

// ******************************************************************
// * FILE_RENAME_INFORMATION and FILE_LINK_RENAME_INFORMATION
// ******************************************************************
typedef struct _FILE_LINK_RENAME_INFORMATION 
{
	BOOLEAN ReplaceIfExists;
	HANDLE RootDirectory;
	ANSI_STRING FileName;
} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION, FILE_LINK_INFORMATION, *PFILE_LINK_INFORMATION;

// ******************************************************************
// * FILE_DISPOSITION_INFORMATION
// ******************************************************************
typedef struct _FILE_DISPOSITION_INFORMATION 
{
	BOOLEAN DeleteFile;
} FILE_DISPOSITION_INFORMATION, *PFILE_DISPOSITION_INFORMATION;

// ******************************************************************
// * FILE_FS_SIZE_INFORMATION
// ******************************************************************
typedef struct _FILE_FS_SIZE_INFORMATION
{
	LARGE_INTEGER   TotalAllocationUnits;
	LARGE_INTEGER   AvailableAllocationUnits;
	ULONG           SectorsPerAllocationUnit;
	ULONG           BytesPerSector;
} FILE_FS_SIZE_INFORMATION, *PFILE_FS_SIZE_INFORMATION;

// ******************************************************************
// * MEMORY_BASIC_INFORMATION
// ******************************************************************
typedef struct _MEMORY_BASIC_INFORMATION
{
	PVOID BaseAddress;            // base address of region 
	PVOID AllocationBase;         // allocation base address 
	DWORD AllocationProtect;      // initial access protection 
	DWORD RegionSize;             // size, in bytes, of region 
	DWORD State;                  // committed, reserved, free 
	DWORD Protect;                // current access protection 
	DWORD Type;                   // type of pages 
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION; 

// ******************************************************************
// * WAIT_TYPE
// ******************************************************************
typedef enum _WAIT_TYPE
{
	WaitAll = 0,
	WaitAny = 1
}
WAIT_TYPE;

// ******************************************************************
// * PKSTART_ROUTINE
// ******************************************************************
// *
// * NOTE: Non-standard call. Similar to stdcall, but first argument
// *       must be located at ebp+4 before calling.
// *
// *       This is different from the NT version: 2 parameters as
// *       opposed to 1.
// *
// ******************************************************************
// * Hmmm... this is supposed to be like the following line, but GCC
// * complains.  Not sure why...
// * typedef VOID (NTAPI *PKSTART_ROUTINE) (PVOID StartContext1, PVOID StartContext2);
// ******************************************************************
typedef VOID (*PKSTART_ROUTINE) (PVOID StartContext1, PVOID StartContext2);

// ******************************************************************
// * RTL_CRITICAL_SECTION
// ******************************************************************
typedef struct _RTL_CRITICAL_SECTION
{
	DWORD               Unknown[4];        // 0x00
	LONG                LockCount;         // 0x10
	LONG                RecursionCount;    // 0x14
	ULONG               OwningThread;      // 0x18
}
RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

// ******************************************************************
// * TIME_FIELDS
// ******************************************************************
typedef struct _TIME_FIELDS
{
	USHORT  Year;
	USHORT  Month;
	USHORT  Day;
	USHORT  Hour;
	USHORT  Minute;
	USHORT  Second;
	USHORT  Millisecond;
	USHORT  Weekday;
}
TIME_FIELDS, *PTIME_FIELDS;

// ******************************************************************
// * LAUNCH_DATA_HEADER
// ******************************************************************
typedef struct _LAUNCH_DATA_HEADER
{
	DWORD   dwLaunchDataType;
	DWORD   dwTitleId;
	char    szLaunchPath[520];
	DWORD   dwFlags;
}
LAUNCH_DATA_HEADER, *PLAUNCH_DATA_HEADER;

// ******************************************************************
// * LAUNCH_DATA_PAGE
// ******************************************************************
typedef struct _LAUNCH_DATA_PAGE
{
	LAUNCH_DATA_HEADER  Header;
	UCHAR               Pad[492];
	UCHAR               LaunchData[3072];
}
LAUNCH_DATA_PAGE, *PLAUNCH_DATA_PAGE;

// ******************************************************************
// * XBOX_HARDWARE_INFO
// ******************************************************************
typedef struct _XBOX_HARDWARE_INFO
{
	ULONG Flags;
	UCHAR Unknown1;
	UCHAR Unknown2;
	UCHAR Unknown3;
	UCHAR Unknown4;
}
XBOX_HARDWARE_INFO;

// ******************************************************************
// * NT_TIB
// ******************************************************************
typedef struct _NT_TIB
{
    struct _EXCEPTION_REGISTRATION_RECORD  *ExceptionList;          // 0x00
    PVOID                                   StackBase;              // 0x04
    PVOID                                   StackLimit;             // 0x08
    PVOID                                   SubSystemTib;           // 0x0C
    union
    {
        PVOID FiberData;                                            // 0x10 for TIB
        ULONG Version;                                              // 0x10 for TEB (?)
    }
    u_a;
    PVOID                                   ArbitraryUserPointer;   // 0x14
    struct _NT_TIB                         *Self;                   // 0x18
}
NT_TIB, *PNT_TIB;

// ******************************************************************
// * ETHREAD
// ******************************************************************
// *
// * NOTE: INCOMPLETE!!
// *
// ******************************************************************
typedef struct _ETHREAD
{
    struct _KTHREAD Tcb;
    UCHAR           UnknownA[0x1C]; // 0x110
    DWORD           UniqueThread;   // 0x12C
}
ETHREAD, *PETHREAD;

// ******************************************************************
// * KPCRB
// ******************************************************************
// *
// * NOTE: INCOMPLETE!!
// *
// ******************************************************************
typedef struct _KPRCB
{
    struct _KTHREAD* CurrentThread;                                 // 0x00, KPCR : 0x28
    struct _KTHREAD* NextThread;                                    // 0x04, KPCR : 0x2C
    struct _KTHREAD* IdleThread;                                    // 0x08, KPCR : 0x30

    // This is the total size of the structure (presumably)
    UCHAR            Unknown[0x250];                                // 0x0C, KPCR : 0x34
}
KPRCB, *PKPRCB;

// ******************************************************************
// * KPCR
// ******************************************************************
// *
// * NOTE: KPCR is the structure which exists at the FS: segment.
// *
// ******************************************************************
typedef struct _KPCR
{
    struct _NT_TIB  NtTib;                                          // 0x00
    struct _KPCR   *SelfPcr;                                        // 0x1C
    struct _KPRCB  *Prcb;                                           // 0x20
    UCHAR           Irql;                                           // 0x24
    struct _KPRCB   PrcbData;                                       // 0x28
}
KPCR, *PKPCR;


// ******************************************************************
// * EEPROM_INDEX
// ******************************************************************
typedef enum _EEPROM_INDEX
{
    EEPROM_MISC = 0x11
}
EEPROM_INDEX, *PEEPROM_INDEX;

#endif
