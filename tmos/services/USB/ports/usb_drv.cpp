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
#include <cmsis_cpp.h>

void USB_DRIVER_DATA::usb_event(USB_EVENT event)
{
	switch(event)
	{
	case e_init:
		usb_state = USB_STATE_DOWN;
		break;

	case e_powered:
		usb_state = USB_STATE_POWERED;
		break;

	case e_susppend:
		usb_state = USB_STATE_SUSPENDED;
		break;


	case e_reset:
		usb_state = USB_STATE_DEFAULT;
    	usb_power &= ~(USB_POWER_WAKING);
    	frame_count = 0;
		break;

	default: ;
	break;
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
	switch(reason)
    {
        case DCR_RESET:
        	// drv_data constructor is not called yet!
        	usb_hal_reset(drv_info);

        	if(drv_info->info.peripheral_indx != ID_NO_PERIPH)
        	{
               	usr_task_init_static(&usbdrv_task_desc, true);
               	usbdrv_task.sp->r0.as_cvoidptr = drv_info;
        	}
            break;
        case DCR_HANDLE:
        		((USB_SVC_HOOK)param->dst.as_int)(drv_info, param);
        	break;


    	case DCR_CLOSE:
        case DCR_CANCEL:
        	usb_hal_cancel_hnd(drv_info, param);
    		break;

        case DCR_OPEN:
			param->res = RES_OK;
			break;
    }
}

void USB_DSR(USB_DRV_INFO drv_info, HANDLE hnd)
{
	unsigned char eptnum;
    HANDLE tmp;
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
		usb_start_tx(drv_info, hnd);
	    return;
    }


	if (hnd->cmd & FLAG_READ)
    {
		eptnum = hnd->mode0;
		usb_start_rx(drv_info, hnd, eptnum, &(drv_data->endpoints[eptnum]));
	    return;
    }


	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}
