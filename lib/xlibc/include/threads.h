#ifndef _XLIBC_THREADS_H
#define _XLIBC_THREADS_H

#include <time.h>
#include <stdatomic.h>
#include <_restrict.h>
#include <_unimplemented.h>

#ifdef __cplusplus
extern "C" {
#endif

#define thread_local _Thread_local;
#define ONCE_FLAG_INIT 0
//#define TSS_DTOR_ITERATIONS ? FIXME

typedef int once_flag;

enum
{
    thrd_success=0,
    thrd_nomem,
    thrd_timedout,
    thrd_busy,
    thrd_error
};

enum
{
    mtx_plain=0,
    mtx_recursive=1,
    mtx_timed=2
};

typedef struct
{
    void *handle;
} mtx_t;

typedef struct
{
    void *eventHandles[2]; // [0] single-receiver signal, [1] broadcast signal
    atomic_int waitCount;
} cnd_t;

typedef unsigned int tss_t;

typedef struct
{
    void *handle;
    void *id;
} thrd_t;

typedef int (*thrd_start_t)(void *arg);

typedef void (*tss_dtor_t)(void *val);

void call_once (once_flag *flag, void (*func)(void));

int cnd_broadcast (cnd_t *cond);
void cnd_destroy (cnd_t *cond);
int cnd_init (cnd_t *cond);
int cnd_signal (cnd_t *cond);
int cnd_timedwait (cnd_t *XLIBC_RESTRICT cond, mtx_t *XLIBC_RESTRICT mtx, const struct timespec *XLIBC_RESTRICT ts);
int cnd_wait (cnd_t *cond, mtx_t *mtx);

void mtx_destroy (mtx_t *mtx);
int mtx_init (mtx_t *mtx, int type);
int mtx_lock (mtx_t *mtx);
int mtx_timedlock (mtx_t *XLIBC_RESTRICT mtx, const struct timespec *XLIBC_RESTRICT ts);
int mtx_trylock (mtx_t *mtx);
int mtx_unlock (mtx_t *mtx);

int thrd_create (thrd_t *thr, thrd_start_t func, void *arg);
thrd_t thrd_current (void);
int thrd_detach (thrd_t thr);
int thrd_equal (thrd_t thr0, thrd_t thr1);
_Noreturn void thrd_exit (int res);
int thrd_join (thrd_t thr, int *res);
int thrd_sleep (const struct timespec *duration, struct timespec *remaining);
void thrd_yield (void);

int tss_create (tss_t *key, tss_dtor_t dtor) __unimplemented;
void tss_delete (tss_t key) __unimplemented;
void *tss_get (tss_t key) __unimplemented;
int tss_set (tss_t key, void *val) __unimplemented;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _XLIBC_THREADS_H */
