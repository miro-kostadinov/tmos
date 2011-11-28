/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
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

/** \file
 *  \section Purpose
 * 
 *    Definitions and methods for USB request structures described by the
 *    USB specification.
 * 
 *  \section Usage
 * 
 */

#ifndef _USBREQUESTS_H_
#define _USBREQUESTS_H_
/** \addtogroup usb_general
 * @{
 * \addtogroup usb_request USB Requests
 *  @{
 */

/*---------------------------------------------------------------------------
 *      Headers
 *---------------------------------------------------------------------------*/

/* These headers were introduced in C99 by working group
 * ISO/IEC JTC1/SC22/WG14.
 */
//#include <stdint.h>

/*---------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/

/*----------- Generic Request ------------*/

/** \addtogroup usb_request_define USB Generic Request definitions
 *      @{
 *  This section lists the codes of USB generic request.
 * 
 *  - \ref usb_request_code USB Request codes
 *     - \ref USBGenericRequest_GETSTATUS
 *     - \ref USBGenericRequest_CLEARFEATURE
 *     - \ref USBGenericRequest_SETFEATURE
 *     - \ref USBGenericRequest_SETADDRESS
 *     - \ref USBGenericRequest_GETDESCRIPTOR
 *     - \ref USBGenericRequest_SETDESCRIPTOR
 *     - \ref USBGenericRequest_GETCONFIGURATION
 *     - \ref USBGenericRequest_SETCONFIGURATION
 *     - \ref USBGenericRequest_GETINTERFACE
 *     - \ref USBGenericRequest_SETINTERFACE
 *     - \ref USBGenericRequest_SYNCHFRAME
 * 
 *  - \ref usb_request_recipient USB Request Recipients
 *     - \ref USBGenericRequest_DEVICE
 *     - \ref USBGenericRequest_INTERFACE
 *     - \ref USBGenericRequest_ENDPOINT
 *     - \ref USBGenericRequest_OTHER
 * 
 *  - \ref usb_request_type USB Request Types
 *     - \ref USBGenericRequest_STANDARD
 *     - \ref USBGenericRequest_CLASS
 *     - \ref USBGenericRequest_VENDOR
 * 
 *  - \ref usb_request_dir USB Request Directions
 *     - \ref USBGenericRequest_IN
 *     - \ref USBGenericRequest_OUT
 */

/** \addtogroup usb_request_code USB Request codes
 *          @{
 *  This section lists the USB generic request codes.
 *  - \ref USBGenericRequest_GETSTATUS
 *  - \ref USBGenericRequest_CLEARFEATURE
 *  - \ref USBGenericRequest_SETFEATURE
 *  - \ref USBGenericRequest_SETADDRESS
 *  - \ref USBGenericRequest_GETDESCRIPTOR
 *  - \ref USBGenericRequest_SETDESCRIPTOR
 *  - \ref USBGenericRequest_GETCONFIGURATION
 *  - \ref USBGenericRequest_SETCONFIGURATION
 *  - \ref USBGenericRequest_GETINTERFACE
 *  - \ref USBGenericRequest_SETINTERFACE
 *  - \ref USBGenericRequest_SYNCHFRAME
 */
/**  GET_STATUS request code. */
#define USBGenericRequest_GETSTATUS             0
/**  CLEAR_FEATURE request code. */
#define USBGenericRequest_CLEARFEATURE          1
/**  SET_FEATURE request code. */
#define USBGenericRequest_SETFEATURE            3
/**  SET_ADDRESS request code. */
#define USBGenericRequest_SETADDRESS            5
/**  GET_DESCRIPTOR request code. */
#define USBGenericRequest_GETDESCRIPTOR         6
/**  SET_DESCRIPTOR request code. */
#define USBGenericRequest_SETDESCRIPTOR         7
/**  GET_CONFIGURATION request code. */
#define USBGenericRequest_GETCONFIGURATION      8
/**  SET_CONFIGURATION request code. */
#define USBGenericRequest_SETCONFIGURATION      9
/**  GET_INTERFACE request code. */
#define USBGenericRequest_GETINTERFACE          10
/**  SET_INTERFACE request code. */
#define USBGenericRequest_SETINTERFACE          11
/**  SYNCH_FRAME request code. */
#define USBGenericRequest_SYNCHFRAME            12
/**         @}*/

/** \addtogroup usb_request_recipient USB Request Recipients
 *          @{
 *  This section lists codes of USB request recipients.
 *  - \ref USBGenericRequest_DEVICE
 *  - \ref USBGenericRequest_INTERFACE
 *  - \ref USBGenericRequest_ENDPOINT
 *  - \ref USBGenericRequest_OTHER
 */
/**  Recipient is the whole device. */
#define USBGenericRequest_DEVICE                0
/**  Recipient is an interface. */
#define USBGenericRequest_INTERFACE             1
/**  Recipient is an endpoint. */
#define USBGenericRequest_ENDPOINT              2
/**  Recipient is another entity. */
#define USBGenericRequest_OTHER                 3
/**         @}*/

