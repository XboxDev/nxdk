#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

void cnd_destroy (cnd_t *cond)
{
    NtClose(cond->eventHandles[0]);
    NtClose(cond->eventHandles[1]);
}
