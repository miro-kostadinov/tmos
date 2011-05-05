/*
 * lwip_api.cpp
 *
 *  Created on: 29.04.2011
 *      Author: Miroslav Kostadinov
 */


#include <tmos.h>
#include <lwip_api.h>
#include <lwip/tcp_impl.h>

//--------------------   TCP NEW   -------------------------------------------//
#ifdef LWIP_CMD_TCP_NEW
RES_CODE lwip_api_tcp_new(tcp_handle* client, struct netif *netif)
{
	RES_CODE res;
	struct tcp_pcb *pcb;

	//release the old one?
	pcb = (struct tcp_pcb *)client->mode.as_voidptr;
	if(pcb)
	{
		res =  RES_SIG_ERROR;
	} else
	{
		pcb = tcp_alloc(client->len);

		if(pcb)
		{
			lwip_tcp_setup(client, pcb);
			client->mode1 = TCPHS_NEW;
			client->mode.as_voidptr = pcb;
			res = RES_SIG_OK;
		} else
		{
			res = RES_SIG_OUT_OF_MEMORY;
		}
	}
	return res;
}

RES_CODE tcp_handle::lwip_tcp_new(unsigned int priority)
{
	if(complete())
	{
		len = priority;
		set_res_cmd(LWIP_CMD_TCP_NEW);
		tsk_start_and_wait();
	}
	return res;
}
#endif


//--------------------   TCP BIND   ------------------------------------------//
#ifdef LWIP_CMD_TCP_BIND

RES_CODE lwip_api_tcp_bind(tcp_handle* client, struct netif *netif)
{
	struct tcp_pcb *pcb;

	//release the old one?
	pcb = (struct tcp_pcb *)client->mode.as_voidptr;
	if(pcb && (client->mode1 == TCPHS_NEW) )
	{
		err_t res;

		res = tcp_bind(pcb, (ip_addr_t *) client->src.as_voidptr, client->dst.as_int);
		client->error = res;
		if(res==ERR_OK)
		{
			client->mode1 = TCPHS_BIND;
			return RES_SIG_OK;
		}
	}
	return RES_SIG_ERROR;
}

RES_CODE tcp_handle::lwip_tcp_bind(ip_addr_t *ipaddr, u16_t port)
{
	if(complete())
	{
		src.as_voidptr = ipaddr;
		dst.as_int = port;
		set_res_cmd(LWIP_CMD_TCP_BIND);
		tsk_start_and_wait();
	}
	return res;
}
#endif

//--------------------   TCP ACCEPT   ----------------------------------------//
#ifdef LWIP_CMD_TCP_ACCEPT

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
	tcp_handle* client = (tcp_handle*)arg;

	if(client == NULL)
		return ERR_VAL;

	if( client->mode1 == TCPHS_LISTEN )
	{
		//client listens...
		if(client->accept_que.push(newpcb))
		{
			//send signal
			if(client->res & FLG_BUSY)
				tsk_HND_SET_STATUS(client, RES_SIG_OK);
    		return ERR_OK;
		}
	}
    return ERR_MEM;

}

RES_CODE lwip_api_tcp_accept(tcp_handle* client, struct netif *netif)
{
	struct tcp_pcb *pcb;

	pcb = (struct tcp_pcb *)client->mode.as_voidptr;
	if(pcb && (client->mode1 == TCPHS_LISTEN) )
	{
		if(client->accept_que.empty())
		{
			//Do nothing -> leave the client in BUSY state
			//the accept callback will send series of signals
			return 0;
		}
		return RES_SIG_OK;
	}
	return RES_SIG_ERROR;

}

RES_CODE tcp_handle::lwip_tcp_accept(struct tcp_pcb*& newpcb)
{
	while(1)
	{
		//if we have waiting connection return it now
		if(accept_que.pop(newpcb))
			return RES_OK;

		if(complete())
		{
			set_res_cmd(LWIP_CMD_TCP_ACCEPT);
			tsk_start_and_wait();
		}
		if(res != RES_OK)
			return res;
	}
}
#endif

//--------------------   TCP LISTEN   ----------------------------------------//
#ifdef LWIP_CMD_TCP_LISTEN
RES_CODE lwip_api_tcp_listen(tcp_handle* client, struct netif *netif)
{
	RES_CODE res;
	struct tcp_pcb *pcb;

	//release the old one?
	res = RES_SIG_ERROR;
	pcb = (struct tcp_pcb *)client->mode.as_voidptr;
	if(pcb && (client->mode1 == TCPHS_BIND) )
	{
		pcb = tcp_listen(pcb);
		if(pcb)
		{
			client->mode.as_voidptr = pcb;
			client->mode1 = TCPHS_LISTEN;
			pcb->callback_arg = client;
			pcb->accept = lwip_cbf_accept;
			res = RES_SIG_OK;
		}
	}
	return res;
}

RES_CODE tcp_handle::lwip_tcp_listen()
{
	if(complete())
	{
		set_res_cmd(LWIP_CMD_TCP_LISTEN);
		tsk_start_and_wait();
	}
	return res;
}
#endif

//--------------------   CONNECT   -------------------------------------------//
#ifdef LWIP_CMD_TCP_CONNECT
/**
 * TCP callback function if a connection (opened by tcp_connect/do_connect) has
 * been established (or reset by the remote host).
 *
 * Function prototype for tcp connected callback functions. Called when a pcb
 * is connected to the remote side after initiating a connection attempt by
 * calling tcp_connect().
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb which is connected
 * @param err An unused error code, always ERR_OK currently ;-) TO DO!
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 *
 * @note When a connection attempt fails, the error callback is currently called!
 */
