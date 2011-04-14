/*
 * lwip_drv.h
 *
 *  Created on: 22.03.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LWIP_DRV_H_
#define LWIP_DRV_H_

#include <tmos.h>
#include <ethernet_lm3s.h>

#include "lwip/opt.h"
#include "lwip/netif.h"


//*****************************************************************************
//
// The lwIP Library abstration layer provides for a host callback function
// to be called periodically in the lwIP context.  This is the timer
// interval, in ms, for this periodic callback.  If the timer interval is
// defined to 0 (the default value), then no periodic host callback is
// performed.
//
//*****************************************************************************
#ifndef HOST_TMR_INTERVAL
#define HOST_TMR_INTERVAL       0
#else
extern "C" void lwIPHostTimerHandler(void);
#endif

/**
 * Number of pbufs supported in low-level tx/rx pbuf queue.
 *
 */
#ifndef STELLARIS_NUM_PBUF_QUEUE
#define STELLARIS_NUM_PBUF_QUEUE    20
#endif

/* Helper macros for accessing pbuf queues. */
#define PBUF_QUEUE_EMPTY(q) \
    (((q)->qwrite == (q)->qread) ? true : false)

#define PBUF_QUEUE_FULL(q) \
    ((((((q)->qwrite + 1) % STELLARIS_NUM_PBUF_QUEUE)) == (q)->qread) ? \
    true : false )

/* Define those to better describe your network interface. */
#define IFNAME0 'l'
#define IFNAME1 'm'

//*****************************************************************************
//
// IP Address Acquisition Modes
//
//*****************************************************************************
#define IPADDR_USE_STATIC       0
#define IPADDR_USE_DHCP         1
#define IPADDR_USE_AUTOIP       2


/* Helper struct to hold a queue of pbufs for transmit and receive. */
struct pbufq
{
	struct pbuf *pbuf[STELLARIS_NUM_PBUF_QUEUE];
	unsigned long qwrite;
	unsigned long qread;
	unsigned long overflow;
};

struct ip_adr_set
{
	unsigned long ip_addr;
	unsigned long net_mask;
	unsigned long gw_addr;
	unsigned char ip_addr_mode; // IPADDR_USE_XXX
};

struct LWIP_DRIVER_MODE
{
	unsigned int	baudrate;	//!< baudrate for the mode
};

struct LWIP_DRIVER_DATA
{
	/** The lwIP network interface structure for the Ethernet MAC. **/
	netif lwip_netif;

	/** The local time for the lwIP Library Abstraction layer, used to support
	 * the Host and lwIP periodic callback functions.  **/
	unsigned long timer_main;

	/** The local time when the soft MDI/MDIX switch will be switched. **/
	unsigned long timer_mdix;

	/** The local time when the TCP timer will be serviced. **/
	unsigned long timer_tcp;

	#if HOST_TMR_INTERVAL
	/** The local time when the HOST timer will be serviced. **/
	unsigned long timer_host;
	#endif


	/** The local time when the ARP timer will be serviced. **/
	#if LWIP_ARP
	unsigned long timer_arp;
	#endif

	/** The local time when the AutoIP timer will be serviced. **/
	#if LWIP_AUTOIP
	unsigned long timer_autoIP;
	#endif

	/** The local time when the DHCP Coarse timer will be serviced. **/
	#if LWIP_DHCP
	unsigned long timer_DHCPCoarse;
	#endif

	/** The local time when the DHCP Fine timer will be serviced. **/
	#if LWIP_DHCP
	unsigned long timer_DHCPFine;
	#endif

	struct pbufq txq;
	struct pbufq rxq;

	/** The default IP address aquisition mode. **/
	unsigned char ip_addr_mode;
};

struct LWIP_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;		//!< standard driver info
	MAC_Type *			hw_base;	//!< pointer to the hardware peripheral
	LWIP_DRIVER_DATA* 	drv_data;	//!< pointer to the driver data
};


void LWIP_DCR(LWIP_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param);
void LWIP_DSR(LWIP_DRIVER_INFO* drv_info, HANDLE hnd);
void LWIP_ISR(LWIP_DRIVER_INFO* drv_info );

#endif /* LWIP_DRV_H_ */
