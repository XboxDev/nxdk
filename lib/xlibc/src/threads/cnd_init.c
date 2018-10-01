#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int cnd_init (cnd_t *cond)
{
    NTSTATUS status;

    atomic_store(&cond->waitCount, 0);

    status = NtCreateEvent(&cond->eventHandles[0], NULL, SynchronizationEvent, FALSE);
    if (!NT_SUCCESS(status))
    {
        return thrd_error;
    }

    // Notification-events are NOT auto-resetting!
    status = NtCreateEvent(&cond->eventHandles[1], NULL, NotificationEvent, FALSE);
    if (!NT_SUCCESS(status))
    {
        NtClose(cond->eventHandles[0]);
        return thrd_error;
    }

    return thrd_success;
}
