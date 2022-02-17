#include <lwip/debug.h>
#include <lwip/tcpip.h>
#include <hal/video.h>
#include <hal/debug.h>
#include <nxdk/net.h>

#define DEBUGGING        0

#include "httpserver.h"

extern struct netif *g_pnetif;

int main(void)
{
	XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

	nxNetInit(NULL);

	debugPrint("\n");
	debugPrint("IP address.. %s\n", ip4addr_ntoa(netif_ip4_addr(g_pnetif)));
	debugPrint("Mask........ %s\n", ip4addr_ntoa(netif_ip4_netmask(g_pnetif)));
	debugPrint("Gateway..... %s\n", ip4addr_ntoa(netif_ip4_gw(g_pnetif)));
	debugPrint("\n");

	http_server_netconn_init();
	while (1) NtYieldExecution();
	return 0;
}
