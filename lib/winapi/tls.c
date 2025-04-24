// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include <windows.h>

DWORD TlsAlloc (void)
{
    return FlsAlloc(NULL);
}

BOOL TlsFree (DWORD dwTlsIndex)
{
    return FlsFree(dwTlsIndex);
}

LPVOID TlsGetValue (DWORD dwTlsIndex)
{
    return FlsGetValue(dwTlsIndex);
}

BOOL TlsSetValue (DWORD dwTlsIndex, LPVOID lpTlsValue)
{
    return FlsSetValue(dwTlsIndex, lpTlsValue);
}
