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

#include <assert.h>
#include <stdlib.h>

#include <xboxkrnl/xboxkrnl.h>

#if LWIP_DEBUG
unsigned char debug_flags;
#endif
typedef struct {
    KSEMAPHORE sem;
    int valid;
} sys_sem_internal_t;
static_assert(sizeof(sys_sem_t) == sizeof(sys_sem_internal_t), "Size mismatch between sys_sem_t and sys_sem_internal_t");

static int ext_sys_arch_sem_try(sys_sem_t *s);

u32_t
sys_now(void)
{
    return KeTickCount;
}

void
sys_init(void)
{
}

sys_thread_t
sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio)
{
    LWIP_UNUSED_ARG(name);
    LWIP_UNUSED_ARG(prio);

    return CreateThread(NULL, stacksize, (void *)function, arg, 0, NULL);
}

err_t
sys_mbox_new(sys_mbox_t *mb, int size)
{
    LWIP_UNUSED_ARG(size);

    mb->first = mb->last = 0;
    sys_sem_new(&mb->read_sem, 0);
    sys_sem_new(&mb->write_sem, SYS_MBOX_SIZE);
    mb->valid = 1;

    SYS_STATS_INC_USED(mbox);
    return ERR_OK;
}

void
sys_mbox_free(sys_mbox_t *mb)
{
    if (mb != NULL) {
        SYS_STATS_DEC(mbox.used);

        sys_sem_free(&mb->read_sem);
        sys_sem_free(&mb->write_sem);
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_free: mbox 0x%p\n", (void *)mb));
    }
}

err_t
sys_mbox_trypost(sys_mbox_t *mb, void *msg)
{
    u8_t first;
    LWIP_ASSERT("invalid mbox", mb != NULL);

    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_trypost: mbox %p msg %p\n",
                            (void *)mb, (void *)msg));

    if (!ext_sys_arch_sem_try(&mb->write_sem)) {
        return ERR_MEM;
    }

    KIRQL prev_irql = KeRaiseIrqlToDpcLevel();
    mb->msgs[mb->last % SYS_MBOX_SIZE] = msg;
    mb->last++;
    KfLowerIrql(prev_irql);

    sys_sem_signal(&mb->read_sem);

    return ERR_OK;
}

err_t
sys_mbox_trypost_fromisr(sys_mbox_t *mb, void *msg)
{
    return sys_mbox_trypost(mb, msg);
}

void
sys_mbox_post(sys_mbox_t *mb, void *msg)
{
    u8_t first;
    LWIP_ASSERT("invalid mbox", mb != NULL);

    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post: mbox %p msg %p\n", (void *)mb, (void *)msg));

    sys_arch_sem_wait(&mb->write_sem, 0);

    KIRQL prev_irql = KeRaiseIrqlToDpcLevel();
    mb->msgs[mb->last % SYS_MBOX_SIZE] = msg;
    mb->last++;
    KfLowerIrql(prev_irql);

    sys_sem_signal(&mb->read_sem);
}

u32_t
sys_arch_mbox_tryfetch(sys_mbox_t *mb, void **msg)
{
    LWIP_ASSERT("invalid mbox", mb != NULL);

    if (!ext_sys_arch_sem_try(&mb->read_sem)) {
        return SYS_MBOX_EMPTY;
    }

    if (msg != NULL) {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_tryfetch: mbox %p msg %p\n", (void *)mb, *msg));
    } else {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_tryfetch: mbox %p, null msg\n", (void *)mb));
    }

    KIRQL prev_irql = KeRaiseIrqlToDpcLevel();
    if (msg != NULL) {
        *msg = mb->msgs[mb->first % SYS_MBOX_SIZE];
    }
    mb->first++;
    KfLowerIrql(prev_irql);

    sys_sem_signal(&mb->write_sem);
    return 0;
}

