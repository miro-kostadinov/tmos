/*
 * lwip_drv.cpp
 *
 *  Created on: 22.03.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <hardware_cpp.h>
#include <rom_lm3s.h>
#include <lwip_drv.h>
//#include "netif/etharp.h"
#include "lwip/tcp_impl.h"
#include "lwip/autoip.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "netif/stellarisif.h"
#include "lwip/stats.h"

#include <lwip_api.h>
//*****************************************************************************
//
// lwIP API Header Files
//
//*****************************************************************************
//#include "lwip/api.h"
//#include "lwip/netifapi.h"
//#include "lwip/tcp.h"
//#include "lwip/udp.h"
//#include "lwip/tcpip.h"
//#include "lwip/sockets.h"
//#include "lwip/mem.h"



//*----------------------------------------------------------------------------
//*			Internal Functions
//*----------------------------------------------------------------------------
//*****************************************************************************
//
//! \addtogroup lwiplib_api
//! @{
//
//*****************************************************************************


//*****************************************************************************
//
// Driverlib headers needed for this library module.
//
//*****************************************************************************
//#include "inc/hw_ints.h"
//#include "inc/hw_ethernet.h"
//#include "inc/hw_memmap.h"
//#include "inc/hw_nvic.h"
//#include "inc/hw_types.h"
//#include "driverlib/debug.h"
//#include "driverlib/ethernet.h"
//#include "driverlib/gpio.h"
//#include "driverlib/sysctl.h"


/**
 * Push a pbuf packet onto a pbuf packet queue
 *
 * @param p is the pbuf to push onto the packet queue.
 * @param q is the packet queue.
 *
 * @return 1 if successful, 0 if q is full.
 */
static int enqueue_packet(struct pbuf *p, struct pbufq *q)
{
	SYS_ARCH_DECL_PROTECT(lev);
	int ret;

	/**
	 * This entire function must run within a "critical section" to preserve
	 * the integrity of the transmit pbuf queue.
	 *
	 */
	SYS_ARCH_PROTECT(lev);

	if (!PBUF_QUEUE_FULL(q))
	{
		/**
		 * The queue isn't full so we add the new frame at the current
		 * write position and move the write pointer.
		 *
		 */
		q->pbuf[q->qwrite] = p;
		q->qwrite = ((q->qwrite + 1) % STELLARIS_NUM_PBUF_QUEUE);
		ret = 1;
	}
	else
	{
		/**
		 * The stack is full so we are throwing away this value.  Keep track
		 * of the number of times this happens.
		 *
		 */
		q->overflow++;
		ret = 0;
	}

	/* Return to prior interrupt state and return the pbuf pointer. */
	SYS_ARCH_UNPROTECT(lev);
	return (ret);
}


/**
 * Pop a pbuf packet from a pbuf packet queue
 *
 * @param q is the packet queue from which to pop the pbuf.
 *
 * @return pointer to pbuf packet if available, NULL otherswise.
 */
static struct pbuf* dequeue_packet(struct pbufq *q)
{
	struct pbuf *pBuf;
	SYS_ARCH_DECL_PROTECT(lev);

	/**
	 * This entire function must run within a "critical section" to preserve
	 * the integrity of the transmit pbuf queue.
	 *
	 */
	SYS_ARCH_PROTECT(lev);

	if (PBUF_QUEUE_EMPTY(q))
	{
		/* Return a NULL pointer if the queue is empty. */
		pBuf = (struct pbuf *) NULL;
	}
	else
	{
		/**
		 * The queue is not empty so return the next frame from it
		 * and adjust the read pointer accordingly.
		 *
		 */
		pBuf = q->pbuf[q->qread];
		q->qread = ((q->qread + 1) % STELLARIS_NUM_PBUF_QUEUE);
	}

