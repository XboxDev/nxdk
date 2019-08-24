#include <sysinfoapi.h>
#include <assert.h>
#include <xboxkrnl/xboxkrnl.h>

void GetSystemTimePreciseAsFileTime (LPFILETIME lpSystemTimeAsFileTime)
{
    assert(lpSystemTimeAsFileTime != NULL);
    LARGE_INTEGER systemTime;
    KeQuerySystemTime(&systemTime);
    lpSystemTimeAsFileTime->dwLowDateTime = systemTime.LowPart;
    lpSystemTimeAsFileTime->dwHighDateTime = systemTime.HighPart;
}

void GetSystemInfo (LPSYSTEM_INFO lpSystemInfo)
{
    assert(lpSystemInfo != NULL);

    lpSystemInfo->DUMMYUNIONNAME.DUMMYSTRUCTNAME.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_INTEL;
    lpSystemInfo->dwPageSize = 4096;
    lpSystemInfo->dwActiveProcessorMask = 1;
    lpSystemInfo->dwNumberOfProcessors = 1;
    lpSystemInfo->dwAllocationGranularity = 4096;
}
