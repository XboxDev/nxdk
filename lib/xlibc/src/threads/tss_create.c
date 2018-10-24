#include <threads.h>
#include <assert.h>
#include <_tss.h>

mtx_t tss_lock;
// Note: This uses range-based initialization, which is a GNU C extension
thread_local void *tss_slots[TSS_SLOTS_NUM] = {[0 ... TSS_SLOTS_NUM-1] = NULL};
tss_dtor_t tss_dtors[TSS_SLOTS_NUM] = {[0 ... TSS_SLOTS_NUM-1] = NULL};
uint32_t tss_bitmap[TSS_SLOTS_NUM / 32] = {[0 ... (TSS_SLOTS_NUM/32)-1] = 0xFFFFFFFF};

int tss_create (tss_t *key, tss_dtor_t dtor)
{
    int retval = thrd_error;

    mtx_lock(&tss_lock);

    for (size_t i=0; i < (TSS_SLOTS_NUM/32); i++)
    {
        if (tss_bitmap[i] == 0) continue;

        unsigned int index = __builtin_ctz(tss_bitmap[i]);
        // Unset the bit
        tss_bitmap[i] &= ~(1 << index);
        *key = i*32 + index;
        retval = thrd_success;
        break;
    }

    mtx_unlock(&tss_lock);

    if (retval == thrd_success)
    {
        tss_dtors[*key] = dtor;
    }

    return retval;
}
