#ifndef __NXDK_NET_H__
#define __NXDK_NET_H__

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * Initializes the networking subsystem.
 * @param parameters nx_net_parameters_t containing configuration data
 * @return 0 on success, negative values indicate various errors.
 */
int nxNetInit(const nx_net_parameters_t *parameters);
int nxNetShutdown();

#ifdef __cplusplus
}
#endif

#endif
