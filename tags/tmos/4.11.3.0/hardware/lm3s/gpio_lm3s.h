/**************************************************************************//**
 * @ingroup	 hardware_lm3s
 * @defgroup lm3s_gpio GPIO
 * @{
 * @file     lm3s/gpio_lm3s.h
 * @brief    GPIO class header
 *
 ******************************************************************************/
#ifndef GPIO_LM3S_H_
#define GPIO_LM3S_H_


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef USE_APB_PERIPHERAL
#define USE_AHB_PERIPHERAL			//!< USE AHB addresses
#endif

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

/// Pin definition bits 23:20 = port
#define PD_PORT_Pos			20						//!< port bits position
#define PD_PORT_Msk			(0xF << PD_PORT_Pos)	//!< port bits mask
#define PD_PORT_Get(x)		(0xF & ((x) >> PD_PORT_Pos)) //!< port value
#define	PD_PORT_A			((unsigned int)0 << PD_PORT_Pos)		//!< port A
#define	PD_PORT_B			((unsigned int)1 << PD_PORT_Pos)		//!< port B
#define	PD_PORT_C			((unsigned int)2 << PD_PORT_Pos)		//!< port C
#define	PD_PORT_D			((unsigned int)3 << PD_PORT_Pos)		//!< port D
#define	PD_PORT_E			((unsigned int)4 << PD_PORT_Pos)		//!< port E
#define	PD_PORT_F			((unsigned int)5 << PD_PORT_Pos)		//!< port F
#define	PD_PORT_G			((unsigned int)6 << PD_PORT_Pos)		//!< port G
#define	PD_PORT_H			((unsigned int)7 << PD_PORT_Pos)		//!< port H
#define	PD_PORT_J			((unsigned int)8 << PD_PORT_Pos)		//!< port J
#define PD_PORT_V           ((unsigned int)0xF << PD_PORT_Pos)	//!< virtual port V
#define PD_LAST_PORT		8						//!< number of physical ports

/// Pin definition bits 19:16 = pin mux
#define PD_MUX_Pos			16						//!< mux bits position
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

/// Pin definition bits 15:12 = pin mode
#define PD_MODE_Pos 		12						//!< mode bits position
#define PD_MODE_Msk			(0xF << PD_MODE_Pos)	//!< mode bits mask
#define PD_MODE_Get(x)		(0xF & ((x) >> PD_MODE_Pos)) //!< mode value
#define PD_MODE_ANALOG		(0 << PD_MODE_Pos)		//!< enable analog mode
#define PD_PUSH_PULL  		(1 << PD_MODE_Pos)		//!< standard push pull mode
#define PD_MODE_OD        	(2 << PD_MODE_Pos)		//!< open drain mode
#define PD_PULL_UP   		(4 << PD_MODE_Pos)		//!< weak pull up
#define PD_PULL_DOWN   		(8 << PD_MODE_Pos)		//!< weak pull down

/// Pin definition bits 11:10 = pin strength
#define PD_STRENGTH_Pos 	10						//!< strength bits position
#define PD_STRENGTH_Msk		(0x3 << PD_STRENGTH_Pos)//!< strength bits mask
#define PD_STRENGTH_Get(x)	(0x3 & ((x) >> PD_STRENGTH_Pos)) //!< strength mask
#define PD_STRENGTH_2MA     (0 << PD_STRENGTH_Pos)	//!< 2mA drive strength
#define PD_STRENGTH_4MA     (1 << PD_STRENGTH_Pos)	//!< 4mA drive strength
#define PD_STRENGTH_8MA     (2 << PD_STRENGTH_Pos)  //!< 8mA drive strength

/// Pin definition bit 9 = slew rate
#define PD_SLEW_RATE		(1 << 9)

/// Pin definition bit 8 = direction
#define PD_INPUT        	(0 << 8)				//!< input pin
#define PD_OUTPUT       	(1 << 8)				//!< output pin

/// Pin definition bit 7 = peripheral
#define PD_AFSEL 		    (1 << 7)

/// Pin definition bits 6:4 = interrupts
#define PD_INT_Pos 			4						//!< interrupt bits position
#define PD_INT_ENABLE		(1 << PD_INT_Pos)		//!< interrupt enabled
#define PD_INT_SENSE		(2 << PD_INT_Pos)		//!< sense - level or edge
#define PD_INT_BE			(4 << PD_INT_Pos)		//!< both edges
#define PD_INT_EVENT		(8 << PD_INT_Pos)		//!< high/low level or rising/falling

/// Pin definition bit 3 = Lock
#define PD_LOCK     	  	(1 << 3)

#define PD_FALLING_EDGE     (PD_INT_ENABLE)			//!< interrupt on falling edge
#define PD_RISING_EDGE      (PD_INT_ENABLE | PD_INT_EVENT)	//!< interrupt on rising edge
#define PD_BOTH_EDGES       (PD_INT_ENABLE | PD_INT_BE)	//!< interrupt on any edge
#define PD_LOW_LEVEL        (PD_INT_ENABLE | PD_INT_SENSE) //!< interrupt on low level
#define PD_HIGH_LEVEL       (PD_INT_ENABLE | PD_INT_SENSE | PD_INT_EVENT) //!< interrupt on high level

/// Pin definition bit 2 = Active level
#define PD_ACTIVE_HIGH 	  	(1 << 2)
#define PD_ACTIVE_LOW 	  	(0 << 2)

//*****************************************************************************
//
// Values that can be passed to Pin_Configure, Pin_Write, Pin_Read as the PIN_DESC parameter.
//
//*****************************************************************************

