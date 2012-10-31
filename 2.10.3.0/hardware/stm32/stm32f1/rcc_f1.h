/*
 * rcc_f1.h
 *
 *  Created on: Oct 25, 2012
 *      Author: miro
 */

#ifndef RCC_F1_H_
#define RCC_F1_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
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

/*******************************************************************************
 * @defgroup RCC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup RCC_CR:	 	(rcc Offset: 0x00) Clock control register		  */
#define RCC_CR_HSION        		0x00000001 //!< Internal High Speed clock enable
#define RCC_CR_HSIRDY           	0x00000002 //!< Internal High Speed clock ready flag
#define RCC_CR_HSITRIM          	0x000000F8 //!< Internal High Speed clock trimming
#define RCC_CR_HSICAL           	0x0000FF00 //!< Internal High Speed clock Calibration
#define RCC_CR_HSEON            	0x00010000 //!< External High Speed clock enable
#define RCC_CR_HSERDY           	0x00020000 //!< External High Speed clock ready flag
#define RCC_CR_HSEBYP           	0x00040000 //!< External High Speed clock Bypass
#define RCC_CR_CSSON            	0x00080000 //!< Clock Security System enable
#define RCC_CR_PLLON            	0x01000000 //!< PLL enable
#define RCC_CR_PLLRDY           	0x02000000 //!< PLL clock ready flag
#ifdef STM32F10X_CL
 #define RCC_CR_PLL2ON          	0x04000000 //!< PLL2 enable
 #define RCC_CR_PLL2RDY         	0x08000000 //!< PLL2 clock ready flag
 #define RCC_CR_PLL3ON          	0x10000000 //!< PLL3 enable
 #define RCC_CR_PLL3RDY         	0x20000000 //!< PLL3 clock ready flag
#endif /* STM32F10X_CL */
/** @} */

/** @defgroup RCC_CFGR:		(rcc offset: 0x04) Clock configuration register   */
/*!< SW configuration */
#define RCC_CFGR_SW    				0x00000003 //!< SW[1:0] bits (System clock Switch)
#define RCC_CFGR_SW_HSI       		0x00000000 //!< HSI selected as system clock
#define RCC_CFGR_SW_HSE         	0x00000001 //!< HSE selected as system clock
#define RCC_CFGR_SW_PLL         	0x00000002 //!< PLL selected as system clock

/*!< SWS configuration */
#define RCC_CFGR_SWS            	0x0000000C //!< SWS[1:0] bits (System Clock Switch Status)
#define RCC_CFGR_SWS_HSI        	0x00000000 //!< HSI oscillator used as system clock
#define RCC_CFGR_SWS_HSE        	0x00000004 //!< HSE oscillator used as system clock
#define RCC_CFGR_SWS_PLL        	0x00000008 //!< PLL used as system clock

/*!< HPRE configuration */
#define RCC_CFGR_HPRE           	0x000000F0 //!< HPRE[3:0] bits (AHB prescaler)
#define RCC_CFGR_HPRE_DIV1      	0x00000000 //!< SYSCLK not divided
#define RCC_CFGR_HPRE_DIV2      	0x00000080 //!< SYSCLK divided by 2
#define RCC_CFGR_HPRE_DIV4      	0x00000090 //!< SYSCLK divided by 4
#define RCC_CFGR_HPRE_DIV8      	0x000000A0 //!< SYSCLK divided by 8
#define RCC_CFGR_HPRE_DIV16     	0x000000B0 //!< SYSCLK divided by 16
#define RCC_CFGR_HPRE_DIV64     	0x000000C0 //!< SYSCLK divided by 64
#define RCC_CFGR_HPRE_DIV128    	0x000000D0 //!< SYSCLK divided by 128
#define RCC_CFGR_HPRE_DIV256    	0x000000E0 //!< SYSCLK divided by 256
#define RCC_CFGR_HPRE_DIV512    	0x000000F0 //!< SYSCLK divided by 512

/*!< PPRE1 configuration */
#define RCC_CFGR_PPRE1          	0x00000700 //!< PRE1[2:0] bits (APB1 prescaler)
#define RCC_CFGR_PPRE1_DIV1     	0x00000000 //!< HCLK not divided
#define RCC_CFGR_PPRE1_DIV2     	0x00000400 //!< HCLK divided by 2
#define RCC_CFGR_PPRE1_DIV4     	0x00000500 //!< HCLK divided by 4
#define RCC_CFGR_PPRE1_DIV8     	0x00000600 //!< HCLK divided by 8
#define RCC_CFGR_PPRE1_DIV16    	0x00000700 //!< HCLK divided by 16

/*!< PPRE2 configuration */
#define RCC_CFGR_PPRE2          	0x00003800 //!< PRE2[2:0] bits (APB2 prescaler)
#define RCC_CFGR_PPRE2_DIV1     	0x00000000 //!< HCLK not divided
#define RCC_CFGR_PPRE2_DIV2     	0x00002000 //!< HCLK divided by 2
#define RCC_CFGR_PPRE2_DIV4     	0x00002800 //!< HCLK divided by 4
#define RCC_CFGR_PPRE2_DIV8     	0x00003000 //!< HCLK divided by 8
#define RCC_CFGR_PPRE2_DIV16    	0x00003800 //!< HCLK divided by 16

