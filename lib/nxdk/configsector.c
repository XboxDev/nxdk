// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2021 Luke Usher
// SPDX-FileCopyrightText: 2022 Stefan Schmidt

#include "configsector.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <winbase.h>
#include <xboxkrnl/xboxkrnl.h>

#define NXDK_HDD_SECTOR_SIZE 512

#define NXDK_NETWORK_CONFIG_SECTOR 0
#define NXDK_MACHINE_ACCOUNT_CONFIG_SECTOR 1
#define NXDK_USER_ACCOUNT_CONFIG_SECTOR 2
#define NXDK_CONFIG_SECTOR_COUNT 23

#define NXDK_CONFIG_SECTOR_HEADER 0x79132568
#define NXDK_CONFIG_SECTOR_FOOTER 0xAA550000
#define NXDK_CONFIG_DATA_SIZE 492

typedef struct
{
    DWORD header;
    DWORD unknown1; // Always set to 1?
    DWORD unknown2; // Always set to 1?
    CHAR data[NXDK_CONFIG_DATA_SIZE];
    DWORD checksum;
    DWORD footer;
}  __attribute__ ((packed)) NXDK_CONFIG_SECTOR;

_Static_assert(sizeof(NXDK_CONFIG_SECTOR) == NXDK_HDD_SECTOR_SIZE, "NXDK_CONFIG_SECTOR size mismatch");
_Static_assert(sizeof(nxdk_network_config_sector_t) == NXDK_CONFIG_DATA_SIZE, "nxdk_network_config_sector_t size mismatch");

#define NXDK_NETWORK_CONFIG_HEADERV2 'XBV2'
#define NXDK_NETWORK_CONFIG_FOOTER 'XBCP'

HANDLE nxOpenConfigPartition(void)
{
    ANSI_STRING str;
    RtlInitAnsiString(&str, "\\Device\\Harddisk0\\partition0");

    OBJECT_ATTRIBUTES obj;
    InitializeObjectAttributes(&obj, &str, OBJ_CASE_INSENSITIVE, NULL, NULL);

    HANDLE hPartition;
    IO_STATUS_BLOCK statusBlock;

    NTSTATUS status = NtOpenFile(&hPartition, GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE, &obj, &statusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_SYNCHRONOUS_IO_ALERT);
    if (!NT_SUCCESS(status)) {
        return INVALID_HANDLE_VALUE;
    }

    return hPartition;
}

void nxCloseConfigPartition (HANDLE hPartition)
{
    if (hPartition != INVALID_HANDLE_VALUE) {
        NtClose(hPartition);
    }
}

DWORD nxNetworkConfigChecksum(NXDK_CONFIG_SECTOR *sectorData, UINT size)
{
    DWORD* data = (DWORD *)sectorData;

    DWORD sum = 0;
    DWORD carry = 0;

    for (int i = size >> 2; i; --i) {
        carry += (*data > (*data + sum));

        sum += *data;
        data++;
    }

    return (carry > (carry + sum)) + carry + sum;
}

bool nxLoadNetworkConfigSector(HANDLE hPartition, UINT sectorIndex, BYTE *buffer, UINT size)
{
    NXDK_CONFIG_SECTOR configSector;

    assert(hPartition != INVALID_HANDLE_VALUE);
    if (hPartition == INVALID_HANDLE_VALUE) {
        return false;
    }

    assert(sectorIndex < NXDK_CONFIG_SECTOR_COUNT);
    if (sectorIndex >= NXDK_CONFIG_SECTOR_COUNT) {
        return false;
    }

    if (size != NXDK_CONFIG_DATA_SIZE) {
        return false;
    }

    LARGE_INTEGER offset;
    offset.QuadPart = (8 + sectorIndex) * NXDK_HDD_SECTOR_SIZE;

    IO_STATUS_BLOCK statusBlock;
    NTSTATUS status = NtReadFile(hPartition, 0, NULL, NULL, &statusBlock, &configSector, NXDK_HDD_SECTOR_SIZE, &offset);

    if (!NT_SUCCESS(status)) {
        return false;
    }

    if (configSector.header != NXDK_CONFIG_SECTOR_HEADER ||
        configSector.footer != NXDK_CONFIG_SECTOR_FOOTER ||
        configSector.unknown1 != 1 ||
        configSector.unknown2 != 1) {
        return false;
    }

    DWORD checksum = configSector.checksum;
    configSector.checksum = 0;
    DWORD newChecksum =  ~nxNetworkConfigChecksum(&configSector, NXDK_HDD_SECTOR_SIZE);
    if (checksum != newChecksum) {
        return false;
    }
    configSector.checksum = checksum;

    memcpy(buffer, configSector.data, NXDK_CONFIG_DATA_SIZE);

    return true;
}