	/* Return to prior interrupt state and return the pbuf pointer. */
	SYS_ARCH_UNPROTECT(lev);
	return (pBuf);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf might be
 * chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 * @note This function MUST be called with interrupts disabled or with the
 *       Stellaris Ethernet transmit fifo protected.
 */
static err_t low_level_transmit(struct netif *netif, struct pbuf *p)
{
	int iBuf;
	unsigned char *pucBuf;
	unsigned long *pulBuf;
	struct pbuf *q;
	int iGather;
	unsigned long ulGather;
	unsigned char *pucGather;
	MAC_Type* mac = (MAC_Type*)netif->state;

	/**
	 * Fill in the first two bytes of the payload data (configured as padding
	 * with ETH_PAD_SIZE = 2) with the total length of the payload data
	 * (minus the Ethernet MAC layer header).
	 *
	 */
	*((unsigned short *) (p->payload)) = p->tot_len - 16;

	/* Initialize the gather register. */
	iGather = 0;
	pucGather = (unsigned char *) &ulGather;
	ulGather = 0;

	/* Copy data from the pbuf(s) into the TX Fifo. */
	for (q = p; q != NULL; q = q->next)
	{
		/* Intialize a char pointer and index to the pbuf payload data. */
		pucBuf = (unsigned char *) q->payload;
		iBuf = 0;

		/**
		 * If the gather buffer has leftover data from a previous pbuf
		 * in the chain, fill it up and write it to the Tx FIFO.
		 *
		 */
		while ((iBuf < q->len) && (iGather != 0))
		{
			/* Copy a byte from the pbuf into the gather buffer. */
			pucGather[iGather] = pucBuf[iBuf++];

			/* Increment the gather buffer index modulo 4. */
			iGather = ((iGather + 1) % 4);
		}

		/**
		 * If the gather index is 0 and the pbuf index is non-zero,
		 * we have a gather buffer to write into the Tx FIFO.
		 *
		 */
		if ((iGather == 0) && (iBuf != 0))
		{
			mac->MACDATA = ulGather;
			ulGather = 0;
		}

		/* Initialze a long pointer into the pbuf for 32-bit access. */
		pulBuf = (unsigned long *)(void*) &pucBuf[iBuf];

		/**
		 * Copy words of pbuf data into the Tx FIFO, but don't go past
		 * the end of the pbuf.
		 *
		 */
		while ((iBuf + 4) <= q->len)
		{
			mac->MACDATA = *pulBuf++;
			iBuf += 4;
		}

		/**
		 * Check if leftover data in the pbuf and save it in the gather
		 * buffer for the next time.
		 *
		 */
		while (iBuf < q->len)
		{
			/* Copy a byte from the pbuf into the gather buffer. */
			pucGather[iGather] = pucBuf[iBuf++];

			/* Increment the gather buffer index modulo 4. */
			iGather = ((iGather + 1) % 4);
		}
	}

	/* Send any leftover data to the FIFO. */
	mac->MACDATA = ulGather;

	/* Wakeup the transmitter. */
	mac->MACTR = MAC_TR_NEWTX;

	/* Dereference the pbuf from the queue. */
	pbuf_free(p);

	LINK_STATS_INC(link.xmit);

	return (ERR_OK);
}

/**
 * This function with either place the packet into the Stellaris transmit fifo,
 * or will place the packet in the interface PBUF Queue for subsequent
 * transmission when the transmitter becomes idle.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	LWIP_DRIVER_DATA* drv_data = (LWIP_DRIVER_DATA*)netif;
	MAC_Type* mac = (MAC_Type*)netif->state;
	SYS_ARCH_DECL_PROTECT(lev);

	/**
	 * This entire function must run within a "critical section" to preserve
	 * the integrity of the transmit pbuf queue.
	 *
	 */
	SYS_ARCH_PROTECT(lev);

	/**
	 * Bump the reference count on the pbuf to prevent it from being
	 * freed till we are done with it.
	 *
	 */
	pbuf_ref(p);

	/**
	 * If the transmitter is idle, and there is nothing on the queue,
	 * send the pbuf now.
	 *
	 */
	if (PBUF_QUEUE_EMPTY(&drv_data->txq) && ((mac->MACTR & MAC_TR_NEWTX) == 0))
	{
		low_level_transmit(netif, p);
	}

	/* Otherwise place the pbuf on the transmit queue. */
	else
	{
		/* Add to transmit packet queue */
		if (!enqueue_packet(p, &drv_data->txq))
		{
			/* if no room on the queue, free the pbuf reference and return error. */
			pbuf_free(p);
			SYS_ARCH_UNPROTECT(lev);
			return (ERR_MEM);
		}
	}

	/* Return to prior interrupt state and return. */
	SYS_ARCH_UNPROTECT(lev);
	return ERR_OK;
}

/**
 * This function will read a single packet from the Stellaris ethernet
 * interface, if available, and return a pointer to a pbuf.  The timestamp
 * of the packet will be placed into the pbuf structure.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return pointer to pbuf packet if available, NULL otherswise.
 */
static struct pbuf *low_level_receive(struct netif *netif)
{
	MAC_Type* mac = (MAC_Type*)netif->state;
	struct pbuf *p, *q;
	u16_t len;
	u32_t temp;
	int i;
	unsigned long *ptr;
#if LWIP_PTPD
	u32_t time_s, time_ns;

	/* Get the current timestamp if PTPD is enabled */
	lwIPHostGetTime(&time_s, &time_ns);
#endif

	/* Check if a packet is available, if not, return NULL packet. */
	if ((mac->MACNP & MAC_NP_NPR_M) == 0)
	{
		return (NULL);
	}

	/**
	 * Obtain the size of the packet and put it into the "len" variable.
	 * Note:  The length returned in the FIFO length position includes the
	 * two bytes for the length + the 4 bytes for the FCS.
	 *
	 */
	temp = mac->MACDATA;
	len = temp & 0xFFFF;

	/* We allocate a pbuf chain of pbufs from the pool. */
	p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

