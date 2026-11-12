#include <SDL.h>
#include <SDL_net.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <lwip/ip4_addr.h>
#include <nxdk/net.h>
#include <windows.h>

int main(void)
{
    IPaddress local;
    UDPsocket udp;

    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    debugPrint("nxdk SDL_net sample\n");

    if (SDL_Init(0) < 0) {
        debugPrint("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    nxNetInit(NULL);

    if (SDLNet_Init() < 0) {
        debugPrint("SDLNet_Init failed: %s\n", SDLNet_GetError());
        SDL_Quit();
        return 1;
    }

    if (SDLNet_ResolveHost(&local, "127.0.0.1", 12345) == 0) {
        debugPrint("Resolved 127.0.0.1:%u\n", SDLNet_Read16(&local.port));
    } else {
        debugPrint("Resolve failed: %s\n", SDLNet_GetError());
    }

    udp = SDLNet_UDP_Open(0);
    if (udp) {
        IPaddress *bound = SDLNet_UDP_GetPeerAddress(udp, -1);
        if (bound) {
            ip4_addr_t addr;
            addr.addr = bound->host;
            debugPrint("UDP bound to %s:%u\n", ip4addr_ntoa(&addr), SDLNet_Read16(&bound->port));
        }
        SDLNet_UDP_Close(udp);
    } else {
        debugPrint("UDP open failed: %s\n", SDLNet_GetError());
    }

    SDLNet_Quit();
    SDL_Quit();

    while (1) {
        Sleep(1000);
    }

    return 0;
}
