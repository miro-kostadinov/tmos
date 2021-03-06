/**************************************************************************//**
 * @file     hardware/stm32/stm32f1/STM32F103RB/mcu_cpp.h
 * @ingroup	 hardware_stm32f103rb
 * @brief    STM32F103RB header
 * @version  V3.00
 * @date     26. October 2012
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#ifndef MCU_CPP_H_
#define MCU_CPP_H_

#include <mcu_inc.h>


/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR STM32F103RB */
/* ************************************************************************** */
/**
 * @ingroup	 STM32F103RB
 * \addtogroup STM32_base Peripheral Base Address Definitions
 * @{
 */
#define RCC			CAST(RCC_TypeDef	, RCC_BASE)  	//!< (RCC) Base Address
#define FLASH       CAST(FLASH_TypeDef	, FLASH_R_BASE)	//!< Flash registers base address
#define DBGMCU      CAST(DBGMCU_TypeDef , DBGMCU_BASE)	//!< Debug MCU registers base address
#define EXTI        CAST(EXTI_TypeDef	, EXTI_BASE)	//!< External Interrupt/Event Controller (EXTI)
#define AFIO        CAST(AFIO_TypeDef 	, AFIO_BASE)	//!< Alternate Function I/O (AFIO)
#define USART1      CAST(USART_TypeDef  , USART1_BASE)	//!< USART1
#define USART2      CAST(USART_TypeDef  , USART2_BASE)	//!< USART2
#define USART3      CAST(USART_TypeDef  , USART3_BASE)	//!< USART3
#define UART4       CAST(USART_TypeDef  , UART4_BASE) 	//!< UART4
#define UART5       CAST(USART_TypeDef  , UART5_BASE) 	//!< UART5
#define RTC			CAST(RTC_TypeDef 	, RTC_BASE)		//!< RTC
#define PWR			CAST(PWR_TypeDef 	, PWR_BASE)		//!< Power control
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
/** @} */

/**
 * @ingroup	 STM32F103RB
 * \addtogroup STM32_base Bit-banding Peripheral aliases
 * @{
 */
#define PWR_BB		CAST(PWR_BB_TypeDef , PWR_BB_BASE)	//!< Power control
/** @} */

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR STM32F1 */
/* ************************************************************************** */
/**
 * @ingroup	 STM32F103RB
 * \addtogroup STM32_id Peripheral Ids Definitions
 * @{
 */
#define ID_PERIPH_DMA1     	((0<<5)|00) //!< DMA1
#define ID_PERIPH_DMA2 		((0<<5)|01) //!< DMA2
#define ID_PERIPH_SRAM      ((0<<5)|02) //!< SRAM
#define ID_PERIPH_FLITF     ((0<<5)|04) //!< FLITF
#define ID_PERIPH_CRC 		((0<<5)|06) //!< CRC
#define ID_PERIPH_OTGFS		((0<<5)|12) //!< OTG FS
#define ID_PERIPH_ETHMAC  	((0<<5)|14) //!< ETH MAC
#define ID_PERIPH_ETHMACTX	((0<<5)|15) //!< ETH MAC TX
#define ID_PERIPH_ETHMACRX	((0<<5)|16) //!< ETH MAC RX
#define ID_PERIPH_AFIO		((1<<5)|00) //!< AFIO
#define ID_PERIPH_GPIOA     ((1<<5)|02) //!< GPIO A
#define ID_PERIPH_GPIOB     ((1<<5)|03) //!< GPIO B
#define ID_PERIPH_GPIOC     ((1<<5)|04) //!< GPIO C
#define ID_PERIPH_GPIOD     ((1<<5)|05) //!< GPIO D
#define ID_PERIPH_GPIOE     ((1<<5)|06) //!< GPIO E
#define ID_PERIPH_ADC1		((1<<5)|09) //!< ADC 1
#define ID_PERIPH_ADC2      ((1<<5)|10) //!< ADC 2
#define ID_PERIPH_TIM1      ((1<<5)|11) //!< TIM 1
#define ID_PERIPH_SPI1      ((1<<5)|12) //!< SPI 1
#define ID_PERIPH_USART1    ((1<<5)|14) //!< USART 1
#define ID_PERIPH_TIM2      ((2<<5)|00) //!< TIM 2
#define ID_PERIPH_TIM3      ((2<<5)|01) //!< TIM 3
#define ID_PERIPH_TIM4      ((2<<5)|02) //!< TIM 4
#define ID_PERIPH_TIM5      ((2<<5)|03) //!< TIM 5
#define ID_PERIPH_TIM6      ((2<<5)|04) //!< TIM 6
#define ID_PERIPH_TIM7      ((2<<5)|05) //!< TIM 7
#define ID_PERIPH_WWDG      ((2<<5)|11) //!< W WDG
#define ID_PERIPH_SPI2      ((2<<5)|14) //!< SPI 2
#define ID_PERIPH_SPI3      ((2<<5)|15) //!< SPI 3
#define ID_PERIPH_USART2    ((2<<5)|17) //!< USART 2
#define ID_PERIPH_USART3    ((2<<5)|18) //!< USART 3
#define ID_PERIPH_USART4    ((2<<5)|19) //!< USART 4
#define ID_PERIPH_USART5    ((2<<5)|20) //!< USART 5
#define ID_PERIPH_I2C1      ((2<<5)|21) //!< I2C 1
#define ID_PERIPH_I2C2      ((2<<5)|22) //!< I2C 2
#define ID_PERIPH_CAN1      ((2<<5)|25) //!< CAN 1
#define ID_PERIPH_CAN2      ((2<<5)|26) //!< CAN 2
#define ID_PERIPH_BKP       ((2<<5)|27) //!< BKP
#define ID_PERIPH_PWR       ((2<<5)|28) //!< PWR
#define ID_PERIPH_DAC       ((2<<5)|29) //!< DAC

#define ID_NO_PERIPH 		(255)		//!< invalid index for peripheral
/** @}*/

#endif /* MCU_CPP_H_ */
