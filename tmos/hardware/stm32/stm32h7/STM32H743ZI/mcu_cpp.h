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
#define ETH_MAC1    CAST(ETH_TypeDef	, BASE_ETH)		//!< Ethernet MAC
#define DAC         CAST(DAC_TypeDef	, BASE_DAC)		//!< DAC
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
// ------------------- index 0 ->   RCC_AHB3ENR  -------------------------------
#define ID_PERIPH_SDMMC1	((0<<5)|16) //!< SDMMC1
#define ID_PERIPH_QSPIEN	((0<<5)|14) //!< QUADSPI
#define ID_PERIPH_FMC		((0<<5)|12) //!< FMC
#define ID_PERIPH_JPGDEC	((0<<5)| 5) //!< JPGDEC
#define ID_PERIPH_DMA2D		((0<<5)| 4) //!< DMA2D
#define ID_PERIPH_MDMA		((0<<5)| 0) //!< MDMA

// ------------------- index 1 ->   RCC_AHB1ENR  -------------------------------
#define ID_PERIPH_USB2OTGHS		((1<<5)|27) //!< USB2OTG (OTG_HS2)
#define ID_PERIPH_USB1OTGHSULPI	((1<<5)|26) //!< USB_PHY1
#define ID_PERIPH_USB1OTGHS		((1<<5)|25) //!< USB1OTG (OTG_HS1)
#define ID_PERIPH_USB2OTGHSULPI	((1<<5)|18) //!< USB_PHY2
#define ID_PERIPH_ETH1RX		((1<<5)|17) //!< Ethernet Reception
#define ID_PERIPH_ETH1TX		((1<<5)|16) //!< Ethernet Transmission
#define ID_PERIPH_ETH1MAC		((1<<5)|15) //!< Ethernet MAC
#define ID_PERIPH_ADC12			((1<<5)| 5) //!< ADC1/2
#define ID_PERIPH_DMA2			((1<<5)| 1) //!< DMA2
#define ID_PERIPH_DMA1			((1<<5)| 0) //!< DMA1

// ------------------- index 2 ->   RCC_AHB2ENR  -------------------------------
#define ID_PERIPH_SRAM3		 	((2<<5)|31) //!< SRAM3
#define ID_PERIPH_SRAM2		 	((2<<5)|30) //!< SRAM2
#define ID_PERIPH_SRAM1		 	((2<<5)|29) //!< SRAM1
#define ID_PERIPH_SDMMC2		((2<<5)| 9) //!< SDMMC2
#define ID_PERIPH_RNG	 		((2<<5)| 6) //!< RNG
#define ID_PERIPH_HASH	 		((2<<5)| 5) //!< HASH
#define ID_PERIPH_CRYPT		 	((2<<5)| 4) //!< CRYPT
#define ID_PERIPH_DCMI	 		((2<<5)| 0) //!< DCMI

// ------------------- index 3 ->   RCC_AHB4ENR  -------------------------------
#define ID_PERIPH_BKPRAM		((3<<5)|28) //!< Backup RAM
#define ID_PERIPH_HSEM	 		((3<<5)|25) //!< HSEM
#define ID_PERIPH_ADC3	 		((3<<5)|24) //!< ADC3
#define ID_PERIPH_BDMA	 		((3<<5)|21) //!< BDMA and DMAMUX2
#define ID_PERIPH_CRC			((3<<5)|19) //!< CRC
#define ID_PERIPH_GPIOK			((3<<5)|10) //!< GPIOK
#define ID_PERIPH_GPIOJ			((3<<5)| 9) //!< GPIOJ
#define ID_PERIPH_GPIOI			((3<<5)| 8) //!< GPIOI
#define ID_PERIPH_GPIOH			((3<<5)| 7) //!< GPIOH
#define ID_PERIPH_GPIOG			((3<<5)| 6) //!< GPIOG
#define ID_PERIPH_GPIOF			((3<<5)| 5) //!< GPIOF
#define ID_PERIPH_GPIOE			((3<<5)| 4) //!< GPIOE
#define ID_PERIPH_GPIOD			((3<<5)| 3) //!< GPIOD
#define ID_PERIPH_GPIOC			((3<<5)| 2) //!< GPIOC
#define ID_PERIPH_GPIOB			((3<<5)| 1) //!< GPIOB
#define ID_PERIPH_GPIOA			((3<<5)| 0) //!< GPIOA

// ------------------- index 4 ->   RCC_APB3ENR  -------------------------------
#define ID_PERIPH_WWDG1			((4<<5)| 6) //!< WWDG1
#define ID_PERIPH_LTDC			((4<<5)| 3) //!< LTDC

