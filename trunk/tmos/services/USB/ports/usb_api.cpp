
#include <tmos.h>
#include <usb_opt.h>
#include <usb_api.h>
#include <usb_hal.h>


//--------------------   Config    -------------------------------------------//
RES_CODE usb_api_device_config(usb_handle* client, USB_DRV_INFO drv_info)
{
	const USBDDriverDescriptors* descriptors;
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	if(drv_data->usb_device_mode == USB_MODE_NONE)
	{
		drv_data->usb_device_mode = USB_MODE_ENABLED;
		drv_data->usb_state = USB_STATE_SUSPENDED;
		descriptors = (const USBDDriverDescriptors*)client->src.as_cvoidptr;
		if(!descriptors)
			descriptors = drv_info->dev_descriptors;
		drv_data->device.Initialize(descriptors);
		usb_hal_configure(drv_info);
	}

	return RES_SIG_OK;
}

RES_CODE usb_handle::device_config(const USBDDriverDescriptors* drv_descriptors)
{
	if(complete())
	{
		src.as_cvoidptr = drv_descriptors;
		set_res_cmd(USB_CMD_DEVICE_CONFIG);
		tsk_start_and_wait();
	}
	return res;
}



extern "C" const USB_API_FUNC usb_api_functions[MAX_USBCALLBACK+1] =
{
	usb_api_device_config
};
