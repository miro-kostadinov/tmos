/*
 * tls_drv.cpp
 *
 *  Created on: Jun 6, 2017
 *      Author: miro
 */

#include <tmos.h>
#include <tls_drv.h>
#include <yarrow.h>
#include <tls_cache.h>

RES_CODE tls_module_t::tls_process_read()
{
	RES_CODE res;
	size_t received = 0;

	res = tls_read(client->dst.as_voidptr, client->len, &received, 0);

	client->len -= received;
	client->dst.as_byteptr += received;
	return res;
}

RES_CODE tls_module_t::tls_process_write()
{
	RES_CODE res=RES_OK;
	uint32_t len;

	//Check parameters
	if (client->src.as_cvoidptr == nullptr && client->len != 0)
		return RES_TLS_INVALID_PARAMETER;

	//Send as much data as possible
	while (client->len)
	{
		//Check current state
		if (tls_state == TLS_STATE_APPLICATION_DATA)
		{
			//The record length must not exceed 16384 bytes
			len = min(client->len, TLS_MAX_RECORD_LENGTH);
			last_txrc.msg_len = len;

			res = tls_record_get_lens(&last_txrc);
			if(res == RES_OK)
			{
				memcpy(last_txrc.msg_start(), client->src.as_cvoidptr, len);
				last_txrc.tls_record.rec_type = TLS_TYPE_APPLICATION_DATA;
				res = tls_record_write(&last_txrc);

				//Check status code
				if (res == RES_OK)
				{
					client->src.as_byteptr += len;
					client->len -= len;
				}
				else
				{
					//Send an alert message to the peer, if applicable
					tlsProcessError(res);
				}
			}
		}
		else
		{
			//The connection has not yet been established
			res = RES_TLS_NOT_CONNECTED;
		}

		if (res != RES_OK)
			break;
	}

	return res;
}

RES_CODE tls_module_t::tls_sock_open()
{
	RES_CODE res;
	const redir_sock_mode_t* mode;

	if(client->sock_state == SOCKET_CLOSED)
	{
		res = tls_init_context(this);
		if(res != RES_OK)
			return res;

		mode = (const redir_sock_mode_t*)client->mode.as_voidptr;
		if(mode->mode.sock_type & REDIRECTED_SOCKET)
		{
			res = target.open(mode->redir_mode);
			if(res == RES_OK)
				client->sock_state = SOCKET_OPEN;
			return res;
		}

	}
	return RES_ERROR;
}

RES_CODE tls_module_t::tls_sock_connect_adr()
{
	RES_CODE res = RES_ERROR;

	if(client->sock_state & SOCKET_OPEN)
	{
		if(target.complete())
		{
			target.src.as_int = client->src.as_int;
			target.dst.as_int = client->dst.as_int;
			target.len = client->len;
			target.set_res_cmd(SOCK_CMD_CONNECT_ADR);
		    target.tsk_start_handle();

		    if(tsk_wait_signal(target.signal | TLS_CANCEL_SIGNAL, TLS_MAX_TIMEOUT) & target.signal)
		    {
		        target.res &= ~FLG_SIGNALED;
		        res = target.res;
		        if(res == RES_OK)
		        {
		        	res = tls_connect(&target);
					if(res == RES_OK)
						client->sock_state = SOCKET_CONECTED;

		        } else
		        	res = target.error;
		    }
		    else
		    {
		    	target.tsk_cancel();
		        res = target.res;
		    }
		}
	} else
	{
		res = NET_ERR_SOCK_CLOSED;
	}
	return res;
}

RES_CODE tls_module_t::tls_sock_connect_url()
{
	RES_CODE res = RES_ERROR;

	if(client->sock_state & SOCKET_OPEN)
	{
		if(target.complete())
		{
			target.src.as_int = client->src.as_int;
			target.dst.as_int = client->dst.as_int;
			target.len = client->len;
			target.set_res_cmd(SOCK_CMD_CONNECT_URL);
		    target.tsk_start_handle();

		    if(tsk_wait_signal(target.signal | TLS_CANCEL_SIGNAL, TLS_MAX_TIMEOUT) & target.signal)
		    {
		        target.res &= ~FLG_SIGNALED;
		        res = target.res;
		        if(res == RES_OK)
		        {
		        	server_name = client->src.as_ccharptr;
		        	res = tls_connect(&target);
					if(res == RES_OK)
						client->sock_state = SOCKET_CONECTED;

		        } else
		        	res = target.error;
		    }
		    else
		    {
		    	target.tsk_cancel();
		        res = target.res;
		    }
		}
	} else
	{
		res = NET_ERR_SOCK_CLOSED;
	}
	return res;
}

RES_CODE tls_module_t::tls_sock_disconect()
{
	RES_CODE res;

	res = tls_shutdown();
	client->sock_state = SOCKET_CLOSED;

	return res;
}

RES_CODE tls_module_t::tls_sock_close()
{
	RES_CODE res;

	res = tls_shutdown();

	target.close();
	client->sock_state = SOCKET_CLOSED;
	return res;
}

