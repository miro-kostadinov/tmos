/*
 * mcu_cpp.h
 *
 *  Created on: Aug 23, 2014
 *      Author: miro
 */

#ifndef MCU_CPP_H_
#define MCU_CPP_H_

#include <mcu_inc.h>

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR TM4C1294NCPDT */
/* ************************************************************************** */
/**
 * @ingroup	 TM4C1294NCPDT
 * \addtogroup TM4C_base Peripheral Base Address Definitions
 * @{
 */
#define WDT0   		CAST(WDT_TypeDef	, BASE_WDT0)   		//!< 0x40000000 Watchdog timer 0
#define WDT1		CAST(WDT_TypeDef	, BASE_WDT1)		//!< 0x40001000 Watchdog timer 1
#define GPIOA     	CAST(GPIO_TypeDef	, BASE_GPIOA)       //!< 0x40004000 GPIO Port A
#define GPIOB    	CAST(GPIO_TypeDef	, BASE_GPIOB)       //!< 0x40005000 GPIO Port B
#define GPIOC    	CAST(GPIO_TypeDef	, BASE_GPIOC)       //!< 0x40006000 GPIO Port C
#define GPIOD    	CAST(GPIO_TypeDef	, BASE_GPIOD)       //!< 0x40007000 GPIO Port D
#define SSI0     	CAST(SSI_TypeDef	, BASE_SSI0)        //!< 0x40008000 SSI0
#define SSI1     	CAST(SSI_TypeDef	, BASE_SSI1)        //!< 0x40009000 SSI1
#define SSI2     	CAST(SSI_TypeDef	, BASE_SSI2)        //!< 0x4000A000 SSI2
#define SSI3     	CAST(SSI_TypeDef	, BASE_SSI3)        //!< 0x4000B000 SSI3
#define UART0    	CAST(UART_TypeDef	, BASE_UART0)       //!< 0x4000C000 UART0
#define UART1    	CAST(UART_TypeDef	, BASE_UART1)       //!< 0x4000D000 UART1
#define UART2    	CAST(UART_TypeDef	, BASE_UART2)       //!< 0x4000E000 UART2
#define UART3    	CAST(UART_TypeDef	, BASE_UART3)       //!< 0x4000F000 UART3
#define UART4    	CAST(UART_TypeDef	, BASE_UART4)       //!< 0x40010000 UART4
#define UART5    	CAST(UART_TypeDef	, BASE_UART5)       //!< 0x40011000 UART5
#define UART6    	CAST(UART_TypeDef	, BASE_UART6)       //!< 0x40012000 UART6
#define UART7    	CAST(UART_TypeDef	, BASE_UART7)       //!< 0x40013000 UART7
#define I2C0     	CAST(I2C_TypeDef	, BASE_I2C0)        //!< 0x40020000 I2C0
#define I2C1     	CAST(I2C_TypeDef	, BASE_I2C1)        //!< 0x40021000 I2C1
#define I2C2     	CAST(I2C_TypeDef	, BASE_I2C2)        //!< 0x40022000 I2C2
#define I2C3     	CAST(I2C_TypeDef	, BASE_I2C3)        //!< 0x40023000 I2C3
#define GPIOE    	CAST(GPIO_TypeDef	, BASE_GPIOE)       //!< 0x40024000 GPIO Port E
#define GPIOF    	CAST(GPIO_TypeDef	, BASE_GPIOF)       //!< 0x40025000 GPIO Port F
#define GPIOG    	CAST(GPIO_TypeDef	, BASE_GPIOG)       //!< 0x40026000 GPIO Port G
#define GPIOH    	CAST(GPIO_TypeDef	, BASE_GPIOH)       //!< 0x40027000 GPIO Port H
#define PWM0     	CAST(PWM_TypeDef	, BASE_PWM0)        //!< 0x40028000 PWM 0
#define QEI0     	CAST(QEI_TypeDef	, BASE_QEI0)        //!< 0x4002C000 QEI0
#define TIM0     	CAST(TIM_TypeDef	, BASE_TIMER0)      //!< 0x40030000 16/32-bit Timer 0
#define TIM1     	CAST(TIM_TypeDef	, BASE_TIMER1)      //!< 0x40031000 16/32-bit Timer 1
#define TIM2     	CAST(TIM_TypeDef	, BASE_TIMER2)      //!< 0x40032000 16/32-bit Timer 2
#define TIM3     	CAST(TIM_TypeDef	, BASE_TIMER3)      //!< 0x40033000 16/32-bit Timer 3
#define TIM4     	CAST(TIM_TypeDef	, BASE_TIMER4)      //!< 0x40034000 16/32-bit Timer 4
#define TIM5     	CAST(TIM_TypeDef	, BASE_TIMER5)      //!< 0x40035000 16/32-bit Timer 5
#define ADC0     	CAST(ADC_TypeDef	, BASE_ADC0)        //!< 0x40038000 ADC0
#define ADC1     	CAST(ADC_TypeDef	, BASE_ADC1)        //!< 0x40039000 ADC1
#define COMP     	CAST(COMP_TypeDef	, BASE_COMP)        //!< 0x4003C000 Analog Comparators
#define GPIOJ    	CAST(GPIO_TypeDef	, BASE_GPIOJ)       //!< 0x4003D000 GPIO Port J
#define CAN0     	CAST(CAN_TypeDef	, BASE_CAN0)        //!< 0x40040000 CAN0 Controller
#define CAN1     	CAST(CAN_TypeDef	, BASE_CAN1)        //!< 0x40041000 CAN1 Controller
#define USB      	CAST(USB_TypeDef	, BASE_USB0)        //!< 0x40050000 USB
#define AHB_GPIOA 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOA)   //!< 0x40058000 GPIO Port A (AHB aperture)
#define AHB_GPIOB 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOB)   //!< 0x40059000 GPIO Port B (AHB aperture)
#define AHB_GPIOC 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOC)   //!< 0x4005A000 GPIO Port C (AHB aperture)
#define AHB_GPIOD 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOD)   //!< 0x4005B000 GPIO Port D (AHB aperture)
#define AHB_GPIOE 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOE)   //!< 0x4005C000 GPIO Port E (AHB aperture)
#define AHB_GPIOF 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOF)   //!< 0x4005D000 GPIO Port F (AHB aperture)
#define AHB_GPIOG 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOG)   //!< 0x4005E000 GPIO Port G (AHB aperture)
#define AHB_GPIOH 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOH)   //!< 0x4005F000 GPIO Port H (AHB aperture)
#define AHB_GPIOJ 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOJ)   //!< 0x40060000 GPIO Port J (AHB aperture)
#define AHB_GPIOK 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOK)   //!< 0x40061000 GPIO Port K (AHB aperture)
#define AHB_GPIOL 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOL)   //!< 0x40062000 GPIO Port L (AHB aperture)
#define AHB_GPIOM 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOM)   //!< 0x40063000 GPIO Port M (AHB aperture)
#define AHB_GPION 	CAST(GPIO_TypeDef	, BASE_AHB_GPION)   //!< 0x40064000 GPIO Port N (AHB aperture)
#define AHB_GPIOP 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOP)   //!< 0x40065000 GPIO Port P (AHB aperture)
#define AHB_GPIOQ 	CAST(GPIO_TypeDef	, BASE_AHB_GPIOQ)   //!< 0x40066000 GPIO Port Q (AHB aperture)
#define I2C8      	CAST(I2C_TypeDef	, BASE_I2C8)        //!< 0x400B8000 I 2 C 8
#define I2C9      	CAST(I2C_TypeDef	, BASE_I2C9)        //!< 0x400B9000 I 2 C 9
#define I2C4      	CAST(I2C_TypeDef	, BASE_I2C4)        //!< 0x400C0000 I 2 C 4
#define I2C5      	CAST(I2C_TypeDef	, BASE_I2C5)        //!< 0x400C1000 I 2 C 5
#define I2C6      	CAST(I2C_TypeDef	, BASE_I2C6)        //!< 0x400C2000 I 2 C 6
#define I2C7      	CAST(I2C_TypeDef	, BASE_I2C7)        //!< 0x400C3000 I 2 C 7
#define EPI0      	CAST(EPI_TypeDef	, BASE_EPI0)        //!< 0x400D0000 EPI 0
#define TIM6      	CAST(TIM_TypeDef	, BASE_TIMER6)      //!< 0x400E0000 16/32-bit Timer 6
#define TIM7      	CAST(TIM_TypeDef	, BASE_TIMER7)      //!< 0x400E1000 16/32-bit Timer 7
#define EMAC	  	CAST(EMAC_TypeDef	, BASE_EMAC0)       //!< 0x400EC000 Ethernet Controller
#define SYSEXC    	CAST(SYSEXC_TypeDef , BASE_SYSEXC)      //!< 0x400F9000 System Exception Module
#define HIBERNATE  	CAST(HIBERNATE_TypeDef, BASE_HIBERNATE) //!< 0x400FC000 Hibernation Module
#define FLASHC     	CAST(FLASHC_TypeDef , BASE_FLASHC)      //!< 0x400FD000 Flash memory control
#define SYSCTL    	CAST(SYSCTL_TypeDef , BASE_SYSCTL)      //!< 0x400FE000 System control
#define UDMA      	CAST(UDMA_TypeDef	, BASE_UDMA)        //!< 0x400FF000 uDMA
#define CRC       	CAST(CRC_TypeDef	, BASE_CRC)         //!< 0x44030000 CRC Module
#define EPHY0     	CAST(EPHY_TypeDef	, BASE_EPHY0)       //!< 0x44054000 EPHY 0
/** @} */


