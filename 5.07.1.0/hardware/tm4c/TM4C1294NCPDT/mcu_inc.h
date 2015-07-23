/*
 * mcu_inc.h for LM2S9B90 rev C3
 *
 *  Created on: 11.11.2010
 *      Author: Miroslav Kostadinov
 */

#ifndef MCU_INC_H_
#define MCU_INC_H_

#include <core_map.h>

#define RAM_SIZE 			0x40000		 //!< 256K RAM size
#define FLASH_SIZE 			0x100000 	 //!< 1M FLASH size

#define BASE_FLASH			(BASE_CODE)			//!< 0x00000000 FLASH base address in the alias region


/********************** Peripheral memory map *********************************/
#define BASE_WDT0   		(BASE_PERIPHERAL + 0x00000)		//!< 0x40000000 Watchdog timer 0
#define BASE_WDT1			(BASE_PERIPHERAL + 0x01000)		//!< 0x40001000 Watchdog timer 1
#define BASE_GPIOA          (BASE_PERIPHERAL + 0x04000)		//!< 0x40004000 GPIO Port A
#define BASE_GPIOB          (BASE_PERIPHERAL + 0x05000)		//!< 0x40005000 GPIO Port B
#define BASE_GPIOC          (BASE_PERIPHERAL + 0x06000)		//!< 0x40006000 GPIO Port C
#define BASE_GPIOD          (BASE_PERIPHERAL + 0x07000)		//!< 0x40007000 GPIO Port D
#define BASE_SSI0           (BASE_PERIPHERAL + 0x08000)		//!< 0x40008000 SSI0
#define BASE_SSI1           (BASE_PERIPHERAL + 0x09000)		//!< 0x40009000 SSI1
#define BASE_SSI2           (BASE_PERIPHERAL + 0x0A000)		//!< 0x4000A000 SSI2
#define BASE_SSI3           (BASE_PERIPHERAL + 0x0B000)		//!< 0x4000B000 SSI3
#define BASE_UART0          (BASE_PERIPHERAL + 0x0C000)		//!< 0x4000C000 UART0
#define BASE_UART1          (BASE_PERIPHERAL + 0x0D000)		//!< 0x4000D000 UART1
#define BASE_UART2          (BASE_PERIPHERAL + 0x0E000)		//!< 0x4000E000 UART2
#define BASE_UART3          (BASE_PERIPHERAL + 0x0F000)		//!< 0x4000F000 UART3
#define BASE_UART4          (BASE_PERIPHERAL + 0x10000)		//!< 0x40010000 UART4
#define BASE_UART5          (BASE_PERIPHERAL + 0x11000)		//!< 0x40011000 UART5
#define BASE_UART6          (BASE_PERIPHERAL + 0x12000)		//!< 0x40012000 UART6
#define BASE_UART7          (BASE_PERIPHERAL + 0x13000)		//!< 0x40013000 UART7

#define BASE_I2C0           (BASE_PERIPHERAL + 0x20000)		//!< 0x40020000 I2C0
#define BASE_I2C1           (BASE_PERIPHERAL + 0x21000)		//!< 0x40021000 I2C1
#define BASE_I2C2           (BASE_PERIPHERAL + 0x22000)		//!< 0x40022000 I2C2
#define BASE_I2C3           (BASE_PERIPHERAL + 0x23000)		//!< 0x40023000 I2C3
#define BASE_GPIOE          (BASE_PERIPHERAL + 0x24000)		//!< 0x40024000 GPIO Port E
#define BASE_GPIOF          (BASE_PERIPHERAL + 0x25000)		//!< 0x40025000 GPIO Port F
#define BASE_GPIOG          (BASE_PERIPHERAL + 0x26000)		//!< 0x40026000 GPIO Port G
#define BASE_GPIOH          (BASE_PERIPHERAL + 0x27000)		//!< 0x40027000 GPIO Port H
#define BASE_PWM0           (BASE_PERIPHERAL + 0x28000)		//!< 0x40028000 PWM 0
#define BASE_QEI0           (BASE_PERIPHERAL + 0x2C000)		//!< 0x4002C000 QEI0
#define BASE_TIMER0         (BASE_PERIPHERAL + 0x30000)		//!< 0x40030000 16/32-bit Timer 0
#define BASE_TIMER1         (BASE_PERIPHERAL + 0x31000)		//!< 0x40031000 16/32-bit Timer 1
#define BASE_TIMER2         (BASE_PERIPHERAL + 0x32000)		//!< 0x40032000 16/32-bit Timer 2
#define BASE_TIMER3         (BASE_PERIPHERAL + 0x33000)		//!< 0x40033000 16/32-bit Timer 3
#define BASE_TIMER4         (BASE_PERIPHERAL + 0x34000)		//!< 0x40034000 16/32-bit Timer 4
#define BASE_TIMER5         (BASE_PERIPHERAL + 0x35000)		//!< 0x40035000 16/32-bit Timer 5
#define BASE_ADC0           (BASE_PERIPHERAL + 0x38000)		//!< 0x40038000 ADC0
#define BASE_ADC1           (BASE_PERIPHERAL + 0x39000)		//!< 0x40039000 ADC1
#define BASE_COMP           (BASE_PERIPHERAL + 0x3C000)		//!< 0x4003C000 Analog Comparators
#define BASE_GPIOJ          (BASE_PERIPHERAL + 0x3D000)		//!< 0x4003D000 GPIO Port J

