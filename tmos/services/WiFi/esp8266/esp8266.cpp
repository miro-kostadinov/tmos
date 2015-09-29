/*
 * esp8266.cpp
 *
 *  Created on: 05.05.2015
 *      Author: Stanly
 */

#include <tmos.h>
#include <wifi_drv.h>
#include <esp8266.h>

WEAK_C void wifi_on_pwron(wifi_module_type* mod)
{
	//TODO: Trace various info
	mod->wifi_watchdog_cnt = WIFI_WDT_PERIOD;
}

RES_CODE esp8266_module::wifi_echo_off(bool lowlevel, uint32_t indx)
{
	RES_CODE res;

	res = RES_ERROR;
	for(int i=0; i < 3; i++)
	{
		rcv_hnd.close();
		snd_hnd.close();
		if(lowlevel)
			return RES_OK;
		tsk_sleep(50);
		if(rcv_hnd.tsk_open(drv_info->iface_driver_index, drv_info->iface_mode_stru[indx]) &&
			snd_hnd.tsk_open(drv_info->iface_driver_index, drv_info->iface_mode_stru[indx])	)
		{
			//wait until rx signals get stable
			tsk_sleep(50);

			//turn off echo
			if(wifi_send_cmd("E0", 1) & WIFI_CMD_STATE_OK)
			{
				res = RES_OK;
				break;
			}
		}
	}
	return res;
}

RES_CODE esp8266_module::wifi_drv_pwron(bool lowlevel)
{
	RES_CODE res;
	WIFI_DRIVER_DATA * drv_data = drv_info->drv_data;
	bool changed = false;

	if(drv_data->wifi_flags_bad & WIFI_FLAG_SHUTDOWN)
		return wifi_error(NET_ERR_WIFI_SHUTDOWN);
	if( drv_data->wifi_flags_ok & WIFI_FLAG_ON)
		return RES_OK;


	TRACE1_WIFI_DEBUG("\r\nWIFI power");
	drv_data->wifi_flags_ok = WIFI_STATE_OFF;

	do
	{
		res = wifi_echo_off(lowlevel, 0);
		if(lowlevel)
			return res;
		if(res != RES_OK)
		{
			res = wifi_echo_off(lowlevel, 1);
			if(res == RES_OK && !changed)
			{
				//try to switch baudrate
				CSTRING cmd;

				cmd.format("+CIOBAUD=%u", *(const uint32_t*)drv_info->iface_mode_stru[0]);
				if(wifi_send_cmd(cmd.c_str(), 2) & WIFI_CMD_STATE_OK)
					changed = true;
			}
		}
		else
			break;
	} while(changed);


	if(res == RES_OK)
	{
		// configure WiFi module
		wifi_send_cmd("+CWQAP", 3);
		// 1. Enable the module to act as “Station”
		res = wifi_send_cmd("+CWMODE=1", 5);
		if(WIFI_CMD_STATE_OK == res)
		{
		// 2.Enable multiple connections or not (1-multiple/ 0- Single
		//	Note: 	This mode can only be changed after all connections are
		//	 		disconnected. If server is started, reboot is required.
			res = wifi_send_cmd("+CIPMUX=1", 50);
			if(WIFI_CMD_STATE_OK == res)
			{
				//Reset (reset MT with resetting the SIM)
				drv_data->wifi_flags_ok = WIFI_FLAG_ON;
				drv_data->wifi_flags_bad &= ~WIFI_FLAG_ON;
				wifi_on_pwron(this);
				return NET_OK;
			}
		}
	}
	drv_data->wifi_error = NET_ERR_WIFI_ON;
	drv_data->wifi_flags_bad |= WIFI_FLAG_ON;
    return wifi_error(NET_ERR_WIFI);
}

