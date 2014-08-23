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





#endif /* MCU_CPP_H_ */
