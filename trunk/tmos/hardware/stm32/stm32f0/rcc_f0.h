/*
 * rcc_f0.h
 *
 *  Created on: Oct 27, 2014
 *      Author: miro
 */

#ifndef RCC_F0_H_
#define RCC_F0_H_

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
  __IO uint32_t RCC_AHBENR;       	//!< (rcc Offset: 0x14) AHB peripheral clock enable register
  __IO uint32_t RCC_APB2ENR;		//!< (rcc Offset: 0x18) APB2 peripheral clock enable register
  __IO uint32_t RCC_APB1ENR;		//!< (rcc Offset: 0x1C) APB1 peripheral clock enable register
  __IO uint32_t RCC_BDCR;			//!< (rcc Offset: 0x20) Backup domain control register
  __IO uint32_t RCC_CSR;			//!< (rcc Offset: 0x24) Control/status register
  __IO uint32_t RCC_AHBRSTR;		//!< (rcc Offset: 0x28) AHB peripheral reset register
  __IO uint32_t RCC_CFGR2;			//!< (rcc offset: 0x2C) Clock configuration register 2
  __IO uint32_t RCC_CFGR3;			//!< (rcc offset: 0x30) Clock configuration register 3
  __IO uint32_t RCC_CR2;			//!< (rcc Offset: 0x34) Clock control register 2

} RCC_TypeDef;

/*******************************************************************************
 * @defgroup RCC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup RCC_CR:		(rcc Offset: 0x00) Clock control register		  */
#define RCC_CR_PLLRDY           	0x02000000 //!< PLL clock ready flag
#define RCC_CR_PLLON            	0x01000000 //!< PLL enable
#define RCC_CR_CSSON            	0x00080000 //!< Clock Security System enable
#define RCC_CR_HSEBYP           	0x00040000 //!< HSE crystal oscillator bypass
#define RCC_CR_HSERDY           	0x00020000 //!< HSE clock ready flag
#define RCC_CR_HSEON            	0x00010000 //!< HSE clock enable
#define RCC_CR_HSICAL           	0x0000FF00 //!< HSI clock calibration
#define RCC_CR_HSITRIM          	0x000000F8 //!< HSI clock trimming
#define RCC_CR_HSIRDY           	0x00000002 //!< HSI clock ready flag
#define RCC_CR_HSION        		0x00000001 //!< HSI clock enable
/** @} */

/** @defgroup RCC_CFGR:		(rcc offset: 0x04) Clock configuration register	  */
#define RCC_CFGR_MCO                0x07000000 //!< Microcontroller clock output
#define RCC_CFGR_MCO_DISABLED       0x00000000 //!<  MCO output disabled, no clock on MCO
#define RCC_CFGR_MCO_HSI14	        0x01000000 //!<  Internal RC 14 MHz (HSI14) oscillator clock selected
#define RCC_CFGR_MCO_LSI	        0x02000000 //!<  Internal low speed (LSI) oscillator clock selected
#define RCC_CFGR_MCO_LSE	        0x03000000 //!<  External low speed (LSE) oscillator clock selected
#define RCC_CFGR_MCO_SYSCLK         0x04000000 //!<  System clock selected
#define RCC_CFGR_MCO_HSI	        0x05000000 //!<  Internal RC 8 MHz (HSI) oscillator clock selected
#define RCC_CFGR_MCO_HSE            0x06000000 //!<  External 4-32 MHz (HSE) oscillator clock selected
#define RCC_CFGR_MCO_PLL            0x07000000 //!<  PLL clock divided by 2 selected
#define RCC_CFGR_PLLMUL             0x003C0000 //!< PLL multiplication factor
#define  RCC_CFGR_PLLMUL_Get(x)    (((x) >> 18) & 0xF)
#define  RCC_CFGR_PLLMUL_Set(x)    (((x) & 0xF) << 18)
#define RCC_CFGR_PLLXTPRE           0x00020000 //!< HSE divider for PLL input clock
#define RCC_CFGR_PLLSRC             0x00010000 //!< PLL entry clock source
#define RCC_CFGR_ADCPRE             0x00004000 //!< ADC prescaler

/*!< PPRE configuration */
#define RCC_CFGR_PPRE        	  	0x00000700 //!< PCLK prescaler
#define RCC_CFGR_PPRE_DIV1     		0x00000000 //!<  HCLK not divided
#define RCC_CFGR_PPRE_DIV2 	    	0x00000400 //!<  HCLK divided by 2
#define RCC_CFGR_PPRE_DIV4  	   	0x00000500 //!<  HCLK divided by 4
#define RCC_CFGR_PPRE_DIV8     		0x00000600 //!<  HCLK divided by 8
#define RCC_CFGR_PPRE_DIV16 	   	0x00000700 //!<  HCLK divided by 8
#define RCC_CFGR_PPRE_Get(x)    	(((x)>>8)& 7)   //!< PPRE[2:0] get

