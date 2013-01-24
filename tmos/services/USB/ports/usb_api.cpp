
#include <tmos.h>
#include <usb_opt.h>
#include <usb_api.h>
#include <usb_hal.h>


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

	return RES_SIG_OK;
}

