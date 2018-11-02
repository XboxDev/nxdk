#include <threads.h>
#include <_threads.h>
#include <_tls.h>
#include "xboxkrnl/xboxkrnl.h"

int thrd_create (thrd_t *thr, thrd_start_t func, void *arg)
{
    if (thr == NULL) {
        return thrd_error;
    }

    NTSTATUS ntstatus;
    ULONG stacksize;
    ULONG tlssize;
    // FIXME: We're directly reading the XBE StackCommit field here.
    //        A cleaner way with proper structs would be nice.
    stacksize = *((ULONG *)0x00010130);
    // Sum up the required amount of memory, round it up to a multiple of
    // 16 bytes and add 4 bytes for the self-reference
    tlssize = (_tls_used.EndAddressOfRawData - _tls_used.StartAddressOfRawData) + _tls_used.SizeOfZeroFill;
    tlssize = (tlssize + 15) & ~15;
    tlssize += 4;
    ntstatus = PsCreateSystemThreadEx(&thr->handle, 0, stacksize, tlssize, &thr->id, (PKSTART_ROUTINE)func, arg, FALSE, FALSE, _xlibc_thread_startup);

    if (NT_SUCCESS(ntstatus))
    {
        return thrd_success;
    }

    return thrd_error;
}
