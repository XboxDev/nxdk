#include <winbase.h>
#include <threads.h>

static thread_local DWORD lastError = 0;

DWORD GetLastError (void)
{
    return lastError;
}

void SetLastError (DWORD error)
{
    lastError = error;
}