bool nxLoadNetworkConfig(nxdk_network_config_sector_t *config)
{
    HANDLE hPartition = nxOpenConfigPartition();

    assert(hPartition != INVALID_HANDLE_VALUE);
    if (hPartition == INVALID_HANDLE_VALUE) {
        return false;
    }

    nxLoadNetworkConfigSector(hPartition, NXDK_NETWORK_CONFIG_SECTOR, (BYTE *)config, NXDK_CONFIG_DATA_SIZE);

    // If the footer is invalid, config is invalid
    if (config->footer != NXDK_NETWORK_CONFIG_FOOTER) {
        memset(config, 0, sizeof(nxdk_network_config_sector_t));
        config->footer = NXDK_NETWORK_CONFIG_FOOTER;
    }

    nxCloseConfigPartition(hPartition);

    return true;
}

bool nxSaveNetworkConfigSector(HANDLE hPartition, UINT sectorIndex, const BYTE *buffer, UINT size)
{
    NXDK_CONFIG_SECTOR configSector;

    assert(hPartition != INVALID_HANDLE_VALUE);
    if (hPartition == INVALID_HANDLE_VALUE) {
        return false;
    }

    assert(sectorIndex < NXDK_CONFIG_SECTOR_COUNT);
    if (sectorIndex >= NXDK_CONFIG_SECTOR_COUNT) {
        return false;
    }

    if (size != NXDK_CONFIG_DATA_SIZE) {
        return false;
    }

    LARGE_INTEGER offset;
    offset.QuadPart = (8 + sectorIndex) * NXDK_HDD_SECTOR_SIZE;

    configSector.header = NXDK_CONFIG_SECTOR_HEADER;
    configSector.footer = NXDK_CONFIG_SECTOR_FOOTER;
    configSector.unknown1 = 1;
    configSector.unknown2 = 1;
    configSector.checksum = 0;

    memcpy(configSector.data, buffer, NXDK_CONFIG_DATA_SIZE);

    configSector.checksum = ~nxNetworkConfigChecksum(&configSector, NXDK_HDD_SECTOR_SIZE);

    IO_STATUS_BLOCK statusBlock;
    NTSTATUS status = NtWriteFile(hPartition, 0, NULL, NULL, &statusBlock, &configSector, NXDK_HDD_SECTOR_SIZE, &offset);

    if (!NT_SUCCESS(status)) {
        return false;
    }

    return true;
}

bool nxSaveNetworkConfig(const nxdk_network_config_sector_t *config)
{
    HANDLE hPartition = nxOpenConfigPartition();

    assert(hPartition != INVALID_HANDLE_VALUE);
    if (hPartition == INVALID_HANDLE_VALUE) {
        return false;
    }

    nxdk_network_config_sector_t configVar;
    memcpy(&configVar, config, sizeof(configVar));
    configVar.headerV2 = NXDK_NETWORK_CONFIG_HEADERV2;
    configVar.footer = NXDK_NETWORK_CONFIG_FOOTER;
    XcHMAC(XboxHDKey, XBOX_KEY_LENGTH, (PUCHAR)&configVar.headerV2, 0x1b0, (PUCHAR)&configVar.headerV2, 4, configVar.hmac);

    bool result = nxSaveNetworkConfigSector(hPartition, 0, (const BYTE *)&configVar, sizeof(nxdk_network_config_sector_t));
    nxCloseConfigPartition(hPartition);
    return result;
}
