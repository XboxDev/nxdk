/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _CRT_RAND_S
int rand_s (unsigned int *randomValue);
#endif

typedef void (__cdecl *_purecall_handler)(void);

_purecall_handler __cdecl _get_purecall_handler (void);
_purecall_handler __cdecl _set_purecall_handler (_purecall_handler function);

#ifdef __cplusplus
}
#endif
