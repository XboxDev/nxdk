#include <threads.h>
#include <assert.h>
#include <_tss.h>

void *tss_get (tss_t key)
{
    assert(key < TSS_SLOTS_NUM);

    return tss_slots[key];
}
