/*
 * usb_host.h
 *
 *  Created on: Apr 25, 2013
 *      Author: miro
 */

#ifndef USB_HOST_H_
#define USB_HOST_H_


#include <tmos.h>
#include <usb_opt.h>
#include <usb_drv.h>
#include <usb_std_reqs.h>

struct usb_remote_dev_t
{
	HANDLE	ep0_hnd;
	const USB_DRIVER_INFO*		usb_drv_info;
	USBConfigurationDescriptor	*config_descriptor;
	usb_hub_port_t				usb_hub_port;
	USBDeviceDescriptor 		dev_descriptor;
	USBGenericRequest 			req;

	usb_remote_dev_t(HANDLE h1):
		ep0_hnd(h1), usb_drv_info(NULL), config_descriptor(NULL)
	{
		dev_descriptor.as_generic.bLength = 0;
		dev_descriptor.as_generic.bDescriptorType = UNKNOWN_DESCRIPTOR;
	};
	virtual ~usb_remote_dev_t()
	{
		clean();
	}

	virtual void clean();

	RES_CODE select_hub_port(uint32_t indx);
	RES_CODE std_request(void* ptr);
	RES_CODE get_device_descriptor();
	RES_CODE get_config_descriptor_part(uint32_t size);
	RES_CODE get_config_descriptor(uint32_t cfg_indx);
	RES_CODE get_status_dev(uint32_t& status);
	RES_CODE set_feature_dev(uint32_t feature);
	RES_CODE clr_feature_dev(uint32_t feature);
	RES_CODE set_configuration(uint32_t cfg_indx);
	RES_CODE set_address(uint32_t adr);
	RES_CODE set_interface(uint32_t iface_indx, uint32_t alt_set);

	RES_CODE hdc_init(uint32_t port_indx);
};


RES_CODE hdc_request(USBGenericRequest* req, void* ptr,  HANDLE hnd);
RES_CODE usb_host_reset_bus(USB_DRV_INFO drv_info, HANDLE hnd);

USBEndpointDescriptor* usb_get_enpoint(const USBConfigurationDescriptor* des,
		int indx);


#endif /* USB_HOST_H_ */
