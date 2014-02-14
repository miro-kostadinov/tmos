/**************************************************************************//**
 * @file     hardware/stm32/stm32f2/STM32F205ZC/mcu_cpp.h
 * @ingroup	 hardware_stm32f205zc
 * @brief    STM32F205ZC header
 * @version  V3.00
 * @date     29. October 2012
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#ifndef MCU_CPP_H_
#define MCU_CPP_H_

#include <mcu_inc.h>


/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR STM32F205ZC */
/* ************************************************************************** */
/**
 * @ingroup	 STM32F205ZC
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
#define GPIOE       CAST(GPIO_TypeDef   , BASE_GPIOE)   //!< GPIO E
#define GPIOF       CAST(GPIO_TypeDef   , BASE_GPIOF)   //!< GPIO F
#define GPIOG       CAST(GPIO_TypeDef   , BASE_GPIOG)   //!< GPIO G
#define GPIOH       CAST(GPIO_TypeDef   , BASE_GPIOH)   //!< GPIO H
#define GPIOI       CAST(GPIO_TypeDef   , BASE_GPIOI)   //!< GPIO I
#define DMA1        CAST(DMA_TypeDef 	, BASE_DMA1)    //!< DMA 1
#define DMA2        CAST(DMA_TypeDef 	, BASE_DMA2)    //!< DMA 2
#define USART1      CAST(USART_TypeDef  , BASE_USART1)	//!< USART1
#define USART2      CAST(USART_TypeDef  , BASE_USART2)	//!< USART2
#define USART3      CAST(USART_TypeDef  , BASE_USART3)	//!< USART3
#define UART4       CAST(USART_TypeDef  , BASE_UART4) 	//!< UART4
#define UART5       CAST(USART_TypeDef  , BASE_UART5) 	//!< UART5
#define USART6      CAST(USART_TypeDef  , BASE_USART6)	//!< USART6
#define SPI1        CAST(SPI_TypeDef 	, BASE_SPI1)	//!< SPI 1
#define SPI2        CAST(SPI_TypeDef 	, BASE_SPI2)	//!< SPI 2
#define SPI3        CAST(SPI_TypeDef 	, BASE_SPI3)	//!< SPI 3
#define OTG_HS		CAST(USB_TypeDef 	, BASE_OTG_HS)	//!< OTG HS
#define OTG_FS		CAST(USB_TypeDef 	, BASE_OTG_FS)	//!< OTG FS
#define RTC			CAST(RTC_TypeDef 	, BASE_RTC)		//!< RTC
#define PWR			CAST(PWR_TypeDef 	, BASE_PWR)		//!< Power control
#define ADC         CAST(ADCS_TypeDef 	, BASE_ADC1)	//!< ADC common
#define ADC1        CAST(ADC_TypeDef 	, BASE_ADC1)	//!< ADC1
#define ADC2        CAST(ADC_TypeDef 	, BASE_ADC2)	//!< ADC2
#define ADC3        CAST(ADC_TypeDef 	, BASE_ADC3)	//!< ADC3
#define TIM1        CAST(TIM_TypeDef    , BASE_TIM1)    //!< TIM1
#define TIM2        CAST(TIM_TypeDef    , BASE_TIM2)    //!< TIM2
#define TIM3        CAST(TIM_TypeDef    , BASE_TIM3)    //!< TIM3
#define TIM4        CAST(TIM_TypeDef    , BASE_TIM4)    //!< TIM4
#define TIM5        CAST(TIM_TypeDef    , BASE_TIM5)    //!< TIM5
#define TIM6        CAST(TIM_TypeDef    , BASE_TIM6)    //!< TIM6
#define TIM7        CAST(TIM_TypeDef    , BASE_TIM7)    //!< TIM7
#define TIM8        CAST(TIM_TypeDef    , BASE_TIM8)    //!< TIM8
#define TIM9        CAST(TIM_TypeDef    , BASE_TIM9)    //!< TIM9
#define TIM11       CAST(TIM_TypeDef    , BASE_TIM11)   //!< TIM11
#define TIM10       CAST(TIM_TypeDef    , BASE_TIM10)   //!< TIM10
#define TIM12       CAST(TIM_TypeDef    , BASE_TIM12)   //!< TIM12
#define TIM13       CAST(TIM_TypeDef    , BASE_TIM13)   //!< TIM13
#define TIM14       CAST(TIM_TypeDef    , BASE_TIM14)   //!< TIM14
#define SDIO        CAST(SDIO_TypeDef	, BASE_SDIO)	//!< SDIO
#define CRC         CAST(CRC_TypeDef	, BASE_CRC)		//!< CRC
/** @} */

