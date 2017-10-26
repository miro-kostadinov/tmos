/*
 * usb_host.cpp
 *
 *  Created on: Apr 25, 2013
 *      Author: miro
 */


#include <tmos.h>
#include <usb_host.h>
#include <usb_hal.h>
#include <usb_api.h>
#include <usb_descriptors.h>
#include <usb_svc.h>

/*
 * 							Host logic
 *
 *  	The USB host is started when a client calls tsk_command(USB_CMD_OTG_CONFIG)
 *  If the host is already running the call returns immediately with RES_OK.
 *  if the OTG controller is already running as a device, the call is put on hold
 *  until the session ends. Then it gets signaled with RES_FATAL and the client
 *  can retry.
 *  If the controller is free, the USB_CMD_OTG_CONFIG command tries to power up
 *  the host. From the usbdrv_thread all drv_data->host_bus.usb_device[] are
 *  initialized	and if/when the host controller gets powered the client gets
 *  signaled with RES_OK.
 * 	The success here means that the host is started and more likely there is a
 * 	devices connected. But the usb bus is not reset and not scanned yet.
 * 	The usbdrv_thread will reset and scan the bus and will set USB_OTG_FLG_HOST_OK
 * 	flag in usb_driver.drv_data->otg_flags on success.
 *
 * 	Bottom line: the client shall keep calling USB_CMD_OTG_CONFIG while it gets
 * 	RES_OK and USB_OTG_FLG_HOST_OK is set.
 *
 *
 */

static RES_CODE hdc_write(HANDLE hnd, const void* ptr, uint32_t len)
{
	RES_CODE res;

	for(int i=0; i<20; i++)
	{
		res = hnd->tsk_write(ptr, len, USB_SETUP_WRITE_TOUT);
		if(res == RES_OK)
			break;
		tsk_sleep(5);
	}
	return res;
}

static RES_CODE hdc_read(HANDLE hnd, void* ptr, uint32_t len)
{
	RES_CODE res;

	hnd->len = len;
	do
	{
		res = hnd->tsk_read(ptr, hnd->len, USB_SETUP_READ_TOUT);
		ptr = hnd->dst.as_voidptr;
	} while(hnd->len && res == RES_OK);

	//flush unexpected data
	if(hnd->mode1 && (res == RES_OK) && !hnd->len)	//mode1 = available data length
	{
		char buf[8];

		while(hnd->tsk_read(buf, sizeof(buf), USB_SETUP_READ_TOUT)== RES_OK)
		{
			if(!hnd->mode1)
				break;
		}
	}
	return res;
}

RES_CODE hdc_request(USBGenericRequest* req, void* ptr,  HANDLE hnd)
{
	RES_CODE res;

	for(int i=0; i<5; i++)
	{
		//SETUP
		hnd->mode1 = 1;	// mode1 = start of setup (use MDATA PID)
		res = hnd->tsk_write(req, sizeof(USBGenericRequest), USB_SETUP_WRITE_TOUT);
		hnd->mode1 = 0;	// mode1 = continue with data
		if(res == RES_OK)
		{
			if((req->bmRequestType & USB_REQ_DIR) == USB_REQ_DIR_OUT)
			{
				//DATA OUT
				if(req->wLength)
					hdc_write(hnd, ptr, req->wLength);

				// STATUS IN
				if(res == RES_OK)
				{
					res = hdc_read(hnd, NULL, 0);
				} else
					hdc_read(hnd, NULL, 0);
			} else
			{
				// DATA IN
				res = hdc_read(hnd, ptr, req->wLength);

				// STATUS OUT
				if(res == RES_OK)
					res = hdc_write(hnd, NULL, 0);
			}
		}
		if(res == RES_OK)
			break;
		tsk_sleep(50);
	}
	return res;
}

void usb_remote_dev_t::clean()
{
	dev_descriptor.as_generic.bLength = 0;
	dev_descriptor.as_generic.bDescriptorType = UNKNOWN_DESCRIPTOR;
	usb_hub_port.dev_adress = 0;
	usb_hub_port.dev_interface = 0;
	usb_hub_port.hub_num = 0;
	usb_hub_port.hub_port = 0;

	if(config_descriptor)
	{
		delete config_descriptor;
		config_descriptor = NULL;
	}
}

RES_CODE usb_remote_dev_t::select_hub_port(uint32_t indx)
{
	RES_CODE res;

	clean();
	ep0_hnd->mode0 = indx;
	res = ep0_hnd->tsk_command(USB_CMD_GET_HDEV, this);
	return res;
}