/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR STM32F2x */
/* ************************************************************************** */
/**
 * @ingroup	 TM4C1294NCPDT
 * \addtogroup TM4C1294NCPDT_id Peripheral Ids Definitions
 * @{
 */

// ------------------- index 0 ->   --------------------------------------------
// (sysctl:0x300) Watchdog Timer Peripheral Present
// (sysctl:0x500) Watchdog Timer Software Reset
// (sysctl:0x600) Watchdog Timer Run Mode Clock Gating Control
// (sysctl:0x700) Watchdog Timer Sleep Mode Clock Gating Control
// (sysctl:0x800) Watchdog Timer Deep-Sleep Mode Clock Gating Control
// (sysctl:0x900) Watchdog Timer Power Control
// (sysctl:0xA00) Watchdog Timer Peripheral Ready
#define  ID_PERIPH_WDOG1       ((0x00<<6)| 1) //!< Watchdog 1
#define  ID_PERIPH_WDOG0       ((0x00<<6)| 0) //!< Watchdog 0

// ------------------- index 4 ->   --------------------------------------------
// (sysctl:0x304) 16/32-Bit General-Purpose Timer Peripheral Present
// (sysctl:0x504) 16/32-Bit General-Purpose Timer Software Reset
// (sysctl:0x604) 16/32-Bit General-Purpose Timer Run Mode Clock Gating  Control
// (sysctl:0x704) 16/32-Bit General-Purpose Timer Sleep Mode Clock Gating Control
// (sysctl:0x804) 16/32-Bit General-Purpose Timer Deep-Sleep Mode Clock Gating Control
// (sysctl:0x904) 16/32-Bit General-Purpose Timer Power Control
// (sysctl:0xA04) 16/32-Bit General-Purpose Timer Peripheral Ready
#define  ID_PERIPH_TIMER7      ((0x04<<6)| 7) //!< Timer 3
#define  ID_PERIPH_TIMER6      ((0x04<<6)| 6) //!< Timer 3
#define  ID_PERIPH_TIMER5      ((0x04<<6)| 5) //!< Timer 3
#define  ID_PERIPH_TIMER4      ((0x04<<6)| 4) //!< Timer 3
#define  ID_PERIPH_TIMER3      ((0x04<<6)| 3) //!< Timer 3
#define  ID_PERIPH_TIMER2      ((0x04<<6)| 2) //!< Timer 2
#define  ID_PERIPH_TIMER1      ((0x04<<6)| 1) //!< Timer 1
#define  ID_PERIPH_TIMER0      ((0x04<<6)| 0) //!< Timer 0

