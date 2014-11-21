/*
 * usb_setup.cpp
 *
 *  Created on: 04.07.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <usb_setup.h>

/** Device product ID. */
#define CDCDSerialDriverDescriptors_PRODUCTID       0x6119
/** Device vendor ID (Atmel). */
#define CDCDSerialDriverDescriptors_VENDORID        0x03EB
/** Device release number. */
#define CDCDSerialDriverDescriptors_RELEASE         0x0100

/** Standard USB device descriptor for the CDC serial driver */
const USBDeviceDescriptor ron200_full_speed_descriptor =
{
	//generic descriptor
	{
	    sizeof(USBDeviceDescriptor),
	    DEVICE_DESCRIPTOR,
	},
    USBDeviceDescriptor_USB2_00,
    CDC_DEVICE_CLASS,
    INTERFACE_DEFINED_SUBCLASS,
    INTERFACE_DEFINED_PROTOCOL,
    64, //Endpoint 0 MAXPACKETSIZE
    CDCDSerialDriverDescriptors_VENDORID,
    CDCDSerialDriverDescriptors_PRODUCTID,
    CDCDSerialDriverDescriptors_RELEASE,
    0, /* No string descriptor for manufacturer */
    1, /* Index of product string descriptor is #1 */
    0, /* No string descriptor for serial number */
    1 /* Device has 1 possible configuration */
};

/** Standard USB configuration descriptor for the CDC serial driver */
const CDCDSerialDriverConfigurationDescriptorsOTG configurationDescriptorsFS =
{

	/* Standard configuration descriptor */
	{
		{
			sizeof(USBConfigurationDescriptor),
			CONFIGURATION_DESCRIPTOR
		},
		sizeof(CDCDSerialDriverConfigurationDescriptorsOTG),
		2, /* There are two interfaces in this configuration */
		1, /* This is configuration #1 */
		0, /* No string descriptor for this configuration */
		USBConfigurationDescriptor_SELFPOWERED_RWAKEUP,
		USBConfigurationDescriptor_POWER(100)
	},
	/* OTG descriptor */
	{
		{
			sizeof(USBOtgDescriptor),
			OTG_DESCRIPTOR
		},
		HNP_SRP_SUPPORT
	},
    /* Communication class interface standard descriptor */
    {
    	{
			sizeof(USBInterfaceDescriptor),
			INTERFACE_DESCRIPTOR
    	},

        0, /* This is interface #0 */
        0, /* This is alternate setting #0 for this interface */
        1, /* This interface uses 1 endpoint */
        CDC_DEVICE_CLASS,
        CDC_ACM_SUBCLASS,
        INTERFACE_DEFINED_PROTOCOL,
        0  /* No string descriptor for this interface */
    },
    /* Class-specific header functional descriptor */
    {
    	{
			sizeof(CDCHeaderDescriptor),
			CDC_SPECIFIC_DESCRIPTOR
    	},
    	CDC_HEADER_DESCRIPTOR,
    	CDC_VERSION_1_10
    },
    /* Class-specific call management functional descriptor */
    {
        {
        	sizeof(CDCCallManagementDescriptor),
        	CDC_SPECIFIC_DESCRIPTOR
        },
        CDC_CALL_MANAGEMENT_DESCRIPTOR,
        CDCCallManagementDescriptor_SELFCALLMANAGEMENT,
        0 /* No associated data interface */
    },
    /* Class-specific abstract control management functional descriptor */
    {
   		{
			sizeof(CDCAbstractControlManagementDescriptor),
			CDC_SPECIFIC_DESCRIPTOR
   		},
   		CDC_ABSTRACT_CONTROL_MANAGEMENT_DESCRIPTOR,
        CDCAbstractControlManagementDescriptor_LINE
    },
    /* Class-specific union functional descriptor with one slave interface */
    {
		{
			sizeof(CDCUnionDescriptor),
			CDC_SPECIFIC_DESCRIPTOR
		},
		CDC_UNION_DESCRIPTOR,
        0, /* Number of master interface is #0 */
        1 /* First slave interface is #1 */
    },
    /* Notification endpoint standard descriptor */
    {
        {
			sizeof(USBEndpointDescriptor),
			ENDPOINT_DESCRIPTOR
        },
        USBEndpointDescriptor_ADDRESS(ENDPOINT_DIRECTION_IN, EPT_3),
        ENDPOINT_TYPE_INTERRUPT,
        USBEndpointDescriptor_MAXINTERRUPTSIZE_FS,
        10 /* Endpoint is polled every 10ms */
    },
    /* Data class interface standard descriptor */
    {
		{
			sizeof(USBInterfaceDescriptor),
			INTERFACE_DESCRIPTOR
		},
        1, /* This is interface #1 */
        0, /* This is alternate setting #0 for this interface */
        2, /* This interface uses 2 endpoints */
        CDCDATA_DEVICE_CLASS,
        INTERFACE_DEFINED_SUBCLASS,
        INTERFACE_DEFINED_PROTOCOL,
        0  /* No string descriptor for this interface */
    },
    /* Bulk-OUT endpoint standard descriptor */
    {
		{
			sizeof(USBEndpointDescriptor),
			ENDPOINT_DESCRIPTOR
		},
        USBEndpointDescriptor_ADDRESS(ENDPOINT_DIRECTION_OUT, EPT_1),
        ENDPOINT_TYPE_BULK,
        USBEndpointDescriptor_MAXBULKSIZE_FS,
        0 /* Must be 0 for full-speed bulk endpoints */
    },
    /* Bulk-IN endpoint descriptor */
    {
        {
			sizeof(USBEndpointDescriptor),
			ENDPOINT_DESCRIPTOR
        },
        USBEndpointDescriptor_ADDRESS(ENDPOINT_DIRECTION_IN, EPT_2),
        ENDPOINT_TYPE_BULK,
        USBEndpointDescriptor_MAXBULKSIZE_FS,
        0 /* Must be 0 for full-speed bulk endpoints */
    }
};

