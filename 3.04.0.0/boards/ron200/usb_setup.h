/*
 * usb_setup.h
 *
 *  Created on: 04.07.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <cdc_descriptors.h>

extern const USBDDriverDescriptors ron200_descriptors;

#define USB_CDC_OUT_EPT 	EPT_1
#define USB_CDC_IN_EPT 		EPT_2
