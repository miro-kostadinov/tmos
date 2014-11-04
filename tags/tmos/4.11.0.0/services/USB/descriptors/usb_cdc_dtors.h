/*
 * usb_cdc_dtors.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_CDC_DTORS_H_
#define USB_CDC_DTORS_H_

#include <usb_std_dtors.h>

/// USB CDC Sub Class Codes
enum USBCDCSubClassCode : unsigned char
{
	CDC_DLCM_SUBCLASS		 	= 0x01,	//!< CDC Direct Line Control Model
	CDC_ACM_SUBCLASS 			= 0x02,	//!< CDC Abstract Control Model
	CDC_TCM_SUBCLASS 			= 0x03,	//!< CDC Telephone Control Model
	CDC_MCCM_SUBCLASS 			= 0x04,	//!< CDC Multi-Channel Control Model
	CDC_CAPI_SUBCLASS 			= 0x05,	//!< CDC CAPI Control Model
	CDC_ETHERNET_SUBCLASS 		= 0x06,	//!< CDC Ethernet Networking Control Model
	CDC_ATM_SUBCLASS 			= 0x07,	//!< CDC ATM Networking Control Model
};

/// CDC class specific descriptor types
enum CDCCS_FUNCTION_TYPES : unsigned char
{
	CDCCS_INTERFACE   	= 0x24, //!< Indicates that a CDC descriptor applies to an interface
	CDCCS_ENDPOINY		= 0x25	//!< Indicates that a CDC descriptor applies to an endpoint
};

/// CDC class specific descriptor subtypes
enum CDC_DESCRIPTOR_SUBTYPE : unsigned char
{
	CDC_HEADER_DESCRIPTOR   					= 0x00, //!< Header Functional Descriptor
	CDC_CALL_MANAGEMENT_DESCRIPTOR				= 0x01,	//!< Call management functional descriptor
	CDC_ABSTRACT_CONTROL_MANAGEMENT_DESCRIPTOR	= 0x02,	//!< Abstract control managementfunctional descriptor
	CDC_DIRECT_LINE_MANAGEMENT_DESCRIPTOR		= 0x03,	//!< Direct Line management functional descriptor
	CDC_TELEPHONE_RINGER_DESCRIPTOR				= 0x04,	//!< Telephone Ringer Functional descriptor
	CDC_TELEPHONE_CALL_REPORTING_DESCRIPTOR		= 0x05,	//!< Telephone Call and Line State Reporting Capabilities Functional Descriptor.
	CDC_UNION_DESCRIPTOR						= 0x06	//!< Union Functional descriptor
	//more
};


//------------------------------------------------------------------------------
//     				CDCHeaderDescriptor (0x24, 0x00)
//------------------------------------------------------------------------------

/// CDC class specific descriptor subtypes
enum CDC_STANDARD_VERSION : unsigned short
{
	CDC_VERSION_1_10   				= 0x0110, //!< Identify CDC specification version 1.10.
};

/// CDCHeaderDescriptor
/// Marks the beginning of the concatenated set of functional descriptors
///      for the interface.
struct CDCHeaderDescriptor
{
	USBGenericDescriptor as_generic;			//!< Generic Descriptor
	CDC_DESCRIPTOR_SUBTYPE bDescriptorSubtype;	//!< Descriptor sub-type
    CDC_STANDARD_VERSION bcdCDC;				//!< USB CDC specification release number
} __attribute__ ((packed)); /* GCC */

//------------------------------------------------------------------------------
//     				CDCUnionDescriptor (0x24, 0x06)
//------------------------------------------------------------------------------

/// CDCUnionDescriptor
/// Describes the relationship between a group of interfaces that can
///       be considered to form a functional unit.
struct CDCUnionDescriptor
{
	USBGenericDescriptor as_generic;			//!< Generic Descriptor
	CDC_DESCRIPTOR_SUBTYPE bDescriptorSubtype;	//!< Descriptor sub-type
    uint8_t bMasterInterface;					//!< Number of the master interface for this union
    uint8_t bSlaveInterface0;					//!< Number of the first slave interface for this union
} __attribute__ ((packed)); /* GCC */

//------------------------------------------------------------------------------
//     				CDCCallManagementDescriptor (0x24, 0x01)
//------------------------------------------------------------------------------
/** Device handles call management itself. */
#define CDCCallManagementDescriptor_SELFCALLMANAGEMENT      (1 << 0)
/** Device can exchange call management information over a Data class interface.
 */
#define CDCCallManagementDescriptor_DATACALLMANAGEMENT      (1 << 1)

