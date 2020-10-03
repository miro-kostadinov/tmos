/**************************************************************************//**
 * @file     hardware/stm32/stm32f2/STM32F205ZC/mcu_inc.h
 * @ingroup	 hardware_stm32f205zc
 * @brief    STM32F205ZC definitions
 * @version  V3.00
 * @date     26. October 2012
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h743zi
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H743ZI chip
 *
 *
 ******************************************************************************/

#ifndef MCU_INC_H_
#define MCU_INC_H_

#include <core_map.h>

//#define RAM_SIZE 			(112*1024) 	 //!< 112K RAM size
//#define SRAM1_SIZE 			(96*1024) 	 //!< 96K SRAM1 size
//#define SRAM2_SIZE 			(16*1024) 	 //!< 16K SRAM2 size
//#define FLASH_SIZE 			((*(unsigned short*)0x1FFF7A22) *1024) 	 //!< FLASH size see Device Electronic signature
//#define FLASH_PAGE_SIZE		0x4000		 //!< 4*16K sectors, 1*64K, 1(-7)*128K
//
//#define BASE_FLASH			(BASE_CODE)			//!< 0x00000000 FLASH base address in the alias region
//#define BASE_FSMC_R         (BASE_EXT_DEVICE) 	//!< 0xA0000000 FSMC registers base address


/********************** Peripheral memory map *********************************/
#define BASE_APB1PERIPH     (BASE_PERIPHERAL + 0x00000000)	//!< 0x40000000
#define BASE_APB2PERIPH     (BASE_PERIPHERAL + 0x00010000)	//!< 0x40010000
#define BASE_AHB1PERIPH     (BASE_PERIPHERAL + 0x00020000)	//!< 0x40020000
#define BASE_AHB2PERIPH     (BASE_PERIPHERAL + 0x10000000)	//!< 0x50000000

/********************** APB1 peripherals **************************************/
#define BASE_TIM2   		(BASE_APB1PERIPH + 0x0000)		//!< 0x40000000
#define BASE_TIM3			(BASE_APB1PERIPH + 0x0400)		//!< 0x40000400
#define BASE_TIM4           (BASE_APB1PERIPH + 0x0800)		//!< 0x40000800
#define BASE_TIM5           (BASE_APB1PERIPH + 0x0C00)		//!< 0x40000C00
#define BASE_TIM6           (BASE_APB1PERIPH + 0x1000)		//!< 0x40001000
#define BASE_TIM7           (BASE_APB1PERIPH + 0x1400)		//!< 0x40001400
#define BASE_TIM12          (BASE_APB1PERIPH + 0x1800)		//!< 0x40001800
#define BASE_TIM13          (BASE_APB1PERIPH + 0x1C00)		//!< 0x40001C00
#define BASE_TIM14          (BASE_APB1PERIPH + 0x2000)		//!< 0x40002000
#define BASE_RTC            (BASE_APB1PERIPH + 0x2800)		//!< 0x40002800
#define BASE_WWDG           (BASE_APB1PERIPH + 0x2C00)		//!< 0x40002C00
#define BASE_IWDG           (BASE_APB1PERIPH + 0x3000)		//!< 0x40003000
#define BASE_SPI2           (BASE_APB1PERIPH + 0x3800)		//!< 0x40003800
#define BASE_SPI3           (BASE_APB1PERIPH + 0x3C00)		//!< 0x40003C00
#define BASE_USART2         (BASE_APB1PERIPH + 0x4400)		//!< 0x40004400
#define BASE_USART3         (BASE_APB1PERIPH + 0x4800)		//!< 0x40004800
#define BASE_UART4          (BASE_APB1PERIPH + 0x4C00)		//!< 0x40004C00
#define BASE_UART5          (BASE_APB1PERIPH + 0x5000)		//!< 0x40005000
#define BASE_I2C1           (BASE_APB1PERIPH + 0x5400)		//!< 0x40005400
#define BASE_I2C2           (BASE_APB1PERIPH + 0x5800)		//!< 0x40005800
#define BASE_I2C3           (BASE_APB1PERIPH + 0x5C00)		//!< 0x40005C00
#define BASE_CAN1           (BASE_APB1PERIPH + 0x6400)		//!< 0x40006400
#define BASE_CAN2           (BASE_APB1PERIPH + 0x6800)		//!< 0x40006800
#define BASE_PWR            (BASE_APB1PERIPH + 0x7000)		//!< 0x40007000
#define BASE_DAC            (BASE_APB1PERIPH + 0x7400)		//!< 0x40007400

