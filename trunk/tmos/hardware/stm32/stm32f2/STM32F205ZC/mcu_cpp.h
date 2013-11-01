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
#define RCC			CAST(RCC_TypeDef	, RCC_BASE)  	//!< (RCC) Base Address
#define FLASH       CAST(FLASH_TypeDef	, FLASH_R_BASE)	//!< Flash registers base address
#define DBGMCU      CAST(DBGMCU_TypeDef , DBGMCU_BASE)	//!< Debug MCU registers base address
#define EXTI        CAST(EXTI_TypeDef	, EXTI_BASE)	//!< External Interrupt/Event Controller (EXTI)
#define SYSCFG      CAST(SYSCFG_TypeDef , SYSCFG_BASE)	//!< System configuration controller
#define GPIOA       CAST(GPIO_TypeDef   , GPIOA_BASE)	//!< GPIO A
#define GPIOB       CAST(GPIO_TypeDef   , GPIOB_BASE)   //!< GPIO B
#define GPIOC       CAST(GPIO_TypeDef   , GPIOC_BASE)   //!< GPIO C
#define GPIOD       CAST(GPIO_TypeDef   , GPIOD_BASE)   //!< GPIO D
#define GPIOE       CAST(GPIO_TypeDef   , GPIOE_BASE)   //!< GPIO E
#define GPIOF       CAST(GPIO_TypeDef   , GPIOF_BASE)   //!< GPIO F
#define GPIOG       CAST(GPIO_TypeDef   , GPIOG_BASE)   //!< GPIO G
#define GPIOH       CAST(GPIO_TypeDef   , GPIOH_BASE)   //!< GPIO H
#define GPIOI       CAST(GPIO_TypeDef   , GPIOI_BASE)   //!< GPIO I
#define DMA1        CAST(DMA_TypeDef 	, DMA1_BASE)    //!< DMA 1
#define DMA2        CAST(DMA_TypeDef 	, DMA2_BASE)    //!< DMA 2
#define USART1      CAST(USART_TypeDef  , USART1_BASE)	//!< USART1
#define USART2      CAST(USART_TypeDef  , USART2_BASE)	//!< USART2
#define USART3      CAST(USART_TypeDef  , USART3_BASE)	//!< USART3
#define UART4       CAST(USART_TypeDef  , UART4_BASE) 	//!< UART4
#define UART5       CAST(USART_TypeDef  , UART5_BASE) 	//!< UART5
#define USART6      CAST(USART_TypeDef  , USART6_BASE)	//!< USART6
#define SPI1        CAST(SPI_TypeDef 	, SPI1_BASE)	//!< SPI 1
#define SPI2        CAST(SPI_TypeDef 	, SPI2_BASE)	//!< SPI 2
#define SPI3        CAST(SPI_TypeDef 	, SPI3_BASE)	//!< SPI 3
#define OTG_HS		CAST(USB_TypeDef 	, 0x40040000)	//!< OTG HS
#define OTG_FS		CAST(USB_TypeDef 	, 0x50000000)	//!< OTG FS
#define RTC			CAST(RTC_TypeDef 	, RTC_BASE)		//!< RTC
#define PWR			CAST(PWR_TypeDef 	, PWR_BASE)		//!< Power control
#define ADC         CAST(ADCS_TypeDef 	, ADC1_BASE)	//!< ADC common
#define ADC1        CAST(ADC_TypeDef 	, ADC1_BASE)	//!< ADC1
#define ADC2        CAST(ADC_TypeDef 	, ADC2_BASE)	//!< ADC2
#define ADC3        CAST(ADC_TypeDef 	, ADC3_BASE)	//!< ADC3
#define TIM1        CAST(TIM_TypeDef    , TIM1_BASE)    //!< TIM1
#define TIM2        CAST(TIM_TypeDef    , TIM2_BASE)    //!< TIM2
#define TIM3        CAST(TIM_TypeDef    , TIM3_BASE)    //!< TIM3
#define TIM4        CAST(TIM_TypeDef    , TIM4_BASE)    //!< TIM4
#define TIM5        CAST(TIM_TypeDef    , TIM5_BASE)    //!< TIM5
#define TIM6        CAST(TIM_TypeDef    , TIM6_BASE)    //!< TIM6
#define TIM7        CAST(TIM_TypeDef    , TIM7_BASE)    //!< TIM7
#define TIM8        CAST(TIM_TypeDef    , TIM8_BASE)    //!< TIM8
#define TIM9        CAST(TIM_TypeDef    , TIM9_BASE)    //!< TIM9
#define TIM11       CAST(TIM_TypeDef    , TIM11_BASE)   //!< TIM11
#define TIM10       CAST(TIM_TypeDef    , TIM10_BASE)   //!< TIM10
#define TIM12       CAST(TIM_TypeDef    , TIM12_BASE)   //!< TIM12
#define TIM13       CAST(TIM_TypeDef    , TIM13_BASE)   //!< TIM13
#define TIM14       CAST(TIM_TypeDef    , TIM14_BASE)   //!< TIM14
#define SDIO        CAST(SDIO_TypeDef	, SDIO_BASE)	//!< SDIO
/** @} */

/**
 * @ingroup	 STM32F205ZC
 * \addtogroup STM32_base Bit-banding Peripheral aliases
 * @{
 */
#define PWR_BB		CAST(PWR_BB_TypeDef , PWR_BB_BASE)	//!< Power control
/** @} */

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR SAM3S */
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
