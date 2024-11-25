/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * Copyright (c) 2015 Matt Borgerson
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 * Xbox Support: Matt Borgerson
 *
 */
#include <lwip/debug.h>
#include <lwip/sys.h>
#include <lwip/opt.h>
#include <lwip/stats.h>
#include <hal/xbox.h>

#if !NO_SYS
#include <xboxkrnl/xboxkrnl.h>
#include <assert.h>
#include <stdlib.h>

static struct sys_sem *sys_sem_new_internal(u8_t count);
static void sys_sem_free_internal(struct sys_sem *sem);
static int ext_sys_arch_sem_try(struct sys_sem **s);

static struct sys_thread *threads = NULL;

struct sys_mbox_msg {
    struct sys_mbox_msg *next;
    void *msg;
};

#define SYS_MBOX_SIZE 128

struct sys_mbox {
    int first, last;
    void *msgs[SYS_MBOX_SIZE];
    struct sys_sem *read_sem;
    struct sys_sem *write_sem;
    struct sys_sem *mutex;
};

struct sys_sem {
    HANDLE handle;
};

struct sys_thread {
    struct sys_thread *next;
    HANDLE handle;
};
#endif /* !NO_SYS */

#if LWIP_DEBUG
unsigned char debug_flags;
#endif

int errno;

u32_t
sys_now(void)
{
    return KeTickCount;
}

void
sys_init(void)
{
}

#if 0
u32_t
sys_jiffies(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}
#endif

#if !NO_SYS

sys_thread_t
sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio)
{
    struct sys_thread *st = NULL;
    LWIP_UNUSED_ARG(name);
    LWIP_UNUSED_ARG(prio);

    st = (struct sys_thread *)malloc(sizeof(struct sys_thread));

    if (NULL == st) {
        abort();
    }

    st->handle = CreateThread(NULL, stacksize, (void *)function, arg, 0, NULL);

    return st;
}

err_t
sys_mbox_new(struct sys_mbox **mb, int size)
{
    struct sys_mbox *mbox;
    LWIP_UNUSED_ARG(size);

    mbox = (struct sys_mbox *)malloc(sizeof(struct sys_mbox));
    if (mbox == NULL) {
        return ERR_MEM;
    }
    mbox->first = mbox->last = 0;
    mbox->read_sem = sys_sem_new_internal(0);
    mbox->write_sem = sys_sem_new_internal(SYS_MBOX_SIZE);
    mbox->mutex = sys_sem_new_internal(1);

    SYS_STATS_INC_USED(mbox);
    *mb = mbox;
    return ERR_OK;
}

void
sys_mbox_free(struct sys_mbox **mb)
{
    if ((mb != NULL) && (*mb != SYS_MBOX_NULL)) {
        struct sys_mbox *mbox = *mb;
        SYS_STATS_DEC(mbox.used);
        sys_arch_sem_wait(&mbox->mutex, 0);

        sys_sem_free_internal(mbox->read_sem);
        sys_sem_free_internal(mbox->write_sem);
        sys_sem_free_internal(mbox->mutex);
        /*  LWIP_DEBUGF("sys_mbox_free: mbox 0x%lx\n", mbox); */
        free(mbox);
    }
}

err_t
sys_mbox_trypost(struct sys_mbox **mb, void *msg)
{
    u8_t first;
    struct sys_mbox *mbox;
    LWIP_ASSERT("invalid mbox", (mb != NULL) && (*mb != NULL));
    mbox = *mb;

    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_trypost: mbox %p msg %p\n",
                            (void *)mbox, (void *)msg));

    if (!ext_sys_arch_sem_try(&mbox->write_sem)) {
        return ERR_MEM;
    }

    sys_arch_sem_wait(&mbox->mutex, 0);
    mbox->msgs[mbox->last % SYS_MBOX_SIZE] = msg;
    mbox->last++;
    sys_sem_signal(&mbox->mutex);

    sys_sem_signal(&mbox->read_sem);

    return ERR_OK;
}

err_t
sys_mbox_trypost_fromisr(struct sys_mbox **mb, void *msg)
{
    return sys_mbox_trypost(mb, msg);
}

void
sys_mbox_post(struct sys_mbox **mb, void *msg)
{
    u8_t first;
    struct sys_mbox *mbox;
    LWIP_ASSERT("invalid mbox", (mb != NULL) && (*mb != NULL));
    mbox = *mb;

    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post: mbox %p msg %p\n", (void *)mbox, (void *)msg));

    sys_arch_sem_wait(&mbox->write_sem, 0);

    sys_arch_sem_wait(&mbox->mutex, 0);
    mbox->msgs[mbox->last % SYS_MBOX_SIZE] = msg;
    mbox->last++;
    sys_sem_signal(&mbox->mutex);

    sys_sem_signal(&mbox->read_sem);
}