/********************** APB2 peripherals **************************************/
#define BASE_TIM1           (BASE_APB2PERIPH + 0x0000)		//!< 0x40010000
#define BASE_TIM8			(BASE_APB2PERIPH + 0x0400)		//!< 0x40010400
#define BASE_USART1         (BASE_APB2PERIPH + 0x1000)		//!< 0x40011000
#define BASE_USART6         (BASE_APB2PERIPH + 0x1400)		//!< 0x40011400
#define BASE_ADC1           (BASE_APB2PERIPH + 0x2000)		//!< 0x40012000
#define BASE_ADC2           (BASE_APB2PERIPH + 0x2100)		//!< 0x40012100
#define BASE_ADC3           (BASE_APB2PERIPH + 0x2200)		//!< 0x40012200
#define BASE_ADC            (BASE_APB2PERIPH + 0x2300)		//!< 0x40012300
#define BASE_SDIO           (BASE_APB2PERIPH + 0x2C00)		//!< 0x40012C00
#define BASE_SPI1           (BASE_APB2PERIPH + 0x3000)		//!< 0x40013000
#define BASE_SYSCFG         (BASE_APB2PERIPH + 0x3800)		//!< 0x40013800
#define BASE_EXTI           (BASE_APB2PERIPH + 0x3C00)		//!< 0x40013C00
#define BASE_TIM9           (BASE_APB2PERIPH + 0x4000)		//!< 0x40014000
#define BASE_TIM10          (BASE_APB2PERIPH + 0x4400)		//!< 0x40014400
#define BASE_TIM11          (BASE_APB2PERIPH + 0x4800)		//!< 0x40014800

