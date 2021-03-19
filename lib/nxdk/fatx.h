#ifndef __NXDK_FATX_H__
#define __NXDK_FATX_H__

#include <stdint.h>

#define FATX_SIGNATURE 'XTAF'

typedef struct _FATX_SUPERBLOCK
{
    uint32_t Signature;
    uint32_t VolumeId;
    uint32_t SectorsPerCluster;
    uint32_t FirstClusterOffset;
    uint16_t Unknown;
} __attribute__((packed)) FATX_SUPERBLOCK;

#endif
