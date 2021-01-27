/*
 * lwip_api.cpp
 *
 *  Created on: 29.04.2011
 *      Author: Miroslav Kostadinov
 */


#include <tmos.h>
#include "lwip/opt.h"
#include <lwip_api.h>
#include "lwip/netif.h"
#include <lwip/tcp.h>
#include <lwip/tcp_impl.h>
#include <lwip/udp.h>
#include <mqueue.h>
#include <lwip_drv.h>

extern "C"
{
#include <lwip/dns.h>
}

#ifndef LWIP_TCP_PCBS_CNT
#define LWIP_TCP_PCBS_CNT 4
#endif

#ifndef LWIP_UDP_PCBS_CNT
#define LWIP_UDP_PCBS_CNT 4
#endif

struct lwip_sock_data_t
{
	struct tcp_pcb* pcb;
	unsigned int recv_pos;
	fmqueue< struct tcp_pcb*, LWIP_DRV_MAX_API_QUEUE >	accept_que;
	fmqueue< struct pbuf*, LWIP_DRV_MAX_API_QUEUE >	recv_que;
};

struct lwip_sock_data_t g_lwip_socks[LWIP_TCP_PCBS_CNT];

#if LWIP_UDP_PCBS_CNT
struct lwip_udp_rcv_data_t
{
	struct pbuf* p;
	ip_addr_t 	addr;
	u16_t 		port;
};
struct lwip_udp_sock_data_t
{
	struct udp_pcb* pcb;
	unsigned int recv_pos;
	fmqueue< lwip_udp_rcv_data_t, LWIP_DRV_MAX_API_QUEUE >	recv_que;
};

struct lwip_udp_sock_data_t g_lwip_udp_socks[LWIP_UDP_PCBS_CNT];
#endif

//--------------------   WRITE   ---------------------------------------------//
/**
 * Sent callback function for TCP netconns.
 *
 * Called when sent data has been acknowledged by the remote side. Use it to
 * free corresponding resources. This also means that the pcb has now space
 * available to send new data.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb for which data has been acknowledged
 * @param len The amount of bytes acknowledged
 * @return ERR_OK: try to send some data by calling tcp_output
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 */
err_t lwip_cbf_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	CSocket* client = (CSocket*)arg;

	TRACELN_LWIP("LWIP sent %x %x", client, len);
	if(client == NULL)
		return (ERR_VAL);

	client->dst.as_byteptr += len;

	if(client->mode1 == TCPHS_WRITING)
	{
		u8_t apiflags;
		err_t res;

		len = client->len;
		if(len)
		{
			if(client->cmd & FLAG_LOCK)
			    apiflags = TCP_WRITE_FLAG_MORE;
			else
			    apiflags = 0;
			if(len > 0xffff)
			{
				len = 0xffff;
			    apiflags = TCP_WRITE_FLAG_MORE;
			}

			if ( len > tcp_sndbuf(pcb))
			{
			    len = tcp_sndbuf(pcb);
			    apiflags = TCP_WRITE_FLAG_MORE;
			}
			res = tcp_write(pcb, client->src.as_voidptr , len, apiflags);
			//process ERR_MEM here ??

			if(res == ERR_OK)
			{
				//data was queued we will receive a callback
				client->src.as_byteptr += len;
				client->len -= len;
				if(!client->len)
					tcp_output(pcb);
			} else
			{
				//some error occurred
				client->error = res;
				//send signal
				client->mode1 = TCPHS_ESTABLISHED;
				if(client->res & FLG_BUSY)
					tsk_HND_SET_STATUS(client, RES_SIG_ERROR);
			}
		} else
		{
			//send signal
			client->mode1 = TCPHS_ESTABLISHED;
			if(client->res & FLG_BUSY)
				tsk_HND_SET_STATUS(client, RES_SIG_OK);
		}
	}
	return (ERR_OK);
}

