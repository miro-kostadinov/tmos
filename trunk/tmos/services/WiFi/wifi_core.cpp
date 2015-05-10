/*
 * wifi_module.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <hardware_cpp.h>
#include <wifi_drv.h>
#include <wifi_core.h>
#include <csocket.h>



WEAK_C NET_CODE wifi_on_register(wifi_module_type* mod)
{
	return NET_OK;
}

WEAK_C NET_CODE wifi_on_deregister(wifi_module_type* mod)
{
	return NET_OK;
}

// CMEE*
// CMEE
bool wifi_module_type::cmd_match(const char* cmd, const char* row)
{
	bool res = false;
	while(*cmd && (*cmd == *row))
	{
		res = true;
		cmd++;
		row++;
	}
	return res && !IS_ALPHANUM(*cmd);
}

/**
 * Return truw if row is starting with the cmd
 * @param cmd
 * @param row
 * @return
 */
bool wifi_module_type::cmd_submatch(const char* cmd, const char* row)
{
	while(*row && (*cmd == *row))
    {
        cmd++;
        row++;
    }
    return (!*cmd);
}

void wifi_module_type::process_input(unsigned int signals, const char* cmd,
									unsigned char hnd_start)
{
	unsigned char ch;

	//if receiver is idle -> start
    if(rcv_hnd.res < FLG_SIGNALED)
    {
		rcv_hnd.tsk_start_read(&received_ch, 1);
    }

    // if there is no signal -> try to get it
	if(!(signals & rcv_hnd.signal))
	{
		signals = tsk_test_signal(rcv_hnd.signal);
		if(signals)
			tsk_get_signal(signals);
	}

	if(signals & rcv_hnd.signal)
	{
		rcv_hnd.res &= ~FLG_SIGNALED;

		ch = received_ch; //accept this symbol

		if(IS_ALPHANUM(ch) || IS_PUNC(ch)  || ch =='\r' || ch == '\n'|| ch == ' ')
		{

			if( ch < ' ')
			{ // \r \n ...
				if(cmd_state & WIFI_CMD_STATE_STARTED)
				{	// ��� ��������� �������, ���� �� ����
					cmd_state ^= WIFI_CMD_STATE_STARTED;
					buf[row_end++] = 0;
					TRACE1_WIFI_DEBUG(" | ");
					//debug_buffer(drv_data, '^');

					cmd_state |= wifi_process_row(cmd);
				}
				// process_row() changes row_start if the row must stay
				// otherwise the row will be dumped
				row_end = row_start;
			}
			else
			{
				buf[row_end++] = ch;
			    TRACE1_WIFI("\e[33m");
				TRACE_CHAR_WIFI(ch);
			    TRACE1_WIFI("\e[m");
				cmd_state |= WIFI_CMD_STATE_STARTED;
				if( hnd_start == ch )
				{
					cmd_state ^= WIFI_CMD_STATE_STARTED;
					buf[row_end] = 0;
					row_end--; // remove it // +SORD 4,123 "........." OK
					cmd_state |= WIFI_CMD_STATE_HND;
					TRACE1_WIFI("^hnd^");
					return;
				}
			}
			if(row_end >= WIFI_BUF_SIZE-1)
			{
				TRACE1_WIFI_ERROR("Dumping: ");
				TRACE1_WIFI_ERROR(buf);

				row_end = 0;
				row_start = 0;
				cmd_state = WIFI_CMD_STATE_FATAL;
			}
		} else
		{
			TRACE_WIFI_ERROR("!!!(%02.2x)\r\n", ch);
		}

		rcv_hnd.tsk_start_read(&received_ch, 1);
	}

}

