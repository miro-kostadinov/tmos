/*
 * lwip_drv.h
 *
 *  Created on: 22.03.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LWIP_DRV_H_
#define LWIP_DRV_H_

#include <tmos.h>
#include <hardware_cpp.h>
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/netif.h"
#include <csocket.h>


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

RES_CODE lwip_api_read(CSocket* client);
RES_CODE lwip_api_write(CSocket* client, struct netif *netif);
RES_CODE lwip_process_cmd(CSocket* client, struct netif *netif);

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
	struct ip_addr ip_addr;
	struct ip_addr net_mask;
	struct ip_addr gw_addr;
	unsigned char ip_addr_mode; // IPADDR_USE_XXX
};

struct LWIP_DRIVER_INFO;

struct LWIP_DRIVER_DATA
{
	/** The lwIP network interface structure for the Ethernet MAC. **/
	netif lwip_netif;

	const LWIP_DRIVER_INFO* drv_info;

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

    HANDLE			waiting;
    HANDLE			helper;

	/** The default IP address aquisition mode. **/
	unsigned char ip_addr_mode;
};

/*
      ETH_MDIO -------------------------> PA2
      ETH_MDC --------------------------> PC1
      ETH_PPS_OUT ----------------------> PB5  / ??
      ETH_MII_CRS ----------------------> PH2  / ??
      ETH_MII_COL ----------------------> PH3  / ??
      ETH_MII_RX_ER --------------------> PI10 / ??
      ETH_MII_RXD2 ---------------------> PH6  / ??
      ETH_MII_RXD3 ---------------------> PH7  / ??
      ETH_MII_TX_CLK -------------------> PC3  / ??
      ETH_MII_TXD2 ---------------------> PC2  / ??
      ETH_MII_TXD3 ---------------------> PB8  / ??
      ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
      ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
      ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
      ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
      ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PG11
      ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13
      ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14

      MCO1							----> PA8
                                                */
struct LWIP_DRIVER_INFO
{
	DRIVER_INFO_Type 		info;		//!< standard driver info
	ETH_TypeDef*			hw_base;	//!< pointer to the hardware peripheral
	LWIP_DRIVER_DATA* 		drv_data;	//!< pointer to the driver data
	const PIN_DESC*	  		eth_pins;	//!< zero terminated PIN_DESC list
	const eth_mac_cfg_t*	mac_cfg;	//!< mac configuration
};


void LWIP_DCR(LWIP_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param);
void LWIP_DSR(LWIP_DRIVER_INFO* drv_info, HANDLE hnd);
void LWIP_ISR(LWIP_DRIVER_INFO* drv_info );

#endif /* LWIP_DRV_H_ */