RES_CODE lwip_api_write(CSocket* client, struct netif *netif)
{
	RES_CODE res = RES_SIG_ERROR | RES_FATAL;
	lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

	//remember the start position
	client->dst.as_voidptr = client->src.as_voidptr;

	if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
	{
		if(client->mode1 == TCPHS_ESTABLISHED && client->sock_id < LWIP_TCP_PCBS_CNT)
		{
			lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];

			if(sock_data->pcb)
			{
				unsigned int len;
				u8_t apiflags;

				len = client->len;
				if(len)
				{

					if(client->cmd & FLAG_LOCK)
					    apiflags = TCP_WRITE_FLAG_MORE;
					else
					    apiflags = 0;
					if(len > 0xffff)
					{
						len = 0xffff;
					    apiflags = TCP_WRITE_FLAG_MORE;
					}

					if ( len > tcp_sndbuf(sock_data->pcb))
					{
					    len = tcp_sndbuf(sock_data->pcb);
					    apiflags = TCP_WRITE_FLAG_MORE;
					}
					res = tcp_write(sock_data->pcb, client->src.as_voidptr , len, apiflags);
					//process ERR_MEM here ??

					if(res == ERR_OK)
					{
						//data was queued we will receive a callback
						client->mode1 = TCPHS_WRITING;

						client->src.as_byteptr += len;
						client->len -= len;
						if( !(client->cmd & FLAG_LOCK) && !client->len)
							tcp_output(sock_data->pcb);
						res = 0;
					} else
					{
						//some error occurred
						client->error = res;
						res = RES_SIG_ERROR;
					}
				} else
				{
					res = RES_SIG_OK;
				}
			}
		}
	} else
	{
#if LWIP_UDP_PCBS_CNT
		if(sock_mode->mode.sock_type == IP_SOCKET_UDP)
		{
			if(client->mode1 == TCPHS_ESTABLISHED && client->sock_id < LWIP_UDP_PCBS_CNT)
			{
				lwip_udp_sock_data_t* sock_data = &g_lwip_udp_socks[client->sock_id];

				if(sock_data->pcb)
				{
					if(netif_is_link_up(netif))
					{
						res = RES_SIG_OK;
						while(client->len)
						{
							struct pbuf *p;
							unsigned int len;

							len = client->len;
							if(len > 508)
								len = 508;

				            p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
				            if(p == NULL)
				            {
								client->error = res;
								res = RES_SIG_ERROR;
								break;
				            }
							memcpy(p->payload, client->src.as_voidptr , len);
							client->error = udp_send(sock_data->pcb, p);
							pbuf_free(p);
							if(client->error == ERR_OK)
							{
								client->src.as_byteptr += len;
								client->len -= len;
							} else
							{
								res = RES_SIG_ERROR;
								break;
							}

						}
					} else
						res = RES_SIG_ERROR;
				}
			}
		}
#endif
	}


	return (res);
}

//--------------------   READ   ---------------------------------------------//
/**
 * Receive callback function for TCP netconns.
 *
 * @see tcp.h (struct tcp_pcb.recv) for parameters and return value
 */
err_t lwip_cbf_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	CSocket* client = (CSocket*)arg;

	TRACELN_LWIP("LWIP recv %x %x", client, err);
	if(p)
	{
		TRACE_LWIP(" len=%d tot=%d", p->len, p->tot_len);
	}

	if(client && client->sock_id < LWIP_TCP_PCBS_CNT)
	{
		lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];

		if(sock_data->recv_que.push(p))
		{
			//send signal
			//Get from the driver
			if(locked_clr_byte(&client->mode1, TCPHS_OP_READING))
			{
				RES_CODE res = lwip_api_read(client);

				if(res & FLG_SIGNALED)
				{
					tsk_HND_SET_STATUS(client, res);
				}
			}

			// Packet is accepted!
			return (ERR_OK);
		}
	}
	//Refuse the packet now. We will'be called again from the tcp timer
	return (ERR_VAL);
}

RES_CODE lwip_api_read(CSocket* client)
{
	RES_CODE res;

	if(!client->len)
	{
		res =  RES_SIG_OK;
	} else
	{
		lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

		res = RES_SIG_ERROR | RES_FATAL;
		if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
		{
			if(client->sock_id < LWIP_TCP_PCBS_CNT)
			{
				lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];
				struct pbuf *p;

				res = RES_IDLE;
				if(client->mode1 == TCPHS_ESTABLISHED)
				{
					while( client->len && !sock_data->recv_que.empty() )
					{
						unsigned int to_read;

						p = sock_data->recv_que.top();

						to_read = pbuf_copy_partial(p, client->dst.as_byteptr, client->len,
								sock_data->recv_pos);
						if(to_read)
						{
							client->dst.as_byteptr += to_read;
							client->len -= to_read;
							sock_data->recv_pos += to_read;

							if(sock_data->recv_pos == p->tot_len)
							{
								tcp_recved(sock_data->pcb, p->tot_len);
								pbuf_free(p);
								sock_data->recv_que.pop(p);
								sock_data->recv_pos =0;
							}

							res = RES_SIG_OK;
						} else
						{
							// nothing was read from this buffer
							if(p)
							{
								to_read = p->tot_len;
								if(sock_data->recv_pos <= to_read)
									to_read -= sock_data->recv_pos;
								sock_data->recv_pos =0;
								tcp_recved(sock_data->pcb, to_read);
								pbuf_free(p);
								res = RES_SIG_OK;

							} else
							{
								if(res != RES_SIG_OK)
									res = RES_EOF | FLG_SIGNALED;
								locked_clr_byte(&client->mode1, TCPHS_OP_ESTABLISHED);
							}

							sock_data->recv_que.pop(p);

							return res;
						}
					}
				} else
					res = RES_EOF | FLG_SIGNALED;
			}
		} else
		{
#if LWIP_UDP_PCBS_CNT
			if(sock_mode->mode.sock_type == IP_SOCKET_UDP && client->sock_id < LWIP_UDP_PCBS_CNT)
			{
				lwip_udp_sock_data_t* sock_data = &g_lwip_udp_socks[client->sock_id];
				struct lwip_udp_rcv_data_t pdata;

				res = RES_IDLE;
				if(client->mode1 == TCPHS_ESTABLISHED)
				{
					if( client->len && !sock_data->recv_que.empty() )
					{
						unsigned int to_read;

						pdata = sock_data->recv_que.top();

						to_read = pbuf_copy_partial(pdata.p, client->dst.as_byteptr, client->len,
								sock_data->recv_pos);
						if(to_read)
						{
							client->dst.as_byteptr += to_read;
							client->len -= to_read;
							sock_data->recv_pos += to_read;

							res = RES_SIG_OK;
							if(sock_data->recv_pos == pdata.p->tot_len)
							{
								pbuf_free(pdata.p);
								sock_data->recv_que.pop(pdata);
								sock_data->recv_pos =0;
								res |= FLG_EOF;
							}
						} else
						{
							// nothing was read from this buffer
							if(pdata.p)
							{
								sock_data->recv_pos =0;
								pbuf_free(pdata.p);
								res = RES_SIG_OK | FLG_EOF;

							} else
							{
								res = RES_SIG_ERROR | RES_FATAL;
							}

							sock_data->recv_que.pop(pdata);
						}
					}
				}
			}
#endif
		}
	}

	if(!res)
	{
		//Give to the driver
		client->mode1 = TCPHS_READING;
	}
	return (res);
}

