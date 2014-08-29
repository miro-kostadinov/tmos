/*
 * gpio_tm4c.h
 *
 *  Created on: Aug 26, 2014
 *      Author: miro
 */

#ifndef GPIO_TM4C_H_
#define GPIO_TM4C_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  General-Purpose Input/Outputs (GPIOs)
 ******************************************************************************/
struct GPIO_TypeDef
{
	__IO uint32_t GPIODATA[256];  //!< (gpio: 0x000) GPIO Data
	__IO uint32_t GPIODIR;        //!< (gpio: 0x400) GPIO Direction
	__IO uint32_t GPIOIS;         //!< (gpio: 0x404) GPIO Interrupt Sense
	__IO uint32_t GPIOIBE;        //!< (gpio: 0x408) GPIO Interrupt Both Edges
	__IO uint32_t GPIOIEV;        //!< (gpio: 0x40C) GPIO Interrupt Event
	__IO uint32_t GPIOIM;         //!< (gpio: 0x410) GPIO Interrupt Mask
	__I  uint32_t GPIORIS;        //!< (gpio: 0x414) GPIO Raw Interrupt Status
	__I  uint32_t GPIOMIS;        //!< (gpio: 0x418) GPIO Masked Interrupt Status
	__O  uint32_t GPIOICR;        //!< (gpio: 0x41C) GPIO Interrupt Clear
	__IO uint32_t GPIOAFSEL;      //!< (gpio: 0x420) GPIO Alternate Function Select
		 uint32_t reserved1[55];  //!< (gpio: 0x424)
	__IO uint32_t GPIODR2R;       //!< (gpio: 0x500) GPIO 2-mA Drive Select
	__IO uint32_t GPIODR4R;       //!< (gpio: 0x504) GPIO 4-mA Drive Select
	__IO uint32_t GPIODR8R;       //!< (gpio: 0x508) GPIO 8-mA Drive Select
	__IO uint32_t GPIOODR;        //!< (gpio: 0x50C) GPIO Open Drain Select
	__IO uint32_t GPIOPUR;        //!< (gpio: 0x510) GPIO Pull-Up Select
	__IO uint32_t GPIOPDR;        //!< (gpio: 0x514) GPIO Pull-Down Select
	__IO uint32_t GPIOSLR;        //!< (gpio: 0x518) GPIO Slew Rate Control Select
	__IO uint32_t GPIODEN;        //!< (gpio: 0x51C) GPIO Digital Enable
	__IO uint32_t GPIOLOCK;       //!< (gpio: 0x520) GPIO Lock
	__IO uint32_t GPIOCR;         //!< (gpio: 0x524) GPIO Commit
	__IO uint32_t GPIOAMSEL;      //!< (gpio: 0x528) GPIO Analog Mode Select
	__IO uint32_t GPIOPCTL;       //!< (gpio: 0x52C) GPIO Port Control

	__IO uint32_t GPIOADCCTL; 	  //!< (gpio: 0x530) GPIO ADC Control
	__IO uint32_t GPIODMACTL; 	  //!< (gpio: 0x534) GPIO DMA Control
	__IO uint32_t GPIOSI; 		  //!< (gpio: 0x538) GPIO Select Interrupt
	__IO uint32_t GPIODR12R; 	  //!< (gpio: 0x53C) GPIO 12-mA Drive Select
	__IO uint32_t GPIOWAKEPEN; 	  //!< (gpio: 0x540) GPIO Wake Pin Enable
	__IO uint32_t GPIOWAKELVL; 	  //!< (gpio: 0x544) GPIO Wake Level
	__I  uint32_t GPIOWAKESTAT;   //!< (gpio: 0x548) GPIO Wake Status
		 uint32_t reserved2[669]; //!< (gpio: 0x54C)
	__I  uint32_t GPIOPP; 		  //!< (gpio: 0xFC0) GPIO Peripheral Property
	__IO uint32_t GPIOPC; 		  //!< (gpio: 0xFC4) GPIO Peripheral Configuration
	     uint32_t reserved3[2];   //!< (gpio: 0xFC8)
 	__I  uint32_t GPIOPeriphID4[4];  //!< (gpio: 0xFD0) GPIO Peripheral Identification 4-7
	__I  uint32_t GPIOPeriphID0[4];  //!< (gpio: 0xFE0) GPIO Peripheral Identification 0-3
	__I  uint32_t GPIOPCellID[4];    //!< (gpio: 0xFF0) GPIO PrimeCell Identification 0-3
  };

/*******************************************************************************
 * @defgroup GPIO_regs_define
 * @{
 ******************************************************************************/

/** @defgroup GPIOLOCK:		(gpio: 0x520) GPIO Lock						      */
#define GPIO_GPIOLOCK_LOCK				0x00000001 		//!< GPIO Lock
#define GPIO_GPIOLOCK_KEY				0x4C4F434B 		//!< GPIO Lock key
/** @} */

/** @} */ // @defgroup GPIO_regs_define

/*******************************************************************************
 * @defgroup STD_PIO Standard PIO definitions
 * @{
 ******************************************************************************/

/** Pin set **/
typedef unsigned char pio_set;

/** Pin definition **/
typedef unsigned int pio_def;

/** Pin configuration **/
typedef unsigned int PIN_DESC;


/// Pin definition bits 31:24 = pin set
#define PD_PINS_Pos			24						//!< pins bits position
#define PD_PINS_Msk			(0xFF << PD_PINS_Pos)	//!< pins bits mask
#define PD_PINS_Get(x)		((x) >> PD_PINS_Pos)	//!< pins value
#define PD_PIN_0			(0x01 << PD_PINS_Pos)	//!< pin 0
#define PD_PIN_1			(0x02 << PD_PINS_Pos)	//!< pin 1
#define PD_PIN_2			(0x04 << PD_PINS_Pos)	//!< pin 2
#define PD_PIN_3			(0x08 << PD_PINS_Pos)	//!< pin 3
#define PD_PIN_4			(0x10 << PD_PINS_Pos)	//!< pin 4
#define PD_PIN_5			(0x20 << PD_PINS_Pos)	//!< pin 5
#define PD_PIN_6			(0x40 << PD_PINS_Pos)	//!< pin 6
#define PD_PIN_7			(0x80 << PD_PINS_Pos)	//!< pin 7

/// Pin definition bits 23:22 = pin strength
#define PD_STRENGTH_Pos 	22						//!< strength bits position
#define PD_STRENGTH_Msk		(0x3 << PD_STRENGTH_Pos)//!< strength bits mask
#define PD_STRENGTH_Get(x)	(0x3 & ((x) >> PD_STRENGTH_Pos)) //!< strength mask
#define PD_STRENGTH_2MA     (0 << PD_STRENGTH_Pos)	//!< 2mA drive strength
#define PD_STRENGTH_4MA     (1 << PD_STRENGTH_Pos)	//!< 4mA drive strength
#define PD_STRENGTH_8MA     (2 << PD_STRENGTH_Pos)  //!< 8mA drive strength

/// Pin definition bit 21 = slew rate
#define PD_SLEW_RATE		(1 << 21)

/// Pin definition bits 20:16 = port
#define PD_PORT_Pos			16						//!< port bits position
#define PD_PORT_Msk			(0x1F << PD_PORT_Pos)	//!< port bits mask
#define PD_PORT_Get(x)		(0x1F & ((x) >> PD_PORT_Pos)) //!< port value
#define	PD_PORT_A			((unsigned int)0 << PD_PORT_Pos)		//!< port A
#define	PD_PORT_B			((unsigned int)1 << PD_PORT_Pos)		//!< port B
#define	PD_PORT_C			((unsigned int)2 << PD_PORT_Pos)		//!< port C
#define	PD_PORT_D			((unsigned int)3 << PD_PORT_Pos)		//!< port D
#define	PD_PORT_E			((unsigned int)4 << PD_PORT_Pos)		//!< port E
#define	PD_PORT_F			((unsigned int)5 << PD_PORT_Pos)		//!< port F
#define	PD_PORT_G			((unsigned int)6 << PD_PORT_Pos)		//!< port G
#define	PD_PORT_H			((unsigned int)7 << PD_PORT_Pos)		//!< port H
#define	PD_PORT_J			((unsigned int)8 << PD_PORT_Pos)		//!< port J
#define	PD_PORT_K			((unsigned int)9 << PD_PORT_Pos)		//!< port K
#define	PD_PORT_L			((unsigned int)10 << PD_PORT_Pos)		//!< port L
#define	PD_PORT_M			((unsigned int)11 << PD_PORT_Pos)		//!< port M
#define	PD_PORT_N			((unsigned int)12 << PD_PORT_Pos)		//!< port N
#define	PD_PORT_P			((unsigned int)13 << PD_PORT_Pos)		//!< port P
#define	PD_PORT_Q			((unsigned int)14 << PD_PORT_Pos)		//!< port Q
#define PD_PORT_V           ((unsigned int)0xF << PD_PORT_Pos)	//!< virtual port V
#define PD_LAST_PORT		14						//!< number of physical ports

