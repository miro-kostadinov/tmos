/*
 * lwip_api.h
 *
 *  Created on: 29.04.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LWIP_API_H_
#define LWIP_API_H_

#include <lwip/netif.h>
#include <lwip/tcp.h>
#include <mqueue.h>

#define LWIP_CMD_TCP_NEW		(( 0 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_BIND		(( 1 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_LISTEN		(( 2 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_ACCEPT
#define LWIP_CMD_TCP_RECV		(( 3 <<4)+CMD_COMMAND)

#define MAX_LWIPCALLBACK 		4

#ifndef LWIP_DRV_MAX_API_QUEUE
#define LWIP_DRV_MAX_API_QUEUE 	3
#endif

enum tcp_handle_state{
	TCPHS_UNKNOWN,
	TCPHS_NEW,
	TCPHS_BIND,
	TCPHS_LISTEN,
	TCPHS_ACCEPT,
	TCPHS_WRITE,
	TCPHS_READ
};

typedef RES_CODE (*LWIP_API_FUNC)(HANDLE client, struct netif *netif);
EXTERN_C const LWIP_API_FUNC lwip_api_functions[MAX_LWIPCALLBACK+1];


class tcp_handle: public CHandle
{
public:
	tcp_handle(): recv_pos(0) {};

	unsigned int recv_pos;
	fmqueue< struct tcp_pcb*, LWIP_DRV_MAX_API_QUEUE >	accept_que;
	fmqueue< struct pbuf*, LWIP_DRV_MAX_API_QUEUE >	recv_que;


	//--- not blocking functions

#ifdef LWIP_CMD_TCP_NEW
	RES_CODE lwip_tcp_new(unsigned int priority = TCP_PRIO_NORMAL);
#endif

#ifdef LWIP_CMD_TCP_BIND
	RES_CODE lwip_tcp_bind(ip_addr_t *ipaddr, u16_t port);
#endif

#ifdef LWIP_CMD_TCP_LISTEN
	RES_CODE lwip_tcp_listen();
#endif

	//--- blocking functions
#ifdef LWIP_CMD_TCP_ACCEPT
	RES_CODE lwip_tcp_accept(struct tcp_pcb*& newpcb);	//waits until connection
#endif

};

RES_CODE lwip_api_read(tcp_handle* client);
RES_CODE lwip_api_write(tcp_handle* client, struct netif *netif);
void lwip_tcp_setup(tcp_handle* client, struct tcp_pcb *pcb);


#endif /* LWIP_API_H_ */
