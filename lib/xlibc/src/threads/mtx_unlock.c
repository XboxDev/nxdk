#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int mtx_unlock (mtx_t *mtx)
{
    if (NT_SUCCESS(NtReleaseMutant(mtx->handle, NULL)))
    {
        return thrd_success;
    }

    return thrd_error;
}
