/*
 * gpio_stm32.h
 *
 *  Created on: Oct 25, 2012
 *      Author: miro
 */

#ifndef GPIO_STM32_H_
#define GPIO_STM32_H_

#include <series_cpp.h>

/*******************************************************************************
 * @defgroup STD_PIO Standard PIO definitions
 * @{
 ******************************************************************************/

/** Pin set **/
typedef unsigned short pio_set;

/** Pin definition **/
typedef unsigned int pio_def;

/** Pin configuration **/
typedef unsigned int PIN_DESC;



/// Pin definition bits 31:30 = MODE
#define PD_MODE_Pos 		30						//!< mode bits position
#define PD_MODE_Msk			(0x3 << PD_MODE_Pos)	//!< mode bits mask
#define PD_MODE_Get(x)		((x) >> PD_MODE_Pos) 	//!< mode value
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
#define PD_INT_Msk			(0x3 << PD_INT_Pos)	//!< output speed bits mask
#define PD_INT_Get(x)		(0x3 & ((x) >> PD_INT_Pos)) //!< output speed value


/// Pin definition bits 25:25 = pulldown
#define PD_PULL_DOWN   		(1 << 26)				//!< weak pull down

/// Pin definition bits 26:26 = pullup
#define PD_PULL_UP   		(1 << 25)				//!< weak pull up


#define PD_PULL_Get(x)	(0x3 & ((x) >> 25)) //!< PUPD value


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
#define	PD_PORT_I			(8 << PD_PORT_Pos)		//!< port I
#define PD_PORT_V           (0xF << PD_PORT_Pos)	//!< virtual port V
#define PD_LAST_PORT		8						//!< number of physical ports



/// Pin definition bits 15:0 = pin bits
#define PD_PINS_Pos			0						//!< pins bits position
#define PD_PINS_Msk			(0xFFFF << PD_PINS_Pos)	//!< pins bits mask
#define PD_PINS_Get(x)		(0xFFFF & (x))			//!< pins value
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






/// Values that can be used in pin definitions

#define PD_PA0				(PD_PORT_A | PD_PIN_0)	//!< pin PA0
#define PD_PA1				(PD_PORT_A | PD_PIN_1)	//!< pin PA1
#define PD_PA2				(PD_PORT_A | PD_PIN_2)	//!< pin PA2
#define PD_PA3				(PD_PORT_A | PD_PIN_3)	//!< pin PA3
#define PD_PA4				(PD_PORT_A | PD_PIN_4)	//!< pin PA4
#define PD_PA5				(PD_PORT_A | PD_PIN_5)	//!< pin PA5
#define PD_PA6				(PD_PORT_A | PD_PIN_6)	//!< pin PA6
#define PD_PA7				(PD_PORT_A | PD_PIN_7)	//!< pin PA7
#define PD_PA8				(PD_PORT_A | PD_PIN_8)	//!< pin PA8
#define PD_PA9				(PD_PORT_A | PD_PIN_9)	//!< pin PA9
#define PD_PA10				(PD_PORT_A | PD_PIN_10)	//!< pin PA10
#define PD_PA11				(PD_PORT_A | PD_PIN_11)	//!< pin PA11
#define PD_PA12				(PD_PORT_A | PD_PIN_12)	//!< pin PA12
#define PD_PA13				(PD_PORT_A | PD_PIN_13)	//!< pin PA13
#define PD_PA14				(PD_PORT_A | PD_PIN_14)	//!< pin PA14
#define PD_PA15				(PD_PORT_A | PD_PIN_15)	//!< pin PA15