/// Pin definition bits 15:12 = pin mux
#define PD_MUX_Pos			12						//!< mux bits position
#define PD_MUX_Msk			(0xF << PD_MUX_Pos)		//!< mux bits mask
#define PD_MUX_Get(x)		(0xF & ((x) >> PD_MUX_Pos)) //!< mux value
#define PD_MUX_0			(0 << PD_MUX_Pos)		//!< mux configuration 0
#define PD_MUX_1			(1 << PD_MUX_Pos)		//!< mux configuration 1
#define PD_MUX_2			(2 << PD_MUX_Pos)		//!< mux configuration 2
#define PD_MUX_3			(3 << PD_MUX_Pos)		//!< mux configuration 3
#define PD_MUX_4			(4 << PD_MUX_Pos)		//!< mux configuration 4
#define PD_MUX_5			(5 << PD_MUX_Pos)		//!< mux configuration 5
#define PD_MUX_6			(6 << PD_MUX_Pos)		//!< mux configuration 6
#define PD_MUX_7			(7 << PD_MUX_Pos)		//!< mux configuration 7
#define PD_MUX_8			(8 << PD_MUX_Pos)		//!< mux configuration 8
#define PD_MUX_9			(9 << PD_MUX_Pos)		//!< mux configuration 9
#define PD_MUX_10			(10 << PD_MUX_Pos)		//!< mux configuration 10
#define PD_MUX_11			(11 << PD_MUX_Pos)		//!< mux configuration 11

/// Pin definition bits 11:8 = pin mode
#define PD_MODE_Pos 		8						//!< mode bits position
#define PD_MODE_Msk			(0xF << PD_MODE_Pos)	//!< mode bits mask
#define PD_MODE_Get(x)		(0xF & ((x) >> PD_MODE_Pos)) //!< mode value
#define PD_MODE_ANALOG		(0 << PD_MODE_Pos)		//!< enable analog mode
#define PD_PUSH_PULL  		(1 << PD_MODE_Pos)		//!< standard push pull mode
#define PD_MODE_OD        	(2 << PD_MODE_Pos)		//!< open drain mode
#define PD_PULL_UP   		(4 << PD_MODE_Pos)		//!< weak pull up
#define PD_PULL_DOWN   		(8 << PD_MODE_Pos)		//!< weak pull down

/// Pin definition bit 7 = direction
#define PD_INPUT        	(0 << 7)				//!< input pin
#define PD_OUTPUT       	(1 << 7)				//!< output pin

/// Pin definition bit 6 = peripheral
#define PD_AFSEL 		    (1 << 6)

/// Pin definition bits 5:3 = interrupts
#define PD_INT_Pos 			3						//!< interrupt bits position
#define PD_INT_ENABLE		(1 << PD_INT_Pos)		//!< interrupt enabled
#define PD_INT_SENSE		(2 << PD_INT_Pos)		//!< sense - level or edge
#define PD_INT_BE			(4 << PD_INT_Pos)		//!< both edges
#define PD_INT_EVENT		(8 << PD_INT_Pos)		//!< high/low level or rising/falling

/// Pin definition bit 2 = Lock
#define PD_LOCK     	  	(1 << 2)

#define PD_FALLING_EDGE     (PD_INT_ENABLE)			//!< interrupt on falling edge
#define PD_RISING_EDGE      (PD_INT_ENABLE | PD_INT_EVENT)	//!< interrupt on rising edge
#define PD_BOTH_EDGES       (PD_INT_ENABLE | PD_INT_BE)	//!< interrupt on any edge
#define PD_LOW_LEVEL        (PD_INT_ENABLE | PD_INT_SENSE) //!< interrupt on low level
#define PD_HIGH_LEVEL       (PD_INT_ENABLE | PD_INT_SENSE | PD_INT_EVENT) //!< interrupt on high level

/// Pin definition bit 1 = Active level
#define PD_ACTIVE_HIGH 	  	(1 << 1)
#define PD_ACTIVE_LOW 	  	(0 << 1)

//*****************************************************************************
//
// Values that can be passed to Pin_Configure, Pin_Write, Pin_Read as the PIN_DESC parameter.
//
//*****************************************************************************

// GPIO pin A0
#define PD_PA0				(PD_PORT_A | PD_PIN_0 | PD_MUX_0)  //!< PA0
#define PD_PA0_U0RX         (PD_PORT_A | PD_PIN_0 | PD_MUX_1)  //!< PA0 as U0RX
#define PD_PA0_I2C9SCL      (PD_PORT_A | PD_PIN_0 | PD_MUX_2)  //!< PA0_as I2C9SCL
#define PD_PA0_T0CCP0       (PD_PORT_A | PD_PIN_0 | PD_MUX_3)  //!< PA0 as T0CCP0
#define PD_PA0_CAN0Rx       (PD_PORT_A | PD_PIN_0 | PD_MUX_7)  //!< PA0 as CAN0Rx

// GPIO pin A1
#define PD_PA1				(PD_PORT_A | PD_PIN_1 | PD_MUX_0)  //!< PA1
#define PD_PA1_U0Tx         (PD_PORT_A | PD_PIN_1 | PD_MUX_1)  //!< PA1 as U0TX
#define PD_PA1_I2C9SDA      (PD_PORT_A | PD_PIN_1 | PD_MUX_2)  //!< PA1 as I2C9SDA
#define PD_PA1_T0CCP1       (PD_PORT_A | PD_PIN_1 | PD_MUX_3)  //!< PA1 as T0CCP1
#define PD_PA1_CAN0Tx       (PD_PORT_A | PD_PIN_1 | PD_MUX_7)  //!< PA1 as CAN0Tx

// GPIO pin A2
#define PD_PA2				(PD_PORT_A | PD_PIN_2 | PD_MUX_0)  //!< PA2
#define PD_PA2_U4Rx         (PD_PORT_A | PD_PIN_2 | PD_MUX_1)  //!< PA2 as U4Rx
#define PD_PA2_I2C8SCL      (PD_PORT_A | PD_PIN_2 | PD_MUX_2)  //!< PA2 as I2C8SCL
#define PD_PA2_T1CCP0       (PD_PORT_A | PD_PIN_2 | PD_MUX_3)  //!< PA2 as T1CCP0
#define PD_PA2_SSI0Clk      (PD_PORT_A | PD_PIN_2 | PD_MUX_15) //!< PA2 as SSI0Clk

// GPIO pin A3
#define PD_PA3				(PD_PORT_A | PD_PIN_3 | PD_MUX_0)  //!< PA3
#define PD_PA3_U4Tx         (PD_PORT_A | PD_PIN_3 | PD_MUX_1)  //!< PA3 as U4Tx
#define PD_PA3_I2C8SDA      (PD_PORT_A | PD_PIN_3 | PD_MUX_2)  //!< PA3 as I2C8SDA
#define PD_PA3_T1CCP1       (PD_PORT_A | PD_PIN_3 | PD_MUX_3)  //!< PA3 as T1CCP1
#define PD_PA3_SSI0Fss      (PD_PORT_A | PD_PIN_3 | PD_MUX_15) //!< PA3 as SSI0Fss

// GPIO pin A4
#define PD_PA4				(PD_PORT_A | PD_PIN_4 | PD_MUX_0)  //!< PA4
#define PD_PA4_U3Rx         (PD_PORT_A | PD_PIN_4 | PD_MUX_1)  //!< PA4 as U3Rx
#define PD_PA4_I2C7SCL      (PD_PORT_A | PD_PIN_4 | PD_MUX_2)  //!< PA4 as I2C7SCL
#define PD_PA4_T2CCP0       (PD_PORT_A | PD_PIN_4 | PD_MUX_3)  //!< PA4 as T2CCP0
#define PD_PA4_SSI0XDAT0    (PD_PORT_A | PD_PIN_4 | PD_MUX_15) //!< PA4 as SSI0XDAT0

