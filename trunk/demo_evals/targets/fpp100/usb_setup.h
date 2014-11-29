/*
 * usb_setup.h
 *
 *  Created on: 04.07.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <usb_cdc_dtors.h>

extern const USBDDriverDescriptors fpp100_descriptors;

#define USB_CDC_IN_EPT 		EPT_1
#define USB_CDC_OUT_EPT 	EPT_1