//
// GPIO pin A0
//
#define PD_PA0				(PD_PORT_A | PD_PIN_0 | PD_MUX_0) //!< PA0
#define PD_PA0_U0RX         (PD_PORT_A | PD_PIN_0 | PD_MUX_1) //!< PA0 as U0RX
#define PD_PA0_I2C1SCL      (PD_PORT_A | PD_PIN_0 | PD_MUX_8) //!< PA0_as I2C1SCL
#define PD_PA0_U1RX         (PD_PORT_A | PD_PIN_0 | PD_MUX_9) //!< PA0 as U1RX
//
// GPIO pin A1
//
#define PD_PA1				(PD_PORT_A | PD_PIN_1 | PD_MUX_0) //!< PA1
#define PD_PA1_U0TX         (PD_PORT_A | PD_PIN_1 | PD_MUX_1) //!< PA1_U0TX
#define PD_PA1_I2C1SDA      (PD_PORT_A | PD_PIN_1 | PD_MUX_8) //!< PA1 as I2C1SDA
#define PD_PA1_U1TX         (PD_PORT_A | PD_PIN_1 | PD_MUX_9) //!< PA1 as U1TX
//
// GPIO pin A2
//
#define PD_PA2				(PD_PORT_A | PD_PIN_2 | PD_MUX_0) //!< PA2
#define PD_PA2_SSI0CLK      (PD_PORT_A | PD_PIN_2 | PD_MUX_1) //!< PA2_SSI0CLK
#define PD_PA2_PWM4         (PD_PORT_A | PD_PIN_2 | PD_MUX_4) //!< PA2_PWM4
#define PD_PA2_I2S0RXSD     (PD_PORT_A | PD_PIN_2 | PD_MUX_9) //!< PA2_I2S0RXSD
//
// GPIO pin A3
//
#define PD_PA3				(PD_PORT_A | PD_PIN_3 | PD_MUX_0) //!< PA3
#define PD_PA3_SSI0FSS      (PD_PORT_A | PD_PIN_3 | PD_MUX_1) //!< PA3_SSI0FSS
#define PD_PA3_PWM5         (PD_PORT_A | PD_PIN_3 | PD_MUX_4) //!< PA3_PWM5
#define PD_PA3_I2S0RXMCLK   (PD_PORT_A | PD_PIN_3 | PD_MUX_9) //!< PA3_I2S0RXMCLK
//
// GPIO pin A4
//
#define PD_PA4				(PD_PORT_A | PD_PIN_4 | PD_MUX_0) //!< PA4
#define PD_PA4_SSI0RX       (PD_PORT_A | PD_PIN_4 | PD_MUX_1) //!< PA4_SSI0RX
#define PD_PA4_PWM6         (PD_PORT_A | PD_PIN_4 | PD_MUX_4) //!< PA4_PWM6
#define PD_PA4_CAN0RX       (PD_PORT_A | PD_PIN_4 | PD_MUX_5) //!< PA4_CAN0RX
#define PD_PA4_I2S0TXSCK    (PD_PORT_A | PD_PIN_4 | PD_MUX_9) //!< PA4_I2S0TXSCK
//
// GPIO pin A5
//
#define PD_PA5				(PD_PORT_A | PD_PIN_5 | PD_MUX_0) //!< PA5
#define PD_PA5_SSI0TX       (PD_PORT_A | PD_PIN_5 | PD_MUX_1) //!< PA5_SSI0TX
#define PD_PA5_PWM7         (PD_PORT_A | PD_PIN_5 | PD_MUX_4) //!< PA5_PWM7
#define PD_PA5_CAN0TX       (PD_PORT_A | PD_PIN_5 | PD_MUX_5) //!< PA5_CAN0TX
#define PD_PA5_I2S0TXWS     (PD_PORT_A | PD_PIN_5 | PD_MUX_9) //!< PA5_I2S0TXWS
//
// GPIO pin A6
//
#define PD_PA6				(PD_PORT_A | PD_PIN_6 | PD_MUX_0) //!< PA6
#define PD_PA6_I2C1SCL      (PD_PORT_A | PD_PIN_6 | PD_MUX_1) //!< PA6_I2C1SCL
#define PD_PA6_CCP1         (PD_PORT_A | PD_PIN_6 | PD_MUX_2) //!< PA6_CCP1
#define PD_PA6_PWM0         (PD_PORT_A | PD_PIN_6 | PD_MUX_4) //!< PA6_PWM0
#define PD_PA6_PWM4         (PD_PORT_A | PD_PIN_6 | PD_MUX_5) //!< PA6_PWM4
#define PD_PA6_CAN0RX       (PD_PORT_A | PD_PIN_6 | PD_MUX_6) //!< PA6_CAN0RX
#define PD_PA6_USB0EPEN     (PD_PORT_A | PD_PIN_6 | PD_MUX_8) //!< PA6_USB0EPEN
#define PD_PA6_U1CTS        (PD_PORT_A | PD_PIN_6 | PD_MUX_9) //!< PA6_U1CTS
//
// GPIO pin A7
//
#define PD_PA7				(PD_PORT_A | PD_PIN_7 | PD_MUX_0) //!< PA7
#define PD_PA7_I2C1SDA      (PD_PORT_A | PD_PIN_7 | PD_MUX_1) //!< PA7_I2C1SDA
#define PD_PA7_CCP4         (PD_PORT_A | PD_PIN_7 | PD_MUX_2) //!< PA7_CCP4
#define PD_PA7_PWM1         (PD_PORT_A | PD_PIN_7 | PD_MUX_4) //!< PA7_PWM1
#define PD_PA7_PWM5         (PD_PORT_A | PD_PIN_7 | PD_MUX_5) //!< PA7_PWM5
#define PD_PA7_CAN0TX       (PD_PORT_A | PD_PIN_7 | PD_MUX_6) //!< PA7_CAN0TX
#define PD_PA7_CCP3         (PD_PORT_A | PD_PIN_7 | PD_MUX_7) //!< PA7_CCP3
#define PD_PA7_USB0PFLT     (PD_PORT_A | PD_PIN_7 | PD_MUX_8) //!< PA7_USB0PFLT
#define PD_PA7_U1DCD        (PD_PORT_A | PD_PIN_7 | PD_MUX_9) //!< PA7_U1DCD
//
// GPIO pin B0
//
#define PD_PB0				(PD_PORT_B | PD_PIN_0 | PD_MUX_0) //!< PB0
#define PD_PB0_CCP0         (PD_PORT_B | PD_PIN_0 | PD_MUX_1) //!< PB0_CCP0
#define PD_PB0_PWM2         (PD_PORT_B | PD_PIN_0 | PD_MUX_2) //!< PB0_PWM2
#define PD_PB0_U1RX         (PD_PORT_B | PD_PIN_0 | PD_MUX_5) //!< PB0_U1RX
//
// GPIO pin B1
//
#define PD_PB1				(PD_PORT_B | PD_PIN_1 | PD_MUX_0) //!< PB1
#define PD_PB1_CCP2         (PD_PORT_B | PD_PIN_1 | PD_MUX_1) //!< PB1_CCP2
#define PD_PB1_PWM3         (PD_PORT_B | PD_PIN_1 | PD_MUX_2) //!< PB1_PWM3
#define PD_PB1_CCP1         (PD_PORT_B | PD_PIN_1 | PD_MUX_4) //!< PB1_CCP1
#define PD_PB1_U1TX         (PD_PORT_B | PD_PIN_1 | PD_MUX_5) //!< PB1_U1TX
//
// GPIO pin B2
//
#define PD_PB2				(PD_PORT_B | PD_PIN_2 | PD_MUX_0) //!< PB2
#define PD_PB2_I2C0SCL      (PD_PORT_B | PD_PIN_2 | PD_MUX_1) //!< PB2_I2C0SCL
#define PD_PB2_IDX0         (PD_PORT_B | PD_PIN_2 | PD_MUX_2) //!< PB2_IDX0
#define PD_PB2_CCP3         (PD_PORT_B | PD_PIN_2 | PD_MUX_4) //!< PB2_CCP3
#define PD_PB2_CCP0         (PD_PORT_B | PD_PIN_2 | PD_MUX_5) //!< PB2_CCP0
#define PD_PB2_USB0EPEN     (PD_PORT_B | PD_PIN_2 | PD_MUX_8) //!< PB2_USB0EPEN
//
// GPIO pin B3
//
#define PD_PB3				(PD_PORT_B | PD_PIN_3 | PD_MUX_0) //!< PB3
#define PD_PB3_I2C0SDA      (PD_PORT_B | PD_PIN_3 | PD_MUX_1) //!< PB3_I2C0SDA
#define PD_PB3_FAULT0       (PD_PORT_B | PD_PIN_3 | PD_MUX_2) //!< PB3_FAULT0
#define PD_PB3_FAULT3       (PD_PORT_B | PD_PIN_3 | PD_MUX_4) //!< PB3_FAULT3
#define PD_PB3_USB0PFLT     (PD_PORT_B | PD_PIN_3 | PD_MUX_8) //!< PB3_USB0PFLT
//
// GPIO pin B4
//
#define PD_PB4				(PD_PORT_B | PD_PIN_4 | PD_MUX_0) //!< PB4
#define PD_PB4_U2RX         (PD_PORT_B | PD_PIN_4 | PD_MUX_4) //!< PB4_U2RX
#define PD_PB4_CAN0RX       (PD_PORT_B | PD_PIN_4 | PD_MUX_5) //!< PB4_CAN0RX
#define PD_PB4_IDX0         (PD_PORT_B | PD_PIN_4 | PD_MUX_6) //!< PB4_IDX0
#define PD_PB4_U1RX         (PD_PORT_B | PD_PIN_4 | PD_MUX_7) //!< PB4_U1RX
#define PD_PB4_EPI0S23      (PD_PORT_B | PD_PIN_4 | PD_MUX_8) //!< PB4_EPI0S23
//
// GPIO pin B5
//
#define PD_PB5				(PD_PORT_B | PD_PIN_5 | PD_MUX_0) //!< PB5
#define PD_PB5_C0O          (PD_PORT_B | PD_PIN_5 | PD_MUX_1) //!< PB5_C0O
#define PD_PB5_CCP5         (PD_PORT_B | PD_PIN_5 | PD_MUX_2) //!< PB5_CCP5
#define PD_PB5_CCP6         (PD_PORT_B | PD_PIN_5 | PD_MUX_3) //!< PB5_CCP6
#define PD_PB5_CCP0         (PD_PORT_B | PD_PIN_5 | PD_MUX_4) //!< PB5_CCP0
#define PD_PB5_CAN0TX       (PD_PORT_B | PD_PIN_5 | PD_MUX_5) //!< PB5_CAN0TX
#define PD_PB5_CCP2         (PD_PORT_B | PD_PIN_5 | PD_MUX_6) //!< PB5_CCP2
#define PD_PB5_U1TX         (PD_PORT_B | PD_PIN_5 | PD_MUX_7) //!< PB5_U1TX
#define PD_PB5_EPI0S22      (PD_PORT_B | PD_PIN_5 | PD_MUX_8) //!< PB5_EPI0S22
//
// GPIO pin B6
//
#define PD_PB6				(PD_PORT_B | PD_PIN_6 | PD_MUX_0) //!< PB6
#define PD_PB6_CCP1         (PD_PORT_B | PD_PIN_6 | PD_MUX_1) //!< PB6_CCP1
#define PD_PB6_CCP7         (PD_PORT_B | PD_PIN_6 | PD_MUX_2) //!< PB6_CCP7
#define PD_PB6_C0O          (PD_PORT_B | PD_PIN_6 | PD_MUX_3) //!< PB6_C0O
#define PD_PB6_FAULT1       (PD_PORT_B | PD_PIN_6 | PD_MUX_4) //!< PB6_FAULT1
#define PD_PB6_IDX0         (PD_PORT_B | PD_PIN_6 | PD_MUX_5) //!< PB6_IDX0
#define PD_PB6_CCP5         (PD_PORT_B | PD_PIN_6 | PD_MUX_6) //!< PB6_CCP5
#define PD_PB6_I2S0TXSCK    (PD_PORT_B | PD_PIN_6 | PD_MUX_9) //!< PB6_I2S0TXSCK
//
// GPIO pin B7
//
#define PD_PB7				(PD_PORT_B | PD_PIN_7 | PD_MUX_0) //!< PB7
#define PD_PB7_NMI			(PD_PORT_B | PD_PIN_7 | PD_MUX_4) //!< PB7_NMI
//
// GPIO pin C0
//
#define PD_PC0				(PD_PORT_C | PD_PIN_0 | PD_MUX_0) //!< PC0
#define PD_PC0_TCK          (PD_PORT_C | PD_PIN_0 | PD_MUX_3) //!< PC0_TCK
//
// GPIO pin C1
//
#define PD_PC1				(PD_PORT_C | PD_PIN_1 | PD_MUX_0) //!< PC1
#define PD_PC1_TMS			(PD_PORT_C | PD_PIN_1 | PD_MUX_3) //!< PC1_TMS
//
// GPIO pin C2
//
#define PD_PC2				(PD_PORT_C | PD_PIN_2 | PD_MUX_0) //!< PC2
#define PD_PC2_TDI			(PD_PORT_C | PD_PIN_2 | PD_MUX_3) //!< PC2_TDI
//
// GPIO pin C3
//
#define PD_PC3				(PD_PORT_C | PD_PIN_3 | PD_MUX_0) //!< PC3
#define PD_PC3_TDO			(PD_PORT_C | PD_PIN_3 | PD_MUX_3) //!< PC3_TDO
//
// GPIO pin C4
//
#define PD_PC4				(PD_PORT_C | PD_PIN_4 | PD_MUX_0) //!< PC4
#define PD_PC4_CCP5         (PD_PORT_C | PD_PIN_4 | PD_MUX_1) //!< PC4_CCP5
#define PD_PC4_PHA0         (PD_PORT_C | PD_PIN_4 | PD_MUX_2) //!< PC4_PHA0
#define PD_PC4_PWM6         (PD_PORT_C | PD_PIN_4 | PD_MUX_4) //!< PC4_PWM6
#define PD_PC4_CCP2         (PD_PORT_C | PD_PIN_4 | PD_MUX_5) //!< PC4_CCP2
#define PD_PC4_CCP4         (PD_PORT_C | PD_PIN_4 | PD_MUX_6) //!< PC4_CCP4
#define PD_PC4_EPI0S2       (PD_PORT_C | PD_PIN_4 | PD_MUX_8) //!< PC4_EPI0S2
#define PD_PC4_CCP1         (PD_PORT_C | PD_PIN_4 | PD_MUX_9) //!< PC4_CCP1
//
// GPIO pin C5
//
#define PD_PC5				(PD_PORT_C | PD_PIN_5 | PD_MUX_0) //!< PC5
#define PD_PC5_CCP1         (PD_PORT_C | PD_PIN_5 | PD_MUX_1) //!< PC5_CCP1
#define PD_PC5_C1O          (PD_PORT_C | PD_PIN_5 | PD_MUX_2) //!< PC5_C1O
#define PD_PC5_C0O          (PD_PORT_C | PD_PIN_5 | PD_MUX_3) //!< PC5_C0O
#define PD_PC5_FAULT2       (PD_PORT_C | PD_PIN_5 | PD_MUX_4) //!< PC5_FAULT2
#define PD_PC5_CCP3         (PD_PORT_C | PD_PIN_5 | PD_MUX_5) //!< PC5_CCP3
#define PD_PC5_USB0EPEN     (PD_PORT_C | PD_PIN_5 | PD_MUX_6) //!< PC5_USB0EPEN
#define PD_PC5_EPI0S3       (PD_PORT_C | PD_PIN_5 | PD_MUX_8) //!< PC5_EPI0S3
//
// GPIO pin C6
//
#define PD_PC6				(PD_PORT_C | PD_PIN_6 | PD_MUX_0) //!< PC6
#define PD_PC6_CCP3         (PD_PORT_C | PD_PIN_6 | PD_MUX_1) //!< PC6_CCP3
#define PD_PC6_PHB0         (PD_PORT_C | PD_PIN_6 | PD_MUX_2) //!< PC6_PHB0
#define PD_PC6_C2O          (PD_PORT_C | PD_PIN_6 | PD_MUX_3) //!< PC6_C2O
#define PD_PC6_PWM7         (PD_PORT_C | PD_PIN_6 | PD_MUX_4) //!< PC6_PWM7
#define PD_PC6_U1RX         (PD_PORT_C | PD_PIN_6 | PD_MUX_5) //!< PC6_U1RX
#define PD_PC6_CCP0         (PD_PORT_C | PD_PIN_6 | PD_MUX_6) //!< PC6_CCP0
#define PD_PC6_USB0PFLT     (PD_PORT_C | PD_PIN_6 | PD_MUX_7) //!< PC6_USB0PFLT
#define PD_PC6_EPI0S4       (PD_PORT_C | PD_PIN_6 | PD_MUX_8) //!< PC6_EPI0S4
//
// GPIO pin C7
//
#define PD_PC7				(PD_PORT_C | PD_PIN_7 | PD_MUX_0) //!< PC7
#define PD_PC7_CCP4         (PD_PORT_C | PD_PIN_7 | PD_MUX_1) //!< PC7_CCP4
#define PD_PC7_PHB0         (PD_PORT_C | PD_PIN_7 | PD_MUX_2) //!< PC7_PHB0
#define PD_PC7_CCP0         (PD_PORT_C | PD_PIN_7 | PD_MUX_4) //!< PC7_CCP0
#define PD_PC7_U1TX         (PD_PORT_C | PD_PIN_7 | PD_MUX_5) //!< PC7_U1TX
#define PD_PC7_USB0PFLT     (PD_PORT_C | PD_PIN_7 | PD_MUX_6) //!< PC7_USB0PFLT
#define PD_PC7_C1O          (PD_PORT_C | PD_PIN_7 | PD_MUX_7) //!< PC7_C1O
#define PD_PC7_EPI0S5       (PD_PORT_C | PD_PIN_7 | PD_MUX_8) //!< PC7_EPI0S5
//
// GPIO pin D0
//
#define PD_PD0				(PD_PORT_D | PD_PIN_0 | PD_MUX_0) //!< PD0
#define PD_PD0_PWM0         (PD_PORT_D | PD_PIN_0 | PD_MUX_1) //!< PD0_PWM0
#define PD_PD0_CAN0RX       (PD_PORT_D | PD_PIN_0 | PD_MUX_2) //!< PD0_CAN0RX
#define PD_PD0_IDX0         (PD_PORT_D | PD_PIN_0 | PD_MUX_3) //!< PD0_IDX0
#define PD_PD0_U2RX         (PD_PORT_D | PD_PIN_0 | PD_MUX_4) //!< PD0_U2RX
#define PD_PD0_U1RX         (PD_PORT_D | PD_PIN_0 | PD_MUX_5) //!< PD0_U1RX
#define PD_PD0_CCP6         (PD_PORT_D | PD_PIN_0 | PD_MUX_6) //!< PD0_CCP6
#define PD_PD0_I2S0RXSCK    (PD_PORT_D | PD_PIN_0 | PD_MUX_8) //!< PD0_I2S0RXSCK
#define PD_PD0_U1CTS        (PD_PORT_D | PD_PIN_0 | PD_MUX_9) //!< PD0_U1CTS
//
// GPIO pin D1
//
#define PD_PD1				(PD_PORT_D | PD_PIN_1 | PD_MUX_0) //!< PD1
#define PD_PD1_PWM1         (PD_PORT_D | PD_PIN_1 | PD_MUX_1) //!< PD1_PWM1
#define PD_PD1_CAN0TX       (PD_PORT_D | PD_PIN_1 | PD_MUX_2) //!< PD1_CAN0TX
#define PD_PD1_PHA0         (PD_PORT_D | PD_PIN_1 | PD_MUX_3) //!< PD1_PHA0
#define PD_PD1_U2TX         (PD_PORT_D | PD_PIN_1 | PD_MUX_4) //!< PD1_U2TX
#define PD_PD1_U1TX         (PD_PORT_D | PD_PIN_1 | PD_MUX_5) //!< PD1_U1TX
#define PD_PD1_CCP7         (PD_PORT_D | PD_PIN_1 | PD_MUX_6) //!< PD1_CCP7
#define PD_PD1_I2S0RXWS     (PD_PORT_D | PD_PIN_1 | PD_MUX_8) //!< PD1_I2S0RXWS
#define PD_PD1_U1DCD        (PD_PORT_D | PD_PIN_1 | PD_MUX_9) //!< PD1_U1DCD
#define PD_PD1_CCP2         (PD_PORT_D | PD_PIN_1 | PD_MUX_10) //!< PD1_CCP2
#define PD_PD1_PHB1         (PD_PORT_D | PD_PIN_1 | PD_MUX_11) //!< PD1_PHB1
//
// GPIO pin D2
//
#define PD_PD2				(PD_PORT_D | PD_PIN_2 | PD_MUX_0) //!< PD2
#define PD_PD2_U1RX         (PD_PORT_D | PD_PIN_2 | PD_MUX_1) //!< PD2_U1RX
#define PD_PD2_CCP6         (PD_PORT_D | PD_PIN_2 | PD_MUX_2) //!< PD2_CCP6
#define PD_PD2_PWM2         (PD_PORT_D | PD_PIN_2 | PD_MUX_3) //!< PD2_PWM2
#define PD_PD2_CCP5         (PD_PORT_D | PD_PIN_2 | PD_MUX_4) //!< PD2_CCP5
#define PD_PD2_EPI0S20      (PD_PORT_D | PD_PIN_2 | PD_MUX_8) //!< PD2_EPI0S20
//
// GPIO pin D3
//
#define PD_PD3				(PD_PORT_D | PD_PIN_3 | PD_MUX_0) //!< PD3
#define PD_PD3_U1TX         (PD_PORT_D | PD_PIN_3 | PD_MUX_1) //!< PD3_U1TX
#define PD_PD3_CCP7         (PD_PORT_D | PD_PIN_3 | PD_MUX_2) //!< PD3_CCP7
#define PD_PD3_PWM3         (PD_PORT_D | PD_PIN_3 | PD_MUX_3) //!< PD3_PWM3
#define PD_PD3_CCP0         (PD_PORT_D | PD_PIN_3 | PD_MUX_4) //!< PD3_CCP0
#define PD_PD3_EPI0S21      (PD_PORT_D | PD_PIN_3 | PD_MUX_8) //!< PD3_EPI0S21
//
// GPIO pin D4
//
#define PD_PD4				(PD_PORT_D | PD_PIN_4 | PD_MUX_0) //!< PD4
#define PD_PD4_CCP0         (PD_PORT_D | PD_PIN_4 | PD_MUX_1) //!< PD4_CCP0
#define PD_PD4_CCP3         (PD_PORT_D | PD_PIN_4 | PD_MUX_2) //!< PD4_CCP3
#define PD_PD4_I2S0RXSD     (PD_PORT_D | PD_PIN_4 | PD_MUX_8) //!< PD4_I2S0RXSD
#define PD_PD4_U1RI         (PD_PORT_D | PD_PIN_4 | PD_MUX_9) //!< PD4_U1RI
#define PD_PD4_EPI0S19      (PD_PORT_D | PD_PIN_4 | PD_MUX_10) //!<PD4_EPI0S19
//
// GPIO pin D5
//
#define PD_PD5				(PD_PORT_D | PD_PIN_5 | PD_MUX_0) //!< PD5
#define PD_PD5_CCP2         (PD_PORT_D | PD_PIN_5 | PD_MUX_1) //!< PD5_CCP2
#define PD_PD5_CCP4         (PD_PORT_D | PD_PIN_5 | PD_MUX_2) //!< PD5_CCP4
#define PD_PD5_I2S0RXMCLK   (PD_PORT_D | PD_PIN_5 | PD_MUX_8) //!< PD5_I2S0RXMCLK
#define PD_PD5_U2RX         (PD_PORT_D | PD_PIN_5 | PD_MUX_9) //!< PD5_U2RX
#define PD_PD5_EPI0S28      (PD_PORT_D | PD_PIN_5 | PD_MUX_10) //!<PD5_EPI0S28
//
// GPIO pin D6
//
#define PD_PD6				(PD_PORT_D | PD_PIN_6 | PD_MUX_0) //!< PD6
#define PD_PD6_FAULT0       (PD_PORT_D | PD_PIN_6 | PD_MUX_1) //!< PD6_FAULT0
#define PD_PD6_I2S0TXSCK    (PD_PORT_D | PD_PIN_6 | PD_MUX_8) //!< PD6_I2S0TXSCK
#define PD_PD6_U2TX         (PD_PORT_D | PD_PIN_6 | PD_MUX_9) //!< PD6_U2TX
#define PD_PD6_EPI0S29      (PD_PORT_D | PD_PIN_6 | PD_MUX_10) //!<PD6_EPI0S29
//
// GPIO pin D7
//
#define PD_PD7				(PD_PORT_D | PD_PIN_7 | PD_MUX_0) //!< PD7
#define PD_PD7_IDX0         (PD_PORT_D | PD_PIN_7 | PD_MUX_1) //!< PD7_IDX0
#define PD_PD7_C0O          (PD_PORT_D | PD_PIN_7 | PD_MUX_2) //!< PD7_C0O
#define PD_PD7_CCP1         (PD_PORT_D | PD_PIN_7 | PD_MUX_3) //!< PD7_CCP1
#define PD_PD7_I2S0TXWS     (PD_PORT_D | PD_PIN_7 | PD_MUX_8) //!< PD7_I2S0TXWS
#define PD_PD7_U1DTR        (PD_PORT_D | PD_PIN_7 | PD_MUX_9) //!< PD7_U1DTR
#define PD_PD7_EPI0S30      (PD_PORT_D | PD_PIN_7 | PD_MUX_10) //!<PD7_EPI0S30
//
// GPIO pin E0
//
#define PD_PE0				(PD_PORT_E | PD_PIN_0 | PD_MUX_0) //!< PE0
#define PD_PE0_PWM4         (PD_PORT_E | PD_PIN_0 | PD_MUX_1) //!< PE0_PWM4
#define PD_PE0_SSI1CLK      (PD_PORT_E | PD_PIN_0 | PD_MUX_2) //!< PE0_SSI1CLK
#define PD_PE0_CCP3         (PD_PORT_E | PD_PIN_0 | PD_MUX_3) //!< PE0_CCP3
#define PD_PE0_EPI0S8       (PD_PORT_E | PD_PIN_0 | PD_MUX_8) //!< PE0_EPI0S8
#define PD_PE0_USB0PFLT     (PD_PORT_E | PD_PIN_0 | PD_MUX_9) //!< PE0_USB0PFLT
//
// GPIO pin E1
//
#define PD_PE1				(PD_PORT_E | PD_PIN_1 | PD_MUX_0) //!< PE1
#define PD_PE1_PWM5         (PD_PORT_E | PD_PIN_1 | PD_MUX_1) //!< PE1_PWM5
#define PD_PE1_SSI1FSS      (PD_PORT_E | PD_PIN_1 | PD_MUX_2) //!< PE1_SSI1FSS
#define PD_PE1_FAULT0       (PD_PORT_E | PD_PIN_1 | PD_MUX_3) //!< PE1_FAULT0
#define PD_PE1_CCP2         (PD_PORT_E | PD_PIN_1 | PD_MUX_4) //!< PE1_CCP2
#define PD_PE1_CCP6         (PD_PORT_E | PD_PIN_1 | PD_MUX_5) //!< PE1_CCP6
#define PD_PE1_EPI0S9       (PD_PORT_E | PD_PIN_1 | PD_MUX_8) //!< PE1_EPI0S9
//
// GPIO pin E2
//
#define PD_PE2				(PD_PORT_E | PD_PIN_2 | PD_MUX_0) //!< PE2
#define PD_PE2_CCP4         (PD_PORT_E | PD_PIN_2 | PD_MUX_1) //!< PE2_CCP4
#define PD_PE2_SSI1RX       (PD_PORT_E | PD_PIN_2 | PD_MUX_2) //!< PE2_SSI1RX
#define PD_PE2_PHB1         (PD_PORT_E | PD_PIN_2 | PD_MUX_3) //!< PE2_PHB1
#define PD_PE2_PHA0         (PD_PORT_E | PD_PIN_2 | PD_MUX_4) //!< PE2_PHA0
#define PD_PE2_CCP2         (PD_PORT_E | PD_PIN_2 | PD_MUX_5) //!< PE2_CCP2
#define PD_PE2_EPI0S24      (PD_PORT_E | PD_PIN_2 | PD_MUX_8) //!< PE2_EPI0S24
//
// GPIO pin E3
//
#define PD_PE3				(PD_PORT_E | PD_PIN_3 | PD_MUX_0) //!< PE3
#define PD_PE3_CCP1         (PD_PORT_E | PD_PIN_3 | PD_MUX_1) //!< PE3_CCP1
#define PD_PE3_SSI1TX       (PD_PORT_E | PD_PIN_3 | PD_MUX_2) //!< PE3_SSI1TX
#define PD_PE3_PHA1         (PD_PORT_E | PD_PIN_3 | PD_MUX_3) //!< PE3_PHA1
#define PD_PE3_PHB0         (PD_PORT_E | PD_PIN_3 | PD_MUX_4) //!< PE3_PHB0
#define PD_PE3_CCP7         (PD_PORT_E | PD_PIN_3 | PD_MUX_5) //!< PE3_CCP7
#define PD_PE3_EPI0S25      (PD_PORT_E | PD_PIN_3 | PD_MUX_8) //!< PE3_EPI0S25
//
// GPIO pin E4
//
#define PD_PE4				(PD_PORT_E | PD_PIN_4 | PD_MUX_0) //!< PE4
#define PD_PE4_CCP3         (PD_PORT_E | PD_PIN_4 | PD_MUX_1) //!< PE4_CCP3
#define PD_PE4_FAULT0       (PD_PORT_E | PD_PIN_4 | PD_MUX_4) //!< PE4_FAULT0
#define PD_PE4_U2TX         (PD_PORT_E | PD_PIN_4 | PD_MUX_5) //!< PE4_U2TX
#define PD_PE4_CCP2         (PD_PORT_E | PD_PIN_4 | PD_MUX_6) //!< PE4_CCP2
#define PD_PE4_I2S0TXWS     (PD_PORT_E | PD_PIN_4 | PD_MUX_9) //!< PE4_I2S0TXWS
//
// GPIO pin E5
//
#define PD_PE5				(PD_PORT_E | PD_PIN_5 | PD_MUX_0) //!< PE5
#define PD_PE5_CCP5         (PD_PORT_E | PD_PIN_5 | PD_MUX_1) //!< PE5_CCP5
#define PD_PE5_I2S0TXSD     (PD_PORT_E | PD_PIN_5 | PD_MUX_9) //!< PE5_I2S0TXSD
//
// GPIO pin E6
//
#define PD_PE6				(PD_PORT_E | PD_PIN_6 | PD_MUX_0) //!< PE6
#define PD_PE6_PWM4         (PD_PORT_E | PD_PIN_6 | PD_MUX_1) //!< PE6_PWM4
#define PD_PE6_C1O          (PD_PORT_E | PD_PIN_6 | PD_MUX_2) //!< PE6_C1O
#define PD_PE6_U1CTS        (PD_PORT_E | PD_PIN_6 | PD_MUX_9) //!< PE6_U1CTS
//
// GPIO pin E7
//
#define PD_PE7				(PD_PORT_E | PD_PIN_7 | PD_MUX_0) //!< PE7
#define PD_PE7_PWM5         (PD_PORT_E | PD_PIN_7 | PD_MUX_1) //!< PE7 as PWM5
#define PD_PE7_C2O          (PD_PORT_E | PD_PIN_7 | PD_MUX_2) //!< PE7 as C2O
#define PD_PE7_U1DCD        (PD_PORT_E | PD_PIN_7 | PD_MUX_9) //!< PE7 as U1DCD
//
// GPIO pin F0
//
#define PD_PF0				(PD_PORT_F | PD_PIN_0 | PD_MUX_0) //!< PF0
#define PD_PF0_CAN1RX       (PD_PORT_F | PD_PIN_0 | PD_MUX_1) //!< PF0 as CAN1RX
#define PD_PF0_PHB0         (PD_PORT_F | PD_PIN_0 | PD_MUX_2) //!< PF0 as PHB0
#define PD_PF0_PWM0         (PD_PORT_F | PD_PIN_0 | PD_MUX_3) //!< PF0 as PWM0
#define PD_PF0_I2S0TXSD     (PD_PORT_F | PD_PIN_0 | PD_MUX_8) //!< PF0 as I2S0TXSD
#define PD_PF0_U1DSR        (PD_PORT_F | PD_PIN_0 | PD_MUX_9) //!< PF0 as U1DSR
//
// GPIO pin F1
//
#define PD_PF1				(PD_PORT_F | PD_PIN_1 | PD_MUX_0) //!< PF1
#define PD_PF1_CAN1TX       (PD_PORT_F | PD_PIN_1 | PD_MUX_1) //!< PF1 as CAN1TX
#define PD_PF1_IDX1         (PD_PORT_F | PD_PIN_1 | PD_MUX_2) //!< PF1 as IDX1
#define PD_PF1_PWM1         (PD_PORT_F | PD_PIN_1 | PD_MUX_3) //!< PF1 as PWM1
#define PD_PF1_I2S0TXMCLK   (PD_PORT_F | PD_PIN_1 | PD_MUX_8) //!< PF1 as I2S0TXMCLK
#define PD_PF1_U1RTS        (PD_PORT_F | PD_PIN_1 | PD_MUX_9) //!< PF1 as U1RTS
#define PD_PF1_CCP3         (PD_PORT_F | PD_PIN_1 | PD_MUX_10) //!<PF1 as CCP3
//
// GPIO pin F2
//
#define PD_PF2				(PD_PORT_F | PD_PIN_2 | PD_MUX_0) //!< PF2
#define PD_PF2_LED1         (PD_PORT_F | PD_PIN_2 | PD_MUX_1) //!< PF2 as LED1
#define PD_PF2_PWM4         (PD_PORT_F | PD_PIN_2 | PD_MUX_2) //!< PF2 as PWM4
#define PD_PF2_PWM2         (PD_PORT_F | PD_PIN_2 | PD_MUX_4) //!< PF2 as PWM2
#define PD_PF2_SSI1CLK      (PD_PORT_F | PD_PIN_2 | PD_MUX_9) //!< PF2 as SSI1CLK
//
// GPIO pin F3
//
#define PD_PF3				(PD_PORT_F | PD_PIN_3 | PD_MUX_0) //!< PF3
#define PD_PF3_LED0         (PD_PORT_F | PD_PIN_3 | PD_MUX_1) //!< PF3 as LED0
#define PD_PF3_PWM5         (PD_PORT_F | PD_PIN_3 | PD_MUX_2) //!< PF3 as PWM5
#define PD_PF3_PWM3         (PD_PORT_F | PD_PIN_3 | PD_MUX_4) //!< PF3 as PWM3
#define PD_PF3_SSI1FSS      (PD_PORT_F | PD_PIN_3 | PD_MUX_9) //!< PF3 as SSI1FSS
//
// GPIO pin F4
//
#define PD_PF4				(PD_PORT_F | PD_PIN_4 | PD_MUX_0) //!< PF4
#define PD_PF4_CCP0         (PD_PORT_F | PD_PIN_4 | PD_MUX_1) //!< PF4 as CCP0
#define PD_PF4_C0O          (PD_PORT_F | PD_PIN_4 | PD_MUX_2) //!< PF4 as C0O
#define PD_PF4_FAULT0       (PD_PORT_F | PD_PIN_4 | PD_MUX_4) //!< PF4 as FAULT0
#define PD_PF4_EPI0S12      (PD_PORT_F | PD_PIN_4 | PD_MUX_8) //!< PF4 as EPI0S12
#define PD_PF4_SSI1RX       (PD_PORT_F | PD_PIN_4 | PD_MUX_9) //!< PF4 as SSI1RX
//
// GPIO pin F5
//
#define PD_PF5				(PD_PORT_F | PD_PIN_5 | PD_MUX_0) //!< PF5
#define PD_PF5_CCP2         (PD_PORT_F | PD_PIN_5 | PD_MUX_1) //!< PF5 as CCP2
#define PD_PF5_C1O          (PD_PORT_F | PD_PIN_5 | PD_MUX_2) //!< PF5 as C1O
#define PD_PF5_EPI0S15      (PD_PORT_F | PD_PIN_5 | PD_MUX_8) //!< PF5 as EPI0S15
#define PD_PF5_SSI1TX       (PD_PORT_F | PD_PIN_5 | PD_MUX_9) //!< PF5 as SSI1TX
//
// GPIO pin F6
//
#define PD_PF6				(PD_PORT_F | PD_PIN_6 | PD_MUX_0) //!< PF6
#define PD_PF6_CCP1         (PD_PORT_F | PD_PIN_6 | PD_MUX_1) //!< PF6 as CCP1
#define PD_PF6_C2O          (PD_PORT_F | PD_PIN_6 | PD_MUX_2) //!< PF6 as C2O
#define PD_PF6_PHA0         (PD_PORT_F | PD_PIN_6 | PD_MUX_4) //!< PF6 as PHA0
#define PD_PF6_I2S0TXMCLK   (PD_PORT_F | PD_PIN_6 | PD_MUX_9) //!< PF6 as I2S0TXMCLK
#define PD_PF6_U1RTS        (PD_PORT_F | PD_PIN_6 | PD_MUX_10) //!<PF6 as U1RTS
//
// GPIO pin F7
//
#define PD_PF7				(PD_PORT_F | PD_PIN_7 | PD_MUX_0) //!< PF7
#define PD_PF7_CCP4         (PD_PORT_F | PD_PIN_7 | PD_MUX_1) //!< PF7 as CCP4
#define PD_PF7_PHB0         (PD_PORT_F | PD_PIN_7 | PD_MUX_4) //!< PF7 as PHB0
#define PD_PF7_EPI0S12      (PD_PORT_F | PD_PIN_7 | PD_MUX_8) //!< PF7 as EPI0S12
#define PD_PF7_FAULT1       (PD_PORT_F | PD_PIN_7 | PD_MUX_9) //!< PF7 as FAULT1
//
// GPIO pin G0
//
#define PD_PG0				(PD_PORT_G | PD_PIN_0 | PD_MUX_0) //!< PG0
#define PD_PG0_U2RX         (PD_PORT_G | PD_PIN_0 | PD_MUX_1) //!< PG0 as U2RX
#define PD_PG0_PWM0         (PD_PORT_G | PD_PIN_0 | PD_MUX_2) //!< PG0 as PWM0
#define PD_PG0_I2C1SCL      (PD_PORT_G | PD_PIN_0 | PD_MUX_3) //!< PG0 as I2C1SCL
#define PD_PG0_PWM4         (PD_PORT_G | PD_PIN_0 | PD_MUX_4) //!< PG0 as PWM4
#define PD_PG0_USB0EPEN     (PD_PORT_G | PD_PIN_0 | PD_MUX_7) //!< PG0 as USB0EPEN
#define PD_PG0_EPI0S13      (PD_PORT_G | PD_PIN_0 | PD_MUX_8) //!< PG0 as EPI0S13
//
// GPIO pin G1
//
#define PD_PG1				(PD_PORT_G | PD_PIN_1 | PD_MUX_0) //!< PG1
#define PD_PG1_U2TX         (PD_PORT_G | PD_PIN_1 | PD_MUX_1) //!< PG1 as U2TX
#define PD_PG1_PWM1         (PD_PORT_G | PD_PIN_1 | PD_MUX_2) //!< PG1 as PWM1
#define PD_PG1_I2C1SDA      (PD_PORT_G | PD_PIN_1 | PD_MUX_3) //!< PG1 as I2C1SDA
#define PD_PG1_PWM5         (PD_PORT_G | PD_PIN_1 | PD_MUX_4) //!< PG1 as PWM5
#define PD_PG1_EPI0S14      (PD_PORT_G | PD_PIN_1 | PD_MUX_8) //!< PG1 as EPI0S14
//
// GPIO pin G2
//
#define PD_PG2				(PD_PORT_G | PD_PIN_2 | PD_MUX_0) //!< PG2
#define PD_PG2_PWM0         (PD_PORT_G | PD_PIN_2 | PD_MUX_1) //!< PG2 as PWM0
#define PD_PG2_FAULT0       (PD_PORT_G | PD_PIN_2 | PD_MUX_4) //!< PG2 as FAULT0
#define PD_PG2_IDX1         (PD_PORT_G | PD_PIN_2 | PD_MUX_8) //!< PG2 as IDX1
#define PD_PG2_I2S0RXSD     (PD_PORT_G | PD_PIN_2 | PD_MUX_9) //!< PG2 as I2S0RXSD
//
// GPIO pin G3
//
#define PD_PG3				(PD_PORT_G | PD_PIN_3 | PD_MUX_0) //!< PG3
#define PD_PG3_PWM1         (PD_PORT_G | PD_PIN_3 | PD_MUX_1) //!< PG3 as PWM1
#define PD_PG3_FAULT2       (PD_PORT_G | PD_PIN_3 | PD_MUX_4) //!< PG3 as FAULT2
#define PD_PG3_FAULT0       (PD_PORT_G | PD_PIN_3 | PD_MUX_8) //!< PG3 as FAULT0
#define PD_PG3_I2S0RXMCLK   (PD_PORT_G | PD_PIN_3 | PD_MUX_9) //!< PG3 as I2S0RXMCLK
//
// GPIO pin G4
//
#define PD_PG4				(PD_PORT_G | PD_PIN_4 | PD_MUX_0) //!< PG4
#define PD_PG4_CCP3         (PD_PORT_G | PD_PIN_4 | PD_MUX_1) //!< PG4 as CCP3
#define PD_PG4_FAULT1       (PD_PORT_G | PD_PIN_4 | PD_MUX_4) //!< PG4 as FAULT1
#define PD_PG4_EPI0S15      (PD_PORT_G | PD_PIN_4 | PD_MUX_8) //!< PG4 as EPI0S15
#define PD_PG4_PWM6         (PD_PORT_G | PD_PIN_4 | PD_MUX_9) //!< PG4 as PWM6
#define PD_PG4_U1RI         (PD_PORT_G | PD_PIN_4 | PD_MUX_10) //!<PG4 as U1RI
//
// GPIO pin G5
//
#define PD_PG5				(PD_PORT_G | PD_PIN_5 | PD_MUX_0) //!< PG5
#define PD_PG5_CCP5         (PD_PORT_G | PD_PIN_5 | PD_MUX_1) //!< PG5 as CCP5
#define PD_PG5_IDX0         (PD_PORT_G | PD_PIN_5 | PD_MUX_4) //!< PG5 as IDX0
#define PD_PG5_FAULT1       (PD_PORT_G | PD_PIN_5 | PD_MUX_5) //!< PG5 as FAULT1
#define PD_PG5_PWM7         (PD_PORT_G | PD_PIN_5 | PD_MUX_8) //!< PG5 as PWM7
#define PD_PG5_I2S0RXSCK    (PD_PORT_G | PD_PIN_5 | PD_MUX_9) //!< PG5 as I2S0RXSCK
#define PD_PG5_U1DTR        (PD_PORT_G | PD_PIN_5 | PD_MUX_10) //!<PG5 as U1DTR
//
// GPIO pin G6
//
#define PD_PG6				(PD_PORT_G | PD_PIN_6 | PD_MUX_0) //!< PG6
#define PD_PG6_PHA1         (PD_PORT_G | PD_PIN_6 | PD_MUX_1) //!< PG6 as PHA1
#define PD_PG6_PWM6         (PD_PORT_G | PD_PIN_6 | PD_MUX_4) //!< PG6 as PWM6
#define PD_PG6_FAULT1       (PD_PORT_G | PD_PIN_6 | PD_MUX_8) //!< PG6 as FAULT1
#define PD_PG6_I2S0RXWS     (PD_PORT_G | PD_PIN_6 | PD_MUX_9) //!< PG6 as I2S0RXWS
#define PD_PG6_U1RI         (PD_PORT_G | PD_PIN_6 | PD_MUX_10) //!<PG6 as U1RI
//
// GPIO pin G7
//
#define PD_PG7				(PD_PORT_G | PD_PIN_7 | PD_MUX_0) //!< PG7
#define PD_PG7_PHB1         (PD_PORT_G | PD_PIN_7 | PD_MUX_1) //!< PG7 as PHB1
#define PD_PG7_PWM7         (PD_PORT_G | PD_PIN_7 | PD_MUX_4) //!< PG7 as PWM7
#define PD_PG7_CCP5         (PD_PORT_G | PD_PIN_7 | PD_MUX_8) //!< PG7 as CCP5
#define PD_PG7_EPI0S31      (PD_PORT_G | PD_PIN_7 | PD_MUX_9) //!< PG7 as EPI0S31
//
// GPIO pin H0
//
#define PD_PH0				(PD_PORT_H | PD_PIN_0 | PD_MUX_0) //!< PH0
#define PD_PH0_CCP6         (PD_PORT_H | PD_PIN_0 | PD_MUX_1) //!< PH0 as CCP6
#define PD_PH0_PWM2         (PD_PORT_H | PD_PIN_0 | PD_MUX_2) //!< PH0 as PWM2
#define PD_PH0_EPI0S6       (PD_PORT_H | PD_PIN_0 | PD_MUX_8) //!< PH0 as EPI0S6
#define PD_PH0_PWM4         (PD_PORT_H | PD_PIN_0 | PD_MUX_9) //!< PH0 as PWM4
//
// GPIO pin H1
//
#define PD_PH1				(PD_PORT_H | PD_PIN_1 | PD_MUX_0) //!< PH1
#define PD_PH1_CCP7         (PD_PORT_H | PD_PIN_1 | PD_MUX_1) //!< PH1 as CCP7
#define PD_PH1_PWM3         (PD_PORT_H | PD_PIN_1 | PD_MUX_2) //!< PH1 as PWM3
#define PD_PH1_EPI0S7       (PD_PORT_H | PD_PIN_1 | PD_MUX_8) //!< PH1 as EPI0S7
#define PD_PH1_PWM5         (PD_PORT_H | PD_PIN_1 | PD_MUX_9) //!< PH1 as PWM5
//
// GPIO pin H2
//
#define PD_PH2				(PD_PORT_H | PD_PIN_2 | PD_MUX_0) //!< PH2
#define PD_PH2_IDX1         (PD_PORT_H | PD_PIN_2 | PD_MUX_1) //!< PH2 as IDX1
#define PD_PH2_C1O          (PD_PORT_H | PD_PIN_2 | PD_MUX_2) //!< PH2 as C1O
#define PD_PH2_FAULT3       (PD_PORT_H | PD_PIN_2 | PD_MUX_4) //!< PH2 as FAULT3
#define PD_PH2_EPI0S1       (PD_PORT_H | PD_PIN_2 | PD_MUX_8) //!< PH2 as EPI0S1
//
// GPIO pin H3
//
#define PD_PH3				(PD_PORT_H | PD_PIN_3 | PD_MUX_0) //!< PH3
#define PD_PH3_PHB0         (PD_PORT_H | PD_PIN_3 | PD_MUX_1) //!< PH3 as PHB0
#define PD_PH3_FAULT0       (PD_PORT_H | PD_PIN_3 | PD_MUX_2) //!< PH3 as FAULT0
#define PD_PH3_USB0EPEN     (PD_PORT_H | PD_PIN_3 | PD_MUX_4) //!< PH3 as USB0EPEN
#define PD_PH3_EPI0S0       (PD_PORT_H | PD_PIN_3 | PD_MUX_8) //!< PH3 as EPI0S0
//
// GPIO pin H4
//
#define PD_PH4				(PD_PORT_H | PD_PIN_4 | PD_MUX_0) //!< PH4
#define PD_PH4_USB0PFLT     (PD_PORT_H | PD_PIN_4 | PD_MUX_4) //!< PH4 as USB0PFLT
#define PD_PH4_EPI0S10      (PD_PORT_H | PD_PIN_4 | PD_MUX_8) //!< PH4 as EPI0S10
#define PD_PH4_SSI1CLK      (PD_PORT_H | PD_PIN_4 | PD_MUX_11) //!<PH4 as SSI1CLK
//
// GPIO pin H5
//
#define PD_PH5				(PD_PORT_H | PD_PIN_5 | PD_MUX_0) //!< PH5
#define PD_PH5_EPI0S11      (PD_PORT_H | PD_PIN_5 | PD_MUX_8) //!< PH5 as EPI0S11
#define PD_PH5_FAULT2       (PD_PORT_H | PD_PIN_5 | PD_MUX_10) //!<PH5 as FAULT2
#define PD_PH5_SSI1FSS      (PD_PORT_H | PD_PIN_5 | PD_MUX_11) //!<PH5 as SSI1FSS
//
// GPIO pin H6
//
#define PD_PH6				(PD_PORT_H | PD_PIN_6 | PD_MUX_0) //!< PH6
#define PD_PH6_EPI0S26      (PD_PORT_H | PD_PIN_6 | PD_MUX_8) //!< PH6 as EPI0S26
#define PD_PH6_PWM4         (PD_PORT_H | PD_PIN_6 | PD_MUX_10) //!<PH6 as PWM4
#define PD_PH6_SSI1RX       (PD_PORT_H | PD_PIN_6 | PD_MUX_11) //!<PH6 as SSI1RX
//
// GPIO pin H7
//
#define PD_PH7				(PD_PORT_H | PD_PIN_7 | PD_MUX_0) //!< PH7
#define PD_PH7_EPI0S27      (PD_PORT_H | PD_PIN_7 | PD_MUX_8) //!< PH7 as EPI0S27
#define PD_PH7_PWM5         (PD_PORT_H | PD_PIN_7 | PD_MUX_10) //!<PH7 as PWM5
#define PD_PH7_SSI1TX       (PD_PORT_H | PD_PIN_7 | PD_MUX_11) //!<PH7 as SSI1TX
//
// GPIO pin J0
//
#define PD_PJ0				(PD_PORT_J | PD_PIN_0 | PD_MUX_0) //!< PJ0
#define PD_PJ0_EPI0S16      (PD_PORT_J | PD_PIN_0 | PD_MUX_8) //!< PJ0 as EPI0S16
#define PD_PJ0_PWM0         (PD_PORT_J | PD_PIN_0 | PD_MUX_10) //!<PJ0 as PWM0
#define PD_PJ0_I2C1SCL      (PD_PORT_J | PD_PIN_0 | PD_MUX_11) //!<PJ0 as I2C1SCL
//
// GPIO pin J1
//
#define PD_PJ1				(PD_PORT_J | PD_PIN_1 | PD_MUX_0) //!< PJ1
#define PD_PJ1_EPI0S17      (PD_PORT_J | PD_PIN_1 | PD_MUX_8) //!< PJ1 as EPI0S17
#define PD_PJ1_USB0PFLT     (PD_PORT_J | PD_PIN_1 | PD_MUX_9) //!< PJ1 as USB0PFLT
#define PD_PJ1_PWM1         (PD_PORT_J | PD_PIN_1 | PD_MUX_10) //!<PJ1 as PWM1
#define PD_PJ1_I2C1SDA      (PD_PORT_J | PD_PIN_1 | PD_MUX_11) //!<PJ1 as I2C1SDA
//
// GPIO pin J2
//
#define PD_PJ2				(PD_PORT_J | PD_PIN_2 | PD_MUX_0) //!< PJ2
#define PD_PJ2_EPI0S18      (PD_PORT_J | PD_PIN_2 | PD_MUX_8) //!< PJ2 as EPI0S18
#define PD_PJ2_CCP0         (PD_PORT_J | PD_PIN_2 | PD_MUX_9) //!< PJ2 as CCP0
#define PD_PJ2_FAULT0       (PD_PORT_J | PD_PIN_2 | PD_MUX_10) //!<PJ2 as FAULT0
//
// GPIO pin J3
//
#define PD_PJ3				(PD_PORT_J | PD_PIN_3 | PD_MUX_0) //!< PJ3
#define PD_PJ3_EPI0S19      (PD_PORT_J | PD_PIN_3 | PD_MUX_8) //!< PJ3 as EPI0S19
#define PD_PJ3_U1CTS        (PD_PORT_J | PD_PIN_3 | PD_MUX_9) //!< PJ3 as U1CTS
#define PD_PJ3_CCP6         (PD_PORT_J | PD_PIN_3 | PD_MUX_10) //!<PJ3 as CCP6
//
// GPIO pin J4
//
#define PD_PJ4				(PD_PORT_J | PD_PIN_4 | PD_MUX_0) //!< PJ4
#define PD_PJ4_EPI0S28      (PD_PORT_J | PD_PIN_4 | PD_MUX_8) //!< PJ4 as EPI0S28
#define PD_PJ4_U1DCD        (PD_PORT_J | PD_PIN_4 | PD_MUX_9) //!< PJ4 as U1DCD
#define PD_PJ4_CCP4         (PD_PORT_J | PD_PIN_4 | PD_MUX_10) //!<PJ4 as CCP4
//
// GPIO pin J5
//
#define PD_PJ5				(PD_PORT_J | PD_PIN_5 | PD_MUX_0) //!< PJ5
#define PD_PJ5_EPI0S29      (PD_PORT_J | PD_PIN_5 | PD_MUX_8) //!< PJ5 as EPI0S29
#define PD_PJ5_U1DSR        (PD_PORT_J | PD_PIN_5 | PD_MUX_9) //!< PJ5 as U1DSR
#define PD_PJ5_CCP2         (PD_PORT_J | PD_PIN_5 | PD_MUX_10) //!<PJ5 as CCP2
//
// GPIO pin J6
//
#define PD_PJ6				(PD_PORT_J | PD_PIN_6 | PD_MUX_0) //!< PJ6
#define PD_PJ6_EPI0S30      (PD_PORT_J | PD_PIN_6 | PD_MUX_8) //!< PJ6 as EPI0S30
#define PD_PJ6_U1RTS        (PD_PORT_J | PD_PIN_6 | PD_MUX_9) //!< PJ6 as U1RTS
#define PD_PJ6_CCP1         (PD_PORT_J | PD_PIN_6 | PD_MUX_10) //!<PJ6 as CCP1
//
// GPIO pin J7
//
#define PD_PJ7				(PD_PORT_J | PD_PIN_7 | PD_MUX_0) //!< PJ7
#define PD_PJ7_U1DTR        (PD_PORT_J | PD_PIN_7 | PD_MUX_9) //!< PJ7 as U1DTR
#define PD_PJ7_CCP0         (PD_PORT_J | PD_PIN_7 | PD_MUX_10) //!<PJ7 as CCP0