#if LWIP_UDP_PCBS_CNT
void lwip_cbf_udp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
	CSocket* client = (CSocket*)arg;

	TRACELN_LWIP("LWIP UDP recv %x", client);
	if(p)
	{
		TRACE_LWIP(" len=%d tot=%d", p->len, p->tot_len);

		if(client && client->sock_id < LWIP_UDP_PCBS_CNT)
		{
			lwip_udp_sock_data_t* sock_data = &g_lwip_udp_socks[client->sock_id];

			if(sock_data->recv_que.push({p,*addr,port}))
			{
				//send signal
				//Get from the driver
				p = NULL;
				if(locked_clr_byte(&client->mode1, TCPHS_OP_READING))
				{
					RES_CODE res = lwip_api_read(client);

					if(res & FLG_SIGNALED)
					{
						tsk_HND_SET_STATUS(client, res);
					}
				} else
					if(locked_clr_byte(&client->mode1, TCPHS_OP_ACCEPTING))
					{
						*client->src.as_intptr = addr->addr;
						*client->dst.as_shortptr = port;

						tsk_HND_SET_STATUS(client, RES_SIG_OK);
					}

			}
		}
	}
	if(p)
		pbuf_free(p);
}
#endif //LWIP_UDP_PCBS_CNT

//--------------------   Helper   --------------------------------------------//
/**
 * Error callback function for TCP netconns.
 * Signals conn->sem, posts to all conn mboxes and calls API_EVENT.
 * The application thread has then to decide what to do.
 *
 * Function prototype for tcp error callback functions. Called when the pcb
 * receives a RST or is unexpectedly closed for any other reason.
 *
 * @note The corresponding pcb is already freed when this callback is called!
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param err Error code to indicate why the pcb has been closed
 *            ERR_ABRT: aborted through tcp_abort or by a TCP timer
 *            ERR_RST: the connection was reset by the remote host
 */
void lwip_cbf_err(void *arg, err_t err)
{
	CSocket* client = (CSocket*)arg;

	TRACELN_LWIP("TCP ERR %x for %x", err, client);
	if(client && client->sock_id < LWIP_TCP_PCBS_CNT)
	{
		//send signal
		client->mode1 = TCPHS_UNKNOWN;
		g_lwip_socks[client->sock_id].pcb = NULL;
		client->sock_id = SOCKET_ID_INVALID;
		client->error = err;

		if (client->res & FLG_BUSY)
			tsk_HND_SET_STATUS(client, RES_SIG_ERROR | RES_FATAL);
	}
}



void lwip_tcp_setup(CSocket* client, struct tcp_pcb *pcb)
{
	pcb->callback_arg = client;
	pcb->recv = lwip_cbf_recv;
	pcb->sent = lwip_cbf_sent;
	pcb->errf = lwip_cbf_err;
}



