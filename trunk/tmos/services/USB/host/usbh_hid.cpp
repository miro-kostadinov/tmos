/*
 * usbh_hid.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: miro
 */

#include <tmos.h>
#include <usbh_hid.h>
#include <usb_descriptors.h>
#include <usb_svc.h>

//------------------------------------------------------------------------------
//	Standard Requests
// 		Note: HID is using Interface recipient and Standard type for GET_DESCRIPTOR
//------------------------------------------------------------------------------
RES_CODE usb_remote_hid_t::get_hid_report_descriptor(void* buf, uint32_t len)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_IN_STANDARD_INTERFACE;
	req.bRequest = USBDeviceRequest_GET_DESCRIPTOR;
	req.wValue = (HIDREPORT_DESCRIPTOR << 8) | 0;		// Descriptor type and index
	req.wIndex = pid->bInterfaceNumber;					// Interface number
	req.wLength = len;

	res = std_request(buf);
	return res;
}

RES_CODE usb_remote_hid_t::get_hid_descriptor(void* buf, uint32_t len)
{
	RES_CODE res;

	// HID is using Interface recipient and Standard type for GET_DESCRIPTOR
	req.bmRequestType = USB_REQ_IN_STANDARD_INTERFACE;
	req.bRequest = USBDeviceRequest_GET_DESCRIPTOR;
	req.wValue = (HID_DESCRIPTOR << 8) | 0;				// Descriptor type and index
	req.wIndex = pid->bInterfaceNumber;					// Interface number
	req.wLength = len;

	res = std_request(buf);
	return res;
}

//------------------------------------------------------------------------------
//	Class Requests
//------------------------------------------------------------------------------
RES_CODE usb_remote_hid_t::get_report(void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id)
{
	RES_CODE res;

	//read status
    TRACELN1("hid: get report");

	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = HIDRequest_GET_REPORT;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = (rep_type << 8) | rep_id;			// HID_REPORTTYPE_INPUT / OUTPUT / FEATURE
	req.wLength = len;
	res = std_request(buf);
	return res;
}

RES_CODE usb_remote_hid_t::set_report(const void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id)
{
	RES_CODE res;

	//read status
    TRACELN1("hid: get report");

	req.bmRequestType = USB_REQ_OUT_CLASS_INTERFACE;
	req.bRequest = HIDRequest_SET_REPORT;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = (rep_type << 8) | rep_id;			// HID_REPORTTYPE_INPUT / OUTPUT / FEATURE
	req.wLength = len;
	res = std_request((void*)buf);
	return res;
}

RES_CODE usb_remote_hid_t::get_idle(uint32_t rep_id)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = HIDRequest_GET_IDLE;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = (0 << 8) | rep_id;					//
	req.wLength = 1;
	res = std_request(&hid_idle);
	return res;
}

RES_CODE usb_remote_hid_t::set_idle(uint32_t duration, uint32_t rep_id)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_OUT_CLASS_INTERFACE;
	req.bRequest = HIDRequest_SET_IDLE;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = (duration << 8) | rep_id;			// duration in 4ms units
	req.wLength = 0;
	res = std_request(NULL);
	if(res == RES_OK)
		hid_idle = duration;
	return res;
}

RES_CODE usb_remote_hid_t::get_protocol(void* buf)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = HIDRequest_GET_PROTOCOL;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = 0;									//
	req.wLength = 1;
	res = std_request(buf);
	return res;
}

RES_CODE usb_remote_hid_t::set_protocol(uint32_t protocol)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_OUT_CLASS_INTERFACE;
	req.bRequest = HIDRequest_SET_PROTOCOL;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = protocol;							// 0 = Boot Protocol; 1 = Report Protocol
	req.wLength = 0;
	res = std_request(NULL);
	return res;
}

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------

RES_CODE usb_remote_hid_t::scan_hid(uint32_t port_indx, USBSubClassCode subcls, USBProtocolCode proto)
{
	RES_CODE res;

	// select EPT_0 and device address
	ep0_hnd->mode.as_bytes[0] = EPT_0; // RX pipe
	ep0_hnd->mode.as_bytes[1] = EPT_0; // TX pipe
	res = hdc_init(port_indx);
	if(res == RES_OK)
	{
		epi_hnd->mode0 = ep0_hnd->mode0;  // device hub port
		// TRACE Device descriptor
		TRACELN("HID: dev found %x:%x", dev_descriptor.idVendor, dev_descriptor.idProduct);
		trace_usb_descriptor(&dev_descriptor.as_generic);
		if(dev_descriptor.bDeviceClass == INTERFACE_DEFINED_CLASS)
		{
			//loop the configurations
			for(uint32_t cfg_indx=0; cfg_indx<dev_descriptor.bNumConfigurations; cfg_indx++)
			{
				res = get_config_descriptor(cfg_indx);
				if(res != RES_OK)
					break;

				// TRACE Configuration descriptor(s)
				trace_usb_descriptor(&config_descriptor->as_generic);

				//loop the interfaces
				for(uint32_t iface_indx=0; iface_indx<config_descriptor->bNumInterfaces; iface_indx++)
				{
					pid = usb_get_interface(config_descriptor, iface_indx);
					if(pid && pid->bDeviceClass == HID_DEVICE_CLASS &&
							pid->bDeviceSubClass == subcls && pid->bDeviceProtocol == proto)
					{
						// set the configuration
						set_configuration(0);
						res = set_configuration(config_descriptor->bConfigurationValue);

						if(res == RES_OK)
						{
							epi_hnd->mode.as_bytes[0] = EPT_0;
							epi_hnd->mode.as_bytes[1] = EPT_0;
							for(int i=0; i<pid->bNumEndpoints && i<2; i++)
							{
								USBEndpointDescriptor* ped;

								ped = usb_get_enpoint(config_descriptor, i);
								if(ped && ped->bmAttributes == ENDPOINT_TYPE_INTERRUPT)
								{
									if( ped->bEndpointAddress & 0x80 )
									{
										if(epi_hnd->mode.as_bytes[0] == EPT_0)
										{
											epi_hnd->mode.as_bytes[0] = ped->bEndpointAddress & 0x7F;
											epi_hnd->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
											usb_svc_configendpoint(epi_hnd, &ped->as_generic);
										}
									} else
									{
										if(epi_hnd->mode.as_bytes[1] == EPT_0)
										{
											epi_hnd->mode.as_bytes[1] = ped->bEndpointAddress ;
											epi_hnd->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
											usb_svc_configendpoint(epi_hnd, &ped->as_generic);
										}
									}
								}
							}
							hid_idle = 255;
							get_idle(HID_REPORT_ALL);
							return RES_OK;

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


