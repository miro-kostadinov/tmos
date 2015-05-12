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

RES_CODE esp8266_module::wifi_drv_pwron(bool lowlevel)
{
	RES_CODE res;

	if(drv_info->drv_data->wifi_flags_bad & WIFI_FLAG_SHUTDOWN)
		return wifi_error(NET_ERR_WIFI_SHUTDOWN);
	if( drv_info->drv_data->wifi_flags_ok & WIFI_FLAG_ON)
		return RES_OK;

	WIFI_DRIVER_DATA * drv_data = drv_info->drv_data;
	res = RES_ERROR;

	TRACE1_WIFI_DEBUG("\r\nWIFI power");
	drv_data->wifi_flags_ok = WIFI_STATE_OFF;

	for(int i=0; i < 3; i++)
	{
		rcv_hnd.close();
		snd_hnd.close();
		if(lowlevel)
			return RES_OK;
		tsk_sleep(50);
		if(rcv_hnd.tsk_open(drv_info->iface_driver_index, drv_info->iface_mode_stru) &&
			snd_hnd.tsk_open(drv_info->iface_driver_index, drv_info->iface_mode_stru)	)
		{
			//wait until rx signals get stable
			tsk_sleep(50);

			//turn off echo
			if(wifi_send_cmd("E0", 1) & WIFI_CMD_STATE_OK)
			{
				drv_data->wifi_flags_ok = WIFI_FLAG_ON;
				drv_data->wifi_flags_bad &= ~WIFI_FLAG_ON;
				res = RES_OK;
				break;
			}
		}
	}
	if(res != RES_OK)
	{
		drv_data->wifi_error = NET_ERR_WIFI_ON;
		drv_data->wifi_flags_bad |= WIFI_FLAG_ON;
	} else
	{
		//Reset (reset MT with resetting the SIM)
		drv_info->drv_data->wifi_flags_ok &= ~(WIFI_FLAG_REGISTERED);
		wifi_on_pwron(this);
		return res;
	}
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
	return 0;
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

NET_CODE esp8266_module::wifi_esp8266_init_net(CSocket * sock)
{
	NET_CODE res;
//	unsigned int start = CURRENT_TIME;
	CSTRING cmd;
	wifi_AP_t AP;

	res = wifi_on_get_AP(this, sock, &AP);
	if (res != NET_OK)
		return wifi_error(res);

/*

	do
	{
		res = NET_ERR_WIFI;

		if(wifi_send_cmd("+CWJAP?", 2) == WIFI_CMD_STATE_ROK) //Check WiFi attach status
		{
			if(buf[8] == '0')
			{
				// Activate GPRS
				gsm_send_cmd("+CGATT=1", 20);
				res = NET_ERR_GPRS_SERVCE;
			}
			else
			if(buf[8] == '1')
			{
				bool check;

				//check APN name & APN username
				check = (AP.name == get_str_cmd("+UPSD=0,1", 5))
						&& (AP.user == get_str_cmd("+UPSD=0,2", 5));
				if (used_sockets)
				{
					if (check)
						return NET_OK;

					return NET_IDLE;
				}

				if (check)
				{
					// PSD profile active?
					if (gsm_send_cmd("+UPSND=0,8", 10) == GSM_CMD_STATE_ROK)
						if(buf[12]=='1')
							return NET_OK;	//GPRS attached, profile is active...

				} else
				{
					res = NET_ERR_GPRS_ACTIVATE;
					// PSD profile active?
					if (gsm_send_cmd("+UPSND=0,8", 10) != GSM_CMD_STATE_ROK)
						break;

					//Deactivate context
					if(buf[12]=='1')
					{
						process_apn_counters(this);
						if (!(gsm_send_cmd("+UPSDA=0,4", 150) & GSM_CMD_STATE_OK))
							break;
					}

					// Reset the profile
					// required because the module does not accept empty strings
					if (!(gsm_send_cmd("+UPSDA=0,0", 5) & GSM_CMD_STATE_OK))
						break;

					// configure APN name
					if (!apn_stru.name.empty())
					{
						cmd.format("+UPSD=0,1,\"%s\"", apn_stru.name.c_str());
						if (!(gsm_send_cmd(cmd.c_str(), 5) & GSM_CMD_STATE_OK))
							break;
					}

					// configure APN username
					if (!apn_stru.user.empty())
					{
						cmd.format("+UPSD=0,2,\"%s\"", apn_stru.user.c_str());
						if (!(gsm_send_cmd(cmd.c_str(), 5) & GSM_CMD_STATE_OK))
							break;
					}

					// configure APN password
					if (!apn_stru.pass.empty())
					{
						cmd.format("+UPSD=0,3,\"%s\"", apn_stru.pass.c_str());
						if (!(gsm_send_cmd(cmd.c_str(), 5) & GSM_CMD_STATE_OK))
							break;
					}

					//Debug info - Dynamic IP and other parameters
					gsm_send_cmd("+UPSD=0", 15);
				}

				// activates a PDP context
				if (gsm_send_cmd("+UPSDA=0,3", 175) & GSM_CMD_STATE_OK)
				{
#if GSM_APN_COUNTERS
					if(last_apn_indx)
						process_apn_counters(this);
					apn_indx = apn_stru.indx & APN_MASK;
#endif
					TRACELN1_GPRS("GPRS: ON");
					return NET_OK;
				}

			}
		}
		tsk_sleep(250);
	} while(seconds_since(start) < 30);
*/

	return wifi_net_error(res);
}

void esp8266_module::wifi_esp8266_socket_close(unsigned int sid)
{
	CSTRING cmd;
	sock_mode_t* mode;
	CSocket* sock;

	cmd.format("+USOCL=%u", sid );
	sock = alloc_sockets[sid];
	if(sock)
	{
		mode = (sock_mode_t*)sock->mode.as_voidptr;
		if (mode && mode->sock_type == IP_SOCKET_UDP)
		{
			wifi_send_cmd(cmd.c_str(), 4);
			return;
		}
	}

	if(get_socket_state(sid))
	{
		wifi_send_cmd(cmd.c_str(), 4);
	}
}

NET_CODE esp8266_module::wifi_esp8266_socket_open(CSocket* sock)
{
	unsigned int sid;
	CSTRING at_cmd("+USOCR=");
	sock_mode_t* mode;

	mode = (sock_mode_t*)sock->mode.as_voidptr;
	if(mode )
	{
		if (mode->sock_type == IP_SOCKET_UDP && mode->port)
		{
			at_cmd.appendf("17,%d", mode->port);
		}
		else
			if (mode->sock_type == IP_SOCKET_TCP)
			{
				at_cmd += "6";
			}
			else
				at_cmd.clear();
		for(int retry=0; retry <2 && !at_cmd.empty(); retry++)
		{
			// try to get a socket
			if( wifi_send_cmd(at_cmd.c_str(),10) == WIFI_CMD_STATE_ROK )
			{
				if(tmos_sscanf(buf, "+USOCR:%u", &sid))
				{
					if(sid < WIFI_ESP8266_MAX_SOCKETS)
					{
						wifi_driver_socket_close(sid, NET_ERR_SOCK_ABORT);

						sock->sock_id = sid;
						sock->sock_state = SOCKET_OPEN;
						alloc_sockets[sid] = sock;
#if USE_GPRS_LISTEN
						if (mode->sock_type == IP_SOCKET_UDP)
							listen_ports[sid] = mode->port;
						accept_id[sid] = GPRS_LEON_MAX_SOCKETS;
#endif
						used_sockets++;

						TRACE_WIFI("\r\nWIFI:%s sock open %d.", sock->client.task->name, sid);
						return NET_OK;
					}
				}
			}
		}
	}
	//try to close some module sockets which are no longer used
	for(sid=0; sid < WIFI_ESP8266_MAX_SOCKETS; sid++)
	{
		if (alloc_sockets[sid] )
		{
			wifi_esp8266_socket_close(sid);
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
	return 0;
}

void esp8266_module::wifi_driver_socket_close(unsigned int sid, unsigned int reason)
{

}

RES_CODE esp8266_module::process_read(CSocket* sock)
{
	return RES_OK;
}

RES_CODE esp8266_module::process_write(CSocket* sock)
{
	return RES_OK;
}

RES_CODE esp8266_module::wifi_sock_connect_adr(CSocket* sock)
{
	return RES_OK;
}

RES_CODE esp8266_module::wifi_sock_connect_url(CSocket* sock)
{
	return RES_OK;
}

RES_CODE esp8266_module::wifi_gethostbyname(CSocket* sock)
{
	return RES_OK;
}



