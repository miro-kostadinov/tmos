/*
 * usb_descriptors.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_


#include <tmos.h>
#include <usb_opt.h>
#include <usb_std_dtors.h>


#if USB_ENABLE_PRN
#include <usb_prn_dtors.h>
#endif

#if USB_ENABLE_HID
#include <usb_hid_dtors.h>
#endif

#if USB_ENABLE_CDC
#include <usb_cdc_dtors.h>
#endif

void usb_get_class_name(CSTRING& s, USBClassCode code);
int trace_usb_descriptor(USBGenericDescriptor* des);

USBInterfaceDescriptor* usb_get_interface(const USBConfigurationDescriptor* des,
		int interface);

#endif /* USB_DESCRIPTORS_H_ */