RES_CODE usb_remote_dev_t::std_request(void* ptr)
{
	return hdc_request(&req, ptr, ep0_hnd);
}

RES_CODE usb_remote_dev_t::get_device_descriptor()
{
	RES_CODE res;

	//read descriptor
    TRACELN_USB("hcd: dev descriptor(%u)", ep0_hnd->mode0);

	req.bmRequestType = USB_REQ_IN_STANDARD_DEVICE;
	req.bRequest = USBDeviceRequest_GET_DESCRIPTOR;
	req.wValue = DEVICE_DESCRIPTOR << 8;							// Descriptor type and index
	req.wIndex = 0;													// Zero or Language ID

	if(dev_descriptor.as_generic.bLength == 0)
	{
		req.wLength = 8;
	} else
		req.wLength = dev_descriptor.as_generic.bLength;

	res = std_request(&dev_descriptor);
	if(res == RES_OK)
		if(dev_descriptor.as_generic.bDescriptorType != DEVICE_DESCRIPTOR)
		{
			TRACELN_USB("hcd: bad dtor type %u", dev_descriptor.as_generic.bDescriptorType);
			res = RES_ERROR;
		}

	return res;
}

RES_CODE usb_remote_dev_t::get_config_descriptor_part(uint32_t size)
{
	RES_CODE res;
	void* new_descriptor;

	new_descriptor = usr_realloc(config_descriptor, size);
	if(size && !new_descriptor)
		return RES_OUT_OF_MEMORY;

	req.wLength = size;
	config_descriptor = (USBConfigurationDescriptor*) new_descriptor;

	res = std_request(config_descriptor);
	if(res == RES_OK)
	{
		if(config_descriptor->as_generic.bDescriptorType != CONFIGURATION_DESCRIPTOR)
		{
			TRACELN_USB("hcd: bad dtor type %u", config_descriptor->as_generic.bDescriptorType);
			res = RES_ERROR;
		}
	}
	return res;
}

RES_CODE usb_remote_dev_t::get_config_descriptor(uint32_t cfg_indx)
{
	RES_CODE res;

    TRACELN_USB("hcd: get cfg dtor(%u)", cfg_indx);

	req.bmRequestType = USB_REQ_IN_STANDARD_DEVICE;
	req.bRequest = USBDeviceRequest_GET_DESCRIPTOR;
	req.wValue = (CONFIGURATION_DESCRIPTOR << 8) | cfg_indx;		// Descriptor type and index
	req.wIndex = 0;													// Zero or Language ID

	res = get_config_descriptor_part(sizeof(USBConfigurationDescriptor));
	if(res==RES_OK && config_descriptor->wTotalLength > sizeof(USBConfigurationDescriptor))
	{
		res = get_config_descriptor_part(config_descriptor->wTotalLength);
	}
	return res;
}

RES_CODE usb_remote_dev_t::get_status_dev(uint32_t& status)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_IN_STANDARD_DEVICE;
	req.bRequest = USBGenericRequest_GET_STATUS;
	req.wValue = 0;
	req.wIndex = 0;
	req.wLength = 2;

	status = 0;
	res = std_request(&status);
	return res;
}

RES_CODE usb_remote_dev_t::set_feature_dev(uint32_t feature)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_OUT_STANDARD_DEVICE;
	req.bRequest = USBGenericRequest_SET_FEATURE;
	req.wValue = feature;											// Configuration Value
	req.wIndex = 0;													// Zero
	req.wLength = 0;

	res = std_request(NULL);
	return res;
}

RES_CODE usb_remote_dev_t::clr_feature_dev(uint32_t feature)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_OUT_STANDARD_DEVICE;
	req.bRequest = USBGenericRequest_CLEAR_FEATURE;
	req.wValue = feature;											// Configuration Value
	req.wIndex = 0;													// Zero
	req.wLength = 0;

	res = std_request(NULL);
	return res;
}

RES_CODE usb_remote_dev_t::set_configuration(uint32_t cfg_indx)
{
	RES_CODE res;

    TRACELN_USB("hcd: set cfg(%u)", cfg_indx);

	req.bmRequestType = USB_REQ_OUT_STANDARD_DEVICE;
	req.bRequest = USBDeviceRequest_SET_CONFIGURATION;
	req.wValue = cfg_indx;											// Configuration Value
	req.wIndex = 0;													// Zero
	req.wLength = 0;

	res = std_request(NULL);
	return res;
}