// SHIFT REGITER
#define PD_PV0				(PD_PORT_V | PD_PIN_0 | PD_MUX_0) //!< PV0
#define PD_PV1				(PD_PORT_V | PD_PIN_1 | PD_MUX_0) //!< PV1
#define PD_PV2				(PD_PORT_V | PD_PIN_2 | PD_MUX_0) //!< PV2
#define PD_PV3				(PD_PORT_V | PD_PIN_3 | PD_MUX_0) //!< PV3
#define PD_PV4				(PD_PORT_V | PD_PIN_4 | PD_MUX_0) //!< PV4
#define PD_PV5				(PD_PORT_V | PD_PIN_5 | PD_MUX_0) //!< PV5
#define PD_PV6				(PD_PORT_V | PD_PIN_6 | PD_MUX_0) //!< PV6
#define PD_PV7				(PD_PORT_V | PD_PIN_7 | PD_MUX_0) //!< PV7


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



/** @} */ // @STD_GPIO



/*----------       General Purpose Input/Output (GPIO)            -----------*/
/**  General Purpose Input/Output (GPIO) **/
struct GPIO_Type
{
	__IO uint32_t GPIODATA[256];  //!< (off: 0x000) GPIO Data
	__IO uint32_t GPIODIR;        //!< (off: 0x400) GPIO Direction
	__IO uint32_t GPIOIS;         //!< (off: 0x404) GPIO Interrupt sense
	__IO uint32_t GPIOIBE;        //!< (off: 0x408) GPIO Interrupt both edges
	__IO uint32_t GPIOIEV;        //!< (off: 0x40C) GPIO Interrupt event
	__IO uint32_t GPIOIM;         //!< (off: 0x410) GPIO Interrupt mask
	__I  uint32_t GPIORIS;        //!< (off: 0x414) GPIO Raw interrupt status
	__I  uint32_t GPIOMIS;        //!< (off: 0x418) GPIO Masked interrupt status
	__O  uint32_t GPIOICR;        //!< (off: 0x41C) GPIO Interrupt clear
	__IO uint32_t GPIOAFSEL;      //!< (off: 0x420) GPIO Mode control select
		 uint8_t  RESERVED1[220]; //!< (off: 0x424) GPIO gap
	__IO uint32_t GPIODR2R;       //!< (off: 0x500) GPIO 2ma drive select
	__IO uint32_t GPIODR4R;       //!< (off: 0x504) GPIO 4ma drive select
	__IO uint32_t GPIODR8R;       //!< (off: 0x508) GPIO 8ma drive select
	__IO uint32_t GPIOODR;        //!< (off: 0x50C) GPIO Open drain select
	__IO uint32_t GPIOPUR;        //!< (off: 0x510) GPIO Pull up select
	__IO uint32_t GPIOPDR;        //!< (off: 0x514) GPIO Pull down select
	__IO uint32_t GPIOSLR;        //!< (off: 0x518) GPIO Slew rate control enable
	__IO uint32_t GPIODEN;        //!< (off: 0x51C) GPIO Digital input enable
	__IO uint32_t GPIOLOCK;       //!< (off: 0x520) GPIO Lock
	__IO uint32_t GPIOCR;         //!< (off: 0x524) GPIO Commit
	__IO uint32_t GPIOAMSEL;      //!< (off: 0x528) GPIO Analog Mode Select
	__IO uint32_t GPIOPCTL;       //!< (off: 0x52C) GPIO Port Control