// ------------------- index 8 ->   --------------------------------------------
// (sysctl:0x308) General-Purpose Input/Output Peripheral Present
// (sysctl:0x508) General-Purpose Input/Output Software Reset
// (sysctl:0x608) General-Purpose Input/Output Run Mode Clock Gating Control
// (sysctl:0x708) General-Purpose Input/Output Sleep Mode Clock Gating Control
// (sysctl:0x808) General-Purpose Input/Output Deep-Sleep Mode Clock Gating Control
// (sysctl:0x908) General-Purpose Input/Output Power Control
// (sysctl:0xA08) General-Purpose Input/Output Peripheral Ready
#define  ID_PERIPH_GPIOQ       ((0x08<<6)|14) //!< GPIO Q
#define  ID_PERIPH_GPIOP       ((0x08<<6)|13) //!< GPIO P
#define  ID_PERIPH_GPION       ((0x08<<6)|12) //!< GPIO N
#define  ID_PERIPH_GPIOM       ((0x08<<6)|11) //!< GPIO M
#define  ID_PERIPH_GPIOL       ((0x08<<6)|10) //!< GPIO L
#define  ID_PERIPH_GPIOK       ((0x08<<6)| 9) //!< GPIO K
#define  ID_PERIPH_GPIOJ       ((0x08<<6)| 8) //!< GPIO J
#define  ID_PERIPH_GPIOH       ((0x08<<6)| 7) //!< GPIO H
#define  ID_PERIPH_GPIOG       ((0x08<<6)| 6) //!< GPIO G
#define  ID_PERIPH_GPIOF       ((0x08<<6)| 5) //!< GPIO F
#define  ID_PERIPH_GPIOE       ((0x08<<6)| 4) //!< GPIO E
#define  ID_PERIPH_GPIOD       ((0x08<<6)| 3) //!< GPIO D
#define  ID_PERIPH_GPIOC       ((0x08<<6)| 2) //!< GPIO C
#define  ID_PERIPH_GPIOB       ((0x08<<6)| 1) //!< GPIO B
#define  ID_PERIPH_GPIOA       ((0x08<<6)| 0) //!< GPIO A

