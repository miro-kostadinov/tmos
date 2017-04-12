/**************************************************************************//**
 * @ingroup	hardware_stm32l1_rcc
 * @file	hardware/stm32/stm32l1/rcc_l1.h
 * @brief  	STM32L1 RCC
 * @date    25. October 2012
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32l1_rcc  Reset and Clock Control (RCC)
 * @ingroup	 hardware_stm32l1
 * Source and definitions for STM32L1 Reset and Clock Control (RCC)
 * @{
 *
 ******************************************************************************/

#ifndef RCC_F1_H_
#define RCC_F1_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Reset and Clock Control registers
 ******************************************************************************/
typedef struct
{
  __IO uint32_t RCC_CR;				//!< (rcc Offset: 0x00) Clock control register
  __IO uint32_t RCC_CFGR;			//!< (rcc offset: 0x04) Clock configuration register
  __IO uint32_t RCC_CIR;			//!< (rcc Offset: 0x08) Clock interrupt register
  __IO uint32_t RCC_APB2RSTR;		//!< (rcc Offset: 0x0C) APB2 peripheral reset register
  __IO uint32_t RCC_APB1RSTR;		//!< (rcc Offset: 0x10) APB1 peripheral reset register
  __IO uint32_t RCC_AHBENR;			//!< (rcc Offset: 0x14) AHB Peripheral Clock enable register
  __IO uint32_t RCC_APB2ENR;		//!< (rcc Offset: 0x18) APB2 peripheral clock enable register
  __IO uint32_t RCC_APB1ENR;		//!< (rcc Offset: 0x1C) APB1 peripheral clock enable register
  __IO uint32_t RCC_BDCR;			//!< (rcc Offset: 0x20) Backup domain control register
  __IO uint32_t RCC_CSR;			//!< (rcc Offset: 0x24) Control/status register

#ifdef STM32F10X_CL
  __IO uint32_t RCC_AHBRSTR;		//!< (rcc Offset: 0x28) AHB peripheral clock reset register
  __IO uint32_t RCC_CFGR2;			//!< (rcc Offset: 0x2C) Clock configuration register2
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  uint32_t 		RESERVED0;			//!< (rcc Offset: 0x28) reserved
  __IO uint32_t RCC_CFGR2;			//!< (rcc Offset: 0x2C) Clock configuration register2
#endif /* STM32F10X_LD_VL || STM32F10X_MD_VL || STM32F10X_HD_VL */
} RCC_TypeDef;

/**************************************************************************//***
 * @relates RCC_TypeDef
 * @{
 ******************************************************************************/

/** @name RCC_CR:	 	(rcc Offset: 0x00) Clock control register			  */
/** @{ */
#define RCC_CR_HSION        	0x00000001 //!< Internal High Speed clock enable
#define RCC_CR_HSIRDY           0x00000002 //!< Internal High Speed clock ready flag
#define RCC_CR_HSITRIM          0x000000F8 //!< Internal High Speed clock trimming
#define RCC_CR_HSICAL           0x0000FF00 //!< Internal High Speed clock Calibration
#define RCC_CR_HSEON            0x00010000 //!< External High Speed clock enable
#define RCC_CR_HSERDY           0x00020000 //!< External High Speed clock ready flag
#define RCC_CR_HSEBYP           0x00040000 //!< External High Speed clock Bypass
#define RCC_CR_CSSON            0x00080000 //!< Clock Security System enable
#define RCC_CR_PLLON            0x01000000 //!< PLL enable
#define RCC_CR_PLLRDY           0x02000000 //!< PLL clock ready flag
/** @} */

/** @name RCC_CFGR:		(rcc offset: 0x04) Clock configuration register 	  */
/** @{ */
/*!< SW configuration */
#define RCC_CFGR_SW    			0x00000003 //!< SW[1:0] bits (System clock Switch)
#define RCC_CFGR_SW_HSI       	0x00000000 //!< HSI selected as system clock
#define RCC_CFGR_SW_HSE         0x00000001 //!< HSE selected as system clock
#define RCC_CFGR_SW_PLL         0x00000002 //!< PLL selected as system clock

