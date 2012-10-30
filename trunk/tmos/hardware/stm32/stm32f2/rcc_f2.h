/*
 * rcc_f2.h
 *
 *  Created on: Oct 29, 2012
 *      Author: miro
 */

#ifndef RCC_F2_H_
#define RCC_F2_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Reset and Clock Control registers
 ******************************************************************************/
typedef struct
{

  __IO uint32_t RCC_CR;				//!< (rcc Offset: 0x00) Clock control register
  __IO uint32_t RCC_PLLCFGR;		//!< (rcc offset: 0x04) RCC PLL configuration register
  __IO uint32_t RCC_CFGR;			//!< (rcc offset: 0x08) Clock configuration register
  __IO uint32_t RCC_CIR;			//!< (rcc Offset: 0x0C) Clock interrupt register
  __IO uint32_t RCC_AHB1RSTR;		//!< (rcc Offset: 0x10) AHB1 peripheral reset register
  __IO uint32_t RCC_AHB2RSTR;		//!< (rcc Offset: 0x14) AHB2 peripheral reset register
  __IO uint32_t RCC_AHB3RSTR;		//!< (rcc Offset: 0x18) AHB3 peripheral reset register
  __IO uint32_t RESERVED0;			//!< (rcc Offset: 0x1C) reserved
  __IO uint32_t RCC_APB1RSTR;		//!< (rcc Offset: 0x20) APB1 peripheral reset register
  __IO uint32_t RCC_APB2RSTR;		//!< (rcc Offset: 0x24) APB2 peripheral reset register
  __IO uint32_t RESERVED1[2];		//!< (rcc Offset: 0x28) reserved
  __IO uint32_t RCC_AHB1ENR;       	//!< (rcc Offset: 0x30) RCC AHB1 peripheral clock register
  __IO uint32_t RCC_AHB2ENR;       	//!< (rcc Offset: 0x34) RCC AHB2 peripheral clock register
  __IO uint32_t RCC_AHB3ENR;      	//!< (rcc Offset: 0x38) RCC AHB3 peripheral clock register
  __IO uint32_t RESERVED2;			//!< (rcc Offset: 0x3C) reserved
  __IO uint32_t RCC_APB1ENR;		//!< (rcc Offset: 0x40) APB1 peripheral clock enable register
  __IO uint32_t RCC_APB2ENR;		//!< (rcc Offset: 0x44) APB2 peripheral clock enable register
  __IO uint32_t RESERVED3[2];		//!< (rcc Offset: 0x48) reserved
  __IO uint32_t RCC_AHB1LPENR;    	//!< (rcc Offset: 0x50) RCC AHB1 peripheral clock enable in low power mode register
  __IO uint32_t RCC_AHB2LPENR;     	//!< (rcc Offset: 0x54) RCC AHB2 peripheral clock enable in low power mode register
  __IO uint32_t RCC_AHB3LPENR;    	//!< (rcc Offset: 0x58) RCC AHB3 peripheral clock enable in low power mode register
  __IO uint32_t RESERVED4;			//!< (rcc Offset: 0x5C) reserved
  __IO uint32_t RCC_APB1LPENR;   	//!< (rcc Offset: 0x60) RCC APB1 peripheral clock enable in low power mode register
  __IO uint32_t RCC_APB2LPENR;   	//!< (rcc Offset: 0x64) RCC APB2 peripheral clock enable in low power mode register
  __IO uint32_t RESERVED5[2];		//!< (rcc Offset: 0x68) reserved
  __IO uint32_t RCC_BDCR;			//!< (rcc Offset: 0x70) Backup domain control register
  __IO uint32_t RCC_CSR;			//!< (rcc Offset: 0x74) Control/status register
  __IO uint32_t RESERVED6[2];		//!< (rcc Offset: 0x78) reserved
  __IO uint32_t RCC_SSCGR;       	//!< (rcc Offset: 0x80) RCC spread spectrum clock generation register
  __IO uint32_t RCC_PLLI2SCFGR;  	//!< (rcc Offset: 0x84) RCC PLLI2S configuration register

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
#define RCC_CR_PLLI2SON          	0x04000000 //!< PLLI2S enable
#define RCC_CR_PLLI2SRDY         	0x08000000 //!< PLLI2S clock ready flag
/** @} */


/** @defgroup RCC_PLLCFGR:	 (rcc offset: 0x04) RCC PLL configuration register  */
#define  RCC_PLLCFGR_PLLM           0x0000003F //!< Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
#define  RCC_PLLCFGR_PLLM_Get(x)    ((x) & 0x3F)
#define  RCC_PLLCFGR_PLLM_Set(x)    ((x) & 0x3F)


#define  RCC_PLLCFGR_PLLN           0x00007FC0 //!< Main PLL (PLL) multiplication factor for VCO
#define  RCC_PLLCFGR_PLLN_Get(x)    (((x) >> 6) & 0x1FF)
#define  RCC_PLLCFGR_PLLN_Set(x)    (((x) & 0x1FF) << 6)

#define  RCC_PLLCFGR_PLLP           0x00030000 //!< Main PLL (PLL) division factor for main system clock
#define  RCC_PLLCFGR_PLLP_DIV2      0x00000000
#define  RCC_PLLCFGR_PLLP_DIV4      0x00010000
#define  RCC_PLLCFGR_PLLP_DIV6      0x00020000
#define  RCC_PLLCFGR_PLLP_DIV8      0x00030000

#define  RCC_PLLCFGR_PLLSRC         0x00400000 //!< Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
#define  RCC_PLLCFGR_PLLSRC_HSE     0x00400000 //!<  HSE oscillator clock selected as PLL and PLLI2S clock entry
#define  RCC_PLLCFGR_PLLSRC_HSI     0x00000000 //!<  HSI clock selected as PLL and PLLI2S clock entry

#define  RCC_PLLCFGR_PLLQ           0x0F000000 //!< Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator
#define  RCC_PLLCFGR_PLLQ_Get(x)    (((x) >> 24) & 0xF)
#define  RCC_PLLCFGR_PLLQ_Set(x)    (((x) & 0xF) << 24)
/** @} */


/** @defgroup RCC_CFGR:		 (rcc offset: 0x08) Clock configuration register    */
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
#define RCC_CFGR_PPRE1          	0x00001C00 //!< PRE1[2:0] APB Low speed prescaler (APB1)
#define RCC_CFGR_PPRE1_DIV1     	0x00000000 //!< AHB not divided
#define RCC_CFGR_PPRE1_DIV2     	0x00001000 //!< AHB divided by 2
#define RCC_CFGR_PPRE1_DIV4     	0x00001400 //!< AHB divided by 4
#define RCC_CFGR_PPRE1_DIV8     	0x00001800 //!< AHB divided by 8
#define RCC_CFGR_PPRE1_DIV16    	0x00001C00 //!< AHB divided by 16

/*!< PPRE2 configuration */
#define RCC_CFGR_PPRE2          	0x0000E000 //!< PRE2[2:0] bits (APB2 prescaler)
#define RCC_CFGR_PPRE2_DIV1     	0x00000000 //!< AHB not divided
#define RCC_CFGR_PPRE2_DIV2     	0x00008000 //!< AHB divided by 2
#define RCC_CFGR_PPRE2_DIV4     	0x0000A000 //!< AHB divided by 4
#define RCC_CFGR_PPRE2_DIV8     	0x0000C000 //!< AHB divided by 8
#define RCC_CFGR_PPRE2_DIV16    	0x0000E000 //!< AHB divided by 16

/*!< RTCPRE configuration */
#define RCC_CFGR_RTCPRE             0x001F0000 //!< HSE division factor for RTC clock
#define RCC_CFGR_RTCPRE_Get(x)    	(((x) >> 16) & 0x1F)
#define RCC_CFGR_RTCPRE_Set(x)    	(((x) & 0x1F) << 16)

/*!< MCO1 configuration */
#define RCC_CFGR_MCO1               0x00600000 //!< Microcontroller clock output 1
#define RCC_CFGR_MCO1_HSI           0x00000000 //!< HSI clock selected
#define RCC_CFGR_MCO1_LSE           0x00200000 //!< LSE oscillator selected
#define RCC_CFGR_MCO1_HSE           0x00400000 //!< HSE oscillator clock selected
#define RCC_CFGR_MCO1_PLL           0x00600000 //!< PLL clock selected

#define RCC_CFGR_I2SSRC             0x00800000 //!< I2S clock selection

#define RCC_CFGR_MCO1PRE            0x07000000 //!< MCO1 prescaler
#define RCC_CFGR_MCO1PRE_DIV1       0x00000000 //!< 0xx: no division
#define RCC_CFGR_MCO1PRE_DIV2       0x04000000 //!< division by 2
#define RCC_CFGR_MCO1PRE_DIV3       0x05000000 //!< division by 3
#define RCC_CFGR_MCO1PRE_DIV4       0x06000000 //!< division by 4
#define RCC_CFGR_MCO1PRE_DIV5       0x07000000 //!< division by 5

#define RCC_CFGR_MCO2PRE            0x38000000 //!< MCO2 prescaler
#define RCC_CFGR_MCO2PRE_DIV1       0x00000000 //!< 0xx: no division
#define RCC_CFGR_MCO2PRE_DIV2       0x20000000 //!< division by 2
#define RCC_CFGR_MCO2PRE_DIV3       0x28000000 //!< division by 3
#define RCC_CFGR_MCO2PRE_DIV4       0x30000000 //!< division by 4
#define RCC_CFGR_MCO2PRE_DIV5       0x38000000 //!< division by 5

#define RCC_CFGR_MCO2               0xC0000000 //!< Microcontroller clock output 2
#define RCC_CFGR_MCO2_SYSCLK        0x00000000 //!< System clock (SYSCLK) selected
#define RCC_CFGR_MCO2_PLLI2S        0x40000000 //!< PLLI2S clock selected
#define RCC_CFGR_MCO2_HSE           0x80000000 //!< HSE oscillator clock selected
#define RCC_CFGR_MCO2_PLL           0xC0000000 //!< PLL clock selected
/** @} */


/** @defgroup RCC_CIR:		 (rcc Offset: 0x0C) Clock interrupt register		*/
#define RCC_CIR_LSIRDYF             0x00000001 //!< LSI Ready Interrupt flag
#define RCC_CIR_LSERDYF             0x00000002 //!< LSE Ready Interrupt flag
#define RCC_CIR_HSIRDYF             0x00000004 //!< HSI Ready Interrupt flag
#define RCC_CIR_HSERDYF             0x00000008 //!< HSE Ready Interrupt flag
#define RCC_CIR_PLLRDYF             0x00000010 //!< PLL Ready Interrupt flag
#define RCC_CIR_PLLI2SRDYF          0x00000020 //!< PLL I2S Ready Interrupt flag
#define RCC_CIR_CSSF                0x00000080 //!< Clock Security System Interrupt flag
#define RCC_CIR_LSIRDYIE            0x00000100 //!< LSI Ready Interrupt Enable
#define RCC_CIR_LSERDYIE            0x00000200 //!< LSE Ready Interrupt Enable
#define RCC_CIR_HSIRDYIE            0x00000400 //!< HSI Ready Interrupt Enable
#define RCC_CIR_HSERDYIE            0x00000800 //!< HSE Ready Interrupt Enable
#define RCC_CIR_PLLRDYIE            0x00001000 //!< PLL Ready Interrupt Enable
#define RCC_CIR_PLLI2SRDYIE         0x00002000 //!< PLL I2S Ready Interrupt Enable
#define RCC_CIR_LSIRDYC             0x00010000 //!< LSI Ready Interrupt Clear
#define RCC_CIR_LSERDYC             0x00020000 //!< LSE Ready Interrupt Clear
#define RCC_CIR_HSIRDYC             0x00040000 //!< HSI Ready Interrupt Clear
#define RCC_CIR_HSERDYC             0x00080000 //!< HSE Ready Interrupt Clear
#define RCC_CIR_PLLRDYC             0x00100000 //!< PLL Ready Interrupt Clear
#define RCC_CIR_PLLI2SRDYC          0x00200000 //!< PLL2 Ready Interrupt Clear
#define RCC_CIR_CSSC                0x00800000 //!< Clock Security System Interrupt Clear
/** @} */


/** @defgroup RCC_AHB1RSTR:	 (rcc Offset: 0x10) AHB1 peripheral reset register	*/
#define RCC_AHB1RSTR_GPIOARST       0x00000001 //!< IO port A reset
#define RCC_AHB1RSTR_GPIOBRST       0x00000002 //!< IO port B reset
#define RCC_AHB1RSTR_GPIOCRST       0x00000004 //!< IO port C reset
#define RCC_AHB1RSTR_GPIODRST       0x00000008 //!< IO port D reset
#define RCC_AHB1RSTR_GPIOERST       0x00000010 //!< IO port E reset
#define RCC_AHB1RSTR_GPIOFRST       0x00000020 //!< IO port F reset
#define RCC_AHB1RSTR_GPIOGRST       0x00000040 //!< IO port G reset
#define RCC_AHB1RSTR_GPIOHRST       0x00000080 //!< IO port H reset
#define RCC_AHB1RSTR_GPIOIRST       0x00000100 //!< IO port I reset
#define RCC_AHB1RSTR_CRCRST         0x00001000 //!< CRC reset
#define RCC_AHB1RSTR_DMA1RST        0x00200000 //!< DMA1 reset
#define RCC_AHB1RSTR_DMA2RST        0x00400000 //!< DMA2 reset
#define RCC_AHB1RSTR_ETHMACRST      0x02000000 //!< Ethernet MAC reset
#define RCC_AHB1RSTR_OTGHRST        0x10000000 //!< USB OTG HS module reset
/** @} */


/** @defgroup RCC_AHB2RSTR:	 (rcc Offset: 0x14) AHB2 peripheral reset register	*/
#define RCC_AHB2RSTR_DCMIRST        0x00000001 //!< Camera interface reset
#define RCC_AHB2RSTR_CRYPRST        0x00000010 //!< Cryptographic module reset
#define RCC_AHB2RSTR_HSAHRST        0x00000020 //!< Hash module reset
#define RCC_AHB2RSTR_RNGRST         0x00000040 //!< Random number generator module reset
#define RCC_AHB2RSTR_OTGFSRST       0x00000080 //!< USB OTG FS module reset
/** @} */

/** @defgroup RCC_AHB3RSTR:	 (rcc Offset: 0x18) AHB3 peripheral reset register	*/
#define RCC_AHB3RSTR_FSMCRST        0x00000001 //!< Flexible static memory controller module reset
/** @} */

/** @defgroup RCC_APB1RSTR:	 (rcc Offset: 0x20) APB1 peripheral reset register	*/
#define RCC_APB1RSTR_TIM2RST        0x00000001 //!< TIM2 reset
#define RCC_APB1RSTR_TIM3RST        0x00000002 //!< TIM3 reset
#define RCC_APB1RSTR_TIM4RST        0x00000004 //!< TIM4 reset
#define RCC_APB1RSTR_TIM5RST        0x00000008 //!< TIM5 reset
#define RCC_APB1RSTR_TIM6RST        0x00000010 //!< TIM6 reset
#define RCC_APB1RSTR_TIM7RST        0x00000020 //!< TIM7 reset
#define RCC_APB1RSTR_TIM12RST       0x00000040 //!< TIM12 reset
#define RCC_APB1RSTR_TIM13RST       0x00000080 //!< TIM13 reset
#define RCC_APB1RSTR_TIM14RST       0x00000100 //!< TIM14 reset
#define RCC_APB1RSTR_WWDGEN         0x00000800 //!< Window watchdog reset
#define RCC_APB1RSTR_SPI2RST        0x00008000 //!< SPI 2 reset
#define RCC_APB1RSTR_SPI3RST        0x00010000 //!< SPI 3 reset
#define RCC_APB1RSTR_USART2RST      0x00020000 //!< USART 2 reset
#define RCC_APB1RSTR_USART3RST      0x00040000 //!< USART 3 reset
#define RCC_APB1RSTR_UART4RST       0x00080000 //!< USART 4 reset
#define RCC_APB1RSTR_UART5RST       0x00100000 //!< USART 5 reset
#define RCC_APB1RSTR_I2C1RST        0x00200000 //!< I2C1 reset
#define RCC_APB1RSTR_I2C2RST        0x00400000 //!< I2C2 reset
#define RCC_APB1RSTR_I2C3RST        0x00800000 //!< I2C3 reset
#define RCC_APB1RSTR_CAN1RST        0x02000000 //!< CAN1 reset
#define RCC_APB1RSTR_CAN2RST        0x04000000 //!< CAN2 reset
#define RCC_APB1RSTR_PWRRST         0x10000000 //!< Power interface reset
#define RCC_APB1RSTR_DACRST         0x20000000 //!< DAC reset
/** @} */


/** @defgroup RCC_APB2RSTR:	 (rcc Offset: 0x24) APB2 peripheral reset register	*/
#define RCC_APB2RSTR_TIM1RST        0x00000001 //!< TIM1 reset
#define RCC_APB2RSTR_TIM8RST        0x00000002 //!< TIM8 reset
#define RCC_APB2RSTR_USART1RST      0x00000010 //!< USART1 reset
#define RCC_APB2RSTR_USART6RST      0x00000020 //!< USART6 reset
#define RCC_APB2RSTR_ADCRST         0x00000100 //!< ADC interface reset (common to all ADCs)
#define RCC_APB2RSTR_SDIORST        0x00000800 //!< SDIO reset
#define RCC_APB2RSTR_SPI1           0x00001000 //!< SPI 1 reset
#define RCC_APB2RSTR_SYSCFGRST      0x00004000 //!< System configuration controller reset
#define RCC_APB2RSTR_TIM9RST        0x00010000 //!< TIM9 reset
#define RCC_APB2RSTR_TIM10RST       0x00020000 //!< TIM10 reset
#define RCC_APB2RSTR_TIM11RST       0x00040000 //!< TIM11 reset
/** @} */

/** @defgroup RCC_AHB1ENR:   (rcc Offset: 0x30) RCC AHB1 peripheral clock register */
#define RCC_AHB1ENR_GPIOAEN         0x00000001 //!< IO port A clock enable
#define RCC_AHB1ENR_GPIOBEN         0x00000002 //!< IO port B clock enable
#define RCC_AHB1ENR_GPIOCEN         0x00000004 //!< IO port C clock enable
#define RCC_AHB1ENR_GPIODEN         0x00000008 //!< IO port D clock enable
#define RCC_AHB1ENR_GPIOEEN         0x00000010 //!< IO port E clock enable
#define RCC_AHB1ENR_GPIOFEN         0x00000020 //!< IO port F clock enable
#define RCC_AHB1ENR_GPIOGEN         0x00000040 //!< IO port G clock enable
#define RCC_AHB1ENR_GPIOHEN         0x00000080 //!< IO port H clock enable
#define RCC_AHB1ENR_GPIOIEN         0x00000100 //!< IO port I clock enable
#define RCC_AHB1ENR_CRCEN           0x00001000 //!< CRC clock enable
#define RCC_AHB1ENR_BKPSRAMEN       0x00040000 //!< Backup SRAM interface clock enable
#define RCC_AHB1ENR_DMA1EN          0x00200000 //!< DMA1 clock enable
#define RCC_AHB1ENR_DMA2EN          0x00400000 //!< DMA2 clock enable
#define RCC_AHB1ENR_ETHMACEN        0x02000000 //!< Ethernet MAC clock enable
#define RCC_AHB1ENR_ETHMACTXEN      0x04000000 //!< Ethernet Transmission clock enable
#define RCC_AHB1ENR_ETHMACRXEN      0x08000000 //!< Ethernet Reception clock enable
#define RCC_AHB1ENR_ETHMACPTPEN     0x10000000 //!< Ethernet PTP clock enable
#define RCC_AHB1ENR_OTGHSEN         0x20000000 //!< USB OTG HS clock enable
#define RCC_AHB1ENR_OTGHSULPIEN     0x40000000 //!< USB OTG HSULPI clock enable
/** @} */

/** @defgroup RCC_AHB2ENR:   (rcc Offset: 0x34) RCC AHB2 peripheral clock register */
#define RCC_AHB2ENR_DCMIEN          0x00000001 //!< Camera interface enable
#define RCC_AHB2ENR_CRYPEN          0x00000010 //!< Cryptographic modules clock enable
#define RCC_AHB2ENR_HASHEN          0x00000020 //!< Hash modules clock enable
#define RCC_AHB2ENR_RNGEN           0x00000040 //!< Random number generator clock enable
#define RCC_AHB2ENR_OTGFSEN         0x00000080 //!< USB OTG FS clock enable
/** @} */

/** @defgroup RCC_AHB3ENR:   (rcc Offset: 0x38) RCC AHB3 peripheral clock register */
#define RCC_AHB3ENR_FSMCEN          0x00000001 //!< Flexible static memory controller module clock enable
/** @} */

/** @defgroup RCC_APB1ENR:	 (rcc Offset: 0x40) APB1 peripheral clock enable register */
#define RCC_APB1ENR_TIM2EN          0x00000001 //!< TIM2 clock enable
#define RCC_APB1ENR_TIM3EN          0x00000002 //!< TIM3 clock enable
#define RCC_APB1ENR_TIM4EN          0x00000004 //!< TIM4 clock enable
#define RCC_APB1ENR_TIM5EN          0x00000008 //!< TIM5 clock enable
#define RCC_APB1ENR_TIM6EN          0x00000010 //!< TIM6 clock enable
#define RCC_APB1ENR_TIM7EN          0x00000020 //!< TIM7 clock enable
#define RCC_APB1ENR_TIM12EN         0x00000040 //!< TIM12 clock enable
#define RCC_APB1ENR_TIM13EN         0x00000080 //!< TIM13 clock enable
#define RCC_APB1ENR_TIM14EN         0x00000100 //!< TIM14 clock enable
#define RCC_APB1ENR_WWDGEN          0x00000800 //!< Window watchdog clock enable
#define RCC_APB1ENR_SPI2EN          0x00004000 //!< SPI2 clock enable
#define RCC_APB1ENR_SPI3EN          0x00008000 //!< SPI3 clock enable
#define RCC_APB1ENR_USART2EN        0x00020000 //!< UART2 clock enable
#define RCC_APB1ENR_USART3EN        0x00040000 //!< UART3 clock enable
#define RCC_APB1ENR_UART4EN         0x00080000 //!< UART4 clock enable
#define RCC_APB1ENR_UART5EN         0x00100000 //!< UART5 clock enable
#define RCC_APB1ENR_I2C1EN          0x00200000 //!< I2C1 clock enable
#define RCC_APB1ENR_I2C2EN          0x00400000 //!< I2C2 clock enable
#define RCC_APB1ENR_I2C3EN          0x00800000 //!< I2C3 clock enable
#define RCC_APB1ENR_CAN1EN          0x02000000 //!< CAN 1 clock enable
#define RCC_APB1ENR_CAN2EN          0x04000000 //!< CAN 2 clock enable
#define RCC_APB1ENR_PWREN           0x10000000 //!< Power interface clock enable
#define RCC_APB1ENR_DACEN           0x20000000 //!< DAC interface clock enable
/** @} */

/** @defgroup RCC_APB2ENR:	 (rcc Offset: 0x44) APB2 peripheral clock enable register */
#define RCC_APB2ENR_TIM1EN          0x00000001 //!< TIM1 clock enable
#define RCC_APB2ENR_TIM8EN          0x00000002 //!< TIM8 clock enable
#define RCC_APB2ENR_USART1EN        0x00000010 //!< USART1 clock enable
#define RCC_APB2ENR_USART6EN        0x00000020 //!< USART6 clock enable
#define RCC_APB2ENR_ADC1EN          0x00000100 //!< ADC1 clock enable
#define RCC_APB2ENR_ADC2EN          0x00000200 //!< ADC2 clock enable
#define RCC_APB2ENR_ADC3EN          0x00000400 //!< ADC3 clock enable
#define RCC_APB2ENR_SDIOEN          0x00000800 //!< SPI0 clock enable
#define RCC_APB2ENR_SPI1EN          0x00001000 //!< SPI1 clock enable
#define RCC_APB2ENR_SYSCFGEN        0x00004000 //!< System configuration controller clock enable
#define RCC_APB2ENR_TIM9EN          0x00010000 //!< TIM9 clock enable
#define RCC_APB2ENR_TIM10EN         0x00020000 //!< TIM10 clock enable
#define RCC_APB2ENR_TIM11EN         0x00040000 //!< TIM11 clock enable
/** @} */

/** @defgroup RCC_AHB1LPENR: (rcc Offset: 0x50) RCC AHB1 peripheral clock enable in low power mode register */
#define RCC_AHB1LPENR_GPIOALPEN     0x00000001 //!< IO port A clock enable during Sleep mode
#define RCC_AHB1LPENR_GPIOBLPEN     0x00000002 //!< IO port B clock enable during Sleep mode
#define RCC_AHB1LPENR_GPIOCLPEN     0x00000004 //!< IO port C clock enable during Sleep mode
#define RCC_AHB1LPENR_GPIODLPEN     0x00000008 //!< IO port D clock enable during Sleep mode
#define RCC_AHB1LPENR_GPIOELPEN     0x00000010 //!< IO port E clock enable during Sleep mode
#define RCC_AHB1LPENR_GPIOFLPEN     0x00000020 //!< IO port F clock enable during Sleep mode
#define RCC_AHB1LPENR_GPIOGLPEN     0x00000040 //!< IO port G clock enable during Sleep mode
#define RCC_AHB1LPENR_GPIOHLPEN     0x00000080 //!< IO port H clock enable during Sleep mode
#define RCC_AHB1LPENR_GPIOILPEN     0x00000100 //!< IO port I clock enable during Sleep mode
#define RCC_AHB1LPENR_CRCLPEN       0x00001000 //!< CRC clock enable during Sleep mode
#define RCC_AHB1LPENR_FLITFLPEN     0x00008000 //!< Flash interface clock enable during Sleep mode
#define RCC_AHB1LPENR_SRAM1LPEN     0x00010000 //!< SRAM 1 interface clock enable during Sleep mode
#define RCC_AHB1LPENR_SRAM2LPEN     0x00020000 //!< SRAM 2 interface clock enable during Sleep mode
#define RCC_AHB1LPENR_BKPSRAMLPEN   0x00040000 //!< Backup SRAM interface clock enable during Sleep mode
#define RCC_AHB1LPENR_DMA1LPEN      0x00200000 //!< DMA1 clock enable during Sleep mode
#define RCC_AHB1LPENR_DMA2LPEN      0x00400000 //!< DMA2 clock enable during Sleep mode
#define RCC_AHB1LPENR_ETHMACLPEN    0x02000000 //!< Ethernet MAC clock enable during Sleep mode
#define RCC_AHB1LPENR_ETHMACTXLPEN  0x04000000 //!< Ethernet transmission clock enable during Sleep mode
#define RCC_AHB1LPENR_ETHMACRXLPEN  0x08000000 //!< Ethernet reception clock enable during Sleep mode
#define RCC_AHB1LPENR_ETHMACPTPLPEN 0x10000000 //!< Ethernet PTP clock enable during Sleep mode
#define RCC_AHB1LPENR_OTGHSLPEN     0x20000000 //!< USB OTG HS clock enable during Sleep mode
#define RCC_AHB1LPENR_OTGHSULPILPEN 0x40000000 //!< USB OTG HS ULPI clock enable during Sleep mode
/** @} */

/** @defgroup RCC_AHB2LPENR: (rcc Offset: 0x54) RCC AHB2 peripheral clock enable in low power mode register */
#define RCC_AHB2LPENR_DCMILPEN      0x00000001 //!< Camera interface enable during Sleep mode
#define RCC_AHB2LPENR_CRYPLPEN      0x00000010 //!< Cryptography modules clock enable during Sleep mode
#define RCC_AHB2LPENR_HASHLPEN      0x00000020 //!< Hash modules clock enable during Sleep mode
#define RCC_AHB2LPENR_RNGLPEN       0x00000040 //!< Random number generator clock enable during Sleep mode
#define RCC_AHB2LPENR_OTGFSLPEN     0x00000080 //!< USB OTG FS clock enable during Sleep mode
/** @} */

/** @defgroup RCC_AHB3LPENR: (rcc Offset: 0x58) RCC AHB3 peripheral clock enable in low power mode register */
#define RCC_AHB3LPENR_FSMCLPEN      0x00000001 //!< Flexible static memory controller module clock enable during Sleep mode
/** @} */

/** @defgroup RCC_APB1LPENR: (rcc Offset: 0x60) RCC APB1 peripheral clock enable in low power mode register */
#define RCC_APB1LPENR_TIM2LPEN      0x00000001 //!< TIM2 clock enable during Sleep mode
#define RCC_APB1LPENR_TIM3LPEN      0x00000002 //!< TIM3 clock enable during Sleep mode
#define RCC_APB1LPENR_TIM4LPEN      0x00000004 //!< TIM4 clock enable during Sleep mode
#define RCC_APB1LPENR_TIM5LPEN      0x00000008 //!< TIM5 clock enable during Sleep mode
#define RCC_APB1LPENR_TIM6LPEN      0x00000010 //!< TIM6 clock enable during Sleep mode
#define RCC_APB1LPENR_TIM7LPEN      0x00000020 //!< TIM7 clock enable during Sleep mode
#define RCC_APB1LPENR_TIM12LPEN     0x00000040 //!< TIM12 clock enable during Sleep mode
#define RCC_APB1LPENR_TIM13LPEN     0x00000080 //!< TIM13 clock enable during Sleep mode
#define RCC_APB1LPENR_TIM14LPEN     0x00000100 //!< TIM14 clock enable during Sleep mode
#define RCC_APB1LPENR_WWDGLPEN      0x00000800 //!< Window watchdog clock enable during Sleep mode
#define RCC_APB1LPENR_SPI2LPEN      0x00004000 //!< SPI2 clock enable during Sleep mode
#define RCC_APB1LPENR_SPI3LPEN      0x00008000 //!< SPI3 clock enable during Sleep mode
#define RCC_APB1LPENR_USART2LPEN    0x00020000 //!< UART2 clock enable during Sleep mode
#define RCC_APB1LPENR_USART3LPEN    0x00040000 //!< UART3 clock enable during Sleep mode
#define RCC_APB1LPENR_UART4LPEN     0x00080000 //!< UART4 clock enable during Sleep mode
#define RCC_APB1LPENR_UART5LPEN     0x00100000 //!< UART5 clock enable during Sleep mode
#define RCC_APB1LPENR_I2C1LPEN      0x00200000 //!< I2C1 clock enable during Sleep mode
#define RCC_APB1LPENR_I2C2LPEN      0x00400000 //!< I2C2 clock enable during Sleep mode
#define RCC_APB1LPENR_I2C3LPEN      0x00800000 //!< I2C3 clock enable during Sleep mode
#define RCC_APB1LPENR_CAN1LPEN      0x02000000 //!< CAN 1 clock enable during Sleep mode
#define RCC_APB1LPENR_CAN2LPEN      0x04000000 //!< CAN 2 clock enable during Sleep mode
#define RCC_APB1LPENR_PWRLPEN       0x10000000 //!< Power interface clock enable during Sleep mode
#define RCC_APB1LPENR_DACLPEN       0x20000000 //!< DAC interface clock enable during Sleep mode
/** @} */

/** @defgroup RCC_APB2LPENR: (rcc Offset: 0x64) RCC APB2 peripheral clock enable in low power mode register */
#define RCC_APB2LPENR_TIM1LPEN      0x00000001 //!< TIM1 clock enable during Sleep mode
#define RCC_APB2LPENR_TIM8LPEN      0x00000002 //!< TIM8 clock enable during Sleep mode
#define RCC_APB2LPENR_USART1LPEN    0x00000010 //!< USART1 clock enable during Sleep mode
#define RCC_APB2LPENR_USART6LPEN    0x00000020 //!< USART6 clock enable during Sleep mode
#define RCC_APB2LPENR_ADC1LPEN      0x00000100 //!< ADC1 clock enable during Sleep mode
#define RCC_APB2LPENR_ADC2PEN       0x00000200 //!< ADC2 clock enable during Sleep mode
#define RCC_APB2LPENR_ADC3LPEN      0x00000400 //!< ADC3 clock enable during Sleep mode
#define RCC_APB2LPENR_SDIOLPEN      0x00000800 //!< SPI0 clock enable during Sleep mode
#define RCC_APB2LPENR_SPI1LPEN      0x00001000 //!< SPI1 clock enable during Sleep mode
#define RCC_APB2LPENR_SYSCFGLPEN    0x00004000 //!< System configuration controller clock enable during Sleep mode
#define RCC_APB2LPENR_TIM9LPEN      0x00010000 //!< TIM9 clock enable during Sleep mode
#define RCC_APB2LPENR_TIM10LPEN     0x00020000 //!< TIM10 clock enable during Sleep mode
#define RCC_APB2LPENR_TIM11LPEN     0x00040000 //!< TIM11 clock enable during Sleep mode
/** @} */

/** @defgroup RCC_BDCR:		 (rcc Offset: 0x70) Backup domain control register 	*/
#define RCC_BDCR_LSEON              0x00000001 //!< External low-speed oscillator enable
#define RCC_BDCR_LSERDY             0x00000002 //!< External low-speed oscillator ready
#define RCC_BDCR_LSEBYP             0x00000004 //!< External low-speed oscillator bypass

#define RCC_BDCR_RTCSEL             0x00000300 //!< RTCSEL[1:0]: RTC clock source selection
#define RCC_BDCR_RTCSEL_NOCLOCK     0x00000000 //!< No clock
#define RCC_BDCR_RTCSEL_LSE         0x00000100 //!< LSE oscillator clock used as RTC clock
#define RCC_BDCR_RTCSEL_LSI         0x00000200 //!< LSI oscillator clock used as RTC clock
#define RCC_BDCR_RTCSEL_HSE         0x00000300 //!< HSE oscillator clock divided by a programmable prescaler (selection through the RTCPRE[4:0]

#define RCC_BDCR_RTCEN              0x00008000 //!< RTC clock enable
#define RCC_BDCR_BDRST              0x00010000 //!< Backup domain software reset
/** @} */

/** @defgroup RCC_CSR:		 (rcc Offset: 0x74) Control/status register 		*/
#define RCC_CSR_LSION               0x00000001 //!< Internal low-speed oscillator enable
#define RCC_CSR_LSIRDY              0x00000002 //!< Internal low-speed oscillator ready
#define RCC_CSR_RMVF                0x01000000 //!< Remove reset flag
#define RCC_CSR_BORRSTF             0x02000000 //!< BOR reset flag
#define RCC_CSR_PADRSTF             0x04000000 //!< PIN reset flag
#define RCC_CSR_PORRSTF             0x08000000 //!< POR/PDR reset flag
#define RCC_CSR_SFTRSTF             0x10000000 //!< Software reset flag
#define RCC_CSR_WDGRSTF             0x20000000 //!< Independent watchdog reset flag
#define RCC_CSR_WWDGRSTF            0x40000000 //!< Window watchdog reset flag
#define RCC_CSR_LPWRRSTF            0x80000000 //!< Low-power reset flag
/** @} */

/** @defgroup RCC_SSCGR:     (rcc Offset: 0x80) RCC spread spectrum clock generation register */
#define RCC_SSCGR_MODPER            0x00001FFF //!< Modulation period
#define RCC_SSCGR_INCSTEP           0x0FFFE000 //!< Incrementation step
#define RCC_SSCGR_SPREADSEL         0x40000000 //!< Spread Select
#define RCC_SSCGR_SPREADSEL_CENTER  0x00000000 //!<  Center spread
#define RCC_SSCGR_SPREADSEL_DOWN    0x40000000 //!<  Down spread
#define RCC_SSCGR_SSCGEN            0x80000000 //!< Spread spectrum modulation enable
/** @} */

/** @defgroup RCC_PLLI2SCFGR:(rcc Offset: 0x84) RCC PLLI2S configuration register */
#define RCC_PLLI2SCFGR_PLLI2SN      0x00007FC0 //!< PLLI2S multiplication factor for VCO
#define RCC_PLLI2SCFGR_PLLI2SR      0x70000000 //!< PLLI2S division factor for I2S clocks
/** @} */


/** @} */ // @defgroup RCC_regs_define

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) Startup
   Timeout value
   */
#define HSE_STARTUP_TIMEOUT   0x0500 /*!< Time out for HSE start up */



#endif /* RCC_F2_H_ */
