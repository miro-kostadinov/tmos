/*
 * gpio_drv.h
 *
 *  Created on: 2010-4-22
 *      Author: stanly
 */

#ifndef GPIO_DRV_H_
#define GPIO_DRV_H_

#define PORTS		9

struct GPIO_DRIVER_DATA
{
	HANDLE 			waiting;		//!< doc!
	unsigned char   enable[PORTS];	//!< doc!
	unsigned char	sreg_val;		//!< doc!
};

struct GPIO_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;		//!< doc!
	GPIO_Type *	   	 	hw_base;	//!< doc!
	unsigned int		port_num;	//!< doc!
	GPIO_DRIVER_DATA * 	drv_data;	//!< doc!
};


#define PIOHND_WAITING		0x01
#define PIOHND_INTPENDING	0x02

void dcr_GPIO_driver(GPIO_DRIVER_INFO * drv_info, unsigned int reason, HANDLE hnd);
void dsr_GPIO_driver(GPIO_DRIVER_INFO * drv_info, HANDLE hnd);
void isr_GPIO_driver(GPIO_DRIVER_INFO * drv_info );

extern "C" {
union PIN_DESC_u
{
	struct{
		unsigned int pin_pattern 	:8;
		unsigned int port_num		:4;
		unsigned int pin_mux		:4;
		unsigned int pad_strength 	:4;
		unsigned int pad_type 		:5;
		unsigned int pin_dir 		:2;
		unsigned int pin_int 		:3;
		unsigned int state			:2;
	};
	unsigned int as_int;			//!<pin as int
	/** default constructor */
	PIN_DESC_u() {
	}
	;

	/** constructor from int
	 *
	 * FIXME This should become POD!!
	 * @param a
	 * @return
	 */
	PIN_DESC_u(unsigned int a): as_int(a) {};
};

typedef PIN_DESC_u PIN_DESC;
}
//
// port bit pattern
//
#define PIN_BITS		8
#define PIN_MASK		0xFF
#define PIN_OFFSET		0
// pin
#define PIN_0			0x01
#define PIN_1			0x02
#define PIN_2			0x04
#define PIN_3			0x08
#define PIN_4			0x10
#define PIN_5			0x20
#define PIN_6			0x40
#define PIN_7			0x80
// pin descriptor (offset 0)
#define PD_PIN_0			0x01
#define PD_PIN_1			0x02
#define PD_PIN_2			0x04
#define PD_PIN_3			0x08
#define PD_PIN_4			0x10
#define PD_PIN_5			0x20
#define PD_PIN_6			0x40
#define PD_PIN_7			0x80

//
// port number 0 - A, 1 - B, ....
//
#define PORT_BITS		4
#define PORT_MASK		0x0F
#define PORT_OFFSET		(PIN_OFFSET+PIN_BITS)
// port value
#define	PORT_A			0
#define	PORT_B			1
#define	PORT_C			2
#define	PORT_D			3
#define	PORT_E			4
#define	PORT_F			5
#define	PORT_G			6
#define	PORT_H			7
#define	PORT_J			8
#define PORT_V			0xF // shift register
// pin descriptor port
#define PD_PORT(VAL)		(VAL<<(PORT_OFFSET))
#define	PD_PORT_A			PD_PORT(PORT_A)
#define	PD_PORT_B			PD_PORT(PORT_B)
#define	PD_PORT_C			PD_PORT(PORT_C)
#define	PD_PORT_D			PD_PORT(PORT_D)
#define	PD_PORT_E			PD_PORT(PORT_E)
#define	PD_PORT_F			PD_PORT(PORT_F)
#define	PD_PORT_G			PD_PORT(PORT_G)
#define	PD_PORT_H			PD_PORT(PORT_H)
#define	PD_PORT_J			PD_PORT(PORT_J)
#define PD_PORT_V			PD_PORT(PORT_V)

#define MUX_BITS			4
#define MUX_MASK			0x0F
#define MUX_OFFSET			(PORT_OFFSET+PORT_BITS)
// mux value - 0,1...11
#define MUX_0				0
#define MUX_1				1
#define MUX_2				2
#define MUX_3				3
#define MUX_4				4
#define MUX_5				5
#define MUX_6				6
#define MUX_7				7
#define MUX_8				8
#define MUX_9				9
#define MUX_10				10
#define MUX_11				11
// pin descriptor mux
#define PD_MUX(VAL)			(VAL<<(MUX_OFFSET))
#define PD_MUX_0			PD_MUX(MUX_0)
#define PD_MUX_1			PD_MUX(MUX_1)
#define PD_MUX_2			PD_MUX(MUX_2)
#define PD_MUX_3			PD_MUX(MUX_3)
#define PD_MUX_4			PD_MUX(MUX_4)
#define PD_MUX_5			PD_MUX(MUX_5)
#define PD_MUX_6			PD_MUX(MUX_6)
#define PD_MUX_7			PD_MUX(MUX_7)
#define PD_MUX_8			PD_MUX(MUX_8)
#define PD_MUX_9			PD_MUX(MUX_9)
#define PD_MUX_10			PD_MUX(MUX_10)
#define PD_MUX_11			PD_MUX(MUX_11)
//
// Pad configuration
//
//#define PAD_OFFSET			(MUX_OFFSET+MUX_BITS)

#define PAD_STRENGTH_BITS	4
#define PAD_STRENGTH_MASK	0xF
#define PAD_STRENGTH_OFFSET  (MUX_OFFSET+MUX_BITS)
//
// Values that can be passed to GPIOPadConfigSet as the ulStrength parameter,
// and returned by GPIOPadConfigGet in the *pulStrength parameter.
//
// pad strength value
#define PAD_STRENGTH_2MA      		GPIO_STRENGTH_2MA  		// 2mA drive strength
#define PAD_STRENGTH_4MA      		GPIO_STRENGTH_4MA  		// 4mA drive strength
#define PAD_STRENGTH_8MA      		GPIO_STRENGTH_8MA 		// 8mA drive strength
#define PAD_STRENGTH_8MA_SC   		GPIO_STRENGTH_8MA_SC  	// 8mA drive with slew rate control
// pin descriptor pad strength
#define DEF_PAD_STRENGTH(VAL)		(VAL<<PAD_STRENGTH_OFFSET)
#define PD_PAD_STRENGTH_2MA  		DEF_PAD_STRENGTH(PAD_STRENGTH_2MA)  	// 2mA drive strength
#define PD_PAD_STRENGTH_4MA  		DEF_PAD_STRENGTH(PAD_STRENGTH_4MA)  	// 4mA drive strength
#define PD_PAD_STRENGTH_8MA  		DEF_PAD_STRENGTH(PAD_STRENGTH_8MA) 	// 8mA drive strength
#define PD_PAD_STRENGTH_8MA_SC		DEF_PAD_STRENGTH(PAD_STRENGTH_8MA_SC)  // 8mA drive with slew rate control

#define PAD_TYPE_BITS		5
#define PAD_TYPE_MASK		0x1F
#define PAD_TYPE_OFFSET		(PAD_STRENGTH_OFFSET+PAD_STRENGTH_BITS)
#define DEF_PAD_TYPE(VAL)	(VAL<<PAD_TYPE_OFFSET)
//
// Values that can be passed to GPIOPadConfigSet as the ulPadType parameter,
// and returned by GPIOPadConfigGet in the *pulPadType parameter.
//
// pin type value
#define PAD_TYPE_STD       				GPIO_PIN_TYPE_STD  		// Push-pull
#define PAD_TYPE_STD_WPU   				GPIO_PIN_TYPE_STD_WPU  	// Push-pull with weak pull-up
#define PAD_TYPE_STD_WPD   				GPIO_PIN_TYPE_STD_WPD  	// Push-pull with weak pull-down
#define PAD_TYPE_OD        				GPIO_PIN_TYPE_OD  		// Open-drain
#define PAD_TYPE_OD_WPU    				GPIO_PIN_TYPE_OD_WPU  	// Open-drain with weak pull-up
#define PAD_TYPE_OD_WPD    				GPIO_PIN_TYPE_OD_WPD  	// Open-drain with weak pull-down
#define PAD_TYPE_ANALOG    				GPIO_PIN_TYPE_ANALOG  	// 0x00000100  // Analog comparator
// pin descriptor pad type
#define DEF_PAD_TYPE(VAL)	(VAL<<PAD_TYPE_OFFSET)
#define PD_PAD_TYPE_STD       				DEF_PAD_TYPE(PAD_TYPE_STD)  		// Push-pull
#define PD_PAD_TYPE_STD_WPU   				DEF_PAD_TYPE(PAD_TYPE_STD_WPU)  	// Push-pull with weak pull-up
#define PD_PAD_TYPE_STD_WPD   				DEF_PAD_TYPE(PAD_TYPE_STD_WPD)  	// Push-pull with weak pull-down
#define PD_PAD_TYPE_OD        				DEF_PAD_TYPE(PAD_TYPE_OD)  		// Open-drain
#define PD_PAD_TYPE_OD_WPU    				DEF_PAD_TYPE(PAD_TYPE_OD_WPU)  	// Open-drain with weak pull-up
#define PD_PAD_TYPE_OD_WPD    				DEF_PAD_TYPE(PAD_TYPE_OD_WPD)  	// Open-drain with weak pull-down
#define PD_PAD_TYPE_ANALOG    				DEF_PAD_TYPE(PAD_TYPE_ANALOG)  	// 0x00000100  // Analog comparator

