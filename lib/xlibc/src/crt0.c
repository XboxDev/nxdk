#include <xboxkrnl/xboxkrnl.h>
#include <hal/video.h>
#include <threads.h>
#include <_tss.h>
#include <_tls.h>

extern int main (int argc, char **argv);

static int main_wrapper ()
{
    _xlibc_tss_init();

    char *_argv=0;
    return main(0,&_argv);
}

void XboxCRTEntry ()
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    DWORD tlssize;
    // Sum up the required amount of memory, round it up to a multiple of
    // 16 bytes and add 4 bytes for the self-reference
    tlssize = (_tls_used.EndAddressOfRawData - _tls_used.StartAddressOfRawData) + _tls_used.SizeOfZeroFill;
    tlssize = (tlssize + 15) & ~15;
    tlssize += 4;
    *((DWORD *)_tls_used.AddressOfIndex) = (int)tlssize / -4;

    thrd_t main_thread;
    thrd_create(&main_thread, main_wrapper, NULL);
    thrd_join(main_thread, NULL);

    HalReturnToFirmware(HalQuickRebootRoutine);
}