/*!< HPRE configuration */
#define RCC_CFGR_HPRE           	0x000000F0 //!< HLCK prescaler
#define RCC_CFGR_HPRE_DIV1      	0x00000000 //!<  SYSCLK not divided
#define RCC_CFGR_HPRE_DIV2      	0x00000080 //!<  SYSCLK divided by 2
#define RCC_CFGR_HPRE_DIV4      	0x00000090 //!<  SYSCLK divided by 4
#define RCC_CFGR_HPRE_DIV8      	0x000000A0 //!<  SYSCLK divided by 8
#define RCC_CFGR_HPRE_DIV16     	0x000000B0 //!<  SYSCLK divided by 16
#define RCC_CFGR_HPRE_DIV64     	0x000000C0 //!<  SYSCLK divided by 64
#define RCC_CFGR_HPRE_DIV128    	0x000000D0 //!<  SYSCLK divided by 128
#define RCC_CFGR_HPRE_DIV256    	0x000000E0 //!<  SYSCLK divided by 256
#define RCC_CFGR_HPRE_DIV512    	0x000000F0 //!<  SYSCLK divided by 512
#define RCC_CFGR_HPRE_Get(x)        (((x)>>4)&0xF)   //!< HPRE[3:0] get

/*!< SWS configuration */
#define RCC_CFGR_SWS            	0x0000000C //!< SWS[1:0] bits (System Clock Switch Status)
#define RCC_CFGR_SWS_HSI        	0x00000000 //!<  HSI oscillator used as system clock
#define RCC_CFGR_SWS_HSE        	0x00000004 //!<  HSE oscillator used as system clock
#define RCC_CFGR_SWS_PLL        	0x00000008 //!<  PLL used as system clock

/*!< SW configuration */
#define RCC_CFGR_SW    				0x00000003 //!< SW[1:0] bits (System clock Switch)
#define RCC_CFGR_SW_HSI       		0x00000000 //!<  HSI selected as system clock
#define RCC_CFGR_SW_HSE         	0x00000001 //!<  HSE selected as system clock
#define RCC_CFGR_SW_PLL         	0x00000002 //!<  PLL selected as system clock
/** @} */

/** @defgroup RCC_CIR:		(rcc Offset: 0x08) Clock interrupt register		  */
#define RCC_CIR_CSSC                0x00800000 //!< Clock Security System Interrupt Clear
#define RCC_CIR_HSI14RDYC           0x00200000 //!< HSI 14 MHz Ready Interrupt Clear
#define RCC_CIR_PLLRDYC             0x00100000 //!< PLL Ready Interrupt Clear
#define RCC_CIR_HSERDYC             0x00080000 //!< HSE Ready Interrupt Clear
#define RCC_CIR_HSIRDYC             0x00040000 //!< HSI Ready Interrupt Clear
#define RCC_CIR_LSERDYC             0x00020000 //!< LSE Ready Interrupt Clear
#define RCC_CIR_LSIRDYC             0x00010000 //!< LSI Ready Interrupt Clear
#define RCC_CIR_HSI14RDYIE          0x00002000 //!< HSI14 ready interrupt enable
#define RCC_CIR_PLLRDYIE            0x00001000 //!< PLL Ready Interrupt Enable
#define RCC_CIR_HSERDYIE            0x00000800 //!< HSE Ready Interrupt Enable
#define RCC_CIR_HSIRDYIE            0x00000400 //!< HSI Ready Interrupt Enable
#define RCC_CIR_LSERDYIE            0x00000200 //!< LSE Ready Interrupt Enable
#define RCC_CIR_LSIRDYIE            0x00000100 //!< LSI Ready Interrupt Enable
#define RCC_CIR_CSSF                0x00000080 //!< Clock Security System Interrupt flag
#define RCC_CIR_HSI14RDYF           0x00000020 //!< HSI14 ready interrupt flag
#define RCC_CIR_PLLRDYF             0x00000010 //!< PLL Ready Interrupt flag
#define RCC_CIR_HSERDYF             0x00000008 //!< HSE Ready Interrupt flag
#define RCC_CIR_HSIRDYF             0x00000004 //!< HSI Ready Interrupt flag
#define RCC_CIR_LSERDYF             0x00000002 //!< LSE Ready Interrupt flag
#define RCC_CIR_LSIRDYF             0x00000001 //!< LSI Ready Interrupt flag
/** @} */

