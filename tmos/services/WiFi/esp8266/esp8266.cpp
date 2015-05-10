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
			gsm_send_cmd(cmd.c_str(), 4);
			return;
		}
	}

	if(get_socket_state(sid))
	{
		gsm_send_cmd(cmd.c_str(), 4);
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
			if( wifi_send_cmd(at_cmd.c_str(),10) == GSM_CMD_STATE_ROK )
			{
				if(tmos_sscanf(buf, "+USOCR:%u", &sid))
				{
					if(sid < GPRS_LEON_MAX_SOCKETS)
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

						TRACE_GPRS("\r\nGPRS:%s sock open %d.", sock->client.task->name, sid);
						return NET_OK;
					}
				}
			}
		}
	}
	//try to close some module sockets which are no longer used
	for(sid=0; sid < GPRS_LEON_MAX_SOCKETS; sid++)
	{
		if (alloc_sockets[sid] )
		{
			gprs_module_socket_close(sid);
		}

	}

	TRACE_GPRS_ERROR("\r\nGPRS:%s create socket ERROR", sock->client.task->name);
	return gsm_net_error(NET_ERR_SOCK_CREATE);

}

