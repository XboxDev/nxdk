// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Mara (apfel)

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "log.h"

/**
 * Registers the UDP logging function.
 * This'll print messages to a UDP socket on port 4406.
 *
 * \param ip The IP address to bind to.
 * \param netmask The subnet mask for the IP address.
 * \param gateway The IP address of the gateway to connect to.
 *
 * \sa nxLogRegisterFunction
 */
uint8_t nxLogUDPRegister();

/**
 * Unregisters the UDP logging function.
 */
void nxLogUDPUnregister();

#ifdef __cplusplus
}
#endif