u32_t
sys_arch_mbox_fetch(sys_mbox_t *mb, void **msg, u32_t timeout)
{
    u32_t time_needed = 0;
    LWIP_ASSERT("invalid mbox", mb != NULL);

    time_needed = sys_arch_sem_wait(&mb->read_sem, timeout);
    if (time_needed == SYS_ARCH_TIMEOUT) {
        return SYS_ARCH_TIMEOUT;
    }

    if (msg != NULL) {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_fetch: mbox %p msg %p\n", (void *)mb, *msg));
    } else {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_fetch: mbox %p, null msg\n", (void *)mb));
    }

    KIRQL prev_irql = KeRaiseIrqlToDpcLevel();
    if (msg != NULL) {
        *msg = mb->msgs[mb->first % SYS_MBOX_SIZE];
    }
    mb->first++;
    KfLowerIrql(prev_irql);

    sys_sem_signal(&mb->write_sem);
    return time_needed;
}

int sys_mbox_valid (sys_mbox_t *mb)
{
    if (mb != NULL) {
        return mb->valid;
    }

    return 0;
}

void sys_mbox_set_invalid (sys_mbox_t *mb)
{
    if (mb != NULL) {
        mb->valid = 0;
    }
}

err_t
sys_sem_new(sys_sem_t *s, u8_t count)
{
    SYS_STATS_INC_USED(sem);
    sys_sem_internal_t *sem = (sys_sem_internal_t *)s;
    KeInitializeSemaphore(&sem->sem, count, 10000);
    sem->valid = 1;
    return ERR_OK;
}

u32_t
sys_arch_sem_wait(sys_sem_t *s, u32_t timeout)
{
    LWIP_ASSERT("invalid sem", s != NULL);
    assert(KeGetCurrentIrql() < DISPATCH_LEVEL);

    sys_sem_internal_t *sem = (sys_sem_internal_t *)s;

    LARGE_INTEGER timeout_large;
    PLARGE_INTEGER timeout_ptr;
    if (timeout) {
        timeout_large.QuadPart = (long long int)timeout * -10000;
        timeout_ptr = &timeout_large;
    } else {
        timeout_ptr = NULL;
    }

    u32_t start_time = sys_now();
    NTSTATUS status = KeWaitForSingleObject(&sem->sem, Executive, KernelMode, FALSE, timeout_ptr);
    if (status == STATUS_TIMEOUT) {
        return SYS_ARCH_TIMEOUT;
    }
    assert(status == STATUS_SUCCESS);

    return sys_now() - start_time;
}

/**
 * Custom function not demanded by lwip. Used for the mbox implementation when waiting is not allowed.
 */
static int ext_sys_arch_sem_try(sys_sem_t *s)
{
    LWIP_ASSERT("invalid sem", s != NULL);

    sys_sem_internal_t *sem = (sys_sem_internal_t *)s;

    LARGE_INTEGER timeout;
    timeout.QuadPart = 0;

    NTSTATUS status = KeWaitForSingleObject(&sem->sem, Executive, KernelMode, FALSE, &timeout);
    if (!NT_SUCCESS(status)) {
        return 0;
    }

    if (status == STATUS_TIMEOUT) {
        return 0;
    }

    return -1;
}

void
sys_sem_signal(sys_sem_t *s)
{
    LWIP_ASSERT("invalid sem", s != NULL);

    sys_sem_internal_t *sem = (sys_sem_internal_t *)s;

    KeReleaseSemaphore(&sem->sem, IO_NETWORK_INCREMENT, 1, FALSE);
}

void
sys_sem_free(sys_sem_t *s)
{
    if (s != NULL) {
        SYS_STATS_DEC(sem.used);
    }
}

int sys_sem_valid (sys_sem_t *s)
{
    sys_sem_internal_t *sem = (sys_sem_internal_t *)s;

    if (sem != NULL) {
        return sem->valid;
    }

    return 0;
}

void sys_sem_set_invalid (sys_sem_t *s)
{
    sys_sem_internal_t *sem = (sys_sem_internal_t *)s;

    sem->valid = 0;
}

sys_prot_t sys_arch_protect()
{
    return KeRaiseIrqlToDpcLevel();
}

void sys_arch_unprotect(sys_prot_t lev)
{
    KfLowerIrql(lev);
}