WEAK void set_tls_prng(tls_module_t& module)
{
	RES_CODE res;
    prng_yarrow_algo_t* yarrow;

    yarrow = new prng_yarrow_algo_t;
    if(yarrow)
    {
		res = yarrow->prng_init();
		if (res == RES_OK)
		{
			uint8_t seed[32];

			//Properly seed the PRNG
#if 1
			memclr(seed, sizeof(seed));
#endif
			res = yarrow->prng_seed(seed, sizeof(seed));
			if (res == RES_OK)
				module.tlsSetPrng(yarrow);
		}
    }
}

//*----------------------------------------------------------------------------
//*			TLS Task
//*----------------------------------------------------------------------------

void tls_thread(TLS_DRIVER_INFO* drv_info, CSocket* client)
{
    unsigned int signals;
    RES_CODE res;
    tls_module_t module(client);


    ALLOCATE_SIGNAL(TLS_SIGNALS);

    set_tls_prng(module);

    while(true)
    {
		signals = tsk_get_signal(TLS_SIGNALS);
		if (signals & TLS_CLOSE_SIGNAL)
			break;

		if (signals & TLS_CLIENT_SIGNAL)
    	{
			res = RES_IDLE;

	    	if( !(signals & TLS_CANCEL_SIGNAL) )
			{
				if (client->cmd & FLAG_READ)
				{
					res = module.tls_process_read();
				}
				else
				{
					if (client->cmd & FLAG_WRITE)
					{
						res = module.tls_process_write();
					}
					else
					{
						switch (client->cmd)
						{
						case SOCK_CMD_OPEN:
							res = module.tls_sock_open();
							break;
#if USE_TLS_LISTEN
							case SOCK_CMD_BIND_ADR:
							res = module.tls_sock_bind_adr();
							break;

							case SOCK_CMD_BIND_URL:
							res = module.tls_sock_bind_url();
							break;

							case SOCK_CMD_LISTEN:
							res = module.tls_sock_listen();
							break;

							case SOCK_CMD_ACCEPT:
							res = module.tls_sock_accept();
							break;

							case SOCK_CMD_GET_ADDR:
							res = module.tls_sock_addr();
							break;
#endif
						case SOCK_CMD_CONNECT_ADR:
							res = module.tls_sock_connect_adr();
							break;

						case SOCK_CMD_CONNECT_URL:
							res = module.tls_sock_connect_url();
							break;

						case SOCK_CMD_DISCONNECT:
							res = module.tls_sock_disconect();
							break;

						case SOCK_CMD_CLOSE:
							res = module.tls_sock_close();
							break;

						default:
							// socket commands
							break;
						}
					}
				}
				client->error = res;
				if(res > RES_EOF)
					res = RES_ERROR;

			}

			tsk_HND_SET_STATUS(client, res | FLG_SIGNALED);
    	}
    }


}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void TLS_DCR(TLS_DRIVER_INFO * drv_info, unsigned int reason, HANDLE param)
{
    TLS_DRIVER_DATA * drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
            break;

	    case DCR_OPEN:
	    	if(drv_data->tls_clients < TLS_MAX_CONNECTIONS)
	    	{
	    		for(int i=0; i < TLS_MAX_CONNECTIONS; i++)
	    		{
	    			if(drv_data->tls_task[i] == nullptr)
	    			{
	    	    		drv_data->tls_task[i] = usr_task_create_dynamic("TLST", (TASK_FUNCTION)tls_thread,
	    						40,	500+TRACE_SIZE);
	    				if(drv_data->tls_task[i])
	    				{
	    					svc_task_schedule(drv_data->tls_task[i]);
	    					drv_data->tls_task[i]->sp->r0.as_cvoidptr = drv_info;
	    					drv_data->tls_task[i]->sp->r1.as_cvoidptr = param;
	    					param->mode0 = i;
	    			    	param->res = RES_OK;
	    			    	drv_data->tls_clients ++;
	    			    	break;
	    				}
	    			}
	    		}
	    	}
	    	break;

	    case DCR_CLOSE:
	    	drv_data->tls_clients --;
	    	if( param->mode0 < TLS_MAX_CONNECTIONS)
			{
				Task* task = drv_info->drv_data->tls_task[param->mode0];

				if (task)
				{
					drv_info->drv_data->tls_task[param->mode0] = nullptr;
					svc_send_signal(task, TLS_CLOSE_SIGNAL);
				}
			}
	    	break;

	    case DCR_CANCEL:
	    	if( (param->res & FLG_BUSY) && param->mode0 < TLS_MAX_CONNECTIONS)
			{
				Task* task = drv_info->drv_data->tls_task[param->mode0];

				if (task)
				{
					svc_send_signal(task, TLS_CANCEL_SIGNAL);
				}
			}
            break;

    }
}

//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void TLS_DSR(TLS_DRIVER_INFO * drv_info, HANDLE hnd)
{
	if(hnd->mode0 < TLS_MAX_CONNECTIONS)
	{
		Task* task = drv_info->drv_data->tls_task[hnd->mode0];

		if(task)
		{
			svc_send_signal(task, TLS_CLIENT_SIGNAL);
			return;
		}
	}
	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}