// ------------------- index C ->   --------------------------------------------
// (sysctl:0x30C) Micro Direct Memory Access Peripheral Present
// (sysctl:0x50C) Micro Direct Memory Access Software Reset
// (sysctl:0x60C) Micro Direct Memory Access Run Mode Clock Gating  Control
// (sysctl:0x70C) Micro Direct Memory Access Sleep Mode Clock Gating Control
// (sysctl:0x80C) Micro Direct Memory Access Deep-Sleep Mode Clock Gating Control
// (sysctl:0x90C) Micro Direct Memory Access Power Control
// (sysctl:0xA0C) Micro Direct Memory Access Peripheral Ready
#define  ID_PERIPH_UDMA        ((0x0C<<6)| 0) //!< uDMA

// ------------------- index 10 ->   --------------------------------------------
// (sysctl:0x310) EPI Peripheral Present
// (sysctl:0x510) EPI Software Reset
// (sysctl:0x610) EPI Run Mode Clock Gating Control
// (sysctl:0x710) EPI Sleep Mode Clock Gating Control
// (sysctl:0x810) EPI Deep-Sleep Mode Clock Gating Control
// (sysctl:0x910) External Peripheral Interface Power Control
// (sysctl:0xA10) EPI Peripheral Ready
#define  ID_PERIPH_EPI0        ((0x10<<6)| 0) //!< EPI0

// ------------------- index 14 ->   --------------------------------------------
// (sysctl:0x314) Hibernation Peripheral Present
// (sysctl:0x514) Hibernation Software Reset
// (sysctl:0x614) Hibernation Run Mode Clock Gating Control
// (sysctl:0x714) Hibernation Sleep Mode Clock Gating Control
// (sysctl:0x814) Hibernation Deep-Sleep Mode Clock Gating Control
// (sysctl:0x914) Hibernation Power Control
// (sysctl:0xA14) Hibernation Peripheral Ready
#define  ID_PERIPH_HIBERNATE   ((0x14<<6)| 0) //!< Hibernation module

