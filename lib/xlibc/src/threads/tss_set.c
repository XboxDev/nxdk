#include <threads.h>
#include <assert.h>
#include <_tss.h>

int tss_set (tss_t key, void *val)
{
    assert(key < TSS_SLOTS_NUM);

    tss_slots[key] = val;
    return thrd_success;
}