#define DIR_BITS			2
#define DIR_MASK			0x03
#define DIR_OFFSET			(PAD_TYPE_OFFSET+PAD_TYPE_BITS)
//
// Values that can be passed to GPIODirModeSet as the ulPinIO parameter, and
// returned from GPIODirModeGet.
//
// pin dir value
#define DIR_IN        				GPIO_DIR_MODE_IN  // Pin is a GPIO input
#define DIR_OUT       				GPIO_DIR_MODE_OUT // Pin is a GPIO output
#define DIR_HW        				GPIO_DIR_MODE_HW  // Pin is a peripheral function
// pin descriptor dir
#define DEF_PIN_DIR(VAL)	(VAL<<DIR_OFFSET)
#define PD_DIR_IN      				DEF_PIN_DIR(DIR_IN)  // Pin is a GPIO input
#define PD_DIR_OUT     				DEF_PIN_DIR(DIR_OUT)  // Pin is a GPIO output
#define PD_DIR_HW      				DEF_PIN_DIR(DIR_HW)  // Pin is a peripheral function

#define INT_BITS			3
#define INT_MASK			0x07
#define INT_OFFSET			(DIR_OFFSET+DIR_BITS)
//
// Values that can be passed to GPIOIntTypeSet as the ulIntType parameter, and
// returned from GPIOIntTypeGet.
//
// pin interrupt value
#define INT_FALLING_EDGE    		GPIO_FALLING_EDGE   // Interrupt on falling edge
#define INT_RISING_EDGE     		GPIO_RISING_EDGE  	// Interrupt on rising edge
#define INT_BOTH_EDGES      		GPIO_BOTH_EDGES  	// Interrupt on both edges
#define INT_LOW_LEVEL       		GPIO_LOW_LEVEL  	// Interrupt on low level
#define INT_HIGH_LEVEL      		GPIO_HIGH_LEVEL  	// Interrupt on high level
// pin descriptor interrupt
#define DEF_PIN_INT(VAL)	(VAL<<INT_OFFSET)
#define PD_INT_FALLING_EDGE    		DEF_PIN_INT(INT_FALLING_EDGE)  // Interrupt on falling edge
#define PD_INT_RISING_EDGE     		DEF_PIN_INT(INT_RISING_EDGE)  	// Interrupt on rising edge
#define PD_INT_BOTH_EDGES      		DEF_PIN_INT(INT_BOTH_EDGES)  	// Interrupt on both edges
#define PD_INT_LOW_LEVEL       		DEF_PIN_INT(INT_LOW_LEVEL)  	// Interrupt on low level
#define PD_INT_HIGH_LEVEL      		DEF_PIN_INT(INT_HIGH_LEVEL)  	// Interrupt on high level


#define STATE_BITS			2
#define STATE_MASK			0x03
#define STATE_OFFSET		(INT_OFFSET+INT_BITS)

// pin value
#define PIN_INT_ENABLE		1
#define PIN_UNLOCK			2
// pin descriptor state
#define DEF_PIN_STATE(VAL)	(VAL<<STATE_OFFSET)
#define PD_PIN_INT_ENABLE	DEF_PIN_STATE(PIN_INT_ENABLE)
#define PD_PIN_UNLOCK		DEF_PIN_STATE(PIN_UNLOCK)


#define PIN_CFG_BITS		(PAD_STRENGTH_BITS+PAD_TYPE_BITS+DIR_BITS+INT_BITS+RESERVED_BITS)
#define PIN_CFG_MASK		((PAD_STRENGTH_MASK<<PAD_STREGTH_OFFSET)|(PAD_TYPE_MASK<<PAD_TYPE_OFFSET)|(DIR_MASK<<DIR_OFFSET)|(INT_MASK<<INT_OFFSET))

// convert from PIN_DESC to pin value
#define GET_PORT(PIN_DESC)				((PIN_DESC>>PORT_OFFSET)&PORT_MASK)
#define GET_PIN(PIN_DESC)				((PIN_DESC>>PIN_OFFSET)&PIN_MASK)
#define GET_MUX(PIN_DESC)				((PIN_DESC>>MUX_OFFSET)&MUX_MASK)
#define GET_CFG(PIN_DESC)				((PIN_DESC>>PIN_CFG_OFFSET)&PIN_CFG_MASK)
//
#define GET_PAD_STRENGTH(PIN_DESC)		((PIN_DESC>>PAD_STRENGTH_OFFSET)&PAD_STRENGTH_MASK)
#define GET_PAD_TYPE(PIN_DESC)			((PIN_DESC>>PAD_TYPE_OFFSET)&PAD_TYPE_MASK)
#define GET_PIN_DIR(PIN_DESC)			((PIN_DESC>>DIR_OFFSET)&DIR_MASK)
#define GET_PIN_INT(PIN_DESC)			((PIN_DESC>>INT_OFFSET)&INT_MASK)
//
#define GET_PIN_PERIPHERAL(PIN_DESC) 	(0x20000000 | (1<<GET_PORT(PIN_DESC)))

//*****************************************************************************
//
// Values that can be passed to Pin_Configure as the PIN_DESC parameter.
// Pin_Configur(PD_PIN_PA0|PD_PAD_TYPE_STD|PD_DIR_HW);
// Pin_Configure(PD_PIN_PA0_U1RX|PD_UART_PIN)
//
//*****************************************************************************
#define PD_UART_PIN						(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_STD|PD_DIR_HW)
#define PD_ADC_PIN						(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_ANALOG|PD_DIR_IN)
#define PD_COMP_PIN						(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_ANALOG|PD_DIR_IN)
#define PD_IN_PIN						(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_STD|PD_DIR_IN)
#define PD_OUT_PIN						(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_STD|PD_DIR_OUT)
#define PD_OUT_OD_PIN					(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_OD|PD_DIR_OUT)
#define PD_USB_ANALOG					(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_ANALOG|PD_DIR_IN)
#define PD_I2C_PIN						(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_OD_WPU|PD_DIR_HW)
#define PD_PWM_PIN						(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_STD|PD_DIR_HW)
#define PD_SSI_PIN						(PD_PAD_STRENGTH_2MA|PD_PAD_TYPE_STD|PD_DIR_HW)

//*****************************************************************************
//
// Values that can be passed to Pin_Configure, Pin_Write, Pin_Read as the PIN_DESC parameter.
//
//*****************************************************************************
#define DEFINE_PIN_DESC( PORT, PIN, MUX ) 			\
	((PORT<<PORT_OFFSET)|(PIN<<PIN_OFFSET)|(MUX<<MUX_OFFSET))

