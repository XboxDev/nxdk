#include <hal/io.h>
#include <hal/xbox.h>
#include <xboxkrnl/xboxkrnl.h>
#include "usb/include/cromwell_types.h"

void wait_ms(DWORD ticks);

/*------------------------------------------------------------------------*/ 
void my_wait_ms(int x)
{
        wait_ms(x);
}
/*------------------------------------------------------------------------*/ 
void my_mdelay(int x)
{
        wait_ms(x);
}
/*------------------------------------------------------------------------*/
void my_udelay(int x)
{
        wait_ms(1+x/1000);
}

void wait_ms(DWORD ticks) {

        /*
                32 Bit range = 1200 sec ! => 20 min
                1. sec = 0x369E99
                1 ms =  3579,545

        */

        DWORD COUNT_start;
        DWORD temp;
        DWORD COUNT_TO;
        DWORD HH;

        // Maximum Input range
        if (ticks>(1200*1000)) ticks = 1200*1000;

        COUNT_TO = (DWORD) ((float)(ticks*3579.545));
        COUNT_start = IoInputDword(0x8008);

        while(1) {

                // Reads out the System timer
                HH = IoInputDword(0x8008);
                temp = HH-COUNT_start;
                // We reached the counter
                if (temp>COUNT_TO) break;

        };


}