RES_CODE esp8266_module::wifi_drv_off()
{
	NET_CODE res;
	WIFI_DRIVER_DATA * drv_data = drv_info->drv_data;

	wifi_drv_pwron();

	res = wifi_on_deregister(this);
	if(res != NET_OK)
		return res;

	drv_data->wifi_flags_ok &= ~(WIFI_FLAG_ON | WIFI_FLAG_REGISTERED );

    wifi_cancelation(true);

	//TODO: try to stop
	//check power state

	drv_data->signal_level = 0;
    TRACE1_WIFI_DEBUG("\r\nWIFI off");
	return RES_OK;
}


NET_CODE esp8266_module::wifi_reset(bool force)
{
    return NET_OK;
}

NET_CODE esp8266_module::wifi_drv_level()
{
	WIFI_DRIVER_DATA * drv_data = drv_info->drv_data;
	unsigned level =0;
	RES_CODE res = NET_ERR_WIFI;

	if( (drv_data->wifi_flags_ok & WIFI_FLAG_ON ) )
	{
		//TODO: get signal strength
		res = RES_OK;
	}
	drv_data->signal_level = level;
	return res;
}

int esp8266_module::wifi_notification(const char* row)
{
	if(cmd_submatch("+CTZV:", row))
	{
//		wifi_on_time(this);
		return 1;
	}
	if(cmd_submatch("Linked", row))
	{
		if(pending_socket && pending_socket->sock_state == SOCKET_OPEN)
			pending_socket->sock_state = SOCKET_CONECTED;
		return 1;
	}
	if(cmd_submatch("Unlink", row))
	{
		if(pending_socket && pending_socket->sock_state == SOCKET_CONECTED)
			pending_socket->sock_state = SOCKET_OPEN;
		return 1;
	}
	return 0;
}

void esp8266_module::wifi_process_tout(void)
{
	CSocket* sock;
	HANDLE list;
	RES_CODE res;

	if(wifi_tout)
	{
		if(!--wifi_tout)
		{
			if(!used_sockets)
			{
				//--- deactivate the context?
				for(int i=0; i < 3; i++ )
				{
					if(wifi_send_cmd("+CWQAP", 150) & WIFI_CMD_STATE_OK)
					{
						wifi_send_cmd("+CWJAP?", 50);
						break;
					}
				}
				wifi_on_disconnect(this);

				TRACELN1("WIFI OFF");

				list = waiting_open;
				waiting_open = NULL;
				while(list)
				{
					sock = (CSocket*)list;
					list = sock->next;
					sock->res = RES_BUSY;
					res = wifi_sock_open(sock);
					if(res & FLG_SIGNALED)
						usr_HND_SET_STATUS(sock, res);
				}
				if(!used_sockets)
					wifi_drv_off();
			}
		}
	}
}

void esp8266_module::wifi_cancelation(bool all)
{
	CSocket* sock;

	//cancel open waiting
	sock = (CSocket*)waiting_open;
	while(sock)
	{
		if(sock->mode1 || all)
		{
			sock->list_remove(waiting_open);
			usr_HND_SET_STATUS(sock, RES_SIG_IDLE);
			sock = (CSocket*)waiting_open;
		} else
		{
			sock = (CSocket*)sock->next;
		}
	}

	//cancel reading sockets
	for(int sid=0; sid < WIFI_ESP8266_MAX_SOCKETS; sid++)
	{
		sock = alloc_sockets[sid];
		if (  sock && (all || sock->mode1) && ((sock->res & RES_BUSY_WAITING) == RES_BUSY_WAITING))
		{
			usr_HND_SET_STATUS(sock, (sock->res & FLG_OK)| FLG_SIGNALED);
		}

	}

}

static const char* message = {"POST /RemoteMeasuringSystem/new_measurement.php HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.tmos-arm.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nContent-Type: application/x-www-form-urlencoded\nContent-Length:65\nConnection: Keep-Alive\n\r\nid=7&pass=123456&type=temp&value=66.6&delayed=0&meas_interval=10"};
//const char* message = "7";

