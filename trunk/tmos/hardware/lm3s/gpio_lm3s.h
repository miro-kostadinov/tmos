/**************************************************************************//**
 * @ingroup	 lm3s_gpio
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
#define USE_AHB_PERIPHERAL			/**< doc! */
#endif
/*----------       General Purpose Input/Output (GPIO)            -----------*/
// <g> General Purpose Input/Output (GPIO)
struct GPIO_Type
{
	__IO uint32_t DATA_Bits[256];   /**< Bit specific data registers	*/
//	__IO uint32_t DATA;             // Data register
	__IO uint32_t DIR;              /**< Data direction register	*/
	__IO uint32_t IS;               /**< Interrupt sense register	*/
	__IO uint32_t IBE;              /**< Interrupt both edges register	*/
	__IO uint32_t IEV;              /**< Interrupt event register	*/
	__IO uint32_t IM;               /**< Interrupt mask register	*/
	__I  uint32_t RIS;              /**< Raw interrupt status register	*/
	__I  uint32_t MIS;              /**< Masked interrupt status reg	*/
	__O  uint32_t ICR;              /**< Interrupt clear register	*/
	__IO uint32_t AFSEL;            /**< Mode control select register	*/
		 uint8_t  RESERVED1[220];	/**< doc! */
	__IO uint32_t DR2R;             /**< 2ma drive select register	*/
	__IO uint32_t DR4R;             /**< 4ma drive select register	*/
	__IO uint32_t DR8R;             /**< 8ma drive select register	*/
	__IO uint32_t ODR;              /**< Open drain select register	*/
	__IO uint32_t PUR;              /**< Pull up select register	*/
	__IO uint32_t PDR;              /**< Pull down select register	*/
	__IO uint32_t SLR;              /**< Slew rate control enable reg	*/
	__IO uint32_t DEN;              /**< Digital input enable register	*/
	__IO uint32_t LOCK;             /**< Lock register	*/
	__IO uint32_t CR;               /**< Commit register	*/
	__IO uint32_t AMSEL;            /**< GPIO Analog Mode Select	*/
	__IO uint32_t PCTL;             /**< GPIO Port Control	*/

	void GPIODirModeSet( unsigned char ucPins, unsigned long ulPinIO);
	unsigned long GPIODirModeGet( unsigned char ucPin);
	void GPIOIntTypeSet(unsigned char ucPins, unsigned long ulIntType);
	unsigned long GPIOIntTypeGet(unsigned char ucPin);
	void GPIOPadConfigSet(unsigned char ucPins, unsigned long ulStrength, unsigned long ulPinType);
	void GPIOPadConfigGet(unsigned char ucPin, unsigned long *pulStrength, unsigned long *pulPinType);
	void GPIOPinIntEnable(unsigned char ucPins);
	void GPIOPinIntDisable( unsigned char ucPins );
	long GPIOPinIntStatus(int bMasked);
	void GPIOPinIntClear(unsigned char ucPins);
	long GPIOPinRead(unsigned char ucPins);
	void GPIOPinWrite(unsigned char ucPins, unsigned char ucVal);
	void GPIOPinTypeADC(unsigned char ucPins);
	void GPIOPinTypeCAN(unsigned char ucPins);
	void GPIOPinTypeComparator(unsigned char ucPins);
	void GPIOPinTypeGPIOInput(unsigned char ucPins);
	void GPIOPinTypeGPIOOutput(unsigned char ucPins);
	void GPIOPinTypeGPIOOutputOD(unsigned char ucPins);
	void GPIOPinTypeI2C(unsigned char ucPins);
	void GPIOPinTypePWM(unsigned char ucPins);
	void GPIOPinTypeQEI(unsigned char ucPins);
	void GPIOPinTypeSSI(unsigned char ucPins);
	void GPIOPinTypeTimer(unsigned char ucPins);
	void GPIOPinTypeUART(unsigned char ucPins);
	void GPIOPinTypeUSBDigital(unsigned char ucPins);
	void GPIOPinTypeUSBAnalog(unsigned char ucPins);
	void GPIOPinTypeI2S(unsigned char ucPins);
	void GPIOPinTypeEthernetLED(unsigned char ucPins);
};
// </g>



//
// Pad configuration
//

//*****************************************************************************
//
// Values that can be passed to GPIOPadConfigSet as the ulStrength parameter,
// and returned by GPIOPadConfigGet in the *pulStrength parameter.
//
//*****************************************************************************
#define GPIO_STRENGTH_2MA       		0x00000001  /**< 2mA drive strength	*/
#define GPIO_STRENGTH_4MA      			0x00000002  /**< 4mA drive strength	*/
#define GPIO_STRENGTH_8MA       		0x00000004  /**< 8mA drive strength	*/
#define GPIO_STRENGTH_8MA_SC    		0x0000000C  /**< 8mA drive with slew rate control	*/