/* RCC registers */
#define REG_RCC_CR				REG_ACCESS(__IO uint32_t, BASE_RCC+0x00)	//!< Clock control register
#define REG_RCC_PLLCFGR			REG_ACCESS(__IO uint32_t, BASE_RCC+0x04)	//!< RCC PLL configuration register
#define REG_RCC_CFGR			REG_ACCESS(__IO uint32_t, BASE_RCC+0x08)	//!< Clock configuration register
#define REG_RCC_CIR				REG_ACCESS(__IO uint32_t, BASE_RCC+0x0C)	//!< Clock interrupt register
#define REG_RCC_AHB1RSTR		REG_ACCESS(__IO uint32_t, BASE_RCC+0x10)	//!< AHB1 peripheral reset register
#define REG_RCC_AHB2RSTR		REG_ACCESS(__IO uint32_t, BASE_RCC+0x14)	//!< AHB2 peripheral reset register
#define REG_RCC_AHB3RSTR		REG_ACCESS(__IO uint32_t, BASE_RCC+0x18)	//!< AHB3 peripheral reset register
#define REG_RCC_APB1RSTR		REG_ACCESS(__IO uint32_t, BASE_RCC+0x20)	//!< APB1 peripheral reset register
#define REG_RCC_APB2RSTR		REG_ACCESS(__IO uint32_t, BASE_RCC+0x24)	//!< APB2 peripheral reset register
#define REG_RCC_AHB1ENR       	REG_ACCESS(__IO uint32_t, BASE_RCC+0x30)  	//!< RCC AHB1 peripheral clock register
#define REG_RCC_AHB2ENR       	REG_ACCESS(__IO uint32_t, BASE_RCC+0x34)  	//!< RCC AHB2 peripheral clock register
#define REG_RCC_AHB3ENR      	REG_ACCESS(__IO uint32_t, BASE_RCC+0x38) 	//!< RCC AHB3 peripheral clock register
#define REG_RCC_APB1ENR			REG_ACCESS(__IO uint32_t, BASE_RCC+0x40)	//!< APB1 peripheral clock enable register
#define REG_RCC_APB2ENR			REG_ACCESS(__IO uint32_t, BASE_RCC+0x44)	//!< APB2 peripheral clock enable register
#define REG_RCC_AHB1LPENR    	REG_ACCESS(__IO uint32_t, BASE_RCC+0x50) 	//!< RCC AHB1 peripheral clock enable in low power mode register
#define REG_RCC_AHB2LPENR     	REG_ACCESS(__IO uint32_t, BASE_RCC+0x54)  	//!< RCC AHB2 peripheral clock enable in low power mode register
#define REG_RCC_AHB3LPENR    	REG_ACCESS(__IO uint32_t, BASE_RCC+0x58) 	//!< RCC AHB3 peripheral clock enable in low power mode register
#define REG_RCC_APB1LPENR   	REG_ACCESS(__IO uint32_t, BASE_RCC+0x60)	//!< RCC APB1 peripheral clock enable in low power mode register
#define REG_RCC_APB2LPENR   	REG_ACCESS(__IO uint32_t, BASE_RCC+0x64)	//!< RCC APB2 peripheral clock enable in low power mode register
#define REG_RCC_BDCR			REG_ACCESS(__IO uint32_t, BASE_RCC+0x70)	//!< Backup domain control register
#define REG_RCC_CSR				REG_ACCESS(__IO uint32_t, BASE_RCC+0x74)	//!< Control/status register
#define REG_RCC_SSCGR       	REG_ACCESS(__IO uint32_t, BASE_RCC+0x80)	//!< RCC spread spectrum clock generation register
#define REG_RCC_PLLI2SCFGR  	REG_ACCESS(__IO uint32_t, BASE_RCC+0x84)	//!< RCC PLLI2S configuration register

