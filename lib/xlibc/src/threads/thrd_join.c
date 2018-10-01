#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int thrd_join (thrd_t thr, int *res)
{
    if (thr.handle == NULL)
    {
        return thrd_error;
    }

    NTSTATUS status = NtWaitForSingleObject(thr.handle, FALSE, NULL);
    if (status != STATUS_WAIT_0)
    {
        return thrd_error;
    }

    if (res)
    {
        PETHREAD threadObject;
        if (!NT_SUCCESS(ObReferenceObjectByHandle(thr.handle, NULL, (PVOID *)&threadObject)))
        {
            return thrd_error;
        }

        *res = threadObject->ExitStatus;

        ObfDereferenceObject(threadObject);
    }

    NtClose(thr.handle);

    return thrd_success;
}
