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
#define RCC			CAST(RCC_TypeDef	, AHBPERIPH_BASE + 0x1000)  //!< (RCC) Base Address
#define FLASH       CAST(FLASH_TypeDef	, AHBPERIPH_BASE + 0x2000)	//!< Flash registers base address
#define DBGMCU      CAST(DBGMCU_TypeDef , 0xE0042000)				//!< Debug MCU registers base address

/** @} */

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR SAM3S */
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