// GPIO pin A5
#define PD_PA5				(PD_PORT_A | PD_PIN_5 | PD_MUX_0)  //!< PA5
#define PD_PA5_U3Tx         (PD_PORT_A | PD_PIN_5 | PD_MUX_1)  //!< PA5 as U3Tx
#define PD_PA5_I2C7SDA      (PD_PORT_A | PD_PIN_5 | PD_MUX_2)  //!< PA5 as I2C7SDA
#define PD_PA5_T2CCP1       (PD_PORT_A | PD_PIN_5 | PD_MUX_3)  //!< PA5 as T2CCP1
#define PD_PA5_SSI0XDAT1    (PD_PORT_A | PD_PIN_5 | PD_MUX_15) //!< PA5 as SSI0XDAT1

// GPIO pin A6
#define PD_PA6				(PD_PORT_A | PD_PIN_6 | PD_MUX_0)  //!< PA6
#define PD_PA6_U2Rx         (PD_PORT_A | PD_PIN_6 | PD_MUX_1)  //!< PA6 as U2Rx
#define PD_PA6_I2C6SCL      (PD_PORT_A | PD_PIN_6 | PD_MUX_2)  //!< PA6 as I2C6SCL
#define PD_PA6_T3CCP0       (PD_PORT_A | PD_PIN_6 | PD_MUX_3)  //!< PA6 as T3CCP0
#define PD_PA6_USB0EPEN     (PD_PORT_A | PD_PIN_6 | PD_MUX_5)  //!< PA6 as USB0EPEN
#define PD_PA6_SSI0XDAT2    (PD_PORT_A | PD_PIN_6 | PD_MUX_13) //!< PA6 as SSI0XDAT2
#define PD_PA6_EPI0S8       (PD_PORT_A | PD_PIN_6 | PD_MUX_15) //!< PA6 as EPI0S8

// GPIO pin A7
#define PD_PA7				(PD_PORT_A | PD_PIN_7 | PD_MUX_0)  //!< PA7
#define PD_PA7_U2Tx         (PD_PORT_A | PD_PIN_7 | PD_MUX_1)  //!< PA7 as U2Tx
#define PD_PA7_I2C6SDA      (PD_PORT_A | PD_PIN_7 | PD_MUX_2)  //!< PA7 as I2C6SDA
#define PD_PA7_T3CCP1       (PD_PORT_A | PD_PIN_7 | PD_MUX_3)  //!< PA7 as T3CCP1
#define PD_PA7_USB0PFLT     (PD_PORT_A | PD_PIN_7 | PD_MUX_5)  //!< PA7 as USB0PFLT
#define PD_PA7_USB0EPEN     (PD_PORT_A | PD_PIN_7 | PD_MUX_11) //!< PA7 as USB0EPEN
#define PD_PA7_SSI0XDAT3    (PD_PORT_A | PD_PIN_7 | PD_MUX_13) //!< PA7 as SSI0XDAT3
#define PD_PA7_EPI0S9       (PD_PORT_A | PD_PIN_7 | PD_MUX_15) //!< PA7 as EPI0S9


// GPIO pin B0
#define PD_PB0				(PD_PORT_B | PD_PIN_0 | PD_MUX_0)  //!< PB0
#define PD_PB0_U1Rx         (PD_PORT_B | PD_PIN_0 | PD_MUX_1)  //!< PB0 as U1Rx
#define PD_PB0_I2C5SCL      (PD_PORT_B | PD_PIN_0 | PD_MUX_2)  //!< PB0 as I2C5SCL
#define PD_PB0_T4CCP0       (PD_PORT_B | PD_PIN_0 | PD_MUX_3)  //!< PB0 as T4CCP0
#define PD_PB0_CAN1Rx       (PD_PORT_B | PD_PIN_0 | PD_MUX_7)  //!< PB0 as CAN1Rx

// GPIO pin B1
#define PD_PB1				(PD_PORT_B | PD_PIN_1 | PD_MUX_0)  //!< PB1
#define PD_PB1_U1Tx         (PD_PORT_B | PD_PIN_1 | PD_MUX_1)  //!< PB1 as U1Tx
#define PD_PB1_I2C5SDA      (PD_PORT_B | PD_PIN_1 | PD_MUX_2)  //!< PB1 as I2C5SDA
#define PD_PB1_T4CCP1       (PD_PORT_B | PD_PIN_1 | PD_MUX_3)  //!< PB1 as T4CCP1
#define PD_PB1_CAN1Tx       (PD_PORT_B | PD_PIN_1 | PD_MUX_7)  //!< PB1 as CAN1Tx

// GPIO pin B2
#define PD_PB2				(PD_PORT_B | PD_PIN_2 | PD_MUX_0)  //!< PB2
#define PD_PB2_I2C0SCL      (PD_PORT_B | PD_PIN_2 | PD_MUX_2)  //!< PB2 as I2C0SCL
#define PD_PB2_T5CCP0       (PD_PORT_B | PD_PIN_2 | PD_MUX_3)  //!< PB2 as T5CCP0
#define PD_PB2_USB0STP      (PD_PORT_B | PD_PIN_2 | PD_MUX_14) //!< PB2 as USB0STP
#define PD_PB2_EPI0S27      (PD_PORT_B | PD_PIN_2 | PD_MUX_15) //!< PB2 as EPI0S27

// GPIO pin B3
#define PD_PB3				(PD_PORT_B | PD_PIN_3 | PD_MUX_0)  //!< PB3
#define PD_PB3_I2C0SDA      (PD_PORT_B | PD_PIN_3 | PD_MUX_2)  //!< PB3 as I2C0SDA
#define PD_PB3_T5CCP1       (PD_PORT_B | PD_PIN_3 | PD_MUX_3)  //!< PB3 as T5CCP1
#define PD_PB3_USB0CLK      (PD_PORT_B | PD_PIN_3 | PD_MUX_14) //!< PB3 as USB0CLK
#define PD_PB3_EPI0S28      (PD_PORT_B | PD_PIN_3 | PD_MUX_15) //!< PB3 as EPI0S28

// GPIO pin B4
#define PD_PB4				(PD_PORT_B | PD_PIN_4 | PD_MUX_0)  //!< PB4
#define PD_PB4_U0CTS        (PD_PORT_B | PD_PIN_4 | PD_MUX_1)  //!< PB4 as U0CTS
#define PD_PB4_I2C5SCL      (PD_PORT_B | PD_PIN_4 | PD_MUX_2)  //!< PB4 as I2C5SCL
#define PD_PB4_SSI1Fss      (PD_PORT_B | PD_PIN_4 | PD_MUX_15) //!< PB4 as SSI1Fss

// GPIO pin B5
#define PD_PB5				(PD_PORT_B | PD_PIN_5 | PD_MUX_0)  //!< PB5
#define PD_PB5_U0RTS        (PD_PORT_B | PD_PIN_5 | PD_MUX_1)  //!< PB5 as U0RTS
#define PD_PB5_I2C5SDA      (PD_PORT_B | PD_PIN_5 | PD_MUX_2)  //!< PB5 as I2C5SDA
#define PD_PB5_SSI1Clk      (PD_PORT_B | PD_PIN_5 | PD_MUX_15) //!< PB5 as SSI1Clk


// GPIO pin C0
#define PD_PC0				(PD_PORT_C | PD_PIN_0 | PD_MUX_0)  //!< PC0
#define PD_PC0_TCK          (PD_PORT_C | PD_PIN_0 | PD_MUX_1)  //!< PC0 as TCK

// GPIO pin C1
#define PD_PC1				(PD_PORT_C | PD_PIN_1 | PD_MUX_0)  //!< PC1
#define PD_PC1_TMS			(PD_PORT_C | PD_PIN_1 | PD_MUX_1)  //!< PC1 as TMS

// GPIO pin C2
#define PD_PC2				(PD_PORT_C | PD_PIN_2 | PD_MUX_0)  //!< PC2
#define PD_PC2_TDI			(PD_PORT_C | PD_PIN_2 | PD_MUX_1)  //!< PC2 as TDI

// GPIO pin C3
#define PD_PC3				(PD_PORT_C | PD_PIN_3 | PD_MUX_0)  //!< PC3
#define PD_PC3_TDO			(PD_PORT_C | PD_PIN_3 | PD_MUX_1)  //!< PC3 as TDO