	/* If a pbuf was allocated, read the packet into the pbuf. */
	if (p != NULL)
	{
		/* Place the first word into the first pbuf location. */
		*(unsigned long *) p->payload = temp;
		p->payload = (char *) (p->payload) + 4;
		p->len -= 4;

		/* Process all but the last buffer in the pbuf chain. */
		q = p;
		while (q != NULL)
		{
			/* Setup a byte pointer into the payload section of the pbuf. */
			ptr = (unsigned long *)q->payload;

			/**
			 * Read data from FIFO into the current pbuf
			 * (assume pbuf length is modulo 4)
			 *
			 */
			for (i = 0; i < q->len; i += 4)
			{
				*ptr++ = mac->MACDATA;
			}

			/* Link in the next pbuf in the chain. */
			q = q->next;
		}

		/* Restore the first pbuf parameters to their original values. */
		p->payload = (char *) (p->payload) - 4;
		p->len += 4;

		/* Adjust the link statistics */
		LINK_STATS_INC(link.recv);

#if LWIP_PTPD
		/* Place the timestamp in the PBUF */
		p->time_s = time_s;
		p->time_ns = time_ns;
#endif
	}

	/* If no pbuf available, just drain the RX fifo. */
	else
	{
		for (i = 4; i < len; i += 4)
		{
			temp = mac->MACDATA;
		}

		/* Adjust the link statistics */
		LINK_STATS_INC(link.memerr);
		LINK_STATS_INC(link.drop);
	}

	return (p);
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
	u32_t temp;
	MAC_Type* mac = (MAC_Type*)netif->state;
	//struct ethernetif *ethernetif = netif->state;

	/* set MAC hardware address length */
	netif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* set MAC hardware address */
	EthernetMACAddrGet(mac, &(netif->hwaddr[0]));

	/* maximum transfer unit */
	netif->mtu = 1500;

	/* device capabilities */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP
			| NETIF_FLAG_LINK_UP;

	/* Do whatever else is needed to initialize interface. */
	/* Disable all Ethernet Interrupts. */
	EthernetIntDisable(
			mac,
			(ETH_INT_PHY | ETH_INT_MDIO | ETH_INT_RXER | ETH_INT_RXOF
					| ETH_INT_TX | ETH_INT_TXER | ETH_INT_RX));
	temp = EthernetIntStatus(mac, false);
	EthernetIntClear(mac, temp);

	/* Initialize the Ethernet Controller. */
	EthernetInitExpClk(mac, system_clock_frequency);//SysCtlClockGet());

	/*
	 * Configure the Ethernet Controller for normal operation.
	 * - Enable TX Duplex Mode
	 * - Enable TX Padding
	 * - Enable TX CRC Generation
	 * - Enable RX Multicast Reception
	 */
	EthernetConfigSet(
			mac,
			(ETH_CFG_TX_DPLXEN | ETH_CFG_TX_CRCEN | ETH_CFG_TX_PADEN
					| ETH_CFG_RX_AMULEN));

	/* Enable the Ethernet Controller transmitter and receiver. */
	EthernetEnable(mac);

	/* Enable the Ethernet Interrupt handler. */
//	IntEnable( INT_ETH);
	NVIC->NVIC_EnableIRQ(Ethernet_IRQn); //THIS IS BAD!

	/* Enable Ethernet TX and RX Packet Interrupts. */
	EthernetIntEnable(mac, ETH_INT_RX | ETH_INT_TX);
}




WEAK_C void lwIPHostTimerHandler(void)
{
	//TRACE1("lwIP tmr");
}

