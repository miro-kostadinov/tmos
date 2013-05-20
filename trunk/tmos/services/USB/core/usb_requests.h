/*
 * usb_requests.h
 *
 *  Created on: 24.06.2011
 *      Author: Miroslav Kostadinov
 *
 *	NOTE: This file is based on some Atmel sources, so the original copyright
 *	follows
 *
 */

/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */


#ifndef USB_REQUESTS_H_
#define USB_REQUESTS_H_

#include <tmos.h>
#include <usb_opt.h>

//------------------------------------------------------------------------------
//         				USBGenericDescriptor
//------------------------------------------------------------------------------

/// Data Phase Transfer Direction - used in bmRequestType
enum RequestType_dir
{
    USB_REQUEST_DIR_OUT 		= 0x00,	//!< Host to Device
    USB_REQUEST_DIR_IN 			= 0x01,	//!< Device to Host
};

/// type of request - used in bmRequestType
enum RequestType_type
{
    USB_REQUEST_TYPE_STANDARD 	= 0x00,	//!< USB Standard Request
    USB_REQUEST_TYPE_CLASS 		= 0x01,	//!< Class specific request
    USB_REQUEST_TYPE_VENDOR 	= 0x02,	//!< Vendor specific request
    USB_REQUEST_TYPE_RESERVED	= 0x03	//!<
};

/// Recipient - used in bmRequestType
enum RequestType_recipient
{
    USB_REQUEST_RECIPIENT_DEVICE 	= 0x00,	//!< Device
    USB_REQUEST_RECIPIENT_INTERFACE	= 0x01,	//!< Interface
    USB_REQUEST_RECIPIENT_ENDPOINT 	= 0x02,	//!< Endpoint
    USB_REQUEST_RECIPIENT_OTHER		= 0x03	//!< Other
};

///USB request type, recipient and direction
union USBRequestType
{
  unsigned char as_byte;
  struct
  {
	  RequestType_recipient recipient : 5;
	  RequestType_type 		type      : 2;
	  RequestType_dir 		dir       : 1;
  };
}__attribute__ ((packed));

enum USBDeviceRequests : unsigned char
{
	USBGenericRequest_GET_STATUS 		= 0,	//!< GET_STATUS request code
	USBGenericRequest_CLEAR_FEATURE 	= 1,	//!< CLEAR_FEATURE request code
	USBGenericRequest_SET_FEATURE		= 3,	//!< SET_FEATURE request code
	USBDeviceRequest_SET_ADDRESS		= 5,	//!< SET_ADDRESS request code
	USBDeviceRequest_GET_DESCRIPTOR		= 6,	//!< GET_DESCRIPTOR request code
	USBDeviceRequest_SET_DESCRIPTOR		= 7,	//!< SET_DESCRIPTOR request code
	USBDeviceRequest_GET_CONFIGURATION	= 8,	//!< GET_CONFIGURATION request code
	USBDeviceRequest_SET_CONFIGURATION	= 9,	//!< SET_CONFIGURATION request code
	USBInterfaceRequest_GET_INTERFACE	=10,	//!< GET_INTERFACE request code
	USBInterfaceRequest_SET_INTERFACE	=11,	//!< SET_INTERFACE request code
	USBEndpointRequest_SYNCH_FRAME		=12		//!< SYNCH_FRAME request code
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
    RequestType_dir GetDirection() const {return (bmRequestType.dir); };

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
	RequestType_recipient GetRecipient() const { return (bmRequestType.recipient); };


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



#endif /* USB_REQUESTS_H_ */