#define BASE_CAN0           (BASE_PERIPHERAL + 0x40000)		//!< 0x40040000 CAN0 Controller
#define BASE_CAN1           (BASE_PERIPHERAL + 0x41000)		//!< 0x40041000 CAN1 Controller
#define BASE_USB0           (BASE_PERIPHERAL + 0x50000)		//!< 0x40050000 USB
#define BASE_AHB_GPIOA      (BASE_PERIPHERAL + 0x58000)		//!< 0x40058000 GPIO Port A (AHB aperture)
#define BASE_AHB_GPIOB      (BASE_PERIPHERAL + 0x59000)		//!< 0x40059000 GPIO Port B (AHB aperture)
#define BASE_AHB_GPIOC      (BASE_PERIPHERAL + 0x5A000)		//!< 0x4005A000 GPIO Port C (AHB aperture)
#define BASE_AHB_GPIOD      (BASE_PERIPHERAL + 0x5B000)		//!< 0x4005B000 GPIO Port D (AHB aperture)
#define BASE_AHB_GPIOE      (BASE_PERIPHERAL + 0x5C000)		//!< 0x4005C000 GPIO Port E (AHB aperture)
#define BASE_AHB_GPIOF      (BASE_PERIPHERAL + 0x5D000)		//!< 0x4005D000 GPIO Port F (AHB aperture)
#define BASE_AHB_GPIOG      (BASE_PERIPHERAL + 0x5E000)		//!< 0x4005E000 GPIO Port G (AHB aperture)
#define BASE_AHB_GPIOH      (BASE_PERIPHERAL + 0x5F000)		//!< 0x4005F000 GPIO Port H (AHB aperture)
#define BASE_AHB_GPIOJ      (BASE_PERIPHERAL + 0x60000)		//!< 0x40060000 GPIO Port J (AHB aperture)
#define BASE_AHB_GPIOK      (BASE_PERIPHERAL + 0x61000)		//!< 0x40061000 GPIO Port K (AHB aperture)
#define BASE_AHB_GPIOL      (BASE_PERIPHERAL + 0x62000)		//!< 0x40062000 GPIO Port L (AHB aperture)
#define BASE_AHB_GPIOM      (BASE_PERIPHERAL + 0x63000)		//!< 0x40063000 GPIO Port M (AHB aperture)
#define BASE_AHB_GPION      (BASE_PERIPHERAL + 0x64000)		//!< 0x40064000 GPIO Port N (AHB aperture)
#define BASE_AHB_GPIOP      (BASE_PERIPHERAL + 0x65000)		//!< 0x40065000 GPIO Port P (AHB aperture)
#define BASE_AHB_GPIOQ      (BASE_PERIPHERAL + 0x66000)		//!< 0x40066000 GPIO Port Q (AHB aperture)
#define BASE_EEPROM         (BASE_PERIPHERAL + 0xAF000)		//!< 0x400AF000 EEPROM and Key Locker
#define BASE_I2C8           (BASE_PERIPHERAL + 0xB8000)		//!< 0x400B8000 I 2 C 8
#define BASE_I2C9           (BASE_PERIPHERAL + 0xB9000)		//!< 0x400B9000 I 2 C 9
#define BASE_I2C4           (BASE_PERIPHERAL + 0xC0000)		//!< 0x400C0000 I 2 C 4
#define BASE_I2C5           (BASE_PERIPHERAL + 0xC1000)		//!< 0x400C1000 I 2 C 5
#define BASE_I2C6           (BASE_PERIPHERAL + 0xC2000)		//!< 0x400C2000 I 2 C 6
#define BASE_I2C7           (BASE_PERIPHERAL + 0xC3000)		//!< 0x400C3000 I 2 C 7
#define BASE_EPI0           (BASE_PERIPHERAL + 0xD0000)		//!< 0x400D0000 EPI 0
#define BASE_TIMER6         (BASE_PERIPHERAL + 0xE0000)		//!< 0x400E0000 16/32-bit Timer 6
#define BASE_TIMER7         (BASE_PERIPHERAL + 0xE1000)		//!< 0x400E1000 16/32-bit Timer 7
#define BASE_EMAC0	        (BASE_PERIPHERAL + 0xEC000)		//!< 0x400EC000 Ethernet Controller
#define BASE_SYSEXC         (BASE_PERIPHERAL + 0xF9000)		//!< 0x400F9000 System Exception Module
#define BASE_HIBERNATE      (BASE_PERIPHERAL + 0xFC000)		//!< 0x400FC000 Hibernation Module
#define BASE_FLASHC         (BASE_PERIPHERAL + 0xFD000)		//!< 0x400FD000 Flash memory control
#define BASE_SYSCTL         (BASE_PERIPHERAL + 0xFE000)		//!< 0x400FE000 System control
#define BASE_UDMA           (BASE_PERIPHERAL + 0xFF000)		//!< 0x400FF000 uDMA

#define BASE_PERIPH44		0x44000000 						//!< Peripheral space 0x44
#define BASE_CRC            (BASE_PERIPH44   + 0x30000)		//!< 0x44030000 CRC Module
#define BASE_EPHY0          (BASE_PERIPH44   + 0x54000)		//!< 0x44054000 EPHY 0



#endif /* MCU_INC_H_ */