err_t lwip_cbf_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
	tcp_handle* client = (tcp_handle*)arg;

	if(client == NULL)
		return ERR_VAL;

	if(client->mode1 == TCPHS_CONECTING)
	{
		client->mode1 = TCPHS_ESTABLISHED;
		if(client->res & FLG_BUSY)
			tsk_HND_SET_STATUS(client, RES_SIG_OK);

	}

	return ERR_OK;
}

RES_CODE lwip_api_tcp_connect(tcp_handle* client, struct netif *netif)
{
	struct tcp_pcb *pcb;

	pcb = (struct tcp_pcb *)client->mode.as_voidptr;
	if(pcb && (client->mode1 == TCPHS_NEW) )
	{
		client->error = tcp_connect(pcb, (ip_addr_t *)client->dst.as_voidptr,
				client->src.as_int, lwip_cbf_connected);
		if(client->error == ERR_OK)
		{
			client->mode1 = TCPHS_CONECTING;
			return 0;
		}
	}
	return RES_SIG_ERROR;
}

RES_CODE tcp_handle::lwip_tcp_connect(ip_addr_t *addr, u16_t port)
{
	if(complete())
	{
		dst.as_voidptr = addr;
		src.as_int = port;
		set_res_cmd(LWIP_CMD_TCP_CONNECT);
		tsk_start_and_wait();
	}
	return res;
}
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
	tcp_handle* client = (tcp_handle*)arg;

	if(client == NULL)
		return ERR_VAL;

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
	return ERR_OK;
}

RES_CODE lwip_api_write(tcp_handle* client, struct netif *netif)
{
	RES_CODE res = RES_SIG_ERROR;

	//remember the start position
	client->dst.as_voidptr = client->src.as_voidptr;

	if(client->mode1 == TCPHS_ESTABLISHED)
	{
		struct tcp_pcb *pcb;

		pcb = (struct tcp_pcb *)client->mode.as_voidptr;
		if(pcb)
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
					client->mode1 = TCPHS_WRITING;

					client->src.as_byteptr += len;
					client->len -= len;
					if( !(client->cmd & FLAG_LOCK) && !client->len)
						tcp_output(pcb);
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

	return res;
}

//--------------------   READ   ---------------------------------------------//
/**
 * Receive callback function for TCP netconns.
 *
 * @see tcp.h (struct tcp_pcb.recv) for parameters and return value
 */
err_t lwip_cbf_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	tcp_handle* client = (tcp_handle*)arg;

	if(client == NULL)
		return ERR_VAL;

	client->error = err;

	if(client->recv_que.push(p))
	{
		//send signal
		if((client->mode1 == TCPHS_READING))
		{
			client->mode1 = TCPHS_ESTABLISHED;
			RES_CODE res = lwip_api_read(client);

			if(res & FLG_SIGNALED)
			{
				tsk_HND_SET_STATUS(client, res);
			}
		}

	} else
	{
		return ERR_MEM;
	}


	return ERR_OK;
}

RES_CODE lwip_api_read(tcp_handle* client)
{
	RES_CODE res = 0;
	char* src;
	struct pbuf *p;

	if(client->mode1 == TCPHS_ESTABLISHED)
	{
		while( !client->recv_que.empty() )
		{
			p = client->recv_que.top();
			if(p)
			{
				unsigned int to_read;

				src = (char*)p->payload;
				if(src && p->tot_len)
				{
					src+= client->recv_pos;

					to_read = p->tot_len - client->recv_pos;
					if( to_read > client->len )
						to_read = client->len;

					memcpy(client->dst.as_voidptr, src, to_read);
					client->dst.as_byteptr += to_read;
					client->len -= to_read;
					client->recv_pos += to_read;
					tcp_recved((struct tcp_pcb *)client->mode.as_voidptr, to_read);

					if(client->recv_pos == p->tot_len)
					{
						pbuf_free(p);
						client->recv_que.pop(p);
						client->recv_pos =0;
					}

					res = RES_SIG_OK;

					if(client->len)
						continue;
				}
			}

			if(!res)
				client->recv_que.pop(p);

			return RES_SIG_OK;
		}
	}

	if(!res)
		client->mode1 = TCPHS_READING;

	return res;
}

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
	tcp_handle* client = (tcp_handle*)arg;

	TRACELN("TCP ERR %x for %x", err, client);
	if(client)
	{
		//send signal
		client->mode1 = TCPHS_UNKNOWN;
		client->mode.as_voidptr = NULL;

		if (client->res & FLG_BUSY)
			tsk_HND_SET_STATUS(client, RES_SIG_ERROR);
	}
}

void lwip_tcp_setup(tcp_handle* client, struct tcp_pcb *pcb)
{
	pcb->callback_arg = client;
	pcb->recv = lwip_cbf_recv;
	pcb->sent = lwip_cbf_sent;
	pcb->errf = lwip_cbf_err;
}

extern "C" const LWIP_API_FUNC lwip_api_functions[MAX_LWIPCALLBACK+1] =
{

#ifdef LWIP_CMD_TCP_NEW
	lwip_api_tcp_new,
#endif

#ifdef LWIP_CMD_TCP_BIND
	lwip_api_tcp_bind,
#endif

#ifdef LWIP_CMD_TCP_LISTEN
	lwip_api_tcp_listen,
#endif

#ifdef LWIP_CMD_TCP_ACCEPT
	lwip_api_tcp_accept,
#endif

#ifdef LWIP_CMD_TCP_CONNECT
	lwip_api_tcp_connect,
#endif

	NULL
};