/*!< ADCPPRE configuration */
#define RCC_CFGR_ADCPRE         	0x0000C000 //!< ADCPRE[1:0] bits (ADC prescaler)
#define RCC_CFGR_ADCPRE_DIV2    	0x00000000 //!< PCLK2 divided by 2
#define RCC_CFGR_ADCPRE_DIV4    	0x00004000 //!< PCLK2 divided by 4
#define RCC_CFGR_ADCPRE_DIV6    	0x00008000 //!< PCLK2 divided by 6
#define RCC_CFGR_ADCPRE_DIV8    	0x0000C000 //!< PCLK2 divided by 8

#define RCC_CFGR_PLLSRC         	0x00010000 //!< PLL entry clock source

#define RCC_CFGR_PLLXTPRE       	0x00020000 //!< HSE divider for PLL entry

/*!< PLLMUL configuration */
#define RCC_CFGR_PLLMULL        	0x003C0000 //!< PLLMUL[3:0] bits (PLL multiplication factor)


#ifdef STM32F10X_CL
 #define RCC_CFGR_PLLSRC_HSI_Div2   0x00000000 //!< HSI clock divided by 2 selected as PLL entry clock source
 #define RCC_CFGR_PLLSRC_PREDIV1    0x00010000 //!< PREDIV1 clock selected as PLL entry clock source

 #define RCC_CFGR_PLLXTPRE_PREDIV1      0x00000000 //!< PREDIV1 clock not divided for PLL entry
 #define RCC_CFGR_PLLXTPRE_PREDIV1_Div2 0x00020000 //!< PREDIV1 clock divided by 2 for PLL entry

 #define RCC_CFGR_PLLMULL4      	0x00080000 //!< PLL input clock * 4
 #define RCC_CFGR_PLLMULL5      	0x000C0000 //!< PLL input clock * 5
 #define RCC_CFGR_PLLMULL6      	0x00100000 //!< PLL input clock * 6
 #define RCC_CFGR_PLLMULL7      	0x00140000 //!< PLL input clock * 7
 #define RCC_CFGR_PLLMULL8      	0x00180000 //!< PLL input clock * 8
 #define RCC_CFGR_PLLMULL9      	0x001C0000 //!< PLL input clock * 9
 #define RCC_CFGR_PLLMULL6_5    	0x00340000 //!< PLL input clock * 6.5

 #define RCC_CFGR_OTGFSPRE      	0x00400000 //!< USB OTG FS prescaler

/*!< MCO configuration */
 #define RCC_CFGR_MCO               0x0F000000 //!< MCO[3:0] bits (Microcontroller Clock Output)
 #define RCC_CFGR_MCO_NOCLOCK       0x00000000 //!< No clock
 #define RCC_CFGR_MCO_SYSCLK        0x04000000 //!< System clock selected as MCO source
 #define RCC_CFGR_MCO_HSI           0x05000000 //!< HSI clock selected as MCO source
 #define RCC_CFGR_MCO_HSE           0x06000000 //!< HSE clock selected as MCO source
 #define RCC_CFGR_MCO_PLLCLK_Div2   0x07000000 //!< PLL clock divided by 2 selected as MCO source
 #define RCC_CFGR_MCO_PLL2CLK       0x08000000 //!< PLL2 clock selected as MCO source
 #define RCC_CFGR_MCO_PLL3CLK_Div2  0x09000000 //!< PLL3 clock divided by 2 selected as MCO source
 #define RCC_CFGR_MCO_Ext_HSE       0x0A000000 //!< XT1 external 3-25 MHz oscillator clock selected as MCO source
 #define RCC_CFGR_MCO_PLL3CLK       0x0B000000 //!< PLL3 clock selected as MCO source
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
 #define RCC_CFGR_PLLSRC_HSI_Div2   0x00000000 //!< HSI clock divided by 2 selected as PLL entry clock source
 #define RCC_CFGR_PLLSRC_PREDIV1    0x00010000 //!< PREDIV1 clock selected as PLL entry clock source

 #define RCC_CFGR_PLLXTPRE_PREDIV1       0x00000000 //!< PREDIV1 clock not divided for PLL entry
 #define RCC_CFGR_PLLXTPRE_PREDIV1_Div2  0x00020000 //!< PREDIV1 clock divided by 2 for PLL entry

 #define RCC_CFGR_PLLMULL2      	0x00000000 //!< PLL input clock*2
 #define RCC_CFGR_PLLMULL3      	0x00040000 //!< PLL input clock*3
 #define RCC_CFGR_PLLMULL4      	0x00080000 //!< PLL input clock*4
 #define RCC_CFGR_PLLMULL5      	0x000C0000 //!< PLL input clock*5
 #define RCC_CFGR_PLLMULL6      	0x00100000 //!< PLL input clock*6
 #define RCC_CFGR_PLLMULL7      	0x00140000 //!< PLL input clock*7
 #define RCC_CFGR_PLLMULL8      	0x00180000 //!< PLL input clock*8
 #define RCC_CFGR_PLLMULL9      	0x001C0000 //!< PLL input clock*9
 #define RCC_CFGR_PLLMULL10     	0x00200000 //!< PLL input clock10
 #define RCC_CFGR_PLLMULL11     	0x00240000 //!< PLL input clock*11
 #define RCC_CFGR_PLLMULL12     	0x00280000 //!< PLL input clock*12
 #define RCC_CFGR_PLLMULL13     	0x002C0000 //!< PLL input clock*13
 #define RCC_CFGR_PLLMULL14     	0x00300000 //!< PLL input clock*14
 #define RCC_CFGR_PLLMULL15     	0x00340000 //!< PLL input clock*15
 #define RCC_CFGR_PLLMULL16     	0x00380000 //!< PLL input clock*16

