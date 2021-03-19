#ifndef __NXDK_FORMAT_H__
#define __NXDK_FORMAT_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <stdint.h>

/**
 * Formats a volume with FATX.
 * WARNING: This is a destructive operation, incorrect use can lead to unexpected data loss!
 * @param volumePath The NT-style path to the volume that is about to be formatted
 * @param bytesPerCluster Specifies the number of bytes per cluster, pass 0 for default
 * @return True on success, false on error. When false, additional error information is available from GetLastError()
 */
bool nxFormatVolume (const char *volumePath, uint32_t bytesPerCluster);

#ifdef __cplusplus
}
#endif

#endif
