/*
 * usb_cfg.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_CFG_H_
#define USB_CFG_H_

#include <brd_cfg.h>


//------------------------------------------------------------------------------
//         				OTG settings
//------------------------------------------------------------------------------

/// System has a high speed USB OTG controller.
#ifndef USB_ENABLE_OTG
#define USB_ENABLE_OTG 0
#endif

//------------------------------------------------------------------------------
//         				HOST settings
//------------------------------------------------------------------------------
/// USB Host mode disabled by default
#ifndef USB_ENABLE_HOST
#define USB_ENABLE_HOST 0
#endif

#ifndef MAX_USB_DEVICES
#define MAX_USB_DEVICES	2
#endif

//------------------------------------------------------------------------------
//         				Device settings
//------------------------------------------------------------------------------
/// USB Device mode enabled by default
#ifndef USB_ENABLE_DEVICE
#define USB_ENABLE_DEVICE 0
#endif

/// Number of max interfaces used
#ifndef USB_MAX_INTERACES
#define	USB_MAX_INTERACES 2
#endif


//------------------------------------------------------------------------------
//         				USB Class settings
//------------------------------------------------------------------------------
#ifndef USB_ENABLE_CDC
#define USB_ENABLE_CDC 0
#endif

#ifndef USB_ENABLE_PRN
#define USB_ENABLE_PRN 0
#endif

#ifndef USB_ENABLE_HID
#define USB_ENABLE_HID 0
#endif

//------------------------------------------------------------------------------
//         				Sanity checks
//------------------------------------------------------------------------------
#if !USB_ENABLE_DEVICE && !USB_ENABLE_HOST
#error "USB enable but no host or device!"
#endif

#endif /* USB_CFG_H_ */