// ------------------- index 18 ->   --------------------------------------------
// (sysctl:0x318) Universal Asynchronous Receiver/Transmitter Peripheral Present
// (sysctl:0x518) Universal Asynchronous Receiver/Transmitter Software Reset
// (sysctl:0x618) Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating Control
// (sysctl:0x718) Universal Asynchronous Receiver/Transmitter Sleep Mode Clock Gating Control
// (sysctl:0x818) Universal Asynchronous Receiver/Transmitter Deep-Sleep Mode Clock Gating Control
// (sysctl:0x918) Universal Asynchronous Receiver/Transmitter Power Control
// (sysctl:0xA18) Universal Asynchronous Receiver/Transmitter Peripheral Ready
#define  ID_PERIPH_UART7       ((0x18<<6)| 7) //!< UART 7
#define  ID_PERIPH_UART6       ((0x18<<6)| 6) //!< UART 6
#define  ID_PERIPH_UART5       ((0x18<<6)| 5) //!< UART 5
#define  ID_PERIPH_UART4       ((0x18<<6)| 4) //!< UART 4
#define  ID_PERIPH_UART3       ((0x18<<6)| 3) //!< UART 3
#define  ID_PERIPH_UART2       ((0x18<<6)| 2) //!< UART 2
#define  ID_PERIPH_UART1       ((0x18<<6)| 1) //!< UART 1
#define  ID_PERIPH_UART0       ((0x18<<6)| 0) //!< UART 0

// ------------------- index 1C ->   --------------------------------------------
// (sysctl:0x31C) Synchronous Serial Interface Peripheral Present
// (sysctl:0x51C) Synchronous Serial Interface Software Reset
// (sysctl:0x61C) Synchronous Serial Interface Run Mode Clock Gating Control
// (sysctl:0x71C) Synchronous Serial Interface Sleep Mode Clock Gating Control
// (sysctl:0x81C) Synchronous Serial Interface Deep-Sleep Mode Clock Gating Control
// (sysctl:0x91C) Synchronous Serial Interface Power Control
// (sysctl:0xA1C) Synchronous Serial Interface Peripheral Ready
#define  ID_PERIPH_SSI3        ((0x1C<<6)| 3) //!< SSI 3
#define  ID_PERIPH_SSI2        ((0x1C<<6)| 2) //!< SSI 2
#define  ID_PERIPH_SSI1        ((0x1C<<6)| 1) //!< SSI 1
#define  ID_PERIPH_SSI0        ((0x1C<<6)| 0) //!< SSI 0

// ------------------- index 20 ->   --------------------------------------------
// (sysctl:0x320) Inter-Integrated Circuit Peripheral Present
// (sysctl:0x520) Inter-Integrated Circuit Software Reset
// (sysctl:0x620) Inter-Integrated Circuit Run Mode Clock Gating Control
// (sysctl:0x720) Inter-Integrated Circuit Sleep Mode Clock Gating Control
// (sysctl:0x820) Inter-Integrated Circuit Deep-Sleep Mode Clock Gating Control
// (sysctl:0x920) Inter-Integrated Circuit Power Control
// (sysctl:0xA20) Inter-Integrated Circuit Peripheral Ready
#define  ID_PERIPH_I2C9        ((0x20<<6)| 9) //!< I2C 9
#define  ID_PERIPH_I2C8        ((0x20<<6)| 8) //!< I2C 8
#define  ID_PERIPH_I2C7        ((0x20<<6)| 7) //!< I2C 7
#define  ID_PERIPH_I2C6        ((0x20<<6)| 6) //!< I2C 6
#define  ID_PERIPH_I2C5        ((0x20<<6)| 5) //!< I2C 5
#define  ID_PERIPH_I2C4        ((0x20<<6)| 4) //!< I2C 4
#define  ID_PERIPH_I2C3        ((0x20<<6)| 3) //!< I2C 3
#define  ID_PERIPH_I2C2        ((0x20<<6)| 2) //!< I2C 2
#define  ID_PERIPH_I2C1        ((0x20<<6)| 1) //!< I2C 1
#define  ID_PERIPH_I2C0        ((0x20<<6)| 0) //!< I2C 0