/*!< MCO configuration */
 #define RCC_CFGR_MCO               0x07000000 //!< MCO[2:0] bits (Microcontroller Clock Output)
 #define RCC_CFGR_MCO_NOCLOCK       0x00000000 //!< No clock
 #define RCC_CFGR_MCO_SYSCLK        0x04000000 //!< System clock selected as MCO source
 #define RCC_CFGR_MCO_HSI           0x05000000 //!< HSI clock selected as MCO source
 #define RCC_CFGR_MCO_HSE           0x06000000 //!< HSE clock selected as MCO source
 #define RCC_CFGR_MCO_PLL           0x07000000 //!< PLL clock divided by 2 selected as MCO source
#else
 #define RCC_CFGR_PLLSRC_HSI_Div2   0x00000000 //!< HSI clock divided by 2 selected as PLL entry clock source
 #define RCC_CFGR_PLLSRC_HSE        0x00010000 //!< HSE clock selected as PLL entry clock source

 #define RCC_CFGR_PLLXTPRE_HSE      0x00000000 //!< HSE clock not divided for PLL entry
 #define RCC_CFGR_PLLXTPRE_HSE_Div2 0x00020000 //!< HSE clock divided by 2 for PLL entry

 #define RCC_CFGR_PLLMULL2          0x00000000 //!< PLL input clock*2
 #define RCC_CFGR_PLLMULL3          0x00040000 //!< PLL input clock*3
 #define RCC_CFGR_PLLMULL4          0x00080000 //!< PLL input clock*4
 #define RCC_CFGR_PLLMULL5          0x000C0000 //!< PLL input clock*5
 #define RCC_CFGR_PLLMULL6          0x00100000 //!< PLL input clock*6
 #define RCC_CFGR_PLLMULL7          0x00140000 //!< PLL input clock*7
 #define RCC_CFGR_PLLMULL8          0x00180000 //!< PLL input clock*8
 #define RCC_CFGR_PLLMULL9          0x001C0000 //!< PLL input clock*9
 #define RCC_CFGR_PLLMULL10         0x00200000 //!< PLL input clock10
 #define RCC_CFGR_PLLMULL11         0x00240000 //!< PLL input clock*11
 #define RCC_CFGR_PLLMULL12         0x00280000 //!< PLL input clock*12
 #define RCC_CFGR_PLLMULL13         0x002C0000 //!< PLL input clock*13
 #define RCC_CFGR_PLLMULL14         0x00300000 //!< PLL input clock*14
 #define RCC_CFGR_PLLMULL15         0x00340000 //!< PLL input clock*15
 #define RCC_CFGR_PLLMULL16         0x00380000 //!< PLL input clock*16
 #define RCC_CFGR_USBPRE            0x00400000 //!< USB Device prescaler

/*!< MCO configuration */
 #define RCC_CFGR_MCO               0x07000000 //!< MCO[2:0] bits (Microcontroller Clock Output)
 #define RCC_CFGR_MCO_NOCLOCK       0x00000000 //!< No clock
 #define RCC_CFGR_MCO_SYSCLK        0x04000000 //!< System clock selected as MCO source
 #define RCC_CFGR_MCO_HSI           0x05000000 //!< HSI clock selected as MCO source
 #define RCC_CFGR_MCO_HSE           0x06000000 //!< HSE clock selected as MCO source
 #define RCC_CFGR_MCO_PLL           0x07000000 //!< PLL clock divided by 2 selected as MCO source
#endif /* STM32F10X_CL */

/** @} */

/** @defgroup RCC_CIR:		(rcc Offset: 0x08) Clock interrupt register		  */
#define RCC_CIR_LSIRDYF             0x00000001 //!< LSI Ready Interrupt flag
#define RCC_CIR_LSERDYF             0x00000002 //!< LSE Ready Interrupt flag
#define RCC_CIR_HSIRDYF             0x00000004 //!< HSI Ready Interrupt flag
#define RCC_CIR_HSERDYF             0x00000008 //!< HSE Ready Interrupt flag
#define RCC_CIR_PLLRDYF             0x00000010 //!< PLL Ready Interrupt flag
#define RCC_CIR_CSSF                0x00000080 //!< Clock Security System Interrupt flag
#define RCC_CIR_LSIRDYIE            0x00000100 //!< LSI Ready Interrupt Enable
#define RCC_CIR_LSERDYIE            0x00000200 //!< LSE Ready Interrupt Enable
#define RCC_CIR_HSIRDYIE            0x00000400 //!< HSI Ready Interrupt Enable
#define RCC_CIR_HSERDYIE            0x00000800 //!< HSE Ready Interrupt Enable
#define RCC_CIR_PLLRDYIE            0x00001000 //!< PLL Ready Interrupt Enable
#define RCC_CIR_LSIRDYC             0x00010000 //!< LSI Ready Interrupt Clear
#define RCC_CIR_LSERDYC             0x00020000 //!< LSE Ready Interrupt Clear
#define RCC_CIR_HSIRDYC             0x00040000 //!< HSI Ready Interrupt Clear
#define RCC_CIR_HSERDYC             0x00080000 //!< HSE Ready Interrupt Clear
#define RCC_CIR_PLLRDYC             0x00100000 //!< PLL Ready Interrupt Clear
#define RCC_CIR_CSSC                0x00800000 //!< Clock Security System Interrupt Clear

