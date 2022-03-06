// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Mara (apfel)

#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define HANDLERS_MAX 4

static nxLogFunc loggingFunctions[HANDLERS_MAX] = { 0 };

static inline size_t dispatch(const char* content, size_t length)
{
    size_t printed = 0, printed_temp = 0;
    for (uint8_t i = 0; i < HANDLERS_MAX; i++)
    {
        if (loggingFunctions[i] == NULL) continue;

        printed_temp = loggingFunctions[i](content, length);
        if (printed_temp > printed) printed = printed_temp;
    }

    return printed;
}

uint8_t nxLogRegisterFunction(nxLogFunc log_func)
{
    for (uint8_t i = 0; i < HANDLERS_MAX + 1; i++)
    {
        if (i == HANDLERS_MAX) return nxLogResult_MaximumNumberReached;
        else if (loggingFunctions[i] == log_func) return nxLogResult_AlreadyRegistered;
        else if (loggingFunctions[i] != NULL) continue;

        loggingFunctions[i] = log_func;
        break;
    }

    return nxLogResult_Success;
}

void nxLogUnregisterFunction(nxLogFunc log_func)
{
    for (uint8_t i = 0; i < HANDLERS_MAX; i++) if (loggingFunctions[i] == log_func) loggingFunctions[i] = NULL;
}

size_t nxLogPrint(const char* message)
{
    return dispatch(message, strlen(message));
}

size_t nxLogPrintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char* buf = calloc(BUFFER_SIZE, sizeof(char));
    if (buf == NULL) return nxLogResult_MemoryFailure;

    vsnprintf(buf, BUFFER_SIZE, format, args);

    va_end(args);

    size_t length = dispatch(buf, strlen(buf));

    free(buf);
    return length;
}