// ------------------- index 5 ->   RCC_APB1LENR -------------------------------
#define ID_PERIPH_UART8 		((5<<5)|31) //!< UART8
#define ID_PERIPH_UART7 		((5<<5)|30) //!< UART7
#define ID_PERIPH_DAC12 		((5<<5)|29) //!< DAC1
#define ID_PERIPH_CEC	 		((5<<5)|27) //!< HDMI-CEC
#define ID_PERIPH_I2C3	 		((5<<5)|23) //!< I2C3
#define ID_PERIPH_I2C2	 		((5<<5)|22) //!< I2C2
#define ID_PERIPH_I2C1	 		((5<<5)|21) //!< I2C1
#define ID_PERIPH_UART5 		((5<<5)|20) //!< UART5
#define ID_PERIPH_UART4 		((5<<5)|19) //!< UART4
#define ID_PERIPH_USART3 		((5<<5)|18) //!< USART3
#define ID_PERIPH_USART2 		((5<<5)|17) //!< USART2
#define ID_PERIPH_SPDIFRX	 	((5<<5)|16) //!< SPDIFRX
#define ID_PERIPH_SPI3	 		((5<<5)|15) //!< SPI3
#define ID_PERIPH_SPI2	 		((5<<5)|14) //!< SPI2
#define ID_PERIPH_LPTIM1 		((5<<5)| 9) //!< LPTIM1
#define ID_PERIPH_TIM14 		((5<<5)| 8) //!< TIM14
#define ID_PERIPH_TIM13 		((5<<5)| 7) //!< TIM13
#define ID_PERIPH_TIM12 		((5<<5)| 6) //!< TIM12
#define ID_PERIPH_TIM7	 		((5<<5)| 5) //!< TIM7
#define ID_PERIPH_TIM6	 		((5<<5)| 4) //!< TIM6
#define ID_PERIPH_TIM5	 		((5<<5)| 3) //!< TIM5
#define ID_PERIPH_TIM4	 		((5<<5)| 2) //!< TIM4
#define ID_PERIPH_TIM3	 		((5<<5)| 1) //!< TIM3
#define ID_PERIPH_TIM2	 		((5<<5)| 0) //!< TIM2

// ------------------- index 6 ->   RCC_APB1HENR -------------------------------
#define ID_PERIPH_FDCAN			((6<<5)| 8) //!< FDCAN
#define ID_PERIPH_MDIOS			((6<<5)| 5) //!< MDIOS
#define ID_PERIPH_OPAMP			((6<<5)| 4) //!< OPAMP
#define ID_PERIPH_SWP			((6<<5)| 2) //!< SWPMI
#define ID_PERIPH_CRS			((6<<5)| 1) //!< Clock Recovery System

// ------------------- index 7 ->   RCC_APB2ENR  -------------------------------
#define ID_PERIPH_HRTIM 		((7<<5)|29) //!< HRTIM
#define ID_PERIPH_DFSDM1		((7<<5)|28) //!< DFSDM1
#define ID_PERIPH_SAI3 			((7<<5)|24) //!< SAI3
#define ID_PERIPH_SAI2 			((7<<5)|23) //!< SAI2
#define ID_PERIPH_SAI1 			((7<<5)|22) //!< SAI1
#define ID_PERIPH_SPI5 			((7<<5)|20) //!< SPI5
#define ID_PERIPH_TIM17 		((7<<5)|18) //!< TIM17
#define ID_PERIPH_TIM16 		((7<<5)|17) //!< TIM16
#define ID_PERIPH_TIM15 		((7<<5)|16) //!< TIM15
#define ID_PERIPH_SPI4 			((7<<5)|13) //!< SPI4
#define ID_PERIPH_SPI1 			((7<<5)|12) //!< SPI1
#define ID_PERIPH_USART6		((7<<5)| 5) //!< USART6
#define ID_PERIPH_USART1		((7<<5)| 4) //!< USART1
#define ID_PERIPH_TIM8 			((7<<5)| 1) //!< TIM8
#define ID_PERIPH_TIM1 			((7<<5)| 0) //!< TIM1

// ------------------- index 8 ->   RCC_APB4ENR  -------------------------------
#define ID_PERIPH_SAI4			((8<<5)|21) //!< SAI4
#define ID_PERIPH_RTCAPB 		((8<<5)|16) //!< RTC APB
#define ID_PERIPH_VREF			((8<<5)|15) //!< VREF
#define ID_PERIPH_COMP12 		((8<<5)|14) //!< COMP1/2
#define ID_PERIPH_LPTIM5 		((8<<5)|12) //!< LPTIM5
#define ID_PERIPH_LPTIM4 		((8<<5)|11) //!< LPTIM4
#define ID_PERIPH_LPTIM3 		((8<<5)|10) //!< LPTIM3
#define ID_PERIPH_LPTIM2 		((8<<5)| 9) //!< LPTIM2
#define ID_PERIPH_I2C4			((8<<5)| 7) //!< I2C4
#define ID_PERIPH_SPI6			((8<<5)| 5) //!< SPI6
#define ID_PERIPH_LPUART1		((8<<5)| 3) //!< LPUART1
#define ID_PERIPH_SYSCFG 		((8<<5)| 1) //!< SYSCFG


#define ID_NO_PERIPH 		(1023)		//!< invalid index for peripheral
/** @}*/


/** Number of USB endpoints */
#define CHIP_USB_NUMENDPOINTS 6		//the USB HS has 1 control, 5 IN and 5 OUT endpoints

#endif /* MCU_CPP_H_ */
