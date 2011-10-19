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

/**
 * \file
 *
 * \section Purpose
 *
 * Definition of a class for implementing a USB device CDC serial driver.
 *
 * \section Usage
 *
 * -# Re-implement the USBDCallbacks_RequestReceived method to pass
 *    received requests to CDCDSerialDriver_RequestHandler. *This is
 *    automatically done unless the NOAUTOCALLBACK symbol is defined*.
 * -# Initialize the CDC serial and USB drivers using
 *    CDCDSerialDriver_Initialize.
 * -# Logically connect the device to the host using USBD_Connect.
 * -# Send serial data to the USB host using CDCDSerialDriver_Write.
 * -# Receive serial data from the USB host using CDCDSerialDriver_Read.
 */

#ifndef CDCDSERIALDRIVER_H
#define CDCDSERIALDRIVER_H

/** \addtogroup usbd_cdc
 *@{
 */

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/

/* These headers were introduced in C99
   by working group ISO/IEC JTC1/SC22/WG14. */
//#include <stdint.h>

#include <USBRequests.h>
#include <cdc_requests.h>
#include <cdc_descriptors.h>
#include <cdc_notifications.h>
#include <cdc_serial.h>

/*------------------------------------------------------------------------------
 *         Definitions
 *------------------------------------------------------------------------------*/

/** \addtogroup usbd_cdc_if USB Device CDC Serial Interface IDs
 *      @{
 */
/** Communication Class Interface ID */
#define CDCDSerialDriver_CC_INTERFACE           0
/** Data Class Interface ID */
#define CDCDSerialDriver_DC_INTERFACE           1
/**     @}*/

/*------------------------------------------------------------------------------
 *         Types
 *------------------------------------------------------------------------------*/

/**
 * \struct _CDCDSerialDriverConfigurationDescriptors
 * \brief Configuration descriptor list for a device implementing a
 *        CDC serial driver.
 */
typedef struct _CDCDSerialDriverConfigurationDescriptors {

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

} __attribute__ ((packed)) CDCDSerialDriverConfigurationDescriptors; //!< doc

/**
 * \struct _CDCDSerialDriverConfigurationDescriptorsOTG
 * \brief Configuration descriptor list for a device implementing a
 *        CDC serial OTG driver.
 */
typedef struct _CDCDSerialDriverConfigurationDescriptorsOTG {

    /** Standard configuration descriptor. */
    USBConfigurationDescriptor configuration;
    /** OTG descriptor */
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

} __attribute__ ((packed)) CDCDSerialDriverConfigurationDescriptorsOTG;


/**@}*/

#endif /*#ifndef CDCSERIALDRIVER_H*/

