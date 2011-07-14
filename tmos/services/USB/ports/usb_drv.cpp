/**************************************************************************//**
 * @file     usbd_drv.cpp
 * @ingroup  SVC_USB
 * @brief    USB driver implementation
 * @version  V3.00
 * @date     23. May 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <usb_opt.h>
#include <usb_drv.h>
#include <usb_hal.h>
#include <usb_api.h>
#include <usb_requests.h>
#include <usb_svc.h>

void USB_DRIVER_DATA::usb_event(USB_EVENT event)
{
	switch(event)
	{
	case e_init:
		usb_state = USB_STATE_DOWN;

		break;

	case e_reset:
    	usb_power &= ~(USB_POWER_WAKING);
    	frame_count = 0;
		break;

	default: ;
	}
}

HANDLE usb_end_transfer(Endpoint *endpoint, unsigned int status)
{
    HANDLE hnd;

    hnd=endpoint->pending;

	if (hnd)
	{
		TRACE1_USB(" EoT ");
		endpoint->pending = hnd->next;
		if (__get_CONTROL() & 2)
		{
			usr_HND_SET_STATUS(hnd, status);
		}
		else
		{
			svc_HND_SET_STATUS(hnd, status);
		}
	}
	return hnd;
}

void usb_end_transfers(Endpoint *endpoint, unsigned int status)
{
	while(usb_end_transfer(endpoint, status))
	{
	}
}

void usbdrv_thread(USB_DRV_INFO drv_info)
{
//	USB_DRIVER_DATA*  drv_data = drv_info->drv_data;
    CHandle helper;
    CHandle req_hnd;
    USBGenericRequest request;


//	ALLOCATE_SIGNAL(LWIP_THREAD_RXTXSIG);


	helper.tsk_safe_open(drv_info->info.drv_index, 0);
	helper.tsk_start_command(NULL, 0);
	req_hnd.tsk_safe_open(drv_info->info.drv_index, 0);
	req_hnd.mode0 = EPT_0;
	req_hnd.mode1 = EPT_0;
	req_hnd.tsk_start_read(&request, 8);
    while(1)
    {
    	unsigned int sig;

    	sig = tsk_get_signal(SIGNAL_ANY);


        // 2) get waiting clients
		if(sig & helper.signal)
        {
			usb_handle* client;

        	helper.res &= ~FLG_SIGNALED;

			while( (client = (usb_handle*)helper.dst.as_voidptr) )
			{
				RES_CODE res  = RES_SIG_ERROR;

				helper.dst.as_voidptr = client->next;

				if((client->cmd>>4) <= MAX_USBCALLBACK)
				{
					res = usb_api_functions[client->cmd>>4](client, drv_info);
				}

			    if(res & FLG_SIGNALED)
			    {
				    tsk_HND_SET_STATUS(client, res);
			    }
			}
			helper.tsk_start_command(NULL, 0);
        }
		if(sig & req_hnd.signal)
		{
        	req_hnd.res &= ~FLG_SIGNALED;
			if(req_hnd.res == RES_OK)
			{
				TRACE_USB(" | req:");
				drv_info->drv_data->device.RequestHandler(drv_info, &request, &req_hnd);
			}
			req_hnd.tsk_start_read(&request, 8);
		}

    }


}
TASK_DECLARE_STATIC(usbdrv_task, "USBH", (void (*)(void))usbdrv_thread, 60, 400 + TRACE_SIZE);

void USB_DCR(USB_DRV_INFO drv_info, unsigned int reason, HANDLE param)
{
//	USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
        	// drv_data constructor is not called yet!
        	usb_hal_reset(drv_info);

           	usr_task_init_static(&usbdrv_task_desc, true);
           	usbdrv_task.sp->r0.as_cvoidptr = drv_info;
            break;
        case DCR_HANDLE:
        		((USB_SVC_HOOK)param->dst.as_int)(drv_info, param);
        	break;


    	case DCR_CLOSE:
        case DCR_CANCEL:

        case DCR_OPEN:
		  {

			param->res = RES_OK;
			break;
		  }



    }
}

void USB_DSR(USB_DRV_INFO drv_info, HANDLE hnd)
{
	unsigned char eptnum;
    Endpoint *endpoint;
    HANDLE prev, tmp;
    USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	hnd->next = NULL;
	hnd->res = RES_BUSY;

	if (hnd->cmd & FLAG_COMMAND)
	{
		if(hnd->mode.as_voidptr)
		{
			// this is a client handle...
			hnd->res = RES_BUSY;
			if( (tmp=drv_data->helper) )
			{
				//the helper task is waiting for object...
				hnd->next = NULL;
				drv_data->helper = NULL;
				tmp->dst.as_voidptr = hnd;
				svc_HND_SET_STATUS(tmp, RES_SIG_OK);
			} else
			{
				//queue while helper task is busy
				hnd->next = drv_data->waiting;
				drv_data->waiting = hnd;
			}

		} else
		{
			// this should be the helper task
			if( (tmp=drv_data->waiting) )
			{
				drv_data->waiting = NULL;
				hnd->dst.as_voidptr = tmp;
				svc_HND_SET_STATUS(hnd, RES_SIG_OK);
			} else
			{
				hnd->res = RES_BUSY;
				drv_data->helper = hnd;
			}

		}
		return;
	}

	if (hnd->cmd & FLAG_WRITE)
    {
		eptnum = hnd->mode1;
	    TRACE_USB(" Write%d(%d) ", eptnum , hnd->len);
		endpoint = &(drv_data->endpoints[eptnum]);
	    if( (prev=endpoint->pending) )
	    {
	    	while(prev->next)
	    		prev = prev->next;
	    	prev->next = hnd;
	    } else
	    {
		    if (endpoint->state != ENDPOINT_STATE_IDLE)
		    {
				svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
		    } else
		    {
			    endpoint->state = ENDPOINT_STATE_SENDING;
			    endpoint->pending = hnd;		//sending
			    usb_write_payload(ENTPOINT_FIFO(drv_info->hw_base, eptnum),
						hnd, endpoint->txfifo_size);
			    usb_hal_txpktrdy(drv_info->hw_base, eptnum, hnd->len);


			    // Enable interrupt on endpoint
			    ENTPOINT_ENABLE_INT(drv_info->hw_base, eptnum);
		    }
	    }
	    return;
    }


	if (hnd->cmd & FLAG_READ)
    {
		eptnum = hnd->mode0;
		endpoint = &(drv_data->endpoints[eptnum]);

//	    TRACE_USB(" Read%d(%d) ", eptnum, hnd->len);

		switch(endpoint->state)
		{

		case ENDPOINT_STATE_RECEIVING_OFF:
    		endpoint->rxfifo_cnt = usb_read_payload(ENTPOINT_FIFO(drv_info->hw_base,
					eptnum), hnd, endpoint->rxfifo_cnt);
            if (endpoint->rxfifo_cnt)
            {
            	svc_HND_SET_STATUS(hnd, RES_SIG_OK);
            }
            else
            {
            	usb_ack_packet(drv_info->hw_base, endpoint, eptnum);
            	endpoint->state = ENDPOINT_STATE_IDLE;
            	svc_HND_SET_STATUS(hnd, RES_SIG_IDLE);
            }
            break;

		default:
		    if( (prev=endpoint->pending) )	//receiving
		    {
		    	while(prev->next)
		    		prev = prev->next;
		    	prev->next = hnd;
		    } else
		    	endpoint->pending = hnd;	//receiving

		    // Enable interrupt on endpoint
		    ENTPOINT_ENABLE_INT(drv_info->hw_base, eptnum);
		}
	    return;
    }


	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}