// GPIO pin C4
#define PD_PC4				(PD_PORT_C | PD_PIN_4 | PD_MUX_0)  //!< PC4
#define PD_PC4_U7Rx         (PD_PORT_C | PD_PIN_4 | PD_MUX_1)  //!< PC4 as U7Rx
#define PD_PC4_EPI0S7       (PD_PORT_C | PD_PIN_4 | PD_MUX_15) //!< PC4 as EPI0S7

// GPIO pin C5
#define PD_PC5				(PD_PORT_C | PD_PIN_5 | PD_MUX_0)  //!< PC5
#define PD_PC5_U7Tx         (PD_PORT_C | PD_PIN_5 | PD_MUX_1)  //!< PC5 as U7Tx
#define PD_PC5_RTCCLK       (PD_PORT_C | PD_PIN_5 | PD_MUX_7)  //!< PC5 as RTCCLK
#define PD_PC5_EPI0S6       (PD_PORT_C | PD_PIN_5 | PD_MUX_15) //!< PC5 as EPI0S6

// GPIO pin C6
#define PD_PC6				(PD_PORT_C | PD_PIN_6 | PD_MUX_0)  //!< PC6
#define PD_PC6_U5Rx         (PD_PORT_C | PD_PIN_6 | PD_MUX_1)  //!< PC6 as U5Rx
#define PD_PC6_EPI0S5       (PD_PORT_C | PD_PIN_6 | PD_MUX_15) //!< PC6 as EPI0S5

// GPIO pin C7
#define PD_PC7				(PD_PORT_C | PD_PIN_7 | PD_MUX_0)  //!< PC7
#define PD_PC7_U5Tx         (PD_PORT_C | PD_PIN_7 | PD_MUX_1)  //!< PC7 as U5Tx
#define PD_PC7_EPI0S4       (PD_PORT_C | PD_PIN_7 | PD_MUX_15) //!< PC7 as EPI0S4


// GPIO pin D0
#define PD_PD0				(PD_PORT_D | PD_PIN_0 | PD_MUX_0)  //!< PD0
#define PD_PD0_I2C7SCL      (PD_PORT_D | PD_PIN_0 | PD_MUX_2)  //!< PD0 as I2C7SCL
#define PD_PD0_T0CCP0       (PD_PORT_D | PD_PIN_0 | PD_MUX_3)  //!< PD0 as T0CCP0
#define PD_PD0_C0o          (PD_PORT_D | PD_PIN_0 | PD_MUX_5)  //!< PD0 as C0o
#define PD_PD0_SSI2XDAT1    (PD_PORT_D | PD_PIN_0 | PD_MUX_15) //!< PD0 as SSI2XDAT1

// GPIO pin D1
#define PD_PD1				(PD_PORT_D | PD_PIN_1 | PD_MUX_0)  //!< PD1
#define PD_PD1_I2C7SDA      (PD_PORT_D | PD_PIN_1 | PD_MUX_2)  //!< PD1 as I2C7SDA
#define PD_PD1_T0CCP1       (PD_PORT_D | PD_PIN_1 | PD_MUX_3)  //!< PD1 as T0CCP1
#define PD_PD1_C1o          (PD_PORT_D | PD_PIN_1 | PD_MUX_5)  //!< PD1 as C1o
#define PD_PD1_SSI2XDAT0    (PD_PORT_D | PD_PIN_1 | PD_MUX_15) //!< PD1 as SSI2XDAT0

// GPIO pin D2
#define PD_PD2				(PD_PORT_D | PD_PIN_2 | PD_MUX_0)  //!< PD2
#define PD_PD2_I2C8SCL      (PD_PORT_D | PD_PIN_2 | PD_MUX_2)  //!< PD2 as I2C8SCL
#define PD_PD2_T1CCP0       (PD_PORT_D | PD_PIN_2 | PD_MUX_3)  //!< PD2 as T1CCP0
#define PD_PD2_C2o          (PD_PORT_D | PD_PIN_2 | PD_MUX_5)  //!< PD2 as C2o
#define PD_PD2_SSI2Fss      (PD_PORT_D | PD_PIN_2 | PD_MUX_15) //!< PD2 as SSI2Fss

// GPIO pin D3
#define PD_PD3				(PD_PORT_D | PD_PIN_3 | PD_MUX_0)  //!< PD3
#define PD_PD3_I2C8SDA      (PD_PORT_D | PD_PIN_3 | PD_MUX_2)  //!< PD3 as I2C8SDA
#define PD_PD3_T1CCP1       (PD_PORT_D | PD_PIN_3 | PD_MUX_3)  //!< PD3 as T1CCP1
#define PD_PD3_SSI2Clk      (PD_PORT_D | PD_PIN_3 | PD_MUX_15) //!< PD3 as SSI2Clk

// GPIO pin D4
#define PD_PD4				(PD_PORT_D | PD_PIN_4 | PD_MUX_0)  //!< PD4
#define PD_PD4_U2Rx         (PD_PORT_D | PD_PIN_4 | PD_MUX_1)  //!< PD4 as U2Rx
#define PD_PD4_T3CCP0       (PD_PORT_D | PD_PIN_4 | PD_MUX_3)  //!< PD4 as T3CCP0
#define PD_PD4_SSI1XDAT2    (PD_PORT_D | PD_PIN_4 | PD_MUX_15) //!< PD4 as SSI1XDAT2

// GPIO pin D5
#define PD_PD5				(PD_PORT_D | PD_PIN_5 | PD_MUX_0)  //!< PD5
#define PD_PD5_U2Tx         (PD_PORT_D | PD_PIN_5 | PD_MUX_1)  //!< PD5 as U2Tx
#define PD_PD5_T3CCP1       (PD_PORT_D | PD_PIN_5 | PD_MUX_3)  //!< PD5 as T3CCP1
#define PD_PD5_SSI1XDAT3    (PD_PORT_D | PD_PIN_5 | PD_MUX_15) //!< PD5 as SSI1XDAT3

// GPIO pin D6
#define PD_PD6				(PD_PORT_D | PD_PIN_6 | PD_MUX_0)  //!< PD6
#define PD_PD6_U2RTS        (PD_PORT_D | PD_PIN_6 | PD_MUX_1)  //!< PD6 as U2RTS
#define PD_PD6_T4CCP0       (PD_PORT_D | PD_PIN_6 | PD_MUX_3)  //!< PD6 as T4CCP0
#define PD_PD6_USB0EPEN     (PD_PORT_D | PD_PIN_6 | PD_MUX_5)  //!< PD6 as USB0EPEN
#define PD_PD6_SSI2XDAT3    (PD_PORT_D | PD_PIN_6 | PD_MUX_15) //!< PD6 as SSI2XDAT3

// GPIO pin D7
#define PD_PD7				(PD_PORT_D | PD_PIN_7 | PD_MUX_0)  //!< PD7
#define PD_PD7_U2CTS        (PD_PORT_D | PD_PIN_7 | PD_MUX_1)  //!< PD7 as U2CTS
#define PD_PD7_T4CCP1       (PD_PORT_D | PD_PIN_7 | PD_MUX_3)  //!< PD7 as T4CCP1
#define PD_PD7_USB0PFLT     (PD_PORT_D | PD_PIN_7 | PD_MUX_5)  //!< PD7 as USB0PFLT
#define PD_PD7_NMI          (PD_PORT_D | PD_PIN_7 | PD_MUX_8)  //!< PD7 as NMI
#define PD_PD7_SSI2XDAT2    (PD_PORT_D | PD_PIN_7 | PD_MUX_15) //!< PD7 as SSI2XDAT2


// GPIO pin E0
#define PD_PE0				(PD_PORT_E | PD_PIN_0 | PD_MUX_0)  //!< PE0
#define PD_PE0_U1RTS        (PD_PORT_E | PD_PIN_0 | PD_MUX_1)  //!< PE0 as U1RTS

// GPIO pin E1
#define PD_PE1				(PD_PORT_E | PD_PIN_1 | PD_MUX_0)  //!< PE1
#define PD_PE1_U1DSR        (PD_PORT_E | PD_PIN_1 | PD_MUX_1)  //!< PE1 as U1DSR

// GPIO pin E2
#define PD_PE2				(PD_PORT_E | PD_PIN_2 | PD_MUX_0)  //!< PE2
#define PD_PE2_U1DCD        (PD_PORT_E | PD_PIN_2 | PD_MUX_1)  //!< PE2 as U1DCD

