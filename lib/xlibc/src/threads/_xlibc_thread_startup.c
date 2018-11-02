#include <threads.h>
#include <assert.h>
#include <string.h>
#include "xboxkrnl/xboxkrnl.h"
#include <_tls.h>

VOID NTAPI _xlibc_thread_startup (PKSTART_ROUTINE StartRoutine, PVOID StartContext)
{
    // Make sure the TLS pointer is valid
    assert(KeGetCurrentThread()->TlsData);

    // Retrieve TLS area address and point first entry to itself
    DWORD *TlsData;
    TlsData = ((DWORD *)KeGetCurrentThread()->TlsData) + 1;
    TlsData[-1] = (DWORD)TlsData;

    // 16 byte alignment required
    assert(((DWORD)TlsData & 15) == 0);

    // Copy initial values of TLS area
    DWORD TlsDataSize;
    TlsDataSize = _tls_used.EndAddressOfRawData - _tls_used.StartAddressOfRawData;
    memcpy(TlsData, (void *)_tls_used.StartAddressOfRawData, TlsDataSize);

    // Zero-initialize the rest
    RtlZeroMemory((char *)TlsData + TlsDataSize, _tls_used.SizeOfZeroFill);

    int res;
    res = ((thrd_start_t)StartRoutine)(StartContext);
    thrd_exit(res);
}
