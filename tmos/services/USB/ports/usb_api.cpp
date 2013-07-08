
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


#if USB_ENABLE_OTG
	uint32_t otg_flags;

	otg_flags = drv_data->otg_flags;
	if( !(otg_flags & USB_OTG_FLG_DEV))
	{
		// this is the first device request
		descriptors = (const USBDDriverDescriptors*)client->dst.as_cvoidptr;
		if(!descriptors)
			descriptors = drv_info->dev_descriptors;
		drv_data->device.Initialize(descriptors);

		drv_data->otg_flags |= USB_OTG_FLG_DEV;
		drv_data->usb_state = USBST_DEVICE_MODE;
	}

	if( !otg_flags )
	{
		// this is the first request
		usb_hal_host_start(drv_info);
	} else
	{
		if(drv_data->otg_flags & USB_OTG_FLG_DEV_OK)
		{
			client->mode.as_ushort[1] = drv_data->otg_state_cnt;
			return RES_SIG_OK;
		}
	}
	do
	{
		client->next = (HANDLE)atomic_fetch((volatile int*)&drv_data->pending);
	} while(atomic_store((volatile int*)&drv_data->pending, (int)client));

	return RES_IDLE;

#else
	if(drv_data->usb_state == USBST_ALL_DOWN)
	{
		drv_data->usb_state = USBST_DEVICE_MODE;
		descriptors = (const USBDDriverDescriptors*)client->dst.as_cvoidptr;
		if(!descriptors)
			descriptors = drv_info->dev_descriptors;
		drv_data->device.Initialize(descriptors);
		usb_hal_device_start(drv_info);
	}
	return RES_SIG_OK;
#endif

}

#if USB_ENABLE_HOST
RES_CODE usb_api_otg_config(USB_DRV_INFO drv_info, HANDLE client)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	uint32_t otg_flags;

	otg_flags = drv_data->otg_flags;
	if( !(otg_flags & USB_OTG_FLG_HOST))
	{
		// this is the first host request
		drv_data->otg_flags |= USB_OTG_FLG_HOST;
	}

	if(!(otg_flags & (USB_OTG_FLG_DEV_CON | USB_OTG_FLG_HOST_CON)) )
	{
		// Host starts here and only here!
		usb_hal_host_start(drv_info);
	} else
	{
		if(drv_data->otg_flags & USB_OTG_FLG_HOST_OK)
		{
			client->mode.as_ushort[1] = drv_data->otg_state_cnt;
			return RES_SIG_OK;
		}
	}
	do
	{
		client->next = (HANDLE)atomic_fetch((volatile int*)&drv_data->pending);
	} while(atomic_store((volatile int*)&drv_data->pending, (int)client));

	return RES_IDLE;

}

RES_CODE usb_api_otg_off(USB_DRV_INFO drv_info, HANDLE client)
{
	usb_otg_clr_flags(drv_info, USB_OTG_FLG_HOST_CON | USB_OTG_FLG_HOST_PWR
			| USB_OTG_FLG_HOST_RST | USB_OTG_FLG_HOST_OK);
	return RES_SIG_OK;
}

#endif
