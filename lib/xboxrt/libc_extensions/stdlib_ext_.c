#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <threads.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>

once_flag init_flag = ONCE_FLAG_INIT;
mtx_t rand_mutex;
unsigned char rand_buffer[116];

static void update_rand_buffer (void)
{
    LARGE_INTEGER performanceCounter;
    QueryPerformanceCounter(&performanceCounter);
    XcSHAUpdate(rand_buffer, (PUCHAR)&performanceCounter, sizeof(performanceCounter));
}

void init_rand_s (void)
{
    mtx_init(&rand_mutex, mtx_plain);

    XcSHAInit(rand_buffer);

    // Hash the EEPROM
    unsigned char eeprom_buffer[256];
    ULONG bytes_read;
    ULONG eeprom_type;
    ExQueryNonVolatileSetting(0xFFFF, &eeprom_type, eeprom_buffer, 256, &bytes_read);
    assert(bytes_read == 256);
    XcSHAUpdate(rand_buffer, eeprom_buffer, 256);

    update_rand_buffer();
}

// While rand_s is supposed to be cryptographically secure, this is not a very secure
// implementation. The choice of SHA-1 as the algorithm is not ideal, but convenient,
// as the kernel provides that for us. As the kernel doesn't help in collecting
// entropy, though, we're very limited in that regard.
int rand_s (unsigned int *randomValue)
{
    call_once(&init_flag, init_rand_s);

    // Microsoft's CRT has special handling for invalid parameters, which
    // nxdk doesn't have. We just act as if the invalid parameter handler
    // allowed continuing execution and make sure to assert in debug builds.
    assert(randomValue != NULL);
    if (!randomValue) {
        errno = EINVAL;
        return EINVAL;
    }

    mtx_lock(&rand_mutex);

    update_rand_buffer();

    // Extract four bytes from the hash as our random value, and feed it back
    // into the SHA algorithm
    unsigned char output_buffer[20];
    XcSHAFinal(rand_buffer, output_buffer);
    *randomValue = *((unsigned int *)output_buffer);
    XcSHAUpdate(rand_buffer, (PUCHAR)randomValue, sizeof(unsigned int));

    mtx_unlock(&rand_mutex);

    return 0;
}

unsigned short _byteswap_ushort (unsigned short val)
{
    return __builtin_bswap16(val);
}

unsigned long _byteswap_ulong (unsigned long val)
{
    return __builtin_bswap32(val);
}

unsigned __int64 _byteswap_uint64 (unsigned __int64 val)
{
    return __builtin_bswap64(val);
}


double strtod( const char * _PDCLIB_restrict nptr, char * * _PDCLIB_restrict endptr )
{
    assert(0);
    return 0.0;
}

float strtof( const char * _PDCLIB_restrict nptr, char * * _PDCLIB_restrict endptr )
{
    assert(0);
    return 0.0;
}

long double strtold( const char * _PDCLIB_restrict nptr, char * * _PDCLIB_restrict endptr )
{
    assert(0);
    return 0.0;
}

int mbtowc (wchar_t *pwc, const char *string, size_t n)
{
    assert(0);
    return 0;
}