#ifdef STM32F10X_CL
 #define RCC_CIR_PLL2RDYF           0x00000020 //!< PLL2 Ready Interrupt flag
 #define RCC_CIR_PLL3RDYF           0x00000040 //!< PLL3 Ready Interrupt flag
 #define RCC_CIR_PLL2RDYIE          0x00002000 //!< PLL2 Ready Interrupt Enable
 #define RCC_CIR_PLL3RDYIE          0x00004000 //!< PLL3 Ready Interrupt Enable
 #define RCC_CIR_PLL2RDYC           0x00200000 //!< PLL2 Ready Interrupt Clear
 #define RCC_CIR_PLL3RDYC           0x00400000 //!< PLL3 Ready Interrupt Clear
#endif /* STM32F10X_CL */
/** @} */

/** @defgroup RCC_APB2RSTR:	(rcc Offset: 0x0C) APB2 peripheral reset register */
#define RCC_APB2RSTR_AFIORST        0x00000001 //!< Alternate Function I/O reset
#define RCC_APB2RSTR_IOPARST        0x00000004 //!< I/O port A reset
#define RCC_APB2RSTR_IOPBRST        0x00000008 //!< I/O port B reset
#define RCC_APB2RSTR_IOPCRST        0x00000010 //!< I/O port C reset
#define RCC_APB2RSTR_IOPDRST        0x00000020 //!< I/O port D reset
#define RCC_APB2RSTR_ADC1RST        0x00000200 //!< ADC 1 interface reset

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
#define RCC_APB2RSTR_ADC2RST        0x00000400 //!< ADC 2 interface reset
#endif

#define RCC_APB2RSTR_TIM1RST        0x00000800 //!< TIM1 Timer reset
#define RCC_APB2RSTR_SPI1RST        0x00001000 //!< SPI 1 reset
#define RCC_APB2RSTR_USART1RST      0x00004000 //!< USART1 reset

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
#define RCC_APB2RSTR_TIM15RST       0x00010000 //!< TIM15 Timer reset
#define RCC_APB2RSTR_TIM16RST       0x00020000 //!< TIM16 Timer reset
#define RCC_APB2RSTR_TIM17RST       0x00040000 //!< TIM17 Timer reset
#endif

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
 #define RCC_APB2RSTR_IOPERST       0x00000040 //!< I/O port E reset
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
 #define RCC_APB2RSTR_IOPFRST       0x00000080 //!< I/O port F reset
 #define RCC_APB2RSTR_IOPGRST       0x00000100 //!< I/O port G reset
 #define RCC_APB2RSTR_TIM8RST       0x00002000 //!< TIM8 Timer reset
 #define RCC_APB2RSTR_ADC3RST       0x00008000 //!< ADC3 interface reset
#endif

#if defined (STM32F10X_HD_VL)
 #define RCC_APB2RSTR_IOPFRST       0x00000080 //!< I/O port F reset
 #define RCC_APB2RSTR_IOPGRST       0x00000100 //!< I/O port G reset
#endif

#ifdef STM32F10X_XL
 #define RCC_APB2RSTR_TIM9RST       0x00080000 //!< TIM9 Timer reset
 #define RCC_APB2RSTR_TIM10RST      0x00100000 //!< TIM10 Timer reset
 #define RCC_APB2RSTR_TIM11RST      0x00200000 //!< TIM11 Timer reset
#endif /* STM32F10X_XL */
/** @} */

/** @defgroup RCC_APB1RSTR:	(rcc Offset: 0x10) APB1 peripheral reset register */
#define RCC_APB1RSTR_TIM2RST        0x00000001 //!< Timer 2 reset
#define RCC_APB1RSTR_TIM3RST        0x00000002 //!< Timer 3 reset
#define RCC_APB1RSTR_WWDGRST        0x00000800 //!< Window Watchdog reset
#define RCC_APB1RSTR_USART2RST      0x00020000 //!< USART 2 reset
#define RCC_APB1RSTR_I2C1RST        0x00200000 //!< I2C 1 reset

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
#define RCC_APB1RSTR_CAN1RST        0x02000000 //!< CAN1 reset
#endif

#define RCC_APB1RSTR_BKPRST         0x08000000 //!< Backup interface reset
#define RCC_APB1RSTR_PWRRST         0x10000000 //!< Power interface reset

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
 #define RCC_APB1RSTR_TIM4RST       0x00000004 //!< Timer 4 reset
 #define RCC_APB1RSTR_SPI2RST       0x00004000 //!< SPI 2 reset
 #define RCC_APB1RSTR_USART3RST     0x00040000 //!< USART 3 reset
 #define RCC_APB1RSTR_I2C2RST       0x00400000 //!< I2C 2 reset
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined (STM32F10X_LD) || defined  (STM32F10X_XL)
 #define RCC_APB1RSTR_USBRST        0x00800000 //!< USB Device reset
#endif

#if defined (STM32F10X_HD) || defined  (STM32F10X_CL) || defined  (STM32F10X_XL)
 #define RCC_APB1RSTR_TIM5RST       0x00000008 //!< Timer 5 reset
 #define RCC_APB1RSTR_TIM6RST       0x00000010 //!< Timer 6 reset
 #define RCC_APB1RSTR_TIM7RST       0x00000020 //!< Timer 7 reset
 #define RCC_APB1RSTR_SPI3RST       0x00008000 //!< SPI 3 reset
 #define RCC_APB1RSTR_UART4RST      0x00080000 //!< UART 4 reset
 #define RCC_APB1RSTR_UART5RST      0x00100000 //!< UART 5 reset
 #define RCC_APB1RSTR_DACRST        0x20000000 //!< DAC interface reset
#endif