NET_CODE esp8266_module::wifi_esp8266_init_net(CSocket * sock)
{
	NET_CODE res;
	CSTRING cmd;
	wifi_AP_t AP;
	uint32_t sig = 0;
	bool found;

	res = wifi_send_cmd("+CIFSR", 50);

	for(int i =0; i < 5; i++)
	{
		found = false;
		cmd_state |= WIFI_CMD_STATE_ROW_STOP;
		res = wifi_send_cmd("+CWLAP", 15);
		cmd_state &= ~WIFI_CMD_STATE_ROW_STOP;

		do
		{
			if(sig)
			{
				process_input(sig, "+CWLAP");
				res = cmd_state;
			}

			if (res >= WIFI_CMD_STATE_OK)
				break;

			if(res & WIFI_CMD_STATE_RETURNED)
			{
				cmd_state = res & ~WIFI_CMD_STATE_RETURNED;
				if(!found)
				{
					res = wifi_on_get_AP(this, sock, &AP);
					if(res == RES_OK)
						found = true;
				}
				row_start = 0;
				row_end = 0;
			}

			sig = tsk_resume_wait_signal(rcv_hnd.signal);
		} while(sig);

		if(cmd_state & WIFI_CMD_STATE_OK)
		{
			if(!found)
				res = NET_ERR_WIFI_NET_NAME;
			else
				res = NET_OK;
			break;
		}
	}
	if (res != NET_OK)
		return wifi_error(res);

	// If the wifi is already connected to a network different than the one
	// requested - return NET_IDLE - unavailable
	if (NET_OK == wifi_get_network_name(cmd))
	{
		if (0 != strcmp(cmd.c_str(), AP.name.c_str()))
		{
			return NET_IDLE;
		}
	}

	cmd.format("+CWJAP=\"%s\",\"%s\"", AP.name.c_str(), AP.pass.c_str());
	for(int i=0; i < 3; i++)
	{
		res = wifi_send_cmd(cmd.c_str(), 50);
		if (WIFI_CMD_STATE_OK == res)
		{
			res = wifi_send_cmd("+CIFSR", 50);
			if (WIFI_CMD_STATE_ROK == res)
			{
				connected_network_name = AP.name;
				return NET_OK;
			}
			break;
		}
	}
	return NET_ERR_WIFI_REGISTER;

	if (WIFI_CMD_STATE_OK == res)
	{
		res = wifi_send_cmd("+CIFSR", 50);
		if (WIFI_CMD_STATE_ROK == res)
		{
			connected_network_name = AP.name;
			wifi_send_cmd("+CIPSTART=\"TCP\",\"www.tmos-arm.com\",80", 50);
			//wifi_send_cmd("+CIPSTART=\"TCP\",\"192.168.147.100\",6112", 50);
			cmd.clear();
			cmd.append("+CIPSEND=");
			cmd.appendf("%d", strlen(message) + 1);
		    wifi_send_cmd(cmd.c_str(), 200);

			// make sure the handle is working if it is open
		    while(rcv_hnd.res < FLG_BUSY)
		    {
				process_input(0, NULL);
		    }

			wifi_sleep(20); //(the recommended value is at least 20 ms)

		    // make sure no URC is coming and the buf is empty
		    if( cmd_state & WIFI_CMD_STATE_STARTED)
		    {
		        if (tsk_wait_signal(rcv_hnd.signal, 8192))
		        {
		    		do
		    		{
		    			process_input(rcv_hnd.signal, NULL);

		    		} while ( (cmd_state & WIFI_CMD_STATE_STARTED) &&
		    				tsk_resume_wait_signal(rcv_hnd.signal) );
		        }
		    }
		    //start clean
		    cmd_state &= (WIFI_CMD_STATE_ROW_STOP | WIFI_CMD_STATE_HND);
		   	row_start = row_end = 0;

		    snd_hnd.tsk_write(message, strlen(message) + 1, WIFI_WRITE_TOT);
			return NET_OK;
		}
	}
	return wifi_net_error(res);
}