/*!< SWS configuration */
#define RCC_CFGR_SWS            0x0000000C //!< SWS[1:0] bits (System Clock Switch Status)
#define RCC_CFGR_SWS_HSI        0x00000000 //!< HSI oscillator used as system clock
#define RCC_CFGR_SWS_HSE        0x00000004 //!< HSE oscillator used as system clock
#define RCC_CFGR_SWS_PLL        0x00000008 //!< PLL used as system clock

/*!< HPRE configuration */
#define RCC_CFGR_HPRE           0x000000F0 //!< HPRE[3:0] bits (AHB prescaler)
#define RCC_CFGR_HPRE_DIV1      0x00000000 //!< SYSCLK not divided
#define RCC_CFGR_HPRE_DIV2      0x00000080 //!< SYSCLK divided by 2
#define RCC_CFGR_HPRE_DIV4      0x00000090 //!< SYSCLK divided by 4
#define RCC_CFGR_HPRE_DIV8      0x000000A0 //!< SYSCLK divided by 8
#define RCC_CFGR_HPRE_DIV16     0x000000B0 //!< SYSCLK divided by 16
#define RCC_CFGR_HPRE_DIV64     0x000000C0 //!< SYSCLK divided by 64
#define RCC_CFGR_HPRE_DIV128    0x000000D0 //!< SYSCLK divided by 128
#define RCC_CFGR_HPRE_DIV256    0x000000E0 //!< SYSCLK divided by 256
#define RCC_CFGR_HPRE_DIV512    0x000000F0 //!< SYSCLK divided by 512

/*!< PPRE configuration */
#define RCC_CFGR_PPRE           0x00000700 //!< PRE[2:0] bits (APB prescaler)
#define RCC_CFGR_PPRE_DIV1      0x00000000 //!< HCLK not divided
#define RCC_CFGR_PPRE_DIV2      0x00000400 //!< HCLK divided by 2
#define RCC_CFGR_PPRE_DIV4      0x00000500 //!< HCLK divided by 4
#define RCC_CFGR_PPRE_DIV8      0x00000600 //!< HCLK divided by 8
#define RCC_CFGR_PPRE_DIV16     0x00000700 //!< HCLK divided by 16

/*!< ADCPPRE configuration */
#define RCC_CFGR_ADCPRE         0x00004000 //!< ADCPRE bit (ADC prescaler)
#define RCC_CFGR_ADCPRE_DIV2    0x00000000 //!< PCLK divided by 2
#define RCC_CFGR_ADCPRE_DIV4    0x00004000 //!< PCLK divided by 4

#define RCC_CFGR_PLLSRC         0x00010000  //!< PLL entry clock source
#define RCC_CFGR_PLLSRC_HSI_Div2 0x00000000 //!< HSI clock divided by 2 selected as PLL entry clock source
#define RCC_CFGR_PLLSRC_PREDIV1 0x00010000  //!< PREDIV1 clock selected as PLL entry clock source

#define RCC_CFGR_PLLXTPRE       0x00020000 //!< HSE divider for PLL entry
#define RCC_CFGR_PLLXTPRE_PREDIV1       0x00000000 //!< PREDIV1 clock not divided for PLL entry
#define RCC_CFGR_PLLXTPRE_PREDIV1_Div2  0x00020000 //!< PREDIV1 clock divided by 2 for PLL entry

/*!< PLLMUL configuration */
#define RCC_CFGR_PLLMULL        0x003C0000 //!< PLLMUL[3:0] bits (PLL multiplication factor)
#define RCC_CFGR_PLLMULL2       0x00000000 //!< PLL input clock*2
#define RCC_CFGR_PLLMULL3       0x00040000 //!< PLL input clock*3
#define RCC_CFGR_PLLMULL4       0x00080000 //!< PLL input clock*4
#define RCC_CFGR_PLLMULL5       0x000C0000 //!< PLL input clock*5
#define RCC_CFGR_PLLMULL6       0x00100000 //!< PLL input clock*6
#define RCC_CFGR_PLLMULL7       0x00140000 //!< PLL input clock*7
#define RCC_CFGR_PLLMULL8       0x00180000 //!< PLL input clock*8
#define RCC_CFGR_PLLMULL9       0x001C0000 //!< PLL input clock*9
#define RCC_CFGR_PLLMULL10      0x00200000 //!< PLL input clock10
#define RCC_CFGR_PLLMULL11      0x00240000 //!< PLL input clock*11
#define RCC_CFGR_PLLMULL12      0x00280000 //!< PLL input clock*12
#define RCC_CFGR_PLLMULL13      0x002C0000 //!< PLL input clock*13
#define RCC_CFGR_PLLMULL14      0x00300000 //!< PLL input clock*14
#define RCC_CFGR_PLLMULL15      0x00340000 //!< PLL input clock*15
#define RCC_CFGR_PLLMULL16      0x00380000 //!< PLL input clock*16