#if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
 #define RCC_APB1RSTR_TIM6RST       0x00000010 //!< Timer 6 reset
 #define RCC_APB1RSTR_TIM7RST       0x00000020 //!< Timer 7 reset
 #define RCC_APB1RSTR_DACRST        0x20000000 //!< DAC interface reset
 #define RCC_APB1RSTR_CECRST        0x40000000 //!< CEC interface reset
#endif

#if defined  (STM32F10X_HD_VL)
 #define RCC_APB1RSTR_TIM5RST       0x00000008 //!< Timer 5 reset
 #define RCC_APB1RSTR_TIM12RST      0x00000040 //!< TIM12 Timer reset
 #define RCC_APB1RSTR_TIM13RST      0x00000080 //!< TIM13 Timer reset
 #define RCC_APB1RSTR_TIM14RST      0x00000100 //!< TIM14 Timer reset
 #define RCC_APB1RSTR_SPI3RST       0x00008000 //!< SPI 3 reset
 #define RCC_APB1RSTR_UART4RST      0x00080000 //!< UART 4 reset
 #define RCC_APB1RSTR_UART5RST      0x00100000 //!< UART 5 reset
#endif

#ifdef STM32F10X_CL
 #define RCC_APB1RSTR_CAN2RST       0x04000000 //!< CAN2 reset
#endif /* STM32F10X_CL */

#ifdef STM32F10X_XL
 #define RCC_APB1RSTR_TIM12RST      0x00000040 //!< TIM12 Timer reset
 #define RCC_APB1RSTR_TIM13RST      0x00000080 //!< TIM13 Timer reset
 #define RCC_APB1RSTR_TIM14RST      0x00000100 //!< TIM14 Timer reset
#endif /* STM32F10X_XL */
/** @} */

/** @defgroup RCC_AHBENR:	(rcc Offset: 0x14) AHB Peripheral Clock enable register */
#define RCC_AHBENR_DMA1EN           0x00000001 //!< DMA1 clock enable
#define RCC_AHBENR_SRAMEN           0x00000004 //!< SRAM interface clock enable
#define RCC_AHBENR_FLITFEN          0x00000010 //!< FLITF clock enable
#define RCC_AHBENR_CRCEN            0x00000040 //!< CRC clock enable

#if defined (STM32F10X_HD) || defined  (STM32F10X_CL) || defined  (STM32F10X_HD_VL)
 #define RCC_AHBENR_DMA2EN          0x00000002 //!< DMA2 clock enable
#endif

#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
 #define RCC_AHBENR_FSMCEN          0x01000000 //!< FSMC clock enable
 #define RCC_AHBENR_SDIOEN          0x04000000 //!< SDIO clock enable
#endif

#if defined (STM32F10X_HD_VL)
 #define RCC_AHBENR_FSMCEN          0x01000000 //!< FSMC clock enable
#endif

#ifdef STM32F10X_CL
 #define RCC_AHBENR_OTGFSEN         0x00001000 //!< USB OTG FS clock enable
 #define RCC_AHBENR_ETHMACEN        0x00004000 //!< ETHERNET MAC clock enable
 #define RCC_AHBENR_ETHMACTXEN      0x00008000 //!< ETHERNET MAC Tx clock enable
 #define RCC_AHBENR_ETHMACRXEN      0x00010000 //!< ETHERNET MAC Rx clock enable
#endif /* STM32F10X_CL */
/** @} */

/** @defgroup RCC_APB2ENR:	(rcc Offset: 0x18) APB2 peripheral clock enable register */
#define RCC_APB2ENR_AFIOEN          0x00000001 //!< Alternate Function I/O clock enable
#define RCC_APB2ENR_IOPAEN          0x00000004 //!< I/O port A clock enable
#define RCC_APB2ENR_IOPBEN          0x00000008 //!< I/O port B clock enable
#define RCC_APB2ENR_IOPCEN          0x00000010 //!< I/O port C clock enable
#define RCC_APB2ENR_IOPDEN          0x00000020 //!< I/O port D clock enable
#define RCC_APB2ENR_ADC1EN          0x00000200 //!< ADC 1 interface clock enable

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
#define RCC_APB2ENR_ADC2EN          0x00000400 //!< ADC 2 interface clock enable
#endif

#define RCC_APB2ENR_TIM1EN          0x00000800 //!< TIM1 Timer clock enable /
#define RCC_APB2ENR_SPI1EN          0x00001000 //!< SPI 1 clock enable
#define RCC_APB2ENR_USART1EN        0x00004000 //!< USART1 clock enable

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
#define RCC_APB2ENR_TIM15EN         0x00010000 //!< TIM15 Timer clock enable
#define RCC_APB2ENR_TIM16EN         0x00020000 //!< TIM16 Timer clock enable
#define RCC_APB2ENR_TIM17EN         0x00040000 //!< TIM17 Timer clock enable
#endif

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
 #define RCC_APB2ENR_IOPEEN         0x00000040 //!< I/O port E clock enable
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
 #define RCC_APB2ENR_IOPFEN         0x00000080 //!< I/O port F clock enable
 #define RCC_APB2ENR_IOPGEN         0x00000100 //!< I/O port G clock enable
 #define RCC_APB2ENR_TIM8EN         0x00002000 //!< TIM8 Timer clock enable
 #define RCC_APB2ENR_ADC3EN         0x00008000 //!< DMA1 clock enable
#endif

#if defined (STM32F10X_HD_VL)
 #define RCC_APB2ENR_IOPFEN         0x00000080 //!< I/O port F clock enable
 #define RCC_APB2ENR_IOPGEN         0x00000100 //!< I/O port G clock enable
#endif

