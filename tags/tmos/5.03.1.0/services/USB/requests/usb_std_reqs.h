/*
 * usb_std_reqs.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_STD_REQS_H_
#define USB_STD_REQS_H_

#include <tmos.h>
#include <usb_opt.h>

//------------------------------------------------------------------------------
//         				USBGenericRequest
//------------------------------------------------------------------------------

#define USB_REQ_DIR					0x80	//!< Request Data transfer direction
#define USB_REQ_DIR_OUT				0x00	//!< 	Host-to-device
#define USB_REQ_DIR_IN				0x80	//!< 	Device-to-host
#define USB_REQ_TYPE				0x60	//!< Request Type
#define USB_REQ_TYPE_STANDARD		0x00	//!< 	USB Standard request
#define USB_REQ_TYPE_CLASS			0x20	//!< 	Class specific request
#define USB_REQ_TYPE_VENDOR			0x40	//!< 	Vendor specific request
#define USB_REQ_TYPE_RESERVED		0x60	//!< 	Reserved type request
#define USB_REQ_RECIPIENT			0x1F	//!< Request Recipient
#define USB_REQ_RECIPIENT_DEVICE	0x00	//!< 	Device Recipient
#define USB_REQ_RECIPIENT_INTERFACE	0x01	//!<	Interface Recipient
#define USB_REQ_RECIPIENT_ENDPOINT	0x02	//!< 	Endpoint Recipient
#define USB_REQ_RECIPIENT_OTHER		0x03	//!< 	Other Recipient

#define USB_REQ_OUT_STANDARD		(USB_REQ_DIR_OUT | USB_REQ_TYPE_STANDARD)
#define USB_REQ_OUT_CLASS			(USB_REQ_DIR_OUT | USB_REQ_TYPE_CLASS)
#define USB_REQ_IN_STANDARD			(USB_REQ_DIR_IN | USB_REQ_TYPE_STANDARD)
#define USB_REQ_IN_CLASS			(USB_REQ_DIR_IN | USB_REQ_TYPE_CLASS)

///USB request type, recipient and direction
enum USBRequestType : unsigned char
{
	USB_REQ_OUT_STANDARD_DEVICE		= (USB_REQ_OUT_STANDARD | USB_REQ_RECIPIENT_DEVICE),
	USB_REQ_OUT_STANDARD_INTERFACE	= (USB_REQ_OUT_STANDARD | USB_REQ_RECIPIENT_INTERFACE),
	USB_REQ_OUT_STANDARD_ENDPOINT	= (USB_REQ_OUT_STANDARD | USB_REQ_RECIPIENT_ENDPOINT),
	USB_REQ_IN_STANDARD_DEVICE		= (USB_REQ_IN_STANDARD | USB_REQ_RECIPIENT_DEVICE),
	USB_REQ_IN_STANDARD_INTERFACE	= (USB_REQ_IN_STANDARD | USB_REQ_RECIPIENT_INTERFACE),
	USB_REQ_IN_STANDARD_ENDPOINT	= (USB_REQ_IN_STANDARD | USB_REQ_RECIPIENT_ENDPOINT),
	USB_REQ_OUT_CLASS_INTERFACE		= (USB_REQ_OUT_CLASS | USB_REQ_RECIPIENT_INTERFACE),
	USB_REQ_IN_CLASS_INTERFACE		= (USB_REQ_IN_CLASS | USB_REQ_RECIPIENT_INTERFACE),
};

/// Generic USB SETUP request sent over Control endpoints.
struct USBGenericRequest
{
	USBRequestType bmRequestType;	//!< Type of request
    unsigned char bRequest;			//!< Request code
    unsigned short wValue;			//!< Request-specific value parameter.
    unsigned short wIndex;			//!< Request-specific index parameter
    unsigned short wLength;			//!< Expected length (in bytes) of the data phase.

    /**
     * Returns the request code of the given request.
     * \return Request code.
     * \sa "USB Request Codes"
     */
    unsigned char GetRequest() const { return (bRequest); };

    /**
     * Returns the wValue field of the given request.
     * \return Request value.
     */
    unsigned short GetValue() const { return (wValue); };

    /**
     * Returns the expected length of the data phase following a request.
     * \return Length of data phase.
     */
    unsigned short GetLength() const { return (wLength); };


    /**
     * Returns the wIndex field of the given request.
     * \return Request index;
     */
    unsigned short GetIndex() const { return (wIndex); };

    /**
     * Returns the direction of the data transfer following the given request.
     * \return Transfer direction.
     * \sa "USB Request Directions"
     */
    unsigned int GetDirection() const {return (bmRequestType & USB_REQ_DIR); };

    /**
     * Returns the type of the descriptor requested by the host given the
     * corresponding GET_DESCRIPTOR request.
     * \return Type of the requested descriptor.
     */
    unsigned int GetDescriptorType() const { return (wValue >> 8); };

    /**
     * Returns the index of the requested descriptor, given the corresponding
     * GET_DESCRIPTOR request.
     * \return Index of the requested descriptor.
     */
    unsigned int GetDescriptorIndex() const { return (wValue & 0xFF); };



    /**
     * Indicates which interface is targetted by a GET_INTERFACE or
     * SET_INTERFACE request.
     * \return Interface number.
     */
	unsigned int GetInterface() const { return (wIndex & 0xFF); };


    /**
     * Indicates the new alternate setting that the interface targetted by a
     * SET_INTERFACE request should use.
     * \return New active setting for the interface.
     */
	unsigned int GetAlternateSetting() const { return (wValue & 0xFF); };


    /**
     * Returns the endpoint number targetted by a given request.
     * \return Endpoint number.
     */
	unsigned int GetEndpointNumber() const {return (wIndex & 0xFF); };


    /**
     * Returns the intended recipient of a given request.
     * \return Request recipient.
     * \sa "USB Request Recipients"
     */
	unsigned int  GetRecipient() const { return (bmRequestType & USB_REQ_RECIPIENT); };


    /**
     *  Returns the feature selector of a given CLEAR_FEATURE or SET_FEATURE
     *  request.
     *  \return Feature selector.
     */
    unsigned short GetFeatureSelector() const { return (wValue); };


    /**
     * Returns the number of the configuration that should be set in response
     * to the given SET_CONFIGURATION request.
     * \return Number of the requested configuration.
     */
    unsigned short GetConfiguration() const { return (wValue); };

}__attribute__ ((packed));


