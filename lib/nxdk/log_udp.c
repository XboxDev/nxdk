// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Mara (apfel)

#include "log_udp.h"

#include <lwip/sockets.h>

#define UDP_LOG_PORT 4405

static int udp_socket;
static struct sockaddr_in udp_sockaddr;

size_t udp_log_function(const char* message, size_t length)
{
    return sendto(udp_socket, message, length, 0, (struct sockaddr*)&udp_sockaddr, sizeof(struct sockaddr_in));
}

uint8_t nxLogUDPRegister()
{
    uint8_t reg_result = nxLogRegisterFunction(&udp_log_function);
    nxLogUnregisterFunction(&udp_log_function);
    switch (reg_result)
    {
    case nxLogResult_AlreadyRegistered:
        return nxLogResult_AlreadyRegistered;

    case nxLogResult_MaximumNumberReached:
        return nxLogResult_MaximumNumberReached;

    default:
        break;
    }

    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket < 0) return nxLogResult_SocketFailure;

    BOOL broadcastEnable = TRUE;
    setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(BOOL));

    memset(&udp_sockaddr, 0, sizeof(struct sockaddr_in));

    udp_sockaddr.sin_family         = AF_INET;
    udp_sockaddr.sin_port           = htons(UDP_LOG_PORT);
    udp_sockaddr.sin_addr.s_addr    = htonl(INADDR_BROADCAST);

    reg_result = nxLogRegisterFunction(&udp_log_function);
    if (reg_result != nxLogResult_Success) shutdown(udp_socket, SHUT_RDWR);

    return reg_result;
}

void nxLogUDPUnregister()
{
    shutdown(udp_socket, SHUT_RDWR);
    nxLogUnregisterFunction(&udp_log_function);
}
