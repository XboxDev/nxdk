// SPDX-License-Identifier: BSD

// SPDX-FileCopyrightText: 2001-2004 Swedish Institute of Computer Science.
// SPDX-FileCopyrightText: 2015 Matt Borgerson
// SPDX-FileCopyrightText: 2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2022 Ryan Wendland

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/mld6.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include "nvnetdrv.h"
#include <xboxkrnl/xboxkrnl.h>
#include <assert.h>

/* Define those to better describe your network interface. */
#define IFNAME0 'x'
#define IFNAME1 'b'
#define RX_BUFF_CNT (RX_RING_SIZE)


#define LINK_SPEED_OF_YOUR_NETIF_IN_BPS 100 * 1000 * 1000 /* 100 Mbps */

static struct netif *g_pnetif;
/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct nforceif
{
    struct eth_addr *ethaddr;
    /* Add whatever per-interface state that is needed here. */
};

/**
 * Create a memory pool of rx pbufs
 * RX buffers are linked to a custom pbuf during its lifecycle in lwip stack.
 * When the user has finished with the pbuf, it is freed by custom_free_function to allow the NIC to reuse the buffer
 * This is entirely zero-copy.
 * */
typedef struct
{
    struct pbuf_custom p;
    void *buff;
} rx_pbuf_t;

LWIP_MEMPOOL_DECLARE(RX_POOL, RX_BUFF_CNT, sizeof(rx_pbuf_t), "Zero-copy RX PBUF pool");
void rx_pbuf_free_callback(struct pbuf *p)
{
    rx_pbuf_t *rx_pbuf = (rx_pbuf_t *)p;
    nvnetdrv_rx_release(rx_pbuf->buff);
    LWIP_MEMPOOL_FREE(RX_POOL, rx_pbuf);
}

void rx_callback(void *buffer, uint16_t length)
{
    rx_pbuf_t *rx_pbuf = (rx_pbuf_t *)LWIP_MEMPOOL_ALLOC(RX_POOL);
    LWIP_ASSERT("RX_POOL full\n", rx_pbuf != NULL);
    rx_pbuf->p.custom_free_function = rx_pbuf_free_callback;
    rx_pbuf->buff = buffer;
    struct pbuf *p = pbuf_alloced_custom(PBUF_RAW,
                                        length + ETH_PAD_SIZE,
                                        PBUF_REF,
                                        &rx_pbuf->p,
                                        buffer - ETH_PAD_SIZE,
                                        NVNET_RX_BUFF_LEN - ETH_PAD_SIZE);

    if (g_pnetif->input(p, g_pnetif) != ERR_OK) {
        pbuf_free(p);
        nvnetdrv_rx_release(buffer);
    }
}

/**
 * In this function, the hardware should be initialized.
 * Called from nforceif_init().
 *
 * @param netif the already initialized lwip network interface structure
 * for this nforceif
 */
static void low_level_init(struct netif *netif)
{
    if (nvnetdrv_init(RX_BUFF_CNT, rx_callback) < 0) {
        abort();
    }

    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */
    memcpy(netif->hwaddr, nvnetdrv_get_ethernet_addr(), 6);

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

#if LWIP_IPV6 && LWIP_IPV6_MLD
    /*
     * For hardware/netifs that implement MAC filtering.
     * All-nodes link-local is handled by default, so we must let the hardware know
     * to allow multicast packets in.
     * Should set mld_mac_filter previously. */
    if (netif->mld_mac_filter != NULL)
    {
        ip6_addr_t ip6_allnodes_ll;
        ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
        netif->mld_mac_filter(netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
    }
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */
    g_pnetif = netif;
}

/**
 * This function gets registered as callback function to free pbufs after the
 * NIC driver is done sending their contents.
 *
 * @param userdata the pbuf address, supplied by low_level_output
 */
void tx_pbuf_free_callback(void *userdata)
{
    struct pbuf *p = (struct pbuf *)userdata;
    pbuf_free(p);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this nforceif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *                 an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *             strange results. You might consider waiting for space in the DMA queue
 *             to become available since the stack doesn't retry to send a packet
 *             dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    nvnetdrv_descriptor_t descriptors[4];
    size_t pbufCount = 0;
    for (struct pbuf *q = p; q != NULL; q = q->next)
    {
        assert(p->len < 4096);
        descriptors[pbufCount].addr = q->payload;
        descriptors[pbufCount].length = q->len;
        descriptors[pbufCount].callback = NULL;

        pbufCount++;
        if (pbufCount > 4)
            return ERR_MEM;

        const uint32_t addr_start = (uint32_t)q->payload;
        const uint32_t addr_end = ((uint32_t)q->payload + q->len);
        if (addr_start >> 12 != addr_end >> 12) {
            // Buffer crosses a page boundary
            const uint32_t addr_boundary = (addr_end & 0xFFFFF000);
            const uint32_t length_a = addr_boundary - addr_start;
            const uint32_t length_b = addr_end - addr_boundary;

            // Buffer ends right at page boundary, so no problem
            if (length_b == 0)
                continue;

            // Fixup the descriptor
            descriptors[pbufCount - 1].length = length_a;

            // Queue another descriptor for the remainder
            descriptors[pbufCount].addr = (void *)addr_boundary;
            descriptors[pbufCount].length = length_b;
            descriptors[pbufCount].callback = NULL;

            pbufCount++;
            if (pbufCount > 4)
                return ERR_MEM;
        }
    }

    // Last descriptor gets the callback to free the pbufs
    descriptors[pbufCount - 1].userdata = (void *)p;
    descriptors[pbufCount - 1].callback = tx_pbuf_free_callback;

    int r = nvnetdrv_acquire_tx_descriptors(pbufCount);
    if (!r) {
        return ERR_MEM;
    }

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    // Increase pbuf refcount so they don't get freed while the NIC requires them
    pbuf_ref(p);

    nvnetdrv_submit_tx_descriptors(descriptors, pbufCount);

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this nforceif
 * @return ERR_OK if the loopif is initialized
 *                 ERR_MEM if private data couldn't be allocated
 *                 any other err_t on error
 */
err_t nvnetif_init(struct netif *netif)
{
    struct nforceif *nforceif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    nforceif = mem_malloc(sizeof(struct nforceif));
    if (nforceif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("nforceif_init: out of memory\n"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state = nforceif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
    netif->linkoutput = low_level_output;

    nforceif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}