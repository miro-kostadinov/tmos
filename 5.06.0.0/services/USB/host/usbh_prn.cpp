/*
 * usbh_prn.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: miro
 */

#include <tmos.h>
#include <usbh_prn.h>
#include <usb_descriptors.h>
#include <usb_svc.h>

bool parse_ieee1284_id(CSTRING& dst, const char* src, const char* id)
{
	size_t len = strlen(id);
	const char* end;

	while(src)
	{
		end = strchr(src, ';');
		if(end)
		{
			if(src[len] == ':' && strncmp(src, id, len) == 0)
			{
				src += len+1;
				if(!dst.empty())
					dst += ' ';
				dst.append(src, end-src);
				return true;
			}
			end++;
		}
		src = end;
	}
	return false;
}


void usb_remote_prn_t::clean()
{
	usb_remote_dev_t::clean();
	device_id.clear();
	port_status =0;

}

RES_CODE usb_remote_prn_t::get_device_id()
{
	RES_CODE res= RES_OK;
	uint32_t len=0;

	//read descriptor

	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = USBPrinter_GET_DEVICE_ID;
	req.wIndex = (pid->bInterfaceNumber << 8) + pid->bAlternateSetting;			// Interface and Alternate Setting
	req.wValue = 0;			// index
	req.wLength = 2;
	res = std_request(&len);

	len = __REV16(len);

	if(len>2 && res == RES_OK)
	{
		if(device_id.reserve(len-2))
		{
			req.wLength = len;
			device_id.storage.ram->buf[len-2] = 0;
			res = std_request(&device_id.storage.ram->len);
			if(res == RES_OK)
			{
				device_id.storage.ram->len = len-2;
				len = strlen(device_id.storage.ram->buf);
				if(device_id.storage.ram->len > len)
					device_id.storage.ram->len = len;

			}

		} else
			res = RES_OUT_OF_MEMORY;
	}
    TRACELN("prn: read id=%x", res);
	return res;

}

RES_CODE usb_remote_prn_t::get_port_status()
{
	RES_CODE res;

	//read status
	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = USBPrinter_GET_PORT_STATUS;
	req.wIndex = (pid->bInterfaceNumber << 8) ; 	// Interface
	req.wValue = 0;			// 0
	req.wLength = 1;
	res = std_request(&port_status);
    TRACELN("prn: res=%x ps=%02X", res, port_status);

	return res;
}

RES_CODE usb_remote_prn_t::soft_reset()
{
	RES_CODE res;

    TRACELN1("prn: reset");

	req.bmRequestType = USB_REQ_OUT_CLASS_INTERFACE;
	req.bRequest = USBPrinter_SOFT_RESET;
	req.wIndex = (pid->bInterfaceNumber << 8) ; 	// Interface
	req.wValue = 0;			// 0
	req.wLength = 0;
	res = std_request(NULL);
	return res;
}

RES_CODE usb_remote_prn_t::scan_printer(uint32_t port_indx)
{
	RES_CODE res;

	// select EPT_0 and device address
	ep0_hnd->mode.as_bytes[0] = EPT_0; // RX pipe
	ep0_hnd->mode.as_bytes[1] = EPT_0; // TX pipe
	res = hdc_init(port_indx);
	if(res == RES_OK)
	{
		if(dev_descriptor.bDeviceClass == INTERFACE_DEFINED_CLASS)
		{
			//loop the configurations
			for(uint32_t cfg_indx=0; cfg_indx<dev_descriptor.bNumConfigurations; cfg_indx++)
			{
				res = get_config_descriptor(cfg_indx);
				if(res != RES_OK)
					break;

				//loop the interfaces
				for(uint32_t iface_indx=0; iface_indx<config_descriptor->bNumInterfaces; iface_indx++)
				{
					pid = usb_get_interface(config_descriptor, iface_indx);
					if(pid)
					{
						if(pid->bDeviceClass == PRINTER_DEVICE_CLASS &&
								pid->bDeviceSubClass == INTERFACE_PRINTER_SUBCLASS)
						{
							// set the configuration
							set_configuration(0);
							res = set_configuration(config_descriptor->bConfigurationValue);

							// set the interface
							if(res == RES_OK)
								res = set_interface(pid->bInterfaceNumber, pid->bAlternateSetting);

							// get device id
							if(res == RES_OK)
								res = get_device_id();

							if(res == RES_OK)
							{
								epts[0] = 0;
								epts[1] = 0;
								for(int i=0; i<pid->bNumEndpoints && i<2; i++)
								{
									USBEndpointDescriptor* ped;
									int config_fifo;

									ped = usb_get_enpoint(config_descriptor, i);
									if(!ped)
										return RES_ERROR;
									if((i+1)<pid->bNumEndpoints)
										config_fifo = 0;
									else
										config_fifo = 1;
									if(ped->bEndpointAddress & 0x80)
									{
										//IN
										epts[1] = ped->bEndpointAddress & 0x7F;
										ep0_hnd->mode.as_bytes[0] = epts[1]; // RX pipe
										ped->bEndpointAddress = 2;

									} else
									{
										//OUT
										epts[0] = ped->bEndpointAddress;
										ep0_hnd->mode.as_bytes[0] = epts[0]; // TX pipe
										ped->bEndpointAddress = 0x81;
									}
									usb_svc_configendpoint(ep0_hnd, &ped->as_generic, config_fifo);
									ep0_hnd->mode.as_bytes[0] = EPT_0;
								}
								return res;
							}
						}
					}
				}
			}
		}
	}

	if(res == RES_OK)
		res = RES_ERROR;
	return res;
}