// GPIO pin E3
#define PD_PE3				(PD_PORT_E | PD_PIN_3 | PD_MUX_0)  //!< PE3
#define PD_PE3_U1DTR        (PD_PORT_E | PD_PIN_3 | PD_MUX_1)  //!< PE3 as U1DTR

// GPIO pin E4
#define PD_PE4				(PD_PORT_E | PD_PIN_4 | PD_MUX_0)  //!< PE4
#define PD_PE4_U1RI         (PD_PORT_E | PD_PIN_4 | PD_MUX_1)  //!< PE4 as U1RI
#define PD_PE4_SSI1XDAT0    (PD_PORT_E | PD_PIN_4 | PD_MUX_15) //!< PE4 as SSI1XDAT0

// GPIO pin E5
#define PD_PE5				(PD_PORT_E | PD_PIN_5 | PD_MUX_0)  //!< PE5
#define PD_PE5_SSI1XDAT1    (PD_PORT_E | PD_PIN_5 | PD_MUX_15) //!< PE5 as SSI1XDAT1


// GPIO pin F0
#define PD_PF0				(PD_PORT_F | PD_PIN_0 | PD_MUX_0)  //!< PF0
#define PD_PF0_EN0LED0      (PD_PORT_F | PD_PIN_0 | PD_MUX_5)  //!< PF0 as EN0LED0
#define PD_PF0_M0PWM0       (PD_PORT_F | PD_PIN_0 | PD_MUX_6)  //!< PF0 as M0PWM0
#define PD_PF0_SSI3XDAT1    (PD_PORT_F | PD_PIN_0 | PD_MUX_14) //!< PF0 as SSI3XDAT1
#define PD_PF0_TRD2         (PD_PORT_F | PD_PIN_0 | PD_MUX_15) //!< PF0 as TRD2

// GPIO pin F1
#define PD_PF1				(PD_PORT_F | PD_PIN_1 | PD_MUX_0)  //!< PF1
#define PD_PF1_EN0LED2      (PD_PORT_F | PD_PIN_1 | PD_MUX_5)  //!< PF1 as EN0LED2
#define PD_PF1_M0PWM1       (PD_PORT_F | PD_PIN_1 | PD_MUX_6)  //!< PF1 as M0PWM1
#define PD_PF1_SSI3XDAT0    (PD_PORT_F | PD_PIN_1 | PD_MUX_14) //!< PF1 as SSI3XDAT0
#define PD_PF1_TRD1         (PD_PORT_F | PD_PIN_1 | PD_MUX_15) //!< PF1 as TRD1

// GPIO pin F2
#define PD_PF2				(PD_PORT_F | PD_PIN_2 | PD_MUX_0)  //!< PF2
#define PD_PF2_M0PWM2       (PD_PORT_F | PD_PIN_2 | PD_MUX_6)  //!< PF2 as M0PWM2
#define PD_PF2_SSI3Fss      (PD_PORT_F | PD_PIN_2 | PD_MUX_14) //!< PF2 as SSI3Fss
#define PD_PF2_TRD0         (PD_PORT_F | PD_PIN_2 | PD_MUX_15) //!< PF2 as TRD0

// GPIO pin F3
#define PD_PF3				(PD_PORT_F | PD_PIN_3 | PD_MUX_0)  //!< PF3
#define PD_PF3_M0PWM3       (PD_PORT_F | PD_PIN_3 | PD_MUX_6)  //!< PF3 as M0PWM3
#define PD_PF3_SSI3Clk      (PD_PORT_F | PD_PIN_3 | PD_MUX_14) //!< PF3 as SSI3Clk
#define PD_PF3_TRCLK        (PD_PORT_F | PD_PIN_3 | PD_MUX_15) //!< PF3 as TRCLK

// GPIO pin F4
#define PD_PF4				(PD_PORT_F | PD_PIN_4 | PD_MUX_0)  //!< PF4
#define PD_PF4_EN0LED1      (PD_PORT_F | PD_PIN_4 | PD_MUX_5)  //!< PF4 as EN0LED1
#define PD_PF4_M0FAULT0     (PD_PORT_F | PD_PIN_4 | PD_MUX_6)  //!< PF4 as M0FAULT0
#define PD_PF4_SSI3XDAT2    (PD_PORT_F | PD_PIN_4 | PD_MUX_14) //!< PF4 as SSI3XDAT2
#define PD_PF4_TRD3	        (PD_PORT_F | PD_PIN_4 | PD_MUX_15) //!< PF4 as TRD3


// GPIO pin G0
#define PD_PG0				(PD_PORT_G | PD_PIN_0 | PD_MUX_0)  //!< PG0
#define PD_PG0_I2C1SCL      (PD_PORT_G | PD_PIN_0 | PD_MUX_2)  //!< PG0 as I2C1SCL
#define PD_PG0_EN0PPS       (PD_PORT_G | PD_PIN_0 | PD_MUX_5)  //!< PG0 as EN0PPS
#define PD_PG0_M0PWM4       (PD_PORT_G | PD_PIN_0 | PD_MUX_6)  //!< PG0 as M0PWM4
#define PD_PG0_EPI0S11      (PD_PORT_G | PD_PIN_0 | PD_MUX_15) //!< PG0 as EPI0S11

// GPIO pin G1
#define PD_PG1				(PD_PORT_G | PD_PIN_1 | PD_MUX_0)  //!< PG1
#define PD_PG1_I2C1SDA      (PD_PORT_G | PD_PIN_1 | PD_MUX_2)  //!< PG1 as I2C1SDA
#define PD_PG1_M0PWM5       (PD_PORT_G | PD_PIN_1 | PD_MUX_6)  //!< PG1 as M0PWM5
#define PD_PG1_EPI0S10      (PD_PORT_G | PD_PIN_1 | PD_MUX_15) //!< PG1 as EPI0S10


// GPIO pin H0
#define PD_PH0				(PD_PORT_H | PD_PIN_0 | PD_MUX_0)  //!< PH0
#define PD_PH0_U0RTS        (PD_PORT_H | PD_PIN_0 | PD_MUX_1)  //!< PH0 as U0RTS
#define PD_PH0_EPI0S0       (PD_PORT_H | PD_PIN_0 | PD_MUX_15) //!< PH0 as EPI0S0

// GPIO pin H1
#define PD_PH1				(PD_PORT_H | PD_PIN_1 | PD_MUX_0)  //!< PH1
#define PD_PH1_U0CTS        (PD_PORT_H | PD_PIN_1 | PD_MUX_1)  //!< PH1 as U0CTS
#define PD_PH1_EPI0S1       (PD_PORT_H | PD_PIN_1 | PD_MUX_15) //!< PH1 as EPI0S1

// GPIO pin H2
#define PD_PH2				(PD_PORT_H | PD_PIN_2 | PD_MUX_0)  //!< PH2
#define PD_PH2_U0DCD        (PD_PORT_H | PD_PIN_2 | PD_MUX_1)  //!< PH2 as U0DCD
#define PD_PH2_EPI0S2       (PD_PORT_H | PD_PIN_2 | PD_MUX_15) //!< PH2 as EPI0S2

// GPIO pin H3
#define PD_PH3				(PD_PORT_H | PD_PIN_3 | PD_MUX_0)  //!< PH3
#define PD_PH3_U0DSR        (PD_PORT_H | PD_PIN_3 | PD_MUX_1)  //!< PH3 as U0DSR
#define PD_PH3_EPI0S3       (PD_PORT_H | PD_PIN_3 | PD_MUX_15) //!< PH3 as EPI0S3


// GPIO pin J0
#define PD_PJ0				(PD_PORT_J | PD_PIN_0 | PD_MUX_0)  //!< PJ0
#define PD_PJ0_U3Rx         (PD_PORT_J | PD_PIN_0 | PD_MUX_1)  //!< PJ0 as U3Rx
#define PD_PJ0_EN0PPS       (PD_PORT_J | PD_PIN_0 | PD_MUX_5)  //!< PJ0 as EN0PPS

// GPIO pin J1
#define PD_PJ1				(PD_PORT_J | PD_PIN_1 | PD_MUX_0)  //!< PJ1
#define PD_PJ1_U3Tx         (PD_PORT_J | PD_PIN_1 | PD_MUX_1)  //!< PJ1 as U3Tx


// GPIO pin K0
#define PD_PK0				(PD_PORT_K | PD_PIN_0 | PD_MUX_0)  //!< PK0
#define PD_PK0_U4Rx			(PD_PORT_K | PD_PIN_0 | PD_MUX_1)  //!< PK0 as U4Rx
#define PD_PK0_EPI0S0		(PD_PORT_K | PD_PIN_0 | PD_MUX_15) //!< PK0 as EPI0S0