//*****************************************************************************
//
//! Service the lwIP timers.
//!
//! This function services all of the lwIP periodic timers, including TCP and
//! Host timers.  This should be called from the lwIP context, which may be
//! the Ethernet interrupt (in the case of a non-RTOS system) or the lwIP
//! thread, in the event that an RTOS is used.
//!
//! \return None.
//
//*****************************************************************************
static void lwIPServiceTimers(struct netif *netif)
{
	LWIP_DRIVER_DATA* drv_data = (LWIP_DRIVER_DATA*)netif;
	MAC_Type* mac = (MAC_Type*)netif->state;
    //
    // Service the MDIX timer.
    //
    if(drv_data->timer_main > drv_data->timer_mdix)
    {
        //
        // See if there has not been a link for 2 seconds.
        //
        if((EthernetPHYRead(mac, PHY_MR1) & PHY_MR1_LINK) == 0)
        {
            // There has not been a link for 2 seconds, so flip the MDI/MDIX
            // switch.  This is handled automatically by Fury rev A2, but is
            // harmless.
            //
        	mac->MDIX  ^= MAC_MDIX_EN;
        }
        //
        // Reset the MDIX timer.
        //
    	drv_data->timer_mdix = drv_data->timer_main + 2048;
    }

    //
    // Service the host timer.
    //
#if HOST_TMR_INTERVAL
    if(drv_data->timer_main > drv_data->timer_host )
    {
    	drv_data->timer_host = drv_data->timer_main + HOST_TMR_INTERVAL;
        lwIPHostTimerHandler();
    }
#endif

    //
    // Service the ARP timer.
    //
#if LWIP_ARP
    if(drv_data->timer_main > drv_data->timer_arp)
    {
    	drv_data->timer_arp = drv_data->timer_main + ARP_TMR_INTERVAL;
        etharp_tmr();
    }
#endif

    //
    // Service the TCP timer.
    //
#if LWIP_TCP
    if(drv_data->timer_main > drv_data->timer_tcp)
    {
    	drv_data->timer_tcp = drv_data->timer_main + TCP_TMR_INTERVAL;
        tcp_tmr();
    }
#endif

    //
    // Service the AutoIP timer.
    //
#if LWIP_AUTOIP
    if(drv_data->timer_main - drv_data->timer_autoIP)
    {
    	drv_data->timer_autoIP = drv_data->timer_main + AUTOIP_TMR_INTERVAL;
        autoip_tmr();
    }
#endif

    //
    // Service the DCHP Coarse Timer.
    //
#if LWIP_DHCP
    if(drv_data->timer_main > drv_data->timer_DHCPCoarse)
    {
    	drv_data->timer_DHCPCoarse = drv_data->timer_main + DHCP_COARSE_TIMER_MSECS;
        dhcp_coarse_tmr();
    }
#endif

    //
    // Service the DCHP Fine Timer.
    //
#if LWIP_DHCP
    if(drv_data->timer_main > drv_data->timer_DHCPFine)
    {
    	drv_data->timer_DHCPFine = drv_data->timer_main + DHCP_FINE_TIMER_MSECS;
        dhcp_fine_tmr();
    }
#endif
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
	LWIP_DRIVER_DATA* drv_data = (LWIP_DRIVER_DATA*)netif;
	LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
	NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 1000000);

//	netif->state = &ethernetif_data;
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;
	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	netif->output = etharp_output;
	netif->linkoutput = low_level_output;

//	drv_data->ethaddr = (struct eth_addr *) &(netif->hwaddr[0]);
	drv_data->txq.qread = drv_data->txq.qwrite = 0;
	drv_data->txq.overflow = 0;
	drv_data->rxq.qread = drv_data->rxq.qwrite = 0;
	drv_data->rxq.overflow = 0;

	/* initialize the hardware */
	low_level_init(netif);

	return ERR_OK;
}