//--------------------   TCP NEW   -------------------------------------------//
RES_CODE lwip_sock_open(CSocket* client, struct netif *netif)
{
	RES_CODE res = RES_SIG_ERROR;
	struct tcp_pcb *pcb;
	lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

	//release the old one?
	if(sock_mode && client->sock_id == SOCKET_ID_INVALID)
	{
		switch(sock_mode->mode.sock_type)
		{
		case IP_SOCKET_TCP:
			for(uint32_t i=0; i <LWIP_TCP_PCBS_CNT; i++)
			{
				if(g_lwip_socks[i].pcb == NULL)
				{
					pcb = tcp_alloc(sock_mode->lwip_priority);
					if(pcb)
					{
						g_lwip_socks[i].pcb = pcb;
						g_lwip_socks[i].recv_pos = 0;
						client->sock_id = i;
						client->mode1 = TCPHS_NEW;
						lwip_tcp_setup(client, pcb);
						res = RES_SIG_OK;
					} else
						res = RES_SIG_OUT_OF_MEMORY;
					break;
				}
			}
			break;
#if LWIP_UDP_PCBS_CNT
		case IP_SOCKET_UDP:
			for(uint32_t i=0; i <LWIP_UDP_PCBS_CNT; i++)
			{
				if(g_lwip_udp_socks[i].pcb == NULL)
				{
					g_lwip_udp_socks[i].pcb = udp_new();
					if(g_lwip_udp_socks[i].pcb)
					{
						g_lwip_udp_socks[i].recv_pos = 0;
						client->sock_id = i;
						client->mode1 = TCPHS_NEW;
						g_lwip_udp_socks[i].pcb->recv_arg = client;
						g_lwip_udp_socks[i].pcb->recv = lwip_cbf_udp_recv;
						res = RES_SIG_OK;
					} else
						res = RES_SIG_OUT_OF_MEMORY;
					break;
				}
			}
			break;
#endif //LWIP_UDP_PCBS_CNT
		}
	}

	return (res);
}


//--------------------   CLOSE   ---------------------------------------------//

static void api_tcp_drain(CSocket* client)
{
	lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;
	struct pbuf *p;

	if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
	{
		if(client->sock_id < LWIP_TCP_PCBS_CNT)
		{
			lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];
			struct tcp_pcb *pcb;

			// Drain the recv_que.
			while( sock_data->recv_que.pop(p) )
			{
				if (p)
				{
					unsigned int len = p->tot_len;
					if(sock_data->recv_pos <= len)
						len -= sock_data->recv_pos;
					tcp_recved(sock_data->pcb,	len);
					pbuf_free(p);
				}
				sock_data->recv_pos = 0;
			}
			// Drain the accept_que.
			while(sock_data->accept_que.pop(pcb))
			{
				tcp_abort(pcb);
			}
		}
	} else
	{
#if LWIP_UDP_PCBS_CNT
		if(client->sock_id < LWIP_UDP_PCBS_CNT)
		{
			lwip_udp_rcv_data_t pdata;
			lwip_udp_sock_data_t* sock_data = &g_lwip_udp_socks[client->sock_id];

			// Drain the recv_que.
			while( sock_data->recv_que.pop(pdata) )
			{
				if (pdata.p)
				{
					pbuf_free(pdata.p);
				}
				sock_data->recv_pos = 0;
			}
		}
#endif
	}

}

static RES_CODE api_close_internal(CSocket* client, unsigned int rxtx)
{
	err_t res;
	lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

	if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
	{
		struct tcp_pcb *pcb;

		if(client->sock_id < LWIP_TCP_PCBS_CNT)
			pcb = g_lwip_socks[client->sock_id].pcb;
		else
			pcb = NULL;

		if(pcb && client->mode1)
		{
			// Set back some callback pointers
			if (rxtx == LWIP_SHUT_RDWR)
			{
				pcb->callback_arg = NULL;
			}

			if (pcb->state == LISTEN)
			{
				pcb->accept = NULL;
			} else
			{
				// some callbacks have to be reset if tcp_close is not successful
				if (rxtx & LWIP_SHUT_RD)
				{
					pcb->recv = NULL;
					pcb->accept = NULL;
				}
				if (rxtx & LWIP_SHUT_WR)
				{
					pcb->sent = NULL;
				}
				if (rxtx == LWIP_SHUT_RDWR)
				{
					pcb->poll = NULL;
					pcb->pollinterval = 4;
					pcb->errf = NULL;
				}
			}

			// Try to close the connection
			if (rxtx == LWIP_SHUT_RDWR)
			{
				res = tcp_close(pcb);
			}
			else
			{
				res = tcp_shutdown(pcb, rxtx & LWIP_SHUT_RD, rxtx & LWIP_SHUT_WR);
			}

			if (res == ERR_OK)
			{
				// Closing succeeded
				client->mode1 = TCPHS_UNKNOWN;
				// Set back some callback pointers as conn is going away
				g_lwip_socks[client->sock_id].pcb = NULL;
				client->sock_id = SOCKET_ID_INVALID;

			}
			else
			{
				// Closing failed, restore some of the callbacks
				// Closing of listen pcb will never fail!
				lwip_tcp_setup(client, pcb);
				// don't restore recv callback: we don't want to receive any more data
				pcb->recv = NULL;

				// If closing didn't succeed, we get called again either
				// from poll_tcp or from sent_tcp
				return RES_SIG_IDLE;
			}
		}
		return RES_SIG_OK;
	} else
	{
#if LWIP_UDP_PCBS_CNT
		if(sock_mode->mode.sock_type == IP_SOCKET_UDP)
		{
			struct udp_pcb *pcb;

			if(client->sock_id < LWIP_UDP_PCBS_CNT)
				pcb = g_lwip_udp_socks[client->sock_id].pcb;
			else
				pcb = NULL;

			if(pcb && client->mode1)
			{
				pcb->recv = NULL;
				udp_disconnect(pcb);
				udp_remove(pcb);
				client->mode1 = TCPHS_UNKNOWN;
				g_lwip_udp_socks[client->sock_id].pcb = NULL;
				client->sock_id = SOCKET_ID_INVALID;

			}
			return RES_SIG_OK;
		}
#endif
	}


	return RES_SIG_ERROR;
}

