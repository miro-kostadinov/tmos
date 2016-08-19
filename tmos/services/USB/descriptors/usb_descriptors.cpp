/*
 * usb_descriptors.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#include <tmos.h>
#include <usb_descriptors.h>

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

static void trace_usb_class(USBClassCode code, const char* label)
{
	CSTRING s;

	usb_get_class_name(s, code);
	TRACELN1_USB("");
	TRACE1_USB(label);
	TRACE1_USB(s.c_str());
}

#if USB_ENABLE_HID
static void trace_usb_hid_descriptor(USBHIDDescriptor* des)
{
	TRACELN_USB("\thid=%04x", des->bcdHID);
	TRACELN_USB("\tcountry=%u", des->bCountryCode);
	TRACELN_USB("\tdtors=%u", des->bNumDescriptors);
	for(int i=0; i < des->bNumDescriptors; i++)
	{
		TRACELN1_USB("\tdtor=");
		switch(des->hid_descriptors[i].bDescriptorType)
		{
		case HIDREPORT_DESCRIPTOR:
			TRACE1_USB("HID_REPORT");
			break;
		case PHYSICAL_DESCRIPTOR:
			TRACE1_USB("PHY_DTOR");
			break;
		default:
			TRACE_USB("%u", des->hid_descriptors[i].bDescriptorType);
			break;
		}
		TRACE_USB(" %u bytes", des->hid_descriptors[i].wDescriptorLength);

	}

}
#endif

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

	TRACELN1_USB("\tSubClass=");
	switch(des->bDeviceClass)
	{
#if USB_ENABLE_CDC
	case CDC_DEVICE_CLASS:
		switch(des->bDeviceSubClass)
		{
		case CDC_DLCM_SUBCLASS:
			TRACE1_USB("CDC_DLCM");
			break;
		case CDC_ACM_SUBCLASS:
			TRACE1_USB("CDC_ACM");
			break;
		case CDC_TCM_SUBCLASS:
			TRACE1_USB("CDC_TCM");
			break;
		case CDC_MCCM_SUBCLASS:
			TRACE1_USB("CDC_MCCM");
			break;
		case CDC_CAPI_SUBCLASS:
			TRACE1_USB("CDC_CAPI");
			break;
		case CDC_ETHERNET_SUBCLASS:
			TRACE1_USB("CDC_ETHERNET");
			break;
		case CDC_ATM_SUBCLASS:
			TRACE1_USB("CDC_ATM");
			break;
		case VENDOR_SPECIFIC_SUBCLASS:
			TRACE1_USB("vendor");
			break;
		default:
			TRACE_USB("%u", des->bDeviceSubClass);
			break;
		}
		TRACELN_USB("\tProto=", des->bDeviceProtocol);
		break;
#endif

#if USB_ENABLE_HID
	case HID_DEVICE_CLASS:
		if(des->bDeviceSubClass == 1)
			TRACE1_USB("BOOT");
		else
			TRACE_USB("%u", des->bDeviceSubClass);
		switch(des->bDeviceProtocol)
		{
		case 1:
			TRACELN1_USB("\tProto=keyboard");
			break;
		case 2:
			TRACELN1_USB("\tProto=mouse");
			break;
		default:
			TRACELN_USB("\tProto=%u", des->bDeviceProtocol);
			break;
		}
		break;
#endif

#if USB_ENABLE_MSC
		case MASSSTORAGE_DEVICE_CLASS:
			if(des->bDeviceSubClass == INTERFACE_MSC_SUBCLASS_SCSI_T)
				TRACE1_USB("SCSI");
			else
				TRACE_USB("%u", des->bDeviceSubClass);
			if(des->bDeviceProtocol == INTERFACE_MSC_PROTOCOL_BBB)
				TRACE1_USB("\tProto=BBB");
			else
				TRACE_USB("\tProto=%02x", des->bDeviceProtocol);
			break;
#endif

	default:
		TRACE_USB("%u", des->bDeviceSubClass);
		TRACELN_USB("\tProto=%02x", des->bDeviceProtocol);
		break;
	}
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

int trace_usb_descriptor(USBGenericDescriptor* des)
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
#if USB_ENABLE_HID
	case HID_DESCRIPTOR:
		TRACELN1_USB("HID_DESCRIPTOR");
		trace_usb_hid_descriptor( (USBHIDDescriptor*)des);
		break;
#endif
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
	tsk_sleep(5);
	return res;
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