#ifdef STM32F10X_XL
 #define RCC_APB2ENR_TIM9EN         0x00080000 //!< TIM9 Timer clock enable
 #define RCC_APB2ENR_TIM10EN        0x00100000 //!< TIM10 Timer clock enable
 #define RCC_APB2ENR_TIM11EN        0x00200000 //!< TIM11 Timer clock enable
#endif
/** @} */

/** @defgroup RCC_APB1ENR:	(rcc Offset: 0x1C) APB1 peripheral clock enable register */
#define RCC_APB1ENR_TIM2EN          0x00000001 //!< Timer 2 clock enabled
#define RCC_APB1ENR_TIM3EN          0x00000002 //!< Timer 3 clock enable
#define RCC_APB1ENR_WWDGEN          0x00000800 //!< Window Watchdog clock enable
#define RCC_APB1ENR_USART2EN        0x00020000 //!< USART 2 clock enable
#define RCC_APB1ENR_I2C1EN          0x00200000 //!< I2C 1 clock enable

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
#define RCC_APB1ENR_CAN1EN          0x02000000 //!< CAN1 clock enable
#endif

#define RCC_APB1ENR_BKPEN           0x08000000 //!< Backup interface clock enable
#define RCC_APB1ENR_PWREN           0x10000000 //!< Power interface clock enable

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
 #define RCC_APB1ENR_TIM4EN         0x00000004 //!< Timer 4 clock enable
 #define RCC_APB1ENR_SPI2EN         0x00004000 //!< SPI 2 clock enable
 #define RCC_APB1ENR_USART3EN       0x00040000 //!< USART 3 clock enable
 #define RCC_APB1ENR_I2C2EN         0x00400000 //!< I2C 2 clock enable
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
 #define RCC_APB1ENR_USBEN          0x00800000 //!< USB Device clock enable
#endif

#if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
 #define RCC_APB1ENR_TIM5EN         0x00000008 //!< Timer 5 clock enable
 #define RCC_APB1ENR_TIM6EN         0x00000010 //!< Timer 6 clock enable
 #define RCC_APB1ENR_TIM7EN         0x00000020 //!< Timer 7 clock enable
 #define RCC_APB1ENR_SPI3EN         0x00008000 //!< SPI 3 clock enable
 #define RCC_APB1ENR_UART4EN        0x00080000 //!< UART 4 clock enable
 #define RCC_APB1ENR_UART5EN        0x00100000 //!< UART 5 clock enable
 #define RCC_APB1ENR_DACEN          0x20000000 //!< DAC interface clock enable
#endif

#if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
 #define RCC_APB1ENR_TIM6EN         0x00000010 //!< Timer 6 clock enable
 #define RCC_APB1ENR_TIM7EN         0x00000020 //!< Timer 7 clock enable
 #define RCC_APB1ENR_DACEN          0x20000000 //!< DAC interface clock enable
 #define RCC_APB1ENR_CECEN          0x40000000 //!< CEC interface clock enable
#endif

#ifdef STM32F10X_HD_VL
 #define RCC_APB1ENR_TIM5EN         0x00000008 //!< Timer 5 clock enable
 #define RCC_APB1ENR_TIM12EN        0x00000040 //!< TIM12 Timer clock enable
 #define RCC_APB1ENR_TIM13EN        0x00000080 //!< TIM13 Timer clock enable
 #define RCC_APB1ENR_TIM14EN        0x00000100 //!< TIM14 Timer clock enable
 #define RCC_APB1ENR_SPI3EN         0x00008000 //!< SPI 3 clock enable
 #define RCC_APB1ENR_UART4EN        0x00080000 //!< UART 4 clock enable
 #define RCC_APB1ENR_UART5EN        0x00100000 //!< UART 5 clock enable
#endif /* STM32F10X_HD_VL */

#ifdef STM32F10X_CL
 #define RCC_APB1ENR_CAN2EN         0x04000000 //!< CAN2 clock enable
#endif /* STM32F10X_CL */

#ifdef STM32F10X_XL
 #define RCC_APB1ENR_TIM12EN        0x00000040 //!< TIM12 Timer clock enable
 #define RCC_APB1ENR_TIM13EN        0x00000080 //!< TIM13 Timer clock enable
 #define RCC_APB1ENR_TIM14EN        0x00000100 //!< TIM14 Timer clock enable
#endif /* STM32F10X_XL */

/** @} */

/** @defgroup RCC_BDCR:		(rcc Offset: 0x20) Backup domain control register */
#define RCC_BDCR_LSEON              0x00000001 //!< External Low Speed oscillator enable
#define RCC_BDCR_LSERDY             0x00000002 //!< External Low Speed oscillator Ready
#define RCC_BDCR_LSEBYP             0x00000004 //!< External Low Speed oscillator Bypass

/*!< RTC congiguration */
#define RCC_BDCR_RTCSEL             0x00000300 //!< RTCSEL[1:0] bits (RTC clock source selection)
#define RCC_BDCR_RTCSEL_NOCLOCK     0x00000000 //!< No clock
#define RCC_BDCR_RTCSEL_LSE         0x00000100 //!< LSE oscillator clock used as RTC clock
#define RCC_BDCR_RTCSEL_LSI         0x00000200 //!< LSI oscillator clock used as RTC clock
#define RCC_BDCR_RTCSEL_HSE         0x00000300 //!< HSE oscillator clock divided by 128 used as RTC clock

#define RCC_BDCR_RTCEN              0x00008000 //!< RTC clock enable
#define RCC_BDCR_BDRST              0x00010000 //!< Backup domain software reset
/** @} */