//*****************************************************************************
//
// Values that can be passed to GPIOPadConfigSet as the ulPadType parameter,
// and returned by GPIOPadConfigGet in the *pulPadType parameter.
//
//*****************************************************************************
#define GPIO_PIN_TYPE_STD       		0x00000008  /**< Push-pull	*/
#define GPIO_PIN_TYPE_STD_WPU   		0x0000000A  /**< Push-pull with weak pull-up	*/
#define GPIO_PIN_TYPE_STD_WPD   		0x0000000C  /**< Push-pull with weak pull-down	*/
#define GPIO_PIN_TYPE_OD        		0x00000009  /**< Open-drain	*/
#define GPIO_PIN_TYPE_OD_WPU    		0x0000000B  /**< Open-drain with weak pull-up	*/
#define GPIO_PIN_TYPE_OD_WPD    		0x0000000D  /**< Open-drain with weak pull-down	*/
#define GPIO_PIN_TYPE_ANALOG    		0x00000000  /**< Analog comparator	*/

//*****************************************************************************
//
// Values that can be passed to GPIODirModeSet as the ulPinIO parameter, and
// returned from GPIODirModeGet.
//
//*****************************************************************************
#define GPIO_DIR_MODE_IN        	0x00000000  /**< Pin is a GPIO input	*/
#define GPIO_DIR_MODE_OUT       	0x00000001  /**< Pin is a GPIO output	*/
#define GPIO_DIR_MODE_HW        	0x00000002  /**< Pin is a peripheral function	*/

//*****************************************************************************
//
// Values that can be passed to GPIOIntTypeSet as the ulIntType parameter, and
// returned from GPIOIntTypeGet.
//
//*****************************************************************************
#define GPIO_FALLING_EDGE       	0x00000000  /**< Interrupt on falling edge	*/
#define GPIO_RISING_EDGE        	0x00000004  /**< Interrupt on rising edge	*/
#define GPIO_BOTH_EDGES         	0x00000001  /**< Interrupt on both edges	*/
#define GPIO_LOW_LEVEL          	0x00000002  /**< Interrupt on low level	*/
#define GPIO_HIGH_LEVEL         	0x00000007  /**< Interrupt on high level	*/

//*****************************************************************************
//
// The following values define the bit field for the ucPins argument to several
// of the APIs.
//
//*****************************************************************************
#define GPIO_PIN_0              0x00000001  /**< GPIO pin 0	*/
#define GPIO_PIN_1              0x00000002  /**< GPIO pin 1	*/
#define GPIO_PIN_2              0x00000004  /**< GPIO pin 2	*/
#define GPIO_PIN_3              0x00000008  /**< GPIO pin 3	*/
#define GPIO_PIN_4              0x00000010  /**< GPIO pin 4	*/
#define GPIO_PIN_5              0x00000020  /**< GPIO pin 5	*/
#define GPIO_PIN_6              0x00000040  /**< GPIO pin 6	*/
#define GPIO_PIN_7              0x00000080  /**< GPIO pin 7	*/

//*****************************************************************************
//
// Values that can be passed to GPIOPinConfigure as the ulPinConfig parameter.
//
//*****************************************************************************
#define UART_PIN	(PAD_2MA|PAD_TYPE_STD|DIR_HW)		/**< doc! */
#define ADC_PIN		(PAD_2MA|PAD_TYPE_ANALOG|DIR_IN)	/**< doc! */
#define COMP_PIN	(PAD_2MA|PAD_TYPE_ANALOG|DIR_IN)	/**< doc! */
#define IN_PIN		(PAD_2MA|PAD_TYPE_STD|DIR_IN)		/**< doc! */
#define OUT_PIN		(PAD_2MA|PAD_TYPE_STD|DIR_OUT)		/**< doc! */
#define OUT_OD_PIN	(PAD_2MA|PAD_TYPE_OD|DIR_OUT)		/**< doc! */
#define USB_ANALOG	(PAD_2MA|PAD_TYPE_ANALOG|DIR_IN)	/**< doc! */
#define I2C_PIN		(PAD_2MA|PAD_TYPE_OD_WPU|DIR_HW)	/**< doc! */
#define PWM_PIN		(PAD_2MA|PAD_TYPE_STD|DIR_HW)		/**< doc! */
#define SSI_PIN		(PAD_2MA|PAD_TYPE_STD|DIR_HW)		/**< doc! */

//*****************************************************************************
//
// Values that can be passed to GPIOPinConfigure as the ulPinConfig parameter.
//
//*****************************************************************************
//
// GPIO pin A0
//
#define GPIO_PA0_U0RX           0x00000001	/**< doc! */
#define GPIO_PA0_I2C1SCL        0x00000008	/**< doc! */
#define GPIO_PA0_U1RX           0x00000009	/**< doc! */

