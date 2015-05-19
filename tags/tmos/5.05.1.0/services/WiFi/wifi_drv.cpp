//////////////////////////////////////////////////////////////////////////
//
//			WIFI Driver
//
//////////////////////////////////////////////////////////////////////////


#include <tmos.h>
#include "wifi_drv.h"
//#include "wifi_core.h"

WEAK_C wifi_module_type* wifi_detect(WIFI_DRIVER_INFO* drv_info)
{
	return NULL;
}

#define WIFI_CANCEL_SIGNAL 1

//*----------------------------------------------------------------------------
//*			WIFI Task
//*----------------------------------------------------------------------------

void wifi_thread(WIFI_DRIVER_INFO* drv_info)
{
    WIFI_DRIVER_DATA * drv_data = drv_info->drv_data;
    HANDLE hlp_hnd = &drv_data->helper_hnd;
    HANDLE client;
    unsigned int signals/*, measure=0*/;
    wifi_module_type* wifi_module;
    RES_CODE res;


    ALLOCATE_SIGNAL(WIFI_CANCEL_SIGNAL);

    hlp_hnd->tsk_safe_open(drv_info->info.drv_index, 0);
    hlp_hnd->tsk_start_read(NULL, 	0);


    wifi_module = drv_data->wifi_module;
	if(!wifi_module)
	{
	    wifi_module = wifi_detect(drv_info);
	}

    for(;;)
    {
        if( wifi_module)
        {
        	// make sure the receiver is ON
        	wifi_module->process_input(0, NULL);
        }
        signals = tsk_wait_signal(SIGNAL_ANY, 1000);

        // step 1 - Check the WIFI
        res = signals & ~hlp_hnd->signal;
        if( res && wifi_module)
        {
        	wifi_module->process_input(res, NULL);
        }

        // step 2 - Check the driver
        if(signals & hlp_hnd->signal)
        {
        	hlp_hnd->res &= ~FLG_SIGNALED;

        	if(hlp_hnd->res == RES_OK)
        	{
        	    client = (HANDLE)hlp_hnd->dst.as_voidptr;

        		if(!wifi_module)
        		{
        		    wifi_module = wifi_detect(drv_info);
        		}

    		    if(wifi_module)
    		    {
                	drv_info->drv_data->wifi_error = 0;
					if(client->cmd & FLAG_READ)
					{
						res = wifi_module->process_read((CSocket*)client);
					} else
					{
						if(client->cmd & FLAG_WRITE)
						{
							res = wifi_module->process_write((CSocket*)client);
						}
						else
	    			    	res = wifi_module->process_cmd(client);
					}
    		    }  else
    		    {
    		    	res = RES_SIG_ERROR;
    		    }

            	if(res & FLG_SIGNALED)
            	{
            		if(res != RES_SIG_OK)
            		{
            			if(wifi_module)
            				wifi_module->hnd_error(client);
            			else
            				client->error = NET_ERR_PHY_NOT_READY;
            		}
            		else
            			client->error = NET_OK;
            		tsk_HND_SET_STATUS(client, res);
            		if(res != RES_SIG_OK && wifi_module)
                    	wifi_module->wifi_reset(true);
            	}
        	}
        	hlp_hnd->tsk_start_read(NULL, 0);
        }

        // step 3 - timeouts
        if(!signals && wifi_module)
        {
        	wifi_module->wifi_process_tout();
        	wifi_module->wifi_reset(false);
        }

        // step 4 - cancelation
        if(signals & WIFI_CANCEL_SIGNAL)
        {
        	wifi_module->wifi_cancelation();
        }
    }
}
TASK_DECLARE_STATIC(wifi_task, "WIFIT", (void(*)())wifi_thread, 85, 256+TRACE_SIZE);

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void WIFI_DCR(WIFI_DRIVER_INFO * drv_info, unsigned int reason, HANDLE param)
{
    WIFI_DRIVER_DATA * drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
           	usr_task_init_static(&wifi_task_desc, true);
           	wifi_task.sp->r0.as_cvoidptr = drv_info;
           	drv_data->wifi_flags_ok = WIFI_STATE_OFF;
           	drv_data->wifi_flags_bad = WIFI_STATE_OFF;
            break;

	    case DCR_OPEN:
	    	if(!(drv_data->wifi_flags_bad & WIFI_FLAG_SHUTDOWN) || param->mode.as_int == 1)
	    	{
		    	param->res = RES_OK;
		    	drv_data->wifi_clients ++;
	    	}
	    	break;

	    case DCR_CLOSE:
	    	drv_data->wifi_clients --;
	    	break;

	    case DCR_CANCEL:
	    	if(!param->svc_list_cancel(drv_info->drv_data->waiting))
	    	{
	    		param->mode1 = 1;
	    		svc_send_signal(&wifi_task, WIFI_CANCEL_SIGNAL);
	    	}
            break;

    }
}

//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void WIFI_DSR(WIFI_DRIVER_INFO * drv_info, HANDLE hnd)
{
	HANDLE tmp;
	WIFI_DRIVER_DATA* drv_data= drv_info->drv_data;

	hnd->res = RES_BUSY;
	hnd->mode1 = 0;
	if(hnd == &drv_data->helper_hnd)
	{
		// Request from the helper task...
		tmp = drv_data->waiting;		//  first waiting handle
		if( tmp )
		{
			//return one handle at a time
			drv_data->waiting = tmp->next;

			hnd->dst.as_voidptr = tmp;
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		}
	} else
	{
		// this is a client handle...
		tmp = &drv_data->helper_hnd;
		if( tmp->res == RES_BUSY )
		{
			//the helper task is waiting for object...
			tmp->dst.as_voidptr = hnd;
			svc_HND_SET_STATUS(tmp, RES_SIG_OK);
		} else
		{
        	hnd->list_add(drv_data->waiting);
		}
	}
}



