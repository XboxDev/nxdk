// pbKit pushbuffer utility functions

// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2007 Guillaume Lamonoca
// SPDX-FileCopyrightText: 2017 espes
// SPDX-FileCopyrightText: 2017-2020 Jannik Vogel
// SPDX-FileCopyrightText: 2018-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Lucas Jansson
// SPDX-FileCopyrightText: 2021-2025 Erik Abair

#include "pbkit_pushbuffer.h"

uint32_t *pb_push1_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD param1)
{
    pb_push_to(subchannel, p, command, 1);
    *(p + 1) = param1;
    return p + 2;
}

uint32_t *pb_push2_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2)
{
    pb_push_to(subchannel, p, command, 2);
    *(p + 1) = param1;
    *(p + 2) = param2;
    return p + 3;
}

uint32_t *pb_push3_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3)
{
    pb_push_to(subchannel, p, command, 3);
    *(p + 1) = param1;
    *(p + 2) = param2;
    *(p + 3) = param3;
    return p + 4;
}

uint32_t *pb_push4_to (DWORD subchannel, uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3,
                       DWORD param4)
{
    pb_push_to(subchannel, p, command, 4);
    *(p + 1) = param1;
    *(p + 2) = param2;
    *(p + 3) = param3;
    *(p + 4) = param4;
    return p + 5;
}

uint32_t *pb_push4f_to (DWORD subchannel, uint32_t *p, DWORD command, float param1, float param2, float param3,
                        float param4)
{
    pb_push_to(subchannel, p, command, 4);
    *((float *)(p + 1)) = param1;
    *((float *)(p + 2)) = param2;
    *((float *)(p + 3)) = param3;
    *((float *)(p + 4)) = param4;
    return p + 5;
}

void pb_push (uint32_t *p, DWORD command, DWORD nparam)
{
    pb_push_to(SUBCH_3D, p, command, nparam);
}

uint32_t *pb_push1 (uint32_t *p, DWORD command, DWORD param1)
{
    return pb_push1_to(SUBCH_3D, p, command, param1);
}

uint32_t *pb_push2 (uint32_t *p, DWORD command, DWORD param1, DWORD param2)
{
    return pb_push2_to(SUBCH_3D, p, command, param1, param2);
}

uint32_t *pb_push3 (uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3)
{
    return pb_push3_to(SUBCH_3D, p, command, param1, param2, param3);
}

uint32_t *pb_push4 (uint32_t *p, DWORD command, DWORD param1, DWORD param2, DWORD param3, DWORD param4)
{
    return pb_push4_to(SUBCH_3D, p, command, param1, param2, param3, param4);
}

uint32_t *pb_push4f (uint32_t *p, DWORD command, float param1, float param2, float param3, float param4)
{
    return pb_push4f_to(SUBCH_3D, p, command, param1, param2, param3, param4);
}

uint32_t *pb_push_transposed_matrix (uint32_t *p, DWORD command, const float *m)
{
    pb_push_to(SUBCH_3D, p++, command, 16);

    *((float *)p++) = m[_11];
    *((float *)p++) = m[_21];
    *((float *)p++) = m[_31];
    *((float *)p++) = m[_41];

    *((float *)p++) = m[_12];
    *((float *)p++) = m[_22];
    *((float *)p++) = m[_32];
    *((float *)p++) = m[_42];

    *((float *)p++) = m[_13];
    *((float *)p++) = m[_23];
    *((float *)p++) = m[_33];
    *((float *)p++) = m[_43];

    *((float *)p++) = m[_14];
    *((float *)p++) = m[_24];
    *((float *)p++) = m[_34];
    *((float *)p++) = m[_44];

    return p;
}

uint32_t *pb_push_4x3_matrix (uint32_t *p, DWORD command, const float *m)
{
    pb_push_to(SUBCH_3D, p++, command, 12);

    *((float *)p++) = m[_11];
    *((float *)p++) = m[_12];
    *((float *)p++) = m[_13];
    *((float *)p++) = m[_14];

    *((float *)p++) = m[_21];
    *((float *)p++) = m[_22];
    *((float *)p++) = m[_23];
    *((float *)p++) = m[_24];

    *((float *)p++) = m[_31];
    *((float *)p++) = m[_32];
    *((float *)p++) = m[_33];
    *((float *)p++) = m[_34];

    return p;
}

uint32_t *pb_push_4x4_matrix (uint32_t *p, DWORD command, const float *m)
{
    pb_push_to(SUBCH_3D, p++, command, 16);

    *((float *)p++) = m[_11];
    *((float *)p++) = m[_12];
    *((float *)p++) = m[_13];
    *((float *)p++) = m[_14];

    *((float *)p++) = m[_21];
    *((float *)p++) = m[_22];
    *((float *)p++) = m[_23];
    *((float *)p++) = m[_24];

    *((float *)p++) = m[_31];
    *((float *)p++) = m[_32];
    *((float *)p++) = m[_33];
    *((float *)p++) = m[_34];

    *((float *)p++) = m[_41];
    *((float *)p++) = m[_42];
    *((float *)p++) = m[_43];
    *((float *)p++) = m[_44];

    return p;
}

uint32_t *pb_push1f (uint32_t *p, DWORD command, float param1)
{
    pb_push_to(SUBCH_3D, p, command, 1);
    *((float *)(p + 1)) = param1;
    return p + 2;
}

uint32_t *pb_push2f (uint32_t *p, DWORD command, float param1, float param2)
{
    pb_push_to(SUBCH_3D, p, command, 2);
    *((float *)(p + 1)) = param1;
    *((float *)(p + 2)) = param2;
    return p + 3;
}

uint32_t *pb_push3f (uint32_t *p, DWORD command, float param1, float param2, float param3)
{
    pb_push_to(SUBCH_3D, p, command, 3);
    *((float *)(p + 1)) = param1;
    *((float *)(p + 2)) = param2;
    *((float *)(p + 3)) = param3;
    return p + 4;
}

uint32_t *pb_push2fv (uint32_t *p, DWORD command, const float *vector2)
{
    pb_push_to(SUBCH_3D, p, command, 2);
    *((float *)(p + 1)) = *vector2++;
    *((float *)(p + 2)) = *vector2++;
    return p + 3;
}

uint32_t *pb_push3fv (uint32_t *p, DWORD command, const float *vector3)
{
    pb_push_to(SUBCH_3D, p, command, 3);
    *((float *)(p + 1)) = *vector3++;
    *((float *)(p + 2)) = *vector3++;
    *((float *)(p + 3)) = *vector3++;
    return p + 4;
}

uint32_t *pb_push4fv (uint32_t *p, DWORD command, const float *vector4)
{
    pb_push_to(SUBCH_3D, p, command, 4);
    *((float *)(p + 1)) = *vector4++;
    *((float *)(p + 2)) = *vector4++;
    *((float *)(p + 3)) = *vector4++;
    *((float *)(p + 4)) = *vector4++;
    return p + 5;
}

uint32_t *pb_push2v (uint32_t *p, DWORD command, const DWORD *vector2)
{
    return pb_push2(p, command, vector2[0], vector2[1]);
}

uint32_t *pb_push3v (uint32_t *p, DWORD command, const DWORD *vector3)
{
    return pb_push3(p, command, vector3[0], vector3[1], vector3[2]);
}

uint32_t *pb_push4v (uint32_t *p, DWORD command, const DWORD *vector4)
{
    return pb_push4(p, command, vector4[0], vector4[1], vector4[2], vector4[3]);
}
