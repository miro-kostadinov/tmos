/*
 * usb_requests.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_REQUESTS_H_
#define USB_REQUESTS_H_


#include <tmos.h>
#include <usb_opt.h>
#include <usb_std_reqs.h>

#if USB_ENABLE_PRN
#include <usb_prn_reqs.h>
#endif

#if USB_ENABLE_HID
#include <usb_hid_reqs.h>
#endif

#if USB_ENABLE_CDC
#include <usb_cdc_reqs.h>
#endif


#endif /* USB_REQUESTS_H_ */