//
// GPIO pin A0
//
#define PD_PIN_PA0				DEFINE_PIN_DESC(PORT_A,PIN_0,0)   // normal pin
#define PD_PIN_PA0_U0RX           DEFINE_PIN_DESC(PORT_A,PIN_0,1) // alternate functions (mux defined)
#define PD_PIN_PA0_I2C1SCL        DEFINE_PIN_DESC(PORT_A,PIN_0,8)
#define PD_PIN_PA0_U1RX           DEFINE_PIN_DESC(PORT_A,PIN_0,9)
//
// GPIO pin A1
//
#define PD_PIN_PA1				DEFINE_PIN_DESC(PORT_A,PIN_1,0)
#define PD_PIN_PA1_U0TX           DEFINE_PIN_DESC(PORT_A,PIN_1,1)
#define PD_PIN_PA1_I2C1SDA        DEFINE_PIN_DESC(PORT_A,PIN_1,8)
#define PD_PIN_PA1_U1TX           DEFINE_PIN_DESC(PORT_A,PIN_1,9)
//
// GPIO pin A2
//
#define PD_PIN_PA2				DEFINE_PIN_DESC(PORT_A,PIN_2,0)
#define PD_PIN_PA2_SSI0CLK        DEFINE_PIN_DESC(PORT_A,PIN_2,1)
#define PD_PIN_PA2_PWM4           DEFINE_PIN_DESC(PORT_A,PIN_2,4)
#define PD_PIN_PA2_I2S0RXSD       DEFINE_PIN_DESC(PORT_A,PIN_2,9)
//
// GPIO pin A3
//
#define PD_PIN_PA3				DEFINE_PIN_DESC(PORT_A,PIN_3,0)
#define PD_PIN_PA3_SSI0FSS        DEFINE_PIN_DESC(PORT_A,PIN_3,1)
#define PD_PIN_PA3_PWM5           DEFINE_PIN_DESC(PORT_A,PIN_3,4)
#define PD_PIN_PA3_I2S0RXMCLK     DEFINE_PIN_DESC(PORT_A,PIN_3,9)
//
// GPIO pin A4
//
#define PD_PIN_PA4				DEFINE_PIN_DESC(PORT_A,PIN_4,0)
#define PD_PIN_PA4_SSI0RX         DEFINE_PIN_DESC(PORT_A,PIN_4,1)
#define PD_PIN_PA4_PWM6           DEFINE_PIN_DESC(PORT_A,PIN_4,4)
#define PD_PIN_PA4_CAN0RX         DEFINE_PIN_DESC(PORT_A,PIN_4,5)
#define PD_PIN_PA4_I2S0TXSCK      DEFINE_PIN_DESC(PORT_A,PIN_4,9)
//
// GPIO pin A5
//
#define PD_PIN_PA5				DEFINE_PIN_DESC(PORT_A,PIN_5,0)
#define PD_PIN_PA5_SSI0TX         DEFINE_PIN_DESC(PORT_A,PIN_5,1)
#define PD_PIN_PA5_PWM7           DEFINE_PIN_DESC(PORT_A,PIN_5,4)
#define PD_PIN_PA5_CAN0TX         DEFINE_PIN_DESC(PORT_A,PIN_5,5)
#define PD_PIN_PA5_I2S0TXWS       DEFINE_PIN_DESC(PORT_A,PIN_5,9)
//
// GPIO pin A6
//
#define PD_PIN_PA6				DEFINE_PIN_DESC(PORT_A,PIN_6,0)
#define PD_PIN_PA6_I2C1SCL        DEFINE_PIN_DESC(PORT_A,PIN_6,1)
#define PD_PIN_PA6_CCP1           DEFINE_PIN_DESC(PORT_A,PIN_6,2)
#define PD_PIN_PA6_PWM0           DEFINE_PIN_DESC(PORT_A,PIN_6,4)
#define PD_PIN_PA6_PWM4           DEFINE_PIN_DESC(PORT_A,PIN_6,5)
#define PD_PIN_PA6_CAN0RX         DEFINE_PIN_DESC(PORT_A,PIN_6,6)
#define PD_PIN_PA6_USB0EPEN       DEFINE_PIN_DESC(PORT_A,PIN_6,8)
#define PD_PIN_PA6_U1CTS          DEFINE_PIN_DESC(PORT_A,PIN_6,9)
//
// GPIO pin A7
//
#define PD_PIN_PA7				DEFINE_PIN_DESC(PORT_A,PIN_7,0)
#define PD_PIN_PA7_I2C1SDA        DEFINE_PIN_DESC(PORT_A,PIN_7,1)
#define PD_PIN_PA7_CCP4           DEFINE_PIN_DESC(PORT_A,PIN_7,2)
#define PD_PIN_PA7_PWM1           DEFINE_PIN_DESC(PORT_A,PIN_7,4)
#define PD_PIN_PA7_PWM5           DEFINE_PIN_DESC(PORT_A,PIN_7,5)
#define PD_PIN_PA7_CAN0TX         DEFINE_PIN_DESC(PORT_A,PIN_7,6)
#define PD_PIN_PA7_CCP3           DEFINE_PIN_DESC(PORT_A,PIN_7,7)
#define PD_PIN_PA7_USB0PFLT       DEFINE_PIN_DESC(PORT_A,PIN_7,8)
#define PD_PIN_PA7_U1DCD          DEFINE_PIN_DESC(PORT_A,PIN_7,9)
//
// GPIO pin B0
//
#define PD_PIN_PB0				DEFINE_PIN_DESC(PORT_B,PIN_0,0)
#define PD_PIN_PB0_CCP0           DEFINE_PIN_DESC(PORT_B,PIN_0,1)
#define PD_PIN_PB0_PWM2           DEFINE_PIN_DESC(PORT_B,PIN_0,2)
#define PD_PIN_PB0_U1RX           DEFINE_PIN_DESC(PORT_B,PIN_0,5)
//
// GPIO pin B1
//
#define PD_PIN_PB1				DEFINE_PIN_DESC(PORT_B,PIN_1,0)
#define PD_PIN_PB1_CCP2           DEFINE_PIN_DESC(PORT_B,PIN_1,1)
#define PD_PIN_PB1_PWM3           DEFINE_PIN_DESC(PORT_B,PIN_1,2)
#define PD_PIN_PB1_CCP1           DEFINE_PIN_DESC(PORT_B,PIN_1,4)
#define PD_PIN_PB1_U1TX           DEFINE_PIN_DESC(PORT_B,PIN_1,5)
//
// GPIO pin B2
//
#define PD_PIN_PB2				DEFINE_PIN_DESC(PORT_B,PIN_2,0)
#define PD_PIN_PB2_I2C0SCL        DEFINE_PIN_DESC(PORT_B,PIN_2,1)
#define PD_PIN_PB2_IDX0           DEFINE_PIN_DESC(PORT_B,PIN_2,2)
#define PD_PIN_PB2_CCP3           DEFINE_PIN_DESC(PORT_B,PIN_2,4)
#define PD_PIN_PB2_CCP0           DEFINE_PIN_DESC(PORT_B,PIN_2,5)
#define PD_PIN_PB2_USB0EPEN       DEFINE_PIN_DESC(PORT_B,PIN_2,8)
//
// GPIO pin B3
//
#define PD_PIN_PB3				DEFINE_PIN_DESC(PORT_B,PIN_3,0)
#define PD_PIN_PB3_I2C0SDA        DEFINE_PIN_DESC(PORT_B,PIN_3,1)
#define PD_PIN_PB3_FAULT0         DEFINE_PIN_DESC(PORT_B,PIN_3,2)
#define PD_PIN_PB3_FAULT3         DEFINE_PIN_DESC(PORT_B,PIN_3,4)
#define PD_PIN_PB3_USB0PFLT       DEFINE_PIN_DESC(PORT_B,PIN_3,8)
//
// GPIO pin B4
//
#define PD_PIN_PB4				DEFINE_PIN_DESC(PORT_B,PIN_4,0)
#define PD_PIN_PB4_U2RX           DEFINE_PIN_DESC(PORT_B,PIN_4,4)
#define PD_PIN_PB4_CAN0RX         DEFINE_PIN_DESC(PORT_B,PIN_4,5)
#define PD_PIN_PB4_IDX0           DEFINE_PIN_DESC(PORT_B,PIN_4,6)
#define PD_PIN_PB4_U1RX           DEFINE_PIN_DESC(PORT_B,PIN_4,7)
#define PD_PIN_PB4_EPI0S23        DEFINE_PIN_DESC(PORT_B,PIN_4,8)
//
// GPIO pin B5
//
#define PD_PIN_PB5				DEFINE_PIN_DESC(PORT_B,PIN_5,0)
#define PD_PIN_PB5_C0O            DEFINE_PIN_DESC(PORT_B,PIN_5,1)
#define PD_PIN_PB5_CCP5           DEFINE_PIN_DESC(PORT_B,PIN_5,2)
#define PD_PIN_PB5_CCP6           DEFINE_PIN_DESC(PORT_B,PIN_5,3)
#define PD_PIN_PB5_CCP0           DEFINE_PIN_DESC(PORT_B,PIN_5,4)
#define PD_PIN_PB5_CAN0TX         DEFINE_PIN_DESC(PORT_B,PIN_5,5)
#define PD_PIN_PB5_CCP2           DEFINE_PIN_DESC(PORT_B,PIN_5,6)
#define PD_PIN_PB5_U1TX           DEFINE_PIN_DESC(PORT_B,PIN_5,7)
#define PD_PIN_PB5_EPI0S22        DEFINE_PIN_DESC(PORT_B,PIN_5,8)
//
// GPIO pin B6
//
#define PD_PIN_PB6				DEFINE_PIN_DESC(PORT_B,PIN_6,0)
#define PD_PIN_PB6_CCP1           DEFINE_PIN_DESC(PORT_B,PIN_6,1)
#define PD_PIN_PB6_CCP7           DEFINE_PIN_DESC(PORT_B,PIN_6,2)
#define PD_PIN_PB6_C0O            DEFINE_PIN_DESC(PORT_B,PIN_6,3)
#define PD_PIN_PB6_FAULT1         DEFINE_PIN_DESC(PORT_B,PIN_6,4)
#define PD_PIN_PB6_IDX0           DEFINE_PIN_DESC(PORT_B,PIN_6,5)
#define PD_PIN_PB6_CCP5           DEFINE_PIN_DESC(PORT_B,PIN_6,6)
#define PD_PIN_PB6_I2S0TXSCK      DEFINE_PIN_DESC(PORT_B,PIN_6,9)
//
// GPIO pin B7
//
#define PD_PIN_PB7				DEFINE_PIN_DESC(PORT_B,PIN_7,0)
#define PD_PIN_PB7_NMI			DEFINE_PIN_DESC(PORT_B,PIN_7,4)
//
// GPIO pin C0
//
#define PD_PIN_PC0				DEFINE_PIN_DESC(PORT_C,PIN_0,0)
#define PD_PIN_PC0_TCK            DEFINE_PIN_DESC(PORT_C,PIN_0,3)
//
// GPIO pin C1
//
#define PD_PIN_PC1				DEFINE_PIN_DESC(PORT_C,PIN_1,0)
#define PD_PIN_PC1_TMS			DEFINE_PIN_DESC(PORT_C,PIN_1,3)
//
// GPIO pin C2
//
#define PD_PIN_PC2				DEFINE_PIN_DESC(PORT_C,PIN_2,0)
#define PD_PIN_PC2_TDI			DEFINE_PIN_DESC(PORT_C,PIN_2,3)
//
// GPIO pin C3
//
#define PD_PIN_PC3				DEFINE_PIN_DESC(PORT_C,PIN_3,0)
#define PD_PIN_PC3_TDO			DEFINE_PIN_DESC(PORT_C,PIN_3,3)
//
// GPIO pin C4
//
#define PD_PIN_PC4				DEFINE_PIN_DESC(PORT_C,PIN_4,0)
#define PD_PIN_PC4_CCP5           DEFINE_PIN_DESC(PORT_C,PIN_4,1)
#define PD_PIN_PC4_PHA0           DEFINE_PIN_DESC(PORT_C,PIN_4,2)
#define PD_PIN_PC4_PWM6           DEFINE_PIN_DESC(PORT_C,PIN_4,4)
#define PD_PIN_PC4_CCP2           DEFINE_PIN_DESC(PORT_C,PIN_4,5)
#define PD_PIN_PC4_CCP4           DEFINE_PIN_DESC(PORT_C,PIN_4,6)
#define PD_PIN_PC4_EPI0S2         DEFINE_PIN_DESC(PORT_C,PIN_4,8)
#define PD_PIN_PC4_CCP1           DEFINE_PIN_DESC(PORT_C,PIN_4,9)
//
// GPIO pin C5
//
#define PD_PIN_PC5				DEFINE_PIN_DESC(PORT_C,PIN_5,0)
#define PD_PIN_PC5_CCP1           DEFINE_PIN_DESC(PORT_C,PIN_5,1)
#define PD_PIN_PC5_C1O            DEFINE_PIN_DESC(PORT_C,PIN_5,2)
#define PD_PIN_PC5_C0O            DEFINE_PIN_DESC(PORT_C,PIN_5,3)
#define PD_PIN_PC5_FAULT2         DEFINE_PIN_DESC(PORT_C,PIN_5,4)
#define PD_PIN_PC5_CCP3           DEFINE_PIN_DESC(PORT_C,PIN_5,5)
#define PD_PIN_PC5_USB0EPEN       DEFINE_PIN_DESC(PORT_C,PIN_5,6)
#define PD_PIN_PC5_EPI0S3         DEFINE_PIN_DESC(PORT_C,PIN_5,8)
//
// GPIO pin C6
//
#define PD_PIN_PC6				DEFINE_PIN_DESC(PORT_C,PIN_6,0)
#define PD_PIN_PC6_CCP3           DEFINE_PIN_DESC(PORT_C,PIN_6,1)
#define PD_PIN_PC6_PHB0           DEFINE_PIN_DESC(PORT_C,PIN_6,2)
#define PD_PIN_PC6_C2O            DEFINE_PIN_DESC(PORT_C,PIN_6,3)
#define PD_PIN_PC6_PWM7           DEFINE_PIN_DESC(PORT_C,PIN_6,4)
#define PD_PIN_PC6_U1RX           DEFINE_PIN_DESC(PORT_C,PIN_6,5)
#define PD_PIN_PC6_CCP0           DEFINE_PIN_DESC(PORT_C,PIN_6,6)
#define PD_PIN_PC6_USB0PFLT       DEFINE_PIN_DESC(PORT_C,PIN_6,7)
#define PD_PIN_PC6_EPI0S4         DEFINE_PIN_DESC(PORT_C,PIN_6,8)
//
// GPIO pin C7
//
#define PD_PIN_PC7				DEFINE_PIN_DESC(PORT_C,PIN_7,0)
#define PD_PIN_PC7_CCP4           DEFINE_PIN_DESC(PORT_C,PIN_7,1)
#define PD_PIN_PC7_PHB0           DEFINE_PIN_DESC(PORT_C,PIN_7,2)
#define PD_PIN_PC7_CCP0           DEFINE_PIN_DESC(PORT_C,PIN_7,4)
#define PD_PIN_PC7_U1TX           DEFINE_PIN_DESC(PORT_C,PIN_7,5)
#define PD_PIN_PC7_USB0PFLT       DEFINE_PIN_DESC(PORT_C,PIN_7,6)
#define PD_PIN_PC7_C1O            DEFINE_PIN_DESC(PORT_C,PIN_7,7)
#define PD_PIN_PC7_EPI0S5         DEFINE_PIN_DESC(PORT_C,PIN_7,8)
//
// GPIO pin D0
//
#define PD_PIN_PD0				DEFINE_PIN_DESC(PORT_D,PIN_0,0)
#define PD_PIN_PD0_PWM0           DEFINE_PIN_DESC(PORT_D,PIN_0,1)
#define PD_PIN_PD0_CAN0RX         DEFINE_PIN_DESC(PORT_D,PIN_0,2)
#define PD_PIN_PD0_IDX0           DEFINE_PIN_DESC(PORT_D,PIN_0,3)
#define PD_PIN_PD0_U2RX           DEFINE_PIN_DESC(PORT_D,PIN_0,4)
#define PD_PIN_PD0_U1RX           DEFINE_PIN_DESC(PORT_D,PIN_0,5)
#define PD_PIN_PD0_CCP6           DEFINE_PIN_DESC(PORT_D,PIN_0,6)
#define PD_PIN_PD0_I2S0RXSCK      DEFINE_PIN_DESC(PORT_D,PIN_0,8)
#define PD_PIN_PD0_U1CTS          DEFINE_PIN_DESC(PORT_D,PIN_0,9)
//
// GPIO pin D1
//
#define PD_PIN_PD1				DEFINE_PIN_DESC(PORT_D,PIN_1,0)
#define PD_PIN_PD1_PWM1           DEFINE_PIN_DESC(PORT_D,PIN_1,1)
#define PD_PIN_PD1_CAN0TX         DEFINE_PIN_DESC(PORT_D,PIN_1,2)
#define PD_PIN_PD1_PHA0           DEFINE_PIN_DESC(PORT_D,PIN_1,3)
#define PD_PIN_PD1_U2TX           DEFINE_PIN_DESC(PORT_D,PIN_1,4)
#define PD_PIN_PD1_U1TX           DEFINE_PIN_DESC(PORT_D,PIN_1,5)
#define PD_PIN_PD1_CCP7           DEFINE_PIN_DESC(PORT_D,PIN_1,6)
#define PD_PIN_PD1_I2S0RXWS       DEFINE_PIN_DESC(PORT_D,PIN_1,8)
#define PD_PIN_PD1_U1DCD          DEFINE_PIN_DESC(PORT_D,PIN_1,9)
#define PD_PIN_PD1_CCP2           DEFINE_PIN_DESC(PORT_D,PIN_1,10)
#define PD_PIN_PD1_PHB1           DEFINE_PIN_DESC(PORT_D,PIN_1,11)
//
// GPIO pin D2
//
#define PD_PIN_PD2				DEFINE_PIN_DESC(PORT_D,PIN_2,0)
#define PD_PIN_PD2_U1RX           DEFINE_PIN_DESC(PORT_D,PIN_2,1)
#define PD_PIN_PD2_CCP6           DEFINE_PIN_DESC(PORT_D,PIN_2,2)
#define PD_PIN_PD2_PWM2           DEFINE_PIN_DESC(PORT_D,PIN_2,3)
#define PD_PIN_PD2_CCP5           DEFINE_PIN_DESC(PORT_D,PIN_2,4)
#define PD_PIN_PD2_EPI0S20        DEFINE_PIN_DESC(PORT_D,PIN_2,8)
//
// GPIO pin D3
//
#define PD_PIN_PD3				DEFINE_PIN_DESC(PORT_D,PIN_3,0)
#define PD_PIN_PD3_U1TX           DEFINE_PIN_DESC(PORT_D,PIN_3,1)
#define PD_PIN_PD3_CCP7           DEFINE_PIN_DESC(PORT_D,PIN_3,2)
#define PD_PIN_PD3_PWM3           DEFINE_PIN_DESC(PORT_D,PIN_3,3)
#define PD_PIN_PD3_CCP0           DEFINE_PIN_DESC(PORT_D,PIN_3,4)
#define PD_PIN_PD3_EPI0S21        DEFINE_PIN_DESC(PORT_D,PIN_3,8)
//
// GPIO pin D4
//
#define PD_PIN_PD4				DEFINE_PIN_DESC(PORT_D,PIN_4,0)
#define PD_PIN_PD4_CCP0           DEFINE_PIN_DESC(PORT_D,PIN_4,1)
#define PD_PIN_PD4_CCP3           DEFINE_PIN_DESC(PORT_D,PIN_4,2)
#define PD_PIN_PD4_I2S0RXSD       DEFINE_PIN_DESC(PORT_D,PIN_4,8)
#define PD_PIN_PD4_U1RI           DEFINE_PIN_DESC(PORT_D,PIN_4,9)
#define PD_PIN_PD4_EPI0S19        DEFINE_PIN_DESC(PORT_D,PIN_4,10)
//
// GPIO pin D5
//
#define PD_PIN_PD5				DEFINE_PIN_DESC(PORT_D,PIN_5,0)
#define PD_PIN_PD5_CCP2           DEFINE_PIN_DESC(PORT_D,PIN_5,1)
#define PD_PIN_PD5_CCP4           DEFINE_PIN_DESC(PORT_D,PIN_5,2)
#define PD_PIN_PD5_I2S0RXMCLK     DEFINE_PIN_DESC(PORT_D,PIN_5,8)
#define PD_PIN_PD5_U2RX           DEFINE_PIN_DESC(PORT_D,PIN_5,9)
#define PD_PIN_PD5_EPI0S28        DEFINE_PIN_DESC(PORT_D,PIN_5,10)
//
// GPIO pin D6
//
#define PD_PIN_PD6				DEFINE_PIN_DESC(PORT_D,PIN_6,0)
#define PD_PIN_PD6_FAULT0         DEFINE_PIN_DESC(PORT_D,PIN_6,1)
#define PD_PIN_PD6_I2S0TXSCK      DEFINE_PIN_DESC(PORT_D,PIN_6,8)
#define PD_PIN_PD6_U2TX           DEFINE_PIN_DESC(PORT_D,PIN_6,9)
#define PD_PIN_PD6_EPI0S29        DEFINE_PIN_DESC(PORT_D,PIN_6,10)
//
// GPIO pin D7
//
#define PD_PIN_PD7				DEFINE_PIN_DESC(PORT_D,PIN_7,0)
#define PD_PIN_PD7_IDX0           DEFINE_PIN_DESC(PORT_D,PIN_7,1)
#define PD_PIN_PD7_C0O            DEFINE_PIN_DESC(PORT_D,PIN_7,2)
#define PD_PIN_PD7_CCP1           DEFINE_PIN_DESC(PORT_D,PIN_7,3)
#define PD_PIN_PD7_I2S0TXWS       DEFINE_PIN_DESC(PORT_D,PIN_7,8)
#define PD_PIN_PD7_U1DTR          DEFINE_PIN_DESC(PORT_D,PIN_7,9)
#define PD_PIN_PD7_EPI0S30        DEFINE_PIN_DESC(PORT_D,PIN_7,10)
//
// GPIO pin E0
//
#define PD_PIN_PE0				DEFINE_PIN_DESC(PORT_E,PIN_0,0)
#define PD_PIN_PE0_PWM4           DEFINE_PIN_DESC(PORT_E,PIN_0,1)
#define PD_PIN_PE0_SSI1CLK        DEFINE_PIN_DESC(PORT_E,PIN_0,2)
#define PD_PIN_PE0_CCP3           DEFINE_PIN_DESC(PORT_E,PIN_0,3)
#define PD_PIN_PE0_EPI0S8         DEFINE_PIN_DESC(PORT_E,PIN_0,8)
#define PD_PIN_PE0_USB0PFLT       DEFINE_PIN_DESC(PORT_E,PIN_0,9)
//
// GPIO pin E1
//
#define PD_PIN_PE1				DEFINE_PIN_DESC(PORT_E,PIN_1,0)
#define PD_PIN_PE1_PWM5           DEFINE_PIN_DESC(PORT_E,PIN_1,1)
#define PD_PIN_PE1_SSI1FSS        DEFINE_PIN_DESC(PORT_E,PIN_1,2)
#define PD_PIN_PE1_FAULT0         DEFINE_PIN_DESC(PORT_E,PIN_1,3)
#define PD_PIN_PE1_CCP2           DEFINE_PIN_DESC(PORT_E,PIN_1,4)
#define PD_PIN_PE1_CCP6           DEFINE_PIN_DESC(PORT_E,PIN_1,5)
#define PD_PIN_PE1_EPI0S9         DEFINE_PIN_DESC(PORT_E,PIN_1,8)
//
// GPIO pin E2
//
#define PD_PIN_PE2				DEFINE_PIN_DESC(PORT_E,PIN_2,0)
#define PD_PIN_PE2_CCP4           DEFINE_PIN_DESC(PORT_E,PIN_2,1)
#define PD_PIN_PE2_SSI1RX         DEFINE_PIN_DESC(PORT_E,PIN_2,2)
#define PD_PIN_PE2_PHB1           DEFINE_PIN_DESC(PORT_E,PIN_2,3)
#define PD_PIN_PE2_PHA0           DEFINE_PIN_DESC(PORT_E,PIN_2,4)
#define PD_PIN_PE2_CCP2           DEFINE_PIN_DESC(PORT_E,PIN_2,5)
#define PD_PIN_PE2_EPI0S24        DEFINE_PIN_DESC(PORT_E,PIN_2,8)
//
// GPIO pin E3
//
#define PD_PIN_PE3				DEFINE_PIN_DESC(PORT_E,PIN_3,0)
#define PD_PIN_PE3_CCP1           DEFINE_PIN_DESC(PORT_E,PIN_3,1)
#define PD_PIN_PE3_SSI1TX         DEFINE_PIN_DESC(PORT_E,PIN_3,2)
#define PD_PIN_PE3_PHA1           DEFINE_PIN_DESC(PORT_E,PIN_3,3)
#define PD_PIN_PE3_PHB0           DEFINE_PIN_DESC(PORT_E,PIN_3,4)
#define PD_PIN_PE3_CCP7           DEFINE_PIN_DESC(PORT_E,PIN_3,5)
#define PD_PIN_PE3_EPI0S25        DEFINE_PIN_DESC(PORT_E,PIN_3,8)
//
// GPIO pin E4
//
#define PD_PIN_PE4				DEFINE_PIN_DESC(PORT_E,PIN_4,0)
#define PD_PIN_PE4_CCP3           DEFINE_PIN_DESC(PORT_E,PIN_4,1)
#define PD_PIN_PE4_FAULT0         DEFINE_PIN_DESC(PORT_E,PIN_4,4)
#define PD_PIN_PE4_U2TX           DEFINE_PIN_DESC(PORT_E,PIN_4,5)
#define PD_PIN_PE4_CCP2           DEFINE_PIN_DESC(PORT_E,PIN_4,6)
#define PD_PIN_PE4_I2S0TXWS       DEFINE_PIN_DESC(PORT_E,PIN_4,9)
//
// GPIO pin E5
//
#define PD_PIN_PE5				DEFINE_PIN_DESC(PORT_E,PIN_5,0)
#define PD_PIN_PE5_CCP5           DEFINE_PIN_DESC(PORT_E,PIN_5,1)
#define PD_PIN_PE5_I2S0TXSD       DEFINE_PIN_DESC(PORT_E,PIN_5,9)
//
// GPIO pin E6
//
#define PD_PIN_PE6				DEFINE_PIN_DESC(PORT_E,PIN_6,0)
#define PD_PIN_PE6_PWM4           DEFINE_PIN_DESC(PORT_E,PIN_6,1)
#define PD_PIN_PE6_C1O            DEFINE_PIN_DESC(PORT_E,PIN_6,2)
#define PD_PIN_PE6_U1CTS          DEFINE_PIN_DESC(PORT_E,PIN_6,9)
//
// GPIO pin E7
//
#define PD_PIN_PE7				DEFINE_PIN_DESC(PORT_E,PIN_7,0)
#define PD_PIN_PE7_PWM5           DEFINE_PIN_DESC(PORT_E,PIN_7,1)
#define PD_PIN_PE7_C2O            DEFINE_PIN_DESC(PORT_E,PIN_7,2)
#define PD_PIN_PE7_U1DCD          DEFINE_PIN_DESC(PORT_E,PIN_7,9)
//
// GPIO pin F0
//
#define PD_PIN_PF0				DEFINE_PIN_DESC(PORT_F,PIN_0,0)
#define PD_PIN_PF0_CAN1RX         DEFINE_PIN_DESC(PORT_F,PIN_0,1)
#define PD_PIN_PF0_PHB0           DEFINE_PIN_DESC(PORT_F,PIN_0,2)
#define PD_PIN_PF0_PWM0           DEFINE_PIN_DESC(PORT_F,PIN_0,3)
#define PD_PIN_PF0_I2S0TXSD       DEFINE_PIN_DESC(PORT_F,PIN_0,8)
#define PD_PIN_PF0_U1DSR         	DEFINE_PIN_DESC(PORT_F,PIN_0,9)
//
// GPIO pin F1
//
#define PD_PIN_PF1				DEFINE_PIN_DESC(PORT_F,PIN_1,0)
#define PD_PIN_PF1_CAN1TX         DEFINE_PIN_DESC(PORT_F,PIN_1,1)
#define PD_PIN_PF1_IDX1           DEFINE_PIN_DESC(PORT_F,PIN_1,2)
#define PD_PIN_PF1_PWM1           DEFINE_PIN_DESC(PORT_F,PIN_1,3)
#define PD_PIN_PF1_I2S0TXMCLK     DEFINE_PIN_DESC(PORT_F,PIN_1,8)
#define PD_PIN_PF1_U1RTS          DEFINE_PIN_DESC(PORT_F,PIN_1,9)
#define PD_PIN_PF1_CCP3           DEFINE_PIN_DESC(PORT_F,PIN_1,10)
//
// GPIO pin F2
//
#define PD_PIN_PF2				DEFINE_PIN_DESC(PORT_F,PIN_2,0)
#define PD_PIN_PF2_LED1           DEFINE_PIN_DESC(PORT_F,PIN_2,1)
#define PD_PIN_PF2_PWM4           DEFINE_PIN_DESC(PORT_F,PIN_2,2)
#define PD_PIN_PF2_PWM2           DEFINE_PIN_DESC(PORT_F,PIN_2,4)
#define PD_PIN_PF2_SSI1CLK        DEFINE_PIN_DESC(PORT_F,PIN_2,9)
//
// GPIO pin F3
//
#define PD_PIN_PF3				DEFINE_PIN_DESC(PORT_F,PIN_3,0)
#define PD_PIN_PF3_LED0           DEFINE_PIN_DESC(PORT_F,PIN_3,1)
#define PD_PIN_PF3_PWM5           DEFINE_PIN_DESC(PORT_F,PIN_3,2)
#define PD_PIN_PF3_PWM3           DEFINE_PIN_DESC(PORT_F,PIN_3,4)
#define PD_PIN_PF3_SSI1FSS        DEFINE_PIN_DESC(PORT_F,PIN_3,9)
//
// GPIO pin F4
//
#define PD_PIN_PF4				DEFINE_PIN_DESC(PORT_F,PIN_4,0)
#define PD_PIN_PF4_CCP0           DEFINE_PIN_DESC(PORT_F,PIN_4,1)
#define PD_PIN_PF4_C0O            DEFINE_PIN_DESC(PORT_F,PIN_4,2)
#define PD_PIN_PF4_FAULT0         DEFINE_PIN_DESC(PORT_F,PIN_4,4)
#define PD_PIN_PF4_EPI0S12        DEFINE_PIN_DESC(PORT_F,PIN_4,8)
#define PD_PIN_PF4_SSI1RX         DEFINE_PIN_DESC(PORT_F,PIN_4,9)
//
// GPIO pin F5
//
#define PD_PIN_PF5				DEFINE_PIN_DESC(PORT_F,PIN_5,0)
#define PD_PIN_PF5_CCP2           DEFINE_PIN_DESC(PORT_F,PIN_5,1)
#define PD_PIN_PF5_C1O            DEFINE_PIN_DESC(PORT_F,PIN_5,2)
#define PD_PIN_PF5_EPI0S15        DEFINE_PIN_DESC(PORT_F,PIN_5,8)
#define PD_PIN_PF5_SSI1TX         DEFINE_PIN_DESC(PORT_F,PIN_5,9)
//
// GPIO pin F6
//
#define PD_PIN_PF6				DEFINE_PIN_DESC(PORT_F,PIN_6,0)
#define PD_PIN_PF6_CCP1           DEFINE_PIN_DESC(PORT_F,PIN_6,1)
#define PD_PIN_PF6_C2O            DEFINE_PIN_DESC(PORT_F,PIN_6,2)
#define PD_PIN_PF6_PHA0           DEFINE_PIN_DESC(PORT_F,PIN_6,4)
#define PD_PIN_PF6_I2S0TXMCLK     DEFINE_PIN_DESC(PORT_F,PIN_6,9)
#define PD_PIN_PF6_U1RTS          DEFINE_PIN_DESC(PORT_F,PIN_6,10)
//
// GPIO pin F7
//
#define PD_PIN_PF7				DEFINE_PIN_DESC(PORT_F,PIN_7,0)
#define PD_PIN_PF7_CCP4           DEFINE_PIN_DESC(PORT_F,PIN_7,1)
#define PD_PIN_PF7_PHB0           DEFINE_PIN_DESC(PORT_F,PIN_7,4)
#define PD_PIN_PF7_EPI0S12        DEFINE_PIN_DESC(PORT_F,PIN_7,8)
#define PD_PIN_PF7_FAULT1         DEFINE_PIN_DESC(PORT_F,PIN_7,9)
//
// GPIO pin G0
//
#define PD_PIN_PG0				DEFINE_PIN_DESC(PORT_G,PIN_0,0)
#define PD_PIN_PG0_U2RX           DEFINE_PIN_DESC(PORT_G,PIN_0,1)
#define PD_PIN_PG0_PWM0           DEFINE_PIN_DESC(PORT_G,PIN_0,2)
#define PD_PIN_PG0_I2C1SCL        DEFINE_PIN_DESC(PORT_G,PIN_0,3)
#define PD_PIN_PG0_PWM4           DEFINE_PIN_DESC(PORT_G,PIN_0,4)
#define PD_PIN_PG0_USB0EPEN       DEFINE_PIN_DESC(PORT_G,PIN_0,7)
#define PD_PIN_PG0_EPI0S13        DEFINE_PIN_DESC(PORT_G,PIN_0,8)
//
// GPIO pin G1
//
#define PD_PIN_PG1				DEFINE_PIN_DESC(PORT_G,PIN_1,0)
#define PD_PIN_PG1_U2TX           DEFINE_PIN_DESC(PORT_G,PIN_1,1)
#define PD_PIN_PG1_PWM1           DEFINE_PIN_DESC(PORT_G,PIN_1,2)
#define PD_PIN_PG1_I2C1SDA        DEFINE_PIN_DESC(PORT_G,PIN_1,3)
#define PD_PIN_PG1_PWM5           DEFINE_PIN_DESC(PORT_G,PIN_1,4)
#define PD_PIN_PG1_EPI0S14        DEFINE_PIN_DESC(PORT_G,PIN_1,8)
//
// GPIO pin G2
//
#define PD_PIN_PG2				DEFINE_PIN_DESC(PORT_G,PIN_2,0)
#define PD_PIN_PG2_PWM0           DEFINE_PIN_DESC(PORT_G,PIN_2,1)
#define PD_PIN_PG2_FAULT0         DEFINE_PIN_DESC(PORT_G,PIN_2,4)
#define PD_PIN_PG2_IDX1           DEFINE_PIN_DESC(PORT_G,PIN_2,8)
#define PD_PIN_PG2_I2S0RXSD       DEFINE_PIN_DESC(PORT_G,PIN_2,9)
//
// GPIO pin G3
//
#define PD_PIN_PG3				DEFINE_PIN_DESC(PORT_G,PIN_3,0)
#define PD_PIN_PG3_PWM1           DEFINE_PIN_DESC(PORT_G,PIN_3,1)
#define PD_PIN_PG3_FAULT2         DEFINE_PIN_DESC(PORT_G,PIN_3,4)
#define PD_PIN_PG3_FAULT0         DEFINE_PIN_DESC(PORT_G,PIN_3,8)
#define PD_PIN_PG3_I2S0RXMCLK     DEFINE_PIN_DESC(PORT_G,PIN_3,9)
//
// GPIO pin G4
//
#define PD_PIN_PG4				DEFINE_PIN_DESC(PORT_G,PIN_4,0)
#define PD_PIN_PG4_CCP3           DEFINE_PIN_DESC(PORT_G,PIN_4,1)
#define PD_PIN_PG4_FAULT1         DEFINE_PIN_DESC(PORT_G,PIN_4,4)
#define PD_PIN_PG4_EPI0S15        DEFINE_PIN_DESC(PORT_G,PIN_4,8)
#define PD_PIN_PG4_PWM6           DEFINE_PIN_DESC(PORT_G,PIN_4,9)
#define PD_PIN_PG4_U1RI           DEFINE_PIN_DESC(PORT_G,PIN_4,10)
//
// GPIO pin G5
//
#define PD_PIN_PG5				DEFINE_PIN_DESC(PORT_G,PIN_5,0)
#define PD_PIN_PG5_CCP5           DEFINE_PIN_DESC(PORT_G,PIN_5,1)
#define PD_PIN_PG5_IDX0           DEFINE_PIN_DESC(PORT_G,PIN_5,4)
#define PD_PIN_PG5_FAULT1         DEFINE_PIN_DESC(PORT_G,PIN_5,5)
#define PD_PIN_PG5_PWM7           DEFINE_PIN_DESC(PORT_G,PIN_5,8)
#define PD_PIN_PG5_I2S0RXSCK      DEFINE_PIN_DESC(PORT_G,PIN_5,9)
#define PD_PIN_PG5_U1DTR          DEFINE_PIN_DESC(PORT_G,PIN_5,10)
//
// GPIO pin G6
//
#define PD_PIN_PG6				DEFINE_PIN_DESC(PORT_G,PIN_6,0)
#define PD_PIN_PG6_PHA1           DEFINE_PIN_DESC(PORT_G,PIN_6,1)
#define PD_PIN_PG6_PWM6           DEFINE_PIN_DESC(PORT_G,PIN_6,4)
#define PD_PIN_PG6_FAULT1         DEFINE_PIN_DESC(PORT_G,PIN_6,8)
#define PD_PIN_PG6_I2S0RXWS       DEFINE_PIN_DESC(PORT_G,PIN_6,9)
#define PD_PIN_PG6_U1RI           DEFINE_PIN_DESC(PORT_G,PIN_6,10)
//
// GPIO pin G7
//
#define PD_PIN_PG7				DEFINE_PIN_DESC(PORT_G,PIN_7,0)
#define PD_PIN_PG7_PHB1           DEFINE_PIN_DESC(PORT_G,PIN_7,1)
#define PD_PIN_PG7_PWM7           DEFINE_PIN_DESC(PORT_G,PIN_7,4)
#define PD_PIN_PG7_CCP5           DEFINE_PIN_DESC(PORT_G,PIN_7,8)
#define PD_PIN_PG7_EPI0S31        DEFINE_PIN_DESC(PORT_G,PIN_7,9)
//
// GPIO pin H0
//
#define PD_PIN_PH0				DEFINE_PIN_DESC(PORT_H,PIN_0,0)
#define PD_PIN_PH0_CCP6           DEFINE_PIN_DESC(PORT_H,PIN_0,1)
#define PD_PIN_PH0_PWM2           DEFINE_PIN_DESC(PORT_H,PIN_0,2)
#define PD_PIN_PH0_EPI0S6         DEFINE_PIN_DESC(PORT_H,PIN_0,8)
#define PD_PIN_PH0_PWM4           DEFINE_PIN_DESC(PORT_H,PIN_0,9)
//
// GPIO pin H1
//
#define PD_PIN_PH1				DEFINE_PIN_DESC(PORT_H,PIN_1,0)
#define PD_PIN_PH1_CCP7           DEFINE_PIN_DESC(PORT_H,PIN_1,1)
#define PD_PIN_PH1_PWM3           DEFINE_PIN_DESC(PORT_H,PIN_1,2)
#define PD_PIN_PH1_EPI0S7         DEFINE_PIN_DESC(PORT_H,PIN_1,8)
#define PD_PIN_PH1_PWM5           DEFINE_PIN_DESC(PORT_H,PIN_1,9)
//
// GPIO pin H2
//
#define PD_PIN_PH2				DEFINE_PIN_DESC(PORT_H,PIN_2,0)
#define PD_PIN_PH2_IDX1           DEFINE_PIN_DESC(PORT_H,PIN_2,1)
#define PD_PIN_PH2_C1O            DEFINE_PIN_DESC(PORT_H,PIN_2,2)
#define PD_PIN_PH2_FAULT3         DEFINE_PIN_DESC(PORT_H,PIN_2,4)
#define PD_PIN_PH2_EPI0S1         DEFINE_PIN_DESC(PORT_H,PIN_2,8)
//
// GPIO pin H3
//
#define PD_PIN_PH3				DEFINE_PIN_DESC(PORT_H,PIN_3,0)
#define PD_PIN_PH3_PHB0           DEFINE_PIN_DESC(PORT_H,PIN_3,1)
#define PD_PIN_PH3_FAULT0         DEFINE_PIN_DESC(PORT_H,PIN_3,2)
#define PD_PIN_PH3_USB0EPEN       DEFINE_PIN_DESC(PORT_H,PIN_3,4)
#define PD_PIN_PH3_EPI0S0         DEFINE_PIN_DESC(PORT_H,PIN_3,8)
//
// GPIO pin H4
//
#define PD_PIN_PH4				DEFINE_PIN_DESC(PORT_H,PIN_4,0)
#define PD_PIN_PH4_USB0PFLT       DEFINE_PIN_DESC(PORT_H,PIN_4,4)
#define PD_PIN_PH4_EPI0S10        DEFINE_PIN_DESC(PORT_H,PIN_4,8)
#define PD_PIN_PH4_SSI1CLK        DEFINE_PIN_DESC(PORT_H,PIN_4,11)
//
// GPIO pin H5
//
#define PD_PIN_PH5				DEFINE_PIN_DESC(PORT_H,PIN_5,0)
#define PD_PIN_PH5_EPI0S11        DEFINE_PIN_DESC(PORT_H,PIN_5,8)
#define PD_PIN_PH5_FAULT2         DEFINE_PIN_DESC(PORT_H,PIN_5,10)
#define PD_PIN_PH5_SSI1FSS        DEFINE_PIN_DESC(PORT_H,PIN_5,11)
//
// GPIO pin H6
//
#define PD_PIN_PH6				DEFINE_PIN_DESC(PORT_H,PIN_6,0)
#define PD_PIN_PH6_EPI0S26        DEFINE_PIN_DESC(PORT_H,PIN_6,8)
#define PD_PIN_PH6_PWM4           DEFINE_PIN_DESC(PORT_H,PIN_6,10)
#define PD_PIN_PH6_SSI1RX         DEFINE_PIN_DESC(PORT_H,PIN_6,11)
//
// GPIO pin H7
//
#define PD_PIN_PH7				DEFINE_PIN_DESC(PORT_H,PIN_7,0)
#define PD_PIN_PH7_EPI0S27        DEFINE_PIN_DESC(PORT_H,PIN_7,8)
#define PD_PIN_PH7_PWM5           DEFINE_PIN_DESC(PORT_H,PIN_7,10)
#define PD_PIN_PH7_SSI1TX         DEFINE_PIN_DESC(PORT_H,PIN_7,11)
//
// GPIO pin J0
//
#define PD_PIN_PJ0				DEFINE_PIN_DESC(PORT_J,PIN_0,0)
#define PD_PIN_PJ0_EPI0S16        DEFINE_PIN_DESC(PORT_J,PIN_0,8)
#define PD_PIN_PJ0_PWM0           DEFINE_PIN_DESC(PORT_J,PIN_0,10)
#define PD_PIN_PJ0_I2C1SCL        DEFINE_PIN_DESC(PORT_J,PIN_0,11)
//
// GPIO pin J1
//
#define PD_PIN_PJ1				DEFINE_PIN_DESC(PORT_J,PIN_1,0)
#define PD_PIN_PJ1_EPI0S17        DEFINE_PIN_DESC(PORT_J,PIN_1,8)
#define PD_PIN_PJ1_USB0PFLT       DEFINE_PIN_DESC(PORT_J,PIN_1,9)
#define PD_PIN_PJ1_PWM1           DEFINE_PIN_DESC(PORT_J,PIN_1,10)
#define PD_PIN_PJ1_I2C1SDA        DEFINE_PIN_DESC(PORT_J,PIN_1,11)
//
// GPIO pin J2
//
#define PD_PIN_PJ2				DEFINE_PIN_DESC(PORT_J,PIN_2,0)
#define PD_PIN_PJ2_EPI0S18        DEFINE_PIN_DESC(PORT_J,PIN_2,8)
#define PD_PIN_PJ2_CCP0           DEFINE_PIN_DESC(PORT_J,PIN_2,9)
#define PD_PIN_PJ2_FAULT0         DEFINE_PIN_DESC(PORT_J,PIN_2,10)
//
// GPIO pin J3
//
#define PD_PIN_PJ3				DEFINE_PIN_DESC(PORT_J,PIN_3,0)
#define PD_PIN_PJ3_EPI0S19        DEFINE_PIN_DESC(PORT_J,PIN_3,8)
#define PD_PIN_PJ3_U1CTS          DEFINE_PIN_DESC(PORT_J,PIN_3,9)
#define PD_PIN_PJ3_CCP6           DEFINE_PIN_DESC(PORT_J,PIN_3,10)
//
// GPIO pin J4
//
#define PD_PIN_PJ4				DEFINE_PIN_DESC(PORT_J,PIN_4,0)
#define PD_PIN_PJ4_EPI0S28        DEFINE_PIN_DESC(PORT_J,PIN_4,8)
#define PD_PIN_PJ4_U1DCD          DEFINE_PIN_DESC(PORT_J,PIN_4,9)
#define PD_PIN_PJ4_CCP4           DEFINE_PIN_DESC(PORT_J,PIN_4,10)
//
// GPIO pin J5
//
#define PD_PIN_PJ5				DEFINE_PIN_DESC(PORT_J,PIN_5,0)
#define PD_PIN_PJ5_EPI0S29        DEFINE_PIN_DESC(PORT_J,PIN_5,8)
#define PD_PIN_PJ5_U1DSR          DEFINE_PIN_DESC(PORT_J,PIN_5,9)
#define PD_PIN_PJ5_CCP2           DEFINE_PIN_DESC(PORT_J,PIN_5,10)
//
// GPIO pin J6
//
#define PD_PIN_PJ6				DEFINE_PIN_DESC(PORT_J,PIN_6,0)
#define PD_PIN_PJ6_EPI0S30        DEFINE_PIN_DESC(PORT_J,PIN_6,8)
#define PD_PIN_PJ6_U1RTS          DEFINE_PIN_DESC(PORT_J,PIN_6,9)
#define PD_PIN_PJ6_CCP1           DEFINE_PIN_DESC(PORT_J,PIN_6,10)
//
// GPIO pin J7
//
#define PD_PIN_PJ7				DEFINE_PIN_DESC(PORT_J,PIN_7,0)
#define PD_PIN_PJ7_U1DTR          DEFINE_PIN_DESC(PORT_J,PIN_7,9)
#define PD_PIN_PJ7_CCP0           DEFINE_PIN_DESC(PORT_J,PIN_7,10)