RES_CODE lwip_sock_close(CSocket* client, struct netif *netif)
{
	RES_CODE res;

	api_tcp_drain(client);

	if(client->mode1 & TCPHS_OP_PCB)
	{
		if((client->mode1 & (TCPHS_OP_WRITING | TCPHS_OP_CONNECTING)))
		{
			/* @to do TCP: abort running write/connect? */
			res = RES_SIG_IDLE;
		} else
		{
			res = api_close_internal(client, LWIP_SHUT_RDWR);
		}
	} else
	{
		//nothing to delete
		res = RES_SIG_OK;
	}
	return (res);

}

//--------------------   DNS   ---------------------------------------------//
static void ipaddr_nto_cstr(const ip_addr_t* addr, CSTRING* str)
{
	uint8_t* ptr = (uint8_t*)addr;
	if(str)
	{
		str->appendf("%u.%u.%u.%u", ptr[0], ptr[1], ptr[2], ptr[3]);
	}
}

/** TCP callback function for tcp connected callback functions. Called when a pcb
 * is connected to the remote side after initiating a connection attempt by
 * calling tcp_connect().
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb which is connected
 * @param err An unused error code, always ERR_OK currently ;-) TODO!
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 *
 * @note When a connection attempt fails, the error callback is currently called!
 */
err_t lwip_cbf_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
	CSocket* client = (CSocket*)arg;

	TRACELN_LWIP("LWIP connect %x", client);
	if(client == NULL)
		return (ERR_VAL);

	if(client->mode1 == TCPHS_CONECTING)
	{
		client->mode1 = TCPHS_ESTABLISHED;
		if(client->res & FLG_BUSY)
			tsk_HND_SET_STATUS(client, RES_SIG_OK);

	}

	return (ERR_OK);
}


RES_CODE lwip_sock_get_adr(CSocket* client, struct netif *netif)
{
	lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

	if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
	{
		if(client->sock_id < LWIP_TCP_PCBS_CNT)
		{
			lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];

			if(sock_data->pcb  )
			{
				if(client->len)
				{
					*client->src.as_intptr = sock_data->pcb->local_ip.addr;
					*client->dst.as_shortptr = sock_data->pcb->local_port;
				} else
				{
					*client->src.as_intptr = sock_data->pcb->remote_ip.addr;
					*client->dst.as_shortptr = sock_data->pcb->remote_port;
				}
				return RES_SIG_OK;
			}
		}
	} else
	{
#if LWIP_UDP_PCBS_CNT
		if(sock_mode->mode.sock_type == IP_SOCKET_UDP)
		{
			if(client->sock_id < LWIP_UDP_PCBS_CNT)
			{
				lwip_udp_sock_data_t* sock_data = &g_lwip_udp_socks[client->sock_id];

				if(sock_data->pcb  )
				{
					if(client->len)
					{
						*client->src.as_intptr = sock_data->pcb->local_ip.addr;
						*client->dst.as_shortptr = sock_data->pcb->local_port;
					} else
					{
						lwip_udp_rcv_data_t pdata;

						if(sock_data->recv_que.empty())
						{
							client->mode1 |= TCPHS_OP_ACCEPTING;
							return RES_IDLE;
						} else
						{
							pdata = sock_data->recv_que.top();
							*client->src.as_intptr = pdata.addr.addr;
							*client->dst.as_shortptr = pdata.port;
						}
					}
					return RES_SIG_OK;
				}
			}
		}
#endif
	}
	return RES_SIG_ERROR;
}