NET_CODE esp8266_module::wifi_get_network_name(CSTRING& name)
{
	if (true != connected_network_name.empty())
	{
		name = connected_network_name;
		return NET_OK;
	}
	return NET_IDLE;
}

void esp8266_module::wifi_esp8266_socket_close(unsigned int sid)
{
	CSTRING cmd;

	if(get_socket_state(sid))
	{
		cmd.format("+CIPCLOSE=%u", sid);
		if(wifi_send_cmd(cmd.c_str(), 45) & WIFI_CMD_STATE_OK)
		{
			if( (sid < WIFI_ESP8266_MAX_SOCKETS) && alloc_sockets[sid])
			{
				pending_socket = alloc_sockets[sid];
				for(int i=0; i< 500; i++)
				{
					wifi_sleep(10);
					if(pending_socket->sock_state == SOCKET_OPEN)
					{
						TRACE_WIFI("\r\nUnlinked %d", sid);
						break;
					}
				}
				pending_socket = NULL;
			}
		}
	}
}

NET_CODE esp8266_module::wifi_esp8266_socket_open(CSocket* sock)
{
	unsigned int sid;
	sock_mode_t* mode;

	RES_CODE res;
	const char* cmd = "+CIPSTATUS";
	unsigned int sig=0;
	CSTRING msg_list("\r\n");
	uint32_t index=1;

	cmd_state |= WIFI_CMD_STATE_ROW_STOP;
	res = wifi_send_cmd(cmd, 15);
	cmd_state &= ~WIFI_CMD_STATE_ROW_STOP;

	do
	{
		if(sig)
		{
			process_input(sig, cmd);
			res = cmd_state;
		}

		if (res >= WIFI_CMD_STATE_OK)
			break;

		if(res & WIFI_CMD_STATE_RETURNED)
		{
			cmd_state = res & ~WIFI_CMD_STATE_RETURNED;
			msg_list.appendf("%u:%s\r\n", index++, buf);
			row_start = 0;
			row_end = 0;
		}

		sig = tsk_resume_wait_signal(rcv_hnd.signal);
	} while(sig);

//	if(cmd_state & WIFI_CMD_STATE_OK)
//		TRACE1(msg_list.c_str());

	mode = (sock_mode_t*)sock->mode.as_voidptr;
	if(mode )
	{
		if (  (mode->sock_type == IP_SOCKET_UDP && mode->port)
			||(mode->sock_type == IP_SOCKET_TCP) )
		{
			for(sid=0; sid < WIFI_ESP8266_MAX_SOCKETS; sid++)
			{
				if(NULL == alloc_sockets[sid])
				{
					sock->sock_id = sid;
					sock->sock_state = SOCKET_OPEN;
					alloc_sockets[sid] = sock;
#if USE_GPRS_LISTEN
					if (mode->sock_type == IP_SOCKET_UDP)
						listen_ports[sid] = mode->port;
					accept_id[sid] = GPRS_LEON_MAX_SOCKETS;
#endif
					used_sockets++;
//					msg_list.format("\r\nWIFI:%s sock open %d.", sock->client.task->name, sid);
//					TRACE1(msg_list.c_str());
					return NET_OK;
				}
			}
		}
	}
	TRACE_WIFI_ERROR("\r\nWIFI:%s create socket ERROR", sock->client.task->name);
	return wifi_net_error(NET_ERR_SOCK_CREATE);

}

