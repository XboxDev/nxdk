#include <threads.h>
#include <stdbool.h>
#include <_tss.h>

void _xlibc_tss_cleanup()
{
    for (int dtor_i=0; dtor_i<TSS_DTOR_ITERATIONS; dtor_i++)
    {
        bool not_done = false;

        for (int slot_i=0; slot_i<TSS_SLOTS_NUM; slot_i++)
        {
            if (tss_dtors[slot_i] == NULL) continue;
            if (tss_slots[slot_i] == NULL) continue;

            void *val;
            val = tss_slots[slot_i];
            tss_slots[slot_i] = NULL;
            tss_dtors[slot_i](val);
        }

        bool done = true;

        for (int slot_i=0; slot_i<TSS_SLOTS_NUM; slot_i++)
        {
            if (tss_slots[slot_i] != NULL && tss_dtors[slot_i] != NULL)
            {
                done = false;
                break;
            }
        }

        if (done) break;
    }
}