RES_CODE usb_remote_dev_t::set_address(uint32_t adr)
{
	RES_CODE res;

    TRACELN_USB("hcd: set adr (%u)=%u", ep0_hnd->mode0, adr);

	req.bmRequestType = USB_REQ_OUT_STANDARD_DEVICE;
	req.bRequest = USBDeviceRequest_SET_ADDRESS;
	req.wValue = adr;												// Device Address
	req.wIndex = 0;													// Zero
	req.wLength = 0;

	res = std_request(NULL);
	tsk_sleep(10);
	if(res == RES_OK)
	{
		usb_hub_port.dev_adress = adr;
		usb_drv_info->drv_data->usb_hub[ep0_hnd->mode0].dev_adress = adr;
	}

	return res;
}

RES_CODE usb_remote_dev_t::set_interface(uint32_t iface_indx, uint32_t alt_set)
{
	RES_CODE res;

    TRACELN("hcd: set inf %u", iface_indx);

	req.bmRequestType = USB_REQ_OUT_STANDARD_INTERFACE;
	req.bRequest = USBInterfaceRequest_SET_INTERFACE;
	req.wIndex = iface_indx;									 	// Interface
	req.wValue = alt_set;											// Alternative setting
	req.wLength = 0;
	res = std_request(NULL);

	return res;
}


RES_CODE usb_remote_dev_t::hdc_init(uint32_t port_indx)
{
	RES_CODE res;

	res = select_hub_port(port_indx);
	if(res == RES_OK)
	{
		//first time? -> set adress and configure endpoints
		if(!usb_hub_port.dev_adress)
		{
			USBEndpointDescriptor desc;

			//configure the control endpoints with default settings
			desc.as_generic.bDescriptorType = ENDPOINT_DESCRIPTOR;
			desc.bmAttributes = ENDPOINT_TYPE_CONTROL;
			desc.wMaxPacketSize = 8;
			desc.bEndpointAddress = 0x00;
			usb_svc_configendpoint(ep0_hnd, &desc.as_generic, 0);

			desc.bEndpointAddress = 0x80;
			usb_svc_configendpoint(ep0_hnd, &desc.as_generic, 0);

			//read device descriptor (1st 8 bytes)
			res = get_device_descriptor();

			//re-configure the endpoints and read the complete device descriptor
			if(res == RES_OK )
			{
				switch(dev_descriptor.bMaxPacketSize0)
				{
				case 8:			//we are fine
					break;

				case 16:
				case 32:
				case 64:
					desc.wMaxPacketSize = dev_descriptor.bMaxPacketSize0;
					desc.bEndpointAddress = 0x00;
					usb_svc_configendpoint(ep0_hnd, &desc.as_generic, 0);
					desc.bEndpointAddress = 0x80;
					usb_svc_configendpoint(ep0_hnd, &desc.as_generic, 0);
					break;

				default:
					TRACELN_USB("Unsupported bMaxPacketSize0=%u", dev_descriptor.bMaxPacketSize0);
					return RES_ERROR;
				}
			}

			//set the new address
			if(res == RES_OK)
			{
				res = set_address(port_indx+1);		//we use port# +1 for the dev adr
			}
		} else
			res = get_device_descriptor();

		//read device descriptor (complete)
		if(res == RES_OK)
			res = get_device_descriptor();
	}

	return res;

}



USBEndpointDescriptor* usb_get_enpoint(const USBConfigurationDescriptor* des,
		int indx)
{
	if (des && des->as_generic.bDescriptorType == CONFIGURATION_DESCRIPTOR)
	{
		int size = des->GetTotalLength();
		size -= sizeof(USBConfigurationDescriptor);

		const USBGenericDescriptor *pd = &des->as_generic;
		while ( size > 0)
		{

			pd = pd->GetNextDescriptor();
			size -= pd->GetLength();

			if (pd->GetType() == ENDPOINT_DESCRIPTOR)
			{
				if (!indx--)
				{

					return (USBEndpointDescriptor*)pd;
				}
			}
		}
	}

	return NULL;
}


RES_CODE usb_host_reset_bus(USB_DRV_INFO drv_info, HANDLE hnd)
{
	RES_CODE res = RES_ERROR;

	// clear reset request and assert it
	__disable_irq();
	usb_otg_clr_flags(drv_info, USB_OTG_FLG_HOST_RST);
	__enable_irq();

	if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST)
	{
		res = usb_hal_host_bus_reset(drv_info);
		if(res != RES_OK)
			return res;

		tsk_sleep(10);
		if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_RST)
		{
			return RES_ERROR;
		}
		// if everything OK -> enumerate
		if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST)
		{
			usb_otg_set_flags(drv_info, USB_OTG_FLG_HOST_OK);
		}
	}
	return res;
}