void lwip_finish_dns(CSocket* client, ip_addr_t *ipaddr)
{
	RES_CODE res = RES_SIG_ERROR;
	lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

	if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
	{
		lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];


		if (ipaddr && client->sock_id < LWIP_TCP_PCBS_CNT)
		{

			switch(client->mode0)
			{
			case 0:	// lwip_sock_get_host
				ipaddr_nto_cstr(ipaddr, (CSTRING*)client->dst.as_voidptr);
				res =  RES_SIG_OK;
				break;

			case 1: // lwip_sock_connect_url
				if(sock_data->pcb && (client->mode1 == TCPHS_NEW) )
				{
					client->error = tcp_connect(sock_data->pcb, ipaddr,
							client->dst.as_int, lwip_cbf_connected);
					if(client->error == ERR_OK)
					{
						client->mode1 = TCPHS_CONECTING;
						return;
					}
				}
				break;

			case 2: // lwip_sock_bind_url
				if(sock_data->pcb && (client->mode1 == TCPHS_NEW) )
				{
					client->error = tcp_bind(sock_data->pcb, ipaddr, client->dst.as_int);

					if(res==ERR_OK)
					{
						client->mode1 = TCPHS_BIND;
						res = RES_SIG_OK;
					}
				}
				break;
			}
		}
	} else
	{
#if LWIP_UDP_PCBS_CNT
		if(sock_mode->mode.sock_type == IP_SOCKET_UDP)
		{
			lwip_udp_sock_data_t* sock_data = &g_lwip_udp_socks[client->sock_id];


			if (ipaddr && client->sock_id < LWIP_UDP_PCBS_CNT)
			{

				switch(client->mode0)
				{
				case 0:	// lwip_sock_get_host
					ipaddr_nto_cstr(ipaddr, (CSTRING*)client->dst.as_voidptr);
					res =  RES_SIG_OK;
					break;

				case 1: // lwip_sock_connect_url
					if(sock_data->pcb && (client->mode1 == TCPHS_NEW) )
					{
						client->error = udp_connect(sock_data->pcb, ipaddr,	client->dst.as_int);
						if(client->error == ERR_OK)
						{
							client->mode1 = TCPHS_ESTABLISHED;
							return;
						}
					}
					break;

				case 2: // lwip_sock_bind_url
					if(sock_data->pcb && (client->mode1 == TCPHS_NEW) )
					{
						client->error = udp_bind(sock_data->pcb, ipaddr, client->dst.as_int);

						if(res==ERR_OK)
						{
							client->mode1 = TCPHS_ESTABLISHED;
							res = RES_SIG_OK;
						}
					}
					break;
				}
			}
		}
#endif
	}

	tsk_HND_SET_STATUS(client, res);
}

/**
 * Callback function that is called when DNS name is resolved
 * (or on timeout). A waiting application thread is waked up by
 * signaling the semaphore.
 */
void lwip_cbf_dns(const char *name, ip_addr_t *ipaddr, void *arg)
{
	CSocket* client = (CSocket*)arg;

	if(client)
	{
		if(client->res & FLG_BUSY)
		{
			lwip_finish_dns(client, ipaddr);
		}
	}
}

RES_CODE lwip_start_dns(CSocket* client, struct netif *netif)
{
	ip_addr_t ip;
	err_t err;

	err = dns_gethostbyname(client->src.as_charptr, &ip, lwip_cbf_dns, client);
	switch (err)
	{
	case ERR_OK:			//done
		break;

	case ERR_INPROGRESS:	// pending
		return 0;

	default:				// failed
		client->error = err;
		return RES_SIG_ERROR;

	}

	// done
	lwip_finish_dns(client, &ip);
	return RES_IDLE;
}

RES_CODE lwip_sock_get_host(CSocket* client, struct netif *netif)
{
	client->mode0 = 0;

	return lwip_start_dns(client, netif);
}


//--------------------   CONNECT   -------------------------------------------//
RES_CODE lwip_sock_connect_adr(CSocket* client, struct netif *netif)
{
	lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

	if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
	{
		if(client->sock_id < LWIP_TCP_PCBS_CNT)
		{
			lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];
			ip_addr ip;

			if(sock_data->pcb && (client->mode1 == TCPHS_NEW) )
			{
				if(ipaddr_aton(client->src.as_charptr, &ip))
				{
					client->error = tcp_connect(sock_data->pcb, &ip,
							client->dst.as_int, lwip_cbf_connected);
					if(client->error == ERR_OK)
					{
						client->mode1 = TCPHS_CONECTING;
						return (0);
					}
				}

			}
		}
	} else
	{
#if LWIP_UDP_PCBS_CNT
		if(sock_mode->mode.sock_type == IP_SOCKET_UDP)
		{
			if(client->sock_id < LWIP_UDP_PCBS_CNT)
			{
				lwip_udp_sock_data_t* sock_data = &g_lwip_udp_socks[client->sock_id];
				ip_addr ip;

				if(sock_data->pcb && (client->mode1 & TCPHS_NEW) )
				{
					if(ipaddr_aton(client->src.as_charptr, &ip))
					{
						client->error = udp_connect(sock_data->pcb, &ip,
								client->dst.as_int);
						if(client->error == ERR_OK)
						{
							client->mode1 = TCPHS_ESTABLISHED;
							return RES_SIG_OK;
						}
					}

				}
			}
		}
#endif
	}
	return RES_SIG_ERROR;
}

RES_CODE lwip_sock_connect_url(CSocket* client, struct netif *netif)
{
	RES_CODE res;

	//try as IP address
	res = lwip_sock_connect_adr(client, netif);
	if(res != RES_IDLE && res != RES_SIG_OK)
	{
		lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

		if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
		{
			if(client->sock_id < LWIP_TCP_PCBS_CNT)
			{
				client->mode0 = 1;

				res = lwip_start_dns(client, netif);
			}
		} else
		{
#if LWIP_UDP_PCBS_CNT
			if(sock_mode->mode.sock_type == IP_SOCKET_UDP)
			{
				if(client->sock_id < LWIP_UDP_PCBS_CNT)
				{
					client->mode0 = 1;

					res = lwip_start_dns(client, netif);
				}
			}
#endif
		}

	}
	return res;
}


