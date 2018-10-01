#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int cnd_signal (cnd_t *cond)
{
    NTSTATUS status;
    status = NtSetEvent(cond->eventHandles[0], NULL);
    if (NT_SUCCESS(status))
        return thrd_success;
    else
        return thrd_error;
}