// GPIO pin K1
#define PD_PK1				(PD_PORT_K | PD_PIN_1 | PD_MUX_0)  //!< PK1
#define PD_PK1_U4Tx			(PD_PORT_K | PD_PIN_1 | PD_MUX_1)  //!< PK1 as U4Tx
#define PD_PK1_EPI0S1		(PD_PORT_K | PD_PIN_1 | PD_MUX_15) //!< PK1 as EPI0S1

// GPIO pin K2
#define PD_PK2				(PD_PORT_K | PD_PIN_2 | PD_MUX_0)  //!< PK2
#define PD_PK2_U4RTS		(PD_PORT_K | PD_PIN_2 | PD_MUX_1)  //!< PK2 as U4RTS
#define PD_PK2_EPI0S2		(PD_PORT_K | PD_PIN_2 | PD_MUX_15) //!< PK2 as EPI0S2

// GPIO pin K3
#define PD_PK3				(PD_PORT_K | PD_PIN_3 | PD_MUX_0)  //!< PK3
#define PD_PK3_U4CTS		(PD_PORT_K | PD_PIN_3 | PD_MUX_1)  //!< PK3 as U4CTS
#define PD_PK3_EPI0S3		(PD_PORT_K | PD_PIN_3 | PD_MUX_15) //!< PK3 as EPI0S3

// GPIO pin K4
#define PD_PK4				(PD_PORT_K | PD_PIN_4 | PD_MUX_0)  //!< PK4
#define PD_PK4_I2C3SCL		(PD_PORT_K | PD_PIN_4 | PD_MUX_2)  //!< PK4 as I2C3SCL
#define PD_PK4_EN0LED0		(PD_PORT_K | PD_PIN_4 | PD_MUX_5)  //!< PK4 as EN0LED0
#define PD_PK4_M0PWM6		(PD_PORT_K | PD_PIN_4 | PD_MUX_6)  //!< PK4 as M0PWM6
#define PD_PK4_EPI0S32		(PD_PORT_K | PD_PIN_4 | PD_MUX_15) //!< PK4 as EPI0S32

// GPIO pin K5
#define PD_PK5				(PD_PORT_K | PD_PIN_5 | PD_MUX_0)  //!< PK5
#define PD_PK5_I2C3SDA		(PD_PORT_K | PD_PIN_5 | PD_MUX_2)  //!< PK5 as I2C3SDA
#define PD_PK5_EN0LED2		(PD_PORT_K | PD_PIN_5 | PD_MUX_5)  //!< PK5 as EN0LED2
#define PD_PK5_M0PWM7		(PD_PORT_K | PD_PIN_5 | PD_MUX_6)  //!< PK5 as M0PWM7
#define PD_PK5_EPI0S31		(PD_PORT_K | PD_PIN_5 | PD_MUX_15) //!< PK5 as EPI0S31

// GPIO pin K6
#define PD_PK6				(PD_PORT_K | PD_PIN_6 | PD_MUX_0)  //!< PK6
#define PD_PK6_I2C4SCL		(PD_PORT_K | PD_PIN_6 | PD_MUX_2)  //!< PK6 as I2C4SCL
#define PD_PK6_EN0LED1		(PD_PORT_K | PD_PIN_6 | PD_MUX_5)  //!< PK6 as EN0LED1
#define PD_PK6_M0FAULT1		(PD_PORT_K | PD_PIN_6 | PD_MUX_6)  //!< PK6 as M0FAULT1
#define PD_PK6_EPI0S25		(PD_PORT_K | PD_PIN_6 | PD_MUX_15) //!< PK6 as EPI0S25

// GPIO pin K7
#define PD_PK7				(PD_PORT_K | PD_PIN_7 | PD_MUX_0)  //!< PK7
#define PD_PK7_U0RI			(PD_PORT_K | PD_PIN_7 | PD_MUX_1)  //!< PK7 as U0RI
#define PD_PK7_I2C4SDA		(PD_PORT_K | PD_PIN_7 | PD_MUX_2)  //!< PK7 as I2C4SDA
#define PD_PK7_RTCCLK		(PD_PORT_K | PD_PIN_7 | PD_MUX_5)  //!< PK7 as RTCCLK
#define PD_PK7_M0FAULT2		(PD_PORT_K | PD_PIN_7 | PD_MUX_6)  //!< PK7 as M0FAULT2
#define PD_PK7_EPI0S24		(PD_PORT_K | PD_PIN_7 | PD_MUX_15) //!< PK7 as EPI0S24


// GPIO pin L0
#define PD_PL0				(PD_PORT_L | PD_PIN_0 | PD_MUX_0)  //!< PL0
#define PD_PL0_I2C2SDA		(PD_PORT_L | PD_PIN_0 | PD_MUX_2)  //!< PL0 as I2C2SDA
#define PD_PL0_M0FAULT3		(PD_PORT_L | PD_PIN_0 | PD_MUX_6)  //!< PL0 as M0FAULT3
#define PD_PL0_USB0D0		(PD_PORT_L | PD_PIN_0 | PD_MUX_14) //!< PL0 as USB0D0
#define PD_PL0_EPI0S16		(PD_PORT_L | PD_PIN_0 | PD_MUX_15) //!< PL0 as EPI0S16

// GPIO pin L1
#define PD_PL1				(PD_PORT_L | PD_PIN_1 | PD_MUX_0)  //!< PL1
#define PD_PL1_I2C2SCL		(PD_PORT_L | PD_PIN_1 | PD_MUX_2)  //!< PL1 as I2C2SCL
#define PD_PL1_PhA0			(PD_PORT_L | PD_PIN_1 | PD_MUX_6)  //!< PL1 as PhA0
#define PD_PL1_USB0D1		(PD_PORT_L | PD_PIN_1 | PD_MUX_14) //!< PL1 as USB0D1
#define PD_PL1_EPI0S17		(PD_PORT_L | PD_PIN_1 | PD_MUX_15) //!< PL1 as EPI0S17

// GPIO pin L2
#define PD_PL2				(PD_PORT_L | PD_PIN_2 | PD_MUX_0)  //!< PL2
#define PD_PL2_C0o			(PD_PORT_L | PD_PIN_2 | PD_MUX_5)  //!< PL2 as C0o
#define PD_PL2_PhB0			(PD_PORT_L | PD_PIN_2 | PD_MUX_6)  //!< PL2 as PhB0
#define PD_PL2_USB0D2		(PD_PORT_L | PD_PIN_2 | PD_MUX_14) //!< PL2 as USB0D2
#define PD_PL2_EPI0S18		(PD_PORT_L | PD_PIN_2 | PD_MUX_15) //!< PL2 as EPI0S18

// GPIO pin L3
#define PD_PL3				(PD_PORT_L | PD_PIN_3 | PD_MUX_0)  //!< PL3
#define PD_PL3_C1o			(PD_PORT_L | PD_PIN_3 | PD_MUX_5)  //!< PL3 as C1o
#define PD_PL3_IDX0			(PD_PORT_L | PD_PIN_3 | PD_MUX_6)  //!< PL3 as IDX0
#define PD_PL3_USB0D3		(PD_PORT_L | PD_PIN_3 | PD_MUX_14) //!< PL3 as USB0D3
#define PD_PL3_EPI0S19		(PD_PORT_L | PD_PIN_3 | PD_MUX_15) //!< PL3 as EPI0S19

// GPIO pin L4
#define PD_PL4				(PD_PORT_L | PD_PIN_4 | PD_MUX_0)  //!< PL4
#define PD_PL4_T0CCP0		(PD_PORT_L | PD_PIN_4 | PD_MUX_3)  //!< PL4 as T0CCP0
#define PD_PL4_USB0D4		(PD_PORT_L | PD_PIN_4 | PD_MUX_14) //!< PL4 as USB0D4
#define PD_PL4_EPI0S26		(PD_PORT_L | PD_PIN_4 | PD_MUX_15) //!< PL4 as EPI0S26

// GPIO pin L5
#define PD_PL5				(PD_PORT_L | PD_PIN_5 | PD_MUX_0)  //!< PL5
#define PD_PL5_T0CCP1		(PD_PORT_L | PD_PIN_5 | PD_MUX_3)  //!< PL5 as T0CCP1
#define PD_PL5_USB0D5		(PD_PORT_L | PD_PIN_5 | PD_MUX_14) //!< PL5 as USB0D5
#define PD_PL5_EPI0S33		(PD_PORT_L | PD_PIN_5 | PD_MUX_15) //!< PL5 as EPI0S33