/** \addtogroup usb_request_type USB Request Types
 *          @{
 *  This section lists codes of USB request types.
 *  - \ref USBGenericRequest_STANDARD
 *  - \ref USBGenericRequest_CLASS
 *  - \ref USBGenericRequest_VENDOR
 */
/**  Request is standard. */
#define USBGenericRequest_STANDARD              0
/**  Request is class-specific. */
#define USBGenericRequest_CLASS                 1
/**  Request is vendor-specific. */
#define USBGenericRequest_VENDOR                2
/**         @}*/

/** \addtogroup usb_request_dir USB Request Directions
 *          @{
 *  This section lists codes of USB request directions.
 *  - \ref USBGenericRequest_IN
 *  - \ref USBGenericRequest_OUT
 */
/**  Transfer occurs from device to the host. */
#define USBGenericRequest_OUT                   0
/**  Transfer occurs from the host to the device. */
#define USBGenericRequest_IN                    1
/**         @}*/
/**     @}*/


/*----------- Feature Request ------------*/

/** \addtogroup usb_feature_def USB Feature Request Definitions
 *      @{
 * This section lists codes of USB Feature Request
 *
 * - \ref usb_feature_sel USB Feature selectors
 *    - \ref USBFeatureRequest_ENDPOINTHALT
 *    - \ref USBFeatureRequest_DEVICEREMOTEWAKEUP
 *    - \ref USBFeatureRequest_TESTMODE
 *
 * - \ref usb_test_sel USB Test mode selectors
 *    - \ref USBFeatureRequest_TESTJ
 *    - \ref USBFeatureRequest_TESTK
 *    - \ref USBFeatureRequest_TESTSE0NAK
 *    - \ref USBFeatureRequest_TESTPACKET
 *    - \ref USBFeatureRequest_TESTFORCEENABLE
 *    - \ref USBFeatureRequest_TESTSENDZLP
 */

/** \addtogroup usb_feature_sel USB Feature selectors
 *          @{
 * This section lists codes of USB feature selectors.
 * - \ref USBFeatureRequest_ENDPOINTHALT
 * - \ref USBFeatureRequest_DEVICEREMOTEWAKEUP
 * - \ref USBFeatureRequest_TESTMODE
 */
/** Halt feature of an endpoint. */
#define USBFeatureRequest_ENDPOINTHALT          0
/** Remote wake-up feature of the device. */
#define USBFeatureRequest_DEVICEREMOTEWAKEUP    1
/** Test mode of the device. */
#define USBFeatureRequest_TESTMODE              2
/** OTG set feature */
#define USBFeatureRequest_OTG                0x0B
/** OTG b_hnp_enable */
#define USBFeatureRequest_OTG_B_HNP_ENABLE      3
/** OTG a_hnp_support */
#define USBFeatureRequest_OTG_A_HNP_SUPPORT     4
/** OTG a_alt_hnp_support */
#define USBFeatureRequest_OTG_A_ALT_HNP_SUPPORT 5
/**         @}*/

/** \addtogroup usb_test_sel USB Test mode selectors
 *          @{
 *  This section lists codes of USB high speed test mode selectors.
 *  - \ref USBFeatureRequest_TESTJ
 *  - \ref USBFeatureRequest_TESTK
 *  - \ref USBFeatureRequest_TESTSE0NAK
 *  - \ref USBFeatureRequest_TESTPACKET
 *  - \ref USBFeatureRequest_TESTFORCEENABLE
 *  - \ref USBFeatureRequest_TESTSENDZLP
 */

/**  Tests the high-output drive level on the D+ line. */
#define USBFeatureRequest_TESTJ                 1
/**  Tests the high-output drive level on the D- line. */
#define USBFeatureRequest_TESTK                 2
/**  Tests the output impedance, low-level output voltage and loading
     characteristics. */
#define USBFeatureRequest_TESTSE0NAK            3
/**  Tests rise and fall times, eye patterns and jitter. */
#define USBFeatureRequest_TESTPACKET            4
/**  Tests the hub disconnect detection. */
#define USBFeatureRequest_TESTFORCEENABLE       5
/**  Send a ZLP in Test Mode. */
#define USBFeatureRequest_TESTSENDZLP           6
/**         @}*/
/**     @}*/

/*---------------------------------------------------------------------------
 *      Types
 *---------------------------------------------------------------------------*/

/* 
 *          Function types
 */

/* 
 *          Descriptor structs types
 */
#ifdef __ICCARM__          /* IAR */
#pragma pack(1)            /* IAR */
#define __attribute__(...) /* IAR */
#endif                     /* IAR */

/**
 *  Generic USB SETUP request sent over Control endpoints.
 */
typedef struct {

    /**  Type of request
     *  \sa usb_request_recipient "USB Request Recipients"
     *  \sa usb_request_type "USB Request Types"
     *  \sa usb_request_dir "USB Request Directions" */
    uint8_t bmRequestType:8;
    /**  Request code
     *  \sa usb_request_code "USB Request Codes" */
    uint8_t bRequest:8;
    /**  Request-specific value parameter. */
    uint16_t wValue:16;
    /**  Request-specific index parameter. */
    uint16_t wIndex:16;
    /**  Expected length (in bytes) of the data phase. */
    uint16_t wLength:16;

} USBGenericRequest;

