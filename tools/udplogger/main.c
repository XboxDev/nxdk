/// Shamelessly C-ified version of devkitPro's udplogserver from their wut-tools.
/// Thanks dimok, WinterMute and others improving wut and its tools, and unintentionally improving the Xbox alongside
/// I guess we're all just a big team after all
///
/// ...
///
/// Never thought I would thank so many people for a bit of UDP code
///
/// oh, btw, TODO: add error messages, lol

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 4405

int main(int argc, char** argv)
{
    char buffer[2048];
    memset(buffer, 0, 2048);

    unsigned short port = SERVER_PORT;
    if (argc == 2) port = atoi(argv[1]);

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR) return -1;
#endif

    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0)
    {
#ifdef _WIN32
        WSACleanup();
#endif

        return -1;
    }

    // Set non blocking
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(fd, FIONBIO, &mode);
#else
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family         = AF_INET;
    addr.sin_addr.s_addr    = htonl(INADDR_ANY);
    addr.sin_port           = htons(port);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
#ifdef _WIN32
        closesocket(fd);
        WSACleanup();
#else
        close(fd);
#endif
        return -1;
    }

    while (true)
    {
        fd_set fdsRead;
        FD_ZERO(&fdsRead);
        FD_SET(fd, &fdsRead);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;

        if (select(fd + 1, &fdsRead, NULL, NULL, &tv) == 1)
        {
            struct sockaddr_in from;
#ifdef _WIN32
            int fromLen = sizeof(from);
#else
            socklen_t fromLen = sizeof(from);
#endif

            int recvd = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &fromLen);
            if (recvd > 0)
            {
                printf("%s", buffer);
                memset(buffer, 0, 2048);
            }
        }
    }

#ifdef _WIN32
    closesocket(fd);
    WSACleanup();
#else
    close(fd);
#endif

    return 0;
}