//
// GPIO pin A1
//
#define GPIO_PA1_U0TX           0x00000401	/**< doc! */
#define GPIO_PA1_I2C1SDA        0x00000408	/**< doc! */
#define GPIO_PA1_U1TX           0x00000409	/**< doc! */

//
// GPIO pin A2
//
#define GPIO_PA2_SSI0CLK        0x00000801	/**< doc! */
#define GPIO_PA2_PWM4           0x00000804	/**< doc! */
#define GPIO_PA2_I2S0RXSD       0x00000809	/**< doc! */

//
// GPIO pin A3
//
#define GPIO_PA3_SSI0FSS        0x00000c01	/**< doc! */
#define GPIO_PA3_PWM5           0x00000c04	/**< doc! */
#define GPIO_PA3_I2S0RXMCLK     0x00000c09	/**< doc! */

//
// GPIO pin A4
//
#define GPIO_PA4_SSI0RX         0x00001001	/**< doc! */
#define GPIO_PA4_PWM6           0x00001004	/**< doc! */
#define GPIO_PA4_CAN0RX         0x00001005	/**< doc! */
#define GPIO_PA4_I2S0TXSCK      0x00001009	/**< doc! */

//
// GPIO pin A5
//
#define GPIO_PA5_SSI0TX         0x00001401	/**< doc! */
#define GPIO_PA5_PWM7           0x00001404	/**< doc! */
#define GPIO_PA5_CAN0TX         0x00001405	/**< doc! */
#define GPIO_PA5_I2S0TXWS       0x00001409	/**< doc! */

//
// GPIO pin A6
//
#define GPIO_PA6_I2C1SCL        0x00001801	/**< doc! */
#define GPIO_PA6_CCP1           0x00001802	/**< doc! */
#define GPIO_PA6_PWM0           0x00001804	/**< doc! */
#define GPIO_PA6_PWM4           0x00001805	/**< doc! */
#define GPIO_PA6_CAN0RX         0x00001806	/**< doc! */
#define GPIO_PA6_USB0EPEN       0x00001808	/**< doc! */
#define GPIO_PA6_U1CTS          0x00001809	/**< doc! */

//
// GPIO pin A7
//
#define GPIO_PA7_I2C1SDA        0x00001c01	/**< doc! */
#define GPIO_PA7_CCP4           0x00001c02	/**< doc! */
#define GPIO_PA7_PWM1           0x00001c04	/**< doc! */
#define GPIO_PA7_PWM5           0x00001c05	/**< doc! */
#define GPIO_PA7_CAN0TX         0x00001c06	/**< doc! */
#define GPIO_PA7_CCP3           0x00001c07	/**< doc! */
#define GPIO_PA7_USB0PFLT       0x00001c08	/**< doc! */
#define GPIO_PA7_U1DCD          0x00001c09	/**< doc! */

//
// GPIO pin B0
//
#define GPIO_PB0_CCP0           0x00010001	/**< doc! */
#define GPIO_PB0_PWM2           0x00010002	/**< doc! */
#define GPIO_PB0_U1RX           0x00010005	/**< doc! */

//
// GPIO pin B1
//
#define GPIO_PB1_CCP2           0x00010401	/**< doc! */
#define GPIO_PB1_PWM3           0x00010402	/**< doc! */
#define GPIO_PB1_CCP1           0x00010404	/**< doc! */
#define GPIO_PB1_U1TX           0x00010405	/**< doc! */

//
// GPIO pin B2
//
#define GPIO_PB2_I2C0SCL        0x00010801	/**< doc! */
#define GPIO_PB2_IDX0           0x00010802	/**< doc! */
#define GPIO_PB2_CCP3           0x00010804	/**< doc! */
#define GPIO_PB2_CCP0           0x00010805	/**< doc! */
#define GPIO_PB2_USB0EPEN       0x00010808	/**< doc! */

//
// GPIO pin B3
//
#define GPIO_PB3_I2C0SDA        0x00010c01	/**< doc! */
#define GPIO_PB3_FAULT0         0x00010c02	/**< doc! */
#define GPIO_PB3_FAULT3         0x00010c04	/**< doc! */
#define GPIO_PB3_USB0PFLT       0x00010c08	/**< doc! */

//
// GPIO pin B4
//
#define GPIO_PB4_U2RX           0x00011004	/**< doc! */
#define GPIO_PB4_CAN0RX         0x00011005	/**< doc! */
#define GPIO_PB4_IDX0           0x00011006	/**< doc! */
#define GPIO_PB4_U1RX           0x00011007	/**< doc! */
#define GPIO_PB4_EPI0S23        0x00011008	/**< doc! */

