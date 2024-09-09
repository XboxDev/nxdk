// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Jannik Vogel
// SPDX-FileCopyrightText: 2021 Stefan Schmidt

#ifndef __WINMM_TIMEAPI_H__
#define __WINMM_TIMEAPI_H__

#include <mmsystem.h>
#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIMERR_NOERROR 0
#define TIMERR_NOCANDO 97

MMRESULT timeBeginPeriod (UINT uPeriod);
MMRESULT timeEndPeriod (UINT uPeriod);
DWORD timeGetTime (void);

#ifdef __cplusplus
}
#endif

#endif
