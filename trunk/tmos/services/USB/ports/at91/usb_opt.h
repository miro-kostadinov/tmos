/*
 * USB stack options for AT91
 *
 */

#ifndef USB_OPT_H_
#define USB_OPT_H_

#include <usbd.h>
#include <fam_cpp.h>
#include <pio_drv.h>
#include <usb_cfg.h>

struct usb_config_t
{
	const PIN_DESC*	usb_pins;
};

/** Endpoint data structure */
struct Endpoint
{
        unsigned char state;            //!< endpoint state
        unsigned char bank;             //!< current RAM bank
        unsigned short size;            //!< endpoint size
        unsigned int rxfifo_cnt;        //!< bytes in rxfifo
        HANDLE pending;                 //!< pending handles
};

#define CLEAR_EPT_IN_FIFOSZ(x)	x.size = 0
#define CLEAR_EPT_OUT_FIFOSZ(x)	while(0)

/**
 * USB Controller Registers Struct
 */
typedef Udp USB_CONTROLLER;

#define USB_SETUP_READ_TOUT		200
#define USB_SETUP_WRITE_TOUT	200


#endif /* USB_OPT_H_ */
