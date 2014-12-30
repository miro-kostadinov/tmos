/**************************************************************************//**
 * @file     hardware/stm32/stm32f0/STM32F030R8/mcu_cpp.h
 * @ingroup	 hardware_stm32f030r8
 * @brief    STM32F030R8 header
 * @version  V3.00
 * @date     29. October 2012
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#ifndef MCU_CPP_H_
#define MCU_CPP_H_

#include <mcu_inc.h>


/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR STM32F030R8T6 */
/* ************************************************************************** */
/**
 * @ingroup	 STM32F030R8
 * \addtogroup STM32_base Peripheral Base Address Definitions
 * @{
 */
#define RCC			CAST(RCC_TypeDef	, BASE_RCC)  	//!< (RCC) Base Address
#define FLASH       CAST(FLASH_TypeDef	, BASE_FLASH_R)	//!< Flash registers base address
#define DBGMCU      CAST(DBGMCU_TypeDef , BASE_DBGMCU)	//!< Debug MCU registers base address
#define EXTI        CAST(EXTI_TypeDef	, BASE_EXTI)	//!< External Interrupt/Event Controller (EXTI)
#define SYSCFG      CAST(SYSCFG_TypeDef , BASE_SYSCFG)	//!< System configuration controller
#define GPIOA       CAST(GPIO_TypeDef   , BASE_GPIOA)	//!< GPIO A
#define GPIOB       CAST(GPIO_TypeDef   , BASE_GPIOB)   //!< GPIO B
#define GPIOC       CAST(GPIO_TypeDef   , BASE_GPIOC)   //!< GPIO C
#define GPIOD       CAST(GPIO_TypeDef   , BASE_GPIOD)   //!< GPIO D
#define GPIOF       CAST(GPIO_TypeDef   , BASE_GPIOF)   //!< GPIO F
#define DMA         CAST(DMA_TypeDef 	, BASE_DMA)     //!< DMA
#define USART1      CAST(USART_TypeDef  , BASE_USART1)	//!< USART1
#define USART2      CAST(USART_TypeDef  , BASE_USART2)	//!< USART2
#define SPI1        CAST(SPI_TypeDef 	, BASE_SPI1)	//!< SPI 1
#define SPI2        CAST(SPI_TypeDef 	, BASE_SPI2)	//!< SPI 2
#define RTC			CAST(RTC_TypeDef 	, BASE_RTC)		//!< RTC
#define PWR			CAST(PWR_TypeDef 	, BASE_PWR)		//!< Power control
#define ADC         CAST(ADC_TypeDef 	, BASE_ADC1)	//!< ADC
#define TIM1        CAST(TIM_TypeDef    , BASE_TIM1)    //!< TIM1
#define TIM3        CAST(TIM_TypeDef    , BASE_TIM3)    //!< TIM3
#define TIM6        CAST(TIM_TypeDef    , BASE_TIM6)    //!< TIM6
#define TIM14       CAST(TIM_TypeDef    , BASE_TIM14)   //!< TIM14
#define TIM15       CAST(TIM_TypeDef    , BASE_TIM15)   //!< TIM15
#define TIM16       CAST(TIM_TypeDef    , BASE_TIM16)   //!< TIM16
#define TIM17       CAST(TIM_TypeDef    , BASE_TIM17)   //!< TIM17
#define CRC         CAST(CRC_TypeDef	, BASE_CRC)		//!< CRC
/** @} */

/* RCC registers */
#define REG_RCC_CR				REG_ACCESS(__IO uint32_t, BASE_RCC+0x00)	//!< Clock control register
#define REG_RCC_CFGR			REG_ACCESS(__IO uint32_t, BASE_RCC+0x04)	//!< Clock configuration register
#define REG_RCC_CIR				REG_ACCESS(__IO uint32_t, BASE_RCC+0x08)	//!< Clock interrupt register
#define REG_RCC_RCC_APB2RSTR	REG_ACCESS(__IO uint32_t, BASE_RCC+0x0C)	//!< APB2 peripheral reset register
#define REG_RCC_RCC_APB1RSTR	REG_ACCESS(__IO uint32_t, BASE_RCC+0x10)	//!< APB1 peripheral reset register
#define REG_RCC_RCC_AHBENR 		REG_ACCESS(__IO uint32_t, BASE_RCC+0x14)	//!< AHB peripheral clock enable register
#define REG_RCC_RCC_APB2ENR		REG_ACCESS(__IO uint32_t, BASE_RCC+0x18)	//!< APB2 peripheral clock enable register
#define REG_RCC_RCC_APB1ENR		REG_ACCESS(__IO uint32_t, BASE_RCC+0x1C)	//!< APB1 peripheral clock enable register
#define REG_RCC_RCC_BDCR    	REG_ACCESS(__IO uint32_t, BASE_RCC+0x20)  	//!< Backup domain control register
#define REG_RCC_RCC_CSR	    	REG_ACCESS(__IO uint32_t, BASE_RCC+0x24)  	//!< Control/status register
#define REG_RCC_RCC_AHBRSTR   	REG_ACCESS(__IO uint32_t, BASE_RCC+0x28) 	//!< AHB peripheral reset register
#define REG_RCC_RCC_CFGR2		REG_ACCESS(__IO uint32_t, BASE_RCC+0x2C)	//!< Clock configuration register 2
#define REG_RCC_RCC_CFGR3		REG_ACCESS(__IO uint32_t, BASE_RCC+0x30)	//!< Clock configuration register 3
#define REG_RCC_RCC_CR2		   	REG_ACCESS(__IO uint32_t, BASE_RCC+0x34) 	//!< Clock control register 2