//
// GPIO pin B5
//
#define GPIO_PB5_C0O            0x00011401	/**< doc! */
#define GPIO_PB5_CCP5           0x00011402	/**< doc! */
#define GPIO_PB5_CCP6           0x00011403	/**< doc! */
#define GPIO_PB5_CCP0           0x00011404	/**< doc! */
#define GPIO_PB5_CAN0TX         0x00011405	/**< doc! */
#define GPIO_PB5_CCP2           0x00011406	/**< doc! */
#define GPIO_PB5_U1TX           0x00011407	/**< doc! */
#define GPIO_PB5_EPI0S22        0x00011408	/**< doc! */

//
// GPIO pin B6
//
#define GPIO_PB6_CCP1           0x00011801	/**< doc! */
#define GPIO_PB6_CCP7           0x00011802	/**< doc! */
#define GPIO_PB6_C0O            0x00011803	/**< doc! */
#define GPIO_PB6_FAULT1         0x00011804	/**< doc! */
#define GPIO_PB6_IDX0           0x00011805	/**< doc! */
#define GPIO_PB6_CCP5           0x00011806	/**< doc! */
#define GPIO_PB6_I2S0TXSCK      0x00011809	/**< doc! */

//
// GPIO pin B7
//
#define GPIO_PB7_NMI            0x00011c04	/**< doc! */

//
// GPIO pin C0
//
#define GPIO_PC0_TCK            0x00020003	/**< doc! */

//
// GPIO pin C1
//
#define GPIO_PC1_TMS            0x00020403	/**< doc! */

//
// GPIO pin C2
//
#define GPIO_PC2_TDI            0x00020803	/**< doc! */

//
// GPIO pin C3
//
#define GPIO_PC3_TDO            0x00020c03	/**< doc! */

//
// GPIO pin C4
//
#define GPIO_PC4_CCP5           0x00021001	/**< doc! */
#define GPIO_PC4_PHA0           0x00021002	/**< doc! */
#define GPIO_PC4_PWM6           0x00021004	/**< doc! */
#define GPIO_PC4_CCP2           0x00021005	/**< doc! */
#define GPIO_PC4_CCP4           0x00021006	/**< doc! */
#define GPIO_PC4_EPI0S2         0x00021008	/**< doc! */
#define GPIO_PC4_CCP1           0x00021009	/**< doc! */

//
// GPIO pin C5
//
#define GPIO_PC5_CCP1           0x00021401	/**< doc! */
#define GPIO_PC5_C1O            0x00021402	/**< doc! */
#define GPIO_PC5_C0O            0x00021403	/**< doc! */
#define GPIO_PC5_FAULT2         0x00021404	/**< doc! */
#define GPIO_PC5_CCP3           0x00021405	/**< doc! */
#define GPIO_PC5_USB0EPEN       0x00021406	/**< doc! */
#define GPIO_PC5_EPI0S3         0x00021408	/**< doc! */

//
// GPIO pin C6
//
#define GPIO_PC6_CCP3           0x00021801	/**< doc! */
#define GPIO_PC6_PHB0           0x00021802	/**< doc! */
#define GPIO_PC6_C2O            0x00021803	/**< doc! */
#define GPIO_PC6_PWM7           0x00021804	/**< doc! */
#define GPIO_PC6_U1RX           0x00021805	/**< doc! */
#define GPIO_PC6_CCP0           0x00021806	/**< doc! */
#define GPIO_PC6_USB0PFLT       0x00021807	/**< doc! */
#define GPIO_PC6_EPI0S4         0x00021808	/**< doc! */

//
// GPIO pin C7
//
#define GPIO_PC7_CCP4           0x00021c01	/**< doc! */
#define GPIO_PC7_PHB0           0x00021c02	/**< doc! */
#define GPIO_PC7_CCP0           0x00021c04	/**< doc! */
#define GPIO_PC7_U1TX           0x00021c05	/**< doc! */
#define GPIO_PC7_USB0PFLT       0x00021c06	/**< doc! */
#define GPIO_PC7_C1O            0x00021c07	/**< doc! */
#define GPIO_PC7_EPI0S5         0x00021c08	/**< doc! */

//
// GPIO pin D0
//
#define GPIO_PD0_PWM0           0x00030001	/**< doc! */
#define GPIO_PD0_CAN0RX         0x00030002	/**< doc! */
#define GPIO_PD0_IDX0           0x00030003	/**< doc! */
#define GPIO_PD0_U2RX           0x00030004	/**< doc! */
#define GPIO_PD0_U1RX           0x00030005	/**< doc! */
#define GPIO_PD0_CCP6           0x00030006	/**< doc! */
#define GPIO_PD0_I2S0RXSCK      0x00030008	/**< doc! */
#define GPIO_PD0_U1CTS          0x00030009	/**< doc! */