/** @defgroup RCC_CSR:		(rcc Offset: 0x24) Control/status register		  */
#define RCC_CSR_LSION               0x00000001 //!< Internal Low Speed oscillator enable
#define RCC_CSR_LSIRDY              0x00000002 //!< Internal Low Speed oscillator Ready
#define RCC_CSR_RMVF                0x01000000 //!< Remove reset flag
#define RCC_CSR_PINRSTF             0x04000000 //!< PIN reset flag
#define RCC_CSR_PORRSTF             0x08000000 //!< POR/PDR reset flag
#define RCC_CSR_SFTRSTF             0x10000000 //!< Software Reset flag
#define RCC_CSR_IWDGRSTF            0x20000000 //!< Independent Watchdog reset flag
#define RCC_CSR_WWDGRSTF            0x40000000 //!< Window watchdog reset flag
#define RCC_CSR_LPWRRSTF            0x80000000 //!< Low-Power reset flag
/** @} */

#ifdef STM32F10X_CL
/** @defgroup RCC_AHBRSTR:	(rcc Offset: 0x28) AHB peripheral clock reset register */
 #define RCC_AHBRSTR_OTGFSRST       0x00001000 //!< USB OTG FS reset
 #define RCC_AHBRSTR_ETHMACRST      0x00004000 //!< ETHERNET MAC reset
/** @} */

/** @defgroup RCC_CFGR2:	(rcc Offset: 0x2C) Clock configuration register2  */
/*!< PREDIV1 configuration */
 #define RCC_CFGR2_PREDIV1          0x0000000F //!< PREDIV1[3:0] bits
 #define RCC_CFGR2_PREDIV1_DIV1     0x00000000 //!< PREDIV1 input clock not divided
 #define RCC_CFGR2_PREDIV1_DIV2     0x00000001 //!< PREDIV1 input clock divided by 2
 #define RCC_CFGR2_PREDIV1_DIV3     0x00000002 //!< PREDIV1 input clock divided by 3
 #define RCC_CFGR2_PREDIV1_DIV4     0x00000003 //!< PREDIV1 input clock divided by 4
 #define RCC_CFGR2_PREDIV1_DIV5     0x00000004 //!< PREDIV1 input clock divided by 5
 #define RCC_CFGR2_PREDIV1_DIV6     0x00000005 //!< PREDIV1 input clock divided by 6
 #define RCC_CFGR2_PREDIV1_DIV7     0x00000006 //!< PREDIV1 input clock divided by 7
 #define RCC_CFGR2_PREDIV1_DIV8     0x00000007 //!< PREDIV1 input clock divided by 8
 #define RCC_CFGR2_PREDIV1_DIV9     0x00000008 //!< PREDIV1 input clock divided by 9
 #define RCC_CFGR2_PREDIV1_DIV10    0x00000009 //!< PREDIV1 input clock divided by 10
 #define RCC_CFGR2_PREDIV1_DIV11    0x0000000A //!< PREDIV1 input clock divided by 11
 #define RCC_CFGR2_PREDIV1_DIV12    0x0000000B //!< PREDIV1 input clock divided by 12
 #define RCC_CFGR2_PREDIV1_DIV13    0x0000000C //!< PREDIV1 input clock divided by 13
 #define RCC_CFGR2_PREDIV1_DIV14    0x0000000D //!< PREDIV1 input clock divided by 14
 #define RCC_CFGR2_PREDIV1_DIV15    0x0000000E //!< PREDIV1 input clock divided by 15
 #define RCC_CFGR2_PREDIV1_DIV16    0x0000000F //!< PREDIV1 input clock divided by 16

/*!< PREDIV2 configuration */
 #define RCC_CFGR2_PREDIV2          0x000000F0 //!< PREDIV2[3:0] bits
 #define RCC_CFGR2_PREDIV2_DIV1     0x00000000 //!< PREDIV2 input clock not divided
 #define RCC_CFGR2_PREDIV2_DIV2     0x00000010 //!< PREDIV2 input clock divided by 2
 #define RCC_CFGR2_PREDIV2_DIV3     0x00000020 //!< PREDIV2 input clock divided by 3
 #define RCC_CFGR2_PREDIV2_DIV4     0x00000030 //!< PREDIV2 input clock divided by 4
 #define RCC_CFGR2_PREDIV2_DIV5     0x00000040 //!< PREDIV2 input clock divided by 5
 #define RCC_CFGR2_PREDIV2_DIV6     0x00000050 //!< PREDIV2 input clock divided by 6
 #define RCC_CFGR2_PREDIV2_DIV7     0x00000060 //!< PREDIV2 input clock divided by 7
 #define RCC_CFGR2_PREDIV2_DIV8     0x00000070 //!< PREDIV2 input clock divided by 8
 #define RCC_CFGR2_PREDIV2_DIV9     0x00000080 //!< PREDIV2 input clock divided by 9
 #define RCC_CFGR2_PREDIV2_DIV10    0x00000090 //!< PREDIV2 input clock divided by 10
 #define RCC_CFGR2_PREDIV2_DIV11    0x000000A0 //!< PREDIV2 input clock divided by 11
 #define RCC_CFGR2_PREDIV2_DIV12    0x000000B0 //!< PREDIV2 input clock divided by 12
 #define RCC_CFGR2_PREDIV2_DIV13    0x000000C0 //!< PREDIV2 input clock divided by 13
 #define RCC_CFGR2_PREDIV2_DIV14    0x000000D0 //!< PREDIV2 input clock divided by 14
 #define RCC_CFGR2_PREDIV2_DIV15    0x000000E0 //!< PREDIV2 input clock divided by 15
 #define RCC_CFGR2_PREDIV2_DIV16    0x000000F0 //!< PREDIV2 input clock divided by 16