/********************** AHB1 peripherals **************************************/
#define BASE_GPIOA       	(BASE_AHB1PERIPH + 0x0000)		//!< 0x40020000
#define BASE_GPIOB          (BASE_AHB1PERIPH + 0x0400)		//!< 0x40020400
#define BASE_GPIOC          (BASE_AHB1PERIPH + 0x0800)		//!< 0x40020800
#define BASE_GPIOD          (BASE_AHB1PERIPH + 0x0C00)		//!< 0x40020C00
#define BASE_GPIOE          (BASE_AHB1PERIPH + 0x1000)		//!< 0x40021000
#define BASE_GPIOF          (BASE_AHB1PERIPH + 0x1400)		//!< 0x40021400
#define BASE_GPIOG          (BASE_AHB1PERIPH + 0x1800)		//!< 0x40021800
#define BASE_GPIOH          (BASE_AHB1PERIPH + 0x1C00)		//!< 0x40021C00
#define BASE_GPIOI          (BASE_AHB1PERIPH + 0x2000)		//!< 0x40022000
#define BASE_CRC            (BASE_AHB1PERIPH + 0x3000)		//!< 0x40023000
#define BASE_RCC            (BASE_AHB1PERIPH + 0x3800)		//!< 0x40023800
#define BASE_FLASH_R        (BASE_AHB1PERIPH + 0x3C00)		//!< 0x40023C00
#define BASE_BKPSRAM        (BASE_AHB1PERIPH + 0x4000)		//!< 0x40024000
#define BASE_DMA1           (BASE_AHB1PERIPH + 0x6000)		//!< 0x40026000
#define BASE_DMA1_Stream0   (BASE_DMA1 		 + 0x0010)		//!< 0x40026010
#define BASE_DMA1_Stream1   (BASE_DMA1       + 0x0028)		//!< 0x40026028
#define BASE_DMA1_Stream2   (BASE_DMA1       + 0x0040)		//!< 0x40026040
#define BASE_DMA1_Stream3   (BASE_DMA1       + 0x0058)		//!< 0x40026058
#define BASE_DMA1_Stream4   (BASE_DMA1       + 0x0070)		//!< 0x40026070
#define BASE_DMA1_Stream5   (BASE_DMA1       + 0x0088)		//!< 0x40026088
#define BASE_DMA1_Stream6   (BASE_DMA1       + 0x00A0)		//!< 0x400260A0
#define BASE_DMA1_Stream7   (BASE_DMA1       + 0x00B8)		//!< 0x400260B8
#define BASE_DMA2	   		(BASE_AHB1PERIPH + 0x6400)		//!< 0x40026400
#define BASE_DMA2_Stream0   (BASE_DMA2       + 0x0010)		//!< 0x40026410
#define BASE_DMA2_Stream1   (BASE_DMA2       + 0x0028)		//!< 0x40026428
#define BASE_DMA2_Stream2   (BASE_DMA2       + 0x0040)		//!< 0x40026440
#define BASE_DMA2_Stream3   (BASE_DMA2       + 0x0058)		//!< 0x40026458
#define BASE_DMA2_Stream4   (BASE_DMA2       + 0x0070)		//!< 0x40026470
#define BASE_DMA2_Stream5   (BASE_DMA2       + 0x0088)		//!< 0x40026488
#define BASE_DMA2_Stream6   (BASE_DMA2       + 0x00A0)		//!< 0x400264A0
#define BASE_DMA2_Stream7   (BASE_DMA2       + 0x00B8)		//!< 0x400264B8
#define BASE_ETH            (BASE_AHB1PERIPH + 0x8000)		//!< 0x40028000
#define BASE_ETH_MAC        (BASE_ETH        + 0x0000)		//!< 0x40028000
#define BASE_ETH_MMC        (BASE_ETH        + 0x0100)		//!< 0x40028100
#define BASE_ETH_PTP        (BASE_ETH        + 0x0700)		//!< 0x40028700
#define BASE_ETH_DMA        (BASE_ETH        + 0x1000)		//!< 0x40029000
#define BASE_OTG_HS			(BASE_AHB1PERIPH + 0x20000)		//!< 0x40040000

/********************** AHB2 peripherals **************************************/
#define BASE_OTG_FS			(BASE_AHB2PERIPH + 0x00000)		//!< 0x50000000
#define BASE_DCMI           (BASE_AHB2PERIPH + 0x50000)		//!< 0x50050000
#define BASE_CRYP           (BASE_AHB2PERIPH + 0x60000)		//!< 0x50060000 (!)
#define BASE_HASH           (BASE_AHB2PERIPH + 0x60400)		//!< 0x50060400 (!)
#define BASE_RNG            (BASE_AHB2PERIPH + 0x60800)		//!< 0x50060800

/**********************  FSMC Bankx registers base address ********************/
#define BASE_FSMC_Bank1_R	(BASE_FSMC_R + 0x0000)			//!< 0xA0000000
#define BASE_FSMC_Bank1E_R	(BASE_FSMC_R + 0x0104)			//!< 0xA0000104
#define BASE_FSMC_Bank2_R	(BASE_FSMC_R + 0x0060)			//!< 0xA0000060
#define BASE_FSMC_Bank3_R	(BASE_FSMC_R + 0x0080)			//!< 0xA0000080
#define BASE_FSMC_Bank4_R	(BASE_FSMC_R + 0x00A0)			//!< 0xA00000A0

/* Debug MCU registers base address */
#define BASE_DBGMCU         (BASE_EXT_PPB + 0x0000)			//!< 0xE0042000

/** Bit-banding registers */
#define BASE_PWR_BB         (BASE_PERIPH_BB + (BASE_PWR - BASE_PERIPHERAL)*32 )

#endif /* MCU_INC_H_ */
