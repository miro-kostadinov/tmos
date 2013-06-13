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

RES_CODE hdc_request(USBGenericRequest* req, HANDLE hnd, void* ptr)
{
	RES_CODE res;

	res = hnd->tsk_write(req, sizeof(USBGenericRequest), USB_SETUP_WRITE_TOUT);
	if(res == RES_OK)
	{
		hnd->len = req->wLength;
		do
		{
			res = hnd->tsk_read(ptr, hnd->len, USB_SETUP_WRITE_TOUT);
			ptr = hnd->dst.as_voidptr;
		} while(hnd->len && res == RES_OK);

		if(hnd->mode1 && (res == RES_OK) && !hnd->len)
		{
			char buf[8];

			while(hnd->tsk_read(buf, sizeof(buf), USB_SETUP_WRITE_TOUT)== RES_OK)
			{
				if(!hnd->mode1)
					break;
			}
		}
	}
	return res;
}

RES_CODE hcd_device_descriptor(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t indx)
{
	USBGenericRequest req;
	usb_remote_device *device;
	RES_CODE res= RES_OK;

	//read descriptor
    TRACELN_USB("hcd: dev descriptor(%u)", indx);

	req.bmRequestType.dir = USB_REQUEST_DIR_IN;
	req.bmRequestType.recipient = USB_REQUEST_RECIPIENT_DEVICE;
	req.bmRequestType.type = USB_REQUEST_TYPE_STANDARD;
	req.bRequest = USBDeviceRequest_GET_DESCRIPTOR;
	req.wValue = DEVICE_DESCRIPTOR << 8;			// Request-specific value parameter.
	req.wIndex = 0;									// Request-specific index parameter

	device = &drv_info->drv_data->host_bus.usb_device[indx];
	if(device->dev_descriptor.bMaxPacketSize0 == 0)
	{
		req.wLength = 8;
		res = hdc_request(&req, hnd, &device->dev_descriptor);
	}
	if(res == RES_OK)
	{
		req.wLength = sizeof(USBDeviceDescriptor);
		res = hdc_request(&req, hnd, &device->dev_descriptor);
	}
	return res;
}

static RES_CODE hdc_request_config(USBGenericRequest* req, HANDLE hnd,
		usb_remote_device *device, uint32_t size)
{
	req->wLength = size;
	device->config_descriptor = (USBConfigurationDescriptor*) usr_realloc(
			device->config_descriptor, size);
	if(!device->config_descriptor)
		return RES_OUT_OF_MEMORY;

	return hdc_request(req, hnd, device->config_descriptor);
}

RES_CODE hcd_config_descriptor(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t indx)
{
	USBGenericRequest req;
	usb_remote_device *device;
	RES_CODE res;

	//read descriptor
    TRACELN_USB("hcd: cfg descriptor(%u)", indx);

	req.bmRequestType.dir = USB_REQUEST_DIR_IN;
	req.bmRequestType.recipient = USB_REQUEST_RECIPIENT_DEVICE;
	req.bmRequestType.type = USB_REQUEST_TYPE_STANDARD;
	req.bRequest = USBDeviceRequest_GET_DESCRIPTOR;
	req.wValue = CONFIGURATION_DESCRIPTOR << 8;		// Request-specific value parameter.
	req.wIndex = 0;									// Request-specific index parameter

	device = &drv_info->drv_data->host_bus.usb_device[indx];

	res = hdc_request_config(&req, hnd, device, sizeof(USBConfigurationDescriptor));

	if (res == RES_OK && device->config_descriptor->wTotalLength
					> sizeof(USBConfigurationDescriptor))
	{
		res = hdc_request_config(&req, hnd, device,
				device->config_descriptor->wTotalLength);
	}

	return res;
}

RES_CODE hcd_set_config(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t indx, uint32_t cfg)
{
	USBGenericRequest req;
	RES_CODE res;

	//read descriptor
    TRACELN_USB("hcd: cfg descriptor(%u)", indx);

	req.bmRequestType.dir = USB_REQUEST_DIR_OUT;
	req.bmRequestType.recipient = USB_REQUEST_RECIPIENT_DEVICE;
	req.bmRequestType.type = USB_REQUEST_TYPE_STANDARD;
	req.bRequest = USBDeviceRequest_SET_CONFIGURATION;
	req.wValue = cfg;		// Request-specific value parameter.
	req.wIndex = 0;			// Request-specific index parameter
	req.wLength = 0;

	res = hdc_request(&req, hnd, NULL);
	return res;
}



