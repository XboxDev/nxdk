// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Mara (apfel)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Represents the prototype of a log handler function.
 *
 * \param message The fully processed message, NULL terminated.
 * \param length The full size of the message, without the NULL terminator.
 */
typedef size_t (__cdecl *nxLogFunc)(const char* message, size_t length);

enum nxLogResult
{
    /**
     * No error occurred.
     */
    nxLogResult_Success,

    /**
     * The log handler has been registered already.
     */
    nxLogResult_AlreadyRegistered,

    /**
     * The maximum number of handlers has been reached.
     */
    nxLogResult_MaximumNumberReached,

    /**
     * A memory error has occurred.
     */
    nxLogResult_MemoryFailure,

    /**
     * Invalid arguments have been passed to the given function.
     */
    nxLogResult_InvalidArguments,

    /**
     * A socket error occurred.
     *
     * @note The underlying socket API will be uninitalized, if possible.
     */
    nxLogResult_SocketFailure,

    /**
     * The framebuffer couldn't be retrieved.
     */
    nxLogResult_FramebufferRetrievalFailure
};

/**
 * Registers a log function.
 *
 * @param log_func The log function that'll be registered.
 */
uint8_t nxLogRegisterFunction(nxLogFunc log_func);

/**
 * Unregisters a log function.
 * This doesn't require the given function to be registered first.
 *
 * @param log_func The log function that'll be unregistered.
 */
void nxLogUnregisterFunction(nxLogFunc log_func);

/**
 * Prints the raw message on screen.
 *
 * @param message The message to print.
 *
 * @return Returns the number of characters that have been printed.
 *
 * @note The returned number of characters is based on which log handler returned the highest value.
 */
size_t nxLogPrint(const char* message);

/**
 * Prints a formatted message on screen.
 *
 * @param format The format of the message.
 *
 * @return Returns the number of characters that have been printed.
 *
 * @note The returned number of characters is based on which log handler returned the highest value.
 * @note Consider using @ref nxLogPrint when formatting is not necessary.
 */
size_t nxLogPrintf(const char* format, ...) __attribute__((format(printf, 1, 2)));

#ifdef __cplusplus
}
#endif