RES_CODE esp8266_module::wifi_sock_open(CSocket* sock) // NET_ERROR OK
{
	if(sock->sock_state == SOCKET_CLOSED)
	{
		//---  check the registration

		if( wifi_drv_on() == NET_OK )
		{
			switch(wifi_esp8266_init_net(sock))
			{
			case NET_IDLE:
				sock->next = waiting_open;
				waiting_open = sock;
				sock->res = RES_BUSY_WAITING;
				return RES_IDLE;

			case NET_OK:
				//--- open new socket
				if( NET_OK == wifi_esp8266_socket_open(sock))
					return RES_SIG_OK;
				break;
			}

		}
	} else
		wifi_net_error(NET_ERR_SOCK_NOT_CLOSED);

	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_sock_close(CSocket* sock) // NET_ERROR OK
{
	unsigned int sid;

	sid = sock->sock_id;
	if( (sock->sock_state & SOCKET_OPEN) && (sid < WIFI_ESP8266_MAX_SOCKETS) )
	{
		wifi_esp8266_socket_close(sid);
		wifi_driver_socket_close(sid, NET_OK);
		return RES_SIG_OK;
	}
	wifi_net_error(NET_ERR_SOCK_CLOSED);
	return RES_SIG_ERROR;

}

unsigned int esp8266_module::get_socket_state(unsigned int sock_id)
{
	RES_CODE res;
	const char* cmd = "+CIPSTATUS";
	unsigned int sig=0;
	uint32_t index;
	bool found = false;

	cmd_state |= WIFI_CMD_STATE_ROW_STOP;
	res = wifi_send_cmd(cmd, 15);
	cmd_state &= ~WIFI_CMD_STATE_ROW_STOP;

	do
	{
		if(sig)
		{
			process_input(sig, cmd);
			res = cmd_state;
		}

		if (res >= WIFI_CMD_STATE_OK)
			break;

		if(res & WIFI_CMD_STATE_RETURNED)
		{
			cmd_state = res & ~WIFI_CMD_STATE_RETURNED;
			if(tmos_sscanf(buf, "+CIPSTATUS:%u", &index))
			{
				if(index == sock_id)
					found = true;
			}
			row_start = 0;
			row_end = 0;
		}

		sig = tsk_resume_wait_signal(rcv_hnd.signal);
	} while(sig);

	if((cmd_state & WIFI_CMD_STATE_OK) && found )
		return 1;
	return 0;
}

void esp8266_module::wifi_driver_socket_close(unsigned int sid, unsigned int reason)
{
	CSTRING cmd;
	CSocket* sock;

	sock = alloc_sockets[sid];
	if(sock)
	{
		if(received_data[sid])
		{
			delete received_data[sid];
			received_data[sid] = NULL;
			received_size[sid] = 0;
		}
		sock->sock_id = SOCKET_ID_INVALID;
		if(sock->sock_state & SOCKET_OPEN)
		{
			sock->sock_state = SOCKET_CLOSED;

			// do we have to signal it?
			if((sock->res & RES_BUSY_WAITING) == RES_BUSY_WAITING)
			{
				//How to signal it?
				sock->error = reason;
				TRACE_WIFI("\r\ndisconnect %d", sid);
				tsk_HND_SET_STATUS(sock, (sock->res & FLG_OK)| FLG_SIGNALED);
			}
			else
			{
				TRACE_WIFI("\r\ndisconnect %d, err %d", sid, reason);
				wifi_net_error(reason);
			}

		}
		alloc_sockets[sid] = NULL;
#if USE_GPRS_LISTEN
		listen_ports[sid] = 0;
		accept_id[sid] = GPRS_LEON_MAX_SOCKETS;
#endif

		if(!--used_sockets)
		{
			if(waiting_open)
				wifi_tout = 1;
			else
			 if(drv_info->drv_data->wifi_clients > 2)
					wifi_tout = 5;
			 else
					wifi_tout = 1;
		}
	}
}

RES_CODE esp8266_module::wifi_receive_check(char sym)
{
	char ch;

	if( rcv_hnd.tsk_read_pkt(&ch, 1, GPRS_READ_TOT) & RES_OK)
	{
		if(ch == sym)
			return RES_OK;
		else
			return RES_EOF;
	}
	return rcv_hnd.res;
}

void esp8266_module::wifi_data_received(const char* row)
{
	uint32_t id, size, len;
	if(2 == tmos_sscanf(row,"+IPD,%u,%u", &id, &size))
	{
		if(id < WIFI_ESP8266_MAX_SOCKETS)
		{
			CSocket* sock = alloc_sockets[id];
			if(sock && sock->sock_state == SOCKET_CONECTED)
			{
				if (sock && ((sock->res & RES_BUSY_WAITING) == RES_BUSY_WAITING))
				{
					len = min(size, sock->len);
					if( rcv_hnd.tsk_read_pkt(sock->dst.as_charptr, len, GPRS_READ_TOT) == RES_OK)
					{
						len -= rcv_hnd.len;
						sock->len -= len;
						sock->dst.as_ccharptr += len;
						tsk_HND_SET_STATUS(sock, RES_SIG_OK);
					}
					else
					{
						len = size;
						tsk_HND_SET_STATUS(sock, RES_SIG_ERROR);
					}
					size -= len;
				}
				if(size)
				{
					received_data[id] = (char*)tsk_realloc(received_data[id], received_size[id]+size);
					if(received_data[id])
					{
						if( rcv_hnd.tsk_read_pkt(received_data[id]+received_size[id], size, GPRS_READ_TOT) == RES_OK)
						{
							received_size[id] += size;
						}

					} else
					{
						//ops out of memory..
						received_size[id] = 0;
					}
				}

			}

			if(wifi_receive_check('\r') == RES_OK)
				if(wifi_receive_check('\n') == RES_OK)
				{
					while(1)
					{
						switch(wifi_receive_check('\r'))
						{
						case RES_OK:
							wifi_receive_check('\n');
							return;
						case RES_EOF:
							break;		// received "OK or something else"
						default:
							return; 	//error
						}
					}
				}
			//flush:
			// if read \r
			// if read \n
			// while !read \r\n
			//
		}
	}
}

RES_CODE esp8266_module::process_read(CSocket* sock)
{
	unsigned size, id;
	CSTRING cmd;

    TRACELN("WIFI: read %d?", sock->len);
	while( sock->sock_state == SOCKET_CONECTED)
	{
		if(!sock->len)
		{
			TRACE1(" done!");
			return (sock->res & FLG_OK)| FLG_SIGNALED;
		}

		if(!received_size[sock->sock_id])
		{
			if(!(sock->res & FLG_OK))
			{
				// ако няма нотификация го слага в списъка с чакащи
				TRACE1(" wait!");
				sock->res |= RES_BUSY_WAITING;
				return RES_IDLE;
			}
			return  RES_SIG_OK;
		}

		if(!received_data[sock->sock_id])
			break;
		id = sock->sock_id;
		size = sock->len;
		if(size > received_size[id])
			size = received_size[id];
		memcpy(sock->dst.as_voidptr, received_data[id], size);

		sock->dst.as_byteptr += size;
		sock->len -= size;
		if(received_size[id] > size)
		{
			memcpy(received_data[id], received_data[id] + size, received_size[id] - size);
			received_size[id] -= size;
		}
		else
		{
			received_size[id] = 0;
			delete received_data[id];
			received_data[id] = NULL;
			return RES_SIG_OK;
		}
	}
	wifi_sleep(120);
	wifi_net_error(NET_ERR_SOCK_READ);
	TRACE_ERROR("\r\nWIFI:%s read INVALID", sock->client.task->name);
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::process_write(CSocket* sock)
{
	unsigned size;//, newsize, id;
//	unsigned int start_size, write_size, sock_state;
	CSTRING cmd;
	unsigned char snd_pending;


	while(1)
	{
		size = sock->len;
		if(!size)
		{
		    TRACELN1("WIFI: write OK");
			return RES_SIG_OK;
		}
		if(size >1022)
			size = 1022;

		if(sock->sock_state != SOCKET_CONECTED)
			break;

		// Send command
		cmd.format("+CIPSEND=%u,%u", sock->sock_id, size);
	    TRACELN("WIFI: WRITE %d?", size);
	    cmd_state |= WIFI_CMD_STATE_HND;
	    if(wifi_send_cmd(cmd.c_str(), 20) != WIFI_CMD_STATE_RETURNED)
	    {
	    	wifi_net_error(NET_ERR_SOCK_WRITE);
	    	return RES_SIG_ERROR;
	    }


	    // Send data
		snd_pending = '>';
		do
		{
			process_input(rcv_hnd.signal, cmd.c_str(), snd_pending);
			if ( cmd_state >= WIFI_CMD_STATE_HND )
			{
				if ( cmd_state & WIFI_CMD_STATE_HND )
				{
					unsigned int mytime;

					cmd_state &= ~WIFI_CMD_STATE_HND;
					snd_pending = 0;

					rcv_hnd.tsk_start_read(&received_ch, 1);
					mytime = CURRENT_TASK->time;
					tsk_sleep(55); // data sheet recomendation
					if( snd_hnd.tsk_write(sock->src.as_voidptr, size, WIFI_WRITE_TOT) != RES_OK)
						break;
					CURRENT_TASK->time = mytime;

				} else
				{
					if ( cmd_state >= WIFI_CMD_STATE_OK )
						break; // command completed with OK, ERROR ..
				}
			}

		} while(tsk_resume_wait_signal(rcv_hnd.signal));

//	    wifi_on_blink_transfer(this, GPRS_TRANSFER_INDICATOR);

	    //Check the result
	    if(cmd_state & WIFI_CMD_STATE_OK)
	    {
			TRACE1(" done!");
			sock->src.as_byteptr += size;
			sock->len -= size;
			continue;
		}

	    if (cmd_state & WIFI_CMD_STATE_CMES)
	    {
	    	TRACE_ERROR("\r\nWIFI:%s write ERROR", sock->client.task->name);
	    }

    	break;
	}
	wifi_sleep(120);
	wifi_net_error(NET_ERR_SOCK_WRITE);
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_sock_connect_adr(CSocket* sock)
{
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_sock_connect_url(CSocket* sock)
{
	CSTRING cmd;;

	for(int try_cnt=0; sock && try_cnt <3; try_cnt++ )
	{
		if(sock->sock_state & SOCKET_OPEN)
		{
			unsigned int sid = sock->sock_id;
			cmd.format("+CIPSTART=%u,\"%s\",\"%s\",%u", sid,
					(((sock_mode_t*)sock->mode.as_voidptr)->sock_type == IP_SOCKET_TCP)?"TCP":"UDP",
					sock->src.as_charptr,
					sock->dst.as_int);
			if (wifi_send_cmd(cmd.c_str(), 40) & WIFI_CMD_STATE_OK)
			{
				pending_socket = sock;
				for(int i=0; i<500; i++)
				{
					wifi_sleep(10);
					if(sock->sock_state == SOCKET_CONECTED)
						break;
				}
				pending_socket = NULL;
				received_size[sock->sock_id] = 0;
				if(sock->sock_state == SOCKET_CONECTED)
				{
					TRACE_WIFI("\r\nLink %d", sid);
					return RES_SIG_OK;
				}
				wifi_esp8266_socket_close(sid);
			}

			//Debug
			get_socket_state(sid);
		} else
		{
			wifi_net_error(NET_ERR_SOCK_CLOSED);
			return RES_SIG_ERROR;
		}
	}
	wifi_net_error(NET_ERR_SOCK_CONNECT);
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_sock_disconect(CSocket* sock)
{
	uint32_t sid;
	if(sock && sock->sock_id < WIFI_ESP8266_MAX_SOCKETS)
	{
		sid =sock->sock_id;
		if(alloc_sockets[sid] == sock && sock->sock_state == SOCKET_CONECTED)
		{
			wifi_esp8266_socket_close(sid);
			if(!get_socket_state(sid))
				return RES_SIG_OK;
		}
	}
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_gethostbyname(CSocket* sock)
{
	return RES_OK;
}