// ------------------- index 28 ->   --------------------------------------------
// (sysctl:0x328) Universal Serial Bus Peripheral Present
// (sysctl:0x528) Universal Serial Bus Software Reset
// (sysctl:0x628) Universal Serial Bus Run Mode Clock Gating Control
// (sysctl:0x728) Universal Serial Bus Sleep Mode Clock Gating Control
// (sysctl:0x828) Universal Serial Bus Deep-Sleep Mode Clock Gating Control
// (sysctl:0x928) Universal Serial Bus Power Control
// (sysctl:0xA28) Universal Serial Bus Peripheral Ready
#define  ID_PERIPH_USB0        ((0x28<<6)| 0) //!< USB0

// ------------------- index 30 ->   --------------------------------------------
// (sysctl:0x330) Ethernet PHY Peripheral Present
#define  ID_PERIPH_EPHY0       ((0x30<<6)| 0) //!< ETH

// ------------------- index 34 ->   --------------------------------------------
// (sysctl:0x334) Controller Area Network Peripheral Present
// (sysctl:0x534) Controller Area Network Software Reset
// (sysctl:0x634) Controller Area Network Run Mode Clock Gating Control
// (sysctl:0x734) Controller Area Network Sleep Mode Clock Gating Control
// (sysctl:0x834) Controller Area Network Deep-Sleep Mode Clock Gating Control
// (sysctl:0x934) Controller Area Network Power Control
// (sysctl:0xA34) Controller Area Network Peripheral Ready
#define  ID_PERIPH_CAN1        ((0x34<<6)| 1) //!< CAN 1
#define  ID_PERIPH_CAN0        ((0x34<<6)| 0) //!< CAN 0

// ------------------- index 38 ->   --------------------------------------------
// (sysctl:0x338) Analog-to-Digital Converter Peripheral Present
// (sysctl:0x538) Analog-to-Digital Converter Software Reset
// (sysctl:0x638) Analog-to-Digital Converter Run Mode Clock Gating Control
// (sysctl:0x738) Analog-to-Digital Converter Sleep Mode Clock Gating Control
// (sysctl:0x838) Analog-to-Digital Converter Deep-Sleep Mode Clock Gating Control
// (sysctl:0x938) Analog-to-Digital Converter Power Control
// (sysctl:0xA38) Analog-to-Digital Converter Peripheral Ready
#define  ID_PERIPH_ADC1        ((0x38<<6)| 1) //!< ADC1
#define  ID_PERIPH_ADC0        ((0x38<<6)| 0) //!< ADC0

// ------------------- index 3C ->   --------------------------------------------
// (sysctl:0x33C) Analog Comparator Peripheral Present
// (sysctl:0x53C) Analog Comparator Software Reset
// (sysctl:0x63C) Analog Comparator Run Mode Clock Gating Control
// (sysctl:0x73C) Analog Comparator Sleep Mode Clock Gating Control
// (sysctl:0x83C) Analog Comparator Deep-Sleep Mode Clock Gating Control
// (sysctl:0x93C) Analog Comparator Power Control
// (sysctl:0xA3C) Analog Comparator Peripheral Ready
#define  ID_PERIPH_COMP0       ((0x3C<<6)| 0) //!< Analog comparator 0

// ------------------- index 40 ->   --------------------------------------------
// (sysctl:0x340) Pulse Width Modulator Peripheral Present
// (sysctl:0x540) Pulse Width Modulator Software Reset
// (sysctl:0x640) Pulse Width Modulator Run Mode Clock Gating Control
// (sysctl:0x740) Pulse Width Modulator Sleep Mode Clock Gating Control
// (sysctl:0x840) Pulse Width Modulator Deep-Sleep Mode Clock Gating Control
// (sysctl:0x940) Pulse Width Modulator Power Control
// (sysctl:0xA40) Pulse Width Modulator Peripheral Ready
#define  ID_PERIPH_PWM0        ((0x40<<6)| 0) //!< PWM