RES_CODE hcd_set_address(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t indx)
{
	USBGenericRequest req;
	RES_CODE res;

    TRACELN_USB("hcd: set adr (%u)=%u", indx, indx+1);

	req.bmRequestType.dir = USB_REQUEST_DIR_OUT;
	req.bmRequestType.recipient = USB_REQUEST_RECIPIENT_DEVICE;
	req.bmRequestType.type = USB_REQUEST_TYPE_STANDARD;
	req.bRequest = USBDeviceRequest_SET_ADDRESS;
	req.wValue = indx+1;	// Request-specific value parameter.
	req.wIndex = 0;			// Request-specific index parameter
	req.wLength = 0;

	res = hdc_request(&req, hnd, NULL);
//	res = hnd->tsk_write(&req, sizeof(USBGenericRequest), USB_SETUP_WRITE_TOUT);
	tsk_sleep(10);
	if(res == RES_OK)
	{
		drv_info->drv_data->host_bus.usb_device[indx].dev_adress = indx+1;
	}


	return res;
}

void usb_get_class_name(CSTRING& s, USBClassCode code)
{
	switch(code)
	{
	case INTERFACE_DEFINED_CLASS:
		s = "interface defined";
		break;
	case AUDIO_DEVICE_CLASS:
		s = "audio";
		break;
	case CDC_DEVICE_CLASS:
		s = "CDC";
		break;
	case HID_DEVICE_CLASS:
		s = "HID";
		break;
	case PHYSICAL_DEVICE_CLASS:
		s = "PHY";
		break;
	case IMAGE_DEVICE_CLASS:
		s = "Image";
		break;
	case PRINTER_DEVICE_CLASS:
		s = "printer";
		break;
	case MASSSTORAGE_DEVICE_CLASS:
		s = "massstorage";
		break;
	case HUB_DEVICE_CLASS:
		s = "hub";
		break;
	case CDCDATA_DEVICE_CLASS:
		s = "cdcd";
		break;
	case SMARTCARD_DEVICE_CLASS:
		s = "smart";
		break;
	case VIDEO_DEVICE_CLASS:
		s = "video";
		break;

	case WIRELESSCONTROLLER_CLASS:
		s = "wireless";
		break;
	case APPLICATION_SPECIFIC_CLASS:
		s = "APP";
		break;
	case VENDOR_SPECIFIC_CLASS:
		s = "vendor";
		break;
	default:
		s.format("unknown (%02x)", code);
		break;
	}
}

USBInterfaceDescriptor* usb_get_interface(const USBConfigurationDescriptor* des,
		int interface)
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

			if (pd->GetType() == INTERFACE_DESCRIPTOR)
			{
				if (!interface--)
				{

					return (USBInterfaceDescriptor*)pd;
				}
			}
		}
	}

	return NULL;
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

RES_CODE usb_get_interface_class_name(CSTRING& s,
		const USBConfigurationDescriptor* des, int interface)
{
	USBInterfaceDescriptor* pi;

	pi =  usb_get_interface(des, interface);
	if(pi)
	{
		usb_get_class_name(s,pi->bDeviceClass);
		return RES_OK;
	}
	return RES_ERROR;
}

static int trace_usb_descriptor(USBGenericDescriptor* des);

static void trace_usb_class(USBClassCode code, const char* label)
{
	CSTRING s;

	usb_get_class_name(s, code);
	TRACELN1("");
	TRACE1_USB(label);
	TRACE1_USB(s.c_str());
}
static int trace_usb_device_descriptor(USBDeviceDescriptor* des)
{
	trace_usb_class(des->bDeviceClass, "\tbDeviceClass=");

	TRACELN_USB("\tvid=%04x pid=%04x", des->idVendor, des->idProduct);
	TRACELN_USB("\tusb=%04x %u configurations", des->bcdUSB, des->bNumConfigurations);
	return sizeof(USBDeviceDescriptor);
}

static int trace_usb_config_descriptor(USBConfigurationDescriptor* des)
{
	int len;

	TRACELN_USB("\twTotalLength=%u", des->wTotalLength);
	TRACELN_USB("\tbNumInterfaces=%u", des->bNumInterfaces);
	TRACELN_USB("\tval=%u pwr=%umA", des->bConfigurationValue, des->bMaxPower*2);

	len = des->wTotalLength - sizeof(USBConfigurationDescriptor);
	USBGenericDescriptor* gen = &des->as_generic;
	while(len > 0)
	{
		gen = gen->GetNextDescriptor();
		len -= trace_usb_descriptor(gen);
	}
	return des->wTotalLength;
}

static int trace_usb_interface_descriptor(USBInterfaceDescriptor* des)
{
	TRACELN_USB("\tbInterfaceNumber=%u", des->bInterfaceNumber);
	TRACELN_USB("\tbAlternateSetting=%u", des->bAlternateSetting);
	TRACELN_USB("\tbNumEndpoints=%u", des->bNumEndpoints);
	TRACELN_USB("\tiInterface=%u", des->iInterface);
	trace_usb_class(des->bDeviceClass, "\tbDeviceClass=");
	return sizeof(USBInterfaceDescriptor);
}