//--------------------   TCP BIND   ------------------------------------------//
#ifdef LWIP_CMD_TCP_BIND

RES_CODE lwip_sock_bind_adr(CSocket* client, struct netif *netif)
{
	lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

	if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
	{
		if(client->sock_id < LWIP_TCP_PCBS_CNT)
		{
			lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];
			err_t res;
			ip_addr ip;
			ip_addr* pip;

			if(sock_data->pcb && (client->mode1 == TCPHS_NEW) )
			{
				if(!client->src.as_charptr)
					pip = &netif->ip_addr;//NULL;
				else
				{
					if(ipaddr_aton(client->src.as_charptr, &ip))
						pip = &ip;
					else
						pip = NULL;
				}
//				if(!pip || ipaddr_aton(client->src.as_charptr, &ip))
				{
					res = tcp_bind(sock_data->pcb, pip, client->dst.as_int);

					client->error = res;
					if(res==ERR_OK)
					{
						client->mode1 = TCPHS_BIND;
						return RES_SIG_OK;
					}
				}
			}
		}
	} else
	{
#if LWIP_UDP_PCBS_CNT
		if(sock_mode->mode.sock_type == IP_SOCKET_UDP)
		{
			if(client->sock_id < LWIP_UDP_PCBS_CNT)
			{
				lwip_udp_sock_data_t* sock_data = &g_lwip_udp_socks[client->sock_id];
				err_t res;
				ip_addr ip;
				ip_addr* pip;

				if(sock_data->pcb && (client->mode1 == TCPHS_NEW) )
				{
					if(!client->src.as_charptr)
						pip = NULL;
					else
						pip = &ip;
					if(!pip || ipaddr_aton(client->src.as_charptr, &ip))
					{
						res = udp_bind(sock_data->pcb, pip, client->dst.as_int);

						client->error = res;
						if(res==ERR_OK)
						{
							client->mode1 = TCPHS_ESTABLISHED;
							return RES_SIG_OK;
						}
					}
				}
			}
		}
#endif
	}
	return RES_SIG_ERROR;
}

RES_CODE lwip_sock_bind_url(CSocket* client, struct netif *netif)
{
	RES_CODE res;

	//try as IP address
	res = lwip_sock_bind_adr(client, netif);
	if(res != RES_SIG_OK)
	{
		lwip_mode_t* sock_mode = (lwip_mode_t*)client->mode.as_voidptr;

		if(sock_mode->mode.sock_type == IP_SOCKET_TCP)
		{
			if(client->sock_id < LWIP_TCP_PCBS_CNT)
			{
				client->mode0 = 2;

				res = lwip_start_dns(client, netif);
			}
		} else
		{
#if LWIP_UDP_PCBS_CNT
			if(sock_mode->mode.sock_type == IP_SOCKET_UDP)
			{
				if(client->sock_id < LWIP_UDP_PCBS_CNT)
				{
					client->mode0 = 2;

					res = lwip_start_dns(client, netif);
				}
			}
#endif
		}

	}
	return res;
}

#endif

//--------------------   TCP ACCEPT   ----------------------------------------//
#ifdef LWIP_CMD_TCP_ACCEPT

RES_CODE lwip_sock_accept_sock(CSocket* client, struct tcp_pcb* newpcb)
{
	RES_CODE res = RES_SIG_ERROR;
	CSocket* sock;

	sock = (CSocket*)client->dst.as_voidptr;
	if(sock && newpcb && sock->sock_id == SOCKET_ID_INVALID)
	{
		for(uint32_t i=0; i <LWIP_TCP_PCBS_CNT; i++)
		{
			if(g_lwip_socks[i].pcb == NULL)
			{
				g_lwip_socks[i].pcb = newpcb;
				g_lwip_socks[i].recv_pos = 0;
				sock->sock_id = i;
				sock->mode1 = TCPHS_ESTABLISHED;
				lwip_tcp_setup(sock, newpcb);
				return RES_SIG_OK;
			}
		}
		if(tcp_close(newpcb) != ERR_OK)
		{
			tcp_abort(newpcb);
		}
		res = RES_SIG_OUT_OF_MEMORY;
	}
	return res;
}