//*****************************************************************************
//
//! Initializes the lwIP TCP/IP stack.
//!
//! \param pucMAC is a pointer to a six byte array containing the MAC
//! address to be used for the interface.
//! \param ulIPAddr is the IP address to be used (static).
//! \param ulNetMask is the network mask to be used (static).
//! \param ulGWAddr is the Gateway address to be used (static).
//! \param ulIPMode is the IP Address Mode.  \b IPADDR_USE_STATIC will force
//! static IP addressing to be used, \b IPADDR_USE_DHCP will force DHCP with
//! fallback to Link Local (Auto IP), while \b IPADDR_USE_AUTOIP will force
//! Link Local only.
//!
//! This function performs initialization of the lwIP TCP/IP stack for the
//! Stellaris Ethernet MAC, including DHCP and/or AutoIP, as configured.
//!
//! \return None.
//
//*****************************************************************************
void lwIPInit(LWIP_DRIVER_INFO* drv_info, ip_adr_set* set)
{
	LWIP_DRIVER_DATA* drv_data = drv_info->drv_data;

    struct ip_addr ip_addr;
    struct ip_addr net_mask;
    struct ip_addr gw_addr;

    //
    // Check the parameters.
    //
#if LWIP_DHCP && LWIP_AUTOIP
    ASSERT((set->ip_addr_mode == IPADDR_USE_STATIC) ||
           (set->ip_addr_mode == IPADDR_USE_DHCP) ||
           (set->ip_addr_mode == IPADDR_USE_AUTOIP));
#elif LWIP_DHCP
    ASSERT((set->ip_addr_mode == IPADDR_USE_STATIC) ||
           (set->ip_addr_mode == IPADDR_USE_DHCP));
#elif LWIP_AUTOIP
    ASSERT((set->ip_addr_mode == IPADDR_USE_STATIC) ||
           (set->ip_addr_mode == IPADDR_USE_AUTOIP);)
#else
    ASSERT(set->ip_addr_mode == IPADDR_USE_STATIC);
#endif

    //
    // Enable the ethernet peripheral.
    //
	SysCtlPeripheralEnable(drv_info->info.peripheral_indx);

    //
    // Program the MAC address into the Ethernet controller.
    //
    unsigned long ulUser0, ulUser1;
    unsigned char pucMACArray[8];
    ROM_FlashUserGet(&ulUser0, &ulUser1);
    if((ulUser0 == 0xffffffff) || (ulUser1 == 0xffffffff))
    {
        //
        // We should never get here.  This is an error if the MAC address has
        // not been programmed into the device.  Exit the program.
        //
        TRACE1("MAC Address Not Programmed!\n");
        ulUser0 = 0x00123456;
        ulUser1 = 0x00123456;
    }

    //
    // Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
    // address needed to program the hardware registers, then program the MAC
    // address into the Ethernet Controller registers.
    //
    pucMACArray[0] = ((ulUser0 >>  0) & 0xff);
    pucMACArray[1] = ((ulUser0 >>  8) & 0xff);
    pucMACArray[2] = ((ulUser0 >> 16) & 0xff);
    pucMACArray[3] = ((ulUser1 >>  0) & 0xff);
    pucMACArray[4] = ((ulUser1 >>  8) & 0xff);
    pucMACArray[5] = ((ulUser1 >> 16) & 0xff);
    EthernetMACAddrSet(drv_info->hw_base, pucMACArray);

    //
    // Initialize lwIP library modules
    //
    lwip_init();

    //
    // Setup the network address values.
    //
    if(set->ip_addr_mode == IPADDR_USE_STATIC)
    {
        ip_addr.addr = htonl(set->ip_addr);
        net_mask.addr = htonl(set->net_mask);
        gw_addr.addr = htonl(set->gw_addr);
    }
#if LWIP_DHCP || LWIP_AUTOIP
    else
    {
        ip_addr.addr = 0;
        net_mask.addr = 0;
        gw_addr.addr = 0;
    }
#endif

    //
    // Create, configure and add the Ethernet controller interface with
    // default settings.
    //
    netif_add(
    		&drv_data->lwip_netif,	//pre-allocated netif structure
    		&ip_addr, 				//IP address for the new netif
    		&net_mask, 				//network mask for the new netif
    		&gw_addr, 				//gateway IP address for the new netif
    		drv_info->hw_base,		//opaque data passed to the new netif
            ethernetif_init,		//callback function that initializes the interface
            ip_input				//callback function that is called to pass
            						//ingress packets up in the protocol layer stack
            );

    netif_set_default(&drv_data->lwip_netif);

    //
    // Indicate that we are running in static IP mode.
    //
    drv_data->ip_addr_mode = set->ip_addr_mode;

    //
    // Start DHCP, if enabled.
    //
#if LWIP_DHCP
    if(drv_data->ip_addr_mode == IPADDR_USE_DHCP)
    {
        dhcp_start(&drv_data->lwip_netif);
    }
#endif

    //
    // Start AutoIP, if enabled and DHCP is not.
    //
#if LWIP_AUTOIP
    if(drv_data->ip_addr_mode == IPADDR_USE_AUTOIP)
    {
        autoip_start(&drv_data->lwip_netif);
    }
#endif

    //
    // Bring the interface up.
    //
    netif_set_up(&drv_data->lwip_netif);
}

//*****************************************************************************
//
//! Handles periodic timer events for the lwIP TCP/IP stack.
//!
//! \param ulTimeMS is the incremental time for this periodic interrupt.
//!
//! This function will update the local timer by the value in \e ulTimeMS.
//! If the system is configured for use without an RTOS, an Ethernet interrupt
//! will be triggered to allow the lwIP periodic timers to be serviced in the
//! Ethernet interrupt.
//!
//! \return None.
//
//*****************************************************************************
//void lwIPTimer(unsigned long ulTimeMS)
//{
//    //
//    // Increment the lwIP Ethernet timer.
//    //
//    drv_data->timer_main += ulTimeMS;
//
//#if NO_SYS
//    //
//    // Generate an Ethernet interrupt.  This will perform the actual work
//    // of checking the lwIP timers and taking the appropriate actions.  This is
//    // needed since lwIP is not re-entrant, and this allows all lwIP calls to
//    // be placed inside the Ethernet interrupt handler ensuring that all calls
//    // into lwIP are coming from the same context, preventing any reentrancy
//    // issues.  Putting all the lwIP calls in the Ethernet interrupt handler
//    // avoids the use of mutexes to avoid re-entering lwIP.
//    //
//    HWREG(NVIC_SW_TRIG) |= INT_ETH - 16;
//
//#else
//    //
//    // If running in an RTOS/SYSTEM environment, what should happen is is that
//    // an event or semaphore should be posted that will wake up the main
//    // processing thread.  The exact mechanism for this needs to be determined.
//    // Until that time, generate an error if this module is compiled for an
//    // RTOS based system.
//    //
//    #error "lwIPTimer does not yet support an RTOS!"
//#endif
//}

