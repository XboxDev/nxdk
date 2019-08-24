#ifndef __SYSINFOAPI_H__
#define __SYSINFOAPI_H__

#include <minwinbase.h>

#ifdef __cplusplus
extern "C" {
#endif

void GetSystemTimePreciseAsFileTime (LPFILETIME lpSystemTimeAsFileTime);

// Unprovided fields are intentionally disabled to catch code trying to access them
typedef struct _SYSTEM_INFO
{
    union
    {
        //DWORD dwOemId;
        struct
        {
            WORD wProcessorArchitecture;
            WORD wReserved;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;
    DWORD dwPageSize;
    //LPVOID lpMinimumApplicationAddress;
    //LPVOID lpMaximumApplicationAddress;
    DWORD_PTR dwActiveProcessorMask;
    DWORD dwNumberOfProcessors;
    //DWORD dwProcessorType;
    DWORD dwAllocationGranularity;
    //WORD wProcessorLevel;
    //WORD wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;

#define PROCESSOR_ARCHITECTURE_INTEL 0
#define PROCESSOR_ARCHITECTURE_ARM 5
#define PROCESSOR_ARCHITECTURE_IA64 6
#define PROCESSOR_ARCHITECTURE_AMD64 9
#define PROCESSOR_ARCHITECTURE_ARM64 12
#define PROCESSOR_ARCHITECTURE_UNKNOWN 0xFFFF

void GetSystemInfo (LPSYSTEM_INFO lpSystemInfo);

#ifdef __cplusplus
}
#endif

#endif