WIFI_CMD_STATE wifi_module_type::wifi_process_row(const char *cmd)
{
	const char * row;

	row = buf + row_start;

	//------- command matching --------//
	if(cmd)
	{
		// OK
		if(cmd_submatch("OK", row))
			return WIFI_CMD_STATE_OK;

		// ERROR
		if(find_in_list(row, SZ(ERROR) SZ(+CME ERROR) SZ(+CMS ERROR), NULL ))
			return WIFI_CMD_STATE_CMES;

		if( cmd[0] && cmd_match(cmd, row))
		{
			//the row is response to the cmd
			row_start = row_end;
			return WIFI_CMD_STATE_RETURNED;
		}

		// check for echo
		if(row[0]=='A' && row[1]=='T' && !strcmp(cmd, row+2))
		{
			TRACE1_WIFI_DEBUG("\e[4;1;33m ");
			TRACE1_WIFI_DEBUG(row);
			TRACE1_WIFI_DEBUG(" \e[m");
		    return WIFI_CMD_STATE_UNK;
//			return 0;
		}
	}

	//------------ WIFI notification -------------//
	if(wifi_notification(row))
	{
		return 0;
	}


	//------- command related --------//
	if(cmd)
	{
		if( (buf[row_start] != '+') || (cmd_state & WIFI_CMD_STATE_ROW_STOP))
		{
			//the row is response to the cmd
			row_start = row_end;
			return WIFI_CMD_STATE_RETURNED;
		}

	}
	TRACE1_WIFI_DEBUG("^UNK[");
	TRACE1_WIFI_DEBUG(row);
	TRACE1_WIFI_DEBUG("]UNK^");
    return WIFI_CMD_STATE_UNK;
}

WIFI_CMD_STATE wifi_module_type::wifi_send_cmd(const char *cmd, unsigned int time)
{
	unsigned int sig;

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

    //trace
    TRACE1_WIFI_DEBUG("\r\n\e[32mAT");
    TRACE1_WIFI_DEBUG(cmd);
    TRACE1_WIFI_DEBUG("\e[m ");

    //start clean
    cmd_state &= (WIFI_CMD_STATE_ROW_STOP | WIFI_CMD_STATE_HND);
   	row_start = row_end = 0;

    if(snd_hnd.tsk_write("AT", 2, WIFI_WRITE_TOT) == RES_OK)
    {
        if(snd_hnd.tsk_write(cmd, strlen(cmd), WIFI_WRITE_TOT) == RES_OK)
        {
            if(snd_hnd.tsk_write("\r", 1, WIFI_WRITE_TOT) == RES_OK)
            {
        		sig = tsk_wait_signal( rcv_hnd.signal, time << 10);
        		if(sig && (cmd_state & WIFI_CMD_STATE_HND))
        		{
        			cmd_state = 0;
        			return WIFI_CMD_STATE_RETURNED;
        		}

            	while(sig)
            	{
            		process_input(sig, cmd); // NORMAL AT COMMAND
        			if ( cmd_state > WIFI_CMD_STATE_RETURN )
        			{
                    	row_start = row_end = 0;
        				return cmd_state & WIFI_CMD_SEND_STATUS;
        			}
        			sig = tsk_resume_wait_signal(rcv_hnd.signal);
            	}
            	row_start = row_end = 0;
            	if(wifi_watchdog_cnt)
            		wifi_watchdog_cnt--;
            }
        }
    }
    cmd_state = WIFI_CMD_STATE_FATAL;
	return cmd_state;
}

void wifi_module_type::wifi_sleep(unsigned int time)
{
    while(rcv_hnd.res < FLG_BUSY)
    {
		process_input(0, NULL);
    }
    if (tsk_wait_signal(rcv_hnd.signal, time))
    {
		do
		{
			process_input(rcv_hnd.signal, NULL); // UNSOLICITED

		} while (tsk_resume_wait_signal(rcv_hnd.signal));
    }
}

RES_CODE wifi_module_type::wifi_error(int err)
{
	drv_info->drv_data->wifi_error = err;
	return RES_ERROR;
}

NET_CODE wifi_module_type::wifi_net_error(int err)
{
	drv_info->drv_data->wifi_error = err;
	return err;
}

void wifi_module_type::hnd_error(HANDLE hnd)
{
	hnd->error = drv_info->drv_data->wifi_error;
}


NET_CODE wifi_module_type::wifi_get_network_name(CSTRING& name)
{
	if(wifi_send_cmd("+COPS=3,1", 50) & WIFI_CMD_STATE_OK) //short format alphanumeric <oper>
	{
//		char* ptr = get_str_cmd("+COPS?", 180);
		if(ptr)
		{
			name = ptr;
			return NET_OK;
		}
	}
	return wifi_net_error(NET_ERR_WIFI_NET_NAME);
}


