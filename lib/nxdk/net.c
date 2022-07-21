// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Stefan Schmidt

#include "net.h"

#include <assert.h>
#include <stdbool.h>

#include <nxdk/configsector.h>
#include <netif/etharp.h>
#include <lwip/dhcp.h>
#include <lwip/dns.h>
#include <lwip/netif.h>
#include <lwip/netifapi.h>
#include <lwip/tcpip.h>
#include <lwip/timeouts.h>
#include <xboxkrnl/xboxkrnl.h>


err_t nvnetif_init(struct netif *netif);

struct netif *g_pnetif;
static struct netif nforce_netif;

static void tcpip_init_done(void *arg)
{
    KEVENT *init_complete = arg;
    KeSetEvent(init_complete, IO_NO_INCREMENT, FALSE);
}

int nxNetInit(const nx_net_parameters_t *parameters)
{
    ip4_addr_t ipaddr, netmask, gateway;
    bool ipv4_dhcp;
    ip_addr_t dns[2];
    memset(dns, 0, sizeof(dns));

    if (!parameters || parameters->ipv4_mode == NX_NET_AUTO) {
        nxdk_network_config_sector_t configSector;
        if (!nxLoadNetworkConfig(&configSector)) {
            return -1;
        }

        if (configSector.dhcpFlags & NXDK_NETWORK_CONFIG_MANUAL_IP) {
            ipv4_dhcp = false;
            IP4_ADDR(&ipaddr, configSector.manual.ip >>  0 & 0xff,
                             (configSector.manual.ip >>  8 & 0xff),
                             (configSector.manual.ip >> 16 & 0xff),
                             (configSector.manual.ip >> 24 & 0xff));
            IP4_ADDR(&gateway, configSector.manual.defaultGateway >>  0 & 0xff,
                              (configSector.manual.defaultGateway >>  8 & 0xff),
                              (configSector.manual.defaultGateway >> 16 & 0xff),
                              (configSector.manual.defaultGateway >> 24 & 0xff));
            IP4_ADDR(&netmask, configSector.manual.subnetMask >>  0 & 0xff,
                              (configSector.manual.subnetMask >>  8 & 0xff),
                              (configSector.manual.subnetMask >> 16 & 0xff),
                              (configSector.manual.subnetMask >> 24 & 0xff));;
        } else {
            ipv4_dhcp = true;
        }

        if (configSector.dhcpFlags & NXDK_NETWORK_CONFIG_MANUAL_DNS) {
            IP4_ADDR(&dns[0].u_addr.ip4, configSector.manual.primaryDns >>  0 & 0xff,
                                 (configSector.manual.primaryDns >>  8 & 0xff),
                                 (configSector.manual.primaryDns >> 16 & 0xff),
                                 (configSector.manual.primaryDns >> 24 & 0xff));
            IP4_ADDR(&dns[1].u_addr.ip4, configSector.manual.secondaryDns >>  0 & 0xff,
                                 (configSector.manual.secondaryDns >>  8 & 0xff),
                                 (configSector.manual.secondaryDns >> 16 & 0xff),
                                 (configSector.manual.secondaryDns >> 24 & 0xff));
            dns[0].type = IPADDR_TYPE_V4;
            dns[1].type = IPADDR_TYPE_V4;
        }
    } else if (parameters->ipv4_mode == NX_NET_DHCP) {
        ipv4_dhcp = true;
    } else if (parameters->ipv4_mode == NX_NET_STATIC) {
        ipv4_dhcp = false;
        IP4_ADDR(&ipaddr, parameters->ipv4_ip >>  0 & 0xff,
                         (parameters->ipv4_ip >>  8 & 0xff),
                         (parameters->ipv4_ip >> 16 & 0xff),
                         (parameters->ipv4_ip >> 24 & 0xff));
        IP4_ADDR(&gateway, parameters->ipv4_gateway >>  0 & 0xff,
                          (parameters->ipv4_gateway >>  8 & 0xff),
                          (parameters->ipv4_gateway >> 16 & 0xff),
                          (parameters->ipv4_gateway >> 24 & 0xff));
        IP4_ADDR(&netmask, parameters->ipv4_netmask >>  0 & 0xff,
                          (parameters->ipv4_netmask >>  8 & 0xff),
                          (parameters->ipv4_netmask >> 16 & 0xff),
                          (parameters->ipv4_netmask >> 24 & 0xff));
    } else {
        assert(false);
        return -1;
    }

    if (ipv4_dhcp) {
        IP4_ADDR(&gateway, 0, 0, 0, 0);
        IP4_ADDR(&ipaddr, 0, 0, 0, 0);
        IP4_ADDR(&netmask, 0, 0, 0, 0);
    }

    KEVENT tcpip_init_complete;
    KeInitializeEvent(&tcpip_init_complete, SynchronizationEvent, FALSE);
    tcpip_init(tcpip_init_done, &tcpip_init_complete);
    KeWaitForSingleObject(&tcpip_init_complete, Executive, KernelMode, FALSE, NULL);

    g_pnetif = &nforce_netif;
    err_t err = netifapi_netif_add(&nforce_netif, &ipaddr, &netmask, &gateway, NULL, nvnetif_init, tcpip_input);
    if (err != ERR_OK) {
        debugPrint("netif_add failed\n");
        return -1;
    }

    netifapi_netif_set_default(&nforce_netif);
    netifapi_netif_set_up(&nforce_netif);

    if (ipv4_dhcp) {
        netifapi_dhcp_start(&nforce_netif);

        LARGE_INTEGER duration;
        duration.QuadPart = ((LONGLONG)1000) * -10000;
        int i = 0;
        while (dhcp_supplied_address(&nforce_netif) == 0) {
            i++;
            if (i == 10) {
                return -2;
            }
            KeDelayExecutionThread(KernelMode, FALSE, &duration);
        }
    }

    if (parameters) {
        if (parameters->ipv4_dns1 != 0 || parameters->ipv4_dns2 != 0) {
            IP4_ADDR(&dns[0].u_addr.ip4, parameters->ipv4_dns1 >>  0 & 0xff,
                                 (parameters->ipv4_dns1 >>  8 & 0xff),
                                 (parameters->ipv4_dns1 >> 16 & 0xff),
                                 (parameters->ipv4_dns1 >> 24 & 0xff));
            IP4_ADDR(&dns[1].u_addr.ip4, parameters->ipv4_dns2 >>  0 & 0xff,
                                 (parameters->ipv4_dns2 >>  8 & 0xff),
                                 (parameters->ipv4_dns2 >> 16 & 0xff),
                                 (parameters->ipv4_dns2 >> 24 & 0xff));
            dns[0].type = IPADDR_TYPE_V4;
            dns[1].type = IPADDR_TYPE_V4;
        }
    }

    if (dns[0].u_addr.ip4.addr != 0) {
        dns_setserver(0, &dns[0]);
    }
    if (dns[1].u_addr.ip4.addr != 0) {
        dns_setserver(1, &dns[1]);
    }

    return 0;
}