/* CRC registers */
#define REG_CRC_DR		REG_ACCESS(__IO uint32_t, BASE_CRC+0x00) //!< CRC Data register
#define REG_CRC_CR	    REG_ACCESS(__IO uint32_t, BASE_CRC+0x08) //!< CRC Control register

/**
 * @ingroup	 STM32F030R8
 * \addtogroup STM32_base Bit-banding Peripheral aliases
 * @{
 */
#define PWR_BB		CAST(PWR_BB_TypeDef , BASE_PWR_BB)	//!< Power control
/** @} */

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR STM32F0x */
/* ************************************************************************** */
/**
 * @ingroup	 STM32F030R8
 * \addtogroup STM32_id Peripheral Ids Definitions
 * @{
 */
// ------------------- index 0 ->   RCC_AHBENR  -------------------------------
#define ID_PERIPH_DMA     	((0<<5)| 0) //!< DMA
#define ID_PERIPH_SRAM      ((0<<5)| 2) //!< SRAM
#define ID_PERIPH_FLITF     ((0<<5)| 4) //!< FLITF
#define ID_PERIPH_CRC 		((0<<5)| 6) //!< CRC
#define ID_PERIPH_GPIOA     ((0<<5)|17) //!< GPIO A
#define ID_PERIPH_GPIOB     ((0<<5)|18) //!< GPIO B
#define ID_PERIPH_GPIOC     ((0<<5)|19) //!< GPIO C
#define ID_PERIPH_GPIOD     ((0<<5)|20) //!< GPIO D
#define ID_PERIPH_GPIOF     ((0<<5)|22) //!< GPIO F

// ------------------- index 1 ->   RCC_APB2ENR  -------------------------------
#define ID_PERIPH_SYSC      ((1<<5)| 0) //!< SYSC
#define ID_PERIPH_ADC		((1<<5)| 9) //!< ADC
#define ID_PERIPH_TIM1      ((1<<5)|11) //!< TIM 1
#define ID_PERIPH_SPI1      ((1<<5)|12) //!< SPI 1
#define ID_PERIPH_USART1    ((1<<5)|14) //!< USART 1
#define ID_PERIPH_TIM15     ((1<<5)|16) //!< TIM 15
#define ID_PERIPH_TIM16     ((1<<5)|17) //!< TIM 16
#define ID_PERIPH_TIM17     ((1<<5)|18) //!< TIM 17
#define ID_PERIPH_DBGMCU    ((1<<5)|22) //!< TIM 17

// ------------------- index 2 ->   RCC_APB1ENR  -------------------------------
#define ID_PERIPH_TIM3      ((2<<5)| 1) //!< TIM 3
#define ID_PERIPH_TIM6      ((2<<5)| 4) //!< TIM 6
#define ID_PERIPH_TIM14     ((2<<5)| 8) //!< TIM 14
#define ID_PERIPH_WWDG      ((2<<5)|11) //!< W WDG
#define ID_PERIPH_SPI2      ((2<<5)|14) //!< SPI 2
#define ID_PERIPH_USART2    ((2<<5)|17) //!< USART 2
#define ID_PERIPH_I2C1      ((2<<5)|21) //!< I2C 1
#define ID_PERIPH_I2C2      ((2<<5)|22) //!< I2C 2
#define ID_PERIPH_PWR       ((2<<5)|28) //!< PWR


#define ID_NO_PERIPH 		(255)		//!< invalid index for peripheral
/** @}*/



#endif /* MCU_CPP_H_ */
