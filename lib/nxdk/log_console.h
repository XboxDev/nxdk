// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Mara (apfel)

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "log.h"

/**
 * Registers the console logging function.
 * This'll print messages to the connected TV.
 *
 * \sa nxLogRegisterFunction
 */
uint8_t nxLogConsoleRegister();

/**
 * Unregisters the console logging function.
 */
void nxLogConsoleUnregister();

/**
 * Moves the cursor on the screen.
 *
 * \param x The x coordinate to move to.
 * \param y The y coordinate to move to.
 *
 * \returns \ref nxLogResult_Success in case of success.
 * \returns \ref nxLogResult_InvalidArguments in case \c x or \c y were too small (< 25px) or too large (> width - 25px or height - 25px respectively).
 *
 * \note \ref nxLogConsoleRegister should be called first.
 */
uint8_t nxLogConsoleMoveCursor(uint16_t x, uint16_t y);

/**
 * Clears the screen.
 *
 * \returns \ref nxLogResult_Success in case of success.
 * \returns \ref nxLogResult_FramebufferRetrievalFailure in case the framebuffer could not be retrieved.
 *
 * \note Does not require \ref nxLogConsoleRegister to be called.
 * \note Requires initializing the framebuffer ahead of time with \ref XVideoSetMode.
 */
uint8_t nxLogConsoleClear();

/**
 * Advances the current console by a single line.
 *
 * \returns \ref nxLogResult_Success in case of success.
 * \returns \ref nxLogResult_FramebufferRetrievalFailure in case the framebuffer could not be retrieved.
 *
 * \note Does not require \ref nxLogConsoleRegister to be called.
 * \note Requires initializing the framebuffer ahead of time with \ref XVideoSetMode.
 */
uint8_t nxLogConsoleAdvance();

#ifdef __cplusplus
}
#endif