//
// GPIO pin D1
//
#define GPIO_PD1_PWM1           0x00030401	/**< doc! */
#define GPIO_PD1_CAN0TX         0x00030402	/**< doc! */
#define GPIO_PD1_PHA0           0x00030403	/**< doc! */
#define GPIO_PD1_U2TX           0x00030404	/**< doc! */
#define GPIO_PD1_U1TX           0x00030405	/**< doc! */
#define GPIO_PD1_CCP7           0x00030406	/**< doc! */
#define GPIO_PD1_I2S0RXWS       0x00030408	/**< doc! */
#define GPIO_PD1_U1DCD          0x00030409	/**< doc! */
#define GPIO_PD1_CCP2           0x0003040a	/**< doc! */
#define GPIO_PD1_PHB1           0x0003040b	/**< doc! */

//
// GPIO pin D2
//
#define GPIO_PD2_U1RX           0x00030801	/**< doc! */
#define GPIO_PD2_CCP6           0x00030802	/**< doc! */
#define GPIO_PD2_PWM2           0x00030803	/**< doc! */
#define GPIO_PD2_CCP5           0x00030804	/**< doc! */
#define GPIO_PD2_EPI0S20        0x00030808	/**< doc! */

//
// GPIO pin D3
//
#define GPIO_PD3_U1TX           0x00030c01	/**< doc! */
#define GPIO_PD3_CCP7           0x00030c02	/**< doc! */
#define GPIO_PD3_PWM3           0x00030c03	/**< doc! */
#define GPIO_PD3_CCP0           0x00030c04	/**< doc! */
#define GPIO_PD3_EPI0S21        0x00030c08	/**< doc! */

//
// GPIO pin D4
//
#define GPIO_PD4_CCP0           0x00031001	/**< doc! */
#define GPIO_PD4_CCP3           0x00031002	/**< doc! */
#define GPIO_PD4_I2S0RXSD       0x00031008	/**< doc! */
#define GPIO_PD4_U1RI           0x00031009	/**< doc! */
#define GPIO_PD4_EPI0S19        0x0003100a	/**< doc! */

//
// GPIO pin D5
//
#define GPIO_PD5_CCP2           0x00031401	/**< doc! */
#define GPIO_PD5_CCP4           0x00031402	/**< doc! */
#define GPIO_PD5_I2S0RXMCLK     0x00031408	/**< doc! */
#define GPIO_PD5_U2RX           0x00031409	/**< doc! */
#define GPIO_PD5_EPI0S28        0x0003140a	/**< doc! */

//
// GPIO pin D6
//
#define GPIO_PD6_FAULT0         0x00031801	/**< doc! */
#define GPIO_PD6_I2S0TXSCK      0x00031808	/**< doc! */
#define GPIO_PD6_U2TX           0x00031809	/**< doc! */
#define GPIO_PD6_EPI0S29        0x0003180a	/**< doc! */

//
// GPIO pin D7
//
#define GPIO_PD7_IDX0           0x00031c01	/**< doc! */
#define GPIO_PD7_C0O            0x00031c02	/**< doc! */
#define GPIO_PD7_CCP1           0x00031c03	/**< doc! */
#define GPIO_PD7_I2S0TXWS       0x00031c08	/**< doc! */
#define GPIO_PD7_U1DTR          0x00031c09	/**< doc! */
#define GPIO_PD7_EPI0S30        0x00031c0a	/**< doc! */

//
// GPIO pin E0
//
#define GPIO_PE0_PWM4           0x00040001	/**< doc! */
#define GPIO_PE0_SSI1CLK        0x00040002	/**< doc! */
#define GPIO_PE0_CCP3           0x00040003	/**< doc! */
#define GPIO_PE0_EPI0S8         0x00040008	/**< doc! */
#define GPIO_PE0_USB0PFLT       0x00040009	/**< doc! */

//
// GPIO pin E1
//
#define GPIO_PE1_PWM5           0x00040401	/**< doc! */
#define GPIO_PE1_SSI1FSS        0x00040402	/**< doc! */
#define GPIO_PE1_FAULT0         0x00040403	/**< doc! */
#define GPIO_PE1_CCP2           0x00040404	/**< doc! */
#define GPIO_PE1_CCP6           0x00040405	/**< doc! */
#define GPIO_PE1_EPI0S9         0x00040408	/**< doc! */

//
// GPIO pin E2
//
#define GPIO_PE2_CCP4           0x00040801	/**< doc! */
#define GPIO_PE2_SSI1RX         0x00040802	/**< doc! */
#define GPIO_PE2_PHB1           0x00040803	/**< doc! */
#define GPIO_PE2_PHA0           0x00040804	/**< doc! */
#define GPIO_PE2_CCP2           0x00040805	/**< doc! */
#define GPIO_PE2_EPI0S24        0x00040808	/**< doc! */

//
// GPIO pin E3
//
#define GPIO_PE3_CCP1           0x00040c01	/**< doc! */
#define GPIO_PE3_SSI1TX         0x00040c02	/**< doc! */
#define GPIO_PE3_PHA1           0x00040c03	/**< doc! */
#define GPIO_PE3_PHB0           0x00040c04	/**< doc! */
#define GPIO_PE3_CCP7           0x00040c05	/**< doc! */
#define GPIO_PE3_EPI0S25        0x00040c08	/**< doc! */

