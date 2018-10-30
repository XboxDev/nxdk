#include "xboxkrnl/xboxkrnl.h"

#ifndef _XLIBC_RT__TLS_H
#define _XLIBC_RT__TLS_H

typedef struct _IMAGE_TLS_DIRECTORY
{
    DWORD StartAddressOfRawData;
    DWORD EndAddressOfRawData;
    DWORD AddressOfIndex;
    DWORD AddressOfCallBacks;
    DWORD SizeOfZeroFill;
    DWORD Characteristics;
} IMAGE_TLS_DIRECTORY;

extern const IMAGE_TLS_DIRECTORY _tls_used;

#endif /* end of include guard: _XLIBC_RT__TLS_H */
