#include <threads.h>
#include <_tss.h>

void _xlibc_tss_init()
{
    mtx_init(&tss_lock, mtx_plain);
}