//
// GPIO pin E4
//
#define GPIO_PE4_CCP3           0x00041001	/**< doc! */
#define GPIO_PE4_FAULT0         0x00041004	/**< doc! */
#define GPIO_PE4_U2TX           0x00041005	/**< doc! */
#define GPIO_PE4_CCP2           0x00041006	/**< doc! */
#define GPIO_PE4_I2S0TXWS       0x00041009	/**< doc! */

//
// GPIO pin E5
//
#define GPIO_PE5_CCP5           0x00041401	/**< doc! */
#define GPIO_PE5_I2S0TXSD       0x00041409	/**< doc! */

//
// GPIO pin E6
//
#define GPIO_PE6_PWM4           0x00041801	/**< doc! */
#define GPIO_PE6_C1O            0x00041802	/**< doc! */
#define GPIO_PE6_U1CTS          0x00041809	/**< doc! */

//
// GPIO pin E7
//
#define GPIO_PE7_PWM5           0x00041c01	/**< doc! */
#define GPIO_PE7_C2O            0x00041c02	/**< doc! */
#define GPIO_PE7_U1DCD          0x00041c09	/**< doc! */

//
// GPIO pin F0
//
#define GPIO_PF0_CAN1RX         0x00050001	/**< doc! */
#define GPIO_PF0_PHB0           0x00050002	/**< doc! */
#define GPIO_PF0_PWM0           0x00050003	/**< doc! */
#define GPIO_PF0_I2S0TXSD       0x00050008	/**< doc! */
#define GPIO_PF0_U1DSR          0x00050009	/**< doc! */

//
// GPIO pin F1
//
#define GPIO_PF1_CAN1TX         0x00050401	/**< doc! */
#define GPIO_PF1_IDX1           0x00050402	/**< doc! */
#define GPIO_PF1_PWM1           0x00050403	/**< doc! */
#define GPIO_PF1_I2S0TXMCLK     0x00050408	/**< doc! */
#define GPIO_PF1_U1RTS          0x00050409	/**< doc! */
#define GPIO_PF1_CCP3           0x0005040a	/**< doc! */

//
// GPIO pin F2
//
#define GPIO_PF2_LED1           0x00050801	/**< doc! */
#define GPIO_PF2_PWM4           0x00050802	/**< doc! */
#define GPIO_PF2_PWM2           0x00050804	/**< doc! */
#define GPIO_PF2_SSI1CLK        0x00050809	/**< doc! */

//
// GPIO pin F3
//
#define GPIO_PF3_LED0           0x00050c01	/**< doc! */
#define GPIO_PF3_PWM5           0x00050c02	/**< doc! */
#define GPIO_PF3_PWM3           0x00050c04	/**< doc! */
#define GPIO_PF3_SSI1FSS        0x00050c09	/**< doc! */

//
// GPIO pin F4
//
#define GPIO_PF4_CCP0           0x00051001	/**< doc! */
#define GPIO_PF4_C0O            0x00051002	/**< doc! */
#define GPIO_PF4_FAULT0         0x00051004	/**< doc! */
#define GPIO_PF4_EPI0S12        0x00051008	/**< doc! */
#define GPIO_PF4_SSI1RX         0x00051009	/**< doc! */

//
// GPIO pin F5
//
#define GPIO_PF5_CCP2           0x00051401	/**< doc! */
#define GPIO_PF5_C1O            0x00051402	/**< doc! */
#define GPIO_PF5_EPI0S15        0x00051408	/**< doc! */
#define GPIO_PF5_SSI1TX         0x00051409	/**< doc! */

//
// GPIO pin F6
//
#define GPIO_PF6_CCP1           0x00051801	/**< doc! */
#define GPIO_PF6_C2O            0x00051802	/**< doc! */
#define GPIO_PF6_PHA0           0x00051804	/**< doc! */
#define GPIO_PF6_I2S0TXMCLK     0x00051809	/**< doc! */
#define GPIO_PF6_U1RTS          0x0005180a	/**< doc! */

//
// GPIO pin F7
//
#define GPIO_PF7_CCP4           0x00051c01	/**< doc! */
#define GPIO_PF7_PHB0           0x00051c04	/**< doc! */
#define GPIO_PF7_EPI0S12        0x00051c08	/**< doc! */
#define GPIO_PF7_FAULT1         0x00051c09	/**< doc! */

//
// GPIO pin G0
//
#define GPIO_PG0_U2RX           0x00060001	/**< doc! */
#define GPIO_PG0_PWM0           0x00060002	/**< doc! */
#define GPIO_PG0_I2C1SCL        0x00060003	/**< doc! */
#define GPIO_PG0_PWM4           0x00060004	/**< doc! */
#define GPIO_PG0_USB0EPEN       0x00060007	/**< doc! */
#define GPIO_PG0_EPI0S13        0x00060008	/**< doc! */