	/**
	 * Reads the values present of the specified pin(s)
	 * @param pinset
	 * @return bit-packed byte providing the state of the specified pin, where
	 * bit 0 of the byte represents GPIO port pin 0, bit 1 represents pin 1, and
	 * so on.
	 */
	uint32_t gpio_read(pio_set pinset) const
	{
	    return (GPIODATA[pinset]);
	}

	/**
	 *  Writes a value to the specified pin(s).
	 * @param pinset is the bit-packed representation of the pin(s).
	 * @param value is the value to write to the pin(s).
	 */
	void gpio_write(pio_set pinset, unsigned int value)
	{
		GPIODATA[pinset]= value;
	}

};
// </g>



#ifndef USE_AHB_PERIPHERAL
#define GPIOA               ((GPIO_Type *)GPIO_PORTA_BASE) //!< GPIOA address
#define GPIOB               ((GPIO_Type *)GPIO_PORTB_BASE) //!< GPIOB address
#define GPIOC               ((GPIO_Type *)GPIO_PORTC_BASE) //!< GPIOC address
#define GPIOD               ((GPIO_Type *)GPIO_PORTD_BASE) //!< GPIOD address
#define GPIOE               ((GPIO_Type *)GPIO_PORTE_BASE) //!< GPIOE address
#define GPIOF               ((GPIO_Type *)GPIO_PORTF_BASE) //!< GPIOF address
#define GPIOG               ((GPIO_Type *)GPIO_PORTG_BASE) //!< GPIOG address
#define GPIOH               ((GPIO_Type *)GPIO_PORTH_BASE) //!< GPIOH address
#define GPIOJ               ((GPIO_Type *)GPIO_PORTJ_BASE) //!< GPIOJ address
#else
// GPIOA_HS....
#define GPIOA            ((GPIO_Type *)GPIO_PORTA_AHB_BASE)	//!< GPIOA address
#define GPIOB            ((GPIO_Type *)GPIO_PORTB_AHB_BASE)	//!< GPIOB address
#define GPIOC            ((GPIO_Type *)GPIO_PORTC_AHB_BASE)	//!< GPIOC address
#define GPIOD            ((GPIO_Type *)GPIO_PORTD_AHB_BASE)	//!< GPIOD address
#define GPIOE            ((GPIO_Type *)GPIO_PORTE_AHB_BASE)	//!< GPIOE address
#define GPIOF            ((GPIO_Type *)GPIO_PORTF_AHB_BASE)	//!< GPIOF address
#define GPIOG            ((GPIO_Type *)GPIO_PORTG_AHB_BASE)	//!< GPIOG address
#define GPIOH            ((GPIO_Type *)GPIO_PORTH_AHB_BASE)	//!< GPIOH address
#define GPIOJ            ((GPIO_Type *)GPIO_PORTJ_AHB_BASE)	//!< GPIOJ address
#endif

