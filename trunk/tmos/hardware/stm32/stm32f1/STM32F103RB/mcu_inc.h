/**************************************************************************//**
 * @file     hardware/stm32/stm32f1/STM32F103RB/mcu_inc.h
 * @ingroup	 hardware_stm32f103rb
 * @brief    STM32F103RB definitions
 * @version  V3.00
 * @date     26. October 2012
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f103rb
 * @ingroup	 hardware_stm32f1
 * Source and definitions for STM32F103RB chip
 *
 *
 ******************************************************************************/

#ifndef MCU_INC_H_
#define MCU_INC_H_

#include <core_map.h>

#define STM32F10X_MD			//!< STM32F103RB is medium density line


#define RAM_SIZE 			(20*1024) 	 //!< 20K RAM size

#define FLASH_BASE			(0x08000000) //!< FLASH base address in the alias region

#define SRAM_BB_BASE        (0x22000000) //!< SRAM base address in the bit-band region
#define PERIPH_BB_BASE      (0x42000000) //!< Peripheral base address in the bit-band region

#define FSMC_R_BASE         (0xA0000000) //!< FSMC registers base address

/*!< Peripheral memory map */
#define APB1PERIPH_BASE     BASE_PERIPHERAL
#define APB2PERIPH_BASE     (BASE_PERIPHERAL + 0x10000)
#define AHBPERIPH_BASE      (BASE_PERIPHERAL + 0x20000)

#define TIM2_BASE           (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE           (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE           (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE           (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE           (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE           (APB1PERIPH_BASE + 0x1400)
#define TIM12_BASE          (APB1PERIPH_BASE + 0x1800)
#define TIM13_BASE          (APB1PERIPH_BASE + 0x1C00)
#define TIM14_BASE          (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE            (APB1PERIPH_BASE + 0x2800)
#define WWDG_BASE           (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE           (APB1PERIPH_BASE + 0x3000)
#define SPI2_BASE           (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE           (APB1PERIPH_BASE + 0x3C00)
#define USART2_BASE         (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE         (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE          (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE          (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE           (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE           (APB1PERIPH_BASE + 0x5800)
#define CAN1_BASE           (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE           (APB1PERIPH_BASE + 0x6800)
#define BKP_BASE            (APB1PERIPH_BASE + 0x6C00)
#define PWR_BASE            (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE            (APB1PERIPH_BASE + 0x7400)
#define CEC_BASE            (APB1PERIPH_BASE + 0x7800)

#define AFIO_BASE           (APB2PERIPH_BASE + 0x0000)
#define EXTI_BASE           (APB2PERIPH_BASE + 0x0400)
#define GPIOA_BASE          (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE          (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE          (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE          (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE          (APB2PERIPH_BASE + 0x1800)
#define GPIOF_BASE          (APB2PERIPH_BASE + 0x1C00)
#define GPIOG_BASE          (APB2PERIPH_BASE + 0x2000)
#define ADC1_BASE           (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE           (APB2PERIPH_BASE + 0x2800)
#define TIM1_BASE           (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE           (APB2PERIPH_BASE + 0x3000)
#define TIM8_BASE           (APB2PERIPH_BASE + 0x3400)
#define USART1_BASE         (APB2PERIPH_BASE + 0x3800)
#define ADC3_BASE           (APB2PERIPH_BASE + 0x3C00)
#define TIM15_BASE          (APB2PERIPH_BASE + 0x4000)
#define TIM16_BASE          (APB2PERIPH_BASE + 0x4400)
#define TIM17_BASE          (APB2PERIPH_BASE + 0x4800)
#define TIM9_BASE           (APB2PERIPH_BASE + 0x4C00)
#define TIM10_BASE          (APB2PERIPH_BASE + 0x5000)
#define TIM11_BASE          (APB2PERIPH_BASE + 0x5400)

#define SDIO_BASE           (BASE_PERIPHERAL + 0x18000)

#define DMA1_BASE           (AHBPERIPH_BASE + 0x0000)
#define DMA1_Channel1_BASE  (AHBPERIPH_BASE + 0x0008)
#define DMA1_Channel2_BASE  (AHBPERIPH_BASE + 0x001C)
#define DMA1_Channel3_BASE  (AHBPERIPH_BASE + 0x0030)
#define DMA1_Channel4_BASE  (AHBPERIPH_BASE + 0x0044)
#define DMA1_Channel5_BASE  (AHBPERIPH_BASE + 0x0058)
#define DMA1_Channel6_BASE  (AHBPERIPH_BASE + 0x006C)
#define DMA1_Channel7_BASE  (AHBPERIPH_BASE + 0x0080)
#define DMA2_BASE           (AHBPERIPH_BASE + 0x0400)
#define DMA2_Channel1_BASE  (AHBPERIPH_BASE + 0x0408)
#define DMA2_Channel2_BASE  (AHBPERIPH_BASE + 0x041C)
#define DMA2_Channel3_BASE  (AHBPERIPH_BASE + 0x0430)
#define DMA2_Channel4_BASE  (AHBPERIPH_BASE + 0x0444)
#define DMA2_Channel5_BASE  (AHBPERIPH_BASE + 0x0458)
#define RCC_BASE            (AHBPERIPH_BASE + 0x1000)
#define CRC_BASE            (AHBPERIPH_BASE + 0x3000)

#define FLASH_R_BASE        (AHBPERIPH_BASE + 0x2000) //!< Flash registers base address
#define OB_BASE             (0x1FFFF800)   			  //!< Flash Option Bytes base address

#define ETH_BASE            (AHBPERIPH_BASE + 0x8000)
#define ETH_MAC_BASE        (ETH_BASE)
#define ETH_MMC_BASE        (ETH_BASE + 0x0100)
#define ETH_PTP_BASE        (ETH_BASE + 0x0700)
#define ETH_DMA_BASE        (ETH_BASE + 0x1000)

#define FSMC_Bank1_R_BASE   (FSMC_R_BASE + 0x0000) 	  //!< FSMC Bank1 registers base address
#define FSMC_Bank1E_R_BASE  (FSMC_R_BASE + 0x0104) 	  //!< FSMC Bank1E registers base address
#define FSMC_Bank2_R_BASE   (FSMC_R_BASE + 0x0060)    //!< FSMC Bank2 registers base address
#define FSMC_Bank3_R_BASE   (FSMC_R_BASE + 0x0080)    //!< FSMC Bank3 registers base address
#define FSMC_Bank4_R_BASE   (FSMC_R_BASE + 0x00A0) 	  //!< FSMC Bank4 registers base address

#define DBGMCU_BASE         (0xE0042000) /*!< Debug MCU registers base address */

/** Bit-banding registers */
#define PWR_BB_BASE           (PERIPH_BB_BASE + (PWR_BASE - BASE_PERIPHERAL)*32 )

#endif /* MCU_INC_H_ */