NET_CODE wifi_module_type::wifi_drv_on()
{
	NET_CODE res;
	WIFI_DRIVER_DATA * drv_data = drv_info->drv_data;
    drv_data->turnoff_time = 0;

	if(drv_data->wifi_flags_ok & WIFI_FLAG_REGISTERED )
		return NET_OK;

	if( !(drv_data->wifi_flags_ok & WIFI_FLAG_ON))
	{
		if(RES_OK != wifi_drv_pwron())
			return wifi_net_error(NET_ERR_PHY_NOT_READY);

	}

#if WIFI_NETWORK_NAME
	drv_data->network_name = "registering";
#endif


	// ---- 2. Check Network registration
	res = wifi_check_reg();
	if(res != NET_OK)
	{
		drv_data->wifi_flags_bad |= WIFI_FLAG_REGISTERED;
		return res;
	}


#if WIFI_NETWORK_NAME
	res = wifi_get_network_name(drv_data->network_name);
	if(res != NET_OK)
		return res;
#endif


	// ---- 4. Notify the application and let it make additional checks
	res = wifi_on_register(this);
	if(res != NET_OK)
		return res;

	// ---- 5. WIFI module is ready now. Do some cosmetic


	TRACE1_WIFI_DEBUG("\r\nWIFI on");
	drv_data->wifi_flags_ok |= WIFI_FLAG_REGISTERED;
	drv_data->wifi_flags_bad &= ~WIFI_FLAG_REGISTERED;
	return NET_OK;
}



NET_CODE wifi_module_type::wifi_check_reg() // NET_ERROR: OK
{
	NET_CODE res;

#if WIFI_NETWORK_NAME
	drv_data->network_name = "searching";
#endif

	for(unsigned int i=0;i<30;i++)
	{
		wifi_sleep(2048);
		res = wifi_drv_level();
		if( res == NET_OK)
			return NET_OK;
		if( res == NET_ERR_WIFI && i > 3 )
			break;
		if(cmd_state & WIFI_CMD_STATE_CMES)
		{
			break;
		}
	}

    return wifi_net_error(res);
}


RES_CODE wifi_module_type::process_cmd(HANDLE client)
{
	RES_CODE res = RES_SIG_ERROR;


	if( !(drv_info->drv_data->wifi_flags_bad & WIFI_FLAG_ON))
	{
		switch (client->cmd)
		{
		case CMD_COMMAND:
			// WIFI Commands
			res = ((WIFI_CBF)client->src.as_voidptr)(this, client);
			break;

		case SOCK_CMD_OPEN:
			res = wifi_sock_open((CSocket*) client);
			break;
	#if USE_WIFI_LISTEN
		case SOCK_CMD_BIND_ADR:
			res =  wifi_sock_bind_adr((CSocket*) client);
			break;

		case SOCK_CMD_BIND_URL:
			res =  wifi_sock_bind_url((CSocket*) client);
			break;

		case SOCK_CMD_LISTEN:
			res = wifi_sock_listen((CSocket*) client);
			break;

		case SOCK_CMD_ACCEPT:
			res = wifi_sock_accept((CSocket*) client);
			break;

		case SOCK_CMD_GET_ADDR:
			res = wifi_sock_addr((CSocket*) client);
			break;
	#endif
		case SOCK_CMD_CONNECT_ADR:
			res = wifi_sock_connect_adr((CSocket*) client);
			break;

		case SOCK_CMD_CONNECT_URL:
			res = wifi_sock_connect_url((CSocket*) client);
			break;

		case SOCK_CMD_CLOSE:
			res = wifi_sock_close((CSocket*) client);
			break;

		default:
			// socket commands
			break;
		}
	}

//	if(RES_IDLE != res)
//		hnd_error(client);
	return res;
}

void wifi_module_type::wifi_process_tout()
{
}

int wifi_module_type::wifi_notification(const char* row)
{
	return 0; // we do not recognize this row
}

void wifi_module_type::wifi_cancelation()
{
}



//*----------------------------------------------------------------------------
//*			WIFI_DRV_OFF_CMD
//*----------------------------------------------------------------------------

RES_CODE wifi_drv_off(wifi_module_type *module, HANDLE hnd)
{

	if (module->wifi_drv_off() == NET_OK)
	{
		return RES_SIG_OK;
	}
	return RES_SIG_ERROR;
}