#define PD_PB0				(PD_PORT_B | PD_PIN_0)	//!< pin PB0
#define PD_PB1				(PD_PORT_B | PD_PIN_1)	//!< pin PB1
#define PD_PB2				(PD_PORT_B | PD_PIN_2)	//!< pin PB2
#define PD_PB3				(PD_PORT_B | PD_PIN_3)	//!< pin PB3
#define PD_PB4				(PD_PORT_B | PD_PIN_4)	//!< pin PB4
#define PD_PB5				(PD_PORT_B | PD_PIN_5)	//!< pin PB5
#define PD_PB6				(PD_PORT_B | PD_PIN_6)	//!< pin PB6
#define PD_PB7				(PD_PORT_B | PD_PIN_7)	//!< pin PB7
#define PD_PB8				(PD_PORT_B | PD_PIN_8)	//!< pin PB8
#define PD_PB9				(PD_PORT_B | PD_PIN_9)	//!< pin PB9
#define PD_PB10				(PD_PORT_B | PD_PIN_10)	//!< pin PB10
#define PD_PB11				(PD_PORT_B | PD_PIN_11)	//!< pin PB11
#define PD_PB12				(PD_PORT_B | PD_PIN_12)	//!< pin PB12
#define PD_PB13				(PD_PORT_B | PD_PIN_13)	//!< pin PB13
#define PD_PB14				(PD_PORT_B | PD_PIN_14)	//!< pin PB14
#define PD_PB15				(PD_PORT_B | PD_PIN_15)	//!< pin PB15

#define PD_PC0				(PD_PORT_C | PD_PIN_0)	//!< pin PC0
#define PD_PC1				(PD_PORT_C | PD_PIN_1)	//!< pin PC1
#define PD_PC2				(PD_PORT_C | PD_PIN_2)	//!< pin PC2
#define PD_PC3				(PD_PORT_C | PD_PIN_3)	//!< pin PC3
#define PD_PC4				(PD_PORT_C | PD_PIN_4)	//!< pin PC4
#define PD_PC5				(PD_PORT_C | PD_PIN_5)	//!< pin PC5
#define PD_PC6				(PD_PORT_C | PD_PIN_6)	//!< pin PC6
#define PD_PC7				(PD_PORT_C | PD_PIN_7)	//!< pin PC7
#define PD_PC8				(PD_PORT_C | PD_PIN_8)	//!< pin PC8
#define PD_PC9				(PD_PORT_C | PD_PIN_9)	//!< pin PC9
#define PD_PC10				(PD_PORT_C | PD_PIN_10)	//!< pin PC10
#define PD_PC11				(PD_PORT_C | PD_PIN_11)	//!< pin PC11
#define PD_PC12				(PD_PORT_C | PD_PIN_12)	//!< pin PC12
#define PD_PC13				(PD_PORT_C | PD_PIN_13)	//!< pin PC13
#define PD_PC14				(PD_PORT_C | PD_PIN_14)	//!< pin PC14
#define PD_PC15				(PD_PORT_C | PD_PIN_15)	//!< pin PC15

#define PD_PD0				(PD_PORT_D | PD_PIN_0)	//!< pin PD0
#define PD_PD1				(PD_PORT_D | PD_PIN_1)	//!< pin PD1
#define PD_PD2				(PD_PORT_D | PD_PIN_2)	//!< pin PD2
#define PD_PD3				(PD_PORT_D | PD_PIN_3)	//!< pin PD3
#define PD_PD4				(PD_PORT_D | PD_PIN_4)	//!< pin PD4
#define PD_PD5				(PD_PORT_D | PD_PIN_5)	//!< pin PD5
#define PD_PD6				(PD_PORT_D | PD_PIN_6)	//!< pin PD6
#define PD_PD7				(PD_PORT_D | PD_PIN_7)	//!< pin PD7
#define PD_PD8				(PD_PORT_D | PD_PIN_8)	//!< pin PD8
#define PD_PD9				(PD_PORT_D | PD_PIN_9)	//!< pin PD9
#define PD_PD10				(PD_PORT_D | PD_PIN_10)	//!< pin PD10
#define PD_PD11				(PD_PORT_D | PD_PIN_11)	//!< pin PD11
#define PD_PD12				(PD_PORT_D | PD_PIN_12)	//!< pin PD12
#define PD_PD13				(PD_PORT_D | PD_PIN_13)	//!< pin PD13
#define PD_PD14				(PD_PORT_D | PD_PIN_14)	//!< pin PD14
#define PD_PD15				(PD_PORT_D | PD_PIN_15)	//!< pin PD15

