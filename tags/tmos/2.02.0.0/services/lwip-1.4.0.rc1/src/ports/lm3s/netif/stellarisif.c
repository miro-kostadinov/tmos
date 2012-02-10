/**
 * @file - stellarisif.c
 * lwIP Ethernet interface for Stellaris Devices
 *
 */

/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
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
 *
 */

/**
 * Copyright (c) 2008 Luminary Micro, Inc.
 *
 * This file is dervied from the ``ethernetif.c'' skeleton Ethernet network
 * interface driver for lwIP.
 *
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"
#include "netif/stellarisif.h"

/**
 * Sanity Check:  This interface driver will NOT work if the following defines
 * are incorrect.
 *
 */
#if (PBUF_LINK_HLEN != 16)
#error "PBUF_LINK_HLEN must be 16 for this interface driver!"
#endif
#if (ETH_PAD_SIZE != 2)
#error "ETH_PAD_SIZE must be 2 for this interface driver!"
#endif
#if (!SYS_LIGHTWEIGHT_PROT)
#error "SYS_LIGHTWEIGHT_PROT must be enabled for this interface driver!"
#endif


/**
 * Setup processing for PTP (IEEE-1588).
 *
 */
#if LWIP_PTPD
extern void lwIPHostGetTime(u32_t *time_s, u32_t *time_ns);
#endif

/**
 * Luminary Micro DriverLib Header Files required for this interface driver.
 *
 */
#include "inc/hw_ethernet.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/ethernet.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"




/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
  struct pbufq txq;
  struct pbufq rxq;
};

/**
 * Global variable for this interface's private data.  Needed to allow
 * the interrupt handlers access to this information outside of the
 * context of the lwIP netif.
 *
 */
static struct ethernetif ethernetif_data;

void lwIPEthernetIntHandler(void)
{
	unsigned long eth_stat = HWREG(ETH_BASE + MAC_O_RIS);
	HWREG(ETH_BASE + MAC_O_IACK) = eth_stat; /* clear the interrupt sources */

	eth_stat &= HWREG(ETH_BASE + MAC_O_IM);/* mask only the enabled sources */

	if ((eth_stat & ETH_INT_RX) != 0)
	{
		EthernetIntDisable(ETH_BASE, ETH_INT_RX); /* disable further RX */
		/* generate ETH_RX event or set the ETH_RX semaphore */
		//signal
	}
	if ((eth_stat & ETH_INT_TX) != 0)
	{
		/* generate ETH_TX event or set the ETH_TX semaphore */
	}
}



#if NETIF_DEBUG
/* Print an IP header by using LWIP_DEBUGF
 * @param p an IP packet, p->payload pointing to the IP header
 */
void
stellarisif_debug_print(struct pbuf *p)
{
  struct eth_hdr *ethhdr = (struct eth_hdr *)p->payload;
  u16_t *plen = (u16_t *)p->payload;

  LWIP_DEBUGF(NETIF_DEBUG, ("ETH header:\n"));
  LWIP_DEBUGF(NETIF_DEBUG, ("Packet Length:%5"U16_F" \n",*plen));
  LWIP_DEBUGF(NETIF_DEBUG, ("Destination: %02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"\n",
    ethhdr->dest.addr[0],
    ethhdr->dest.addr[1],
    ethhdr->dest.addr[2],
    ethhdr->dest.addr[3],
    ethhdr->dest.addr[4],
    ethhdr->dest.addr[5]));
  LWIP_DEBUGF(NETIF_DEBUG, ("Source: %02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"\n",
    ethhdr->src.addr[0],
    ethhdr->src.addr[1],
    ethhdr->src.addr[2],
    ethhdr->src.addr[3],
    ethhdr->src.addr[4],
    ethhdr->src.addr[5]));
  LWIP_DEBUGF(NETIF_DEBUG, ("Packet Type:0x%04"U16_F" \n", ethhdr->type));
}
#endif /* NETIF_DEBUG */