/** @defgroup RCC_APB2RSTR:	(rcc Offset: 0x0C) APB2 peripheral reset register */
#define RCC_APB2RSTR_DBGMCURST      0x00400000 //!< Debug MCU reset
#define RCC_APB2RSTR_TIM17RST       0x00040000 //!< TIM17 timer reset
#define RCC_APB2RSTR_TIM16RST       0x00020000 //!< TIM16 timer reset
#define RCC_APB2RSTR_TIM15RST       0x00010000 //!< TIM15 timer reset
#define RCC_APB2RSTR_USART1RST      0x00004000 //!< USART1 reset
#define RCC_APB2RSTR_SPI1RST        0x00001000 //!< SPI1 reset
#define RCC_APB2RSTR_TIM1RST        0x00000800 //!< TIM1 timer reset
#define RCC_APB2RSTR_ADCRST         0x00000200 //!< ADC interface reset
#define RCC_APB2RSTR_SYSCFGCOMPRST  0x00000001 //!< SYSCFG and COMP reset
/** @} */

/** @defgroup RCC_APB1RSTR:	(rcc Offset: 0x10) APB1 peripheral reset register */
#define RCC_APB1RSTR_CECRST         0x40000000 //!< HDMI CEC reset
#define RCC_APB1RSTR_DACRST         0x20000000 //!< DAC reset
#define RCC_APB1RSTR_PWRRST         0x10000000 //!< Power interface reset
#define RCC_APB1RSTR_I2C2RST        0x00400000 //!< I2C2 reset
#define RCC_APB1RSTR_I2C1RST        0x00200000 //!< I2C1 reset
#define RCC_APB1RSTR_USART2RST      0x00020000 //!< USART 2 reset
#define RCC_APB1RSTR_SPI2RST        0x00004000 //!< SPI 2 reset
#define RCC_APB1RSTR_WWDGEN         0x00000800 //!< Window watchdog reset
#define RCC_APB1RSTR_TIM14RST       0x00000100 //!< TIM14 reset
#define RCC_APB1RSTR_TIM6RST        0x00000010 //!< TIM6 reset
#define RCC_APB1RSTR_TIM3RST        0x00000002 //!< TIM3 reset
#define RCC_APB1RSTR_TIM2RST        0x00000001 //!< TIM2 reset
/** @} */

/** @defgroup RCC_AHBENR:	(rcc Offset: 0x14) AHB peripheral clock enable register	*/
#define RCC_AHBENR_TSCEN            0x01000000 //!< Touch sensing controller clock enable
#define RCC_AHBENR_IOPFEN           0x00400000 //!< I/O port F clock enable
#define RCC_AHBENR_IOPDEN           0x00100000 //!< I/O port D clock enable
#define RCC_AHBENR_IOPCEN           0x00080000 //!< I/O port C clock enable
#define RCC_AHBENR_IOPBEN           0x00040000 //!< I/O port B clock enable
#define RCC_AHBENR_IOPAEN           0x00020000 //!< I/O port A clock enable
#define RCC_AHBENR_CRCEN            0x00000040 //!< CRC clock enable
#define RCC_AHBENR_FLITFEN          0x00000010 //!< FLITF clock enable
#define RCC_AHBENR_SRAMEN           0x00000004 //!< SRAM interface clock enable during Sleep mode
#define RCC_AHBENR_DMAEN            0x00000001 //!< DMA clock enable
/** @} */

/** @defgroup RCC_APB2ENR:	(rcc Offset: 0x18) APB2 peripheral clock enable register */
#define RCC_APB2ENR_DBGMCUEN        0x00400000 //!< MCU debug module clock enable
#define RCC_APB2ENR_TIM17EN         0x00040000 //!< TIM17 timer clock enable
#define RCC_APB2ENR_TIM16EN         0x00020000 //!< TIM16 timer clock enable
#define RCC_APB2ENR_TIM15EN         0x00010000 //!< TIM15 timer clock enable
#define RCC_APB2ENR_USART1EN        0x00004000 //!< USART1 clock enable
#define RCC_APB2ENR_SPI1EN          0x00001000 //!< SPI1 clock enable
#define RCC_APB2ENR_TIM1EN          0x00000800 //!< TIM1 timer clock enable
#define RCC_APB2ENR_ADCEN           0x00000200 //!< ADC interface clock enable
#define RCC_APB2ENR_SYSCFGCOMPEN    0x00000001 //!< SYSCFG & COMP clock enable
/** @} */

