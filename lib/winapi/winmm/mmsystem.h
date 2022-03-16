// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Jannik Vogel

#ifndef __WINMM_MMSYSTEM_H__
#define __WINMM_MMSYSTEM_H__

#include <windef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define MMSYSERR_NOERROR 0

typedef UINT MMRESULT;

#include <timeapi.h>

#ifdef __cplusplus
}
#endif

#endif