//
// GPIO pin G1
//
#define GPIO_PG1_U2TX           0x00060401	/**< doc! */
#define GPIO_PG1_PWM1           0x00060402	/**< doc! */
#define GPIO_PG1_I2C1SDA        0x00060403	/**< doc! */
#define GPIO_PG1_PWM5           0x00060404	/**< doc! */
#define GPIO_PG1_EPI0S14        0x00060408	/**< doc! */

//
// GPIO pin G2
//
#define GPIO_PG2_PWM0           0x00060801	/**< doc! */
#define GPIO_PG2_FAULT0         0x00060804	/**< doc! */
#define GPIO_PG2_IDX1           0x00060808	/**< doc! */
#define GPIO_PG2_I2S0RXSD       0x00060809	/**< doc! */

//
// GPIO pin G3
//
#define GPIO_PG3_PWM1           0x00060c01	/**< doc! */
#define GPIO_PG3_FAULT2         0x00060c04	/**< doc! */
#define GPIO_PG3_FAULT0         0x00060c08	/**< doc! */
#define GPIO_PG3_I2S0RXMCLK     0x00060c09	/**< doc! */

//
// GPIO pin G4
//
#define GPIO_PG4_CCP3           0x00061001	/**< doc! */
#define GPIO_PG4_FAULT1         0x00061004	/**< doc! */
#define GPIO_PG4_EPI0S15        0x00061008	/**< doc! */
#define GPIO_PG4_PWM6           0x00061009	/**< doc! */
#define GPIO_PG4_U1RI           0x0006100a	/**< doc! */

//
// GPIO pin G5
//
#define GPIO_PG5_CCP5           0x00061401	/**< doc! */
#define GPIO_PG5_IDX0           0x00061404	/**< doc! */
#define GPIO_PG5_FAULT1         0x00061405	/**< doc! */
#define GPIO_PG5_PWM7           0x00061408	/**< doc! */
#define GPIO_PG5_I2S0RXSCK      0x00061409	/**< doc! */
#define GPIO_PG5_U1DTR          0x0006140a	/**< doc! */

//
// GPIO pin G6
//
#define GPIO_PG6_PHA1           0x00061801	/**< doc! */
#define GPIO_PG6_PWM6           0x00061804	/**< doc! */
#define GPIO_PG6_FAULT1         0x00061808	/**< doc! */
#define GPIO_PG6_I2S0RXWS       0x00061809	/**< doc! */
#define GPIO_PG6_U1RI           0x0006180a	/**< doc! */

//
// GPIO pin G7
//
#define GPIO_PG7_PHB1           0x00061c01	/**< doc! */
#define GPIO_PG7_PWM7           0x00061c04	/**< doc! */
#define GPIO_PG7_CCP5           0x00061c08	/**< doc! */
#define GPIO_PG7_EPI0S31        0x00061c09	/**< doc! */

//
// GPIO pin H0
//
#define GPIO_PH0_CCP6           0x00070001	/**< doc! */
#define GPIO_PH0_PWM2           0x00070002	/**< doc! */
#define GPIO_PH0_EPI0S6         0x00070008	/**< doc! */
#define GPIO_PH0_PWM4           0x00070009	/**< doc! */

//
// GPIO pin H1
//
#define GPIO_PH1_CCP7           0x00070401	/**< doc! */
#define GPIO_PH1_PWM3           0x00070402	/**< doc! */
#define GPIO_PH1_EPI0S7         0x00070408	/**< doc! */
#define GPIO_PH1_PWM5           0x00070409	/**< doc! */

//
// GPIO pin H2
//
#define GPIO_PH2_IDX1           0x00070801	/**< doc! */
#define GPIO_PH2_C1O            0x00070802	/**< doc! */
#define GPIO_PH2_FAULT3         0x00070804	/**< doc! */
#define GPIO_PH2_EPI0S1         0x00070808	/**< doc! */

//
// GPIO pin H3
//
#define GPIO_PH3_PHB0           0x00070c01	/**< doc! */
#define GPIO_PH3_FAULT0         0x00070c02	/**< doc! */
#define GPIO_PH3_USB0EPEN       0x00070c04	/**< doc! */
#define GPIO_PH3_EPI0S0         0x00070c08	/**< doc! */

//
// GPIO pin H4
//
#define GPIO_PH4_USB0PFLT       0x00071004	/**< doc! */
#define GPIO_PH4_EPI0S10        0x00071008	/**< doc! */
#define GPIO_PH4_SSI1CLK        0x0007100b	/**< doc! */

//
// GPIO pin H5
//
#define GPIO_PH5_EPI0S11        0x00071408	/**< doc! */
#define GPIO_PH5_FAULT2         0x0007140a	/**< doc! */
#define GPIO_PH5_SSI1FSS        0x0007140b	/**< doc! */