#define PD_PE0				(PD_PORT_E | PD_PIN_0)	//!< pin PE0
#define PD_PE1				(PD_PORT_E | PD_PIN_1)	//!< pin PE1
#define PD_PE2				(PD_PORT_E | PD_PIN_2)	//!< pin PE2
#define PD_PE3				(PD_PORT_E | PD_PIN_3)	//!< pin PE3
#define PD_PE4				(PD_PORT_E | PD_PIN_4)	//!< pin PE4
#define PD_PE5				(PD_PORT_E | PD_PIN_5)	//!< pin PE5
#define PD_PE6				(PD_PORT_E | PD_PIN_6)	//!< pin PE6
#define PD_PE7				(PD_PORT_E | PD_PIN_7)	//!< pin PE7
#define PD_PE8				(PD_PORT_E | PD_PIN_8)	//!< pin PE8
#define PD_PE9				(PD_PORT_E | PD_PIN_9)	//!< pin PE9
#define PD_PE10				(PD_PORT_E | PD_PIN_10)	//!< pin PE10
#define PD_PE11				(PD_PORT_E | PD_PIN_11)	//!< pin PE11
#define PD_PE12				(PD_PORT_E | PD_PIN_12)	//!< pin PE12
#define PD_PE13				(PD_PORT_E | PD_PIN_13)	//!< pin PE13
#define PD_PE14				(PD_PORT_E | PD_PIN_14)	//!< pin PE14
#define PD_PE15				(PD_PORT_E | PD_PIN_15)	//!< pin PE15

#define PD_PF0				(PD_PORT_F | PD_PIN_0)	//!< pin PF0
#define PD_PF1				(PD_PORT_F | PD_PIN_1)	//!< pin PF1
#define PD_PF2				(PD_PORT_F | PD_PIN_2)	//!< pin PF2
#define PD_PF3				(PD_PORT_F | PD_PIN_3)	//!< pin PF3
#define PD_PF4				(PD_PORT_F | PD_PIN_4)	//!< pin PF4
#define PD_PF5				(PD_PORT_F | PD_PIN_5)	//!< pin PF5
#define PD_PF6				(PD_PORT_F | PD_PIN_6)	//!< pin PF6
#define PD_PF7				(PD_PORT_F | PD_PIN_7)	//!< pin PF7
#define PD_PF8				(PD_PORT_F | PD_PIN_8)	//!< pin PF8
#define PD_PF9				(PD_PORT_F | PD_PIN_9)	//!< pin PF9
#define PD_PF10				(PD_PORT_F | PD_PIN_10)	//!< pin PF10
#define PD_PF11				(PD_PORT_F | PD_PIN_11)	//!< pin PF11
#define PD_PF12				(PD_PORT_F | PD_PIN_12)	//!< pin PF12
#define PD_PF13				(PD_PORT_F | PD_PIN_13)	//!< pin PF13
#define PD_PF14				(PD_PORT_F | PD_PIN_14)	//!< pin PF14
#define PD_PF15				(PD_PORT_F | PD_PIN_15)	//!< pin PF15

#define PD_PG0				(PD_PORT_G | PD_PIN_0)	//!< pin PG0
#define PD_PG1				(PD_PORT_G | PD_PIN_1)	//!< pin PG1
#define PD_PG2				(PD_PORT_G | PD_PIN_2)	//!< pin PG2
#define PD_PG3				(PD_PORT_G | PD_PIN_3)	//!< pin PG3
#define PD_PG4				(PD_PORT_G | PD_PIN_4)	//!< pin PG4
#define PD_PG5				(PD_PORT_G | PD_PIN_5)	//!< pin PG5
#define PD_PG6				(PD_PORT_G | PD_PIN_6)	//!< pin PG6
#define PD_PG7				(PD_PORT_G | PD_PIN_7)	//!< pin PG7
#define PD_PG8				(PD_PORT_G | PD_PIN_8)	//!< pin PG8
#define PD_PG9				(PD_PORT_G | PD_PIN_9)	//!< pin PG9
#define PD_PG10				(PD_PORT_G | PD_PIN_10)	//!< pin PG10
#define PD_PG11				(PD_PORT_G | PD_PIN_11)	//!< pin PG11
#define PD_PG12				(PD_PORT_G | PD_PIN_12)	//!< pin PG12
#define PD_PG13				(PD_PORT_G | PD_PIN_13)	//!< pin PG13
#define PD_PG14				(PD_PORT_G | PD_PIN_14)	//!< pin PG14
#define PD_PG15				(PD_PORT_G | PD_PIN_15)	//!< pin PG15