#define GPIO_LOCK_UNLOCKED      0x00000000  //!< GPIO_CR register is unlocked
#define GPIO_LOCK_LOCKED        0x00000001  //!< GPIO_CR register is locked
#define GPIO_LOCK_KEY           0x1ACCE551  //!< Unlocks the GPIO_CR register
#define GPIO_LOCK_KEY_DD        0x4C4F434B  //!< Unlocks the GPIO_CR register on
                                            //!< DustDevil-class devices and
                                            //!< later

/// Calculate port address for a given pin definition
#define PORT_ADDRESS(num) ((GPIO_Type*)(GPIO_PORTA_AHB_BASE + ((num)<<12)))

extern const DRIVER_INDEX GPIO_IRQ_INDX[];

void PIO_Cfg(PIN_DESC cfg);
void PIO_CfgOutput1(PIN_DESC pins);
void PIO_CfgOutput0(PIN_DESC pins);
void PIO_CfgInput(PIN_DESC pins);
void PIO_Cfg_List(const PIN_DESC* list);
void PIO_CfgInput_List(PIN_DESC * list);
void PIO_Free(PIN_DESC cfg);
void PIO_Free_List(PIN_DESC* list);

pio_set PIO_Read(PIN_DESC pins);
void PIO_Write(PIN_DESC pins, unsigned int val);
void PIO_SetOutput(PIN_DESC pins);
void PIO_ClrOutput(PIN_DESC pins);
void PIO_Assert(PIN_DESC pins);
void PIO_Deassert(PIN_DESC pins);

// Virtual PIO functions which must be implemented from the application
/**
 * Write to virtual port
 * @note must be implemented from the application
 * @param pio
 * @param val
 */
void VPIO_Write(pio_def pio, unsigned int val );
/**
 * Read from virtual port
 * @note must be implemented from the application
 * @param pio
 * @return
 */
pio_set VPIO_Read(pio_def pio);


#ifdef __cplusplus
}
#endif

#endif //  GPIO_LM3S_H_
/** @} defgroup lm3s_gpio */
