/*
 * usb_device.h
 *
 *  Created on: 21.06.2011
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

/* Used files:
 * 		usb/device/core/USBDDriver.h
 *
 * Modifications:
 * 	USBDDriver structure ->
 *
 */
#ifndef USB_DEVICE_H_
#define USB_DEVICE_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_descriptors.h>
#include <usb_requests.h>
#include <usb_interface.h>

/** USB device driver structure
 *
 * 	It is holding a list of descriptors identifying the device as well as the
 * 	driver current state.
 *
 */
struct usb_device
{
	/// List of descriptors used by the device.
	const USBDDriverDescriptors *pDescriptors;
	/// usb interfaces
	usb_interface* interfaces[USB_MAX_INTERACES];
	/// Current configuration number (0 -> device is not configured).
	unsigned char cfgnum;
	/// Indicates if remote wake up has been enabled by the host.
	unsigned char isRemoteWakeUpEnabled;
#if USB_ENABLE_OTG
	/// Features supported by OTG
	unsigned char otg_features_supported;
#endif

	void Initialize(const USBDDriverDescriptors *pDescriptors);
	void RequestHandler(const void* drv,
			const USBGenericRequest *pRequest, HANDLE hnd);

private:
	const USBConfigurationDescriptor* GetConfiguration(uint8_t cfg);
	void GetDescriptor(const USBGenericRequest *pRequest, HANDLE hnd);
	void SetConfiguration(const void* drv, uint8_t cfg, HANDLE hnd);
	void GetDeviceStatus(HANDLE hnd);
	void GetEndpointStatus(const void* drv, uint8_t eptnum, HANDLE hnd);
	void SetInterface(uint8_t infnum, uint8_t setting, HANDLE hnd);
	void GetInterface(uint8_t infnum, HANDLE hnd);
	void ShutdownInterfaces();
	bool ConfigInterface(const USBInterfaceDescriptor* ds, int size, HANDLE hnd);
};

extern "C" usb_interface* InterfaceFactory(const USBInterfaceDescriptor* ds, int size);

#endif /* USB_DEVICE_H_ */