#define PD_PH0				(PD_PORT_H | PD_PIN_0)	//!< pin PH0
#define PD_PH1				(PD_PORT_H | PD_PIN_1)	//!< pin PH1
#define PD_PH2				(PD_PORT_H | PD_PIN_2)	//!< pin PH2
#define PD_PH3				(PD_PORT_H | PD_PIN_3)	//!< pin PH3
#define PD_PH4				(PD_PORT_H | PD_PIN_4)	//!< pin PH4
#define PD_PH5				(PD_PORT_H | PD_PIN_5)	//!< pin PH5
#define PD_PH6				(PD_PORT_H | PD_PIN_6)	//!< pin PH6
#define PD_PH7				(PD_PORT_H | PD_PIN_7)	//!< pin PH7
#define PD_PH8				(PD_PORT_H | PD_PIN_8)	//!< pin PH8
#define PD_PH9				(PD_PORT_H | PD_PIN_9)	//!< pin PH9
#define PD_PH10				(PD_PORT_H | PD_PIN_10)	//!< pin PH10
#define PD_PH11				(PD_PORT_H | PD_PIN_11)	//!< pin PH11
#define PD_PH12				(PD_PORT_H | PD_PIN_12)	//!< pin PH12
#define PD_PH13				(PD_PORT_H | PD_PIN_13)	//!< pin PH13
#define PD_PH14				(PD_PORT_H | PD_PIN_14)	//!< pin PH14
#define PD_PH15				(PD_PORT_H | PD_PIN_15)	//!< pin PH15

#define PD_PI0				(PD_PORT_I | PD_PIN_0)	//!< pin PI0
#define PD_PI1				(PD_PORT_I | PD_PIN_1)	//!< pin PI1
#define PD_PI2				(PD_PORT_I | PD_PIN_2)	//!< pin PI2
#define PD_PI3				(PD_PORT_I | PD_PIN_3)	//!< pin PI3
#define PD_PI4				(PD_PORT_I | PD_PIN_4)	//!< pin PI4
#define PD_PI5				(PD_PORT_I | PD_PIN_5)	//!< pin PI5
#define PD_PI6				(PD_PORT_I | PD_PIN_6)	//!< pin PI6
#define PD_PI7				(PD_PORT_I | PD_PIN_7)	//!< pin PI7
#define PD_PI8				(PD_PORT_I | PD_PIN_8)	//!< pin PI8
#define PD_PI9				(PD_PORT_I | PD_PIN_9)	//!< pin PI9
#define PD_PI10				(PD_PORT_I | PD_PIN_10)	//!< pin PI10
#define PD_PI11				(PD_PORT_I | PD_PIN_11)	//!< pin PI11
#define PD_PI12				(PD_PORT_I | PD_PIN_12)	//!< pin PI12
#define PD_PI13				(PD_PORT_I | PD_PIN_13)	//!< pin PI13
#define PD_PI14				(PD_PORT_I | PD_PIN_14)	//!< pin PI14
#define PD_PI15				(PD_PORT_I | PD_PIN_15)	//!< pin PI15

#define PD_IN				(PD_MODE_INPUT ) 					//!< input pin
#define PD_OUT				(PD_MODE_OUTPUT | PD_OSPEED_2MHz) 	//!< output pin
#define PD_AF				(PD_MODE_AF)						//!< open drain pin
#define PD_ANALOG			(PD_MODE_ANALOG)					//!< open drain pin

#define PD_AF_OUT_2MHz		(PD_MODE_AF | PD_OSPEED_2MHz) 	//!< AF output 2MHz pin
#if CFG_SERIES == stm32f1
#define PD_AF_OUT_10MHz		(PD_MODE_AF | PD_OSPEED_10MHz) 	//!< AF output 10MHz pin
#define PD_AF_OUT_50MHz		(PD_MODE_AF | PD_OSPEED_50MHz) 	//!< AF output 50MHz pin
#else
#define PD_AF_OUT_25MHz		(PD_MODE_AF | PD_OSPEED_25MHz) 	//!< AF output 25MHz pin
#define PD_AF_OUT_50MHz		(PD_MODE_AF | PD_OSPEED_50MHz) 	//!< AF output 50MHz pin
#define PD_AF_OUT_100MHz	(PD_MODE_AF | PD_OSPEED_100MHz) //!< AF output 100MHz pin
#endif


/** @} */ // @STD_GPIO


#endif /* GPIO_STM32_H_ */
