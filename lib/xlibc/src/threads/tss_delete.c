#include <threads.h>
#include <assert.h>
#include <_tss.h>

void tss_delete (tss_t key)
{
    assert(key < TSS_SLOTS_NUM);

    mtx_lock(&tss_lock);

    tss_bitmap[key / 32] |= (1 << (key % 32));
    tss_slots[key] = 0;
    tss_dtors[key] = NULL;

    mtx_unlock(&tss_lock);
}
