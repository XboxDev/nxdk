#include <stdbool.h>
#include <nxdk/log_udp.h>
#include <windows.h>

int main(void)
{
    nxLogUDPRegister(NULL, NULL, NULL);

    while (true)
    {
        nxLogPrint("Hello nxdk! hopefully this connection is reliable enough\n");
        Sleep(1000);
    }

    return 0;
}
