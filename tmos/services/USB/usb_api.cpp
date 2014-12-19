
#include <tmos.h>
#include <usb_opt.h>
#include <usb_api.h>
#include <usb_hal.h>
#if USB_ENABLE_OTG
#include <tmos_atomic.h>
#endif
#if USB_ENABLE_HOST
#include <usb_host.h>
#endif

#if USB_ENABLE_DEVICE
/**
 * Configure the USB core in device mode
 *
 * called from the client with CHandle::tsk_command(USB_CMD_DEVICE_CONFIG, desc)
 * where desc can be NULL (to use default device descriptors)
 *
 * The function is executed from the usb task context
 *
 * @param drv_info
 * @param client
 * @return
 */
RES_CODE usb_api_device_config(USB_DRV_INFO drv_info, HANDLE client)
{
	const USBDDriverDescriptors* descriptors;
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	if(drv_data->usb_state == USBST_ALL_DOWN)
	{
		drv_data->usb_state = USBST_DEVICE_MODE;
		descriptors = (const USBDDriverDescriptors*)client->dst.as_cvoidptr;
		if(!descriptors)
			descriptors = drv_info->dev_descriptors;
		drv_data->device.Initialize(descriptors);
		usb_hal_device_start(drv_info);
	}
#if USB_ENABLE_OTG
	drv_data->otg_flags |= USB_OTG_FLG_DEV;
	client->mode.as_ushort[1] = drv_data->drv_state_cnt & ~USB_STATE_CNT_INVALID;
#endif
	return RES_SIG_OK;
}
#endif

#if USB_ENABLE_HOST
/**
 * Configure the USB core in host mode
 *
 * called from the client with CHandle::tsk_command(USB_CMD_OTG_CONFIG, NULL)
 *
 * The function is executed from the usb task context
 *
 * @param drv_info
 * @param client
 * @return
 */
RES_CODE usb_api_otg_config(USB_DRV_INFO drv_info, HANDLE client)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	uint32_t otg_flags;
	RES_CODE res = RES_IDLE;

	TRACE_USB_NAME(drv_info);
	TRACE1_USB(" cfg");
	otg_flags = drv_data->otg_flags;
	if( !(otg_flags & USB_OTG_FLG_HOST))
	{
		// this is the first host request
		drv_data->otg_flags |= USB_OTG_FLG_HOST;
	}

	if(!(otg_flags & (USB_OTG_FLG_DEV_CON | USB_OTG_FLG_HOST_CON)) )
	{
		// Host starts here and only here!
		if((drv_info->cfg->stm32_otg & CFG_STM32_OTG_FORCE_HOST)||
				(drv_info->hw_base->core_regs.GINTSTS & OTG_GINTSTS_CMOD ))
			res = usb_hal_host_start(drv_info);
	} else
	{
		if(drv_data->otg_flags & USB_OTG_FLG_HOST_OK)
		{
			client->mode.as_ushort[1] = drv_data->drv_state_cnt;
			res = RES_SIG_OK;
		}
	}
	if( !(res & FLG_SIGNALED))
	{
		do
		{
			client->next = (HANDLE)atomic_fetch((volatile int*)&drv_data->pending);
		} while(atomic_store((volatile int*)&drv_data->pending, (int)client));

#if USE_CPU_SLEEP_MODE
		locked_dec_int(&cpu_sleep_counter);			//enable sleep
		locked_inc_int(&drv_data->otg_sleep_flag);	// till next irq
#endif

	}

	TRACELN_USB("USB cfg res %u", res);
	return res;
}

RES_CODE usb_api_otg_off(USB_DRV_INFO drv_info, HANDLE client)
{
	usb_otg_clr_flags(drv_info, USB_OTG_FLG_HOST_CON | USB_OTG_FLG_HOST_PWR
			| USB_OTG_FLG_HOST_RST | USB_OTG_FLG_HOST_OK);
	return RES_SIG_OK;
}

RES_CODE usb_api_get_hdev(USB_DRV_INFO drv_info, HANDLE client)
{
	usb_remote_dev_t*	dst;
	usb_hub_port_t*	src;

	dst = (usb_remote_dev_t*)client->dst.as_voidptr;
	if(dst && client->mode0 <= MAX_HUB_PORTS)
	{
		src = &drv_info->drv_data->usb_hub[client->mode0];

		dst->usb_drv_info = drv_info;
		memcpy(&dst->usb_hub_port, src, sizeof(usb_hub_port_t));
		return RES_SIG_OK;
	}
	return RES_SIG_ERROR;
}

RES_CODE usb_api_host_resume(USB_DRV_INFO drv_info, HANDLE client)
{
	usb_hal_host_resume(drv_info);
	__disable_irq();
	drv_info->drv_data->otg_flags &= ~USB_OTG_FLG_SUSPEND;
	__enable_irq();
	return RES_SIG_OK;
}

#endif //USB_ENABLE_HOST