// ------------------- index 44 ->   --------------------------------------------
// (sysctl:0x344) Quadrature Encoder Interface Peripheral Present
// (sysctl:0x544) Quadrature Encoder Interface Software Reset
// (sysctl:0x644) Quadrature Encoder Interface Run Mode Clock Gating Control
// (sysctl:0x744) Quadrature Encoder Interface Sleep Mode Clock Gating Control
// (sysctl:0x844) Quadrature Encoder Interface Deep-Sleep Mode Clock Gating Control
// (sysctl:0x944) Quadrature Encoder Interface Power Control
// (sysctl:0xA44) Quadrature Encoder Interface Peripheral Ready
#define  ID_PERIPH_QEI0        ((0x44<<6)| 0) //!< QEI 0

// ------------------- index 48 ->   --------------------------------------------
// (sysctl:0x348) Low Pin Count Interface Peripheral Present

// ------------------- index 50 ->   --------------------------------------------
// (sysctl:0x350) Platform Environment Control Interface Peripheral Present

// ------------------- index 54 ->   --------------------------------------------
// (sysctl:0x354) Fan Control Peripheral Present

// ------------------- index 58 ->   --------------------------------------------
// (sysctl:0x358) EEPROM Peripheral Present
// (sysctl:0x558) EEPROM Software Reset
// (sysctl:0x658) EEPROM Run Mode Clock Gating Control
// (sysctl:0x758) EEPROM Sleep Mode Clock Gating Control
// (sysctl:0x858) EEPROM Deep-Sleep Mode Clock Gating Control
// (sysctl:0x958) EEPROM Power Control
// (sysctl:0xA58) EEPROM Peripheral Ready

// ------------------- index 5C ->   --------------------------------------------
// (sysctl:0x35C) 32/64-Bit Wide General-Purpose Timer Peripheral  Present

// ------------------- index 70 ->   --------------------------------------------
// (sysctl:0x370) Remote Temperature Sensor Peripheral Present

// ------------------- index 74 ->   --------------------------------------------
// (sysctl:0x374) CRC and Cryptographic Modules Peripheral Present
// (sysctl:0x574) CRC and Cryptographic Modules Software Reset
// (sysctl:0x674) CRC and Cryptographic Modules Run Mode Clock Gating Control
// (sysctl:0x774) CRC and Cryptographic Modules Sleep Mode Clock Gating Control
// (sysctl:0x874) CRC and Cryptographic Modules Deep-Sleep Mode Clock Gating Control
// (sysctl:0x974) CRC and Cryptographic Modules Power Control
// (sysctl:0xA74) CRC and Cryptographic Modules Peripheral Ready

// ------------------- index 90 ->   --------------------------------------------
// (sysctl:0x390) LCD Peripheral Present

// ------------------- index 98 ->   --------------------------------------------
// (sysctl:0x398) 1-Wire Peripheral Present

// ------------------- index 9C ->   --------------------------------------------
// (sysctl:0x39C) Ethernet MAC Peripheral Present
// (sysctl:0x59C) Ethernet MAC Software Reset
// (sysctl:0x69C) Ethernet MAC Run Mode Clock Gating Control
// (sysctl:0x79C) Ethernet MAC Sleep Mode Clock Gating Control
// (sysctl:0x89C) Ethernet MAC Deep-Sleep Mode Clock Gating Control
// (sysctl:0x99C) Ethernet MAC Power Control
// (sysctl:0xA9C) Ethernet MAC Peripheral Ready

// ------------------- index A0 ->   --------------------------------------------
// (sysctl:0x3A0) Power Regulator Bus Peripheral Present

// ------------------- index A4 ->   --------------------------------------------
// (sysctl:0x3A4) Human Interface Master Peripheral Present




#define ID_NO_PERIPH 		(0xFFFF)		//!< invalid index for peripheral
/** @}*/



#endif /* MCU_CPP_H_ */
