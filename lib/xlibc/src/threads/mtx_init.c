#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int mtx_init (mtx_t *mtx, int type)
{
    // FIXME: At the moment, all mutexes are timed and recursive! Critical sections are faster when that's not needed.
    NTSTATUS status;
    status = NtCreateMutant(&mtx->handle, NULL, FALSE);

    if (NT_SUCCESS(status))
    {
        return thrd_success;
    }

    return thrd_error;
}
