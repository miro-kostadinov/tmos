
#include <tmos.h>
#include <usb_opt.h>
#include <usb_api.h>
#include <usb_hal.h>
#if USB_ENABLE_OTG
#include <tmos_atomic.h>
#endif


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
	client->mode.as_ushort[1] = drv_data->otg_state_cnt & ~USB_STATE_CNT_INVALID;
#endif
	return RES_SIG_OK;
}

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
		res = usb_hal_host_start(drv_info);
	} else
	{
		if(drv_data->otg_flags & USB_OTG_FLG_HOST_OK)
		{
			client->mode.as_ushort[1] = drv_data->otg_state_cnt;
			res = RES_SIG_OK;
		}
	}
	if( !(res & FLG_SIGNALED))
	{
		do
		{
			client->next = (HANDLE)atomic_fetch((volatile int*)&drv_data->pending);
		} while(atomic_store((volatile int*)&drv_data->pending, (int)client));
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
	usb_remote_device*	dst;
	usb_remote_device*	src;

	dst = (usb_remote_device*)client->dst.as_voidptr;
	if(dst && dst->dev_adress && dst->dev_adress <= MAX_USB_DEVICES)
	{
		src = &drv_info->drv_data->host_bus.usb_device[dst->dev_adress-1];

		if(dst->config_descriptor)
			tsk_free(dst->config_descriptor);
		memcpy(dst, src, sizeof(usb_remote_device));
		if(src->config_descriptor)
		{
			uint32_t size;

			if(src->config_descriptor->as_generic.bDescriptorType == CONFIGURATION_DESCRIPTOR)
				size = src->config_descriptor->wTotalLength;
			else
				size = src->config_descriptor->as_generic.bLength;

			dst->config_descriptor = (USBConfigurationDescriptor*)tsk_malloc(size);
			if(dst->config_descriptor)
			{
				memcpy(dst->config_descriptor, src->config_descriptor, size);
				return RES_SIG_OK;
			}
		}
	}
	return RES_SIG_ERROR;
}

#endif
