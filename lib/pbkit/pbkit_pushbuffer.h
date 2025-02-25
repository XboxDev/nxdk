// pbKit pushbuffer utility functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021-2025 Erik Abair

#ifndef PBKIT_PUSHBUFFER_H
#define PBKIT_PUSHBUFFER_H

#include <stdint.h>
#include <xboxkrnl/xboxkrnl.h>

#if defined(__cplusplus)
extern "C" {
#endif

// 4x4 matrix indices
#define _11 0
#define _12 1
#define _13 2
#define _14 3
#define _21 4
#define _22 5
#define _23 6
#define _24 7
#define _31 8
#define _32 9
#define _33 10
#define _34 11
#define _41 12
#define _42 13
#define _43 14
#define _44 15

// GPU subchannels
#define SUBCH_3D   0
#define SUBCH_2    2
#define SUBCH_3    3
#define SUBCH_4    4
#define NEXT_SUBCH 5

// push buffer size, must be >64Kb and a power of 2
#define PBKIT_PUSHBUFFER_SIZE 512 * 1024

// Start a block of pushbuffer commands.
// There is a hard limit of PBKIT_PUSHBUFFER_SIZE between flushes, so avoid pushing more than PBKIT_PUSHBUFFER_SIZE /
// 4096 DWORDs per block.
uint32_t *pb_begin (void);

// Pushes the given command to the given subchannel with nparam following DWORDs as parameters.
void pb_push_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD nparam);

// Pushes the given command and param to the given subchannel, returning a pointer to the next pushbuffer index to
// facilitate chaining.
uint32_t *pb_push1_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD param1);

// Pushes the given command and params to the given subchannel, returning a pointer to the next pushbuffer index to
// facilitate chaining.
uint32_t *pb_push2_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2);
// Pushes the given command and params to the given subchannel, returning a pointer to the next pushbuffer index to
// facilitate chaining.
uint32_t *pb_push3_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3);
// Pushes the given command and params to the given subchannel, returning a pointer to the next pushbuffer index to
// facilitate chaining.
uint32_t *pb_push4_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3,
                       DWORD param4);

// Pushes the given command and floating point params to the given subchannel, returning a pointer to the next
// pushbuffer index to facilitate chaining.
uint32_t *pb_push4f_to (DWORD subchannel, uint32_t *p, DWORD command, float param1, float param2, float param3,
                        float param4);

// Pushes the given command and floating point param to the subchannel assigned for 3D operations, returning a pointer
// to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push1f (uint32_t *p, DWORD command, float param1);
// Pushes the given command and floating point params to the subchannel assigned for 3D operations, returning a pointer
// to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push2f (uint32_t *p, DWORD command, float param1, float param2);
// Pushes the given command and floating point params to the subchannel assigned for 3D operations, returning a pointer
// to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push3f (uint32_t *p, DWORD command, float param1, float param2, float param3);
// Pushes the given command and floating point params to the subchannel assigned for 3D operations, returning a pointer
// to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push4f (uint32_t *p, DWORD command, float param1, float param2, float param3, float param4);

// Pushes the given command and 2-element vector of floating point params to the subchannel assigned for 3D operations,
// returning a pointer to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push2fv (uint32_t *p, DWORD command, const float *vector2);
// Pushes the given command and 3-element vector of floating point params to the subchannel assigned for 3D operations,
// returning a pointer to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push3fv (uint32_t *p, DWORD command, const float *vector3);
// Pushes the given command and 4-element vector of floating point params to the subchannel assigned for 3D operations,
// returning a pointer to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push4fv (uint32_t *p, DWORD command, const float *vector4);

// Pushes the given command and 2-element vector of DWORD params to the subchannel assigned for 3D operations, returning
// a pointer to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push2v (uint32_t *p, DWORD command, const DWORD *vector2);
// Pushes the given command and 3-element vector of DWORD params to the subchannel assigned for 3D operations, returning
// a pointer to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push3v (uint32_t *p, DWORD command, const DWORD *vector3);
// Pushes the given command and 4-element vector of DWORD params to the subchannel assigned for 3D operations, returning
// a pointer to the next pushbuffer index to facilitate chaining.
uint32_t *pb_push4v (uint32_t *p, DWORD command, const DWORD *vector4);

// Pushes the given command to the subchannel assigned for 3D with nparam following DWORDs as parameters.
void pb_push (uint32_t *p, DWORD command, DWORD nparam);
// Pushes the given command and param to the subchannel assigned for 3D operations, returning a pointer to the next
// pushbuffer index to facilitate chaining.
uint32_t *pb_push1 (uint32_t *p, DWORD command, DWORD param1);
// Pushes the given command and params to the subchannel assigned for 3D operations, returning a pointer to the next
// pushbuffer index to facilitate chaining.
uint32_t *pb_push2 (uint32_t *p, DWORD command, DWORD param1, DWORD param2);
// Pushes the given command and params to the subchannel assigned for 3D operations, returning a pointer to the next
// pushbuffer index to facilitate chaining.
uint32_t *pb_push3 (uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3);
// Pushes the given command and params to the subchannel assigned for 3D operations, returning a pointer to the next
// pushbuffer index to facilitate chaining.
uint32_t *pb_push4 (uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3, DWORD param4);

// Pushes the given command and 4x4 floating point matrix to the subchannel assigned for 3D operations, returning a
// pointer to the next pushbuffer index to facilitate chaining. The matrix is pushed in transposed order.
uint32_t *pb_push_transposed_matrix (uint32_t *p, DWORD command, const float *m);

// Pushes a 4 column x 3 row matrix (used to push the inverse model view matrix which intentionally omits the 4th row
// and is not transposed).
uint32_t *pb_push_4x3_matrix (uint32_t *p, DWORD command, const float *m);

// Pushes a 4x4 matrix without transposing it.
uint32_t *pb_push_4x4_matrix (uint32_t *p, DWORD command, const float *m);

// Ends a block of pushbuffer commands that was started via pb_begin, sending queued data to the GPU.
void pb_end (uint32_t *pEnd);

#if defined(__cplusplus)
}
#endif

#endif // PBKIT_PUSHBUFFER_H