/** Language ID string descriptor */
const unsigned char languageIdStringDescriptor[] = {

    USBStringDescriptor_LENGTH(1),
    STRING_DESCRIPTOR,
    USBStringDescriptor_ENGLISH_US
};


/** Product string descriptor */
const unsigned char productStringDescriptor[] = {

    USBStringDescriptor_LENGTH(13),
    STRING_DESCRIPTOR,
    USBStringDescriptor_UNICODE('A'),
    USBStringDescriptor_UNICODE('T'),
    USBStringDescriptor_UNICODE('9'),
    USBStringDescriptor_UNICODE('1'),
    USBStringDescriptor_UNICODE('U'),
    USBStringDescriptor_UNICODE('S'),
    USBStringDescriptor_UNICODE('B'),
    USBStringDescriptor_UNICODE('S'),
    USBStringDescriptor_UNICODE('e'),
    USBStringDescriptor_UNICODE('r'),
    USBStringDescriptor_UNICODE('i'),
    USBStringDescriptor_UNICODE('a'),
    USBStringDescriptor_UNICODE('l')
};

/** List of string descriptors used by the device */
const unsigned char *stringDescriptors[] = {

    languageIdStringDescriptor,
    productStringDescriptor,
};

/** List of standard descriptors for the serial driver. */
const USBDDriverDescriptors ron200_descriptors =
{
    &ron200_full_speed_descriptor,
    (USBConfigurationDescriptor *) &(configurationDescriptorsFS),
    0, /* No full-speed device qualifier descriptor */
    0, /* No full-speed other speed configuration */
    0, /* No high-speed device descriptor */
    0, /* No high-speed configuration descriptor */
    0, /* No high-speed device qualifier descriptor */
    0, /* No high-speed other speed configuration descriptor */
    stringDescriptors,
    2 /* 2 string descriptors in list */
};