/*!< MCO configuration */
#define RCC_CFGR_MCO            0x07000000 //!< MCO[2:0] bits (Microcontroller Clock Output)
#define RCC_CFGR_MCO_NOCLOCK    0x00000000 //!< No clock
#define RCC_CFGR_MCO_HSI14      0x01000000 //!< HSI14 clock selected as MCO source
#define RCC_CFGR_MCO_LSI        0x02000000 //!< LSI clock selected as MCO source
#define RCC_CFGR_MCO_LSE        0x03000000 //!< LSE clock selected as MCO source
#define RCC_CFGR_MCO_SYSCLK     0x04000000 //!< System clock selected as MCO source
#define RCC_CFGR_MCO_HSI        0x05000000 //!< HSI clock selected as MCO source
#define RCC_CFGR_MCO_HSE        0x06000000 //!< HSE clock selected as MCO source
#define RCC_CFGR_MCO_PLL        0x07000000 //!< PLL clock divided by 2 selected as MCO source
/** @} */

/** @name RCC_CIR:		(rcc Offset: 0x08) Clock interrupt register			  */
/** @{ */
#define RCC_CIR_LSIRDYF         0x00000001 //!< LSI Ready Interrupt flag
#define RCC_CIR_LSERDYF         0x00000002 //!< LSE Ready Interrupt flag
#define RCC_CIR_HSIRDYF         0x00000004 //!< HSI Ready Interrupt flag
#define RCC_CIR_HSERDYF         0x00000008 //!< HSE Ready Interrupt flag
#define RCC_CIR_PLLRDYF         0x00000010 //!< PLL Ready Interrupt flag
#define RCC_CIR_HSI14RDYF       0x00000020 //!< HSI14 Ready Interrupt flag
#define RCC_CIR_CSSF            0x00000080 //!< Clock Security System Interrupt flag
#define RCC_CIR_LSIRDYIE        0x00000100 //!< LSI Ready Interrupt Enable
#define RCC_CIR_LSERDYIE        0x00000200 //!< LSE Ready Interrupt Enable
#define RCC_CIR_HSIRDYIE        0x00000400 //!< HSI Ready Interrupt Enable
#define RCC_CIR_HSERDYIE        0x00000800 //!< HSE Ready Interrupt Enable
#define RCC_CIR_PLLRDYIE        0x00001000 //!< PLL Ready Interrupt Enable
#define RCC_CIR_HSI14RDYIE      0x00002000 //!< HSI14 Ready Interrupt Enable
#define RCC_CIR_LSIRDYC         0x00010000 //!< LSI Ready Interrupt Clear
#define RCC_CIR_LSERDYC         0x00020000 //!< LSE Ready Interrupt Clear
#define RCC_CIR_HSIRDYC         0x00040000 //!< HSI Ready Interrupt Clear
#define RCC_CIR_HSERDYC         0x00080000 //!< HSE Ready Interrupt Clear
#define RCC_CIR_PLLRDYC         0x00100000 //!< PLL Ready Interrupt Clear
#define RCC_CIR_HSI14RDYC       0x00200000 //!< HSI14 Ready Interrupt Clear
#define RCC_CIR_CSSC            0x00800000 //!< Clock Security System Interrupt Clear
/** @} */

