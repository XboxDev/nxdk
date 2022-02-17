#ifndef __NXDK_NET_H__
#define __NXDK_NET_H__

#include <stdint.h>

typedef enum nx_net_mode_t_
{
    NX_NET_AUTO = 0,
    NX_NET_DHCP,
    NX_NET_STATIC
} nx_net_mode_t;

typedef struct nx_net_parameters_t_
{
    nx_net_mode_t ipv4_mode;
    nx_net_mode_t ipv6_mode;

    uint32_t ipv4_ip;
    uint32_t ipv4_gateway;
    uint32_t ipv4_netmask;
    uint32_t ipv4_dns1;
    uint32_t ipv4_dns2;
    // TODO:
    // ipv6 static ip fields
} nx_net_parameters_t;

int nxNetInit(const nx_net_parameters_t *parameters);
int nxNetShutdown();


#endif
