// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2021 Luke Usher
// SPDX-FileCopyrightText: 2022 Stefan Schmidt

#ifndef __NXDK_CONFIGSECTOR_H__
#define __NXDK_CONFIGSECTOR_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NXDK_NETWORK_CONFIG_MANUAL_IP 0x00000004
#define NXDK_NETWORK_CONFIG_MANUAL_DNS 0x00000008

typedef struct
{
    uint8_t unknown1[0x40];

    uint32_t dhcpFlags;
    uint32_t unknown2;

    struct
    {
        uint32_t ip;
        uint32_t subnetMask;
        uint32_t defaultGateway;
        uint32_t primaryDns;
        uint32_t secondaryDns;
    } __attribute__ ((packed)) manual;

    uint8_t unknown3[0x104];

    struct
    {
        uint32_t ip;
        uint32_t subnetMask;
        uint32_t defaultGateway;
        uint32_t primaryDns;
        uint32_t secondaryDns;
    } __attribute__ ((packed)) automatic;

    uint8_t unknown4[0x74];
    uint32_t footer;
} __attribute__ ((packed)) nxdk_network_config_sector_t;

bool nxLoadNetworkConfig(nxdk_network_config_sector_t *config);

#ifdef __cplusplus
}
#endif

#endif