#ifdef __ICCARM__          /* IAR */
#pragma pack()             /* IAR */
#endif                     /* IAR */

/**
 * Returns the request code of the given request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Request code.
 * \sa "USB Request Codes"
 */
//extern uint8_t USBGenericRequest_GetRequest(
//    const USBGenericRequest *request);
#define USBGenericRequest_GetRequest(r)  ((r)->bRequest)


/**
 * Returns the wValue field of the given request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Request value.
 */
//extern uint16_t USBGenericRequest_GetValue(
//    const USBGenericRequest *request);
#define	USBGenericRequest_GetValue(r) 	((r)->wValue)


/**
 * Returns the expected length of the data phase following a request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Length of data phase.
 */
//extern uint16_t USBGenericRequest_GetLength(
//    const USBGenericRequest *request);
#define USBGenericRequest_GetLength(r) 	((r)->wLength)


/**
 * Returns the wIndex field of the given request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Request index;
 */
//extern uint16_t USBGenericRequest_GetIndex(
//    const USBGenericRequest *request);
#define USBGenericRequest_GetIndex(r)	((r)->wIndex)

/**
 * Returns the direction of the data transfer following the given request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Transfer direction.
 * \sa "USB Request Directions"
 */
//extern uint8_t USBGenericRequest_GetDirection(
//    const USBGenericRequest *request);
#define USBGenericRequest_GetDirection(r) \
	(((r)->bmRequestType & 0x80)?USBGenericRequest_IN:USBGenericRequest_OUT)


/**
 * Returns the type of the descriptor requested by the host given the
 * corresponding GET_DESCRIPTOR request.
 * \param r Pointer to a USBGenericDescriptor instance.
 * \return Type of the requested descriptor.
 */
//extern uint8_t USBGetDescriptorRequest_GetDescriptorType(
//    const USBGenericRequest *request);
#define USBGetDescriptorRequest_GetDescriptorType(r) \
	((USBGenericRequest_GetValue(r) >> 8) & 0xFF)


/**
 * Returns the index of the requested descriptor, given the corresponding
 * GET_DESCRIPTOR request.
 * \param r Pointer to a USBGenericDescriptor instance.
 * \return Index of the requested descriptor.
 */
//extern uint8_t USBGetDescriptorRequest_GetDescriptorIndex(
//    const USBGenericRequest *request);
#define USBGetDescriptorRequest_GetDescriptorIndex(r) \
	(USBGenericRequest_GetValue(r) & 0xFF)



/**
 * Indicates which interface is targetted by a GET_INTERFACE or
 * SET_INTERFACE request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Interface number.
 */
//extern uint8_t USBInterfaceRequest_GetInterface(
//    const USBGenericRequest *request);
#define USBInterfaceRequest_GetInterface(r) \
	((USBGenericRequest_GetIndex(r) & 0xFF))


/**
 * Indicates the new alternate setting that the interface targetted by a
 * SET_INTERFACE request should use.
 * \param r Pointer to a USBGenericRequest instance.
 * \return New active setting for the interface.
 */
//extern uint8_t USBInterfaceRequest_GetAlternateSetting(
//    const USBGenericRequest *request);
#define USBInterfaceRequest_GetAlternateSetting(r) \
	(USBGenericRequest_GetValue(r) & 0xFF)


/**
 * Returns the endpoint number targetted by a given request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Endpoint number.
 */
//extern uint8_t USBGenericRequest_GetEndpointNumber(
//    const USBGenericRequest *request);
#define USBGenericRequest_GetEndpointNumber(r) \
	(USBGenericRequest_GetIndex(r) & 0xF)


/**
 * Returns the intended recipient of a given request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Request recipient.
 * \sa "USB Request Recipients"
 */
//extern uint8_t USBGenericRequest_GetRecipient(
//    const USBGenericRequest *request);
#define USBGenericRequest_GetRecipient(r) \
	((r)->bmRequestType & 0xF)


/**
 *  Returns the feature selector of a given CLEAR_FEATURE or SET_FEATURE
 *  request.
 *  \param r Pointer to a USBGenericRequest instance.
 *  \return Feature selector.
 */
//extern uint8_t USBFeatureRequest_GetFeatureSelector(
//    const USBGenericRequest *request);
#define USBFeatureRequest_GetFeatureSelector(r) \
	USBGenericRequest_GetValue(r)


/**
 * Returns the number of the configuration that should be set in response
 * to the given SET_CONFIGURATION request.
 * \param r Pointer to a USBGenericRequest instance.
 * \return Number of the requested configuration.
 */
//extern uint8_t USBSetConfigurationRequest_GetConfiguration(
//    const USBGenericRequest *request);
#define USBSetConfigurationRequest_GetConfiguration(r) \
	USBGenericRequest_GetValue(r)


/*---------------------------------------------------------------------------
 *      Exported Functions
 *---------------------------------------------------------------------------*/


/** @}*/
/**@}*/
#endif /* #ifndef _USBREQUESTS_H_ */