//
// GPIO pin H6
//
#define GPIO_PH6_EPI0S26        0x00071808	/**< doc! */
#define GPIO_PH6_PWM4           0x0007180a	/**< doc! */
#define GPIO_PH6_SSI1RX         0x0007180b	/**< doc! */

//
// GPIO pin H7
//
#define GPIO_PH7_EPI0S27        0x00071c08	/**< doc! */
#define GPIO_PH7_PWM5           0x00071c0a	/**< doc! */
#define GPIO_PH7_SSI1TX         0x00071c0b	/**< doc! */

//
// GPIO pin J0
//
#define GPIO_PJ0_EPI0S16        0x00080008	/**< doc! */
#define GPIO_PJ0_PWM0           0x0008000a	/**< doc! */
#define GPIO_PJ0_I2C1SCL        0x0008000b	/**< doc! */

//
// GPIO pin J1
//
#define GPIO_PJ1_EPI0S17        0x00080408	/**< doc! */
#define GPIO_PJ1_USB0PFLT       0x00080409	/**< doc! */
#define GPIO_PJ1_PWM1           0x0008040a	/**< doc! */
#define GPIO_PJ1_I2C1SDA        0x0008040b	/**< doc! */

//
// GPIO pin J2
//
#define GPIO_PJ2_EPI0S18        0x00080808	/**< doc! */
#define GPIO_PJ2_CCP0           0x00080809	/**< doc! */
#define GPIO_PJ2_FAULT0         0x0008080a	/**< doc! */

//
// GPIO pin J3
//
#define GPIO_PJ3_EPI0S19        0x00080c08	/**< doc! */
#define GPIO_PJ3_U1CTS          0x00080c09	/**< doc! */
#define GPIO_PJ3_CCP6           0x00080c0a	/**< doc! */

//
// GPIO pin J4
//
#define GPIO_PJ4_EPI0S28        0x00081008	/**< doc! */
#define GPIO_PJ4_U1DCD          0x00081009	/**< doc! */
#define GPIO_PJ4_CCP4           0x0008100a	/**< doc! */

//
// GPIO pin J5
//
#define GPIO_PJ5_EPI0S29        0x00081408	/**< doc! */
#define GPIO_PJ5_U1DSR          0x00081409	/**< doc! */
#define GPIO_PJ5_CCP2           0x0008140a	/**< doc! */

//
// GPIO pin J6
//
#define GPIO_PJ6_EPI0S30        0x00081808	/**< doc! */
#define GPIO_PJ6_U1RTS          0x00081809	/**< doc! */
#define GPIO_PJ6_CCP1           0x0008180a	/**< doc! */

//
// GPIO pin J7
//
#define GPIO_PJ7_U1DTR          0x00081c09	/**< doc! */
#define GPIO_PJ7_CCP0           0x00081c0a	/**< doc! */

#ifndef USE_AHB_PERIPHERAL
#define GPIOA               ((GPIO_Type *)GPIO_PORTA_BASE)
#define GPIOB               ((GPIO_Type *)GPIO_PORTB_BASE)
#define GPIOC               ((GPIO_Type *)GPIO_PORTC_BASE)
#define GPIOD               ((GPIO_Type *)GPIO_PORTD_BASE)
#define GPIOE               ((GPIO_Type *)GPIO_PORTE_BASE)
#define GPIOF               ((GPIO_Type *)GPIO_PORTF_BASE)
#define GPIOG               ((GPIO_Type *)GPIO_PORTG_BASE)
#define GPIOH               ((GPIO_Type *)GPIO_PORTH_BASE)
#define GPIOJ               ((GPIO_Type *)GPIO_PORTJ_BASE)
#else
// GPIOA_HS....
#define GPIOA            ((GPIO_Type *)GPIO_PORTA_AHB_BASE)	/**< doc! */
#define GPIOB            ((GPIO_Type *)GPIO_PORTB_AHB_BASE)	/**< doc! */
#define GPIOC            ((GPIO_Type *)GPIO_PORTC_AHB_BASE)	/**< doc! */
#define GPIOD            ((GPIO_Type *)GPIO_PORTD_AHB_BASE)	/**< doc! */
#define GPIOE            ((GPIO_Type *)GPIO_PORTE_AHB_BASE)	/**< doc! */
#define GPIOF            ((GPIO_Type *)GPIO_PORTF_AHB_BASE)	/**< doc! */
#define GPIOG            ((GPIO_Type *)GPIO_PORTG_AHB_BASE)	/**< doc! */
#define GPIOH            ((GPIO_Type *)GPIO_PORTH_AHB_BASE)	/**< doc! */
#define GPIOJ            ((GPIO_Type *)GPIO_PORTJ_AHB_BASE)	/**< doc! */
#endif

#ifdef __cplusplus
}
#endif

#endif //  GPIO_LM3S_H_
/** @} ingroup lm3s_gpio */
