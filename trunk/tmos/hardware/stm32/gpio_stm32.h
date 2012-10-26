/*
 * gpio_stm32.h
 *
 *  Created on: Oct 25, 2012
 *      Author: miro
 */

#ifndef GPIO_STM32_H_
#define GPIO_STM32_H_

#include <series_cpp.h>


/// Pin definition bits 31:30 = MODE
#define PD_MODE_Pos 		30						//!< mode bits position
#define PD_MODE_Msk			(0x3 << PD_MODE_Pos)	//!< mode bits mask
#define PD_MODE_Get(x)		(0x3 & ((x) >> PD_MODE_Pos)) //!< mode value
#define PD_MODE_INPUT		(0 << PD_MODE_Pos)		//!< input (reset state)
#define PD_MODE_OUTPUT		(1 << PD_MODE_Pos)		//!< general purpose output mode
#define PD_MODE_AF			(2 << PD_MODE_Pos)		//!< alternative function mode
#define PD_MODE_ANALOG		(3 << PD_MODE_Pos)		//!< analog mode


/// Pin definition bits 29:29 = output type
#define PD_OTYPE_Pos 		29						//!< output type bits position
#define PD_OTYPE_Msk		(0x1 << PD_OTYPE_Pos)	//!< output type bits mask
#define PD_OTYPE_Get(x)		(0x1 & ((x) >> PD_OTYPE_Pos)) //!< output type value
#define PD_OTYPE_PP			(0 << PD_OTYPE_Pos)		//!< Output push-pull (reset state)
#define PD_OTYPE_OD			(1 << PD_OTYPE_Pos)		//!< Output open-drain


/// Pin definition bits 28:27 = output speed or interrupt type
#define PD_OSPEED_Pos 		27						//!< output speed bits position
#define PD_OSPEED_Msk		(0x3 << PD_OSPEED_Pos)	//!< output speed bits mask
#define PD_OSPEED_Get(x)	(0x3 & ((x) >> PD_OSPEED_Pos)) //!< output speed value
#if CFG_SERIES == stm32f1
#define PD_OSPEED_2MHz		(2 << PD_OSPEED_Pos)	//!< 2MHz output speed
#define PD_OSPEED_10MHz		(1 << PD_OSPEED_Pos)	//!< 10MHz output speed
#define PD_OSPEED_50MHz		(3 << PD_OSPEED_Pos)	//!< 50MHz output speed
#else
#define PD_OSPEED_2MHz		(0 << PD_OSPEED_Pos)	//!< 2MHz output speed
#define PD_OSPEED_25MHz		(1 << PD_OSPEED_Pos)	//!< 25MHz output speed
#define PD_OSPEED_50MHz		(2 << PD_OSPEED_Pos)	//!< 50MHz output speed
#define PD_OSPEED_100MHz	(3 << PD_OSPEED_Pos)	//!< 100MHz output speed
#endif

#define PD_INT_Pos 			27						//!< interrupt bits position
#define PD_INT_FE			(1 << PD_INT_Pos)		//!< falling edge
#define PD_INT_RE			(2 << PD_INT_Pos)		//!< rising edge
#define PD_INT_BE			(3 << PD_INT_Pos)		//!< both edges


/// Pin definition bits 26:26 = pullup
#define PD_PULL_UP   		(1 << 26)				//!< weak pull up


/// Pin definition bits 25:25 = pullup
#define PD_PULL_DOWN   		(1 << 25)				//!< weak pull down


/// Pin definition bit 24 = Active level
#define PD_ACTIVE_HIGH 	  	(1 << 24)
#define PD_ACTIVE_LOW 	  	(0 << 24)


