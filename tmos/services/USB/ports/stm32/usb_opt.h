/*
 * usb_opt.h
 *
 *  Created on: Dec 3, 2012
 *      Author: miro
 */

#ifndef USB_OPT_H_
#define USB_OPT_H_

#include <fam_cpp.h>
#include <otg_hs_stm32.h>

#define CFG_STM32_OTG_ULPI			0x0001		//!< USE ULPI PHY
#define CFG_STM32_OTG_SOF_OUTPUT	0x0002		//!< SOF output enable
#define CFG_STM32_OTG_INTERNAL_VBUS 0x0004		//!< ULPI External VBUS Drive
#define CFG_STM32_OTG_VBUS_SENS		0x0008		//!< Enable the VBUS sensing
#define CFG_STM32_OTG_MODE			0x0010		//!< OTG Mode
#define CFG_STM32_OTG_HS_CORE		0x0020		//!< OTG HS peripheral
#define CFG_STM32_OTG_LOW_POWER		0x0040		//!< low power
#define CFG_STM32_OTG_FORCE_DEVICE	0x0080		//!< Forced device mode
#define CFG_STM32_OTG_FORCE_HOST	0x0100		//!< Forced host mode

/// Number of endpoints used
#ifndef USB_NUMENDPOINTS
#define	USB_NUMENDPOINTS CHIP_USB_NUMENDPOINTS
#endif

/// Dedicated EP1 interrupts (not recommended)
#ifndef USB_STM32_DEDICATED_EP1
#define USB_STM32_DEDICATED_EP1 0
#endif


struct usb_config_t
{
	uint32_t	stm32_otg;						//!< stm32 otg mode settings
	uint16_t	rx_fifo_sz;
	uint16_t	tx_fifo_sz[USB_NUMENDPOINTS];
	const PIN_DESC*	usb_pins;
};

struct ep_dir_state_t		// directional endpoint state
{
	uint8_t		epd_state;	  	//!< Endpoint state
	uint8_t		epd_type;		//!< Endpoint type
	uint16_t	epd_fifo_sz;	//!< Enpoint fifo size
	HANDLE		epd_pending;	//!< Enpoint pending handles
};

struct Endpoint
{
	ep_dir_state_t epd_in;		//!< Endpoint IN direction state (tx)
	ep_dir_state_t epd_out;		//!< Endpoint OUT direction state (rx)
	unsigned short rxfifo_cnt;	//!< Bytes available in the rx Fifo
	unsigned short top_rx_cnt; 	//!< Bytes available in the top_rx_word
	unsigned int   top_rx_word;	//!< top FIFO word
};

#define CLEAR_EPT_IN_FIFOSZ(x)	x.epd_in.epd_fifo_sz = 0
#define CLEAR_EPT_OUT_FIFOSZ(x)	x.epd_out.epd_fifo_sz = 0

/**
 * USB Controller Registers Structure
 */
typedef USB_TypeDef USB_CONTROLLER;

#define USB_SETUP_READ_TOUT		200
#define USB_SETUP_WRITE_TOUT	200

/// Number of max interfaces used
#ifndef USB_MAX_INTERACES
#define	USB_MAX_INTERACES 2
#endif

/// System has a high speed USB OTG controller.
#ifndef USB_ENABLE_OTG
#define USB_ENABLE_OTG 0
#endif

/// USB Host mode
#ifndef USB_ENABLE_HOST
#define USB_ENABLE_HOST 0
#endif

#ifndef MAX_USB_DEVICES
#define MAX_USB_DEVICES	2
#endif


#endif /* USB_OPT_H_ */
