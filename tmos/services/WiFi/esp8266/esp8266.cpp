/*
 * esp8266.cpp
 *
 *  Created on: 05.05.2015
 *      Author: Stanly
 */

#include <tmos.h>
#include <hardware_cpp.h>
#include <wifi_drv.h>
#include <esp8266.h>

#define SOCKET_WAIT_ACCEPT	(0x10 + SOCKET_LISTEN)

WEAK_C void wifi_on_pwron(wifi_module_type* mod)
{
	//TODO: Trace various info
	mod->wifi_watchdog_cnt = WIFI_WDT_PERIOD;
}

WEAK bool wifi_name_pass(CSTRING& name)
{
	name += "\"Eltrade ECR\"";
	return false;
}

bool wifi_get_param(const char*row, CSTRING& param, unsigned int num)
{
	const char* start=row;
	const char* end;

	if(row && &param)
	{
		param.clear();
		if(*start == '+')
		{
			start++;
			while(*start && !IS_SPACE(*start) && *start != ':')
				start++;
			while(*start && (IS_SPACE(*start)||*start == ':'))
				start++;
 		}
		while(num > 1)
		{
			start=strchr(start,',');
			if(!start)
				return false;
			start++;
			num--;
		}
		if(*start)
		{
			if(*start == ',' || IS_SPACE(*start))
			{
				start++;
				while(*start && IS_SPACE(*start))
					start++;
			}
			if(*start == '"')
			{
				end = strchr(++start, '"');
				if(!end)
					return false;
				param.assign(start, end-start);
				return true;
			}
			end = strchr(start, ',');
			if(end)
				param.assign(start, end-start);
			else
				param=start;
			return true;
		}

	}
	return false;
}

