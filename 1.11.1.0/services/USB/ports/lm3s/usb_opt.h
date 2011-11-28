/*
 * usb_opt.h
 *
 *  Created on: 28.05.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef USB_OPT_H_
#define USB_OPT_H_

#include <usb_lm3s.h>
#include <fam_cpp.h>

/**
 * USB Controller Registers Structure
 */
typedef USB_Type USB_CONTROLLER;

#define USB_SETUP_READ_TOUT		200
#define USB_SETUP_WRITE_TOUT	200

/// Number of endpoints used
#ifndef USB_NUMENDPOINTS
#define	USB_NUMENDPOINTS CHIP_USB_NUMENDPOINTS
#endif

/// Number of max interfaces used
#ifndef USB_MAX_INTERACES
#define	USB_MAX_INTERACES 2
#endif

/// System has a high speed USB OTG controller.
#ifndef USE_USB_OTGHS
#define USE_USB_OTGHS 1
#endif

#endif /* USB_OPT_H_ */