/* CRC registers */
#define REG_CRC_DR		REG_ACCESS(__IO uint32_t, BASE_CRC+0x00) //!< CRC Data register
#define REG_CRC_CR	    REG_ACCESS(__IO uint32_t, BASE_CRC+0x08) //!< CRC Control register

/**
 * @ingroup	 STM32F205ZC
 * \addtogroup STM32_base Bit-banding Peripheral aliases
 * @{
 */
#define PWR_BB		CAST(PWR_BB_TypeDef , BASE_PWR_BB)	//!< Power control
/** @} */

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR STM32F2x */
/* ************************************************************************** */
/**
 * @ingroup	 STM32F205ZC
 * \addtogroup STM32_id Peripheral Ids Definitions
 * @{
 */
// ------------------- index 0 ->   RCC_AHB1ENR  -------------------------------
#define ID_PERIPH_GPIOA     ((0<<5)| 0) //!< GPIO A
#define ID_PERIPH_GPIOB     ((0<<5)| 1) //!< GPIO B
#define ID_PERIPH_GPIOC     ((0<<5)| 2) //!< GPIO C
#define ID_PERIPH_GPIOD     ((0<<5)| 3) //!< GPIO D
#define ID_PERIPH_GPIOE     ((0<<5)| 4) //!< GPIO E
#define ID_PERIPH_GPIOF     ((0<<5)| 5) //!< GPIO F
#define ID_PERIPH_GPIOG     ((0<<5)| 6) //!< GPIO G
#define ID_PERIPH_GPIOH     ((0<<5)| 7) //!< GPIO H
#define ID_PERIPH_GPIOI     ((0<<5)| 8) //!< GPIO I
#define ID_PERIPH_CRC 		((0<<5)|12) //!< CRC
#define ID_PERIPH_FLITF     ((0<<5)|15) //!< FLITF
#define ID_PERIPH_SRAM1     ((0<<5)|16) //!< SRAM 1
#define ID_PERIPH_SRAM2     ((0<<5)|17) //!< SRAM 2
#define ID_PERIPH_BKPSPA    ((0<<5)|18) //!< BKPA
#define ID_PERIPH_DMA1     	((0<<5)|21) //!< DMA1
#define ID_PERIPH_DMA2 		((0<<5)|22) //!< DMA2
#define ID_PERIPH_ETHMAC  	((0<<5)|25) //!< ETH MAC
#define ID_PERIPH_ETHMACTX	((0<<5)|26) //!< ETH MAC TX
#define ID_PERIPH_ETHMACRX	((0<<5)|27) //!< ETH MAC RX
#define ID_PERIPH_ETHPTP	((0<<5)|28) //!< ETH PTP
#define ID_PERIPH_OTGHS		((0<<5)|29) //!< OTG HS
#define ID_PERIPH_OTGHS_ULPI ((0<<5)|30) //!< OTG HS ULPI

// ------------------- index 1 ->   RCC_AHB2ENR  -------------------------------
#define ID_PERIPH_DCMI		((1<<5)| 0) //!< DCMI
#define ID_PERIPH_CRYP		((1<<5)| 4) //!< CRYP
#define ID_PERIPH_HASH		((1<<5)| 5) //!< HASH
#define ID_PERIPH_RNG		((1<<5)| 6) //!< RNG
#define ID_PERIPH_OTGFS		((1<<5)| 7) //!< OTG FS

