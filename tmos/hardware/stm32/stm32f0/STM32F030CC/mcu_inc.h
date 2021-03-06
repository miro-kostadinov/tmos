/**************************************************************************//**
 * @file     hardware/stm32/stm32f0/STM32F030CC/mcu_inc.h
 * @ingroup	 hardware_stm32f030cc
 * @brief    STM32F030CC definitions
 * @version  V3.00
 * @date     26. October 2012
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f030cc
 * @ingroup	 hardware_stm32f0
 * Source and definitions for STM32F030CC chip
 *
 *
 ******************************************************************************/

#ifndef MCU_INC_H_
#define MCU_INC_H_

#include <core_map.h>

#define RAM_SIZE 			(32*1024) 	 //!< 32K RAM size
#define FLASH_SIZE 			(256 *1024)  //!< FLASH size
#define FLASH_PAGE_SIZE		0x1000		 //!< 16*4K sectors

#define BASE_FLASH			(BASE_CODE)			//!< 0x00000000 FLASH base address in the alias region
#define BASE_FSMC_R         (BASE_EXT_DEVICE) 	//!< 0xA0000000 FSMC registers base address


/********************** Peripheral memory map *********************************/
#define BASE_APB1PERIPH     (BASE_PERIPHERAL + 0x00000000)	//!< 0x40000000
#define BASE_APB2PERIPH     (BASE_PERIPHERAL + 0x00010000)	//!< 0x40010000
#define BASE_AHB1PERIPH     (BASE_PERIPHERAL + 0x00020000)	//!< 0x40020000
#define BASE_AHB2PERIPH     (BASE_PERIPHERAL + 0x08000000)	//!< 0x48000000

/********************** APB1 peripherals **************************************/

#define BASE_TIM3       	(BASE_APB1PERIPH + 0x0400)		//!< 0x40000400
#define BASE_TIM6       	(BASE_APB1PERIPH + 0x1000)		//!< 0x40001000
#define BASE_TIM7       	(BASE_APB1PERIPH + 0x1400)		//!< 0x40001400
#define BASE_TIM14      	(BASE_APB1PERIPH + 0x2000)		//!< 0x40002000
#define BASE_RTC        	(BASE_APB1PERIPH + 0x2800)		//!< 0x40002800
#define BASE_WWDG       	(BASE_APB1PERIPH + 0x2C00)		//!< 0x40002C00
#define BASE_IWDG       	(BASE_APB1PERIPH + 0x3000)		//!< 0x40003000
#define BASE_SPI2       	(BASE_APB1PERIPH + 0x3800)		//!< 0x40003800
#define BASE_USART2     	(BASE_APB1PERIPH + 0x4400)		//!< 0x40004400
#define BASE_USART3     	(BASE_APB1PERIPH + 0x4800)		//!< 0x40004800
#define BASE_USART4     	(BASE_APB1PERIPH + 0x4C00)		//!< 0x40004C00
#define BASE_USART5     	(BASE_APB1PERIPH + 0x5000)		//!< 0x40005000
#define BASE_I2C1       	(BASE_APB1PERIPH + 0x5400)		//!< 0x40005400
#define BASE_I2C2       	(BASE_APB1PERIPH + 0x5800)		//!< 0x40005800
#define BASE_PWR			(BASE_APB1PERIPH + 0x7000)		//!< 0x40007000

/********************** APB2 peripherals **************************************/

#define BASE_SYSCFG         (BASE_APB2PERIPH + 0x0000)		//!< 0x40010000
#define BASE_EXTI           (BASE_APB2PERIPH + 0x0400)		//!< 0x40010400
#define BASE_USART6     	(BASE_APB2PERIPH + 0x1400)		//!< 0x40011400
#define BASE_ADC            (BASE_APB2PERIPH + 0x2400)		//!< 0x40012400
#define BASE_TIM1           (BASE_APB2PERIPH + 0x2C00)		//!< 0x40012C00
#define BASE_SPI1           (BASE_APB2PERIPH + 0x3000)		//!< 0x40013000
#define BASE_USART1         (BASE_APB2PERIPH + 0x3800)		//!< 0x40013800
#define BASE_TIM15          (BASE_APB2PERIPH + 0x4000)		//!< 0x40014000
#define BASE_TIM16          (BASE_APB2PERIPH + 0x4400)		//!< 0x40014400
#define BASE_TIM17          (BASE_APB2PERIPH + 0x4800)		//!< 0x40014800
#define BASE_DBGMCU		    (BASE_APB2PERIPH + 0x5800)		//!< 0x40015800

/********************** AHB1 peripherals **************************************/
#define BASE_DMA		    (BASE_AHB1PERIPH + 0x0000)		//!< 0x40020000
#define BASE_RCC            (BASE_AHB1PERIPH + 0x1000)      //!< 0x40021000
#define BASE_FLASH_R	    (BASE_AHB1PERIPH + 0x2000)		//!< 0x40022000
#define BASE_CRC            (BASE_AHB1PERIPH + 0x3000)      //!< 0x40023000

/********************** AHB2 peripherals **************************************/
#define BASE_GPIOA 			(BASE_AHB2PERIPH + 0x0000)		//!< 0x48000000
#define BASE_GPIOB 			(BASE_AHB2PERIPH + 0x0400)		//!< 0x48000400
#define BASE_GPIOC 			(BASE_AHB2PERIPH + 0x0800)		//!< 0x48000800
#define BASE_GPIOD 			(BASE_AHB2PERIPH + 0x0C00)		//!< 0x48000C00
#define BASE_GPIOF 			(BASE_AHB2PERIPH + 0x1400)		//!< 0x48001400


#endif /* MCU_INC_H_ */