/** @defgroup RCC_APB1ENR:	(rcc Offset: 0x1C) APB1 peripheral clock enable register */
#define RCC_APB1ENR_CECEN           0x40000000 //!< HDMI CEC interface clock enable
#define RCC_APB1ENR_DACEN           0x20000000 //!< DAC interface clock enable
#define RCC_APB1ENR_PWREN           0x10000000 //!< Power interface clock enable
#define RCC_APB1ENR_I2C2EN          0x00400000 //!< I2C2 clock enable
#define RCC_APB1ENR_I2C1EN          0x00200000 //!< I2C1 clock enable
#define RCC_APB1ENR_USART2EN        0x00020000 //!< USART2 clock enable
#define RCC_APB1ENR_SPI2EN          0x00004000 //!< SPI2 clock enable
#define RCC_APB1ENR_WWDGEN          0x00000800 //!< Window watchdog clock enable
#define RCC_APB1ENR_TIM14EN         0x00000100 //!< TIM14 timer clock enable
#define RCC_APB1ENR_TIM6EN          0x00000010 //!< TIM6 timer clock enable
#define RCC_APB1ENR_TIM3EN          0x00000002 //!< TIM3 timer clock enable
#define RCC_APB1ENR_TIM2EN          0x00000001 //!< TIM2 timer clock enable
/** @} */

/** @defgroup RCC_BDCR:		(rcc Offset: 0x20) Backup domain control register */
#define RCC_BDCR_BDRST              0x00010000 //!< Backup domain software reset
#define RCC_BDCR_RTCEN              0x00008000 //!< RTC clock enable
#define RCC_BDCR_RTCSEL             0x00000300 //!< RTCSEL[1:0]: RTC clock source selection
#define RCC_BDCR_RTCSEL_NOCLOCK     0x00000000 //!<  No clock
#define RCC_BDCR_RTCSEL_LSE         0x00000100 //!<  LSE oscillator clock used as RTC clock
#define RCC_BDCR_RTCSEL_LSI         0x00000200 //!<  LSI oscillator clock used as RTC clock
#define RCC_BDCR_RTCSEL_HSE         0x00000300 //!<  HSE oscillator clock divided by 32 used as RTC clock
#define RCC_BDCR_LSEDRV             0x00000018 //!< LSE oscillator drive capability
#define RCC_BDCR_LSEDRV_0           0x00000000 //!<  ‘Xtal mode’ lower driving capability
#define RCC_BDCR_LSEDRV_1           0x00000008 //!<  ‘Xtal mode’ medium low driving capability
#define RCC_BDCR_LSEDRV_2           0x00000010 //!<  ‘Xtal mode’ medium high driving capability
#define RCC_BDCR_LSEDRV_3           0x00000018 //!<  ‘Xtal mode’ higher driving capability (reset value)
#define RCC_BDCR_LSEBYP             0x00000004 //!< External low-speed oscillator bypass
#define RCC_BDCR_LSERDY             0x00000002 //!< External low-speed oscillator ready
#define RCC_BDCR_LSEON              0x00000001 //!< External low-speed oscillator enable
/** @} */

/** @defgroup RCC_CSR:		(rcc Offset: 0x24) Control/status register		  */
#define RCC_CSR_LPWRRSTF            0x80000000 //!< Low-power reset flag
#define RCC_CSR_WWDGRSTF            0x40000000 //!< Window watchdog reset flag
#define RCC_CSR_IWDGRSTF            0x20000000 //!< Independent watchdog reset flag
#define RCC_CSR_SFTRSTF             0x10000000 //!< Software reset flag
#define RCC_CSR_PORRSTF             0x08000000 //!< POR/PDR reset flag
#define RCC_CSR_PINRSTF             0x04000000 //!< PIN reset flag
#define RCC_CSR_OBLRSTF             0x02000000 //!< Option byte loader reset flag
#define RCC_CSR_RMVF                0x01000000 //!< Remove reset flag
#define RCC_CSR_PORV18RSTF          0x00800000 //!< POR/PDR: Reset flag of the V18 voltage domain
#define RCC_CSR_LSIRDY              0x00000002 //!< Internal low-speed oscillator ready
#define RCC_CSR_LSION               0x00000001 //!< Internal low-speed oscillator enable
/** @} */