// GPIO pin L6
#define PD_PL6				(PD_PORT_L | PD_PIN_6 | PD_MUX_0)  //!< PL6
#define PD_PL6_T1CCP0		(PD_PORT_L | PD_PIN_6 | PD_MUX_3)  //!< PL6 as T1CCP0

// GPIO pin L7
#define PD_PL7				(PD_PORT_L | PD_PIN_7 | PD_MUX_0)  //!< PL7
#define PD_PL7_T1CCP1		(PD_PORT_L | PD_PIN_7 | PD_MUX_3)  //!< PL7 as T1CCP1



// GPIO pin M0
#define PD_PM0				(PD_PORT_M | PD_PIN_0 | PD_MUX_0)  //!< PM0
#define PD_PM0_T2CCP0		(PD_PORT_M | PD_PIN_0 | PD_MUX_3)  //!< PM0 as T2CCP0
#define PD_PM0_EPI0S15		(PD_PORT_M | PD_PIN_0 | PD_MUX_15) //!< PM0 as EPI0S15

// GPIO pin M1
#define PD_PM1				(PD_PORT_M | PD_PIN_1 | PD_MUX_0)  //!< PM1
#define PD_PM1_T2CCP1		(PD_PORT_M | PD_PIN_1 | PD_MUX_3)  //!< PM1 as T2CCP1
#define PD_PM1_EPI0S14		(PD_PORT_M | PD_PIN_1 | PD_MUX_15) //!< PM1 as EPI0S14

// GPIO pin M2
#define PD_PM2				(PD_PORT_M | PD_PIN_2 | PD_MUX_0)  //!< PM2
#define PD_PM2_T3CCP0		(PD_PORT_M | PD_PIN_2 | PD_MUX_3)  //!< PM2 as T3CCP0
#define PD_PM2_EPI0S13		(PD_PORT_M | PD_PIN_2 | PD_MUX_15) //!< PM2 as EPI0S13

// GPIO pin M3
#define PD_PM3				(PD_PORT_M | PD_PIN_3 | PD_MUX_0)  //!< PM3
#define PD_PM3_T3CCP1		(PD_PORT_M | PD_PIN_3 | PD_MUX_3)  //!< PM3 as T3CCP1
#define PD_PM3_EPI0S12		(PD_PORT_M | PD_PIN_3 | PD_MUX_15) //!< PM3 as EPI0S12

// GPIO pin M4
#define PD_PM4				(PD_PORT_M | PD_PIN_4 | PD_MUX_0)  //!< PM4
#define PD_PM4_U0CTS		(PD_PORT_M | PD_PIN_4 | PD_MUX_1)  //!< PM4 as U0CTS
#define PD_PM4_T4CCP0		(PD_PORT_M | PD_PIN_4 | PD_MUX_3)  //!< PM4 as T4CCP0

// GPIO pin M5
#define PD_PM5				(PD_PORT_M | PD_PIN_5 | PD_MUX_0)  //!< PM5
#define PD_PM5_U0DCD		(PD_PORT_M | PD_PIN_5 | PD_MUX_1)  //!< PM5 as U0DCD
#define PD_PM5_T4CCP1		(PD_PORT_M | PD_PIN_5 | PD_MUX_3)  //!< PM5 as T4CCP1

// GPIO pin M6
#define PD_PM6				(PD_PORT_M | PD_PIN_6 | PD_MUX_0)  //!< PM6
#define PD_PM6_U0DSR		(PD_PORT_M | PD_PIN_6 | PD_MUX_1)  //!< PM6 as U0DSR
#define PD_PM6_T5CCP0		(PD_PORT_M | PD_PIN_6 | PD_MUX_3)  //!< PM6 as T5CCP0

// GPIO pin M7
#define PD_PM7				(PD_PORT_M | PD_PIN_7 | PD_MUX_0)  //!< PM7
#define PD_PM7_U0RI			(PD_PORT_M | PD_PIN_7 | PD_MUX_1)  //!< PM7 as U0RI
#define PD_PM7_T5CCP1		(PD_PORT_M | PD_PIN_7 | PD_MUX_3)  //!< PM7 as T5CCP1



// GPIO pin N0
#define PD_PN0				(PD_PORT_N | PD_PIN_0 | PD_MUX_0)  //!< PN0
#define PD_PN0_U1RTS		(PD_PORT_N | PD_PIN_0 | PD_MUX_1)  //!< PN0 as U1RTS

// GPIO pin N1
#define PD_PN1				(PD_PORT_N | PD_PIN_1 | PD_MUX_0)  //!< PN1
#define PD_PN1_U1CTS		(PD_PORT_N | PD_PIN_1 | PD_MUX_1)  //!< PN1 as U1CTS

// GPIO pin N2
#define PD_PN2				(PD_PORT_N | PD_PIN_2 | PD_MUX_0)  //!< PN2
#define PD_PN2_U1DCD		(PD_PORT_N | PD_PIN_2 | PD_MUX_1)  //!< PN2 as U1DCD
#define PD_PN2_U2RTS		(PD_PORT_N | PD_PIN_2 | PD_MUX_2)  //!< PN2 as U2RTS
#define PD_PN2_EPI0S29		(PD_PORT_N | PD_PIN_2 | PD_MUX_15) //!< PN2 as EPI0S29

// GPIO pin N3
#define PD_PN3				(PD_PORT_N | PD_PIN_3 | PD_MUX_0)  //!< PN3
#define PD_PN3_U1DSR		(PD_PORT_N | PD_PIN_3 | PD_MUX_1)  //!< PN3 as U1DSR
#define PD_PN3_U2CTS		(PD_PORT_N | PD_PIN_3 | PD_MUX_2)  //!< PN3 as U2CTS
#define PD_PN3_EPI0S30		(PD_PORT_N | PD_PIN_3 | PD_MUX_15) //!< PN3 as EPI0S30

// GPIO pin N4
#define PD_PN4				(PD_PORT_N | PD_PIN_4 | PD_MUX_0)  //!< PN4
#define PD_PN4_U1DTR		(PD_PORT_N | PD_PIN_4 | PD_MUX_1)  //!< PN4 as U1DTR
#define PD_PN4_U3RTS		(PD_PORT_N | PD_PIN_4 | PD_MUX_2)  //!< PN4 as U3RTS
#define PD_PN4_I2C2SDA		(PD_PORT_N | PD_PIN_4 | PD_MUX_3)  //!< PN4 as I2C2SDA
#define PD_PN4_EPI0S34		(PD_PORT_N | PD_PIN_4 | PD_MUX_15) //!< PN4 as EPI0S34

// GPIO pin N5
#define PD_PN5				(PD_PORT_N | PD_PIN_5 | PD_MUX_0)  //!< PN5
#define PD_PN5_U1RI			(PD_PORT_N | PD_PIN_5 | PD_MUX_1)  //!< PN5 as U1RI
#define PD_PN5_U3CTS		(PD_PORT_N | PD_PIN_5 | PD_MUX_2)  //!< PN5 as U3CTS
#define PD_PN5_I2C2SCL		(PD_PORT_N | PD_PIN_5 | PD_MUX_3)  //!< PN5 as I2C2SCL
#define PD_PN5_EPI0S35		(PD_PORT_N | PD_PIN_5 | PD_MUX_15) //!< PN5 as EPI0S35


// GPIO pin P0
#define PD_PP0				(PD_PORT_P | PD_PIN_0 | PD_MUX_0)  //!< PP0
#define PD_PP0_U6Rx			(PD_PORT_P | PD_PIN_0 | PD_MUX_1)  //!< PP0 as U6Rx
#define PD_PP0_SSI3XDAT2	(PD_PORT_P | PD_PIN_0 | PD_MUX_15) //!< PP0 as SSI3XDAT2

// GPIO pin P1
#define PD_PP1				(PD_PORT_P | PD_PIN_1 | PD_MUX_0)  //!< PP1
#define PD_PP1_U6Tx			(PD_PORT_P | PD_PIN_1 | PD_MUX_1)  //!< PP1 as U6Tx
#define PD_PP1_SSI3XDAT3	(PD_PORT_P | PD_PIN_1 | PD_MUX_15) //!< PP1 as SSI3XDAT3