/**
 * CDCCallManagementDescriptor
 * Describes the processing of calls for the communication class
 *        interface.
 */
struct CDCCallManagementDescriptor
{
	USBGenericDescriptor as_generic;			//!< Generic Descriptor
	CDC_DESCRIPTOR_SUBTYPE bDescriptorSubtype;	//!< Descriptor sub-type
    /** Configuration capabilities
        \sa usb_cdc_cb_man_desc CDC CallManagement Capabilities. */
    uint8_t bmCapabilities;
    /** Interface number of the data class interface used for call management
        (optional). */
    uint8_t bDataInterface;

} __attribute__ ((packed)); /* GCC */

//------------------------------------------------------------------------------
//     	CDCAbstractControlManagementDescriptor (0x24, 0x02)
//------------------------------------------------------------------------------
/** Device supports the request combination of SetCommFeature, ClearCommFeature
    and GetCommFeature. */
#define CDCAbstractControlManagementDescriptor_COMMFEATURE          (1 << 0)
/** Device supports the request combination of SetLineCoding, GetLineCoding and
    SetControlLineState. */
#define CDCAbstractControlManagementDescriptor_LINE                 (1 << 1)
/** Device supports the SendBreak request. */
#define CDCAbstractControlManagementDescriptor_SENDBREAK            (1 << 2)
/** Device supports the NetworkConnection notification. */
#define CDCAbstractControlManagementDescriptor_NETWORKCONNECTION    (1 << 3)

/**
 * CDCAbstractControlManagementDescriptor
 * Describes the command supported by the communication interface class
 *        with the Abstract Control Model subclass code.
 */
struct CDCAbstractControlManagementDescriptor
{
	USBGenericDescriptor as_generic;			//!< Generic Descriptor
	CDC_DESCRIPTOR_SUBTYPE bDescriptorSubtype;	//!< Descriptor sub-type
    /** Configuration capabilities.
        \sa usb_cdc_acm CDC ACM Capabilities. */
    uint8_t bmCapabilities;

} __attribute__ ((packed)); /* GCC */


/**
 * \typedef CDCDSerialDriverConfigurationDescriptors
 * \brief Configuration descriptor list for a device implementing a
 *        CDC serial driver.
 */
struct CDCDSerialDriverConfigurationDescriptors
{
    /** Standard configuration descriptor. */
    USBConfigurationDescriptor configuration;
    /** Communication interface descriptor. */
    USBInterfaceDescriptor  communication;
    /** CDC header functional descriptor. */
    CDCHeaderDescriptor header;
    /** CDC call management functional descriptor. */
    CDCCallManagementDescriptor callManagement;
    /** CDC abstract control management functional descriptor. */
    CDCAbstractControlManagementDescriptor abstractControlManagement;
    /** CDC union functional descriptor (with one slave interface). */
    CDCUnionDescriptor union1;
    /** Notification endpoint descriptor. */
    USBEndpointDescriptor notification;
    /** Data interface descriptor. */
    USBInterfaceDescriptor data;
    /** Data OUT endpoint descriptor. */
    USBEndpointDescriptor dataOut;
    /** Data IN endpoint descriptor. */
    USBEndpointDescriptor dataIn;

} __attribute__ ((packed));

/**
 * \typedef CDCDSerialDriverConfigurationDescriptorsOTG
 * \brief Configuration descriptor list for a device implementing a
 *        CDC serial OTG driver.
 */
struct CDCDSerialDriverConfigurationDescriptorsOTG
{

    /** Standard configuration descriptor. */
    USBConfigurationDescriptor configuration;
    /* OTG descriptor */
    USBOtgDescriptor otgDescriptor;
    /** Communication interface descriptor. */
    USBInterfaceDescriptor  communication;
    /** CDC header functional descriptor. */
    CDCHeaderDescriptor header;
    /** CDC call management functional descriptor. */
    CDCCallManagementDescriptor callManagement;
    /** CDC abstract control management functional descriptor. */
    CDCAbstractControlManagementDescriptor abstractControlManagement;
    /** CDC union functional descriptor (with one slave interface). */
    CDCUnionDescriptor union1;
    /** Notification endpoint descriptor. */
    USBEndpointDescriptor notification;
    /** Data interface descriptor. */
    USBInterfaceDescriptor data;
    /** Data OUT endpoint descriptor. */
    USBEndpointDescriptor dataOut;
    /** Data IN endpoint descriptor. */
    USBEndpointDescriptor dataIn;

} __attribute__ ((packed));



#endif /* USB_CDC_DTORS_H_ */
