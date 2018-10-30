#include "xboxkrnl/xboxkrnl.h"
#include "_tls.h"

ULONG _tls_index = 0;

#pragma data_seg(".tls")
char _tls_start = 0;
#pragma data_seg(".tls$ZZZ")
char _tls_end = 0;

#pragma data_seg(".rdata")
const IMAGE_TLS_DIRECTORY _tls_used =
{
    (DWORD) &_tls_start,
    (DWORD) &_tls_end,
    (DWORD) &_tls_index,
    (DWORD) NULL,
    (DWORD) 0,
    (DWORD) 0
};
