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
#if USB_ENABLE_HOST
#include <usb_host.h>
#include <tmos_atomic.h>
#endif


void usbdrv_thread(USB_DRV_INFO drv_info)
{
    CHandle helper;
    CHandle req_hnd;
    USBGenericRequest request;
    bool requested = false;
	unsigned int sig=0;

    ALLOCATE_SIGNAL(USB_DRIVER_SIG);
	helper.tsk_safe_open(drv_info->info.drv_index, 0);
	helper.tsk_start_command(NULL, 0);
	req_hnd.tsk_safe_open(drv_info->info.drv_index, USB_DRV_MODE(EPT_0, EPT_0));
    while(1)
    {

    	sig |= tsk_get_signal(SIGNAL_ANY);


        // 2) get waiting clients
		if(sig & helper.signal)
        {
			HANDLE client;

			sig ^= helper.signal;
        	helper.res &= ~FLG_SIGNALED;
			client = (HANDLE)helper.dst.as_voidptr;
			if(client)
			{
				RES_CODE res;

				res = ((USB_API_FUNC)client->src.as_voidptr)(drv_info, client);
				if(res & FLG_SIGNALED)
				{
					tsk_HND_SET_STATUS(client, res);
				}
			}

			helper.tsk_start_command(NULL, 0);
        }
		if(sig & req_hnd.signal)
		{
			sig ^= req_hnd.signal;
        	req_hnd.res &= ~FLG_SIGNALED;
			if(req_hnd.res == RES_OK)
			{
				TRACE_USB(" | req:");
				drv_info->drv_data->device.RequestHandler(drv_info, &request, &req_hnd);
			}
			requested = false;
		}
		if(!requested)
		{
#if USB_ENABLE_OTG
			if(sig == USB_DRIVER_SIG)
			{
				if(drv_info->drv_data->otg_h_sig & OTG_H_SIG_CON)
				{
					do
					{
						sig = atomic_fetch((volatile int*)&drv_info->drv_data->otg_h_sig);
						sig &= ~OTG_H_SIG_CON;
					} while(atomic_store((volatile int*)&drv_info->drv_data->otg_h_sig, sig));

					//wait 1s for connect
					for(int retries =0; retries <10; ++retries)
					{
						if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_RST)
							break;
				    	sig = tsk_wait_signal(USB_DRIVER_SIG, 100);
				    	if (sig)
				    		break;
					}
					if( !(drv_info->drv_data->otg_h_sig & OTG_H_SIG_RST))
					{
						usb_api_otg_off(drv_info, NULL);
					}
				}
				if(drv_info->drv_data->otg_h_sig & OTG_H_SIG_RST)
				{
					do
					{
						sig = atomic_fetch((volatile int*)&drv_info->drv_data->otg_h_sig);
						sig &= ~OTG_H_SIG_RST;
					} while(atomic_store((volatile int*)&drv_info->drv_data->otg_h_sig, sig));

					if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_RST)
					{
						RES_CODE res;

						// Reset requested
						for(int retries =0; retries <3; ++retries)
						{
							res = usb_host_reset_bus(drv_info, &req_hnd);
							if( res == RES_OK)
								break;
						}
						if(res != RES_OK)
							usb_api_otg_off(drv_info, NULL);

					}
				}
				if(drv_info->drv_data->otg_h_sig & OTG_H_SIG_RESUME)
				{
					do
					{
						sig = atomic_fetch((volatile int*)&drv_info->drv_data->otg_h_sig);
						sig &= ~OTG_H_SIG_RESUME;
					} while(atomic_store((volatile int*)&drv_info->drv_data->otg_h_sig, sig));
					usb_host_resume(drv_info);


				}
				sig = 0;
			}
			if(drv_info->drv_data->otg_flags & USB_OTG_FLG_DEV_OK)
			{

				req_hnd.mode.as_ushort[1] = drv_info->drv_data->otg_state_cnt;
#endif
				req_hnd.tsk_start_read(&request, 8);
				requested = true;
#if USB_ENABLE_OTG
			}
#endif
		}

    }

}

void USB_DCR(USB_DRV_INFO drv_info, unsigned int reason, HANDLE param)
{
	switch(reason)
    {
        case DCR_RESET:
        	// drv_data constructor is not called yet!
#if USB_ENABLE_OTG
        	drv_info->drv_data->otg_state_cnt = USB_STATE_CNT_INVALID;
#endif
        	usb_drv_reset(drv_info);
            break;

        case DCR_HANDLE:
        		((USB_SVC_HOOK)param->dst.as_int)(drv_info, param);
        	break;

    	case DCR_CLOSE:
        case DCR_CANCEL:
        	usb_drv_cancel_hnd(drv_info, param);
    		break;

        case DCR_OPEN:
			param->res = RES_OK;
			break;
    }
}

void USB_DSR(USB_DRV_INFO drv_info, HANDLE hnd)
{
    HANDLE tmp;
    USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	hnd->next = NULL;
	hnd->res = RES_BUSY;

	if (hnd->cmd & FLAG_COMMAND)
	{
		if(hnd->mode.as_voidptr)
		{
			// this is a client handle...
			if( (tmp=drv_data->helper) )
			{
				//the helper task is waiting for object...
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
				drv_data->waiting = tmp->next;
				hnd->dst.as_voidptr = tmp;
				svc_HND_SET_STATUS(hnd, RES_SIG_OK);
			} else
			{
				drv_data->helper = hnd;
			}
		}
		return;
	}

#if USB_ENABLE_OTG
	if(hnd->mode.as_ushort[1] != drv_data->otg_state_cnt)
	{
		//driver state has changed, handle must update...
		svc_HND_SET_STATUS(hnd, FLG_SIGNALED | RES_FATAL);
		return;
	}
#endif

	if (hnd->cmd & FLAG_WRITE)
    {
		usb_drv_start_tx(drv_info, hnd);
	    return;
    }


	if (hnd->cmd & FLAG_READ)
    {
		usb_drv_start_rx(drv_info, hnd);
	    return;
    }


	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}