/** Function prototype for tcp accept callback functions. Called when a new
 * connection can be accepted on a listening pcb.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param newpcb The new connection pcb
 * @param err An error code if there has been an error accepting.
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 */
err_t lwip_cbf_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	CSocket* client = (CSocket*)arg;

	TRACELN_LWIP("LWIP accept %x", client);
	if(client == NULL)
		return (ERR_VAL);

	if( newpcb && (client->mode1 & TCPHS_OP_LISTEN ))
	{
		//client listens...
		if(client->res & FLG_BUSY)
		{
			//Get from the driver
			if(locked_clr_byte(&client->mode1, TCPHS_OP_ACCEPTING))
			{
				tsk_HND_SET_STATUS(client, lwip_sock_accept_sock(client, newpcb));
	    		return (ERR_OK);
			}
		}

		if(client->sock_id < LWIP_TCP_PCBS_CNT)
		{
			lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];

			newpcb->callback_arg = NULL;
			newpcb->recv = lwip_cbf_recv;
			newpcb->errf = lwip_cbf_err;
			if(sock_data->accept_que.push(newpcb))
			{
	    		return (ERR_OK);
			}
		}
	}
    return (ERR_MEM);

}

RES_CODE lwip_sock_accept(CSocket* client, struct netif *netif)
{
	RES_CODE res = RES_SIG_ERROR;
	struct tcp_pcb* newpcb;

	if(client->sock_id < LWIP_TCP_PCBS_CNT)
	{
		lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];

		if(sock_data->pcb && (client->mode1 == TCPHS_LISTEN) )
		{
			//if we have waiting connection return it now
			if(sock_data->accept_que.pop(newpcb))
			{
				res = lwip_sock_accept_sock(client, newpcb);
			} else
			{
				//Do nothing -> leave the client in BUSY state
				//the accept callback will send series of signals

				//Give to the driver
				client->mode1 = TCPHS_LISTEN | TCPHS_OP_ACCEPTING;
				res = RES_IDLE;
			}
		}

	}
	return res;
}

#endif

//--------------------   TCP LISTEN   ----------------------------------------//
#ifdef LWIP_CMD_TCP_LISTEN
RES_CODE lwip_sock_listen(CSocket* client, struct netif *netif)
{
	RES_CODE res = RES_SIG_ERROR;
	struct tcp_pcb *pcb;

	if(client->sock_id < LWIP_TCP_PCBS_CNT)
	{
		lwip_sock_data_t* sock_data = &g_lwip_socks[client->sock_id];

		if(sock_data->pcb && (client->mode1 == TCPHS_BIND) )
		{
			pcb = tcp_listen(sock_data->pcb);
			if(pcb)
			{
				sock_data->pcb = pcb;
				client->mode1 = TCPHS_LISTEN;
				pcb->callback_arg = client;
				pcb->accept = lwip_cbf_accept;
				res = RES_SIG_OK;
			}
		}
	}
	return res;
}
#endif

//--------------------   DELETE   --------------------------------------------//
#ifdef LWIP_CMD_TCP_DELETE
RES_CODE lwip_api_tcp_delete(CSocket* client, struct netif *netif)
{
	RES_CODE res;

	api_tcp_drain(client);

	if(client->mode.as_int && (client->mode1 & TCPHS_OP_PCB))
	{
		if((client->mode1 & (TCPHS_OP_WRITING | TCPHS_OP_CONNECTING)))
		{
			/* @to do TCP: abort running write/connect? */
			res = RES_SIG_IDLE;
		} else
		{
			res = api_close_internal(client, LWIP_SHUT_RDWR);
		}
	} else
	{
		//nothing to delete
		res = RES_SIG_OK;
	}
	return (res);
}

RES_CODE CSocket::lwip_tcp_delete()
{
	if(complete())
	{
		set_res_cmd(LWIP_CMD_TCP_DELETE);
		tsk_start_and_wait();
	}
	return (res);
}
#endif



RES_CODE lwip_process_cmd(CSocket* client, struct netif *netif)
{
	RES_CODE res = RES_SIG_ERROR;

	switch (client->cmd)
	{
	case SOCK_CMD_OPEN:
		res = lwip_sock_open(client, netif);
		break;
//#if USE_LWIP_LISTEN
	case SOCK_CMD_BIND_ADR:
		res =  lwip_sock_bind_adr(client, netif);
		break;

	case SOCK_CMD_BIND_URL:
		res =  lwip_sock_bind_url(client, netif);
		break;

	case SOCK_CMD_LISTEN:
		res = lwip_sock_listen(client, netif);
		break;

	case SOCK_CMD_ACCEPT:
		res = lwip_sock_accept(client, netif);
		break;

//#endif
	case SOCK_CMD_GET_ADDR:
		res = lwip_sock_get_adr(client, netif);
		break;

	case SOCK_CMD_CONNECT_ADR:
		res = lwip_sock_connect_adr(client, netif);
		break;

	case SOCK_CMD_CONNECT_URL:
		res = lwip_sock_connect_url(client, netif);
		break;

	case SOCK_CMD_CLOSE:
		res = lwip_sock_close(client, netif);
		break;

	case SOCK_CMD_GET_HOST:
		res = lwip_sock_get_host(client, netif);
		break;

	default:
		// socket commands
		break;
	}

	//	if(RES_IDLE != res)
	//		hnd_error(client);
	return res;
}