static int trace_usb_endpoit_descriptor(USBEndpointDescriptor* des)
{
	TRACELN_USB("\tbEndpointAddress=%x", des->bEndpointAddress);
	TRACELN_USB("\tbmAttributes=%u", des->bmAttributes);
	TRACELN_USB("\twMaxPacketSize=%u", des->wMaxPacketSize);
	TRACELN_USB("\tbInterval=%u", des->bInterval);
	return sizeof(USBEndpointDescriptor);
}

static int trace_usb_descriptor(USBGenericDescriptor* des)
{
	int res = des->bLength;

	if(res < (int)sizeof(USBGenericDescriptor))
		res = sizeof(USBGenericDescriptor);

	TRACELN_USB("-----------------\r\nbLength=%d", des->bLength);
	switch(des->bDescriptorType)
	{
	case DEVICE_DESCRIPTOR:
		TRACELN1_USB("DEVICE_DESCRIPTOR");
		res = trace_usb_device_descriptor((USBDeviceDescriptor*) des);
		break;
	case CONFIGURATION_DESCRIPTOR:
		TRACELN1_USB("CONFIGURATION_DESCRIPTOR");
		res = trace_usb_config_descriptor((USBConfigurationDescriptor*) des);
		break;
	case STRING_DESCRIPTOR:
		TRACELN1_USB("STRING_DESCRIPTOR");
		break;
	case INTERFACE_DESCRIPTOR:
		TRACELN1_USB("INTERFACE_DESCRIPTOR");
		res = trace_usb_interface_descriptor((USBInterfaceDescriptor*) des);
		break;
	case ENDPOINT_DESCRIPTOR:
		TRACELN1_USB("ENDPOINT_DESCRIPTOR");
		res = trace_usb_endpoit_descriptor((USBEndpointDescriptor*) des);
		break;
	case DEVICEQUALIFIER_DESCRIPTOR:
		TRACELN1_USB("DEVICEQUALIFIER_DESCRIPTOR");
		break;
	case OTHERSPEEDCONFIGURATION_DESCRIPTOR:
		TRACELN1_USB("OTHERSPEEDCONFIGURATION_DESCRIPTOR");
		break;
	case INTERFACEPOWER_DESCRIPTOR:
		TRACELN1_USB("INTERFACEPOWER_DESCRIPTOR");
		break;
	case OTG_DESCRIPTOR:
		TRACELN1_USB("OTG_DESCRIPTOR");
		break;
	case HID_DESCRIPTOR:
		TRACELN1_USB("HID_DESCRIPTOR");
		break;
	case PHYSICAL_DESCRIPTOR:
		TRACELN1_USB("PHY_DESCRIPTOR");
		break;
	case CDC_SPECIFIC_DESCRIPTOR:
		TRACELN1_USB("CDC_SPECIFIC_DESCRIPTOR");
		break;
	case HUB_DESCRIPTOR:
		TRACELN1_USB("HUB_DESCRIPTOR");
		break;
	default:
		TRACELN1_USB("UNKNOWN DESCRIPTOR");
		break;
	}
	return res;
}

static void trace_usb_dev(usb_remote_device *device)
{
	TRACELN1_USB("USBH Found:");

	trace_usb_descriptor(&device->dev_descriptor.as_generic);
	if(device->config_descriptor)
	{
		trace_usb_descriptor(&device->config_descriptor->as_generic);
	} else
	{
		TRACELN1_USB("no cfg des!");
	}
}

RES_CODE usb_host_reset_bus(USB_DRV_INFO drv_info, HANDLE hnd)
{
	unsigned int res = RES_ERROR;

	// clear reset request and assert it
	__disable_irq();
	usb_otg_clr_flags(drv_info, USB_OTG_FLG_HOST_RST);
	__enable_irq();

	if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST)
	{
		drv_info->hw_base->USBPOWER |= USB_USBPOWER_RESET;
		// deassert reset
		tsk_sleep(20);
		drv_info->hw_base->USBPOWER &= ~USB_USBPOWER_RESET;
		tsk_sleep(10);

		// if everything OK -> enumerate
		if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST)
		{
			uint32_t cnt;
			//read device descriptor

			cnt = hnd->mode.as_ushort[1];
			hnd->mode.as_ushort[1] = drv_info->drv_data->otg_state_cnt;
			hnd->mode0 = 0;	//device 0
			res = hcd_device_descriptor(drv_info, hnd, 0);

			if(res == RES_OK)
			{
				res = hcd_set_address(drv_info, hnd, 0);
				if(res == RES_OK)
				{
					res = hcd_config_descriptor(drv_info, hnd, 0);
					if(res == RES_OK)
					{
						__disable_irq();
						usb_otg_set_flags(drv_info, USB_OTG_FLG_HOST_OK);
						__enable_irq();

						trace_usb_dev(&drv_info->drv_data->host_bus.usb_device[0]);

					}
				}
			}
			hnd->mode.as_ushort[1] = cnt;
		}
	}
	return res;
}