// SHIFT REGITER
#define PD_PIN_PV0				DEFINE_PIN_DESC(PORT_V,PIN_0,0)
#define PD_PIN_PV1				DEFINE_PIN_DESC(PORT_V,PIN_1,0)
#define PD_PIN_PV2				DEFINE_PIN_DESC(PORT_V,PIN_2,0)
#define PD_PIN_PV3				DEFINE_PIN_DESC(PORT_V,PIN_3,0)
#define PD_PIN_PV4				DEFINE_PIN_DESC(PORT_V,PIN_4,0)
#define PD_PIN_PV5				DEFINE_PIN_DESC(PORT_V,PIN_5,0)
#define PD_PIN_PV6				DEFINE_PIN_DESC(PORT_V,PIN_6,0)
#define PD_PIN_PV7				DEFINE_PIN_DESC(PORT_V,PIN_7,0)

void PIO_Cfg(PIN_DESC Pin );
void PIO_Cfg_List(PIN_DESC * list );
void PIO_CfgOutput1(PIN_DESC Pin);
void PIO_CfgOutput0(PIN_DESC Pin);
void PIO_Free(PIN_DESC Pin);
void PIO_Free_List(PIN_DESC * list );

GPIO_Type * PIO_Base(PIN_DESC Pin);

unsigned char PIO_Read(PIN_DESC Pin);
void PIO_Write(PIN_DESC Pin, unsigned char val );
void PIO_SetOutput(PIN_DESC Pin);
void PIO_ClrOutput(PIN_DESC Pin);

#endif /* GPIO_DRV_H_ */