//------------------------------------------------------------------------------
//         				Standard Device Request codes
//------------------------------------------------------------------------------


enum USBDeviceRequests : unsigned char
{
	USBGenericRequest_GET_STATUS 		= 0,	//!< *** GET_STATUS request code
	USBGenericRequest_CLEAR_FEATURE 	= 1,	//!< *** CLEAR_FEATURE request code
	USBGenericRequest_SET_FEATURE		= 3,	//!< *** SET_FEATURE request code
	USBDeviceRequest_SET_ADDRESS		= 5,	//!< *   SET_ADDRESS request code
	USBDeviceRequest_GET_DESCRIPTOR		= 6,	//!< *   GET_DESCRIPTOR request code
	USBDeviceRequest_SET_DESCRIPTOR		= 7,	//!< *   SET_DESCRIPTOR request code
	USBDeviceRequest_GET_CONFIGURATION	= 8,	//!< *   GET_CONFIGURATION request code
	USBDeviceRequest_SET_CONFIGURATION	= 9,	//!< *   SET_CONFIGURATION request code
	USBInterfaceRequest_GET_INTERFACE	=10,	//!<  *  GET_INTERFACE request code
	USBInterfaceRequest_SET_INTERFACE	=11,	//!<  *  SET_INTERFACE request code
	USBEndpointRequest_SYNCH_FRAME		=12		//!<   * SYNCH_FRAME request code
												//!< |||
												//!< || \___ endpoint
												//!< | \____ interface
												//!<  \_____ device
};


///USB Feature Request selectors
enum USBFeatureRequestSelectors
{
	USBFeatureRequest_ENDPOINTHALT			= 0, //!< Halt feature of an endpoint
	USBFeatureRequest_DEVICEREMOTEWAKEUP	= 1, //!< Remote wake-up feature of the device
	USBFeatureRequest_TESTMODE				= 2, //!< Test mode of the device
	USBFeatureRequest_OTG_B_HNP_ENABLE		= 3, //!< OTG b_hnp_enable
	USBFeatureRequest_OTG_A_HNP_SUPPORT		= 4, //!< OTG a_hnp_support
	USBFeatureRequest_OTG_A_ALT_HNP_SUPPORT = 5, //!< OTG a_alt_hnp_support
	USBFeatureRequest_OTG					= 0x0B //!< OTG set feature
};



#endif /* USB_STD_REQS_H_ */