bool wifi_get_param(const char*row, unsigned int& param, unsigned int num)
{
	CSTRING str;
	if(wifi_get_param(row, str, num))
	{
		if(tmos_sscanf(str.c_str(), "%u", &param) == 1)
			return true;
	}
	return false;
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
	if(wifi_pin_on)
		PIO_Cfg(wifi_pin_on);
	if(wifi_pin_boot)
		PIO_Cfg(wifi_pin_boot);
	PIO_Deassert(wifi_pin_on);
	tsk_sleep(1000);
	PIO_Assert(wifi_pin_on);
	tsk_sleep(50);
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

				cmd.format(WIFI_BAUD"=%u", *(const uint32_t*)drv_info->iface_mode_stru[0]);
#if !USE_DEPRECATED_AT_CMD
				cmd += ", 8, 1, 0, 0";
#endif
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
		wifi_send_cmd(WIFI_QUIT_AP, 3);
		// 1. Enable the module to act as “Station”
		// 2. AP mode
		// 3  AP + Station mode
		wifi_send_cmd(WIFI_SET_AP_IP"=\"192.168.0.101\"", 5);
		res = wifi_send_cmd(WIFI_MODE"=3", 5);
		if(WIFI_CMD_STATE_OK == res)
		{
		// 2.Enable multiple connections or not (1-multiple/ 0- Single
		//	Note: 	This mode can only be changed after all connections are
		//	 		disconnected. If server is started, reboot is required.
			res = wifi_send_cmd(WFI_MUX"=1", 50);
			if(WIFI_CMD_STATE_OK == res)
			{
				drv_data->wifi_flags_ok = WIFI_FLAG_ON;
				drv_data->wifi_flags_bad &= ~WIFI_FLAG_ON;
				// configure AP
				CSTRING ssid(WIFI_CFG_AP"=");
				if(wifi_name_pass(ssid))
					ssid += ",6,3";
				else
					ssid += "\"\",6,0";
				res = wifi_send_cmd(ssid.c_str(), 5);
				if(WIFI_CMD_STATE_OK == res)
				{
					wifi_send_cmd(WIFI_CFG_AP"?", 5);
					wifi_on_pwron(this);
					return NET_OK;
				}
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
	if(IS_DIGIT(row[0]) && row[1] == ',')
	{
		if(cmd_submatch("CONNECT", row+2))
		{
			notify_state = WIFI_NOTIFY_LINK;
			tsk_send_signal(CURRENT_TASK, WIFI_NOTIFY_SIGNAL);
			return 1;
		}
		if(cmd_submatch("CLOSED", row+2))
		{
			notify_state = WIFI_NOTIFY_UNLINK;
			tsk_send_signal(CURRENT_TASK, WIFI_NOTIFY_SIGNAL);
			return 1;
		}
	}
	if(cmd_submatch("Linked", row))
	{
		notify_state = WIFI_NOTIFY_LINKED;
		tsk_send_signal(CURRENT_TASK, WIFI_NOTIFY_SIGNAL);
//		if(pending_socket && pending_socket->sock_state == SOCKET_OPEN)
//			pending_socket->sock_state = SOCKET_CONECTED;
		return 1;
	}
	if(cmd_submatch("Link", row))
	{
		notify_state = WIFI_NOTIFY_LINK;
		tsk_send_signal(CURRENT_TASK, WIFI_NOTIFY_SIGNAL);
//		if(pending_socket && pending_socket->sock_state == SOCKET_OPEN)
//			pending_socket->sock_state = SOCKET_CONECTED;
		return 1;
	}
	if(cmd_submatch("Unlink", row))
	{
		notify_state = WIFI_NOTIFY_UNLINK;
//		if(pending_socket && pending_socket->sock_state == SOCKET_CONECTED)
//			pending_socket->sock_state = SOCKET_OPEN;
		tsk_send_signal(CURRENT_TASK, WIFI_NOTIFY_SIGNAL);
		return 1;
	}
	return 0;
}

void esp8266_module::wifi_notificatoin_response()
{
	switch(notify_state)
	{
	case WIFI_NOTIFY_IDLE:
		break;
	case WIFI_NOTIFY_LINK:
#if USE_WIFI_LISTEN
		if(listen_socket)
		{
			get_socket_state(10);
		}
#endif
		break;
	case WIFI_NOTIFY_LINKED:
		if(pending_socket && pending_socket->sock_state == SOCKET_OPEN)
			pending_socket->sock_state = SOCKET_CONECTED;
		break;
	case WIFI_NOTIFY_UNLINK:
		if(pending_socket && pending_socket->sock_state == SOCKET_CONECTED)
			pending_socket->sock_state = SOCKET_OPEN;
#if USE_WIFI_LISTEN
		if(listen_socket)
		{
			get_socket_state(10);
		}
#endif
		break;
	}
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
					if(wifi_send_cmd(WIFI_QUIT_AP, 150) & WIFI_CMD_STATE_OK)
					{
						wifi_send_cmd(WIFI_JOIN_TO_AP"?", 50);
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
#if USE_WIFI_LISTEN
	if(listen_socket && (all || listen_socket->mode1) && ((listen_socket->res & RES_BUSY_WAITING) == RES_BUSY_WAITING))
	{
		usr_HND_SET_STATUS(listen_socket, (listen_socket->res & FLG_OK)| FLG_SIGNALED);
		listen_socket = nullptr;
		listen_port = 0;
	}
#endif
}

//static const char* message = {"POST /RemoteMeasuringSystem/new_measurement.php HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.tmos-arm.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nContent-Type: application/x-www-form-urlencoded\nContent-Length:65\nConnection: Keep-Alive\n\r\nid=7&pass=123456&type=temp&value=66.6&delayed=0&meas_interval=10"};
//const char* message = "7";

NET_CODE esp8266_module::wifi_esp8266_init_net(CSocket * sock)
{
	NET_CODE res;
	CSTRING cmd;
	wifi_AP_t AP;
	uint32_t sig = 0;
	bool found;

	res = wifi_send_cmd(WIFI_GET_LOCAL_IP, 50);

	for(int i =0; i < 5; i++)
	{
		found = false;
		cmd_state |= WIFI_CMD_STATE_ROW_STOP;
		res = wifi_send_cmd(WIFI_LIST_AVAILABLE_AP, 15);
		cmd_state &= ~WIFI_CMD_STATE_ROW_STOP;

		do
		{
			if(sig)
			{
				process_input(sig, WIFI_LIST_AVAILABLE_AP);
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

	cmd.format(WIFI_JOIN_TO_AP"=\"%s\",\"%s\"", AP.name.c_str(), AP.pass.c_str());
	for(int i=0; i < 3; i++)
	{
		res = wifi_send_cmd(cmd.c_str(), 50);
		if (WIFI_CMD_STATE_OK == res)
		{
			res = wifi_send_cmd(WIFI_GET_LOCAL_IP, 50);
			if (WIFI_CMD_STATE_ROK == res)
			{
				connected_network_name = AP.name;
				return NET_OK;
			}
			break;
		}
	}
	return NET_ERR_WIFI_REGISTER;
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
		cmd.format(WIFI_DISCONNECT"=%u", sid);
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
						TRACE_WIFI_DEBUG("\r\nUnlinked %d", sid);
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
	const char* cmd = WIFI_CONNECTION_STAT;
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
#if USE_WIFI_LISTEN
			if (mode->interface == WIFI_SERVER_INTERFACE)
			{
				if(listen_socket)
				{
					return wifi_net_error(NET_ERR_SOCK_CREATE);
				}
				else
				{
					listen_socket = sock;
					listen_socket->sock_id = WIFI_ESP8266_MAX_SOCKETS;
					listen_socket->sock_state = SOCKET_OPEN;
					used_sockets++;
					return NET_OK;
				}
			}
#endif
			for(sid=0; sid < WIFI_ESP8266_MAX_SOCKETS; sid++)
			{
				if(NULL == alloc_sockets[sid])
				{
					sock->sock_id = sid;
					sock->sock_state = SOCKET_OPEN;
					alloc_sockets[sid] = sock;
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
		sock_mode_t *mode = (sock_mode_t*)sock->mode.as_voidptr;
		if( mode && wifi_drv_on() == NET_OK )
		{
			NET_CODE res = NET_ERROR;
			if(mode->interface == WIFI_SERVER_INTERFACE)
				res = NET_OK;
			else
				res = wifi_esp8266_init_net(sock);
			switch(res)
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
#if USE_WIFI_LISTEN
	if(listen_socket && sock == listen_socket)
	{
		listen_port = 0;
		listen_socket = nullptr;

		sock->sock_state = SOCKET_CLOSED;
		// do we have to signal it?
		if((sock->res & RES_BUSY_WAITING) == RES_BUSY_WAITING)
		{
			//How to signal it?
			sock->error = NET_OK;
			TRACE_WIFI_DEBUG("\r\ndisconnect %d", sid);
			tsk_HND_SET_STATUS(sock, (sock->res & FLG_OK)| FLG_SIGNALED);
		}
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
		return RES_SIG_OK;
	}
#endif
	if( (sock->sock_state & SOCKET_OPEN) && (sid < WIFI_ESP8266_MAX_SOCKETS) )
	{
		wifi_esp8266_socket_close(sid);
		wifi_driver_socket_close(sid, NET_OK);
		return RES_SIG_OK;
	}
	wifi_net_error(NET_ERR_SOCK_CLOSED);
	return RES_SIG_ERROR;

}

void esp8266_module::module_accepted_socket(unsigned int sock_id)
{
	if(listen_socket && (listen_socket->res & RES_BUSY_WAITING) == RES_BUSY_WAITING )
	{
		RES_CODE res = RES_SIG_ERROR;
		CSocket *new_socket = (CSocket *)(listen_socket->dst.as_voidptr);
		if(new_socket)
		{
			new_socket->sock_id = sock_id;
			new_socket->sock_state = SOCKET_CONECTED;
			if(alloc_sockets[sock_id])
				wifi_driver_socket_close(sock_id, NET_ERR_SOCK_DISCONNECT);
			alloc_sockets[sock_id] = new_socket;
			used_sockets++;
			res = RES_SIG_OK;
		}
		usr_HND_SET_STATUS(listen_socket, res);
	}
}

unsigned int esp8266_module::get_socket_state(unsigned int sock_id)
{
	RES_CODE res;
	const char* cmd = WIFI_CONNECTION_STAT;;
	unsigned int sig=0;
	uint32_t id;
	bool found = false;
	unsigned int status;
	unsigned int scaned=0;

	cmd_state |= WIFI_CMD_STATE_ROW_STOP;
	res = wifi_send_cmd(cmd, 15);
	if(res <=WIFI_CMD_STATE_RETURNED)
	{
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
				if(tmos_sscanf(buf, "STATUS:%u", &status))
				{
					if(status != 3) // Connected
					{
						TRACE_WIFI_DEBUG("\r\nMisssing connections!");
					}
				}

				if(tmos_sscanf(buf, WIFI_CONNECTION_STAT":%u", &id) && id < WIFI_ESP8266_MAX_SOCKETS)
				{
					// +CIPSTATUS:id,type,addr,port,tetype
					if(wifi_get_param(buf, status, WIFI_CONNECTION_TYPE_INDX))
					{
						scaned |= (1<<id);
						if(status == 1)
						{
							//1: ESP8266 runs as server
							if( accept_id[id] == id)
							{
								// already accepted
								if(!alloc_sockets[id]) // but not allocated
									module_accepted_socket(id);

							}
							else
							{
								if(accept_id[id] == WIFI_ESP8266_MAX_SOCKETS)
								{
									accept_id[id] = id;
									module_accepted_socket(id);
								}
								else
								{
									// mismatch between id and accepted
									if(alloc_sockets[id])
									{
										wifi_driver_socket_close(id, NET_ERR_SOCK_DISCONNECT);
									}
									accept_id[id] = id;
									module_accepted_socket(id);
								}
							}
						}
					}
					if(id == sock_id)
						found = true;

				}
				row_start = 0;
				row_end = 0;
			}

			sig = tsk_resume_wait_signal(rcv_hnd.signal);
		} while(sig);

	}
	cmd_state &= ~WIFI_CMD_STATE_ROW_STOP;
	if((cmd_state & WIFI_CMD_STATE_OK))
	{
		for(unsigned int i=0; i < WIFI_ESP8266_MAX_SOCKETS; i++)
		{
			if(scaned & (1<<i))
				continue;
			// missing socket
			if(alloc_sockets[i])
				wifi_driver_socket_close(i, NET_ERR_SOCK_DISCONNECT);
			accept_id[i] = WIFI_ESP8266_MAX_SOCKETS;
		}
		if(found)
			return 1;
	}
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
				TRACE_WIFI_DEBUG("\r\ndisconnect %d", sid);
				tsk_HND_SET_STATUS(sock, (sock->res & FLG_OK)| FLG_SIGNALED);
			}
			else
			{
				TRACE_WIFI_DEBUG("\r\ndisconnect %d, err %d", sid, reason);
				wifi_net_error(reason);
			}

		}
		alloc_sockets[sid] = NULL;
#if USE_WIFI_LISTEN
		accept_id[sid] = WIFI_ESP8266_MAX_SOCKETS;
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
		if(ch >= 0x20)
			TRACE_CHAR(ch);
		else
			TRACE("(%02u)", ch);
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
						TRACE_BUF(sock->dst.as_charptr, len, TC_TXT_BLUE);
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
				else
				{
					TRACELN1("Buffered");
				}
				if(size)
				{
					received_data[id] = (char*)tsk_realloc(received_data[id], received_size[id]+size);
					if(received_data[id])
					{
						if( rcv_hnd.tsk_read_pkt(received_data[id]+received_size[id], size, GPRS_READ_TOT) == RES_OK)
						{
							TRACE_BUF(received_data[id]+received_size[id], size, TC_TXT_CYAN);
							received_size[id] += size;
						}

					} else
					{
						//ops out of memory..
						received_size[id] = 0;
					}
				}

			}
			else
			{
				if(listen_socket)
				{
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
			}

#if USE_DEPRECATED_AT_CMD
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
#endif
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
			return RES_SIG_OK;
		}

		if(!received_size[sock->sock_id])
		{
			if(!(sock->res & FLG_OK))
			{
				// ако няма нотификация го слага в списъка с чакащи
				TRACE1(" wait!");
				sock->res = RES_BUSY_WAITING;
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
		sock->res |= FLG_OK;
		sock->dst.as_byteptr += size;
		sock->len -= size;
		if(received_size[id] > size)
		{
			memmove(received_data[id], received_data[id] + size, received_size[id] - size);
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
	return RES_FATAL|FLG_SIGNALED;
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
		cmd.format(WIFI_SEND_DATA"=%u,%u", sock->sock_id, size);
	    TRACELN("WIFI: WRITE %d?", size);
#if USE_DEPRECATED_AT_CMD
	    cmd_state |= WIFI_CMD_STATE_HND;
	    if(wifi_send_cmd(cmd.c_str(), 20) != WIFI_CMD_STATE_RETURNED)
#else
	    if(wifi_send_cmd(cmd.c_str(), 20) != WIFI_CMD_STATE_OK)
#endif
	    {
	    	wifi_net_error(NET_ERR_SOCK_WRITE);
	    	return RES_SIG_ERROR;
	    }

	    // Send data
		snd_pending = '>';
#if !USE_DEPRECATED_AT_CMD
		cmd_state = 0;
#endif
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
			cmd.format(WIFI_CONNECT"=%u,\"%s\",\"%s\",%u", sid,
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
					TRACE_WIFI_DEBUG("\r\nLink %d", sid);
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



RES_CODE esp8266_module::wifi_sock_bind_adr(CSocket* sock)
{
	if(listen_socket && sock == listen_socket)
	{
		listen_port = sock->dst.as_int;
		return RES_SIG_OK;
	}
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_sock_bind_url(CSocket* sock)
{
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_sock_listen(CSocket* sock)
{
	CSTRING cmd;
	if(listen_socket && sock == listen_socket)
	{
		cmd.format(WIFI_SERVER"=1,%u", listen_port);
		if(wifi_send_cmd(cmd.c_str(), 5) == WIFI_CMD_STATE_OK)
		{
			wifi_send_cmd(WIFI_SRVER_TIMEOUT"=10", 5);
			sock->sock_state = SOCKET_LISTEN;
			return RES_SIG_OK;
		}
	}
	else
		wifi_net_error(NET_ERR_SOCK_CONNECT);
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_sock_accept(CSocket* sock)
{
	if(listen_socket)
	{
		get_socket_state(10);
		if(listen_socket->res & FLG_SIGNALED)
			return listen_socket->res;
		listen_socket->res = RES_BUSY_WAITING;
		return RES_IDLE;
	}
	return RES_SIG_ERROR;
}

RES_CODE esp8266_module::wifi_sock_addr(CSocket* sock)
{
	return RES_SIG_ERROR;
}

#define WIFI_UPGRADE_BUF_SIZE		WIFI_BUF_SIZE
RES_CODE esp8266_module::module_upgrade(HANDLE hnd)
{
	char gsm_ack[32];
	unsigned int sig, old_sig, total, tm, last;
	HND_CLIENTS old_client;
	RES_CODE res;
	CSTRING log;

    if(!wifi_pin_on || !wifi_pin_boot)
    {
	    hnd->tsk_write("\r\n----- Invalid hardware -----\r\n");
    }

    //change hnd client
    old_sig = hnd->signal;
    old_client = hnd->client;
    sig = CURRENT_TASK->aloc_sig;
    sig = (sig+1) & ~sig;
    sig &= 255;
    if( sig )
    {
    	hnd->signal = sig;
	    hnd->client.task = CURRENT_TASK;

//		wifi_drv_off();

	    hnd->tsk_write("\r\n----- READY -----\r\n");

	    PIO_Cfg(wifi_pin_on);
	    PIO_Cfg(wifi_pin_boot);
	    tsk_sleep(50);
	    //gsm_drv_pwron(true);
	    PIO_Deassert(wifi_pin_on);
	    tsk_sleep(1000);
	    PIO_Assert(wifi_pin_boot);
	    tsk_sleep(200);
	    PIO_Assert(wifi_pin_on);
	    tsk_sleep(500);

//		res = hnd->tsk_read(buf, 1, 2*25000);
//		if(res == RES_OK)
		{
			// upgrade started
			rcv_hnd.close();
			snd_hnd.close();

			if(rcv_hnd.tsk_open(drv_info->iface_driver_index, drv_info->iface_mode_stru[0]) &&
				snd_hnd.tsk_open(drv_info->iface_driver_index, drv_info->iface_mode_stru[0])	)
			{
		    	unsigned int pc_rcv=0, gsm_rcv=0;

			    TRACE1("\r\n------ GSM UPGRADE --------");

//			    //flush PC
//			    hnd->tsk_read(buf, WIFI_UPGRADE_BUF_SIZE, 2);
//		    	pc_rcv =WIFI_UPGRADE_BUF_SIZE - hnd->len;
//		    	if(pc_rcv)
//		    	{
//		    		TRACE_BUF(buf, pc_rcv, TC_TXT_RED);
//		    	}

		    	last = 0;
		    	tm = CURRENT_TIME;
		    	total = 0;

			    while(1)
			    {

			    	//start reading from PC
			    	if(!(hnd->res & FLG_BUSY))
						if(!hnd->tsk_start_read(buf, WIFI_UPGRADE_BUF_SIZE))
							break;

			    	//start reading from GSM
			    	if(!(rcv_hnd.res & FLG_BUSY))
						if(!rcv_hnd.tsk_start_read(gsm_ack, sizeof(gsm_ack)))
								break;

			    	sig = tsk_wait_signal(hnd->signal | rcv_hnd.signal, 2*25000);

			    	//process PC
			    	if(sig & hnd->signal)
			    	{
			    		hnd->res &= ~FLG_SIGNALED;
				    	pc_rcv = WIFI_UPGRADE_BUF_SIZE - hnd->len;
			    		TRACELN(" pc%u:", pc_rcv);
			    		if(last == 0)
			    		{
			    			last =1;
			    			TRACELN1("");
			    			log.format("GSM -> PC %u bytes %u mS", total, CURRENT_TIME - tm);
			    			TRACE_BUF(log.c_str(), log.length(), TC_TXT_RED);
			    			TRACELN1("");
			    			tm = CURRENT_TIME;
			    			total = 0;
			    		}
			    		total += pc_rcv;
				    	if(pc_rcv)
				    	{
//				    		TRACE_BUF(buf, pc_rcv, TC_TXT_MAGENTA);
				    		if (snd_hnd.tsk_write(buf, pc_rcv, 10000) != RES_OK)
				    		{
				    			TRACELN1("TIMEOUT\r\n");
				    		}
				    	}
			    	}

			    	//process GSM
			    	if(sig & rcv_hnd.signal)
			    	{
			    		rcv_hnd.res &= ~FLG_SIGNALED;
				    	gsm_rcv = sizeof(gsm_ack) - rcv_hnd.len;
			    		TRACELN(" gsm%u:", gsm_rcv);
			    		if(last == 1)
			    		{
			    			last =0;
			    			TRACELN1("");
			    			log.format("PC -> GSM %u bytes %u mS", total, CURRENT_TIME - tm);
			    			TRACE_BUF(log.c_str(), log.length(), TC_TXT_RED);
			    			TRACELN1("");
			    			tm = CURRENT_TIME;
			    			total = 0;
			    		}
			    		total += gsm_rcv;
				    	if(gsm_rcv)
				    	{
//				    		TRACE_BUF(gsm_ack, gsm_rcv, TC_TXT_CYAN);

				    		hnd->tsk_cancel();
					    	pc_rcv = WIFI_UPGRADE_BUF_SIZE - hnd->len;
				    		TRACE(" pccan%u:", pc_rcv);
					    	if(pc_rcv)
					    	{
//					    		TRACE_BUF(buf, pc_rcv, TC_TXT_MAGENTA);
					    		if(snd_hnd.tsk_write(buf, pc_rcv, 10000) != RES_OK)
					    		{
					    			TRACELN1("TIMEOUT\r\n");
					    		}
					    	}

				    		hnd->tsk_write(gsm_ack, gsm_rcv);
				    	}
			    	}
			    	if(!sig)
			    	{
			    		hnd->tsk_cancel();
			    		rcv_hnd.tsk_cancel();
			    		break;
			    	}


			    }
			    if(last ==1)
	    			log.format("\r\nPC -> GSM %u bytes %u mS\r\n", total, CURRENT_TIME - tm);
			    else
	    			log.format("\r\nGSM -> PC %u bytes %u mS\r\n", total, CURRENT_TIME - tm);
    			TRACE_BUF(log.c_str(), log.length(), TC_TXT_RED);

			    TRACE1("\r\n------ UPGRADE END --------");
			}
		}// else
		//	TRACE1("\r\nTIMEOUT\r\n");

		PIO_Deassert(wifi_pin_boot);
		PIO_Deassert(wifi_pin_on);
	    wifi_drv_pwron();
	    //restore hnd owner
    	hnd->signal = old_sig;
	    hnd->client = old_client;
	} else
	{
		res = RES_ERROR;
	}

	return res;
}
