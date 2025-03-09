/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * Copyright (c) 2015 Matt Borgerson
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
#ifndef LWIP_ARCH_SYS_ARCH_H
#define LWIP_ARCH_SYS_ARCH_H

void *nxdk_lwip_malloc(size_t size);
void *nxdk_lwip_calloc(size_t nmemb, size_t size);
void nxdk_lwip_free(void *ptr);

#define SYS_MBOX_NULL NULL
#define SYS_SEM_NULL  NULL

#if LWIP_DEBUG
extern unsigned char debug_flags;
#endif

typedef unsigned char sys_prot_t;

/**
 * The provided semaphores are KSEMAPHORE, but we don't want to pollute the
 * namespace by including the kernel header, so we define an equivalent and
 * check the size for correctness with a static_assert in sys_arch.c
 */
typedef struct {
    struct {
        struct {
            unsigned char Type;
            unsigned char Absolute;
            unsigned char Size;
            unsigned char Inserted;
            long SignalState;
            struct {
                void *Flink;
                void *Blink;
            } WaitListHead;
        } Header;
        long LIMIT;
    } sem;
    int valid;
} sys_sem_t;

/* let sys.h use binary semaphores for mutexes */
#define LWIP_COMPAT_MUTEX 1

#define SYS_MBOX_SIZE 128

typedef struct {
    volatile int first, last;
    void * volatile msgs[SYS_MBOX_SIZE];
    sys_sem_t read_sem;
    sys_sem_t write_sem;
    int valid;
} sys_mbox_t;

typedef void * sys_thread_t;

#endif /* LWIP_ARCH_SYS_ARCH_H */

