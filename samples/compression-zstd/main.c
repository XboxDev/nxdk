#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hal/debug.h>
#include <hal/video.h>
#include <hal/input.h>
#include <hal/xbox.h>

#include <xboxkrnl/xboxkrnl.h>

#include <zstd.h>

const char* example = "This text has been compressed on the Microsoft Xbox using Zstandard. The application was built with the nxdk.";

int main(void)
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    size_t size = strlen(example), compressed_size = ZSTD_compressBound(strlen(example));

    char* compressed_buffer = calloc(compressed_size + 1, sizeof(char));
    memset(compressed_buffer, 0, (compressed_size + 1) * sizeof(char));

    int code = ZSTD_compress(compressed_buffer, compressed_size, example, size, 16);
    debugPrint("Compression result code: %d\n", code);
    debugPrint("Original size: %lu\n", size);
    debugPrint("Compressed size: %lu\n", compressed_size);

    debugPrint("Compressed data:\n");
    debugPrintHex(compressed_buffer, compressed_size);
    debugPrint("\n");

    char* decompressed_buffer = calloc(size + 1, sizeof(char));
    code = ZSTD_decompress(decompressed_buffer, size, compressed_buffer, compressed_size);
    debugPrint("Decompression result code: %d\n", code);
    debugPrint("Decompressed data:\n\"%s\"\n", decompressed_buffer);

    free(decompressed_buffer);
    free(compressed_buffer);

    while (TRUE) NtYieldExecution();

    return 0;
}