/*!< PLL2MUL configuration */
 #define RCC_CFGR2_PLL2MUL          0x00000F00 //!< PLL2MUL[3:0] bits
 #define RCC_CFGR2_PLL2MUL8         0x00000600 //!< PLL2 input clock * 8
 #define RCC_CFGR2_PLL2MUL9         0x00000700 //!< PLL2 input clock * 9
 #define RCC_CFGR2_PLL2MUL10        0x00000800 //!< PLL2 input clock * 10
 #define RCC_CFGR2_PLL2MUL11        0x00000900 //!< PLL2 input clock * 11
 #define RCC_CFGR2_PLL2MUL12        0x00000A00 //!< PLL2 input clock * 12
 #define RCC_CFGR2_PLL2MUL13        0x00000B00 //!< PLL2 input clock * 13
 #define RCC_CFGR2_PLL2MUL14        0x00000C00 //!< PLL2 input clock * 14
 #define RCC_CFGR2_PLL2MUL16        0x00000E00 //!< PLL2 input clock * 16
 #define RCC_CFGR2_PLL2MUL20        0x00000F00 //!< PLL2 input clock * 20

/*!< PLL3MUL configuration */
 #define RCC_CFGR2_PLL3MUL          0x0000F000 //!< PLL3MUL[3:0] bits
 #define RCC_CFGR2_PLL3MUL8         0x00006000 //!< PLL3 input clock * 8
 #define RCC_CFGR2_PLL3MUL9         0x00007000 //!< PLL3 input clock * 9
 #define RCC_CFGR2_PLL3MUL10        0x00008000 //!< PLL3 input clock * 10
 #define RCC_CFGR2_PLL3MUL11        0x00009000 //!< PLL3 input clock * 11
 #define RCC_CFGR2_PLL3MUL12        0x0000A000 //!< PLL3 input clock * 12
 #define RCC_CFGR2_PLL3MUL13        0x0000B000 //!< PLL3 input clock * 13
 #define RCC_CFGR2_PLL3MUL14        0x0000C000 //!< PLL3 input clock * 14
 #define RCC_CFGR2_PLL3MUL16        0x0000E000 //!< PLL3 input clock * 16
 #define RCC_CFGR2_PLL3MUL20        0x0000F000 //!< PLL3 input clock * 20

 #define RCC_CFGR2_PREDIV1SRC       0x00010000 //!< PREDIV1 entry clock source
 #define RCC_CFGR2_PREDIV1SRC_PLL2  0x00010000 //!< PLL2 selected as PREDIV1 entry clock source
 #define RCC_CFGR2_PREDIV1SRC_HSE   0x00000000 //!< HSE selected as PREDIV1 entry clock source
 #define RCC_CFGR2_I2S2SRC          0x00020000 //!< I2S2 entry clock source
 #define RCC_CFGR2_I2S3SRC          0x00040000 //!< I2S3 clock source
/** @} */
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
/** @defgroup RCC_CFGR2:	(rcc Offset: 0x2C) Clock configuration register2  */
/*!< PREDIV1 configuration */
 #define RCC_CFGR2_PREDIV1          0x0000000F //!< PREDIV1[3:0] bits */
 #define RCC_CFGR2_PREDIV1_DIV1     0x00000000 //!< PREDIV1 input clock not divided
 #define RCC_CFGR2_PREDIV1_DIV2     0x00000001 //!< PREDIV1 input clock divided by 2
 #define RCC_CFGR2_PREDIV1_DIV3     0x00000002 //!< PREDIV1 input clock divided by 3
 #define RCC_CFGR2_PREDIV1_DIV4     0x00000003 //!< PREDIV1 input clock divided by 4
 #define RCC_CFGR2_PREDIV1_DIV5     0x00000004 //!< PREDIV1 input clock divided by 5
 #define RCC_CFGR2_PREDIV1_DIV6     0x00000005 //!< PREDIV1 input clock divided by 6
 #define RCC_CFGR2_PREDIV1_DIV7     0x00000006 //!< PREDIV1 input clock divided by 7
 #define RCC_CFGR2_PREDIV1_DIV8     0x00000007 //!< PREDIV1 input clock divided by 8
 #define RCC_CFGR2_PREDIV1_DIV9     0x00000008 //!< PREDIV1 input clock divided by 9
 #define RCC_CFGR2_PREDIV1_DIV10    0x00000009 //!< PREDIV1 input clock divided by 10
 #define RCC_CFGR2_PREDIV1_DIV11    0x0000000A //!< PREDIV1 input clock divided by 11
 #define RCC_CFGR2_PREDIV1_DIV12    0x0000000B //!< PREDIV1 input clock divided by 12
 #define RCC_CFGR2_PREDIV1_DIV13    0x0000000C //!< PREDIV1 input clock divided by 13
 #define RCC_CFGR2_PREDIV1_DIV14    0x0000000D //!< PREDIV1 input clock divided by 14
 #define RCC_CFGR2_PREDIV1_DIV15    0x0000000E //!< PREDIV1 input clock divided by 15
 #define RCC_CFGR2_PREDIV1_DIV16    0x0000000F //!< PREDIV1 input clock divided by 16
#endif
/** @} */





/** @} */ // @defgroup RCC_regs_define

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) Startup
   Timeout value
   */
#define HSE_STARTUP_TIMEOUT   0x0500 /*!< Time out for HSE start up */


#endif /* RCC_F1_H_ */
