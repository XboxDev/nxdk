#ifndef _XLIBC__TSS_H
#define _XLIBC__TSS_H

#include <threads.h>

#define TSS_SLOTS_NUM 64

extern mtx_t tss_lock;
extern thread_local void *tss_slots[TSS_SLOTS_NUM];
extern tss_dtor_t tss_dtors[TSS_SLOTS_NUM];
extern uint32_t tss_bitmap[TSS_SLOTS_NUM / 32];

void _xlibc_tss_cleanup();
void _xlibc_tss_init();

#endif /* end of include guard: _XLIBC__TSS_H */