// GPIO pin P2
#define PD_PP2				(PD_PORT_P | PD_PIN_2 | PD_MUX_0)  //!< PP2
#define PD_PP2_U0DTR		(PD_PORT_P | PD_PIN_2 | PD_MUX_1)  //!< PP2 as U0DTR
#define PD_PP2_USB0NXT		(PD_PORT_P | PD_PIN_2 | PD_MUX_14) //!< PP2 as USB0NXT
#define PD_PP2_EPI0S29		(PD_PORT_P | PD_PIN_2 | PD_MUX_15) //!< PP2 as EPI0S29

// GPIO pin P3
#define PD_PP3				(PD_PORT_P | PD_PIN_3 | PD_MUX_0)  //!< PP3
#define PD_PP3_U1CTS		(PD_PORT_P | PD_PIN_3 | PD_MUX_1)  //!< PP3 as U1CTS
#define PD_PP3_U0DCD		(PD_PORT_P | PD_PIN_3 | PD_MUX_2)  //!< PP3 as U0DCD
#define PD_PP3_RTCCLK		(PD_PORT_P | PD_PIN_3 | PD_MUX_7)  //!< PP3 as RTCCLK
#define PD_PP3_USB0DIR		(PD_PORT_P | PD_PIN_3 | PD_MUX_14) //!< PP3 as USB0DIR
#define PD_PP3_EPI0S30		(PD_PORT_P | PD_PIN_3 | PD_MUX_15) //!< PP3 as EPI0S30

// GPIO pin P4
#define PD_PP4				(PD_PORT_P | PD_PIN_4 | PD_MUX_0)  //!< PP4
#define PD_PP4_U3RTS		(PD_PORT_P | PD_PIN_4 | PD_MUX_1)  //!< PP4 as U3RTS
#define PD_PP4_U0DSR		(PD_PORT_P | PD_PIN_4 | PD_MUX_2)  //!< PP4 as U0DSR
#define PD_PP4_USB0D7		(PD_PORT_P | PD_PIN_4 | PD_MUX_14) //!< PP4 as USB0D7

// GPIO pin P5
#define PD_PP5				(PD_PORT_P | PD_PIN_5 | PD_MUX_0)  //!< PP5
#define PD_PP5_U3CTS		(PD_PORT_P | PD_PIN_5 | PD_MUX_1)  //!< PP5 as U3CTS
#define PD_PP5_I2C2SCL		(PD_PORT_P | PD_PIN_5 | PD_MUX_2)  //!< PP5 as I2C2SCL
#define PD_PP5_USB0D6		(PD_PORT_P | PD_PIN_5 | PD_MUX_14) //!< PP5 as USB0D6


// GPIO pin Q0
#define PD_PQ0				(PD_PORT_Q | PD_PIN_0 | PD_MUX_0)  //!< PQ0
#define PD_PQ0_SSI3Clk		(PD_PORT_Q | PD_PIN_0 | PD_MUX_14) //!< PQ0 as SSI3Clk
#define PD_PQ0_EPI0S20		(PD_PORT_Q | PD_PIN_0 | PD_MUX_15) //!< PQ0 as EPI0S20

// GPIO pin Q1
#define PD_PQ1				(PD_PORT_Q | PD_PIN_1 | PD_MUX_0)  //!< PQ1
#define PD_PQ1_SSI3Fss		(PD_PORT_Q | PD_PIN_1 | PD_MUX_14) //!< PQ1 as SSI3Fss
#define PD_PQ1_EPI0S21		(PD_PORT_Q | PD_PIN_1 | PD_MUX_15) //!< PQ1 as EPI0S21

// GPIO pin Q2
#define PD_PQ2				(PD_PORT_Q | PD_PIN_2 | PD_MUX_0)  //!< PQ2
#define PD_PQ2_SSI3XDAT0	(PD_PORT_Q | PD_PIN_2 | PD_MUX_14) //!< PQ2 as SSI3XDAT0
#define PD_PQ2_EPI0S22		(PD_PORT_Q | PD_PIN_2 | PD_MUX_15) //!< PQ2 as EPI0S22

// GPIO pin Q3
#define PD_PQ3				(PD_PORT_Q | PD_PIN_3 | PD_MUX_0)  //!< PQ3
#define PD_PQ3_SSI3XDAT1	(PD_PORT_Q | PD_PIN_3 | PD_MUX_14) //!< PQ3 as SSI3XDAT1
#define PD_PQ3_EPI0S23		(PD_PORT_Q | PD_PIN_3 | PD_MUX_15) //!< PQ3 as EPI0S23

// GPIO pin Q4
#define PD_PQ4				(PD_PORT_Q | PD_PIN_4 | PD_MUX_0)  //!< PQ4
#define PD_PQ4_U1Rx			(PD_PORT_Q | PD_PIN_4 | PD_MUX_1)  //!< PQ4 as U1Rx
#define PD_PQ4_DIVSCLK		(PD_PORT_Q | PD_PIN_4 | PD_MUX_7)  //!< PQ4 as DIVSCLK



// Combined flags for pin definitions
#define PD_STRENGTH_8MA_SC  (PD_STRENGTH_8MA | PD_SLEW_RATE)  //!< 8mA drive with slew rate

#define PD_IN				(PD_INPUT | PD_STRENGTH_2MA | PD_PUSH_PULL) //!< input pin
#define PD_OUT				(PD_OUTPUT| PD_STRENGTH_2MA | PD_PUSH_PULL) //!< output pin
#define PD_OUT_8MA			(PD_OUTPUT| PD_STRENGTH_8MA | PD_PUSH_PULL) //!< output pin 8mA drive
#define PD_OD				(PD_OUTPUT| PD_STRENGTH_2MA | PD_MODE_OD)	//!< open drain pin
#define PD_OD_8MA			(PD_OUTPUT| PD_STRENGTH_8MA | PD_MODE_OD)	//!< open drain 8mA drive

#define PD_PERIPHERAL		(PD_AFSEL | PD_STRENGTH_2MA | PD_PUSH_PULL)	//!< peripheral pin
#define PD_PERIPHERAL_8MA 	(PD_AFSEL | PD_STRENGTH_8MA | PD_PUSH_PULL)	//!< peripheral pin 8mA
#define PD_PERIPHERAL_OD 	(PD_AFSEL | PD_STRENGTH_2MA | PD_MODE_OD)	//!< peripheral open drain
#define PD_PERIPHERAL_8MA_SC (PD_AFSEL | PD_STRENGTH_8MA_SC | PD_PUSH_PULL) //!< peripheral pin 8mA slew rate

#define PD_PIN_COMP			(PD_INPUT | PD_STRENGTH_2MA | PD_MODE_ANALOG) //!< compartor pin
#define PD_USB_ANALOG		(PD_INPUT | PD_STRENGTH_2MA | PD_MODE_ANALOG) //!< usb analog pin

#define PD_ADC				(PD_AFSEL | PD_STRENGTH_2MA | PD_MODE_ANALOG) //!< adc pin
#define PD_UART				(PD_PERIPHERAL)					  //!< uart pin
#define PD_I2C				(PD_PERIPHERAL_OD | PD_PULL_UP)   //!< I2C pin
#define PD_PWM				(PD_PERIPHERAL)					  //!< PWM pin
#define PD_SSI				(PD_PERIPHERAL)					  //!< SSI pin
#define PD_CAN				(PD_PERIPHERAL_8MA)				  //!< CAN pin

/// Calculate port address for a given pin definition
#define PORT_ADDRESS(num) ((GPIO_TypeDef*)(BASE_AHB_GPIOA + ((num)<<12)))


/** @} */ // @STD_GPIO



void PIO_Cfg(PIN_DESC cfg);
void PIO_Cfg_List(const PIN_DESC* list );
void PIO_CfgInput_List(PIN_DESC * list );
void PIO_Free(PIN_DESC cfg);
void PIO_Free_List(PIN_DESC* list);
void PIO_Write(PIN_DESC pins, unsigned int val);
pio_set PIO_Read(PIN_DESC pins);
void PIO_SetOutput(PIN_DESC pins);
void PIO_ClrOutput(PIN_DESC pins);
void PIO_CfgOutput1(PIN_DESC pins);
void PIO_CfgOutput0(PIN_DESC pins);
void PIO_Assert(PIN_DESC pins);
void PIO_Deassert(PIN_DESC pins);
void PIO_CfgInput(PIN_DESC pins);

#endif /* GPIO_TM4C_H_ */