u32_t
sys_arch_mbox_tryfetch(struct sys_mbox **mb, void **msg)
{
    struct sys_mbox *mbox;
    LWIP_ASSERT("invalid mbox", (mb != NULL) && (*mb != NULL));
    mbox = *mb;

    if (!ext_sys_arch_sem_try(&mbox->read_sem)) {
        return SYS_MBOX_EMPTY;
    }

    sys_arch_sem_wait(&mbox->mutex, 0);
    if (msg != NULL) {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_tryfetch: mbox %p msg %p\n", (void *)mbox, *msg));
        *msg = mbox->msgs[mbox->first % SYS_MBOX_SIZE];
    } else {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_tryfetch: mbox %p, null msg\n", (void *)mbox));
    }
    mbox->first++;
    sys_sem_signal(&mbox->mutex);

    sys_sem_signal(&mbox->write_sem);
    return 0;
}

u32_t
sys_arch_mbox_fetch(struct sys_mbox **mb, void **msg, u32_t timeout)
{
    u32_t time_needed = 0;
    struct sys_mbox *mbox;
    LWIP_ASSERT("invalid mbox", (mb != NULL) && (*mb != NULL));
    mbox = *mb;

    time_needed = sys_arch_sem_wait(&mbox->read_sem, timeout);
    if (time_needed == SYS_ARCH_TIMEOUT) {
        return SYS_ARCH_TIMEOUT;
    }

    sys_arch_sem_wait(&mbox->mutex, 0);
    if (msg != NULL) {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_fetch: mbox %p msg %p\n", (void *)mbox, *msg));
        *msg = mbox->msgs[mbox->first % SYS_MBOX_SIZE];
    } else {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_fetch: mbox %p, null msg\n", (void *)mbox));
    }
    mbox->first++;
    sys_sem_signal(&mbox->mutex);

    sys_sem_signal(&mbox->write_sem);
    return time_needed;
}

static struct sys_sem *
sys_sem_new_internal(u8_t count)
{
    struct sys_sem *sem;

    sem = (struct sys_sem *)malloc(sizeof(struct sys_sem));
    if (sem != NULL) {
        NTSTATUS status = NtCreateSemaphore(&(sem->handle), NULL, count, 10000);
        if (!NT_SUCCESS(status)) {
            free(sem);
            return NULL;
        }
    }
    return sem;
}

err_t
sys_sem_new(struct sys_sem **sem, u8_t count)
{
    SYS_STATS_INC_USED(sem);
    *sem = sys_sem_new_internal(count);
    if (*sem == NULL) {
        return ERR_MEM;
    }
    return ERR_OK;
}

u32_t
sys_arch_sem_wait(struct sys_sem **s, u32_t timeout)
{
    struct sys_sem *sem;
    LWIP_ASSERT("invalid sem", (s != NULL) && (*s != NULL));
    sem = *s;

    LARGE_INTEGER timeout_large;
    PLARGE_INTEGER timeout_ptr;
    if (timeout) {
        timeout_large.QuadPart = (long long int)timeout * -10000;
        timeout_ptr = &timeout_large;
    } else {
        timeout_ptr = NULL;
    }

    u32_t start_time = sys_now();
    NTSTATUS status = NtWaitForSingleObject(sem->handle, FALSE, timeout_ptr);
    if (status == STATUS_TIMEOUT) {
        return SYS_ARCH_TIMEOUT;
    }
    assert(status == STATUS_SUCCESS);

    return sys_now() - start_time;
}

/**
 * Custom function not demanded by lwip. Used for the mbox implementation when waiting is not allowed.
 */
static int ext_sys_arch_sem_try(struct sys_sem **s)
{
    struct sys_sem *sem;
    LWIP_ASSERT("invalid sem", (s != NULL) && (*s != NULL));
    sem = *s;

    LARGE_INTEGER timeout;
    timeout.QuadPart = 0;

    NTSTATUS status = NtWaitForSingleObject(sem->handle, FALSE, &timeout);
    if (!NT_SUCCESS(status)) {
        return 0;
    }

    if (status == STATUS_TIMEOUT) {
        return 0;
    }

    return -1;
}

void
sys_sem_signal(struct sys_sem **s)
{
    struct sys_sem *sem;
    LWIP_ASSERT("invalid sem", (s != NULL) && (*s != NULL));
    sem = *s;

    NtReleaseSemaphore(sem->handle, 1, NULL);
}

static void
sys_sem_free_internal(struct sys_sem *sem)
{
    NTSTATUS status = NtClose((void*)sem->handle);
    if (!NT_SUCCESS(status)) {
        abort();
    }
    free(sem);
}

void
sys_sem_free(struct sys_sem **sem)
{
    if ((sem != NULL) && (*sem != SYS_SEM_NULL)) {
        SYS_STATS_DEC(sem.used);
        sys_sem_free_internal(*sem);
    }
}

sys_prot_t sys_arch_protect()
{
    return KeRaiseIrqlToDpcLevel();
}

void sys_arch_unprotect(sys_prot_t lev)
{
    KfLowerIrql(lev);
}

#endif /* !NO_SYS */
