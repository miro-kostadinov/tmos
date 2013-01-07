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

struct usb_config_t
{
	const PIN_DESC*	usb_pins;
};

struct Endpoint
{
	unsigned char	state;		  //!< Endpoint state
	unsigned char 	bank;		  //!< current bank if dual banks are used
	unsigned short 	txfifo_size;  //!< Endpoint size in bytes
	unsigned short 	rxfifo_size;  //!< Endpoint size in bytes
	unsigned short 	rxfifo_cnt;	  //!< Bytes available in the rx Fifo
	HANDLE 			pending;
};

#define CLEAR_EPT_IN_FIFOSZ(x)	x.txfifo_size = 0
#define CLEAR_EPT_OUT_FIFOSZ(x)	x.rxfifo_size = 0

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
