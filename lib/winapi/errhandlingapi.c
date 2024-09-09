// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include <string.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>

VOID RaiseException (DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR *lpArguments)
{
    EXCEPTION_RECORD exception_record;

    exception_record.ExceptionCode = dwExceptionCode;
    exception_record.ExceptionRecord = NULL;
    exception_record.ExceptionAddress = (PVOID)RaiseException;
    exception_record.ExceptionFlags = dwExceptionFlags & EXCEPTION_NONCONTINUABLE;

    if (!lpArguments) {
        exception_record.NumberParameters = 0;
    } else {
        if (nNumberOfArguments > EXCEPTION_MAXIMUM_PARAMETERS) {
            nNumberOfArguments = EXCEPTION_MAXIMUM_PARAMETERS;
        }

        exception_record.NumberParameters = nNumberOfArguments;
        memcpy(exception_record.ExceptionInformation, lpArguments, nNumberOfArguments * sizeof(ULONG));
    }

    RtlRaiseException(&exception_record);
}
