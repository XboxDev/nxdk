#include <threads.h>
#include <xboxkrnl/xboxkrnl.h>

void mtx_destroy (mtx_t *mtx)
{
    NtClose(mtx->handle);
}
