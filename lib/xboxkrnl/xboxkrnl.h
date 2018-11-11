/**
 * @file xboxkrnl.h
 * @author Stefan Schmidt
 * @brief The complete interface to the Xbox-Kernel.
 * This specific file is licensed under the CC0 1.0.
 * Look here for details: https://creativecommons.org/publicdomain/zero/1.0/
 */

#pragma once

/* stop clang from crying */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wlanguage-extension-token"

/* MSVC-compatibility for structure-packing */
#pragma ms_struct on

#if defined(__cplusplus)
extern "C"
{
#endif

#define IN
#define OUT
#define UNALIGNED
#define OPTIONAL
#define XBAPI __declspec(dllimport)
#define NTAPI __attribute__((__stdcall__))
#define CDECL __attribute__((__cdecl__))
#define FASTCALL __attribute__((fastcall))
#define DECLSPEC_NORETURN __attribute__((noreturn))
#define RESTRICTED_POINTER __restrict__

#define CONST const

typedef unsigned int SIZE_T, *PSIZE_T;
typedef unsigned char BOOLEAN, *PBOOLEAN;
typedef int BOOL, *PBOOL;
typedef void VOID, *PVOID, *LPVOID;
typedef unsigned char UCHAR, *PUCHAR;
typedef unsigned short USHORT, *PUSHORT, CSHORT;
typedef unsigned long ULONG, *PULONG;
typedef unsigned long long ULONGLONG;

#ifndef NULL
    #define NULL ((PVOID)0)
#endif
#ifndef TRUE
    #define TRUE 1
#endif
#ifndef FALSE
    #define FALSE 0
#endif

typedef char CHAR, *PCHAR, CCHAR, *LPCH, *PCH, OCHAR, *POCHAR;
typedef short SHORT, *PSHORT;
typedef long LONG, *PLONG;
typedef long long LONGLONG, *PLONGLONG;

typedef signed char SCHAR, *PSCHAR;

typedef LONG NTSTATUS;
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define STATUS_INVALID_PARAMETER ((NTSTATUS)0xC000000DL)
#define STATUS_INVALID_HANDLE ((NTSTATUS)0xC0000008L)
#define STATUS_HANDLE_NOT_CLOSABLE ((NTSTATUS)0xC0000235L)

#define WAIT_FAILED ((NTSTATUS)0xFFFFFFFFL)
#define STATUS_WAIT_0 ((NTSTATUS)0x00000000L)
#define STATUS_ABANDONED_WAIT_0 ((NTSTATUS)0x00000080L)
#define STATUS_USER_APC ((NTSTATUS)0x000000C0L)
#define STATUS_ALERTED ((NTSTATUS)0x00000101L)
#define STATUS_TIMEOUT ((NTSTATUS)0x00000102L)
#define STATUS_PENDING ((NTSTATUS)0x00000103L)
#define STATUS_SEGMENT_NOTIFICATION ((NTSTATUS)0x40000005L)
#define STATUS_GUARD_PAGE_VIOLATION ((NTSTATUS)0x80000001L)
#define STATUS_DATATYPE_MISALIGNMENT ((NTSTATUS)0x80000002L)
#define STATUS_BREAKPOINT ((NTSTATUS)0x80000003L)
#define STATUS_SINGLE_STEP ((NTSTATUS)0x80000004L)
#define STATUS_ACCESS_VIOLATION ((NTSTATUS)0xC0000005L)
#define STATUS_IN_PAGE_ERROR ((NTSTATUS)0xC0000006L)
#define STATUS_NO_MEMORY ((NTSTATUS)0xC0000017L)
#define STATUS_ILLEGAL_INSTRUCTION ((NTSTATUS)0xC000001DL)
#define STATUS_NONCONTINUABLE_EXCEPTION ((NTSTATUS)0xC0000025L)
#define STATUS_INVALID_DISPOSITION ((NTSTATUS)0xC0000026L)
#define STATUS_OBJECT_NAME_INVALID ((NTSTATUS)0xC0000033L)
#define STATUS_OBJECT_NAME_NOT_FOUND ((NTSTATUS)0xC0000034L)
#define STATUS_OBJECT_NAME_COLLISION ((NTSTATUS)0xC0000035L)
#define STATUS_ARRAY_BOUNDS_EXCEEDED ((NTSTATUS)0xC000008CL)
#define STATUS_FLOAT_DENORMAL_OPERAND ((NTSTATUS)0xC000008DL)
#define STATUS_FLOAT_DIVIDE_BY_ZERO ((NTSTATUS)0xC000008EL)
#define STATUS_FLOAT_INEXACT_RESULT ((NTSTATUS)0xC000008FL)
#define STATUS_FLOAT_INVALID_OPERATION ((NTSTATUS)0xC0000090L)
#define STATUS_FLOAT_OVERFLOW ((NTSTATUS)0xC0000091L)
#define STATUS_FLOAT_STACK_CHECK ((NTSTATUS)0xC0000092L)
#define STATUS_FLOAT_UNDERFLOW ((NTSTATUS)0xC0000093L)
#define STATUS_INTEGER_DIVIDE_BY_ZERO ((NTSTATUS)0xC0000094L)
#define STATUS_INTEGER_OVERFLOW ((NTSTATUS)0xC0000095L)
#define STATUS_PRIVILEGED_INSTRUCTION ((NTSTATUS)0xC0000096L)
#define STATUS_DATA_OVERRUN ((NTSTATUS)0xC000003CL)
#define STATUS_INVALID_IMAGE_FORMAT ((NTSTATUS)0xC000007BL)
#define STATUS_INSUFFICIENT_RESOURCES ((NTSTATUS)0xC000009AL)
#define STATUS_STACK_OVERFLOW ((NTSTATUS)0xC00000FDL)
#define STATUS_CONTROL_C_EXIT ((NTSTATUS)0xC000013AL)
#define STATUS_TOO_MANY_SECRETS ((NTSTATUS)0xC0000156L)
#define STATUS_IMAGE_GAME_REGION_VIOLATION ((NTSTATUS)0xC0050001L)
#define STATUS_IMAGE_MEDIA_TYPE_VIOLATION ((NTSTATUS)0xC0050002L)

// Used in AvSendTVEncoderOption
#define AV_PACK_NONE 0x00000000
#define AV_PACK_STANDARD 0x00000001
#define AV_PACK_RFU 0x00000002
#define AV_PACK_SCART 0x00000003
#define AV_PACK_HDTV 0x00000004
#define AV_PACK_VGA 0x00000005
#define AV_PACK_SVIDEO 0x00000006

typedef PVOID HANDLE, *PHANDLE;
typedef ULONG PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;
typedef UCHAR KIRQL, *PKIRQL;
typedef ULONG ULONG_PTR;
typedef LONG LONG_PTR;
typedef ULONG PFN_COUNT;
typedef ULONG PFN_NUMBER, *PPFN_NUMBER;
typedef LONG KPRIORITY;
typedef ULONG DEVICE_TYPE;
typedef ULONG LOGICAL;

typedef unsigned char BYTE;
typedef unsigned short WORD, WCHAR, *PWSTR;
typedef unsigned int DWORD, *PDWORD, *LPDWORD;
typedef const char *PCSZ, *PCSTR, *LPCSTR;
typedef char *PSZ, *PSTR;
typedef CONST WCHAR *LPCWSTR, *PCWSTR;

typedef ULONG ACCESS_MASK, *PACCESS_MASK;

typedef CCHAR KPROCESSOR_MODE;
typedef enum _MODE {
    KernelMode,
    UserMode,
    MaximumMode
} MODE;

/**
 * This struct defines a counted string used for ANSI-strings
 */
typedef struct _ANSI_STRING
{
    USHORT Length;        /**< Length in bytes of the string stored in the buffer */
    USHORT MaximumLength; /**< Length in bytes of the buffer */
    PSTR Buffer;          /**< Pointer to the buffer used for the character-string */
} ANSI_STRING, *PANSI_STRING, STRING, *PSTRING, OBJECT_STRING, *POBJECT_STRING;

/**
 * This struct defines a counted string used for UNICODE-strings
 */
typedef struct _UNICODE_STRING
{
    USHORT Length;        /**< Length in bytes of the string stored in the buffer */
    USHORT MaximumLength; /**< Length in bytes of the buffer */
    PWSTR Buffer;         /**< Pointer to the buffer used for the character-string */
} UNICODE_STRING, *PUNICODE_STRING;

typedef CONST UNICODE_STRING *PCUNICODE_STRING;

/**
 * Header or descriptor for an entry in a doubly linked list.
 * Initialized by InitializeListHead, members shouldn't be updated manually.
 */
typedef struct _LIST_ENTRY
{
	struct _LIST_ENTRY *Flink; /**< Points to the next entry of the list or the header if there is no next entry */
	struct _LIST_ENTRY *Blink; /**< Points to the previous entry of the list or the header if there is no previous entry */
} LIST_ENTRY, *PLIST_ENTRY;

/*
    VOID InitializeListHead (
        PLIST_ENTRY ListHead
    );
*/
#define InitializeListHead (ListHead) ((Listhead)->Flink = (ListHead)->Blink = (ListHead))

#define IsListEmpty (ListHead) ((ListHead)->Flink == (ListHead))

#define RemoveHeadList (Listhead) (ListHead)->Flink;{RemoveEntryList((ListHead)->Flink)}

#define RemoveTailList (ListHead) (ListHead)->Blink;{RemoveEntryList((ListHead)->Blink)}

#define RemoveEntryList (Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_Flink;\
    _EX_Flink = (Entry)->Flink;\
    _EX_Blink = (Entry)->Blink;\
    _EX_Blink->Flink = _EX_Flink;\
    _EX_Flink->Blink = _EX_Blink;\
}

#define InsertTailList (ListHead, Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
}

#define InsertHeadList (ListHead, Entry) {\
    PLIST_ENTRY _EX_Flink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Flink = _EX_ListHead->Flink;\
    (Entry)->Flink = _EX_Flink;\
    (Entry)->Blink = _EX_ListHead;\
    _EX_Flink->Blink = (Entry);\
    _EX_ListHead->Flink = (Entry);\
}

#define PopEntryList (ListHead) \
    (ListHead)->Next;\
    {\
        PSINGLE_LIST_ENTRY FirstEntry;\
        FirstEntry = (ListHead)->Next;\
        if (FirstEntry != NULL) {\
            (ListHead)->Next = FirstEntry->Next;\
        }\
    }

#define PushEntryList (ListHead, Entry) \
    (Entry)->Next = (ListHead)->Next; \
    (ListHead)->Next = (Entry);

/**
 * Struct for modelling critical sections in the XBOX-kernel
 */