/** @name RCC_APB2RSTR:	(rcc Offset: 0x0C) APB2 peripheral reset register	  */
/** @{ */
#define RCC_APB2RSTR_SYSCFGRST  0x00000001 //!< SYSCFG clock reset
#define RCC_APB2RSTR_ADC1RST    0x00000200 //!< ADC1 clock reset
#define RCC_APB2RSTR_TIM1RST    0x00000800 //!< TIM1 clock reset
#define RCC_APB2RSTR_SPI1RST    0x00001000 //!< SPI1 clock reset
#define RCC_APB2RSTR_USART1RST  0x00004000 //!< USART1 clock reset
#define RCC_APB2RSTR_TIM15RST   0x00010000 //!< TIM15 clock reset
#define RCC_APB2RSTR_TIM16RST   0x00020000 //!< TIM16 clock reset
#define RCC_APB2RSTR_TIM17RST   0x00040000 //!< TIM17 clock reset
#define RCC_APB2RSTR_DBGMCURST  0x00400000 //!< DBGMCU clock reset
/** @} */

/** @name RCC_APB1RSTR:	(rcc Offset: 0x10) APB1 peripheral reset register	  */
/** @{ */
#define RCC_APB1RSTR_TIM2RST    0x00000001 //!< Timer 2 clock reset
#define RCC_APB1RSTR_TIM3RST    0x00000002 //!< Timer 3 clock reset
#define RCC_APB1RSTR_TIM6RST    0x00000010 //!< Timer 6 clock reset
#define RCC_APB1RSTR_TIM14RST   0x00000100 //!< Timer 14 clock reset
#define RCC_APB1RSTR_WWDGRST    0x00000800 //!< Window Watchdog clock reset
#define RCC_APB1RSTR_SPI2RST    0x00004000 //!< SPI2 clock reset
#define RCC_APB1RSTR_USART2RST  0x00020000 //!< USART 2 clock reset
#define RCC_APB1RSTR_I2C1RST    0x00200000 //!< I2C 1 clock reset
#define RCC_APB1RSTR_I2C2RST    0x00400000 //!< I2C 2 clock reset
#define RCC_APB1RSTR_PWRRST     0x10000000 //!< PWR clock reset
#define RCC_APB1RSTR_DACRST     0x20000000 //!< DAC clock reset
#define RCC_APB1RSTR_CECRST     0x40000000 //!< CEC clock reset
/** @} */

/** @name RCC_AHBENR:	(rcc Offset: 0x14) AHB Peripheral Clock enable register */
/** @{ */
#define RCC_AHBENR_DMA1EN       0x00000001 //!< DMA1 clock enable
#define RCC_AHBENR_SRAMEN       0x00000004 //!< SRAM interface clock enable
#define RCC_AHBENR_FLITFEN      0x00000010 //!< FLITF clock enable
#define RCC_AHBENR_CRCEN        0x00000040 //!< CRC clock enable
#define RCC_AHBENR_GPIOAEN      0x00020000 //!< GPIOA clock enable
#define RCC_AHBENR_GPIOBEN      0x00040000 //!< GPIOB clock enable
#define RCC_AHBENR_GPIOCEN      0x00080000 //!< GPIOC clock enable
#define RCC_AHBENR_GPIODEN      0x00100000 //!< GPIOD clock enable
#define RCC_AHBENR_GPIOFEN      0x00400000 //!< GPIOF clock enable
#define RCC_AHBENR_TSEN         0x01000000 //!< TS clock enable
/** @} */

/** @name RCC_APB2ENR:	(rcc Offset: 0x18) APB2 peripheral clock enable register */
/** @{ */
#define RCC_APB2ENR_SYSCFGEN    0x00000001 //!< SYSCFG clock enable
#define RCC_APB2ENR_ADC1EN      0x00000200 //!< ADC1 clock enable
#define RCC_APB2ENR_TIM1EN      0x00000800 //!< TIM1 clock enable
#define RCC_APB2ENR_SPI1EN      0x00001000 //!< SPI1 clock enable
#define RCC_APB2ENR_USART1EN    0x00004000 //!< USART1 clock enable
#define RCC_APB2ENR_TIM15EN     0x00010000 //!< TIM15 clock enable
#define RCC_APB2ENR_TIM16EN     0x00020000 //!< TIM16 clock enable
#define RCC_APB2ENR_TIM17EN     0x00040000 //!< TIM17 clock enable
#define RCC_APB2ENR_DBGMCUEN    0x00400000 //!< DBGMCU clock enable
/** @} */