/** @defgroup RCC_AHBRSTR:	(rcc Offset: 0x28) AHB peripheral reset register  */
#define RCC_AHBRSTR_TSCRST          0x01000000 //!< Touch sensing controller reset
#define RCC_AHBRSTR_IOPFRST         0x00400000 //!< I/O port F reset
#define RCC_AHBRSTR_IOPDRST         0x00100000 //!< I/O port D reset
#define RCC_AHBRSTR_IOPCRST         0x00080000 //!< I/O port C reset
#define RCC_AHBRSTR_IOPBRST         0x00040000 //!< I/O port B reset
#define RCC_AHBRSTR_IOPARST         0x00020000 //!< I/O port A reset
/** @} */

/** @defgroup RCC_CFGR2:	(rcc offset: 0x2C) Clock configuration register 2 */
#define RCC_CFGR2_PREDIV	        0x0000000F //!< PREDIV division factor
/** @} */

/** @defgroup RCC_CFGR3:	(rcc offset: 0x30) Clock configuration register 3 */
#define RCC_CFGR3_ADCSW 	        0x00000100 //!< ADC clock source selection
#define RCC_CFGR3_ADCSW_HSI14       0x00000000 //!<  HSI14 clock selected as ADC kernel clock (default)
#define RCC_CFGR3_ADCSW_PLCK        0x00000100 //!<  PLCK divided by 2 or 4, selected as ADC clock
#define RCC_CFGR3_CECSW 	        0x00000040 //!< HDMI CEC clock source selection
#define RCC_CFGR3_CECSW_HSI_DIV244  0x00000000 //!<  HSI clock, divided by 244, selected as CEC clock (default)
#define RCC_CFGR3_CECSW_LSE         0x00000040 //!<  LSE clock selected as CEC clock
#define RCC_CFGR3_I2C1SW 	        0x00000010 //!< I2C1 clock source selection
#define RCC_CFGR3_I2C1SW_HSI        0x00000000 //!<  HSI clock selected as I2C1 clock source (default)
#define RCC_CFGR3_I2C1SW_SYSCLK     0x00000010 //!<  System clock (SYSCLK) selected as I2C1 clock
#define RCC_CFGR3_USART1SW 	        0x00000003 //!< USART1 clock source selection
#define RCC_CFGR3_USART1SW_PCLK     0x00000000 //!<  PCLK selected as USART1 clock source (default)
#define RCC_CFGR3_USART1SW_SYSCLK   0x00000001 //!<  System clock (SYSCLK) selected as USART1 clock
#define RCC_CFGR3_USART1SW_LSE      0x00000002 //!<  LSE clock selected as USART1 clock
#define RCC_CFGR3_USART1SW_HSI      0x00000003 //!<  HSI clock selected as USART1 clock
/** @} */

/** @defgroup RCC_CR2:		(rcc Offset: 0x34) Clock control register 2		  */
#define RCC_CR2_HSI14CAL		    0x0000FF00 //!< HSI14 clock calibration
#define RCC_CR2_HSI14TRIM		    0x000000F8 //!< HSI14 clock trimming
#define RCC_CR2_HSI14DIS		    0x00000004 //!< HSI14 clock request from ADC disable
#define RCC_CR2_HSI14RDY		    0x00000002 //!< HSI14 clock ready flag
#define RCC_CR2_HSI14ON 		    0x00000001 //!< HSI14 clock enable
/** @} */




/** @} */ // @defgroup RCC_regs_define

/**
 * @brief In the following line adjust the value of External High Speed oscillator (HSE)
   used in your application

   Tip: To avoid modifying this file each time you need to use different HSE, you
        can define the HSE value in your toolchain compiler preprocessor.
  */
#ifndef HSE_VALUE
#define HSE_VALUE            8000000 /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) Startup
   Timeout value
   */
#ifndef HSE_STARTUP_TIMEOUT
#define HSE_STARTUP_TIMEOUT  0x0500   /*!< Time out for HSE start up */
#endif /* HSE_STARTUP_TIMEOUT */

#ifndef HSI_VALUE
#define HSI_VALUE            8000000 /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#ifndef LSE_VALUE
#define LSE_VALUE            32768 /*!< Value of the LSE oscillator in Hz*/
#endif


typedef struct
{
	uint32_t SYSCLK_Frequency; 	//!< SYSCLK clock frequency expressed in Hz
	uint32_t HCLK_Frequency; 	//!< HCLK clock frequency expressed in Hz
	uint32_t PCLK1_Frequency; 	//!< PCLK1 clock frequency expressed in Hz
	uint32_t PCLK2_Frequency; 	//!< PCLK2 clock frequency expressed in Hz
} RCC_ClocksTypeDef;


void RCCPeripheralReset(unsigned int periph_id);
void RCCPeripheralEnable(unsigned int periph_id);
void RCCPeripheralDisable(unsigned int periph_id);

void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);




#endif /* RCC_F0_H_ */