//*****************************************************************************
//
//! Change the configuration of the lwIP network interface.
//!
//! \param ulIPAddr is the new IP address to be used (static).
//! \param ulNetMask is the new network mask to be used (static).
//! \param ulGWAddr is the new Gateway address to be used (static).
//! \param ulIPMode is the IP Address Mode.  \b IPADDR_USE_STATIC 0 will force
//! static IP addressing to be used, \b IPADDR_USE_DHCP will force DHCP with
//! fallback to Link Local (Auto IP), while \b IPADDR_USE_AUTOIP will force
//! Link Local only.
//!
//! This function will evaluate the new configuration data.  If necessary, the
//! interface will be brought down, reconfigured, and then brought back up
//! with the new configuration.
//!
//! \return None.
//
//*****************************************************************************
void lwIPNetworkConfigChange(LWIP_DRIVER_INFO* drv_info, ip_adr_set* set)
{
    struct ip_addr ip_addr;
    struct ip_addr net_mask;
    struct ip_addr gw_addr;
    struct netif *netif = &drv_info->drv_data->lwip_netif;
	LWIP_DRIVER_DATA* drv_data = drv_info->drv_data;


    //
    // Check the parameters.
    //
#if LWIP_DHCP && LWIP_AUTOIP
    ASSERT((set->ip_addr_mode == IPADDR_USE_STATIC) ||
           (set->ip_addr_mode == IPADDR_USE_DHCP) ||
           (set->ip_addr_mode == IPADDR_USE_AUTOIP));
#elif LWIP_DHCP
    ASSERT((set->ip_addr_mode == IPADDR_USE_STATIC) ||
           (set->ip_addr_mode == IPADDR_USE_DHCP));
#elif LWIP_AUTOIP
    ASSERT((set->ip_addr_mode == IPADDR_USE_STATIC) ||
           (set->ip_addr_mode == IPADDR_USE_AUTOIP));
#else
    ASSERT(set->ip_addr_mode == IPADDR_USE_STATIC);
#endif

    //
    // Setup the network address values.
    //
    if(set->ip_addr_mode == IPADDR_USE_STATIC)
    {
        ip_addr.addr = htonl(set->ip_addr);
        net_mask.addr = htonl(set->net_mask);
        gw_addr.addr = htonl(set->gw_addr);
    }
#if LWIP_DHCP || LWIP_AUTOIP
    else
    {
        ip_addr.addr = 0;
        net_mask.addr = 0;
        gw_addr.addr = 0;
    }
#endif

    //
    // Switch on the current IP Address Aquisition mode.
    //
    switch(drv_data->ip_addr_mode)
    {
        //
        // Static IP
        //
        case IPADDR_USE_STATIC:
        {
            //
            // Set the new address parameters.  This will change the address
            // configuration in lwIP, and if necessary, will reset any links
            // that are active.  This is valid for all three modes.
            //
            netif_set_addr(netif, &ip_addr, &net_mask, &gw_addr);

            //
            // If we are going to DHCP mode, then start the DHCP server now.
            //
#if LWIP_DHCP
            if(set->ip_addr_mode == IPADDR_USE_DHCP)
            {
                dhcp_start(netif);
            }
#endif

            //
            // If we are going to AutoIP mode, then start the AutoIP process
            // now.
            //
#if LWIP_AUTOIP
            if(set->ip_addr_mode == IPADDR_USE_AUTOIP)
            {
                autoip_start(netif);
            }
#endif
            //
            // And we're done.
            //
            break;
        }

        //
        // DHCP (with AutoIP fallback).
        //
#if LWIP_DHCP
        case IPADDR_USE_DHCP:
        {
            //
            // If we are going to static IP addressing, then disable DHCP and
            // force the new static IP address.
            //
            if(set->ip_addr_mode == IPADDR_USE_STATIC)
            {
                dhcp_stop(netif);
                netif_set_addr(netif, &ip_addr, &net_mask, &gw_addr);
            }

            //
            // If we are going to AUTO IP addressing, then disable DHCP, set
            // the default addresses, and start AutoIP.
            //
#if LWIP_AUTOIP
            else if(set->ip_addr_mode == IPADDR_USE_AUTOIP)
            {
                dhcp_stop(netif);
                netif_set_addr(netif, &ip_addr, &net_mask, &gw_addr);
                autoip_start(netif);
            }
#endif
            break;
        }
#endif

        //
        // AUTOIP
        //
#if LWIP_AUTOIP
        case IPADDR_USE_AUTOIP:
        {
            //
            // If we are going to static IP addressing, then disable AutoIP and
            // force the new static IP address.
            //
            if(set->ip_addr_mode == IPADDR_USE_STATIC)
            {
                autoip_stop(netif);
                netif_set_addr(netif, &ip_addr, &net_mask, &gw_addr);
            }

            //
            // If we are going to DHCP addressing, then disable AutoIP, set the
            // default addresses, and start dhcp.
            //
#if LWIP_DHCP
            else if(set->ip_addr_mode == IPADDR_USE_AUTOIP)
            {
                autoip_stop(netif);
                netif_set_addr(netif, &ip_addr, &net_mask, &gw_addr);
                dhcp_start(netif);
            }
#endif
            break;
        }
#endif
    }

    //
    // Save the new mode.
    //
    drv_data->ip_addr_mode = set->ip_addr_mode;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
int ethernetif_input(struct netif *netif)
{
	LWIP_DRIVER_DATA* drv_data = (LWIP_DRIVER_DATA*)netif;
	struct pbuf *p;
	int count = 0;

	/* move received packet into a new pbuf */
	while ((p = dequeue_packet(&drv_data->rxq)) != NULL)
	{
		count++;
		/* process the packet. */
		if (ethernet_input(p, netif) != ERR_OK)
		{
			LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: input error\n"));
			pbuf_free(p);
			p = NULL;
		}
	}

	return (count);
}

/**
 * Process tx and rx packets at the low-level interrupt.
 *
 * Should be called from the Stellaris Ethernet Interrupt Handler.  This
 * function will read packets from the Stellaris Ethernet fifo and place them
 * into a pbuf queue.  If the transmitter is idle and there is at least one packet
 * on the transmit queue, it will place it in the transmit fifo and start the
 * transmitter.
 *
 */
void stellarisif_interrupt(struct netif *netif)
{
	LWIP_DRIVER_DATA* drv_data = (LWIP_DRIVER_DATA*)netif;
	MAC_Type* mac = (MAC_Type*)netif->state;
	struct pbuf *p;

	/*
	 * Process the transmit and receive queues as long as there is receive
	 * data available
	 *
	 */
	p = low_level_receive(netif);
	while (p != NULL)
	{
    	TRACELN1(">");
		/* Add the rx packet to the rx queue */
		if (!enqueue_packet(p, &drv_data->rxq))
		{
			/* Could not place the packet on the queue, bail out. */
			pbuf_free(p);
			break;
		}

		/* Check if TX fifo is empty and packet available */
		if ((mac->MACTR & MAC_TR_NEWTX) == 0)
		{
			p = dequeue_packet(&drv_data->txq);
			if (p != NULL)
			{
				low_level_transmit(netif, p);
			}
		}

		/* Read another packet from the RX fifo */
		p = low_level_receive(netif);
	}

	/* One more check of the transmit queue/fifo */
	if ((mac->MACTR & MAC_TR_NEWTX) == 0)
	{
		p = dequeue_packet(&drv_data->txq);
		if (p != NULL)
		{
			low_level_transmit(netif, p);
		}
	}
}


//*----------------------------------------------------------------------------
//*			lwIP thread
//*----------------------------------------------------------------------------


#define LWIP_THREAD_RXTXSIG		1
void lwipdrv_thread(LWIP_DRIVER_INFO* drv_info)
{
	LWIP_DRIVER_DATA*  drv_data = drv_info->drv_data;
    CHandle helper;

	ip_adr_set set;

	ALLOCATE_SIGNAL(LWIP_THREAD_RXTXSIG);

	set.ip_addr = 0;
	set.gw_addr = 0;
	set.net_mask = 0;
	set.ip_addr_mode = IPADDR_USE_DHCP;

	lwIPInit(drv_info, &set);
	helper.tsk_safe_open(drv_info->info.drv_index, 0);
	helper.mode0 = 0; //mode0=0 control handle
	helper.tsk_start_read(NULL, 0);
    while(1)
    {
    	unsigned int sig;

    	sig = tsk_wait_signal(SIGNAL_ANY, 10);

        // 1) process ISRs
    	if(sig)
    	{
			//queue in/out packets
            stellarisif_interrupt(&drv_data->lwip_netif);

			//process input
		    ethernetif_input(&drv_data->lwip_netif);
	    	drv_info->hw_base->MACIM |= (ETH_INT_TX |  ETH_INT_RX);
    	}

        // 2) get waiting clients
		if(sig & helper.signal)
        {
			tcp_handle* client;

        	helper.res &= ~FLG_SIGNALED;

			while( (client = (tcp_handle*)helper.dst.as_voidptr) )
			{
				RES_CODE res  = RES_SIG_ERROR;

				helper.dst.as_voidptr = client->next;

			    if(client->cmd & FLAG_COMMAND)
			    {
			    	if((client->cmd>>4) <= MAX_LWIPCALLBACK)
			    	{
			    		res = lwip_api_functions[client->cmd>>4](client, &drv_data->lwip_netif);
			    	}
			    } else
			    {
				    if(client->cmd & FLAG_WRITE)
				    {
				    	res = lwip_api_write(client, &drv_data->lwip_netif);
				    } else
					    if(client->cmd & FLAG_READ)
					    {
					    	res = lwip_api_read(client);
					    }
			    }

			    if(res & FLG_SIGNALED)
			    {
				    tsk_HND_SET_STATUS(client, res);
			    }
			}
			helper.tsk_start_read(NULL, 0);
        }

        // 3) process lwip timeouts
		sig = CURRENT_TIME;
    	if(sig != drv_data->timer_main)
    	{
        	if(sig < drv_data->timer_main)
        	{
        		//timer overflow
        		drv_data->timer_mdix &= 0x7FFFFFFF;
        		drv_data->timer_tcp  &= 0x7FFFFFFF;
				#if HOST_TMR_INTERVAL
				drv_data->timer_host &= 0x7FFFFFFF;
				#endif
				#if LWIP_ARP
				drv_data->timer_arp &= 0x7FFFFFFF;
				#endif
				#if LWIP_AUTOIP
				drv_data->timer_autoIP &= 0x7FFFFFFF;
				#endif
				#if LWIP_DHCP
				drv_data->timer_DHCPCoarse &= 0x7FFFFFFF;
				drv_data->timer_DHCPFine &= 0x7FFFFFFF;
				#endif
        	}
        	drv_data->timer_main = sig;
            lwIPServiceTimers(&drv_data->lwip_netif);
    	}
    }


}
TASK_DECLARE_STATIC(lwipdrv_task, "LWIP", (void (*)(void))lwipdrv_thread, 60, 400 + TRACE_SIZE);


//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void LWIP_DCR(LWIP_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param)
{
	LWIP_DRIVER_DATA* drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
        	drv_info->drv_data->lwip_netif.state = drv_info->hw_base;
        	NVIC->NVIC_SetPriority(drv_info->info.drv_index,
				drv_info->info.isr_priority);
//        	SysCtlPeripheralDisable(drv_info->info.peripheral_indx);
           	usr_task_init_static(&lwipdrv_task_desc, true);
           	lwipdrv_task.sp->r0.as_voidptr = drv_info;

        	break;

        case DCR_OPEN:
        	param->mode0 = 1;	// mark the handle as client
        						// helper must clear this field

        	if(param->mode.as_byteptr)
        	{
        		lwip_tcp_setup((tcp_handle*) param, (struct tcp_pcb*)param->mode.as_voidptr);
        		param->mode1 = TCPHS_ESTABLISHED;
        	} else
        	{
            	param->mode1 = TCPHS_UNKNOWN;
        	}

			param->res = RES_OK;
        	break;

	    case DCR_CANCEL:
	    	if(param->mode0)
	    	{
	    		// this is a client handle...
	    		// first try the waiting
	    		if(!param->svc_list_cancel(drv_data->waiting))
	    		{
	    			//try to cancel read & accept
	    			if(param->res & FLG_BUSY)
	    			{
						if(locked_clr_byte(&param->res, TCPHS_CANCELABLE))
						{
							svc_HND_SET_STATUS(param, RES_SIG_CANCEL);
						}
	    			}

	    		}
	    	} else
	    	{
	    		// helper
	    		if(param == drv_data->helper)
	    		{
					//the helper task is waiting for object...
					drv_data->helper = NULL;
					param->dst.as_voidptr = NULL;
					svc_HND_SET_STATUS(param, RES_SIG_OK);
	    		}
	    	}

			break;

   }

}