// ------------------- index 2 ->   RCC_AHB3ENR  -------------------------------
#define ID_PERIPH_FSMC		((2<<5)| 0) //!< FSMC

// ------------------- index 3 ->   reserved     -------------------------------

// ------------------- index 4 ->   RCC_APB1ENR  -------------------------------
#define ID_PERIPH_TIM2      ((4<<5)| 0) //!< TIM 2
#define ID_PERIPH_TIM3      ((4<<5)| 1) //!< TIM 3
#define ID_PERIPH_TIM4      ((4<<5)| 2) //!< TIM 4
#define ID_PERIPH_TIM5      ((4<<5)| 3) //!< TIM 5
#define ID_PERIPH_TIM6      ((4<<5)| 4) //!< TIM 6
#define ID_PERIPH_TIM7      ((4<<5)| 5) //!< TIM 7
#define ID_PERIPH_TIM12     ((4<<5)| 6) //!< TIM 12
#define ID_PERIPH_TIM13     ((4<<5)| 7) //!< TIM 13
#define ID_PERIPH_TIM14     ((4<<5)| 8) //!< TIM 14
#define ID_PERIPH_WWDG      ((4<<5)|11) //!< W WDG
#define ID_PERIPH_SPI2      ((4<<5)|14) //!< SPI 2
#define ID_PERIPH_SPI3      ((4<<5)|15) //!< SPI 3
#define ID_PERIPH_USART2    ((4<<5)|17) //!< USART 2
#define ID_PERIPH_USART3    ((4<<5)|18) //!< USART 3
#define ID_PERIPH_USART4    ((4<<5)|19) //!< USART 4
#define ID_PERIPH_USART5    ((4<<5)|20) //!< USART 5
#define ID_PERIPH_I2C1      ((4<<5)|21) //!< I2C 1
#define ID_PERIPH_I2C2      ((4<<5)|22) //!< I2C 2
#define ID_PERIPH_I2C3      ((4<<5)|23) //!< I2C 3
#define ID_PERIPH_CAN1      ((4<<5)|25) //!< CAN 1
#define ID_PERIPH_CAN2      ((4<<5)|26) //!< CAN 2
#define ID_PERIPH_PWR       ((4<<5)|28) //!< PWR
#define ID_PERIPH_DAC       ((4<<5)|29) //!< DAC

// ------------------- index 5 ->   RCC_APB2ENR  -------------------------------
#define ID_PERIPH_TIM1      ((5<<5)| 0) //!< TIM 1
#define ID_PERIPH_TIM8      ((5<<5)| 1) //!< TIM 8
#define ID_PERIPH_USART1    ((5<<5)| 4) //!< USART 1
#define ID_PERIPH_USART6    ((5<<5)| 5) //!< USART 6
#define ID_PERIPH_ADC1		((5<<5)| 8) //!< ADC 1
#define ID_PERIPH_ADC2      ((5<<5)| 9) //!< ADC 2
#define ID_PERIPH_ADC3      ((5<<5)|10) //!< ADC 3
#define ID_PERIPH_SDIO      ((5<<5)|11) //!< SDIO
#define ID_PERIPH_SPI1      ((5<<5)|12) //!< SPI 1
#define ID_PERIPH_SYSC      ((5<<5)|14) //!< SYSC
#define ID_PERIPH_TIM9      ((5<<5)|16) //!< TIM 9
#define ID_PERIPH_TIM10     ((5<<5)|17) //!< TIM 10
#define ID_PERIPH_TIM11     ((5<<5)|18) //!< TIM 11

#define ID_NO_PERIPH 		(255)		//!< invalid index for peripheral
/** @}*/


/** Number of USB endpoints */
#define CHIP_USB_NUMENDPOINTS 6		//the USB HS has 1 control, 5 IN and 5 OUT endpoints

#endif /* MCU_CPP_H_ */