typedef struct _RTL_CRITICAL_SECTION
{
    union {
        struct {
            UCHAR Type;
            UCHAR Absolute;
            UCHAR Size;
            UCHAR Inserted;
            LONG SignalState;
            LIST_ENTRY WaitListHead;
        } Event;
        ULONG RawEvent[4];
    } Synchronization;

    LONG LockCount;
    LONG RecursionCount;
    PVOID OwningThread;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

/**
 * MS's way to represent a 64-bit signed int on platforms that may not support
 * them directly.
 */
typedef union _LARGE_INTEGER
{
    struct
    {
        ULONG LowPart; /**< The low-order 32 bits. */
        LONG HighPart; /**< The high-order 32 bits. */
    };
    struct
    {
        ULONG LowPart; /**< The low-order 32 bits. */
        LONG HighPart; /**< The high-order 32 bits. */
    } u;
    LONGLONG QuadPart; /**< A signed 64-bit integer. */
} LARGE_INTEGER, *PLARGE_INTEGER;

/**
 * MS's way to represent a 64-bit unsigned int on platforms that may not support
 * them directly.
 */
typedef union _ULARGE_INTEGER
{
	struct
	{
		ULONG LowPart; /**< The low-order 32 bits. */
		ULONG HighPart; /**< The high-order 32 bits. */ /**< The high-order 32 bits. */
	};
	struct
	{
		ULONG LowPart; /**< The low-order 32 bits. */
		ULONG HighPart; /**< The high-order 32 bits. */
	} u;
	ULONGLONG QuadPart; /**< An unsigned 64-bit integer. */
} ULARGE_INTEGER, *PULARGE_INTEGER;

/**
 * Time information
 */
typedef struct _TIME_FIELDS
{
    SHORT Year; /**< Specifies a value from 1601 on */
    SHORT Month; /**< Specifies a value from 1 to 12 */
    SHORT Day; /**< Specifies a value from 1 to 31 */
    SHORT Hour; /**< Specifies a value from 0 to 23 */
    SHORT Minute; /**< Specifies a value from 0 to 59 */
    SHORT Second; /**< Specifies a value from 0 to 59 */
    SHORT Millisecond; /**< Specifies a value from 0 to 999 */
    SHORT Weekday; /**< Specifies a value from 0 to 6 (Sunday to Saturday) */
} TIME_FIELDS, *PTIME_FIELDS;

typedef struct _OVERLAPPED
{
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    DWORD Offset;
    DWORD OffsetHigh;
    HANDLE hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef enum _WAIT_TYPE
{
    WaitAll = 0,
    WaitAny = 1
} WAIT_TYPE;

typedef struct _IO_STATUS_BLOCK
{
    union
    {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef enum _FSINFOCLASS
{
    FileFsVolumeInformation = 1,
    FileFsLabelInformation,
    FileFsSizeInformation,
    FileFsDeviceInformation,
    FileFsAttributeInformation,
    FileFsControlInformation,
    FileFsFullSizeInformation,
    FileFsObjectIdInformation,
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

typedef struct _FILE_FS_LABEL_INFORMATION
{
    ULONG VolumeLabelLength;
    OCHAR VolumeLabel[1];
} FILE_FS_LABEL_INFORMATION, *PFILE_FS_LABEL_INFORMATION;

typedef struct _FILE_FS_VOLUME_INFORMATION
{
    LARGE_INTEGER VolumeCreationTime;
    ULONG VolumeSerialNumber;
    ULONG VolumeLabelLength;
    BOOLEAN SupportsObjects;
    OCHAR VolumeLabel[1];
} FILE_FS_VOLUME_INFORMATION, *PFILE_FS_VOLUME_INFORMATION;

typedef struct _FILE_FS_SIZE_INFORMATION
{
    LARGE_INTEGER TotalAllocationUnits;
    LARGE_INTEGER AvailableAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_SIZE_INFORMATION, *PFILE_FS_SIZE_INFORMATION;

typedef struct _FILE_FS_FULL_SIZE_INFORMATION
{
    LARGE_INTEGER TotalAllocationUnits;
    LARGE_INTEGER CallerAvailableAllocationUnits;
    LARGE_INTEGER ActualAvailableAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_FULL_SIZE_INFORMATION, *PFILE_FS_FULL_SIZE_INFORMATION;

typedef struct _FILE_FS_OBJECTID_INFORMATION
{
    UCHAR ObjectId[16];
    UCHAR ExtendedInfo[48];
} FILE_FS_OBJECTID_INFORMATION, *PFILE_FS_OBJECTID_INFORMATION;

typedef struct _FILE_FS_DEVICE_INFORMATION
{
    DEVICE_TYPE DeviceType;
    ULONG Characteristics;
} FILE_FS_DEVICE_INFORMATION, *PFILE_FS_DEVICE_INFORMATION;

typedef struct _FILE_FS_ATTRIBUTE_INFORMATION
{
    ULONG FileSystemAttributes;
    LONG MaximumComponentNameLength;
    ULONG FileSystemNameLength;
    OCHAR FileSystemName[1];
} FILE_FS_ATTRIBUTE_INFORMATION, *PFILE_FS_ATTRIBUTE_INFORMATION;

typedef struct _MEMORY_BASIC_INFORMATION
{
    PVOID BaseAddress;
    PVOID AllocationBase;
    DWORD AllocationProtect;
    DWORD RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef struct _SID_IDENTIFIER_AUTHORITY
{
    BYTE Value[6];
} SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;

typedef struct _SID
{
    BYTE Revision;
    BYTE SubAuthorityCount;
    SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
    DWORD SubAuthority[1]; /**< Actually has size 'SubAuthorityCount' **/
} SID, *PISID;

typedef enum _FILE_INFORMATION_CLASS
{
    FileDirectoryInformation = 1,
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
    FileObjectIdInformation,
    FileCompletionInformation,
    FileMoveClusterInformation,
    FileQuotaInformation,
    FileReparsePointInformation,
    FileNetworkOpenInformation,
    FileAttributeTagInformation,
    FileTrackingInformation,
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef struct _FILE_DIRECTORY_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    OCHAR FileName[1];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_FULL_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    WCHAR FileName[1];
} FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;

typedef struct _FILE_BOTH_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

typedef struct _FILE_BASIC_INFORMATION
{
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _FILE_STANDARD_INFORMATION
{
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;

typedef struct _FILE_INTERNAL_INFORMATION
{
    LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;

typedef struct _FILE_EA_INFORMATION
{
    ULONG EaSize;
} FILE_EA_INFORMATION, *PFILE_EA_INFORMATION;

typedef struct _FILE_ACCESS_INFORMATION
{
    ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;

typedef struct _FILE_NAME_INFORMATION
{
    ULONG FileNameLength;
    OCHAR FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

typedef struct _FILE_RENAME_INFORMATION
{
    BOOLEAN ReplaceIfExists;
    HANDLE RootDirectory;
    OBJECT_STRING FileName;
} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;

typedef struct _FILE_LINK_INFORMATION
{
    BOOLEAN ReplaceIfExists;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    OCHAR FileName[1];
} FILE_LINK_INFORMATION, *PFILE_LINK_INFORMATION;

typedef struct _FILE_NAMES_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    ULONG FileNameLength;
    OCHAR FileName[1];
} FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;

typedef struct _FILE_DISPOSITION_INFORMATION
{
    BOOLEAN DeleteFile;
} FILE_DISPOSITION_INFORMATION, *PFILE_DISPOSITION_INFORMATION;

typedef struct _FILE_POSITION_INFORMATION
{
    LARGE_INTEGER CurrentByteOffset;
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;

typedef struct _FILE_FULL_EA_INFORMATION
{
    ULONG NextEntryOffset;
    UCHAR Flags;
    UCHAR EaNameLength;
    USHORT EaValueLength;
    CHAR EaName[1];
} FILE_FULL_EA_INFORMATION, *PFILE_FULL_EA_INFORMATION;

typedef struct _FILE_MODE_INFORMATION
{
    ULONG Mode;
} FILE_MODE_INFORMATION, *PFILE_MODE_INFORMATION;

typedef struct _FILE_ALIGNMENT_INFORMATION
{
    ULONG AlignmentRequirement;
} FILE_ALIGNMENT_INFORMATION, *PFILE_ALIGNMENT_INFORMATION;

typedef struct _FILE_ALL_INFORMATION
{
    FILE_BASIC_INFORMATION BasicInformation;
    FILE_STANDARD_INFORMATION StandardInformation;
    FILE_INTERNAL_INFORMATION InternalInformation;
    FILE_EA_INFORMATION EaInformation;
    FILE_ACCESS_INFORMATION AccessInformation;
    FILE_POSITION_INFORMATION PositionInformation;
    FILE_MODE_INFORMATION ModeInformation;
    FILE_ALIGNMENT_INFORMATION AlignmentInformation;
    FILE_NAME_INFORMATION NameInformation;
} FILE_ALL_INFORMATION, *PFILE_ALL_INFORMATION;

typedef struct _FILE_ALLOCATION_INFORMATION
{
    LARGE_INTEGER AllocationSize;
} FILE_ALLOCATION_INFORMATION, *PFILE_ALLOCATION_INFORMATION;

typedef struct _FILE_END_OF_FILE_INFORMATION
{
    LARGE_INTEGER EndOfFile;
} FILE_END_OF_FILE_INFORMATION, *PFILE_END_OF_FILE_INFORMATION;

typedef struct _FILE_STREAM_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG StreamNameLength;
    LARGE_INTEGER StreamSize;
    LARGE_INTEGER StreamAllocationSize;
    OCHAR StreamName[1];
} FILE_STREAM_INFORMATION, *PFILE_STREAM_INFORMATION;

typedef struct _FILE_PIPE_INFORMATION
{
    ULONG ReadMode;
    ULONG CompletionMode;
} FILE_PIPE_INFORMATION, *PFILE_PIPE_INFORMATION;

typedef struct _FILE_PIPE_LOCAL_INFORMATION
{
    ULONG NamedPipeType;
    ULONG NamedPipeConfiguration;
    ULONG MaximumInstances;
    ULONG CurrentInstances;
    ULONG InboundQuota;
    ULONG ReadDataAvailable;
    ULONG OutboundQuota;
    ULONG WriteQuotaAvailable;
    ULONG NamedPipeState;
    ULONG NamedPipeEnd;
} FILE_PIPE_LOCAL_INFORMATION, *PFILE_PIPE_LOCAL_INFORMATION;

typedef struct _FILE_PIPE_REMOTE_INFORMATION
{
    LARGE_INTEGER CollectDataTime;
    ULONG MaximumCollectionCount;
} FILE_PIPE_REMOTE_INFORMATION, *PFILE_PIPE_REMOTE_INFORMATION;

typedef struct _FILE_MAILSLOT_QUERY_INFORMATION
{
    ULONG MaximumMessageSize;
    ULONG MailslotQuota;
    ULONG NextMessageSize;
    ULONG MessagesAvailable;
    LARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_QUERY_INFORMATION, *PFILE_MAILSLOT_QUERY_INFORMATION;

typedef struct _FILE_MAILSLOT_SET_INFORMATION
{
    PLARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_SET_INFORMATION, *PFILE_MAILSLOT_SET_INFORMATION;

typedef struct _FILE_COMPRESSION_INFORMATION
{
    LARGE_INTEGER CompressedFileSize;
    USHORT CompressionFormat;
    UCHAR CompressionUnitShift;
    UCHAR ChunkShift;
    UCHAR ClusterShift;
    UCHAR Reserved[3];
} FILE_COMPRESSION_INFORMATION, *PFILE_COMPRESSION_INFORMATION;

typedef struct _FILE_OBJECTID_INFORMATION
{
    LONGLONG FileReference;
    UCHAR ObjectId[16];
    union
    {
        struct
        {
            UCHAR BirthVolumeId[16];
            UCHAR BirthObjectId[16];
            UCHAR DomainId[16];
        };
        UCHAR ExtendedInfo[48];
    };
} FILE_OBJECTID_INFORMATION, *PFILE_OBJECTID_INFORMATION;

typedef struct _FILE_COMPLETION_INFORMATION
{
    HANDLE Port;
    PVOID Key;
} FILE_COMPLETION_INFORMATION, *PFILE_COMPLETION_INFORMATION;

typedef struct _FILE_MOVE_CLUSTER_INFORMATION
{
    ULONG ClusterCount;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    OCHAR FileName[1];
} FILE_MOVE_CLUSTER_INFORMATION, *PFILE_MOVE_CLUSTER_INFORMATION;

typedef struct _FILE_QUOTA_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG SidLength;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER QuotaUsed;
    LARGE_INTEGER QuotaThreshold;
    LARGE_INTEGER QuotaLimit;
    SID Sid;
} FILE_QUOTA_INFORMATION, *PFILE_QUOTA_INFORMATION;

typedef struct _FILE_REPARSE_POINT_INFORMATION
{
    LONGLONG FileReference;
    ULONG Tag;
} FILE_REPARSE_POINT_INFORMATION, *PFILE_REPARSE_POINT_INFORMATION;

typedef struct _FILE_NETWORK_OPEN_INFORMATION
{
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

typedef struct _FILE_ATTRIBUTE_TAG_INFORMATION
{
    ULONG FileAttributes;
    ULONG ReparseTag;
} FILE_ATTRIBUTE_TAG_INFORMATION, *PFILE_ATTRIBUTE_TAG_INFORMATION;

typedef struct _FILE_TRACKING_INFORMATION
{
    HANDLE DestinationFile;
    ULONG ObjectInformationLength;
    CHAR ObjectInformation[1];
} FILE_TRACKING_INFORMATION, *PFILE_TRACKING_INFORMATION;

/**
 * Object Attributes Structure
 */
typedef struct _OBJECT_ATTRIBUTES
{
        HANDLE RootDirectory; /**< Optional handle to the root object directory for the path name specified by the ObjectName member. If RootDirectory is NULL, ObjectName must point to a fully qualified object name that includes the full path to the target object. If RootDirectory is non-NULL, ObjectName specifies an object name relative to the RootDirectory directory. */
        PANSI_STRING ObjectName; /**< Pointer to a Unicode string that contains the name of the object for which a handle is to be opened. This must either be a fully qualified object name, or a relative path name to the directory specified by the RootDirectory member. */
        ULONG Attributes; /**< Bitmask of flags that specify object handle attributes. */
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

#define OBJ_INHERIT 0x00000002L
#define OBJ_PERMANENT 0x00000010L
#define OBJ_EXCLUSIVE 0x00000020L
#define OBJ_CASE_INSENSITIVE 0x00000040L
#define OBJ_OPENIF 0x00000080L
#define OBJ_OPENLINK 0x00000100L
#define OBJ_VALID_ATTRIBUTES 0x000001F2L

/*
    VOID InitializeObjectAttributes (
        OUT POBJECT_ATTRIBUTES p,
        IN PUNICODE_STRING n,
        IN ULONG a,
        IN HANDLE r,
        IN PSECURITY_DESCRIPTOR s
    );
*/
#define InitializeObjectAttributes(p, n, a, r, s) { \
    (p)->RootDirectory = r; \
    (p)->Attributes = a; \
    (p)->ObjectName = n; \
}

typedef enum _EVENT_TYPE
{
	NotificationEvent = 0,
	SynchronizationEvent
} EVENT_TYPE;

/**
 * Memory manager statistics
 */
typedef struct _MM_STATISTICS
{
	ULONG Length; /**< Length of MM_STATISTICS in bytes */
	ULONG TotalPhysicalPages;
	ULONG AvailablePages;
	ULONG VirtualMemoryBytesCommitted;
	ULONG VirtualMemoryBytesReserved;
	ULONG CachePagesCommitted;
	ULONG PoolPagesCommitted;
	ULONG StackPagesCommitted;
	ULONG ImagePagesCommitted;
} MM_STATISTICS, *PMM_STATISTICS;

typedef struct _LAUNCH_DATA_HEADER
{
	DWORD dwLaunchDataType;
	DWORD dwTitleId;
	CHAR szLaunchPath[520];
	DWORD dwFlags;
} LAUNCH_DATA_HEADER, *PLAUNCH_DATA_HEADER;

typedef struct _LAUNCH_DATA_PAGE
{
	LAUNCH_DATA_HEADER Header;
	UCHAR Pad[492];
	UCHAR LaunchData[3072];
} LAUNCH_DATA_PAGE, *PLAUNCH_DATA_PAGE;

#define LDT_LAUNCH_DASHBOARD 1
#define LDT_NONE 0xFFFFFFFF

typedef struct _DISPATCHER_HEADER
{
	UCHAR Type;
	UCHAR Absolute;
	UCHAR Size;
	UCHAR Inserted;
	LONG SignalState;
	LIST_ENTRY WaitListHead;
} DISPATCHER_HEADER;

typedef struct _KDPC
{
	CSHORT Type;
	BOOLEAN Inserted;
	UCHAR Padding;
	LIST_ENTRY DpcListEntry;
	PVOID DeferredRoutine;
	PVOID DeferredContext;
	PVOID SystemArgument1;
	PVOID SystemArgument2;
} KDPC, *PKDPC, *RESTRICTED_POINTER PRKDPC;

/**
 * The kernels way of representing a timer object. MS recommends not to
 * manipulate the members directly.
 */
typedef struct _KTIMER
{
	DISPATCHER_HEADER Header;
	ULARGE_INTEGER DueTime;
	LIST_ENTRY TimerListEntry;
	PKDPC Dpc;
	LONG Period;
} KTIMER, *PKTIMER;

typedef struct _KPROCESS
{
    LIST_ENTRY ReadListHead;
    LIST_ENTRY ThreadListHead;
    ULONG StackCount;
    LONG ThreadQuantum;
    SCHAR BasePriority;
    UCHAR DisableBoost;
    UCHAR DisableQuantum;
} KPROCESS, *PKPROCESS;

typedef struct _KAPC_STATE
{
    LIST_ENTRY ApcListHead[2];
    PKPROCESS Process;
    UCHAR KernelApcInProgress;
    UCHAR KernelApcPending;
    UCHAR UserApcPending;
    UCHAR ApcQueueable;
} KAPC_STATE;

typedef struct _KQUEUE
{
    DISPATCHER_HEADER Header;
    LIST_ENTRY EntryListHead;
    ULONG CurrentCount;
    ULONG MaximumCount;
    LIST_ENTRY ThreadListHead;
} KQUEUE, *PKQUEUE, *RESTRICTED_POINTER PRKQUEUE;

struct KTHREAD;

typedef struct _KWAIT_BLOCK
{
    LIST_ENTRY WaitListEntry;
    struct KTHREAD *Thread;
    PVOID Object;
    struct _KWAIT_BLOCK *NextWaitBlock;
    SHORT WaitKey;
    SHORT WaitType;
} KWAIT_BLOCK, *PKWAIT_BLOCK;

typedef struct _KAPC
{
    SHORT Type;
    CHAR ApcMode;
    UCHAR Inserted;
    struct KTHREAD *Thread;
    LIST_ENTRY ApcListEntry;
    PVOID KernelRoutine;
    PVOID RundownRoutine;
    PVOID NormalRoutine;
    PVOID NormalContext;
    PVOID SystemArgument1;
    PVOID SystemArgument2;
} KAPC, *PKAPC, *RESTRICTED_POINTER PRKAPC;

typedef struct _KSEMAPHORE
{
    DISPATCHER_HEADER Header;
    LONG Limit;
} KSEMAPHORE, *PKSEMAPHORE, *RESTRICTED_POINTER PRKSEMAPHORE;

typedef struct _KTHREAD
{
	DISPATCHER_HEADER Header;
    LIST_ENTRY MutantListHead;
    ULONG KernelTime;
    PVOID StackBase;
    PVOID StackLimit;
    PVOID KernelStack;
    PVOID TlsData;
    UCHAR State;
    UCHAR Alerted[2];
    UCHAR Alertable;
    UCHAR NpxState;
    CHAR Saturation;
    SCHAR Priority;
    UCHAR Padding;
    KAPC_STATE ApcState;
    ULONG ContextSwitches;
    LONG WaitStatus;
    UCHAR WaitIrql;
    CHAR WaitMode;
    UCHAR WaitNext;
    UCHAR WaitReason;
    PKWAIT_BLOCK WaitBlockList;
    LIST_ENTRY WaitListEntry;
    ULONG WaitTime;
    ULONG KernelApcDisable;
    LONG Quantum;
    SCHAR BasePriority;
    UCHAR DecrementCount;
    SCHAR PriorityDecrement;
    UCHAR DisableBoost;
    UCHAR NpxIrql;
    CHAR SuspendCount;
    UCHAR Preempted;
    UCHAR HasTerminated;
    PKQUEUE Queue;
    LIST_ENTRY QueueListEntry;
    KTIMER Timer;
    KWAIT_BLOCK TimerWaitBlock;
    KAPC SuspendApc;
    KSEMAPHORE SuspendSemaphore;
    LIST_ENTRY ThreadListEntry;
} KTHREAD, *PKTHREAD, *RESTRICTED_POINTER PRKTHREAD;

typedef enum _TIMER_TYPE
{
	NotificationTimer,
	SynchronizationTimer
} TIMER_TYPE;

typedef struct _KINTERRUPT
{
    PVOID ServiceRoutine;
    PVOID ServiceContext;
    ULONG BusInterruptLevel;
	ULONG Irql;
    UCHAR Connected;
    UCHAR ShareVector;
    UCHAR Mode;
    UCHAR Padding7;
    ULONG ServiceCount;
    ULONG DispatchCode[22];
} KINTERRUPT, *PKINTERRUPT;

typedef struct _KSYSTEM_TIME
{
    ULONG LowPart;
    LONG High1Time;
    LONG High2Time;
} KSYSTEM_TIME;

typedef enum _FIRMWARE_REENTRY
{
	HalHaltRoutine,
	HalRebootRoutine,
	HalQuickRebootRoutine,
	HalKdRebootRoutine,
	HalFatalErrorRebootRoutine,
	HalMaximumRoutine
} FIRMWARE_REENTRY;

typedef struct _XBEIMAGE_SECTION
{
    ULONG SectionFlags;
    ULONG virtualAddress;
    ULONG VirtuaSize;
    ULONG PointerToRawData;
    ULONG SizeOfRawData;
    PUCHAR SectionName;
    ULONG SectionReferenceCount;
    PSHORT HeadSharedPageReferenceCount;
    PSHORT TailSharedPageReferenceCount;
    UCHAR SectionDigest[20];
} XBEIMAGE_SECTION, *PXBEIMAGE_SECTION;

typedef struct _OBJECT_TYPE
{
    PVOID AllocateProcedure;
    PVOID FreeProcedure;
    PVOID CloseProcedure;
    PVOID DeleteProcedure;
    PVOID ParseProcedure;
    PVOID DefaultObject;
    ULONG PoolTag;
} OBJECT_TYPE, *POBJECT_TYPE;

typedef NTAPI VOID (*PKDEFERRED_ROUTINE) (
    IN PKDPC Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
);

typedef struct _KDEVICE_QUEUE
{
    SHORT Type;
    UCHAR Size;
    UCHAR Busy;
    LIST_ENTRY DeviceListHead;
} KDEVICE_QUEUE, *PKDEVICE_QUEUE;

typedef VOID (*PKSTART_ROUTINE) (
    IN PVOID StartContext
);

typedef union _FILE_SEGMENT_ELEMENT
{
    PVOID Buffer;
    DWORD Alignment;
} FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;

typedef VOID (*PIO_APC_ROUTINE) (
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
);

typedef struct _SEMAPHORE_BASIC_INFORMATION
{
    LONG CurrentCount;
    LONG MaximumCount;
} SEMAPHORE_BASIC_INFORMATION, *PSEMAPHORE_BASIC_INFORMATION;

typedef struct _MUTANT_BASIC_INFORMATION
{
    LONG CurrentCount;
    UCHAR OwnedByCaller;
    UCHAR AbandonedState;
} MUTANT_BASIC_INFORMATION, *PMUTANT_BASIC_INFORMATION;

typedef struct _IO_COMPLETION_BASIC_INFORMATION
{
    LONG Depth;
} IO_COMPLETION_BASIC_INFORMATION, *PIO_COMPLETION_BASIC_INFORMATION;

typedef struct _EVENT_BASIC_INFORMATION
{
    EVENT_TYPE EventType;
    LONG EventState;
} EVENT_BASIC_INFORMATION, *PEVENT_BASIC_INFORMATION;

typedef struct _PS_STATISTICS
{
    ULONG Length;
    ULONG ThreadCount;
    ULONG HandleCount;
} PS_STATISTICS, *PPS_STATISTICS;

typedef struct _ETHREAD
{
    KTHREAD Tcb;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER ExitTime;

    union
    {
        NTSTATUS ExitStatus;
        PVOID OfsChain;
    };

    union
    {
        LIST_ENTRY ReaperListEntry;
        LIST_ENTRY ActiveTimerListHead;
    };

    HANDLE UniqueThread;
    PVOID StartAddress;
    LIST_ENTRY IrpList;
    PVOID DebugData;
} ETHREAD, *PETHREAD;

typedef VOID (*PCREATE_THREAD_NOTIFY_ROUTINE) (
    IN PETHREAD Thread,
    IN HANDLE ThreadId,
    IN BOOLEAN Create
);

/**
 * Enumeration type for indicating whether an interrupt is level- or
 * edge-triggered.
 */
typedef enum _KINTERRUPT_MODE
{
    LevelSensitive, /**< Interrupt is level-triggered. Used for traditional PCI line-based interrupts. */
    Latched /**< Interrupt is edge-triggered. Used for PCI message-signaled interrupts */
} KINTERRUPT_MODE;

typedef BOOLEAN (* NTAPI PKSERVICE_ROUTINE) (
    IN PKINTERRUPT Interrupt,
    IN PVOID ServiceContext
);

typedef struct _TIMER_BASIC_INFORMATION
{
    LARGE_INTEGER RemainingTime;
    BOOLEAN TimerState;
} TIMER_BASIC_INFORMATION, *PTIMER_BASIC_INFORMATION;

typedef VOID (*PTIMER_APC_ROUTINE) (
    IN PVOID TimerContext,
    IN ULONG TimerLowValue,
    IN LONG TimerHighValue
);

typedef struct _XBOX_KRNL_VERSION
{
    USHORT Major;
    USHORT Minor;
    USHORT Build;
    USHORT Qfe;
} XBOX_KRNL_VERSION, *PXBOX_KRNL_VERSION;

/**
 * Information about the XBOX-hardware
 */
typedef struct _XBOX_HARDWARE_INFO
{
    ULONG Flags;
    UCHAR GpuRevision;
    UCHAR McpRevision;
    UCHAR reserved[2];
} XBOX_HARDWARE_INFO;

#define XBOX_HW_FLAG_INTERNAL_USB_HUB 0x00000001
#define XBOX_HW_FLAG_DEVKIT_KERNEL 0x00000002
#define XBOX_480P_MACROVISION_ENABLED 0x00000004
#define XBOX_HW_FLAG_ARCADE 0x00000008

#define XBOX_KEY_LENGTH 16
typedef UCHAR XBOX_KEY_DATA[XBOX_KEY_LENGTH];

typedef struct _GENERIC_MAPPING
{
    ACCESS_MASK GenericRead;
    ACCESS_MASK GenericWrite;
    ACCESS_MASK GenericExecute;
    ACCESS_MASK GenericAll;
} GENERIC_MAPPING, *PGENERIC_MAPPING;

/**
 * Describes an entry in (or the header of) a singly linked list
 */
typedef struct _SINGLE_LIST_ENTRY
{
    struct _SINGLE_LIST_ENTRY *Next; /**< Pointer to the next (or first, if this is a header) entry in the singly linked list (NULL if there is none) */
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;

/**
 * Serves as a header for a singly linked list. Initialized by ExInitializeSListHead
 */
typedef struct _SLIST_HEADER
{
    ULONGLONG Alignment;
    struct
    {
        SINGLE_LIST_ENTRY Next;
        USHORT Depth;
        USHORT Sequence;
    };
} SLIST_HEADER, *PSLIST_HEADER;

typedef struct _KEVENT
{
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *RESTRICTED_POINTER PRKEVENT;

typedef struct _KDEVICE_QUEUE_ENTRY
{
    LIST_ENTRY DeviceListEntry;
    ULONG SortKey;
    BOOLEAN Inserted;
} KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY;

typedef struct _IO_COMPLETION_CONTEXT
{
    PVOID Port;
    PVOID Key;
} IO_COMPLETION_CONTEXT, *PIO_COMPLETION_CONTEXT;

struct _DEVICE_OBJECT;

typedef struct _FILE_OBJECT
{
    CSHORT Type;
    BOOLEAN DeletePending : 1;
    BOOLEAN ReadAccess : 1;
    BOOLEAN WriteAccess : 1;
    BOOLEAN DeleteAccess : 1;
    BOOLEAN SharedRead : 1;
    BOOLEAN SharedWrite : 1;
    BOOLEAN SharedDelete : 1;
    BOOLEAN Reserved : 1;
    UCHAR Flags;
    struct _DEVICE_OBJECT *DeviceObject;
    PVOID FsContext;
    PVOID FsContext2;
    NTSTATUS FinalStatus;
    LARGE_INTEGER CurrentByteOffset;
    struct _FILE_OBJECT *RelatedFileObject;
    PIO_COMPLETION_CONTEXT CompletionContext;
    LONG LockCount;
    KEVENT Lock;
    KEVENT Event;
} FILE_OBJECT, *PFILE_OBJECT;

/**
 * This struct represents an I/O request packet
 */
typedef struct _IRP
{
    CSHORT Type;
    USHORT Size;
    ULONG Flags; /**< Flags for the packet */
    LIST_ENTRY ThreadListEntry;
    IO_STATUS_BLOCK IoStatus;
    CHAR StackCount;
    CHAR CurrentLocation;
    BOOLEAN PendingReturned;
    BOOLEAN Cancel; /**< Has the packet been canceled? */
    PIO_STATUS_BLOCK UserIosb;
    PKEVENT UserEvent;

    union
    {
        struct
        {
            PIO_APC_ROUTINE UserApcRoutine;
            PVOID UserApcContext;
        } AsynchronousParameters;
        LARGE_INTEGER AllocationSize;
    } Overlay;

    PVOID UserBuffer;
    PFILE_SEGMENT_ELEMENT SegmentArray;
    ULONG LockedBufferLength;

    union
    {
        struct
        {
            union
            {
                KDEVICE_QUEUE_ENTRY DeviceQueueEntry;
                struct
                {
                    PVOID DriverContext[5];
                };
            };
            PETHREAD Thread;

            struct
            {
                LIST_ENTRY ListEntry;
                union
                {
                    struct _IO_STATUS_LOCATION *CurrentStackLocation;
                    ULONG PacketType;
                };
            };

            PFILE_OBJECT OriginalFileObject;
        } Overlay;

        KAPC Apc;
        PVOID CompletionKey;
    } Tail;
} IRP, *PIRP;

#define IRP_NOCACHE 0x00000001
#define IRP_MOUNT_COMPLETION 0x00000002
#define IRP_SYNCHRONOUS_API 0x00000004
#define IRP_CREATE_OPERATION 0x00000008
#define IRP_READ_OPERATION 0x00000010
#define IRP_WRITE_OPERATION 0x00000020
#define IRP_CLOSE_OPERATION 0x00000040
#define IRP_DEFER_IO_COMPLETION 0x00000080
#define IRP_OB_QUERY_NAME 0x00000100
#define IRP_UNLOCK_USER_BUFFER 0x00000200
#define IRP_SCATTER_GATHER_OPERATION 0x00000400
#define IRP_UNMAP_SEGMENT_ARRAY 0x00000800
#define IRP_NO_CANCELIO 0x00001000

struct _DEVICE_OBJECT;

typedef NTAPI VOID (*PDRIVER_STARTIO) (
    IN struct _DEVICE_OBJECT *DeviceObject,
    IN struct _IRP *Irp
);

typedef NTAPI VOID (*PDRIVER_DELETEDEVICE) (
    IN struct _DEVICE_OBJECT *DeviceObject
);

typedef NTAPI NTSTATUS (*PDRIVER_DISMOUNTVOLUME) (
    IN struct _DEVICE_OBJECT *DeviceObject
);

typedef NTAPI NTSTATUS (*PDRIVER_DISPATCH) (
    IN struct _DEVICE_OBJECT *DeviceObject,
    IN struct _IRP *Irp
);

typedef struct _DRIVER_OBJECT
{
    PDRIVER_STARTIO DriverStartIo;
    PDRIVER_DELETEDEVICE DriverDeleteDevice;
    PDRIVER_DISMOUNTVOLUME DriverDismountVolume;
    PDRIVER_DISPATCH MajorFunction[0x0E];
} DRIVER_OBJECT, *PDRIVER_OBJECT;

typedef struct _DEVICE_OBJECT
{
    CSHORT Type;
    USHORT Size;
    LONG ReferenceCount;
    PDRIVER_OBJECT DriverObject;
    struct _DEVICE_OBJECT *MountedOrSelfDevice;
    PIRP CurrentIrp;
    ULONG Flags;
    PVOID DeviceExtension;
    UCHAR DeviceType;
    UCHAR StartIoFlags;
    CCHAR StackSize;
    BOOLEAN DeletePending;
    ULONG SectorSize;
    ULONG AlignmentRequirement;
    KDEVICE_QUEUE DeviceQueue;
    KEVENT DeviceLock;
    ULONG StartIoKey;
} DEVICE_OBJECT, *PDEVICE_OBJECT;

typedef struct _ERWLOCK
{
    LONG LockCount;
    ULONG WritersWaitingCount;
    ULONG ReadersWaitingCount;
    ULONG ReadersEntryCount;
    KEVENT WriterEvent;
    KSEMAPHORE ReaderSemaphore;
} ERWLOCK, *PERWLOCK;

typedef struct _EXCEPTION_RECORD
{
    NTSTATUS ExceptionCode;
    ULONG ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    PVOID ExceptionAddress;
    ULONG NumberParameters;
    ULONG_PTR ExceptionInformation[15];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;

typedef struct _XBOX_REFURB_INFO
{
    ULONG Signature;
    ULONG PowerCycleCount;
    LARGE_INTEGER FirstSetTime;
} XBOX_REFURB_INFO;

typedef struct _FLOATING_SAVE_AREA
{
    WORD ControlWord;
    WORD StatusWord;
    WORD TagWord;
    WORD ErrorOpcode;
    DWORD ErrorOffset;
    DWORD ErrorSelector;
    DWORD DataOffset;
    DWORD DataSelector;
    DWORD MXCsr;
    DWORD Reserved2;
    BYTE RegisterArea[128];
    BYTE XmmRegisterArea[128];
    BYTE Reserved4[224];
    DWORD Cr0NpxState;
} __attribute__((packed)) FLOATING_SAVE_AREA, *PFLOATING_SAVE_AREA;

typedef struct _CONTEXT
{
    DWORD ContextFlags;
    FLOATING_SAVE_AREA FloatSave;
    DWORD Edi;
    DWORD Esi;
    DWORD Ebx;
    DWORD Edx;
    DWORD Ecx;
    DWORD Eax;
    DWORD Ebp;
    DWORD Eip;
    DWORD SegCs;
    DWORD EFlags;
    DWORD Esp;
    DWORD SegSs;
} CONTEXT, *PCONTEXT;

typedef struct _KFLOATING_SAVE
{
    ULONG ControlWord;
    ULONG StatusWord;
    ULONG ErrorOffset;
    ULONG ErrorSelector;
    ULONG DataOffset;
    ULONG DataSelector;
    ULONG Cr0NpxState;
    ULONG Spare1;
} KFLOATING_SAVE, *PKFLOATING_SAVE;

typedef struct _HARDWARE_PTE
{
    ULONG Valid : 1;
    ULONG Write : 1;
    ULONG Owner : 1;
    ULONG WriteThrough : 1;
    ULONG CacheDisable : 1;
    ULONG Accessed : 1;
    ULONG Dirty : 1;
    ULONG LargePage : 1;
    ULONG Global : 1;
    ULONG GuardOrEndOfAllocation : 1;
    ULONG PersistAllocation : 1;
    ULONG reserved : 1;
    ULONG PageFrameNumber : 20;
} HARDWARE_PTE, *PHARDWARE_PTE;

typedef VOID (*PPS_APC_ROUTINE) (
    IN PVOID ApcArgument1,
    IN PVOID ApcArgument2,
    IN PVOID ApcArgument3
);

typedef enum _KWAIT_REASON
{
    Executive,
    FreePage,
    PageIn,
    PoolAllocation,
    DelayExecution,
    Suspended,
    UserRequest,
    WrExecutive,
    WrFreePage,
    WrPageIn,
    WrPoolAllocation,
    WrDelayExecution,
    WrSuspended,
    WrUserRequest,
    WrEventPair,
    WrQueue,
    WrLpcReceive,
    WrLpcReply,
    WrVirtualMemory,
    WrPageOut,
    WrRendezvous,
    WrFsCacheIn,
    WrFsCacheOut,
    Spare4,
    Spare5,
    Spare6,
    WrKernel,
    MaximumWaitReason
} KWAIT_REASON;

typedef struct _KMUTANT
{
    DISPATCHER_HEADER Header;
    LIST_ENTRY MutantListEntry;
    PRKTHREAD OwnerThread;
    BOOLEAN Abandoned;
} KMUTANT, *PKMUTANT, *RESTRICTED_POINTER PRKMUTANT;

typedef struct _SHARE_ACCESS
{
    UCHAR OpenCount;
    UCHAR Readers;
    UCHAR Writers;
    UCHAR Deleters;
    UCHAR SharedRead;
    UCHAR SharedWrite;
    UCHAR SharedDelete;
} SHARE_ACCESS, *PSHARE_ACCESS;

typedef BOOLEAN (*PKSYNCHRONIZE_ROUTINE) (
    IN PVOID SynchronizeContext
);

typedef VOID (*PKRUNDOWN_ROUTINE) (
    IN PKAPC Apc
);

typedef VOID (*PKNORMAL_ROUTINE) (
    IN PVOID NormalContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
);

typedef VOID (*PKKERNEL_ROUTINE) (
    IN PKAPC Apc,
    IN OUT PKNORMAL_ROUTINE *NormalRoutine,
    IN OUT PVOID *NormalContext,
    IN OUT PVOID *SystemArgument1,
    IN OUT PVOID *SystemArgument2
);

struct _HAL_SHUTDOWN_REGISTRATION;

typedef NTAPI VOID (*PHAL_SHUTDOWN_NOTIFICATION) (
    IN struct _HAL_SHUTDOWN_REGISTRATION *ShutdownRegistration
);

typedef struct _HAL_SHUTDOWN_REGISTRATION
{
    PHAL_SHUTDOWN_NOTIFICATION NotificationRoutine;
    LONG Priority;
    LIST_ENTRY ListEntry;
} HAL_SHUTDOWN_REGISTRATION, *PHAL_SHUTDOWN_REGISTRATION;

typedef VOID (*pfXcSHAInit) (PUCHAR pbSHAContext);
typedef VOID (*pfXcSHAUpdate) (PUCHAR pbSHAContext, PUCHAR pbInput, ULONG dwInputLength);
typedef VOID (*pfXcSHAFinal) (PUCHAR pbSHAContext, PUCHAR pbDigest);
typedef VOID (*pfXcRC4Key) (PUCHAR pbKeyStruct, ULONG dwKeyLength, PUCHAR pbKey);
typedef VOID (*pfXcRC4Crypt) (PUCHAR pbKeyStruct, ULONG dwInputLength, PUCHAR pbInput);
typedef VOID (*pfXcHMAC) (PUCHAR pbKey, ULONG dwKeyLength, PUCHAR pbInput, ULONG dwInputLength, PUCHAR pbInput2, ULONG dwInputLength2, PUCHAR pbDigest);
typedef ULONG (*pfXcPKEncPublic) (PUCHAR pbPubKey, PUCHAR pbInput, PUCHAR pbOutput);
typedef ULONG (*pfXcPKDecPrivate) (PUCHAR pbPrvKey, PUCHAR pbInput, PUCHAR pbOutput);
typedef ULONG (*pfXcPKGetKeyLen) (PUCHAR pbPubKey);
typedef BOOLEAN (*pfXcVerifyPKCS1Signature) (PUCHAR pbSig, PUCHAR pbPubKey, PUCHAR pbDigest);
typedef ULONG (*pfXcModExp) (PULONG pA, PULONG pB, PULONG pC, PULONG pD, ULONG dwN);
typedef VOID (*pfXcDESKeyParity) (PUCHAR pbKey, ULONG dwKeyLength);
typedef VOID (*pfXcKeyTable) (ULONG dwCipher, PUCHAR pbKeyTable, PUCHAR pbKey);
typedef VOID (*pfXcBlockCrypt) (ULONG dwCipher, PUCHAR pbOutput, PUCHAR pbInput, PUCHAR pbKeyTable, ULONG dwOp);
typedef VOID (*pfXcBlockCryptCBC) (ULONG dwCipher, ULONG dwInputLength, PUCHAR pbOutput, PUCHAR pbInput, PUCHAR pbKeyTable, ULONG dwOp, PUCHAR pbFeedback);
typedef ULONG (*pfXcCryptService) (ULONG dwOp, PVOID pArgs);

typedef struct
{
    pfXcSHAInit pXcSHAInit;
    pfXcSHAUpdate pXcSHAUpdate;
    pfXcSHAFinal pXcSHAFinal;
    pfXcRC4Key pXcRC4Key;
    pfXcRC4Crypt pXcRC4Crypt;
    pfXcHMAC pXcHMAC;
    pfXcPKEncPublic pXcPKEncPublic;
    pfXcPKDecPrivate pXcPKDecPrivate;
    pfXcPKGetKeyLen pXcPKGetKeyLen;
    pfXcVerifyPKCS1Signature pXcVerifyPKCS1Signature;
    pfXcModExp pXcModExp;
    pfXcDESKeyParity pXcDESKeyParity;
    pfXcKeyTable pXcKeyTable;
    pfXcBlockCrypt pXcBlockCrypt;
    pfXcBlockCryptCBC pXcBlockCryptCBC;
    pfXcCryptService pXcCryptService;
} CRYPTO_VECTOR, *PCRYPTO_VECTOR;

typedef struct _OBJECT_HANDLE_TABLE
{
    LONG HandleCount;
    LONG_PTR FirstFreeTableEntry;
    HANDLE NextHandleNeedingPool;
    PVOID **RootTable;
    PVOID *BuiltinRootTable[8];
} OBJECT_HANDLE_TABLE, *POBJECT_HANDLE_TABLE;

typedef struct _MMPFNFREE
{
    USHORT PackedPfnFlink;
    USHORT PackedPfnBlink;
} MMPFNFREE, *PMMPFNFREE;

typedef struct _MMPFNREGION
{
    MMPFNFREE FreePagesByColor[32];
    PFN_COUNT AvailablePages;
} MMPFNREGION, *PMMPFNREGION;

typedef struct _MMPTE
{
    union
    {
        ULONG Long;
        HARDWARE_PTE Hard;
        struct
        {
            ULONG Valid : 1;
            ULONG OneEntry : 1;
            ULONG NextEntry : 30;
        } List;
    };
} MMPTE, *PMMPTE;

typedef enum _MMPFN_BUSY_TYPE
{
    MmUnknownUsage,
    MmStackUsage,
    MmVirtualPageTableUsage,
    MmSystemPageTableUsage,
    MmPoolUsage,
    MmVirtualMemoryUsage,
    MmSystemMemoryUsage,
    MmImageUsage,
    MmFsCacheUsage,
    MmContiguousUsage,
    MmDebuggerUsage,
    MmMaximumUsage
} MMPFN_BUSY_TYPE;

typedef PFN_NUMBER (FASTCALL *PMMREMOVE_PAGE_ROUTINE) (
    IN MMPFN_BUSY_TYPE BusyType,
    IN PMMPTE TargetPte
);

typedef struct _MMPTERANGE
{
    MMPTE HeadPte;
    PMMPTE FirstCommittedPte;
    PMMPTE LastCommittedPte;
    PMMPTE LastReservedPte;
    PFN_COUNT *AvailablePages;
    PMMREMOVE_PAGE_ROUTINE RemovePageRoutine;
} MMPTERANGE, *PMMPTERANGE;

typedef struct _MMADDRESS_NODE
{
    ULONG_PTR StartingVpn;
    ULONG_PTR EndingVpn;
    struct _MMADDRESS_NODE *Parent;
    struct _MMADDRESS_NODE *LeftChild;
    struct _MMADDRESS_NODE *RightChild;
} MMADDRESS_NODE, *PMMADDRESS_NODE;

typedef struct _MMGLOBALDATA
{
    PMMPFNREGION RetailPfnRegion;
    PMMPTERANGE SystemPteRange;
    PULONG AvailablePages;
    PFN_COUNT *AllocatedPagesByUsage;
    PRTL_CRITICAL_SECTION AddressSpaceLock;
    PMMADDRESS_NODE *VadRoot;
    PMMADDRESS_NODE *VadHint;
    PMMADDRESS_NODE *VadFreeHint;
} MMGLOBALDATA, *PMMGLOBALDATA;

typedef VOID (*PIDE_INTERRUPT_ROUTINE) (void);

typedef VOID (*PIDE_FINISHIO_ROUTINE) (void);

typedef BOOLEAN (*PIDE_POLL_RESET_COMPLETE_ROUTINE) (void);

typedef VOID (*PIDE_TIMEOUT_EXPIRED_ROUTINE) (void);

typedef VOID (*PIDE_START_PACKET_ROUTINE) (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
);

typedef VOID (*PIDE_START_NEXT_PACKET_ROUTINE) (void);

typedef struct _IDE_CHANNEL_OBJECT
{
    PIDE_INTERRUPT_ROUTINE InterruptRoutine;
    PIDE_FINISHIO_ROUTINE FinishIoRoutine;
    PIDE_POLL_RESET_COMPLETE_ROUTINE PollResetCompleteRoutine;
    PIDE_TIMEOUT_EXPIRED_ROUTINE TimeoutExpiredRoutine;
    PIDE_START_PACKET_ROUTINE StartPacketRoutine;
    PIDE_START_NEXT_PACKET_ROUTINE StartNextPacketRoutine;
    KIRQL InterruptIrql;
    BOOLEAN ExpectingBusMasterInterrupt;
    BOOLEAN StartPacketBusy;
    BOOLEAN StartPacketRequested;
    UCHAR Timeout;
    UCHAR IoRetries;
    UCHAR MaximumIoRetries;
    PIRP CurrentIrp;
    KDEVICE_QUEUE DeviceQueue;
    ULONG PhysicalRegionDescriptorTablePhysical;
    KDPC TimerDpc;
    KDPC FinishDpc;
    KTIMER Timer;
    KINTERRUPT InterruptObject;
} IDE_CHANNEL_OBJECT, *PIDE_CHANNEL_OBJECT;

typedef VOID (* NTAPI PKSYSTEM_ROUTINE) (
    IN PKSTART_ROUTINE StartRoutine OPTIONAL,
    IN PVOID StartContext OPTIONAL
);

// KPRIORITY values
#define LOW_PRIORITY 0              // Lowest thread priority level
#define LOW_REALTIME_PRIORITY 16    // Lowest realtime priority level
#define HIGH_PRIORITY 31            // Highest thread priority level
#define MAXIMUM_PRIORITY 32         // Number of thread priority levels

/**
 * Decrements the reference count of the section and unloads it if the reference count reaches zero.
 * @param Section The section to be unloaded.
 * @return STATUS_SUCCESS or the error.
 */
XBAPI NTSTATUS NTAPI XeUnloadSection
(
    IN OUT PXBEIMAGE_SECTION Section
);

XBAPI UCHAR XePublicKeyData[284];

/**
 * Loads the supplied section into memory or increases its reference count if it's already loaded.
 * @param Section The section to be loaded.
 * @return STATUS_SUCCESS or the error.
 */
XBAPI NTSTATUS NTAPI XeLoadSection
(
    IN PXBEIMAGE_SECTION Section
);

XBAPI ANSI_STRING XeImageFileName[1];

XBAPI BOOLEAN NTAPI XcVerifyPKCS1Signature
(
    IN PUCHAR pbSig,
    IN PUCHAR pbPubKey,
    IN PUCHAR pbDigest
);

XBAPI VOID NTAPI XcUpdateCrypto
(
    IN PCRYPTO_VECTOR pNewVector,
    OUT PCRYPTO_VECTOR pROMVector OPTIONAL
);

XBAPI VOID NTAPI XcSHAUpdate
(
    IN OUT PUCHAR pbSHAContext,
    IN PUCHAR pbInput,
    IN ULONG dwInputLength
);

XBAPI VOID NTAPI XcSHAInit
(
    OUT PUCHAR pbSHAContext
);

XBAPI VOID NTAPI XcSHAFinal
(
    IN PUCHAR pbSHAContext,
    OUT PUCHAR pbDigest
);

XBAPI VOID NTAPI XcRC4Key
(
    OUT PUCHAR pbKeyStruct,
    IN ULONG dwKeyLength,
    IN PUCHAR pbKey
);

XBAPI VOID NTAPI XcRC4Crypt
(
    IN PUCHAR pbKeyStruct,
    IN ULONG dwInputLength,
    IN OUT PUCHAR pbInput
);

XBAPI ULONG NTAPI XcPKGetKeyLen
(
    IN PUCHAR pbPubKey
);

XBAPI ULONG NTAPI XcPKEncPublic
(
    IN PUCHAR pbPubKey,
    IN PUCHAR pbInput,
    OUT PUCHAR pbOutput
);

XBAPI ULONG NTAPI XcPKDecPrivate
(
    IN PUCHAR pbPrvKey,
    IN PUCHAR pbInput,
    OUT PUCHAR pbOutput
);

XBAPI ULONG NTAPI XcModExp
(
    OUT PULONG pA,
    IN PULONG pB,
    IN PULONG pC,
    IN PULONG pD,
    IN ULONG dwN
);

XBAPI VOID NTAPI XcKeyTable
(
    IN ULONG dwCipher,
    OUT PUCHAR pbKeyTable,
    IN PUCHAR pbKey
);

XBAPI VOID NTAPI XcHMAC
(
    IN PUCHAR pbKey,
    IN ULONG dwKeyLength,
    IN PUCHAR pbInput,
    IN ULONG dwInputLength,
    IN PUCHAR pbInput2,
    IN ULONG dwInputLength2,
    OUT PUCHAR pbDigest
);

XBAPI VOID NTAPI XcDESKeyParity
(
    IN OUT PUCHAR pbKey,
    IN ULONG dwKeyLength
);

XBAPI ULONG NTAPI XcCryptService
(
    IN ULONG dwOp,
    IN PVOID pArgs
);

XBAPI VOID NTAPI XcBlockCryptCBC
(
    IN ULONG dwCipher,
    IN ULONG dwInputLength,
    OUT PUCHAR pbOutput,
    IN PUCHAR pbInput,
    IN PUCHAR pbKeyTable,
    IN ULONG dwOp,
    IN PUCHAR pbFeedback
);

XBAPI VOID NTAPI XcBlockCrypt
(
    IN ULONG dwCipher,
    OUT PUCHAR pbOutput,
    IN PUCHAR pbInput,
    IN PUCHAR pbKeyTable,
    IN ULONG dwOp
);

XBAPI XBOX_KEY_DATA XboxSignatureKey;

XBAPI XBOX_KEY_DATA XboxLANKey;

XBAPI XBOX_KRNL_VERSION XboxKrnlVersion;

XBAPI XBOX_HARDWARE_INFO XboxHardwareInfo;

XBAPI XBOX_KEY_DATA XboxHDKey;

XBAPI XBOX_KEY_DATA XboxEEPROMKey;

XBAPI XBOX_KEY_DATA XboxAlternateSignatureKeys[];

XBAPI VOID NTAPI WRITE_PORT_BUFFER_USHORT
(
    IN PUSHORT Port,
    IN PUSHORT Buffer,
    IN ULONG Count
);

XBAPI VOID NTAPI WRITE_PORT_BUFFER_ULONG
(
    IN PULONG Port,
    IN PULONG Buffer,
    IN ULONG Count
);

XBAPI VOID NTAPI WRITE_PORT_BUFFER_UCHAR
(
    IN PUCHAR Port,
    IN PUCHAR Buffer,
    IN ULONG Count
);

/**
 * Fills a specified memory area with zeroes
 * @param Destination A pointer to the memory block which is to be filled
 * @param Length The length of the memory block which is to be filled
 */
XBAPI VOID NTAPI RtlZeroMemory
(
    IN VOID UNALIGNED *Destination,
    IN SIZE_T Length
);

XBAPI ULONG NTAPI RtlWalkFrameChain
(
    OUT PVOID *Callers,
    IN ULONG Count,
    IN ULONG Flags
);

XBAPI VOID CDECL RtlVsprintf
(
    CHAR *,
    CONST CHAR*,
    ...
);

XBAPI VOID CDECL RtlVsnprintf
(
    CHAR *,
    SIZE_T,
    CONST CHAR*,
    ...
);

XBAPI VOID NTAPI RtlUpperString
(
    PSTRING DestinationString,
    PSTRING SourceString
);

/**
 * Returns the uppercased equivalent of a single character
 * @param Character The character which will be converted
 * @return The uppercased character
 */
XBAPI CHAR NTAPI RtlUpperChar
(
    CHAR Character
);

XBAPI NTSTATUS NTAPI RtlUpcaseUnicodeToMultiByteN
(
    PCHAR MultiByteString,
    ULONG MaxBytesInMultiByteString,
    PULONG BytesInMultiByteString,
    PWSTR UnicodeString,
    ULONG BytesInUnicodeString
);

XBAPI NTSTATUS NTAPI RtlUpcaseUnicodeString
(
    PUNICODE_STRING DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
);

XBAPI WCHAR NTAPI RtlUpcaseUnicodeChar
(
    WCHAR SourceCharacter
);

XBAPI VOID NTAPI RtlUnwind
(
    IN PVOID TargetFrame OPTIONAL,
    IN PVOID TargetIp OPTIONAL,
    IN PEXCEPTION_RECORD ExceptionRecord OPTIONAL,
    IN PVOID ReturnValue
);

XBAPI NTSTATUS NTAPI RtlUnicodeToMultiByteSize
(
    PULONG BytesInMultiByteString,
    PWSTR UnicodeString,
    ULONG BytesInUnicodeString
);

XBAPI NTSTATUS NTAPI RtlUnicodeToMultiByteN
(
    PCHAR MultiByteString,
    ULONG MaxBytesInMultiByteString,
    PULONG BytesInMultiByteString,
    PWSTR UnicodeString,
    ULONG BytesInUnicodeString
);

XBAPI NTSTATUS XBAPI RtlUnicodeStringToInteger
(
    PUNICODE_STRING String,
    ULONG Base,
    PULONG Value
);

XBAPI NTSTATUS NTAPI RtlUnicodeStringToAnsiString
(
    OUT PSTRING DestinationString,
    IN PUNICODE_STRING SourceString,
    IN BOOLEAN AllocateDestinationString
);

/**
 * Attempts to enter a critical section without blocking. If the call is successful, the calling thread takes ownership of the critical section.
 * @param CriticalSection A pointer to the critical section object.
 * @return If the critical section is successfully entered or the current thread already owns the critical section, the return value is nonzero. If another thread already owns the critical section, the return value is zero.
 */
XBAPI BOOLEAN NTAPI RtlTryEnterCriticalSection
(
    IN PRTL_CRITICAL_SECTION CriticalSection
);

/**
 * The RtlTimeToTimeFields routine converts system time into a TIME_FIELDS structure
 * @param Time Pointer to a buffer containing the absolute system time as a large integer, accurate to 100-nanosecond resolution
 * @param TimeFields Pointer to a caller-allocated buffer, which must be at least sizeof(TIME_FIELDS), to contain the returned information
 */
XBAPI VOID NTAPI RtlTimeToTimeFields
(
    IN PLARGE_INTEGER Time,
    OUT PTIME_FIELDS TimeFields
);

XBAPI BOOLEAN NTAPI RtlTimeFieldsToTime
(
    IN PTIME_FIELDS TimeFields,
    OUT PLARGE_INTEGER Time
);

XBAPI VOID CDECL RtlSprintf
(
    CHAR *,
    CONST CHAR *,
    ...
);

XBAPI VOID CDECL RtlSnprintf
(
    CHAR *,
    SIZE_T,
    CONST CHAR *,
    ...
);

XBAPI VOID NTAPI RtlRip
(
    IN PVOID ApiName,
    IN PVOID Expression,
    IN PVOID Message
);

XBAPI VOID NTAPI RtlRaiseStatus
(
    IN NTSTATUS Status
);

XBAPI VOID NTAPI RtlRaiseException
(
    IN PEXCEPTION_RECORD ExceptionRecord
);

XBAPI ULONG NTAPI RtlNtStatusToDosError
(
    IN NTSTATUS Status
);

XBAPI NTSTATUS NTAPI RtlMultiByteToUnicodeSize
(
    PULONG BytesInUnicodeString,
    PCHAR MultiByteString,
    ULONG BytesInMultiByteString
);

XBAPI NTSTATUS NTAPI RtlMultiByteToUnicodeN
(
    PWSTR UnicodeString,
    ULONG MaxBytesInUnicodeString,
    PULONG BytesInUnicodeString,
    PCHAR MultiByteString,
    ULONG BytesInMultiByteString
);

XBAPI VOID NTAPI RtlMoveMemory
(
    PVOID Destination,
    CONST PVOID *Source,
    ULONG Length
);

XBAPI VOID NTAPI RtlMapGenericMask
(
    PACCESS_MASK AccessMask,
    PGENERIC_MAPPING GenericMapping
);

/**
 * Returns the lowercase equivalent of a single character
 * @param Character The character which will be converted
 * @return The lowercased character
 */
XBAPI CHAR NTAPI RtlLowerChar
(
    IN CHAR Character
);

XBAPI VOID NTAPI RtlLeaveCriticalSectionAndRegion
(
    PRTL_CRITICAL_SECTION CriticalSection
);

/**
 * Releases ownership of the specified critical section object.
 * @param CriticalSection A pointer to the critical section object.
 */
XBAPI VOID NTAPI RtlLeaveCriticalSection
(
    IN PRTL_CRITICAL_SECTION CriticalSection
);

XBAPI NTSTATUS NTAPI RtlIntegerToUnicodeString
(
    ULONG Value,
    ULONG Base,
    PUNICODE_STRING String
);

XBAPI NTSTATUS NTAPI RtlIntegerToChar
(
    ULONG Value,
    ULONG Base,
    LONG OutputLength,
    PSZ String
);

/**
 * Initializes a critical section object.
 * @param CriticalSection A pointer to the critical section object.
 */
XBAPI VOID NTAPI RtlInitializeCriticalSection
(
    IN PRTL_CRITICAL_SECTION CriticalSection
);

XBAPI VOID NTAPI RtlInitUnicodeString
(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
);

XBAPI VOID NTAPI RtlInitAnsiString
(
    PANSI_STRING DestinationString,
    IN PCSZ SourceString
);

XBAPI VOID NTAPI RtlGetCallersAddress
(
    OUT PVOID *CallersAddress,
    OUT PVOID *CallersCaller
);

XBAPI VOID NTAPI RtlFreeUnicodeString
(
    PUNICODE_STRING UnicodeString
);

XBAPI VOID NTAPI RtlFreeAnsiString
(
    PANSI_STRING AnsiString
);

/**
 * Fills a specified memory area with repetitions of a ULONG value
 * @param Destination A pointer to the (ULONG-aligned) memory block which is to be filled
 * @param Length The length of the memory block which is to be filled
 * @param Fill The ULONG-value with which the memory block will be filled
 */
XBAPI VOID NTAPI RtlFillMemoryUlong
(
    PVOID Destination,
    SIZE_T Length,
    ULONG Pattern
);

/**
 * Fills a specified memory area with a specified value
 * @param Destination A pointer to the memory block which is to be filled
 * @param Length The length of the memory block which is to be filled
 * @param Fill The byte-value with which the memory block will be filled
 */
XBAPI VOID NTAPI RtlFillMemory
(
    PVOID Destination,
    ULONG Length,
    UCHAR Fill
);

XBAPI LARGE_INTEGER NTAPI RtlExtendedMagicDivide
(
    LARGE_INTEGER Dividend,
    LARGE_INTEGER MagicDivisor,
    CCHAR ShiftCount
);

XBAPI LARGE_INTEGER NTAPI RtlExtendedLargeIntegerDivide
(
    LARGE_INTEGER Dividend,
    ULONG Divisor,
    PULONG Remainder
);

XBAPI LARGE_INTEGER NTAPI RtlExtendedIntegerMultiply
(
    LARGE_INTEGER Multiplicand,
    LONG Multiplier
);

/**
 * Compares two counted unicode strings (UNICODE_STRING objects, NOT C-style strings!) for equality
 * @param String1 Pointer to the first unicode string
 * @param String2 Pointer to the second unicode string
 * @param CaseInSensitive Whether to ignore the case of the strings or not
 * @return TRUE if the strings are equal, FALSE if not
 */
XBAPI BOOLEAN NTAPI RtlEqualUnicodeString
(
    IN CONST PUNICODE_STRING String1,
    IN CONST PUNICODE_STRING String2,
    IN BOOLEAN CaseInSensitive
);

/**
 * Compares two counted strings (ANSI_STRING objects, NOT C-style strings!) for equality
 * @param String1 Pointer to the first string
 * @param String2 Pointer to the second string
 * @param CaseInSensitive Whether to ignore the case of the strings or not
 * @return TRUE if the strings are equal, FALSE if not
 */
XBAPI BOOLEAN NTAPI RtlEqualString
(
    IN CONST PSTRING String1,
    IN CONST PSTRING String2,
    IN BOOLEAN CaseInSensitive
);

XBAPI VOID NTAPI RtlEnterCriticalSectionAndRegion
(
    PRTL_CRITICAL_SECTION CriticalSection
);

/**
 * Waits for ownership of the specified critical section object. The function returns when the calling thread is granted ownership.
 * @param CriticalSection A pointer to the critical section object.
 */
XBAPI VOID NTAPI RtlEnterCriticalSection
(
    IN PRTL_CRITICAL_SECTION CriticalSection
);

XBAPI NTSTATUS NTAPI RtlDowncaseUnicodeString
(
    OUT PUNICODE_STRING DestinationString,
    IN PUNICODE_STRING SourceString,
    IN BOOLEAN AllocateDestinationString
);

XBAPI WCHAR NTAPI RtlDowncaseUnicodeChar
(
    WCHAR SourceCharacter
);

XBAPI BOOLEAN NTAPI RtlCreateUnicodeString
(
    OUT PUNICODE_STRING DestinationString,
    IN PCWSTR SourceString
);

XBAPI VOID NTAPI RtlCopyUnicodeString
(
    PUNICODE_STRING DestinationString,
    PUNICODE_STRING SourceString
);

XBAPI VOID NTAPI RtlCopyString
(
    OUT PSTRING DestinationString,
    IN PSTRING SourceString
);

/**
 * Compares two counted unicode strings (UNICODE_STRING objects, NOT C-style strings!)
 * @param String1 Pointer to the first string
 * @param String2 Pointer to the second string
 * @param CaseInSensitive Whether to ignore the case of the strings or not
 * @return Zero if the strings are equal, less than zero if String1 is less than String2, greater than zero if String1 is greater than String2
 */
XBAPI LONG NTAPI RtlCompareUnicodeString
(
    PUNICODE_STRING String1,
    PUNICODE_STRING String2,
    BOOLEAN CaseInSensitive
);

/**
 * Compares two counted strings (ANSI_STRING objects, NOT C-style strings!)
 * @param String1 Pointer to the first string
 * @param String2 Pointer to the second string
 * @param CaseInSensitive Whether to ignore the case of the strings or not
 * @return Zero if the strings are equal, less than zero if String1 is less than String2, greater than zero if String1 is greater than String2
 */
XBAPI LONG NTAPI RtlCompareString
(
    IN CONST PSTRING String1,
    IN CONST PSTRING String2,
    IN BOOLEAN CaseInSensitive
);

/**
 * Returns how many bytes in a block of memory match a specified pattern.
 * @param Source Pointer to a block of memory. Must be aligned on a ULONG boundary.
 * @param Length Number of bytes over which the comparison should be done. Must be a multiple of sizeof(ULONG).
 * @param Pattern Pattern to be compared byte by byte, repeatedly, through the specified memory range.
 * @return The number of bytes that were compared and found to be equal. If all bytes compare as equal, the "Length"-value is returned. If "Source" is not ULONG-aligned or if "Length" is not a multiple of sizeof(ULONG), zero is returned.
 */
XBAPI SIZE_T NTAPI RtlCompareMemoryUlong
(
    PVOID Source,
    SIZE_T Length,
    ULONG Pattern
);

/**
 * Compares two blocks of memory and returns the number of bytes that match.
 * @param Source1 A pointer to the first block of memory.
 * @param Source2 A pointer to the second block of memory.
 * @param Length The number of bytes to compare.
 * @return The number of bytes in the two blocks that match. If all bytes match, the "Length"-value is returned.
 */
XBAPI SIZE_T NTAPI RtlCompareMemory
(
    IN CONST VOID *Source1,
    IN CONST VOID *Source2,
    IN SIZE_T Length
);

/**
 * Converts a single-byte character string (C-style string, NOT an ANSI_STRING object!) to an integer value
 * @param String1 Pointer to a null-terminated single-byte string
 * @param Base Specifies the base (decimal, binary, octal, hexadecimal). If not given, the routine looks for prefixes in the given string (0x, 0o, 0b), default is decimal.
 * @param Value Pointer to a ULONG variable where the converted value will be stored
 * @return STATUS_SUCCESS if the string was successfully converted, STATUS_INVALID_PARAMETER otherwise
 */
XBAPI NTSTATUS NTAPI RtlCharToInteger
(
    IN PCSZ String,
    IN ULONG Base OPTIONAL,
    OUT PULONG Value
);

XBAPI USHORT NTAPI RtlCaptureStackBackTrace
(
    IN ULONG FramesToSkip,
    IN ULONG FramesToCapture,
    OUT PVOID *BackTrace,
    OUT PULONG BackTraceHash
);

XBAPI VOID NTAPI RtlCaptureContext
(
    OUT PCONTEXT ContextRecord
);

/**
 * Reports a failed assertion to an attached debugger.
 * @param FailedAssertion A string containing the expression that made the assertion fail
 * @param FileName A string containing the name of the file containing the assertion
 * @param LineNumber The linenumber containing the assertion
 * @param Message An optional message (will also be shown by the debugger)
 */
XBAPI VOID NTAPI RtlAssert
(
    IN PVOID FailedAssertion,
    IN PVOID FileName,
    IN ULONG LineNumber,
    IN PCHAR Message OPTIONAL
);

XBAPI NTSTATUS NTAPI RtlAppendUnicodeToString
(
    PUNICODE_STRING Destination,
    PCWSTR Source
);

XBAPI NTSTATUS NTAPI RtlAppendUnicodeStringToString
(
    PUNICODE_STRING Destination,
    PUNICODE_STRING Source
);

XBAPI NTSTATUS NTAPI RtlAppendStringToString
(
    IN PSTRING Destination,
    IN PSTRING Source
);

XBAPI NTSTATUS NTAPI RtlAnsiStringToUnicodeString
(
    PUNICODE_STRING DestinationString,
    PSTRING SourceString,
    BOOLEAN AllocateDestinationString
);

XBAPI VOID NTAPI READ_PORT_BUFFER_USHORT
(
    IN PUSHORT Port,
    OUT PUSHORT Buffer,
    IN ULONG Count
);

XBAPI VOID NTAPI READ_PORT_BUFFER_ULONG
(
    IN PULONG Port,
    OUT PULONG Buffer,
    IN PULONG Count
);

XBAPI VOID NTAPI READ_PORT_BUFFER_UCHAR
(
    IN PUCHAR Port,
    OUT PUCHAR Buffer,
    IN ULONG Count
);

XBAPI volatile OBJECT_TYPE PsThreadObjectType;

XBAPI VOID NTAPI __attribute__ ((noreturn)) PsTerminateSystemThread
(
    IN NTSTATUS ExitStatus
);

XBAPI NTSTATUS NTAPI PsSetCreateThreadNotifyRoutine
(
    IN PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine
);

XBAPI NTSTATUS NTAPI PsQueryStatistics
(
    IN OUT PPS_STATISTICS ProcessStatistics
);


XBAPI NTSTATUS NTAPI PsCreateSystemThreadEx
(
    OUT PHANDLE ThreadHandle,
    IN SIZE_T ThreadExtensionSize,
    IN SIZE_T KernelStackSize,
    IN SIZE_T TlsDataSize,
    OUT PHANDLE ThreadId OPTIONAL,
    IN PKSTART_ROUTINE StartRoutine,
    IN PVOID StartContext,
    IN BOOLEAN CreateSuspended,
    IN BOOLEAN DebuggerThread,
    IN PKSYSTEM_ROUTINE SystemRoutine OPTIONAL
);

XBAPI NTSTATUS NTAPI PsCreateSystemThread
(
    OUT PHANDLE ThreadHandle,
    OUT PHANDLE ThreadId OPTIONAL,
    IN PKSTART_ROUTINE StartRoutine,
    IN PVOID StartContext,
    IN BOOLEAN DebuggerThread
);

/**
 * Initialize the ethernet PHY
 * @param forceReset Whether to force a reset
 * @param param Optional parameters (seemingly unused)
 * @return Status code (zero on success)
 */
XBAPI NTSTATUS NTAPI PhyInitialize
(
    BOOLEAN forceReset,
    PVOID param OPTIONAL
);

/**
 * Read the status information from the NICs' registers
 * @param update
 * @return Flags describing the status of the NIC
 */
XBAPI DWORD NTAPI PhyGetLinkState
(
    BOOLEAN update
);

XBAPI volatile OBJECT_HANDLE_TABLE ObpObjectHandleTable;
XBAPI volatile OBJECT_TYPE ObSymbolicLinkObjectType;

XBAPI NTSTATUS NTAPI ObReferenceObjectByPointer
(
    IN PVOID Object,
    IN POBJECT_TYPE ObjectType
);

XBAPI NTSTATUS NTAPI ObReferenceObjectByName
(
    IN POBJECT_STRING ObjectName,
    IN ULONG Attributes,
    IN POBJECT_TYPE ObjectType,
    IN OUT PVOID ParseContext OPTIONAL,
    OUT PVOID *Object
);

XBAPI BOOLEAN NTAPI ObReferenceObjectByHandle
(
    IN HANDLE Handle,
    IN POBJECT_TYPE ObjectType OPTIONAL,
    OUT PVOID *ReturnedObject
);

XBAPI NTSTATUS NTAPI ObOpenObjectByPointer
(
    IN PVOID Object,
    IN POBJECT_TYPE ObjectType,
    OUT PHANDLE Handle
);

XBAPI NTSTATUS NTAPI ObOpenObjectByName
(
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN POBJECT_TYPE ObjectType,
    IN OUT PVOID ParseContext OPTIONAL,
    OUT PHANDLE Handle
);

XBAPI VOID NTAPI ObMakeTemporaryObject
(
    IN PVOID Object
);

XBAPI NTSTATUS NTAPI ObInsertObject
(
    IN PVOID Object,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN ULONG ObjectPointerBias,
    OUT PHANDLE Handle
);

XBAPI volatile OBJECT_TYPE ObDirectoryObjectType;

XBAPI NTSTATUS NTAPI ObCreateObject
(
    IN POBJECT_TYPE ObjectType,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN ULONG ObjectBodySize,
    OUT PVOID *Object
);

/**
 * yields execution of the current thread for one timeslice
 */
XBAPI NTSTATUS NTAPI NtYieldExecution(void);

XBAPI BOOLEAN NTAPI NtWriteFileGather
(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PFILE_SEGMENT_ELEMENT SegmentArray,
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL
);

XBAPI NTSTATUS NTAPI NtWriteFile
(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID Buffer,
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL
);

/**
 * Waits until the specified object attains a state of "signaled". Also see NtWaitForSingleObject.
 * @param Handle The handle to the wait object.
 * @param WaitMode Specifies the processor mode in which the wait is to occur.
 * @param Alertable Specifies whether an alert can be delivered when the object is waiting (watch out for the STATUS_ALERTED return value).
 * @param Timeout An optional (set to NULL when not used) pointer to an absolute or relative time over which the wait is to occur. If an explicit timeout value of zero is specified, then no wait occurs if the wait cannot be satisfied immediately.
 * @return The wait completion status. Can be STATUS_SUCCESS (specified object satisifed the wait), STATUS_TIMEOUT (a timeout occured), STATUS_ALERTED (the wait was aborted to deliver an alert to the current thread) or STATUS_USER_APC (the wait was aborted to deliver a user APC to the current thread).
 */
XBAPI NTSTATUS NTAPI NtWaitForSingleObjectEx
(
    IN HANDLE Handle,
    IN KPROCESSOR_MODE WaitMode,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
);

/**
 * Waits until the specified object attains a state of "signaled". When used with a semaphore, the semaphore gets decremented and the threaad continues, or the thread waits until the semaphore-count becomes non-zero and then decrements the seamphore count and continues.
 * @param Handle The handle to the wait object.
 * @param Alertable Specifies whether an alert can be delivered when the object is waiting (watch out for the STATUS_ALERTED return value).
 * @param Timeout An optional (set to NULL when not used) pointer to an absolute or relative time over which the wait is to occur. If an explicit timeout value of zero is specified, then no wait occurs if the wait cannot be satisfied immediately.
 * @return The wait completion status. Can be STATUS_SUCCESS (specified object satisifed the wait), STATUS_TIMEOUT (a timeout occured), STATUS_ALERTED (the wait was aborted to deliver an alert to the current thread) or STATUS_USER_APC (the wait was aborted to deliver a user APC to the current thread).
 */
XBAPI NTSTATUS NTAPI NtWaitForSingleObject
(
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
);

/**
 * Waits until the specified objects attain a state of "signaled". The wait can be specified to either wait until all objects are signaled or until one of the objects is signaled. Also see NtWaitForSingleObject.
 * @param Count Specifies the number of objects that are to be waited on.
 * @param Handles An array of handles to wait objects.
 * @param WaitType Specifies the type of wait to perform (WaitAll or WaitAny)
 * @param WaitMode Specifies the processor mode in which the wait is to occur.
 * @param Alertable Specifies whether an alert can be delivered when the object is waiting (watch out for the STATUS_ALERTED return value).
 * @param Timeout An optional (set to NULL when not used) pointer to an absolute or relative time over which the wait is to occur. If an explicit timeout value of zero is specified, then no wait occurs if the wait cannot be satisfied immediately.
 * @return The wait completion status. The index of the object in the array that satisfied the wait is returned. Can also be STATUS_TIMEOUT (a timeout occured), STATUS_ALERTED (the wait was aborted to deliver an alert to the current thread) or STATUS_USER_APC (the wait was aborted to deliver a user APC to the current thread).
 */
XBAPI NTSTATUS NTAPI NtWaitForMultipleObjectsEx
(
    IN ULONG Count,
    IN HANDLE Handles[],
    IN WAIT_TYPE WaitType,
    IN KPROCESSOR_MODE WaitMode,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
);

XBAPI VOID NTAPI NtUserIoApcDispatcher
(
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
);

/**
 * Suspends the target thread and optionally returns the previous suspend count.
 * @param The handle of the thread object to suspend.
 * @param PreviousSuspendCount Optional pointer to a variable that receives the thread's previous suspend count.
 * @return The status of the operation.
 */
XBAPI NTSTATUS NTAPI NtSuspendThread
(
    IN HANDLE ThreadHandle,
    OUT PULONG PreviousSuspendCount OPTIONAL
);

XBAPI NTSTATUS NTAPI NtSignalAndWaitForSingleObjectEx
(
    IN HANDLE SignalHandle,
    IN HANDLE WaitHandle,
    IN KPROCESSOR_MODE WaitMode,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
);

XBAPI NTSTATUS NTAPI NtSetTimerEx
(
    IN HANDLE TimerHandle,
    IN PLARGE_INTEGER DueTime,
    IN PTIMER_APC_ROUTINE TimerApcRoutine OPTIONAL,
    IN KPROCESSOR_MODE ApcMode,
    IN PVOID TimerContext OPTIONAL,
    IN BOOLEAN ResumeTimer,
    IN LONG Period OPTIONAL,
    OUT PBOOLEAN PreviousState OPTIONAL
);

/**
 * Sets the current system time and optionally returns the old system time.
 * @param SystemTime A pointer to the new value for the system time.
 * @param PreviousTime An optional pointer to a variable that receives the previous system time.
 * @return The status of the operation. STATUS_SUCCESS when successfull, STATUS_ACCESS_VIOLATION if the input parameter cannot be read or the output cannot be written, STATUS_INVALID_PARAMETER if the input time is negative.
 */
XBAPI NTSTATUS NTAPI NtSetSystemTime
(
    IN PLARGE_INTEGER SystemTime,
    OUT PLARGE_INTEGER PreviousTime OPTIONAL
);

XBAPI NTSTATUS NTAPI NtSetIoCompletion
(
    IN HANDLE IoCompletionHandle,
    IN PVOID KeyContext,
    IN PVOID ApcContext,
    IN NTSTATUS IoStatus,
    IN ULONG_PTR IoStatusInformation
);

XBAPI NTSTATUS NTAPI NtSetInformationFile
(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass
);

XBAPI NTSTATUS NTAPI NtSetEvent
(
    IN HANDLE EventHandle,
    OUT PLONG PreviousState OPTIONAL
);

/**
 * Resumes the target thread (see NtSuspendThread) and optionally returns the previous suspend count.
 * @param The handle of the thread object to resume.
 * @param PreviousSuspendCount Optional pointer to a variable that receives the thread's previous suspend count.
 * @return The status of the operation.
 */
XBAPI NTSTATUS NTAPI NtResumeThread
(
    IN HANDLE ThreadHandle,
    OUT PULONG PreviousSuspendCount OPTIONAL
);

XBAPI NTSTATUS NTAPI NtRemoveIoCompletion
(
    IN HANDLE IoCompletionHandle,
    OUT PVOID *KeyContext,
    OUT PVOID *ApcContext,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER Timeout
);

/**
 * Releases a semaphore object. When the semaphore is released, the current count of the semaphore is incremented by "ReleaseCount". Any threads that are waiting for the semaphore are examined to see if the current semaphore value is sufficient to satisfy their wait. If the value specified by "ReleaseCount" would cause the maximum count for the semaphore to be exceeded, then the count for the semaphore is not affected and an error status is returned.
 * @param SemaphoreHandle An open handle to a semaphore object.
 * @param ReleaseCount The release count for the semaphore. The count must be greater than zero and less than the maximum value specified for the semaphore.
 * @param PreviousCount An optional pointer to a variable that receives the previous count for the semaphore.
 * @return The status of the release operation, STATUS_SUCCESS on success.
 */
XBAPI NTSTATUS NTAPI NtReleaseSemaphore
(
    IN HANDLE SemaphoreHandle,
    IN LONG ReleaseCount,
    OUT PLONG PreviousCount OPTIONAL
);

/**
 * Releases a mutant object (mutex).
 * @param MutantHandle The handle to the mutant object.
 * @param PreviousCount An optional pointer to a variable that receives the previous mutant count.
 * @return The status of the operation.
 */
XBAPI NTSTATUS NTAPI NtReleaseMutant
(
    IN HANDLE MutantHandle,
    OUT PLONG PreviousCount OPTIONAL
);

XBAPI NTSTATUS NTAPI NtReadFileScatter
(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PFILE_SEGMENT_ELEMENT SegmentArray,
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL
);

XBAPI NTSTATUS NTAPI NtReadFile
(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID Buffer,
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL
);

XBAPI NTSTATUS NTAPI NtQueueApcThread
(
    IN HANDLE ThreadHandle,
    IN PPS_APC_ROUTINE ApcRoutine,
    IN PVOID ApcArgument1,
    IN PVOID ApcArgument2,
    IN PVOID ApcArgument3
);

XBAPI NTSTATUS NTAPI NtQueryVolumeInformationFile
(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FsInformation,
    IN ULONG Length,
    IN FS_INFORMATION_CLASS FsInformationClass
);

XBAPI NTSTATUS NTAPI NtQueryVirtualMemory
(
    IN PVOID BaseAddress,
    OUT PMEMORY_BASIC_INFORMATION MemoryInformation
);

XBAPI NTSTATUS NTAPI NtQueryTimer
(
    IN HANDLE TimerHandle,
    OUT PTIMER_BASIC_INFORMATION TimerInformation
);

XBAPI NTSTATUS NTAPI NtQuerySymbolicLinkObject
(
    IN HANDLE LinkHandle,
    IN OUT POBJECT_STRING LinkTarget,
    OUT PULONG ReturnedLength OPTIONAL
);

/**
 * Queries the state of a semaphore object.
 * @param SemaphoreHandle An open handle to a semaphore object.
 * @param SemaphoreInformation A pointer to a buffer (SEMAPHORE_BASIC_INFORMATION-structure) that receives the queried information.
 * @return The status code of the query operation, STATUS_SUCCESS on success.
 */
XBAPI NTSTATUS NTAPI NtQuerySemaphore
(
    IN HANDLE SemaphoreHandle,
    OUT PSEMAPHORE_BASIC_INFORMATION SemaphoreInformation
);

/**
 * Queries the state of a mutant object.
 * @param MutantHandle A handle to a mutant object.
 * @param MutantInformation A pointer to a MUTANT_BASIC_INFORMATION-structure that receives the requested information.
 * @return The status of the operation.
 */
XBAPI NTSTATUS NTAPI NtQueryMutant
(
    IN HANDLE MutantHandle,
    OUT PMUTANT_BASIC_INFORMATION MutantInformation
);

XBAPI NTSTATUS NTAPI NtQueryIoCompletion
(
    IN HANDLE IoCompletionHandle,
    OUT PIO_COMPLETION_BASIC_INFORMATION IoCompletionInformation
);

XBAPI NTSTATUS NTAPI NtQueryInformationFile
(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass
);

XBAPI NTSTATUS NTAPI NtQueryFullAttributesFile
(
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PFILE_NETWORK_OPEN_INFORMATION FileInformation
);

XBAPI NTSTATUS NTAPI NtQueryEvent
(
    IN HANDLE EventHandle,
    OUT PEVENT_BASIC_INFORMATION EventInformation
);

XBAPI NTSTATUS NTAPI NtQueryDirectoryObject
(
    IN HANDLE DirectoryHandle,
    OUT PVOID Buffer,
    IN ULONG Length,
    IN BOOLEAN RestartScan,
    IN OUT PULONG Context,
    OUT PULONG ReturnLength OPTIONAL
);

XBAPI NTSTATUS NTAPI NtQueryDirectoryFile
(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass,
    IN POBJECT_STRING FileName OPTIONAL,
    IN BOOLEAN RestartScan
);

XBAPI NTSTATUS NTAPI NtPulseEvent
(
    IN HANDLE EventHandle,
    OUT PLONG PreviousState OPTIONAL
);

XBAPI NTSTATUS NTAPI NtProtectVirtualMemory
(
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN ULONG NewProtect,
    OUT PULONG OldProtect
);

XBAPI NTSTATUS NTAPI NtOpenSymbolicLinkObject
(
    OUT PHANDLE LinkHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes
);

XBAPI NTSTATUS NTAPI NtOpenFile
(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG ShareAccess,
    IN ULONG OpenOptions
);

XBAPI NTSTATUS NTAPI NtOpenDirectoryObject
(
    OUT PHANDLE DirectoryHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes
);

XBAPI NTSTATUS NTAPI NtFsControlFile
(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG FsControlCode,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
);

XBAPI NTSTATUS NTAPI NtFreeVirtualMemory
(
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN ULONG FreeType
);

XBAPI NTSTATUS NTAPI NtFlushBuffersFile
(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock
);

XBAPI NTSTATUS NTAPI NtDuplicateObject
(
    IN HANDLE SourceHandle,
    OUT PHANDLE TargetHandle,
    IN ULONG Options
);

XBAPI NTSTATUS NTAPI NtDeviceIoControlFile
(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
);


XBAPI BOOLEAN NTAPI NtDeleteFile
(
    IN POBJECT_ATTRIBUTES ObjectAttributes
);

XBAPI NTSTATUS NTAPI NtCreateTimer
(
    OUT PHANDLE TimerHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN TIMER_TYPE TimerType
);

/**
 * Create a semaphore object with the specified initial and maximum count.
 * @param SemaphoreHandle A pointer to a variable that receives the value of the semaphore object handle.
 * @param ObjectAttributes An optional pointer to a structure that specifies the object's attributes.
 * @param InitialCount The initial count for the semaphore, this value must be positive and less than or equal to the maximum count.
 * @param MaximumCount The maximum count for the semaphore, this value must be greater than zero.
 * @return STATUS_SUCCESS on success or error code.
 */
XBAPI NTSTATUS NTAPI NtCreateSemaphore
(
    OUT PHANDLE SemaphoreHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN LONG InitialCount,
    IN LONG MaximumCount
);

/**
 * Creates a mutant object (mutex), sets its initial count to one (which means "signaled"), and opens a handle to the object.
 * @param MutantHandle A pointer to a variable that receives the mutant object handle.
 * @param ObjectAttributes A pointer to a OBJECT_ATTRIBUTES-structure that specifies object attributes.
 * @pararm InitialOwner A boolean value that specifies whether the creator of the mutant object wants immediate ownership.
 * @return The status of the operation.
 */
XBAPI NTSTATUS NTAPI NtCreateMutant
(
    OUT PHANDLE MutantHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN BOOLEAN InitialOwner
);

XBAPI NTSTATUS NTAPI NtCreateIoCompletion
(
    OUT PHANDLE IoCompletionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN ULONG Count OPTIONAL
);

XBAPI NTSTATUS NTAPI NtCreateFile
(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions
);


/* values for CreateDisposition */
/*
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5
*/

/* values for DesiredAccess */
#define DELETE 0x00010000L
#define READ_CONTROL 0x00020000L
#define WRITE_DAC 0x00040000L
#define WRITE_OWNER 0x00080000L
#define SYNCHRONIZE 0x00100000L
#define STANDARD_RIGHTS_REQUIRED 0x000F0000L
#define STANDARD_RIGHTS_READ READ_CONTROL
#define STANDARD_RIGHTS_WRITE READ_CONTROL
#define STANDARD_RIGHTS_EXECUTE READ_CONTROL
#define STANDARD_RIGHTS_ALL 0x001F0000L
#define SPECIFIC_RIGHTS_ALL 0x0000FFFFL
#define ACCESS_SYSTEM_SECURITY 0x01000000L
#define MAXIMUM_ALLOWED 0x02000000L
#define GENERIC_READ 0x80000000L
#define GENERIC_WRITE 0x40000000L
#define GENERIC_EXECUTE 0x20000000L
#define GENERIC_ALL 0x10000000L

#define FILE_READ_DATA 0x0001
#define FILE_LIST_DIRECTORY 0x0001
#define FILE_WRITE_DATA 0x0002
#define FILE_ADD_FILE 0x0002
#define FILE_APPEND_DATA 0x0004
#define FILE_ADD_SUBDIRECTORY 0x0004
#define FILE_CREATE_PIPE_INSTANCE 0x0004
#define FILE_READ_EA 0x0008
#define FILE_WRITE_EA 0x0010
#define FILE_EXECUTE 0x0020
#define FILE_TRAVERSE 0x0020
#define FILE_DELETE_CHILD 0x0040
#define FILE_READ_ATTRIBUTES 0x0080
#define FILE_WRITE_ATTRIBUTES 0x0100
#define FILE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF)

#define FILE_GENERIC_READ (STANDARD_RIGHTS_READ | FILE_READ_DATA | FILE_READ_ATTRIBUTES | FILE_READ_EA | SYNCHRONIZE)
#define FILE_GENERIC_WRITE (STANDARD_RIGHTS_WRITE | FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA | FILE_APPEND_DATA | SYNCHRONIZE)
#define FILE_GENERIC_EXECUTE (STANDARD_RIGHTS_EXECUTE | FILE_READ_ATTRIBUTES | FILE_EXECUTE | SYNCHRONIZE)

/* values for FileAttributes */
#define FILE_ATTRIBUTE_READONLY 0x00000001
#define FILE_ATTRIBUTE_HIDDEN 0x00000002
#define FILE_ATTRIBUTE_SYSTEM 0x00000004
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_ARCHIVE 0x00000020
#define FILE_ATTRIBUTE_DEVICE 0x00000040
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define FILE_ATTRIBUTE_TEMPORARY 0x00000100

/* values for CreateOptions */
#define FILE_DIRECTORY_FILE 0x00000001
#define FILE_WRITE_THROUGH 0x00000002
#define FILE_SEQUENTIAL_ONLY 0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING 0x00000008
#define FILE_SYNCHRONOUS_IO_ALERT 0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT 0x00000020
#define FILE_NON_DIRECTORY_FILE 0x00000040
#define FILE_CREATE_TREE_CONNECTION 0x00000080
#define FILE_COMPLETE_IF_OPLOCKED 0x00000100
#define FILE_NO_EA_KNOWLEDGE 0x00000200
#define FILE_OPEN_FOR_RECOVERY 0x00000400
#define FILE_RANDOM_ACCESS 0x00000800
#define FILE_DELETE_ON_CLOSE 0x00001000
#define FILE_OPEN_BY_FILE_ID 0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT 0x00004000
#define FILE_NO_COMPRESSION 0x00008000
#define FILE_RESERVE_OPFILTER 0x00100000
#define FILE_OPEN_REPARSE_POINT 0x00200000
#define FILE_OPEN_NO_RECALL 0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY 0x00800000
#define FILE_COPY_STRUCTURED_STORAGE 0x00000041
#define FILE_STRUCTURED_STORAGE 0x00000441
#define FILE_VALID_OPTION_FLAGS 0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS 0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS 0x00000032
#define FILE_VALID_SET_FLAGS 0x00000036

#define FILE_SUPERSEDE 0x00000000
#define FILE_OPEN 0x00000001
#define FILE_CREATE 0x00000002
#define FILE_OPEN_IF 0x00000003
#define FILE_OVERWRITE 0x00000004
#define FILE_OVERWRITE_IF 0x00000005
#define FILE_MAXIMUM_DISPOSITION 0x00000005

/* values of the IoStatusBlock */
#define FILE_SUPERSEDED 0x00000000
#define FILE_OPENED 0x00000001
#define FILE_CREATED 0x00000002
#define FILE_OVERWRITTEN 0x00000003
#define FILE_EXISTS 0x00000004
#define FILE_DOES_NOT_EXIST 0x00000005

XBAPI NTSTATUS NTAPI NtCreateEvent
(
    OUT PHANDLE EventHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN EVENT_TYPE EventType,
    IN BOOLEAN InitialState
);

XBAPI NTSTATUS NTAPI NtCreateDirectoryObject
(
    OUT PHANDLE DirectoryHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes
);

/**
 * Closes an object handle.
 * @param Handle Handle to an object
 * @return STATE_SUCCESS on success, error code (e.g. STATUS_INVALID_HANDLE, STATUS_HANDLE_NOT_CLOSABLE) otherwise
 */
XBAPI NTSTATUS NTAPI NtClose
(
    IN HANDLE Handle
);

XBAPI NTSTATUS NTAPI NtClearEvent
(
    IN HANDLE EventHandle
);

XBAPI NTSTATUS NTAPI NtCancelTimer
(
    IN HANDLE TimerHandle,
    OUT PBOOLEAN CurrentState OPTIONAL
);

XBAPI NTSTATUS NTAPI NtAllocateVirtualMemory
(
    IN OUT PVOID *BaseAddress,
    IN ULONG_PTR ZeroBits,
    IN OUT PSIZE_T RegionSize,
    IN ULONG AllocationType,
    IN ULONG Protect
);

/**
 * Flags for NtAllocateVirtualMemory
 */
#define PAGE_NOACCESS 0x01
#define PAGE_READONLY 0x02
#define PAGE_READWRITE 0x04
#define PAGE_WRITECOPY 0x08
#define PAGE_EXECUTE 0x10
#define PAGE_EXECUTE_READ 0x20
#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_EXECUTE_WRITECOPY 0x80
#define PAGE_GUARD 0x100
#define PAGE_NOCACHE 0x200
#define PAGE_WRITECOMBINE 0x400
#define PAGE_VIDEO 0x0
#define PAGE_OLD_VIDEO 0x800
#define MEM_COMMIT 0x1000
#define MEM_RESERVE 0x2000
#define MEM_DECOMMIT 0x4000
#define MEM_RELEASE 0x8000
#define MEM_FREE 0x10000
#define MEM_PRIVATE 0x20000
#define MEM_MAPPED 0x40000
#define MEM_RESET 0x80000
#define MEM_TOP_DOWN 0x100000
#define MEM_NOZERO 0x800000
#define MEM_LARGE_PAGES 0x20000000
#define MEM_4MB_PAGES 0x80000000

XBAPI PVOID NTAPI MmUnmapIoSpace
(
    IN PVOID BaseAddress,
    IN SIZE_T NumberOfBytes
);

XBAPI VOID NTAPI MmSetAddressProtect
(
    IN PVOID BaseAddress,
    IN ULONG NumberOfBytes,
    IN ULONG NewProtect
);

XBAPI NTSTATUS NTAPI MmQueryStatistics
(
    IN OUT PMM_STATISTICS MemoryStatistics
);

XBAPI SIZE_T NTAPI MmQueryAllocationSize
(
    IN PVOID BaseAddress
);

XBAPI ULONG NTAPI MmQueryAddressProtect
(
    IN PVOID VirtualAddress
);

/**
 * Marks a contiguous area of memory to be preserved across a quick reboot.
 * @param BaseAddress The virtual address of the memory area.
 * @param NumberOfBytes The number of bytes to be preserved.
 * @param Persist TRUE if the memory should be persistent, else FALSE.
 */
XBAPI VOID NTAPI MmPersistContiguousMemory
(
    IN PVOID BaseAddress,
    IN SIZE_T NumberOfBytes,
    IN BOOLEAN Persist
);

XBAPI PVOID NTAPI MmMapIoSpace
(
    IN ULONG_PTR PhysicalAddress,
    IN SIZE_T NumberOfBytes,
    IN ULONG Protect
);

XBAPI VOID NTAPI MmLockUnlockPhysicalPage
(
    IN ULONG_PTR PhysicalAddress,
    IN BOOLEAN UnlockPage
);

XBAPI VOID NTAPI MmLockUnlockBufferPages
(
    IN PVOID BaseAddress,
    IN SIZE_T NumberOfBytes,
    IN BOOLEAN UnlockPages
);

/**
 * Checks whether a page fault would occur for a read operation on a specified address.
 * @param VirtualAddress The virtual address to be checked.
 * @return TRUE if a page fault would occur, FALSE if not.
 */
XBAPI BOOLEAN NTAPI MmIsAddressValid
(
    IN PVOID VirtualAddress
);

XBAPI volatile PMMGLOBALDATA MmGlobalData;

/**
 * Returns the physical address for a virtual address.
 * @param BaseAddress A valid virtual address for which the physical address is to be returned.
 * @return The corresponding physical address.
 */
XBAPI ULONG_PTR NTAPI MmGetPhysicalAddress
(
    IN PVOID BaseAddress
);

XBAPI ULONG NTAPI MmFreeSystemMemory
(
    IN PVOID BaseAddress,
    IN SIZE_T NumberOfBytes
);

XBAPI VOID NTAPI MmFreeContiguousMemory
(
    IN PVOID BaseAddress
);

XBAPI VOID NTAPI MmDeleteKernelStack
(
    IN PVOID KernelStackBase,
    IN PVOID KernelStackLimit
);

XBAPI PVOID NTAPI MmDbgWriteCheck
(
    IN PVOID VirtualAddress,
    IN PHARDWARE_PTE Opaque
);

XBAPI VOID NTAPI MmDbgReleaseAddress
(
    IN PVOID VirtualAddress,
    IN PHARDWARE_PTE Opaque
);

XBAPI PFN_COUNT NTAPI MmDbgQueryAvailablePages (void);

XBAPI ULONG NTAPI MmDbgFreeMemory
(
    IN PVOID BaseAddress,
    IN SIZE_T NumberOfBytes
);

XBAPI PVOID NTAPI MmDbgAllocateMemory
(
    IN SIZE_T NumberOfBytes,
    IN ULONG Protect
);

XBAPI PVOID NTAPI MmCreateKernelStack
(
    IN SIZE_T NumberOfBytes,
    IN BOOLEAN DebuggerThread
);

XBAPI PVOID NTAPI MmClaimGpuInstanceMemory
(
    IN SIZE_T NumberOfBytes,
    OUT SIZE_T *NumberOfPaddingBytes
);

XBAPI PVOID NTAPI MmAllocateSystemMemory
(
    IN SIZE_T NumberOfBytes,
    IN ULONG Protect
);

XBAPI PVOID NTAPI MmAllocateContiguousMemoryEx
(
    IN SIZE_T NumberOfBytes,
    IN ULONG_PTR LowestAcceptableAddress,
    IN ULONG_PTR HighestAcceptableAddress,
    IN ULONG_PTR Alignment,
    IN ULONG Protect
);

XBAPI PVOID NTAPI MmAllocateContiguousMemory
(
    IN SIZE_T NumberOfBytes
);

XBAPI PLAUNCH_DATA_PAGE LaunchDataPage;

XBAPI volatile ULONG KiBugCheckData[];

XBAPI NTSTATUS NTAPI KeWaitForSingleObject
(
    IN PVOID Object,
    IN KWAIT_REASON WaitReason,
    IN KPROCESSOR_MODE WaitMode,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
);

XBAPI NTSTATUS NTAPI KeWaitForMultipleObjects
(
    IN ULONG Count,
    IN PVOID Object[],
    IN WAIT_TYPE WaitType,
    IN KWAIT_REASON WaitReason,
    IN KPROCESSOR_MODE WaitMode,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL,
    IN PKWAIT_BLOCK WaitBlockArray
);

XBAPI volatile CONST ULONG KeTimeIncrement;

/**
 * Contains the number of milliseconds elapsed since the system was started.
 */
XBAPI volatile DWORD KeTickCount;

XBAPI BOOLEAN NTAPI KeTestAlertThread
(
    IN KPROCESSOR_MODE ProcessorMode
);

XBAPI volatile KSYSTEM_TIME KeSystemTime;

XBAPI BOOLEAN NTAPI KeSynchronizeExecution
(
    IN PKINTERRUPT Interrupt,
    IN PKSYNCHRONIZE_ROUTINE SynchronizeRoutine,
    IN PVOID SynchronizeContext
);

XBAPI ULONG NTAPI KeSuspendThread
(
    IN PKTHREAD Thread
);

/**
 * Stalls the caller on the current processor for a specified time Interval.
 * @param MicroSeconds Specifies the number of microseconds to stall.
 */
XBAPI VOID NTAPI KeStallExecutionProcessor
(
    IN ULONG MicroSeconds
);

XBAPI BOOLEAN NTAPI KeSetTimerEx
(
    IN PKTIMER Timer,
    IN LARGE_INTEGER DueTime,
    IN LONG Period OPTIONAL,
    IN PKDPC Dpc OPTIONAL
);

XBAPI BOOLEAN NTAPI KeSetTimer
(
    IN PKTIMER Timer,
    IN LARGE_INTEGER DueTime,
    IN PKDPC Dpc OPTIONAL
);

/**
 * Sets the run-time priority of a thread.
 * @param Thread Pointer to the thread.
 * @param Priority Specifies the priority of the thread, usually to the real-time priority value, LOW_REALTIME_PRIORITY. The value LOW_PRIORITY is reserved for system use.
 * @return Returns the old priority of the thread.
 */
XBAPI KPRIORITY NTAPI KeSetPriorityThread
(
    IN PKTHREAD Thread,
    IN KPRIORITY Priority
);

XBAPI KPRIORITY NTAPI KeSetPriorityProcess
(
    IN PKPROCESS Process,
    IN KPRIORITY BasePriority
);

XBAPI VOID NTAPI KeSetEventBoostPriority
(
    IN PRKEVENT Event,
    IN PRKTHREAD *Thread OPTIONAL
);

XBAPI LONG NTAPI KeSetEvent
(
    IN PRKEVENT Event,
    IN KPRIORITY Increment,
    IN BOOLEAN Wait
);

XBAPI LOGICAL NTAPI KeSetDisableBoostThread
(
    IN PKTHREAD Thread,
    IN LOGICAL Disable
);

XBAPI LONG NTAPI KeSetBasePriorityThread
(
    IN PKTHREAD Thread,
    IN LONG Increment
);

XBAPI NTSTATUS NTAPI KeSaveFloatingPointState
(
    OUT PKFLOATING_SAVE FloatSave
);

XBAPI PLIST_ENTRY NTAPI KeRundownQueue
(
    IN PRKQUEUE Queue
);

XBAPI ULONG NTAPI KeResumeThread
(
    IN PKTHREAD Thread
);

XBAPI NTSTATUS KeRestoreFloatingPointState
(
    IN PKFLOATING_SAVE FloatSave
);

XBAPI LONG NTAPI KeResetEvent
(
    IN PRKEVENT Event
);

XBAPI BOOLEAN NTAPI KeRemoveQueueDpc
(
    IN PRKDPC Dpc
);

XBAPI PLIST_ENTRY NTAPI KeRemoveQueue
(
    IN PRKQUEUE Queue,
    IN KPROCESSOR_MODE WaitMode,
    IN PLARGE_INTEGER Timeout OPTIONAL
);

XBAPI BOOLEAN NTAPI KeRemoveEntryDeviceQueue
(
    IN PKDEVICE_QUEUE DeviceQueue,
    IN PKDEVICE_QUEUE DeviceQueueEntry
);

XBAPI PKDEVICE_QUEUE_ENTRY NTAPI KeRemoveDeviceQueue
(
    IN PKDEVICE_QUEUE DeviceQueue
);

XBAPI PKDEVICE_QUEUE_ENTRY NTAPI KeRemoveByKeyDeviceQueue
(
    IN PKDEVICE_QUEUE DeviceQueue,
    IN ULONG SortKey
);

XBAPI LONG NTAPI KeReleaseSemaphore
(
    IN PRKSEMAPHORE Semaphore,
    IN KPRIORITY Increment,
    IN LONG Adjustment,
    IN BOOLEAN Wait
);

XBAPI LONG NTAPI KeReleaseMutant
(
    IN PRKMUTANT Mutant,
    IN KPRIORITY Increment,
    IN BOOLEAN Abandoned,
    IN BOOLEAN Wait
);

XBAPI KIRQL NTAPI KeRaiseIrqlToSynchLevel (void);
XBAPI KIRQL NTAPI KeRaiseIrqlToDpcLevel (void);

/**
 * Obtains the current system time.
 * @param CurrentTime The system time in 100-nanosecond intervals since January 1, 1601, in GMT.
 */
XBAPI VOID NTAPI KeQuerySystemTime
(
    OUT PLARGE_INTEGER CurrentTime
);

XBAPI ULONGLONG NTAPI KeQueryPerformanceFrequency(void);

XBAPI ULONGLONG NTAPI KeQueryPerformanceCounter(void);

XBAPI ULONGLONG NTAPI KeQueryInterruptTime (void);

XBAPI LONG NTAPI KeQueryBasePriorityThread
(
    IN PKTHREAD Thread
);

XBAPI LONG NTAPI KePulseEvent
(
    IN PRKEVENT Event,
    IN KPRIORITY Increment,
    IN BOOLEAN Wait
);

XBAPI VOID NTAPI KeLeaveCriticalRegion (void);

XBAPI BOOLEAN NTAPI KeIsExecutingDpc (void);

XBAPI volatile KSYSTEM_TIME KeInterruptTime;

XBAPI BOOLEAN NTAPI KeInsertQueueDpc
(
    IN PRKDPC Dpc,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
);

XBAPI BOOLEAN NTAPI KeInsertQueueApc
(
    IN PRKAPC Apc,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2,
    IN KPRIORITY Increment
);

XBAPI LONG NTAPI KeInsertQueue
(
    IN PRKQUEUE Queue,
    IN PLIST_ENTRY Entry
);

XBAPI LONG NTAPI KeInsertHeadQueue
(
    IN PRKQUEUE Queue,
    IN PLIST_ENTRY Entry
);

XBAPI BOOLEAN NTAPI KeInsertDeviceQueue
(
    IN PKDEVICE_QUEUE DeviceQueue,
    IN PKDEVICE_QUEUE_ENTRY DeviceQueueEntry
);

XBAPI BOOLEAN NTAPI KeInsertByKeyDeviceQueue
(
    IN PKDEVICE_QUEUE DeviceQueue,
    IN PKDEVICE_QUEUE_ENTRY DeviceQueueEntry,
    IN ULONG SortKey
);

XBAPI VOID NTAPI KeInitializeTimerEx
(
    IN PKTIMER Timer,
    IN TIMER_TYPE Type
);

XBAPI VOID NTAPI KeInitializeSemaphore
(
    IN PRKSEMAPHORE Semaphore,
    IN LONG Count,
    IN LONG Limit
);

XBAPI VOID NTAPI KeInitializeQueue
(
    IN PRKQUEUE Queue,
    IN ULONG Count OPTIONAL
);

XBAPI VOID NTAPI KeInitializeMutant
(
    IN PRKMUTANT Mutant,
    IN BOOLEAN InitialOwner
);

XBAPI VOID NTAPI KeInitializeInterrupt
(
    IN PKINTERRUPT Interrupt,
    IN PKSERVICE_ROUTINE ServiceRoutine,
    IN PVOID ServiceContext,
    IN ULONG Vector,
    IN KIRQL Irql,
    IN KINTERRUPT_MODE InterruptMode,
    IN BOOLEAN ShareVector
);

XBAPI VOID NTAPI KeInitializeEvent
(
    IN PRKEVENT Event,
    IN EVENT_TYPE Type,
    IN BOOLEAN State
);

XBAPI VOID NTAPI KeInitializeDpc
(
    OUT KDPC *Dpc,
    IN PKDEFERRED_ROUTINE DeferredRoutine,
    IN PVOID DeferredContext OPTIONAL
);

XBAPI VOID NTAPI KeInitializeDeviceQueue
(
    OUT PKDEVICE_QUEUE DeviceQueue
);

XBAPI VOID NTAPI KeInitializeApc
(
    IN PRKAPC Apc,
    IN PRKTHREAD Thread,
    IN PKKERNEL_ROUTINE KernelRoutine,
    IN PKRUNDOWN_ROUTINE RundownRoutine OPTIONAL,
    IN PKNORMAL_ROUTINE NormalRoutine OPTIONAL,
    IN KPROCESSOR_MODE ProcessorMode OPTIONAL,
    IN PVOID NormalContext OPTIONAL
);

/**
 * Returns a pointer to the thread object belonging to the current thread.
 * @return A pointer to an opaque thread object.
 */
XBAPI PKTHREAD NTAPI KeGetCurrentThread(void);

XBAPI KIRQL NTAPI KeGetCurrentIrql(void);

XBAPI VOID NTAPI KeEnterCriticalRegion(void);

XBAPI BOOLEAN NTAPI KeDisconnectInterrupt
(
    IN PKINTERRUPT Interrupt
);

/**
 * Puts the current thread into an alertable or nonalertable wait state for a specified interval
 * @param WaitMode Specifies the processor mode in which the caller is waiting, which can be either KernelMode or UserMode.
 * @param Alertable TRUE if the wait is alertable.
 * @param Interval Specifies the absolute or relative time, in units of 100 nanoseconds, for which the wait is to occur. A negative value indicates relative time. Absolute expiration times track any changes in system time, relative expiration times are not affected by system time changes.
 * @return STATUS_SUCCESS (the delay completed because the specified interval elapsed), STATUS_ALERTED (the delay completed because the thread was alerted) or STATUS_USER_APC (a user-mode APC was delivered before the specified interval expired).
 */
XBAPI NTSTATUS NTAPI KeDelayExecutionThread
(
    IN KPROCESSOR_MODE WaitMode,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Interval
);

XBAPI BOOLEAN NTAPI KeConnectInterrupt
(
    IN PKINTERRUPT Interrupt
);

XBAPI BOOLEAN NTAPI KeCancelTimer
(
    IN PKTIMER Timer
);

XBAPI VOID NTAPI DECLSPEC_NORETURN KeBugCheckEx
(
    IN ULONG BugCheckCode,
    IN ULONG_PTR BugCheckParameter1,
    IN ULONG_PTR BugCheckParameter2,
    IN ULONG_PTR BugCheckParameter3,
    IN ULONG_PTR BugCheckParameter4
);

XBAPI VOID NTAPI DECLSPEC_NORETURN KeBugCheck
(
    IN ULONG BugCheckCode
);

XBAPI VOID NTAPI KeBoostPriorityThread
(
    IN PKTHREAD Thread,
    IN KPRIORITY Increment
);

XBAPI BOOLEAN NTAPI KeAlertThread
(
    IN PKTHREAD Thread,
    IN KPROCESSOR_MODE ProcessorMode
);

XBAPI ULONG NTAPI KeAlertResumeThread
(
    IN PKTHREAD Thread
);

XBAPI volatile BOOLEAN KdDebuggerNotPresent;
XBAPI volatile BOOLEAN KdDebuggerEnabled;

XBAPI NTSTATUS NTAPI IoSynchronousFsdRequest
(
    IN ULONG MajorFunction,
    IN PDEVICE_OBJECT DeviceObject,
    IN OUT PVOID Buffer OPTIONAL,
    IN ULONG Length OPTIONAL,
    IN PLARGE_INTEGER StartingOffset OPTIONAL
);

XBAPI NTSTATUS NTAPI IoSynchronousDeviceIoControlRequest
(
    IN ULONG IoControlCode,
    IN PDEVICE_OBJECT DeviceObject,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength,
    OUT PULONG ReturnedOutputBufferLength OPTIONAL,
    IN BOOLEAN InternalDeviceIoControl
);

XBAPI VOID NTAPI IoStartPacket
(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PULONG Key OPTIONAL
);

XBAPI VOID NTAPI IoStartNextPacketByKey
(
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG Key
);

XBAPI VOID NTAPI IoStartNextPacket
(
    IN PDEVICE_OBJECT DeviceObject
);

XBAPI VOID NTAPI IoSetShareAccess
(
    IN ACCESS_MASK DesiredAccess,
    IN ULONG DesiredShareAccess,
    IN OUT PFILE_OBJECT FileObject,
    OUT PSHARE_ACCESS ShareAccess
);

XBAPI NTSTATUS NTAPI IoSetIoCompletion
(
    IN PVOID IoCompletion,
    IN PVOID KeyContext,
    IN PVOID ApcContext,
    IN NTSTATUS IoStatus,
    IN ULONG_PTR IoStatusInformation
);

XBAPI VOID NTAPI IoRemoveShareAccess
(
    IN PFILE_OBJECT FileObject,
    IN OUT PSHARE_ACCESS ShareAccess
);

XBAPI VOID NTAPI IoQueueThreadIrp
(
    IN PIRP Irp
);

XBAPI NTSTATUS NTAPI IoQueryVolumeInformation
(
    IN PFILE_OBJECT FileObject,
    IN FS_INFORMATION_CLASS FsInformationClass,
    IN ULONG Length,
    OUT PVOID FsInformation,
    OUT PULONG ReturnedLength
);

XBAPI NTSTATUS NTAPI IoQueryFileInformation
(
    IN PFILE_OBJECT FileObject,
    IN FILE_INFORMATION_CLASS FileInformationClass,
    IN ULONG Length,
    OUT PVOID FileInformation,
    OUT PULONG ReturnedLength
);

XBAPI VOID NTAPI IoMarkIrpMustComplete
(
    IN OUT PIRP Irp
);

XBAPI NTSTATUS NTAPI IoInvalidDeviceRequest
(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
);

XBAPI VOID NTAPI IoInitializeIrp
(
    IN OUT PIRP Irp,
    IN USHORT PacketSize,
    IN CCHAR StackSize
);

XBAPI VOID NTAPI IoFreeIrp
(
    IN PIRP Irp
);

XBAPI volatile OBJECT_TYPE IoFileObjectType;

XBAPI NTSTATUS NTAPI IoDismountVolumeByName
(
    IN POBJECT_STRING DeviceName
);

XBAPI NTSTATUS NTAPI IoDismountVolume
(
    IN PDEVICE_OBJECT DeviceObject
);

XBAPI volatile OBJECT_TYPE IoDeviceObjectType;

XBAPI NTSTATUS NTAPI IoDeleteSymbolicLink
(
    IN POBJECT_STRING SymbolicLinkName
);

XBAPI VOID NTAPI IoDeleteDevice
(
    IN PDEVICE_OBJECT DeviceObject
);

XBAPI NTSTATUS NTAPI IoCreateSymbolicLink
(
    IN POBJECT_STRING SymbolicLinkName,
    IN POBJECT_STRING DeviceName
);

XBAPI NTSTATUS NTAPI IoCreateFile
(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG Disposition,
    IN ULONG CreateOptions,
    IN ULONG Options
);

XBAPI NTSTATUS NTAPI IoCreateDevice
(
    IN PDRIVER_OBJECT DriverObject,
    IN ULONG DeviceExtensionSize,
    IN POBJECT_STRING DeviceName OPTIONAL,
    IN DEVICE_TYPE DeviceType,
    IN BOOLEAN Exclusive,
    OUT PDEVICE_OBJECT *DeviceObject
);

XBAPI volatile OBJECT_TYPE IoCompletionObjectType;

XBAPI NTSTATUS NTAPI IoCheckShareAccess
(
    IN ACCESS_MASK DesiredAccess,
    IN ULONG DesiredShareAccess,
    IN OUT PFILE_OBJECT FileObject,
    IN OUT PSHARE_ACCESS ShareAccess,
    IN BOOLEAN Update
);

XBAPI PIRP NTAPI IoBuildSynchronousFsdRequest
(
    IN ULONG MajorFunction,
    IN PDEVICE_OBJECT DeviceObject,
    IN OUT PVOID Buffer OPTIONAL,
    IN ULONG Length OPTIONAL,
    IN PLARGE_INTEGER StartingOffset OPTIONAL,
    IN PKEVENT Event,
    OUT PIO_STATUS_BLOCK IoStatusBlock
);

XBAPI PIRP NTAPI IoBuildDeviceIoControlRequest
(
    IN ULONG IoControlCode,
    IN PDEVICE_OBJECT DeviceObject,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength,
    IN BOOLEAN InternalDeviceIoControl,
    IN PKEVENT Event,
    OUT PIO_STATUS_BLOCK IoStatusBlock
);

XBAPI PIRP NTAPI IoBuildAsynchronousFsdRequest
(
    IN ULONG MajorFunction,
    IN PDEVICE_OBJECT DeviceObject,
    IN OUT PVOID Buffer OPTIONAL,
    IN ULONG Length OPTIONAL,
    IN PLARGE_INTEGER StartingOffset OPTIONAL,
    IN PIO_STATUS_BLOCK IoStatusBlock OPTIONAL
);

XBAPI PIRP NTAPI IoAllocateIrp
(
    IN CCHAR StackSize
);

XBAPI volatile IDE_CHANNEL_OBJECT IdexChannelObject;

XBAPI NTSTATUS NTAPI HalWriteSMCScratchRegister
(
    IN ULONG ScratchRegister
);

XBAPI NTSTATUS NTAPI HalWriteSMBusValue
(
    IN UCHAR SlaveAddress,
    IN UCHAR CommandCode,
    IN BOOLEAN WriteWordValue,
    IN ULONG DataValue
);

XBAPI VOID DECLSPEC_NORETURN NTAPI HalReturnToFirmware
(
    IN FIRMWARE_REENTRY Routine
);

XBAPI VOID NTAPI HalRegisterShutdownNotification
(
    IN PHAL_SHUTDOWN_REGISTRATION ShutdownRegistration,
    IN BOOLEAN Register
);

XBAPI VOID NTAPI HalReadWritePCISpace
(
    IN ULONG BusNumber,
    IN ULONG SlotNumber,
    IN ULONG RegisterNumber,
    IN PVOID Buffer,
    IN ULONG Length,
    IN BOOLEAN WritePCISpace
);

XBAPI NTSTATUS NTAPI HalReadSMCTrayState
(
    OUT PULONG TrayState,
    OUT PULONG TrayStateChangeCount OPTIONAL
);

XBAPI NTSTATUS NTAPI HalReadSMBusValue
(
    IN UCHAR SlaveAddress,
    IN UCHAR CommandCode,
    IN BOOLEAN ReadWordValue,
    OUT ULONG *DataValue
);

/**
 * Checks whether the console is in the middle of a reset or shutdown sequence.
 * @return TRUE if the console is in the middle of a reset or shutdown sequence, else FALSE.
 */
XBAPI BOOLEAN NTAPI HalIsResetOrShutdownPending(void);

/**
 * Initiates a shutdown. May return if the SMBus-lock is already owned, the shutdown will start as soon as the lock is available.
 **/
XBAPI VOID NTAPI HalInitiateShutdown(void);

XBAPI ULONG NTAPI HalGetInterruptVector
(
    IN ULONG BusInterruptLevel,
    OUT PKIRQL Irql
);

XBAPI VOID NTAPI HalEnableSystemInterrupt
(
    IN ULONG BusInterruptLevel,
    IN KINTERRUPT_MODE InterruptMode
);

/**
 * Switches the console to secure mode, where a tray eject or tray open interrupt causes the console to reboot. After the console is switched into secure mode, it cannot switch back.
 */
XBAPI VOID NTAPI HalEnableSecureTrayEject(void);
XBAPI volatile STRING HalDiskSerialNumber;
XBAPI volatile STRING HalDiskModelNumber;
XBAPI volatile ULONG HalDiskCachePartitionCount;

XBAPI VOID NTAPI HalDisableSystemInterrupt
(
    IN ULONG BusInterruptLevel
);

XBAPI DWORD HalBootSMCVideoMode(void);


XBAPI NTSTATUS NTAPI FscSetCacheSize
(
    IN PFN_COUNT NumberOfCachePages
);

XBAPI VOID NTAPI FscInvalidateIdleBlocks (void);

XBAPI PFN_COUNT NTAPI FscGetCacheSize (void);

XBAPI volatile OBJECT_TYPE ExTimerObjectType;
XBAPI volatile OBJECT_TYPE ExSemaphoreObjectType;

XBAPI NTSTATUS NTAPI ExSaveNonVolatileSetting
(
    IN ULONG ValueIndex,
    IN ULONG Type,
    IN CONST PVOID Value,
    IN ULONG ValueLength
);

XBAPI VOID NTAPI ExReleaseReadWriteLock
(
    IN PERWLOCK ReadWriteLock
);

XBAPI NTSTATUS NTAPI ExReadWriteRefurbInfo
(
    OUT XBOX_REFURB_INFO *RefurbInfo,
    IN ULONG ValueLength,
    BOOLEAN DoWrite
);

XBAPI VOID NTAPI ExRaiseStatus
(
    IN NTSTATUS Status
);

XBAPI VOID NTAPI ExRaiseException
(
    PEXCEPTION_RECORD ExceptionRecord
);

/**
 * Returns the size of the pool block.
 * @param PoolBlock The address of the pool block.
 * @return The size of the pool block.
 */
XBAPI ULONG NTAPI ExQueryPoolBlockSize
(
    IN PVOID PoolBlock
);

XBAPI NTSTATUS NTAPI ExQueryNonVolatileSetting
(
    IN ULONG ValueIndex,
    OUT PULONG Type,
    OUT PVOID Value,
    IN ULONG ValueLength,
    OUT PULONG ResultLength
);

XBAPI volatile OBJECT_TYPE ExMutantObjectType;

XBAPI LARGE_INTEGER NTAPI ExInterlockedAddLargeInteger
(
    IN PLARGE_INTEGER Addend,
    IN LARGE_INTEGER Increment
);

XBAPI VOID NTAPI ExInitializeReadWriteLock
(
    IN PERWLOCK ReadWriteLock
);

/**
 * Deallocates a block of pool memory.
 * @param P Specifies the address of the block of pool memory being deallocated.
 */
XBAPI VOID NTAPI ExFreePool
(
    IN PVOID P
);

XBAPI volatile OBJECT_TYPE ExEventObjectType;

/**
 * Allocates pool memory and returns a pointer to the allocated block.
 * @oaram NumberOfBytes The number of bytes to allocate.
 * @param Tag The pool tag to use for the allocated memory. Specify the pool tag as a character literal of up to four characters delimited by single quotation marks (for example, 'Tag1'). The string is usually specified in reverse order (for example, '1gaT'). Each ASCII character in the tag must be a value in the range 0x20 (space) to 0x126 (tilde). Each allocation code path should use a unique pool tag to help debuggers and verifiers identify the code path.
 * @return NULL if there is insufficient memory in the free pool to satisfy the request. Otherwise, the routine returns a pointer to the allocated memory.
 */
XBAPI PVOID NTAPI ExAllocatePoolWithTag
(
    IN SIZE_T NumberOfBytes,
    IN ULONG Tag
);

/**
 * Allocates pool memory and returns a pointer to the allocated block.
 * @oaram NumberOfBytes The number of bytes to allocate.
 * @return NULL if there is insufficient memory in the free pool to satisfy the request. Otherwise, the routine returns a pointer to the allocated memory.
 */
XBAPI PVOID NTAPI ExAllocatePool
(
    IN SIZE_T NumberOfBytes
);

XBAPI VOID NTAPI ExAcquireReadWriteLockShared
(
    IN PERWLOCK ReadWriteLock
);

XBAPI VOID NTAPI ExAcquireReadWriteLockExclusive
(
    IN PERWLOCK ReadWriteLock
);

XBAPI VOID NTAPI DbgUnLoadImageSymbols
(
    PSTRING FileName,
    PVOID ImageBase,
    ULONG_PTR ProcessId
);

/**
 * WARNING: This function crashes my XBox, so probably don't use.
 * Displays a prompt-string on the debugging console, then reads a line of text from the debugging console.
 * @param Prompt The string that gets output on the debugging console.
 * @param Response Specifies where to store the response string read from the debugging console.
 * @param MaximumResponseLength Maximum number of characters that fit into the response-buffer.
 * @return Number of characters stored into the response buffer, including newline.
 */
XBAPI ULONG NTAPI DbgPrompt
(
    PCH Prompt,
    PCH Response,
    ULONG MaximumResponseLength
);

/**
 * "printf"-style output function for the kernel debugger.
 * @param Format "printf"-style format string
 * @param ... Arguments matching the format string.
 * @return A status-code.
 */
XBAPI ULONG CDECL DbgPrint
(
    PCH Format,
    ...
);

XBAPI VOID NTAPI DbgLoadImageSymbols
(
    PSTRING FileName,
    PVOID ImageBase,
    ULONG_PTR ProcessId
);

XBAPI VOID NTAPI DbgBreakPointWithStatus
(
    IN ULONG Status
);

XBAPI VOID NTAPI DbgBreakPoint (void);

XBAPI VOID NTAPI AvSetSavedDataAddress
(
    IN PVOID Address
);

XBAPI ULONG NTAPI AvSetDisplayMode
(
    IN PVOID RegisterBase,
    IN ULONG Step,
    IN ULONG DisplayMode,
    IN ULONG SourceColorFormat,
    IN ULONG Pitch,
    IN ULONG FrameBuffer
);

XBAPI VOID NTAPI AvSendTVEncoderOption
(
    IN PVOID RegisterBase,
    IN ULONG Option,
    IN ULONG Param,
    OUT PULONG Result
);

XBAPI PVOID NTAPI AvGetSavedDataAddress(void);

/**
 * Performs a byte-swap (big-endian <-> little-endian) conversion of a USHORT
 * @param Source The USHORT-value which is to be swapped
 * @return The byte-swapped value
 */
XBAPI USHORT FASTCALL RtlUshortByteSwap
(
    IN USHORT Source
);

/**
 * Performs a byte-swap (big-endian <-> little-endian) conversion of a ULONG
 * @param Source The ULONG-value which is to be swapped
 * @return The byte-swapped value
 */
XBAPI ULONG FASTCALL RtlUlongByteSwap
(
    IN ULONG Source
);

XBAPI VOID FASTCALL ObfReferenceObject
(
    IN PVOID Object
);

XBAPI VOID FASTCALL ObfDereferenceObject
(
    IN PVOID Object
);

XBAPI VOID FASTCALL KiUnlockDispatcherDatabase
(
    IN KIRQL OldIrql
);

/**
 * Raises the hardware priority to the specified IRQL value, thereby masking off interrupts of equivalent or lower IRQL on the current processor.
 * @param NewIrql Specifies the new IRQL to which the hardware priority is to be raised.
 * @return The original IRQL value to be used in a subsequent call to KfLowerIrql.
 */
XBAPI KIRQL FASTCALL KfRaiseIrql
(
    IN KIRQL NewIrql
);

/**
 * Restores the IRQL on the current processor to its original value.
 * @param NewIrql Specifies the IRQL that was returned from KfRaiseIrql.
 */
XBAPI VOID FASTCALL KfLowerIrql
(
    IN KIRQL NewIrql
);

XBAPI VOID FASTCALL IofCompleteRequest
(
    IN PIRP Irp,
    IN CCHAR PriorityBoost
);

XBAPI NTSTATUS FASTCALL IofCallDriver
(
    IN PDEVICE_OBJECT DeviceObject,
    IN OUT PIRP Irp
);

XBAPI PSINGLE_LIST_ENTRY FASTCALL InterlockedPushEntrySList
(
    IN PSLIST_HEADER ListHead,
    IN PSINGLE_LIST_ENTRY ListEntry
);

XBAPI PSINGLE_LIST_ENTRY FASTCALL InterlockedPopEntrySList
(
    IN PSLIST_HEADER ListHead
);

/**
 * Atomically increments a LONG-value
 * @param Addend Pointer to the LONG-variable which is to be incremented
 * @return The resulting incremented value (also stored at 'Addend')
 */
XBAPI LONG FASTCALL InterlockedIncrement
(
    IN PLONG Addend
);

XBAPI PSINGLE_LIST_ENTRY FASTCALL InterlockedFlushSList
(
    IN PSLIST_HEADER ListHead
);

XBAPI LONG FASTCALL InterlockedExchangeAdd
(
    IN OUT PLONG Addend,
    IN LONG Increment
);

XBAPI LONG FASTCALL InterlockedExchange
(
    IN OUT PLONG Target,
    IN LONG Value
);

/**
 * Atomically decrements a LONG-value
 * @param Addend Pointer to the LONG-variable which is to be decremented
 * @return The resulting decremented value (also stored at 'Addend')
 */
XBAPI LONG FASTCALL InterlockedDecrement
(
    IN PLONG Addend
);

XBAPI LONG FASTCALL InterlockedCompareExchange
(
    IN OUT PLONG Destination,
    IN LONG ExChange,
    IN LONG Comparand
);

/**
 * Requests a software interrupt.
 * @param RequestIrql The request IRQL value.
 */
XBAPI VOID FASTCALL HalRequestSoftwareInterrupt
(
    KIRQL RequestIrql
);

/**
 * Clears a possible pending software interrupt.
 * @param RequestIrql The request IRQL value.
 */
XBAPI VOID FASTCALL HalClearSoftwareInterrupt
(
    IN KIRQL RequestIrql
);

/**
 * Atomically removes an entry from the beginning of a doubly linked list (of LIST_ENTRY structs).
 * @param ListHead Pointer to the LIST_ENTRY that serves as the list's head
 * @return A pointer to the LIST_ENTRY which was removed from the list (NULL if the list was empty).
 */
XBAPI PLIST_ENTRY FASTCALL ExfInterlockedRemoveHeadList
(
    IN PLIST_ENTRY ListHead
);

/**
 * Atomically inserts an entry at the end of a doubly linked list (of LIST_ENTRY structs).
 * @param ListHead Pointer to the LIST_ENTRY that serves as the list's head
 * @param ListEntry Pointer to the LIST_ENTRY that is to be inserted
 * @return A pointer to the last list entry before the new entry was inserted (NULL if the list was empty).
 */
XBAPI PLIST_ENTRY FASTCALL ExfInterlockedInsertTailList
(
    IN PLIST_ENTRY ListHead,
    IN PLIST_ENTRY ListEntry
);

/**
 * Atomically inserts an entry at the beginning of a doubly linked list (of LIST_ENTRY structs).
 * @param ListHead Pointer to the LIST_ENTRY that serves as the list's head
 * @param ListEntry Pointer to the LIST_ENTRY that is to be inserted
 * @return A pointer to the first list entry before the new entry was inserted (NULL if the list was empty).
 */
XBAPI PLIST_ENTRY FASTCALL ExfInterlockedInsertHeadList
(
    IN PLIST_ENTRY ListHead,
    IN PLIST_ENTRY ListEntry
);

/**
 * Compares one integer variable to another and, if they're equal, sets the first variable to a caller-supplied value.
 * @param Destination A pointer to an integer that will be compared and possibly replaced.
 * @param Exchange A pointer to an integer that will replace the one at Destination if the comparison results in equality.
 * @param Comparand A pointer to an integer with which the value at Destination will be compared.
 * @return The inital value of the variable at Destination when the call occured.
 */
XBAPI LONGLONG FASTCALL ExInterlockedCompareExchange64
(
    IN PLONGLONG Destination,
    IN PLONGLONG Exchange,
    IN PLONGLONG Comparand
);

/**
 * Performs an interlocked addition of a ULONG increment value to a LARGE_INTEGER variable.
 * @param Addend A pointer to the LARGE_INTEGER variable that is incremented by the value of Increment.
 * @param Increment Specifies a ULONG value that is added to the variable that Addend points to.
 */
XBAPI VOID FASTCALL ExInterlockedAddLargeStatistic
(
    IN PLARGE_INTEGER Addend,
    IN ULONG Increment
);

#if defined(__cplusplus)
}
#endif

#pragma ms_struct reset

#pragma clang diagnostic pop
