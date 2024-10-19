#define DHCP


#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include <lwip/debug.h>
#ifdef DHCP
#include <lwip/dhcp.h>
#else
#include <lwip/autoip.h>
#endif
#include <lwip/init.h>
#include <lwip/netif.h>
#include <lwip/sys.h>
#include <lwip/tcpip.h>
#include <lwip/timeouts.h>
#include <netif/etharp.h>
#include <pktdrv.h>
#include <xboxkrnl/xboxkrnl.h>

#include <hal/debug.h>

#include "winsock2.h"


#define PKT_TMR_INTERVAL 5 /* ms */
#define DEBUGGING        0

struct netif nforce_netif, *g_pnetif;

err_t nforceif_init(struct netif *netif);
static void packet_timer(void *arg);

static void tcpip_init_done(void *arg)
{
    sys_sem_t *init_complete = arg;
    sys_sem_signal(init_complete);
}

static void packet_timer(void *arg)
{
    LWIP_UNUSED_ARG(arg);
    Pktdrv_ReceivePackets();
    sys_timeout(PKT_TMR_INTERVAL, packet_timer, NULL);
}

#include <windows.h>
#include <assert.h>

static int map_error_to_winsock(int error) {
  switch(error) {
#define MAP_ERROR(error_name) case error_name: return WSA ## error_name;
#undef MAP_ERROR
  default:
    debugPrint("unhandled error: %d\n", error);
    assert(0);
    break;
  }
  return -1;
}


int WSAStartup(
  WORD      wVersionRequired,
  LPWSADATA lpWSAData
) {
  assert(wVersionRequired == MAKEDWORD(1,1));
  lpWSAData->wVersion = wVersionRequired;
  lpWSAData->wHighVersion = MAKEDWORD(1,1);





    sys_sem_t init_complete;
    const ip4_addr_t *ip;
    static ip4_addr_t ipaddr, netmask, gw;

#if DEBUGGING
    asm volatile ("jmp .");
    debug_flags = LWIP_DBG_ON;
#else
    debug_flags = 0;
#endif

    /* Initialize the TCP/IP stack. Wait for completion. */
    sys_sem_new(&init_complete, 0);
    tcpip_init(tcpip_init_done, &init_complete);
    sys_sem_wait(&init_complete);
    sys_sem_free(&init_complete);

    g_pnetif = netif_add(&nforce_netif, &ipaddr, &netmask, &gw,
                         NULL, nforceif_init, ethernet_input);
    if (!g_pnetif) {
        debugPrint("netif_add failed\n");
        return 1;
    }

    netif_set_default(g_pnetif);
    netif_set_up(g_pnetif);

#ifdef DHCP
    dhcp_start(g_pnetif);
#else
    autoip_start(g_pnetif);
#endif

    packet_timer(NULL);


#ifdef DHCP
    debugPrint("Waiting for IP (DHCP)...\n");
    while (dhcp_supplied_address(g_pnetif) == 0)
        NtYieldExecution();
#else
    debugPrint("Waiting for IP (AUTOIP)...\n");
    while (autoip_supplied_address(g_pnetif) == 0)
        NtYieldExecution();
#endif
    debugPrint("IP bound!\n");

    debugPrint("\n");
    debugPrint("IP address.. %s\n", ip4addr_ntoa(netif_ip4_addr(g_pnetif)));
    debugPrint("Mask........ %s\n", ip4addr_ntoa(netif_ip4_netmask(g_pnetif)));
    debugPrint("Gateway..... %s\n", ip4addr_ntoa(netif_ip4_gw(g_pnetif)));
    debugPrint("\n");



  return 0;
}


static int last_winsock_error = 0;

int WSAGetLastError() {
  return last_winsock_error;
}

void WSASetLastError(
  int iError
) {
  last_winsock_error = iError;
}

int WSAAPI WSASendTo(
  SOCKET                             s,
  LPWSABUF                           lpBuffers,
  DWORD                              dwBufferCount,
  LPDWORD                            lpNumberOfBytesSent,
  DWORD                              dwFlags,
  const sockaddr                     *lpTo,
  int                                iTolen,
  LPWSAOVERLAPPED                    lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
) {
  assert(lpOverlapped == NULL);
  assert(lpCompletionRoutine == NULL);

  int flags = dwFlags; //FIXME: Should we filter certain flags?

  struct iovec *iov = __builtin_alloca(sizeof(struct iovec) * dwBufferCount);
  for(int i = 0; i < dwBufferCount; i++) {
    iov[i].iov_base = lpBuffers[i].buf;
    iov[i].iov_len = lpBuffers[i].len;
  }

  struct msghdr msg;

  msg.msg_name = (const sockaddr *)lpTo;
  msg.msg_namelen = iTolen;
  msg.msg_iov = iov;
  msg.msg_iovlen = dwBufferCount;
  msg.msg_control = NULL;
  msg.msg_controllen = 0;
  msg.msg_flags = 0;
  ssize_t ret = sendmsg(s, &msg, flags);
  if (ret == -1) {
    WSASetLastError(map_error_to_winsock(errno));
    return SOCKET_ERROR;
  }

  *lpNumberOfBytesSent = ret;

  return 0;
}

int WSAAPI WSARecvFrom(
  SOCKET                             s,
  LPWSABUF                           lpBuffers,
  DWORD                              dwBufferCount,
  LPDWORD                            lpNumberOfBytesRecvd,
  LPDWORD                            lpFlags,
  sockaddr                           *lpFrom,
  LPINT                              lpFromlen,
  LPWSAOVERLAPPED                    lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
) {
  assert(lpOverlapped == NULL);
  assert(lpCompletionRoutine == NULL);
  assert(dwBufferCount == 1);

  int flags = *lpFlags; //FIXME: Should we filter certain flags?

  struct iovec *iov = __builtin_alloca(sizeof(struct iovec) * dwBufferCount);
  for(int i = 0; i < dwBufferCount; i++) {
    iov[i].iov_base = lpBuffers[i].buf;
    iov[i].iov_len = lpBuffers[i].len;
  }

  struct msghdr msg;

  msg.msg_name = (const sockaddr *)lpFrom;
  msg.msg_namelen = *lpFromlen;
  msg.msg_iov = iov;
  msg.msg_iovlen = dwBufferCount;
  msg.msg_control = NULL;
  msg.msg_controllen = 0;
  msg.msg_flags = 0;
  ssize_t ret = recvmsg(s, &msg, flags);
  if (ret == -1) {
    WSASetLastError(map_error_to_winsock(errno));
    return SOCKET_ERROR;
  }

  *lpFromlen = msg.msg_namelen; //FIXME: Is this updated? It almost certainly isn't winsock behaviour
  *lpFlags = msg.msg_flags;
  *lpNumberOfBytesRecvd = ret;

  return 0;
}
