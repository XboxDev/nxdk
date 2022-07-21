// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2022 Ryan Wendland

#ifndef __NVNETDRV_H__
#define __NVNETDRV_H__

#include <stddef.h>
#include <stdint.h>
#include <xboxkrnl/xboxkrnl.h>

#ifndef RX_RING_SIZE
#define RX_RING_SIZE 64
#endif
#ifndef TX_RING_SIZE
#define TX_RING_SIZE 64
#endif

// Must be greated than max thernet frame size. A multiple of page size prevents page boundary crossing
#define NVNET_RX_BUFF_LEN (PAGE_SIZE / 2)

// NVNET error codes
#define NVNET_OK 0
#define NVNET_NO_MEM -1
#define NVNET_NO_MAC -2
#define NVNET_PHY_ERR -3
#define NVNET_SYS_ERR -4

typedef void (*nvnetdrv_rx_callback_t) (void *buffer, uint16_t length);
typedef void (*nvnetdrv_tx_callback_t) (void *userdata);

typedef struct _nvnetdrv_descriptor_t
{
    void *addr;
    size_t length;
    nvnetdrv_tx_callback_t callback;
    void *userdata;
} nvnetdrv_descriptor_t;

/**
 * Temporarily stop sending and receiving ethernet packets. TX packets will be held. RX packets will be dropped.
 */
void nvnetdrv_stop_txrx (void);

/**
 * Resume sending and receiving ethernet packets after nvnetdrv_stop_txrx();
 */
void nvnetdrv_start_txrx (void);

/**
 * Initialised the low level NIC hardware.
 * @param rx_buffer_count The number of receive buffers to reserve. This should be atleast two.
 * Recommend this this equal to or greater than RX_RING_SIZE.
 * @param rx_callback. Pointer to a callback function that is called when a new packet is received by the NIC.
 * @return NVNET_OK or the error.
 */
int nvnetdrv_init (size_t rx_buffer_count, nvnetdrv_rx_callback_t rx_callback);

/**
 * Stop the low level NIC hardware. Should be called after nvnetdrv_init() to shutdown the NIC hardware.
 */
void nvnetdrv_stop (void);

/**
 * Returns the ethernet MAC Address.
 * @return A pointer to an array containing the 6 byte ethernet MAC address.
 */
const uint8_t *nvnetdrv_get_ethernet_addr ();

/**
 * Reserves 1-4 descriptors. If the requested number is not immediately available,
 * this function will block until the request can be satisfied. This should be called prior to nvnetdrv_submit_tx_descriptors
 * This function is thread-safe.
 * @param count The number of descriptors to reserve
 * @return Zero if the reservation failed, non-zero if it succeeded.
 */
int nvnetdrv_acquire_tx_descriptors (size_t count);

/**
 * Queues a packet, which consists of 1-4 buffers, for sending. The descriptors for this
 * need to be allocated beforehand using nvnetdrv_acquire_tx_descriptors()/
 * This function is thread-safe.
 * @param buffers Pointer to an array of buffers which will be queued for sending as a packet
 * @param count The number of buffers to queue
 */
void nvnetdrv_submit_tx_descriptors (nvnetdrv_descriptor_t *buffers, size_t count);

/**
 * Releases an RX buffer given out by nvnetdrv. All RX buffers need to be
 * released eventually, or the NIC will run out of buffers to use.
 * This function is thread-safe.
 * @param buffer_virt Pointer to the buffer given out by nvnetdrv.
 */
void nvnetdrv_rx_release(void *buffer_virt);

#endif