//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void LWIP_DSR(LWIP_DRIVER_INFO* drv_info, HANDLE hnd)
{
	LWIP_DRIVER_DATA* drv_data = drv_info->drv_data;
	HANDLE tmp;

	if(hnd->mode0)
	{
		// this is a client handle...
		hnd->res = RES_BUSY;
		if( (tmp=drv_data->helper) )
		{
			//the helper task is waiting for object...
			hnd->next = NULL;
			drv_data->helper = NULL;
			tmp->dst.as_voidptr = hnd;
			svc_HND_SET_STATUS(tmp, RES_SIG_OK);
		} else
		{
			//queue the WINDOW object while helper task is busy
			hnd->next = drv_data->waiting;
			drv_data->waiting = hnd;
		}
	} else
	{
		// this should be the helper task
		if( (tmp=drv_data->waiting) )
		{
			drv_data->waiting = NULL;
			hnd->dst.as_voidptr = tmp;
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		} else
		{
			hnd->res = RES_BUSY;
			drv_data->helper = hnd;
		}
	}
}

//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
//*****************************************************************************
//
//! Handles Ethernet interrupts for the lwIP TCP/IP stack.
//!
//! This function handles Ethernet interrupts for the lwIP TCP/IP stack.  At
//! the lowest level, all receive packets are placed into a packet queue for
//! processing at a higher level.  Also, the transmit packet queue is checked
//! and packets are drained and transmitted through the Ethernet MAC as needed.
//! If the system is configured without an RTOS, additional processing is
//! performed at the interrupt level.  The packet queues are processed by the
//! lwIP TCP/IP code, and lwIP periodic timers are serviced (as needed).
//!
//! \return None.
//
//*****************************************************************************

void LWIP_ISR(LWIP_DRIVER_INFO* drv_info )
{
	MAC_Type* mac = drv_info->hw_base;
    unsigned long ulStatus;

    //
    // Read and Clear the interrupt.
    //
    ulStatus = mac->MACRIS;
    mac->MACIACK = ulStatus;

    //
    // If a transmit/rx interrupt was active, run the low-level interrupt
    // handler.
    //
    if(ulStatus)
    {
    	usr_send_signal(&lwipdrv_task, LWIP_THREAD_RXTXSIG);
    	mac->MACIM &= ~  (ETH_INT_TX |  ETH_INT_RX);
    }

}

