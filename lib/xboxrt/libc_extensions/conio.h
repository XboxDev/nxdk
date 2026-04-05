// SPDX-License-Identifier: MIT

#pragma once

/*
 * Provide the tiny conio.h subset that some cross-built utility code expects.
 * The Xbox runtime does not provide a console keyboard in these call sites, so
 * the helpers intentionally report no pending input.
 */

#ifdef __cplusplus
extern "C" {
#endif

static inline int _kbhit(void)
{
    return 0;
}

static inline int _getch(void)
{
    return -1;
}

static inline int kbhit(void)
{
    return _kbhit();
}

static inline int getch(void)
{
    return _getch();
}

#ifdef __cplusplus
}
#endif