/** @name RCC_APB1ENR:	(rcc Offset: 0x1C) APB1 peripheral clock enable register */
/** @{ */
#define RCC_APB1ENR_TIM2EN      0x00000001 //!< Timer 2 clock enable
#define RCC_APB1ENR_TIM3EN      0x00000002 //!< Timer 3 clock enable
#define RCC_APB1ENR_TIM6EN      0x00000010 //!< Timer 6 clock enable
#define RCC_APB1ENR_TIM14EN     0x00000100 //!< Timer 14 clock enable
#define RCC_APB1ENR_WWDGEN      0x00000800 //!< Window Watchdog clock enable
#define RCC_APB1ENR_SPI2EN      0x00004000 //!< SPI2 clock enable
#define RCC_APB1ENR_USART2EN    0x00020000 //!< USART2 clock enable
#define RCC_APB1ENR_I2C1EN      0x00200000 //!< I2C1 clock enable
#define RCC_APB1ENR_I2C2EN      0x00400000 //!< I2C2 clock enable
#define RCC_APB1ENR_PWREN       0x10000000 //!< PWR clock enable
#define RCC_APB1ENR_DACEN       0x20000000 //!< DAC clock enable
#define RCC_APB1ENR_CECEN       0x40000000 //!< CEC clock enable
/** @} */

/** @name RCC_BDCR:		(rcc Offset: 0x20) Backup domain control register 	  */
/** @{ */
#define RCC_BDCR_LSEON          0x00000001 //!< External Low Speed oscillator enable
#define RCC_BDCR_LSERDY         0x00000002 //!< External Low Speed oscillator Ready
#define RCC_BDCR_LSEBYP         0x00000004 //!< External Low Speed oscillator Bypass

#define RCC_BDCR_LSEDRV         0x00000018 //!< LSEDRV[1:0] bits (LSE Osc. drive capability)

/*!< RTC congiguration */
#define RCC_BDCR_RTCSEL         0x00000300 //!< RTCSEL[1:0] bits (RTC clock source selection)
#define RCC_BDCR_RTCSEL_NOCLOCK 0x00000000 //!< No clock
#define RCC_BDCR_RTCSEL_LSE     0x00000100 //!< LSE oscillator clock used as RTC clock
#define RCC_BDCR_RTCSEL_LSI     0x00000200 //!< LSI oscillator clock used as RTC clock
#define RCC_BDCR_RTCSEL_HSE     0x00000300 //!< HSE oscillator clock divided by 128 used as RTC clock

#define RCC_BDCR_RTCEN          0x00008000 //!< RTC clock enable
#define RCC_BDCR_BDRST          0x00010000 //!< Backup domain software reset
/** @} */

/** @name RCC_CSR:		(rcc Offset: 0x24) Control/status register			  */
/** @{ */
#define RCC_CSR_LSION           0x00000001 //!< Internal Low Speed oscillator enable
#define RCC_CSR_LSIRDY          0x00000002 //!< Internal Low Speed oscillator Ready
#define RCC_CSR_V18PWRRSTF      0x00800000 //!< V1.8 power domain reset flag
#define RCC_CSR_RMVF            0x01000000 //!< Remove reset flag
#define RCC_CSR_OBL             0x02000000 //!< OBL reset flag
#define RCC_CSR_PINRSTF         0x04000000 //!< PIN reset flag
#define RCC_CSR_PORRSTF         0x08000000 //!< POR/PDR reset flag
#define RCC_CSR_SFTRSTF         0x10000000 //!< Software Reset flag
#define RCC_CSR_IWDGRSTF        0x20000000 //!< Independent Watchdog reset flag
#define RCC_CSR_WWDGRSTF        0x40000000 //!< Window watchdog reset flag
#define RCC_CSR_LPWRRSTF        0x80000000 //!< Low-Power reset flag
/** @} */

/** @name RCC_AHBRSTR:	(rcc Offset: 0x28) AHB peripheral clock reset register */
/** @{ */
#define RCC_AHBRSTR_GPIOARST    0x00020000 //!< GPIOA clock reset
#define RCC_AHBRSTR_GPIOBRST    0x00040000 //!< GPIOB clock reset
#define RCC_AHBRSTR_GPIOCRST    0x00080000 //!< GPIOC clock reset
#define RCC_AHBRSTR_GPIODRST    0x00010000 //!< GPIOD clock reset
#define RCC_AHBRSTR_GPIOFRST    0x00040000 //!< GPIOF clock reset
#define RCC_AHBRSTR_TSRST       0x00100000 //!< TS clock reset
/** @} */

