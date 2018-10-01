#include <threads.h>
#include <_threads.h>
#include "xboxkrnl/xboxkrnl.h"

int thrd_create (thrd_t *thr, thrd_start_t func, void *arg)
{
    if (thr == NULL) {
        return thrd_error;
    }

    NTSTATUS ntstatus;
    ULONG stacksize;
    stacksize = *((ULONG *)0x00010130);
    ntstatus = PsCreateSystemThreadEx(&thr->handle, 0, stacksize, 0, &thr->id, (PKSTART_ROUTINE)func, arg, FALSE, FALSE, _xlibc_thread_startup);

    if (NT_SUCCESS(ntstatus))
    {
        return thrd_success;
    }

    return thrd_error;
}
