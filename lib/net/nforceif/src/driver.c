/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * Copyright (c) 2015 Matt Borgerson
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 * Xbox Support: Matt Borgerson
 *
 */
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"

/* Define those to better describe your network interface. */
#define IFNAME0 'x'
#define IFNAME1 'b'

#include "../../pktdrv/pktdrv.h"
#define LINK_SPEED_OF_YOUR_NETIF_IN_BPS 100*1000*1000 /* 100 Mbps */
#include <xboxkrnl/xboxkrnl.h>

static unsigned int g_rx_buffer_packetsize;
static unsigned char *g_rx_buffer, *g_tx_buffer;
extern struct netif *g_pnetif;

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct nforceif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};

/* Forward declarations. */
void nforceif_input(struct netif *netif);

int Pktdrv_Callback(unsigned char *packetaddr, unsigned int size)
{
  g_rx_buffer_packetsize=size;
  memcpy(g_rx_buffer,packetaddr,g_rx_buffer_packetsize);
  nforceif_input(g_pnetif);
  return 1;
}

/**
 * In this function, the hardware should be initialized.
 * Called from nforceif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this nforceif
 */
static void
low_level_init(struct netif *netif)
{
  struct nforceif *nforceif = netif->state;

  if (!Pktdrv_Init())
  {
    debugPrint("Failed to initialize packet driver!\n");
    abort();
  }

  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  Pktdrv_GetEthernetAddr(netif->hwaddr);

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
  if (netif->mld_mac_filter != NULL) {
    ip6_addr_t ip6_allnodes_ll;
    ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
    netif->mld_mac_filter(netif, &ip6_allnodes_ll, MLD6_ADD_MAC_FILTER);
  }
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */

  /* Do whatever else is needed to initialize interface. */

  g_rx_buffer=(unsigned char *)MmAllocateContiguousMemoryEx(
      1520,
      0,    //lowest acceptable
      0xFFFFFFFF, //highest acceptable
      0,      //no need to align to specific boundaries multiple
      4);   //non cached, non ordered

  g_tx_buffer=(unsigned char *)MmAllocateContiguousMemoryEx(
      1520,
      0,    //lowest acceptable
      0xFFFFFFFF, //highest acceptable
      0,      //no need to align to specific boundaries multiple
      4);   //non cached, non ordered

  if (!g_rx_buffer || !g_tx_buffer)
  {
    debugPrint("Failed to allocate packet buffer!\n");
    abort();
  }
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this nforceif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
  struct nforceif *nforceif = netif->state;
  struct pbuf *q;

  unsigned long buf_pos = 0;

  //initiate transfer();
  while (Pktdrv_GetQueuedTxPkts()>0) /* spin */; // FIXME!

#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  for (q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
    //send data from(q->payload, q->len);
    memcpy(g_tx_buffer+buf_pos, q->payload, q->len);
    buf_pos += q->len;
  }

  //signal that packet should be sent();
  Pktdrv_SendPacket(g_tx_buffer, buf_pos);

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this nforceif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
  struct nforceif *nforceif = netif->state;
  struct pbuf *p, *q;
  u16_t len;

  unsigned long buf_pos = 0;

  /* Obtain the size of the packet and put it into the "len"
     variable. */
  len = g_rx_buffer_packetsize;

#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

  if (p != NULL) {

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* We iterate over the pbuf chain until we have read the entire
     * packet into the pbuf. */
    for (q = p; (q != NULL) && (buf_pos < len); q = q->next) {
      /* Read enough bytes to fill this pbuf in the chain. The
       * available data in the pbuf is given by the q->len
       * variable.
       * This does not necessarily have to be a memcpy, you can also preallocate
       * pbufs for a DMA-enabled MAC and after receiving truncate it to the
       * actually received size. In this case, ensure the tot_len member of the
       * pbuf is the sum of the chained pbuf len members.
       */
      // read data into(q->payload, q->len);
      memcpy(q->payload, g_rx_buffer+buf_pos, q->len);
      buf_pos += q->len;
    }

    // acknowledge that packet has been read();

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.recv);
  } else {
    // drop packet();
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
  }

  return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this nforceif
 */
 void // FIXME make static
nforceif_input(struct netif *netif)
{
  struct nforceif *nforceif;
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  nforceif = netif->state;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_IPV6:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ("nforceif_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
     }
    break;

  default:
    pbuf_free(p);
    p = NULL;
    break;
  }
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
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
nforceif_init(struct netif *netif)
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