/** @name RCC_CFGR2:	(rcc Offset: 0x2C) Clock configuration register2	  */
/** @{ */
/*!< PREDIV1 configuration */
#define RCC_CFGR2_PREDIV1       0x0000000F //!< PREDIV1[3:0] bits */
#define RCC_CFGR2_PREDIV1_DIV1  0x00000000 //!< PREDIV1 input clock not divided
#define RCC_CFGR2_PREDIV1_DIV2  0x00000001 //!< PREDIV1 input clock divided by 2
#define RCC_CFGR2_PREDIV1_DIV3  0x00000002 //!< PREDIV1 input clock divided by 3
#define RCC_CFGR2_PREDIV1_DIV4  0x00000003 //!< PREDIV1 input clock divided by 4
#define RCC_CFGR2_PREDIV1_DIV5  0x00000004 //!< PREDIV1 input clock divided by 5
#define RCC_CFGR2_PREDIV1_DIV6  0x00000005 //!< PREDIV1 input clock divided by 6
#define RCC_CFGR2_PREDIV1_DIV7  0x00000006 //!< PREDIV1 input clock divided by 7
#define RCC_CFGR2_PREDIV1_DIV8  0x00000007 //!< PREDIV1 input clock divided by 8
#define RCC_CFGR2_PREDIV1_DIV9  0x00000008 //!< PREDIV1 input clock divided by 9
#define RCC_CFGR2_PREDIV1_DIV10 0x00000009 //!< PREDIV1 input clock divided by 10
#define RCC_CFGR2_PREDIV1_DIV11 0x0000000A //!< PREDIV1 input clock divided by 11
#define RCC_CFGR2_PREDIV1_DIV12 0x0000000B //!< PREDIV1 input clock divided by 12
#define RCC_CFGR2_PREDIV1_DIV13 0x0000000C //!< PREDIV1 input clock divided by 13
#define RCC_CFGR2_PREDIV1_DIV14 0x0000000D //!< PREDIV1 input clock divided by 14
#define RCC_CFGR2_PREDIV1_DIV15 0x0000000E //!< PREDIV1 input clock divided by 15
#define RCC_CFGR2_PREDIV1_DIV16 0x0000000F //!< PREDIV1 input clock divided by 16
/** @} */



///*******************  Bit definition for RCC_CFGR3 register  ******************/
///*!< USART1 Clock source selection */
//#define RCC_CFGR3_USART1SW      0x00000003 //!< USART1SW[1:0] bits */
//#define RCC_CFGR3_USART1SW_0    0x00000001 //!< Bit 0 */
//#define RCC_CFGR3_USART1SW_1    0x00000002 //!< Bit 1 */
///*!< I2C1 Clock source selection */
//#define RCC_CFGR3_I2C1SW        0x00000010 //!< I2C1SW bits */
//#define RCC_CFGR3_CECSW         0x00000040 //!< CECSW bits */
//#define RCC_CFGR3_ADCSW         0x00000100 //!< ADCSW bits */
//
///*******************  Bit definition for RCC_CR2 register  ********************/
//#define RCC_CR2_HSI14ON         0x00000001 //!< Internal High Speed 14MHz clock enable */
//#define RCC_CR2_HSI14RDY        0x00000002 //!< Internal High Speed 14MHz clock ready flag */
//#define RCC_CR2_HSI14DIS        0x00000004 //!< Internal High Speed 14MHz clock disable */
//#define RCC_CR2_HSI14TRIM       0x000000F8 //!< Internal High Speed 14MHz clock trimming */
//#define RCC_CR2_HSI14CAL        0x0000FF00 //!< Internal High Speed 14MHz clock Calibration */

/** @} */ // @relates RCC_TypeDef

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) Startup
   Timeout value
   */
#define HSE_STARTUP_TIMEOUT   0x0500 /*!< Time out for HSE start up */


#endif /* RCC_F1_H_ */

/** @} */
