#include <xboxkrnl/xboxkrnl.h>
#include <hal/video.h>

extern void main(void);

void XboxCRT(void) {
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    main();

    /* If user application returns, just reboot. */
    HalReturnToFirmware(HalQuickRebootRoutine);
}