/// Pin definition bits 23:20 = pin mux
#define PD_MUX_Pos			20						//!< mux bits position
#define PD_MUX_Msk			(0xF << PD_MUX_Pos)		//!< mux bits mask
#define PD_MUX_Get(x)		(0xF & ((x) >> PD_MUX_Pos)) //!< mux value
#define PD_MUX_AF0			(0 << PD_MUX_Pos)		//!< mux configuration 0
#define PD_MUX_AF1			(1 << PD_MUX_Pos)		//!< mux configuration 1
#define PD_MUX_AF2			(2 << PD_MUX_Pos)		//!< mux configuration 2
#define PD_MUX_AF3			(3 << PD_MUX_Pos)		//!< mux configuration 3
#define PD_MUX_AF4			(4 << PD_MUX_Pos)		//!< mux configuration 4
#define PD_MUX_AF5			(5 << PD_MUX_Pos)		//!< mux configuration 5
#define PD_MUX_AF6			(6 << PD_MUX_Pos)		//!< mux configuration 6
#define PD_MUX_AF7			(7 << PD_MUX_Pos)		//!< mux configuration 7
#define PD_MUX_AF8			(8 << PD_MUX_Pos)		//!< mux configuration 8
#define PD_MUX_AF9			(9 << PD_MUX_Pos)		//!< mux configuration 9
#define PD_MUX_AF10			(10 << PD_MUX_Pos)		//!< mux configuration 10
#define PD_MUX_AF11			(11 << PD_MUX_Pos)		//!< mux configuration 11
#define PD_MUX_AF12			(12 << PD_MUX_Pos)		//!< mux configuration 12
#define PD_MUX_AF13			(13 << PD_MUX_Pos)		//!< mux configuration 13
#define PD_MUX_AF14			(14 << PD_MUX_Pos)		//!< mux configuration 14
#define PD_MUX_AF15			(15 << PD_MUX_Pos)		//!< mux configuration 15


/// Pin definition bits 19:16 = port
#define PD_PORT_Pos			16						//!< port bits position
#define PD_PORT_Msk			(0xF << PD_PORT_Pos)	//!< port bits mask
#define PD_PORT_Get(x)		(0xF & ((x) >> PD_PORT_Pos)) //!< port value
#define	PD_PORT_A			(0 << PD_PORT_Pos)		//!< port A
#define	PD_PORT_B			(1 << PD_PORT_Pos)		//!< port B
#define	PD_PORT_C			(2 << PD_PORT_Pos)		//!< port C
#define	PD_PORT_D			(3 << PD_PORT_Pos)		//!< port D
#define	PD_PORT_E			(4 << PD_PORT_Pos)		//!< port E
#define	PD_PORT_F			(5 << PD_PORT_Pos)		//!< port F
#define	PD_PORT_G			(6 << PD_PORT_Pos)		//!< port G
#define	PD_PORT_H			(7 << PD_PORT_Pos)		//!< port H
#define	PD_PORT_I			(8 << PD_PORT_Pos)		//!< port J
#define PD_PORT_V           (0xF << PD_PORT_Pos)	//!< virtual port V
#define PD_LAST_PORT		8						//!< number of physical ports



/// Pin definition bits 15:0 = pin bits
#define PD_PINS_Pos			0						//!< pins bits position
#define PD_PINS_Msk			(0xFFFF << PD_PINS_Pos)	//!< pins bits mask
#define PD_PINS_Get(x)		(0xFFFF & ((x))			//!< pins value
#define PD_PIN_0			(0x0001 << PD_PINS_Pos)	//!< pin 0
#define PD_PIN_1			(0x0002 << PD_PINS_Pos)	//!< pin 1
#define PD_PIN_2			(0x0004 << PD_PINS_Pos)	//!< pin 2
#define PD_PIN_3			(0x0008 << PD_PINS_Pos)	//!< pin 3
#define PD_PIN_4			(0x0010 << PD_PINS_Pos)	//!< pin 4
#define PD_PIN_5			(0x0020 << PD_PINS_Pos)	//!< pin 5
#define PD_PIN_6			(0x0040 << PD_PINS_Pos)	//!< pin 6
#define PD_PIN_7			(0x0080 << PD_PINS_Pos)	//!< pin 7
#define PD_PIN_8			(0x0100 << PD_PINS_Pos)	//!< pin 8
#define PD_PIN_9			(0x0200 << PD_PINS_Pos)	//!< pin 9
#define PD_PIN_10  			(0x0400 << PD_PINS_Pos)	//!< pin 10
#define PD_PIN_11			(0x0800 << PD_PINS_Pos)	//!< pin 11
#define PD_PIN_12			(0x1000 << PD_PINS_Pos)	//!< pin 12
#define PD_PIN_13			(0x2000 << PD_PINS_Pos)	//!< pin 13
#define PD_PIN_14			(0x4000 << PD_PINS_Pos)	//!< pin 14
#define PD_PIN_15			(0x8000 << PD_PINS_Pos)	//!< pin 15






#endif /* GPIO_STM32_H_ */
