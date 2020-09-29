/**************************************************************************//**
 * @ingroup	hardware_stm32h7_rcc
 * @file	hardware/stm32/stm32h7/rcc_h7.h
 * @brief  	STM32H7 rcc
 * @date    29. October 2012
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_rcc  Reset and Clock Control (RCC)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 Reset and Clock Control (RCC)
 * @{
 *
 ******************************************************************************/

#ifndef RCC_H7_H_
#define RCC_H7_H_

#include <mcu_inc.h>
#include <tmos_types.h>

// revision V by default
#ifndef STM32_H7_CPU_REVISION_Y
#define STM32_H7_CPU_REVISION_Y 1
#endif
#ifndef STM32_H7_CPU_REVISION_V
#define STM32_H7_CPU_REVISION_V 2
#endif
#ifndef STM32_H7_CPU_REVISION
#define STM32_H7_CPU_REVISION STM32_H7_CPU_REVISION_V
#endif

/***************************************************************************//**
 *  Reset and Clock Control registers
 ******************************************************************************/
typedef struct
{
	__IO uint32_t RCC_CR; 				//!< (rcc Offset: 0x000) RCC Source Control Register
#if STM32_H7_CPU_REVISION == STM32_H7_CPU_REVISION_V
	__IO uint32_t RCC_HSICFGR;			//!< (rcc Offset: 0x004) RCC HSI configuration register
#else
	__IO uint32_t RCC_ICSCR;			//!< (rcc Offset: 0x004) RCC Internal Clock Source Calibration Register
#endif
	__IO uint32_t RCC_CRRCR;			//!< (rcc Offset: 0x008) Clock Recovery RC Register
#if STM32_H7_CPU_REVISION == STM32_H7_CPU_REVISION_V
	__IO uint32_t RCC_CSICFGR;			//!< (rcc Offset: 0x00C) CSI Clock Calibration Register
#else
	__IO uint32_t reserved0;			//!< (rcc Offset: 0x00C) reserved
#endif
	__IO uint32_t RCC_CFGR;				//!< (rcc Offset: 0x010) RCC clock configuration register
	__IO uint32_t reserved1;			//!< (rcc Offset: 0x014) reserved
	__IO uint32_t RCC_D1CFGR;			//!< (rcc Offset: 0x018) RCC Domain 1 configuration register
	__IO uint32_t RCC_D2CFGR;			//!< (rcc Offset: 0x01C) RCC Domain 2 configuration register
	__IO uint32_t RCC_D3CFGR;			//!< (rcc Offset: 0x020) RCC Domain 3 configuration register
	__IO uint32_t reserved2; 			//!< (rcc Offset: 0x024) reserved
	__IO uint32_t RCC_PLLCKSELR; 		//!< (rcc Offset: 0x028) RCC PLLs Clock Source Selection Register
	__IO uint32_t RCC_PLLCFGR; 			//!< (rcc Offset: 0x02C) RCC PLLs  Configuration Register
	__IO uint32_t RCC_PLL1DIVR; 		//!< (rcc Offset: 0x030) RCC PLL1 Dividers Configuration Register
	__IO uint32_t RCC_PLL1FRACR; 		//!< (rcc Offset: 0x034) RCC PLL1 Fractional Divider Configuration Register
	__IO uint32_t RCC_PLL2DIVR; 		//!< (rcc Offset: 0x038) RCC PLL2 Dividers Configuration Register
	__IO uint32_t RCC_PLL2FRACR; 		//!< (rcc Offset: 0x03C) RCC PLL2 Fractional Divider Configuration Register
	__IO uint32_t RCC_PLL3DIVR; 		//!< (rcc Offset: 0x040) RCC PLL3 Dividers Configuration Register
	__IO uint32_t RCC_PLL3FRACR; 		//!< (rcc Offset: 0x044) RCC PLL3 Fractional Divider Configuration Register
	__IO uint32_t reserved3; 			//!< (rcc Offset: 0x048) reserved
	__IO uint32_t RCC_D1CCIPR;			//!< (rcc Offset: 0x04C) RCC Domain 1 Kernel Clock Configuration Register
	__IO uint32_t RCC_D2CCIP1R; 		//!< (rcc Offset: 0x050) RCC Domain 2 Kernel Clock Configuration Register
	__IO uint32_t RCC_D2CCIP2R; 		//!< (rcc Offset: 0x054) RCC Domain 2 Kernel Clock Configuration Register
	__IO uint32_t RCC_D3CCIPR; 			//!< (rcc Offset: 0x058) RCC Domain 3 Kernel Clock Configuration Register
	__IO uint32_t reserved4; 			//!< (rcc Offset: 0x05C) reserved
	__IO uint32_t RCC_CIER; 			//!< (rcc Offset: 0x060) RCC Clock Source Interrupt Enable Register
	__IO uint32_t RCC_CIFR; 			//!< (rcc Offset: 0x064) RCC Clock Source Interrupt Flag Register
	__IO uint32_t RCC_CICR; 			//!< (rcc Offset: 0x068) RCC Clock Source Interrupt Clear Register
	__IO uint32_t reserved5; 			//!< (rcc Offset: 0x06C) Reserved
	__IO uint32_t RCC_BDCR; 			//!< (rcc Offset: 0x070) RCC Vswitch Backup Domain Control Register
	__IO uint32_t RCC_CSR; 				//!< (rcc Offset: 0x074) RCC clock control & status register
	__IO uint32_t reserved6; 			//!< (rcc Offset: 0x078) reserved
	__IO uint32_t RCC_AHB3RSTR; 		//!< (rcc Offset: 0x07C) RCC AHB3 peripheral reset register
	__IO uint32_t RCC_AHB1RSTR; 		//!< (rcc Offset: 0x080) RCC AHB1 peripheral reset register
	__IO uint32_t RCC_AHB2RSTR; 		//!< (rcc Offset: 0x084) RCC AHB2 peripheral reset register
	__IO uint32_t RCC_AHB4RSTR; 		//!< (rcc Offset: 0x088) RCC AHB4 peripheral reset register
	__IO uint32_t RCC_APB3RSTR; 		//!< (rcc Offset: 0x08C) RCC APB3 peripheral reset register
	__IO uint32_t RCC_APB1LRSTR; 		//!< (rcc Offset: 0x090) RCC APB1 peripheral reset Low Word register
	__IO uint32_t RCC_APB1HRSTR; 		//!< (rcc Offset: 0x094) RCC APB1 peripheral reset High Word register
	__IO uint32_t RCC_APB2RSTR; 		//!< (rcc Offset: 0x098) RCC APB2 peripheral reset register
	__IO uint32_t RCC_APB4RSTR; 		//!< (rcc Offset: 0x09C) RCC APB4 peripheral reset register
	__IO uint32_t RCC_GCR; 				//!< (rcc Offset: 0x0A0) RCC RCC Global Control  Register
	__IO uint32_t reserved8; 			//!< (rcc Offset: 0x0A4) reserved
	__IO uint32_t RCC_D3AMR; 			//!< (rcc Offset: 0x0A8) RCC Domain 3 Autonomous Mode Register
	__IO uint32_t reserved11[9]; 		//!< (rcc Offset: 0x0AC) reserved 0x0AC-0x0CC
	__IO uint32_t RCC_RSR; 				//!< (rcc Offset: 0x0D0) RCC Reset status register
	__IO uint32_t RCC_AHB3ENR; 			//!< (rcc Offset: 0x0D4) RCC AHB3 peripheral clock  register
	__IO uint32_t RCC_AHB1ENR; 			//!< (rcc Offset: 0x0D8) RCC AHB1 peripheral clock  register
	__IO uint32_t RCC_AHB2ENR; 			//!< (rcc Offset: 0x0DC) RCC AHB2 peripheral clock  register
	__IO uint32_t RCC_AHB4ENR; 			//!< (rcc Offset: 0x0E0) RCC AHB4 peripheral clock  register
	__IO uint32_t RCC_APB3ENR; 			//!< (rcc Offset: 0x0E4) RCC APB3 peripheral clock  register
	__IO uint32_t RCC_APB1LENR; 		//!< (rcc Offset: 0x0E8) RCC APB1 peripheral clock  Low Word register
	__IO uint32_t RCC_APB1HENR; 		//!< (rcc Offset: 0x0EC) RCC APB1 peripheral clock  High Word register
	__IO uint32_t RCC_APB2ENR; 			//!< (rcc Offset: 0x0F0) RCC APB2 peripheral clock  register
	__IO uint32_t RCC_APB4ENR; 			//!< (rcc Offset: 0x0F4) RCC APB4 peripheral clock  register
	__IO uint32_t reserved12; 			//!< (rcc Offset: 0x0F8) reserved
	__IO uint32_t RCC_AHB3LPENR; 		//!< (rcc Offset: 0x0FC) RCC AHB3 peripheral sleep clock  register
	__IO uint32_t RCC_AHB1LPENR; 		//!< (rcc Offset: 0x100) RCC AHB1 peripheral sleep clock  register
	__IO uint32_t RCC_AHB2LPENR; 		//!< (rcc Offset: 0x104) RCC AHB2 peripheral sleep clock  register
	__IO uint32_t RCC_AHB4LPENR; 		//!< (rcc Offset: 0x108) RCC AHB4 peripheral sleep clock  register
	__IO uint32_t RCC_APB3LPENR; 		//!< (rcc Offset: 0x10C) RCC APB3 peripheral sleep clock  register
	__IO uint32_t RCC_APB1LLPENR; 		//!< (rcc Offset: 0x110) RCC APB1 peripheral sleep clock  Low Word register
	__IO uint32_t RCC_APB1HLPENR; 		//!< (rcc Offset: 0x114) RCC APB1 peripheral sleep clock  High Word register
	__IO uint32_t RCC_APB2LPENR; 		//!< (rcc Offset: 0x118) RCC APB2 peripheral sleep clock  register
	__IO uint32_t RCC_APB4LPENR; 		//!< (rcc Offset: 0x11C) RCC APB4 peripheral sleep clock  register
	__IO uint32_t reserved13[4]; 		//!< (rcc Offset: 0x120) reserved 0x120-0x12C

} RCC_TypeDef;

/***************************************************************************//**
 * @relates RCC_TypeDef
 * @{
 ******************************************************************************/

/** @name RCC_CR: 		(rcc Offset: 0x000) RCC Source Control Register		   */
/** @{ */
#define RCC_CR_PLL3RDY 			0x20000000 //!< PLL3 clock ready flag
#define RCC_CR_PLL3ON 			0x10000000 //!< PLL3 enable
#define RCC_CR_PLL2RDY 			0x08000000 //!< PLL2 clock ready flag
#define RCC_CR_PLL2ON 			0x04000000 //!< PLL2 enable
#define RCC_CR_PLL1RDY 			0x02000000 //!< PLL1 clock ready flag
#define RCC_CR_PLL1ON 			0x01000000 //!< PLL1 enable
#define RCC_CR_HSECSSON 		0x00080000 //!< HSE Clock Security System enable
#define RCC_CR_HSEBYP 			0x00040000 //!< HSE clock bypass
#define RCC_CR_HSERDY 			0x00020000 //!< HSE clock ready flag
#define RCC_CR_HSEON 			0x00010000 //!< HSE clock enable
#define RCC_CR_D2CKRDY 			0x00008000 //!< D2 domain clocks ready flag
#define RCC_CR_D1CKRDY 			0x00004000 //!< D1 domain clocks ready flag
#define RCC_CR_HSI48RDY 		0x00002000 //!< HSI48 clock ready flag
#define RCC_CR_HSI48ON 			0x00001000 //!< HSI48 clock enable
#define RCC_CR_CSIKERON 		0x00000200 //!< CSI clock enable in Stop mode
#define RCC_CR_CSIRDY 			0x00000100 //!< CSI clock ready flag
#define RCC_CR_CSION 			0x00000080 //!< CSI clock enable
#define RCC_CR_HSIDIVF 			0x00000020 //!< HSI divider flag
#define RCC_CR_HSIDIV_1			0x00000000 //!< 00: Division by 1, hsi(_ker)_ck = 64 MHz (default after reset)
#define RCC_CR_HSIDIV_2		 	0x00000030 //!< 01: Division by 2, hsi(_ker)_ck = 32 MHz
#define RCC_CR_HSIDIV_4		 	0x00000040 //!< 10: Division by 4, hsi(_ker)_ck = 16 MHz
#define RCC_CR_HSIDIV_8		 	0x00000050 //!< 11: Division by 8, hsi(_ker)_ck = 8 MHz
#define RCC_CR_HSIRDY 			0x00000004 //!< HSI clock ready flag
#define RCC_CR_HSIKERON			0x00000002 //!< High Speed Internal clock enable in Stop mode
#define RCC_CR_HSION 			0x00000001 //!< High Speed Internal clock enable
/** @} */

#if STM32_H7_CPU_REVISION == STM32_H7_CPU_REVISION_V
/** @name RCC_HSICFGR:	(rcc Offset: 0x004) RCC HSI configuration register	   */
/** @{ */
#define RCC_HSICFGR_HSITRIM 	0xCF00000 //!< HSI clock trimming
#define RCC_HSICFGR_HSICAL	 	0x0000FFF //!< HSI clock calibration
/** @} */

#else
/** @name RCC_ICSCR:	(rcc Offset: 0x004) RCC Internal Clock Source Calibration Register */
/** @{ */
#define RCC_ICSCR_CSITRIM[5] 	0x7C000000 //!< CSI clock trimming
#define RCC_ICSCR_CSICAL[8]		0x0EFE0000 //!< CSI clock calibration
#define RCC_ICSCR_HSITRIM[6] 	0x000EF000 //!< HSI clock trimming
#define RCC_ICSCR_HSICAL[12]	0x00000FFF //!< HSI clock calibration
/** @} */
#endif


/** @name RCC_CRRCR:	(rcc Offset: 0x008) Clock Recovery RC Register		   */
/** @{ */
#define RCC_CRRCR_HSI48CAL[10] 	0x000003FF //!< Internal RC 48 MHz clock calibration
/** @} */

#if STM32_H7_CPU_REVISION == STM32_H7_CPU_REVISION_V
/** @name RCC_CSICFGR:	(rcc Offset: 0x00C) CSI Clock Calibration Register	   */
/** @{ */
#define RCC_CSICFGR_CSITRIM[6] 	0x3F000000 //!< CSI clock trimming
#define RCC_CSICFGR_CSICAL[10]	0x000003FF //!< CSI clock calibration
/** @} */

#endif
/** @name RCC_CFGR:		(rcc Offset: 0x010) RCC clock configuration register  */
/** @{ */
#define RCC_CFGR_MCO2			0xE0000000 //!< Micro-controller clock output 2
#define RCC_CFGR_MCO2PRE		0x1C000000 //!< MCO2 prescaler
#define RCC_CFGR_MCO1			0x01C00000 //!< Micro-controller clock output 1
#define RCC_CFGR_MCO11PRE		0x003C0000 //!< MCO1 prescaler
#define RCC_CFGR_TIMPRE			0x00008000 //!< Timers clocks prescaler selection
#define RCC_CFGR_HRTIMSEL		0x00004000 //!< High Resolution Timer clock prescaler selection
#define RCC_CFGR_RTCPRE[6]		0x00003F00 //!< HSE division factor for RTC clock
#define RCC_CFGR_STOPKERWUCK	0x00000080 //!< Kernel clock selection after a wake up from system Stop
#define RCC_CFGR_STOPWUCK		0x00000040 //!< System clock selection after a wake up from system Stop
#define RCC_CFGR_SWS			0x00000038 //!< System clock switch status
#define RCC_CFGR_SW				0x00000007 //!< System clock switch
/** @} */

/** @name RCC_D1CFGR:	(rcc Offset: 0x018) RCC Domain 1 configuration register */
/** @{ */
#define RCC_D1CFGR_D1CPRE[4]	0x00000F00 //!< D1 domain Core prescaler
#define RCC_D1CFGR_D1PPRE[3]	0x00000070 //!< D1 domain APB3 prescaler
#define RCC_D1CFGR_HPRE[4]		0x0000000F //!< D1 domain AHB prescaler
/** @} */

/** @name RCC_D2CFGR:	(rcc Offset: 0x01C) RCC Domain 2 configuration register */
/** @{ */
#define RCC_D2CFGR_D2PPRE2 		0x00000700 //!< D2 domain APB2 prescaler
#define RCC_D2CFGR_D2PPRE1      0x00000070 //!< D2 domain APB1 prescaler
/** @} */

/** @name RCC_D3CFGR:	(rcc Offset: 0x020) RCC Domain 3 configuration register */
/** @{ */
#define RCC_D3CFGR_D3PPRE       0x00000070 //!< D3 domain APB4 prescaler
/** @} */

/** @name RCC_PLLCKSELR:	(rcc Offset: 0x028) RCC PLLs clock source selection register  */
/** @{ */
#define RCC_PLLCKSELR_DIVM3[5]  0x03F00000 //!< Prescaler for PLL3
#define RCC_PLLCKSELR_DIVM2[6]	0x00002F00 //!< Prescaler for PLL2
#define RCC_PLLCKSELR_DIVM1[6]	0x000002F0 //!< Prescaler for PLL1
#define RCC_PLLCKSELR_PLLSRC[2]	0x00000002 //!< DIVMx and PLLs clock source selection
/** @} */ // @relates RCC_TypeDef

/** @name RCC_PLLCFGR:	(rcc Offset: 0x02C) RCC PLLs configuration register   */
/** @{ */
#define RCC_PLLCFGR_DIVR3EN 	0x01000000 //!< PLL3 DIVR divider output enable
#define RCC_PLLCFGR_DIVQ3EN 	0x00800000 //!< PLL3 DIVQ divider output enable
#define RCC_PLLCFGR_DIVP3EN		0x00400000 //!< PLL3 DIVP divider output enable
#define RCC_PLLCFGR_DIVR2EN		0x00200000 //!< PLL2 DIVR divider output enable
#define RCC_PLLCFGR_DIVQ2EN 	0x00100000 //!< PLL2 DIVQ divider output enable
#define RCC_PLLCFGR_DIVP2EN 	0x00080000 //!< PLL2 DIVP divider output enable
#define RCC_PLLCFGR_DIVR1EN 	0x00040000 //!< PLL1 DIVR divider output enable
#define RCC_PLLCFGR_DIVQ1EN		0x00020000 //!< PLL1 DIVQ divider output enable
#define RCC_PLLCFGR_DIVP1EN 	0x00010000 //!< PLL1 DIVP divider output enable
#define RCC_PLLCFGR_PLL3RGE[2] 	0x00000C00 //!< PLL3 input frequency range
#define RCC_PLLCFGR_PLL3VCOSEL 	0x00000200 //!< PLL3 VCO selection
#define RCC_PLLCFGR_PLL3FRACEN 	0x00000100 //!< PLL3 fractional latch enable
#define RCC_PLLCFGR_PLL2RGE[2] 	0x000000C0 //!< PLL2 input frequency range
#define RCC_PLLCFGR_PLL2VCOSEL 	0x00000020 //!< PLL2 VCO selection
#define RCC_PLLCFGR_PLL2FRACEN 	0x00000010 //!< PLL2 fractional latch enable
#define RCC_PLLCFGR_PLL1RGE[2] 	0x0000000C //!< PLL2 fractional latch enable
#define RCC_PLLCFGR_PLL1VCOSEL 	0x00000002 //!< PLL1 input frequency range
#define RCC_PLLCFGR_PLL1FRACEN 	0x00000001 //!< PLL1 fractional latch enable
/** @} */

/** @name RCC_PLL1DIVR:	(rcc Offset: 0x030) RCC PLL1 dividers configuration register    */
/** @{ */
#define RCC_PLL1DIVR_DIVR1[7] 	0x7F000000 //!< PLL1 DIVR division factor
#define RCC_PLL1DIVR_DIVQ1[7]	0x007F0000 //!< PLL1 DIVQ division factor
#define RCC_PLL1DIVR_DIVP1[7] 	0x0000FE00 //!< PLL1 DIVP division factor
#define RCC_PLL1DIVR_DIVN1[9] 	0x000001FF //!< Multiplication factor for PLL1 VCO
/** @} */

/** @name RCC_PLL1FRACR:	(rcc Offset: 0x034) RCC PLL1 fractional divider register    */
/** @{ */
#define RCC_PLL1FRACR_FRACN1	0x0000FFF8 //!< Fractional part of the multiplication factor for PLL1 VCO
/** @} */

/** @name RCC_PLL2DIVR:	(rcc Offset: 0x038) RCC PLL2 dividers configuration register    */
/** @{ */
#define RCC_PLL2DIVR_DIVR2[7]	0x07F00000 //!< PLL2 DIVR division factor
#define RCC_PLL2DIVR_DIVQ2[7]	0x007F0000 //!< PLL2 DIVQ division factor
#define RCC_PLL2DIVR_DIVP2[7]	0x0000FE00 //!< PLL2 DIVP division factor
#define RCC_PLL2DIVR_DIVN2[9]	0x000001FF //!< Multiplication factor for PLL2 VCO
/** @} */

/** @name RCC_PLL2FRACR:	(rcc Offset: 0x03C) RCC PLL2 fractional divider register   */
/** @{ */
#define RCC_PLL2FRACR_FRACN2[13]0x0000FFF8 //!< Fractional part of the multiplication factor for PLL2 VCO
/** @} */

/** @name RCC_PLL3DIVR:	(rcc Offset: 0x040) RCC PLL3 dividers configuration register    */
/** @{ */
#define RCC_PLL3DIVR_DIVR3[7]	0xCF000000 //!< PLL3 DIVR division factor
#define RCC_PLL3DIVR_DIVQ3[7]	0x00CF0000 //!< PLL3 DIVQ division factor
#define RCC_PLL3DIVR_DIVP3[7]	0x0000FC00 //!< PLL3 DIVP division factor
#define RCC_PLL3DIVR_DIVN3[8]	0x000001FF //!< Multiplication factor for PLL3 VCO
/** @} */

/** @name RCC_PLL3FRACR:	(rcc Offset: 0x044) RCC PLL3 fractional divider register   */
/** @{ */
#define RCC_PLL3FRACR_FRACN3[13]0x0000FFF8 //!< Fractional part of the multiplication factor for PLL3 VCO
/** @} */

/** @name RCC_D1CCIPR:	(rcc Offset: 0x04C) RCC Domain 1 kernel clock configuration register   */
/** @{ */
#define RCC_D1CCIPR_CKPERSEL[2]	0x30000000 //!< per_ck clock source selection
#define RCC_D1CCIPR_SDMMCSEL 	0x00010000 //!< SDMMC kernel clock source selection
#define RCC_D1CCIPR_QSPISEL[2]	0x00000060 //!< QUADSPI kernel clock source selection
#define RCC_D1CCIPR_FMCSEL[2]	0x00000003 //!< FMC kernel clock source selection
/** @} */

/** @name RCC_D2CCIP1R:	(rcc Offset: 0x050) RCC Domain 2 kernel clock configuration register   */
/** @{ */
#define RCC_D2CCIP1R_SWPSEL			0x80000000 //!< SWPMI kernel clock source selection
#define RCC_D2CCIP1R_FDCANSEL[2]	0x30000000 //!< FDCAN kernel clock source selection
#define RCC_D2CCIP1R_DFSDM1SEL		0x01000000 //!< DFSDM1 kernel Clk clock source selection
#define RCC_D2CCIP1R_SPDIFSEL[2]	0x00300000 //!< SPDIFRX kernel clock source selection
#define RCC_D2CCIP1R_SPI45SEL[3]	0x00070000 //!< SPI4 and 5 kernel clock source selection
#define RCC_D2CCIP1R_SPI123SEL[3]	0x00007000 //!< SPI/I2S1,2 and 3 kernel clock source selection
#define RCC_D2CCIP1R_SAI23SEL[3]	0x00000380 //!< SAI2 and SAI3 kernel clock source selection
#define RCC_D2CCIP1R_SAI1SEL[3]		0x00000007 //!< SAI1 and DFSDM1 kernel Aclk clock source selection
/** @} */

/** @name RCC_D2CCIP2R:	(rcc Offset: 0x054) RCC Domain 2 Kernel Clock Configuration Register    */
/** @{ */
#define RCC_D2CCIP2R_LPTIM1SEL[3]	0x70000000 //!< LPTIM1 kernel clock source selection
#define RCC_D2CCIP2R_CECSEL[2]		0x00C00000 //!< HDMI-CEC kernel clock source selection
#define RCC_D2CCIP2R_USBSEL[2]		0x00030000 //!< USBOTG 1 and 2 kernel clock source selection
#define RCC_D2CCIP2R_I2C123SEL[2]	0x00003000 //!< I2C1,2,3 kernel clock source selection
#define RCC_D2CCIP2R_RNGSEL[2]		0x00000300 //!< RNG kernel clock source selection
#define RCC_D2CCIP2R_USART16SEL[3]	0x00000038 //!< USART1 and 6 kernel clock source selection
#define RCC_D2CCIP2R_USART234578SEL[3] 0x00000007 //!< USART2/3, UART4,5, 7/8 (APB1) kernel clock source selection
/** @} */

/** @name RCC_D3CCIPR:	(rcc Offset: 0x058) RCC Domain 3 Kernel Clock Configuration Register    */
/** @{ */
#define RCC_D3CCIPR_SPI6SEL[3]		0x70000000 //!< SPI6 kernel clock source selection
#define RCC_D3CCIPR_SAI4BSEL[3]		0x07000000 //!< Sub-Block B of SAI4 kernel clock source selection
#define RCC_D3CCIPR_SAI4ASEL[3]		0x00E00000 //!< Sub-Block A of SAI4 kernel clock source selection
#define RCC_D3CCIPR_ADCSEL[2]		0x00030000 //!< SAR ADC kernel clock source selection
#define RCC_D3CCIPR_LPTIM345SEL[3]	0x0000E000 //!< LPTIM3,4,5 kernel clock source selection
#define RCC_D3CCIPR_LPTIM2SEL[3]	0x000001C0 //!< LPTIM2 kernel clock source selection
#define RCC_D3CCIPR_I2C4SEL[2]		0x00000030 //!< I2C4 kernel clock source selection
#define RCC_D3CCIPR_LPUART1SEL[3]	0x00000007 //!< LPUART1 kernel clock source selection
/** @} */

/** @name RCC_CIER:	(rcc Offset: 0x060) RCC Clock Source Interrupt Enable Register   */
/** @{ */
#define RCC_CIER_LSECSSIE		0x00000200 //!< LSE clock security system Interrupt Enable
#define RCC_CIER_PLL3RDYIE		0x00000100 //!< PLL3 ready Interrupt Enable
#define RCC_CIER_PLL2RDYIE		0x00000080 //!< PLL2 ready Interrupt Enable
#define RCC_CIER_PLL1RDYIE		0x00000040 //!< PLL1 ready Interrupt Enable
#define RCC_CIER_HSI48RDYIE		0x00000020 //!< HSI48 ready Interrupt Enable
#define RCC_CIER_CSIRDYIE		0x00000010 //!< CSI ready Interrupt Enable
#define RCC_CIER_HSERDYIE		0x00000008 //!< HSE ready Interrupt Enable
#define RCC_CIER_HSIRDYIE		0x00000004 //!< HSI ready Interrupt Enable
#define RCC_CIER_LSERDYIE		0x00000002 //!< LSE ready Interrupt Enable
#define RCC_CIER_LSIRDYIE		0x00000001 //!< LSI ready Interrupt Enable
/** @} */

/** @name RCC_CIFR:	(rcc Offset: 0x064) RCC Clock Source Interrupt Flag Register   */
/** @{ */
#define RCC_CIFR_HSECSSF		0x00000400 //!< HSE clock security system Interrupt Flag
#define RCC_CIFR_LSECSSF		0x00000200 //!< LSE clock security system Interrupt Flag
#define RCC_CIFR_PLL3RDYF		0x00000100 //!< PLL3 ready Interrupt Flag
#define RCC_CIFR_PLL2RDYF		0x00000080 //!< PLL2 ready Interrupt Flag
#define RCC_CIFR_PLL1RDYF		0x00000040 //!< PLL1 ready Interrupt Flag
#define RCC_CIFR_HSI48RDYF		0x00000020 //!< HSI48 ready Interrupt Flag
#define RCC_CIFR_CSIRDYF		0x00000010 //!< CSI ready Interrupt Flag
#define RCC_CIFR_HSERDYF		0x00000008 //!< HSE ready Interrupt Flag
#define RCC_CIFR_HSIRDYF		0x00000004 //!< HSI ready Interrupt Flag
#define RCC_CIFR_LSERDYF		0x00000002 //!< LSE ready Interrupt Flag
#define RCC_CIFR_LSIRDYF		0x00000001 //!< LSI ready Interrupt Flag
/** @} */

/** @name RCC_CICR:	(rcc Offset: 0x068) RCC Clock Source Interrupt Clear Register    */
/** @{ */
#define RCC_CICR_HSECSSC		0x00000400 //!< HSE clock security system Interrupt Clear
#define RCC_CICR_LSECSSC		0x00000200 //!< LSE clock security system Interrupt Clear
#define RCC_CICR_PLL3RDYC		0x00000100 //!< PLL3 ready Interrupt Clear
#define RCC_CICR_PLL2RDYC 		0x00000080 //!< PLL2 ready Interrupt Clear
#define RCC_CICR_PLL1RDYC 		0x00000040 //!< PLL1 ready Interrupt Clear
#define RCC_CICR_HSI48RDYC		0x00000020 //!< HSI48 ready Interrupt Clear
#define RCC_CICR_CSIRDYC 		0x00000010 //!< CSI ready Interrupt Clear
#define RCC_CICR_HSERDYC 		0x00000008 //!< HSE ready Interrupt Clear
#define RCC_CICR_HSIRDYC		0x00000004 //!< HSI ready Interrupt Clear
#define RCC_CICR_LSERDYC		0x00000002 //!< LSE ready Interrupt Clear
#define RCC_CICR_LSIRDYC 		0x00000001 //!< LSI ready Interrupt Clear
/** @} */

/** @name RCC_BDCR:	(rcc Offset: 0x070) RCC Backup Domain Control Register    */
/** @{ */
#define RCC_BDCR_BDRST			0x00010000 //!< Backup domain software reset
#define RCC_BDCR_RTCEN			0x00008000 //!< RTC clock enable
#define RCC_BDCR_RTCSEL[2] 		0x00000300 //!< RTC clock source selection
#define RCC_BDCR_LSECSSD 		0x00000040 //!< LSE clock security system failure detection
#define RCC_BDCR_LSECSSON		0x00000020 //!< LSE clock security system enable
#define RCC_BDCR_LSEDRV[2]	 	0x00000018 //!<	LSE oscillator driving capability
#define RCC_BDCR_LSEBYP 		0x00000004 //!< LSE oscillator bypass
#define RCC_BDCR_LSERDY 		0x00000002 //!< LSE oscillator ready
#define RCC_BDCR_LSERDY 		0x00000001 //!< LSE oscillator ready
/** @} */

/** @name RCC_CSR:	(rcc Offset: 0x074) RCC Clock Control and Status Register   */
/** @{ */
#define RCC_CSR_LSIRDY 			0x00000000 //!< LSI oscillator ready
#define RCC_CSR_LSION 			0x00000000 //!< LSI oscillator enable
/** @} */

/** @name RCC_AHB3RSTR:	(rcc Offset: 0x07C) RCC AHB3 Reset Register   */
/** @{ */
#define RCC_AHB3RSTR_SDMMC1RST 	0x00010000 //!< SDMMC1 and SDMMC1 delay block reset
#define RCC_AHB3RSTR_QSPIRST  	0x00004000 //!< QUADSPI and QUADSPI delay block reset
#define RCC_AHB3RSTR_FMCRST 	0x00001000 //!< FMC block reset
#define RCC_AHB3RSTR_JPGDECRST 	0x00000020 //!< JPGDEC block reset
#define RCC_AHB3RSTR_DMA2DRST 	0x00000010 //!< DMA2D block reset
#define RCC_AHB3RSTR_MDMARST 	0x00000001 //!< DMA2D block reset
/** @} */

/** @name RCC_AHB1RSTR:	(rcc Offset: 0x080) RCC AHB1 Peripheral Reset Register   */
/** @{ */
#define RCC_AHB1RSTR_USB2OTGRST	0x00000000 //!< USB2OTG (OTG_HS2) block reset
#define RCC_AHB1RSTR_USB1OTGRST	0x08000000 //!< USB1OTG (OTG_HS1) block reset
#define RCC_AHB1RSTR_ETH1MACRST 0x02008000 //!< ETH1MAC block reset
#define RCC_AHB1RSTR_ADC12RST 	0x00000020 //!< ADC1 and 2 block reset
#define RCC_AHB1RSTR_DMA2RST 	0x00000002 //!< DMA2 block reset
#define RCC_AHB1RSTR_DMA1RST 	0x00000001 //!< DMA1 block reset
/** @} */

/** @name RCC_AHB2RSTR:	(rcc Offset: 0x084) RCC AHB2 Peripheral Reset Register    */
/** @{ */
#define RCC_AHB2RSTR_SDMMC2RST 	0x00000200 //!< SDMMC2 and SDMMC2 Delay block reset
#define RCC_AHB2RSTR_RNGRST 	0x00000040 //!< Random Number Generator block reset
#define RCC_AHB2RSTR_HASHRST 	0x00000020 //!< Hash block reset
#define RCC_AHB2RSTR_CRYPTRST 	0x00000010 //!< Cryptography block reset
#define RCC_AHB2RSTR_CAMITFRST 	0x00000001 //!< CAMITF block reset
/** @} */

/** @name RCC_AHB4RSTR:	(rcc Offset: 0x088) RCC AHB4 Peripheral Reset Register   */
/** @{ */
#define RCC_AHB4RSTR_HSEMRST 	0x02000000 //!< HSEM block reset
#define RCC_AHB4RSTR_ADC3RST 	0x01000000 //!< ADC3 block reset
#define RCC_AHB4RSTR_BDMARST 	0x00200000 //!< BDMA block reset
#define RCC_AHB4RSTR_CRCRST 	0x00080000 //!< CRC block reset
#define RCC_AHB4RSTR_GPIOKRST 	0x00000400 //!< GPIOK block reset
#define RCC_AHB4RSTR_GPIOJRST 	0x00000200 //!< GPIOJ block reset
#define RCC_AHB4RSTR_GPIOIRST 	0x00000100 //!< GPIOI block reset
#define RCC_AHB4RSTR_GPIOHRST 	0x00000080 //!< GPIOH block reset
#define RCC_AHB4RSTR_GPIOGRST 	0x00000040 //!< GPIOG block reset
#define RCC_AHB4RSTR_GPIOFRST 	0x00000020 //!< GPIOF block reset
#define RCC_AHB4RSTR_GPIOERST 	0x00000010 //!< GPIOE block reset
#define RCC_AHB4RSTR_GPIODRST 	0x00000008 //!< GPIOD block reset
#define RCC_AHB4RSTR_GPIOCRST 	0x00000004 //!< GPIOC block reset
#define RCC_AHB4RSTR_GPIOBRST 	0x00000002 //!< GPIOB block reset
#define RCC_AHB4RSTR_GPIOARST 	0x00000001 //!< GPIOA block reset
/** @} */

/** @name RCC_APB3RSTR:	(rcc Offset: 0x08C) RCC APB3 Peripheral Reset Register    */
/** @{ */
#define RCC_APB3RSTR_LTDCRST 	0x00000008 //!< LTDC block reset
/** @} */

/** @name RCC_APB1LRSTR:	(rcc Offset: 0x000) RCC APB1 Peripheral Reset Register   */
/** @{ */
#define RCC_APB1LRSTR_UART8RST 	0x80000000 //!< UART8 block reset
#define RCC_APB1LRSTR_UART7RST 	0x40000000 //!< UART7 block reset
#define RCC_APB1LRSTR_DAC12RST 	0x20000000 //!< DAC1 and 2 Blocks Reset
#define RCC_APB1LRSTR_CECRST 	0x04000000 //!< HDMI-CEC block reset
#define RCC_APB1LRSTR_I2C3RST 	0x00800000 //!< I2C3 block reset
#define RCC_APB1LRSTR_I2C2RST 	0x00400000 //!< I2C2 block reset
#define RCC_APB1LRSTR_I2C1RST 	0x00200000 //!< I2C1 block reset
#define RCC_APB1LRSTR_UART5RST 	0x00100000 //!< UART5 block reset
#define RCC_APB1LRSTR_UART4RST 	0x00080000 //!< UART4 block reset
#define RCC_APB1LRSTR_USART3RST 0x00040000 //!< USART3 block reset
#define RCC_APB1LRSTR_USART2RST 0x00020000 //!< USART2 block reset
#define RCC_APB1LRSTR_SPDIFRXRST 0x00010000 //!<SPDIFRX block reset
#define RCC_APB1LRSTR_SPI3RST 	0x00008000 //!< SPI3 block reset
#define RCC_APB1LRSTR_SPI2RST 	0x00004000 //!< SPI2 block reset
#define RCC_APB1LRSTR_LPTIM1RST	0x00000200 //!< LPTIM1 block reset
#define RCC_APB1LRSTR_TIM14RST 	0x00000100 //!< TIM14 block reset
#define RCC_APB1LRSTR_TIM13RST 	0x00000080 //!< TIM13 block reset
#define RCC_APB1LRSTR_TIM12RST 	0x00000040 //!< TIM12 block reset
#define RCC_APB1LRSTR_TIM7RST 	0x00000020 //!< TIM7 block reset
#define RCC_APB1LRSTR_TIM6RST 	0x00000010 //!< TIM6 block reset
#define RCC_APB1LRSTR_TIM5RST 	0x00000008 //!< TIM5 block reset
#define RCC_APB1LRSTR_TIM4RST 	0x00000004 //!< TIM4 block reset
#define RCC_APB1LRSTR_TIM3RST 	0x00000002 //!< TIM3 block reset
#define RCC_APB1LRSTR_TIM2RST 	0x00000001 //!< TIM2 block reset
/** @} */

/** @name RCC_APB1HRSTR:	(rcc Offset: 0x094) RCC APB1 Peripheral Reset Register    */
/** @{ */
#define RCC_APB1HRSTR_FDCANRST	0x00000100 //!< FDCAN block reset
#define RCC_APB1HRSTR_MDIOSRST 	0x00000020 //!< MDIOS block reset
#define RCC_APB1HRSTR_OPAMPRST 	0x00000010 //!< OPAMP block reset
#define RCC_APB1HRSTR_SWPRST 	0x00000004 //!< SWPMI block reset
#define RCC_APB1HRSTR_CRSRST 	0x00000002 //!< Clock Recovery System reset
/** @} */

/** @name RCC_APB2RSTR:	(rcc Offset: 0x098) RCC APB2 Peripheral Reset Register		*/
/** @{ */
#define RCC_APB2RSTR_HRTIMRST 	0x20000000 //!< HRTIM block reset
#define RCC_APB2RSTR_DFSDM1RST 	0x10000000 //!< DFSDM1 block reset
#define RCC_APB2RSTR_SAI3RST 	0x01000000 //!< SAI3 block reset
#define RCC_APB2RSTR_SAI2RST  	0x00800000 //!< SAI2 block reset
#define RCC_APB2RSTR_SAI1RST 	0x00400000 //!< SAI1 block reset
#define RCC_APB2RSTR_SPI5RST 	0x00100000 //!< SPI5 block reset
#define RCC_APB2RSTR_TIM17RST 	0x00040000 //!< TIM17 block reset
#define RCC_APB2RSTR_TIM16RST 	0x00020000 //!< TIM16 block reset
#define RCC_APB2RSTR_TIM15RST 	0x00010000 //!< TIM15 block reset
#define RCC_APB2RSTR_SPI4RST 	0x00002000 //!< SPI4 block reset
#define RCC_APB2RSTR_SPI1RST 	0x00001000 //!< SPI1 block reset
#define RCC_APB2RSTR_USART6RST 	0x00000020 //!< USART6 block reset
#define RCC_APB2RSTR_USART1RST 	0x00000010 //!< USART1 block reset
#define RCC_APB2RSTR_TIM8RST 	0x00000002 //!< TIM8 block reset
#define RCC_APB2RSTR_TIM1RST 	0x00000001 //!< TIM1 block reset
/** @} */

/** @name RCC_APB4RSTR:	(rcc Offset: 0x09C) RCC APB4 Peripheral Reset Register   */
/** @{ */
#define RCC_APB4RSTR_SAI4RST 	0x00040000 //!< SAI4 block reset
#define RCC_APB4RSTR_VREFRST 	0x00008000 //!< VREF block reset
#define RCC_APB4RSTR_COMP12RST 	0x00004000 //!< COMP12 Blocks Reset
#define RCC_APB4RSTR_LPTIM5RST 	0x00001000 //!< LPTIM5 block reset
#define RCC_APB4RSTR_LPTIM4RST 	0x00000800 //!< LPTIM4 block reset
#define RCC_APB4RSTR_LPTIM3RST 	0x00000400 //!< LPTIM3 block reset
#define RCC_APB4RSTR_LPTIM2RST 	0x00000200 //!< LPTIM2 block reset
#define RCC_APB4RSTR_I2C4RST 	0x00000080 //!< I2C4 block reset
#define RCC_APB4RSTR_SPI6RST 	0x00000020 //!< SPI6 block reset
#define RCC_APB4RSTR_LPUART1RST 0x00000008 //!< LPUART1 block reset
#define RCC_APB4RSTR_SYSCFGRST 	0x00000002 //!< SYSCFG block reset
/** @} */

/** @name RCC_GCR:	(rcc Offset: 0x0A0) RCC Global Control Register   */
/** @{ */
#define RCC_GCR_WW1RSC			0x00000000 //!< WWDG1 reset scope control
/** @} */

/** @name RCC_D3AMR:	(rcc Offset: 0x0A8) RCC D3 Autonomous mode Register */
/** @{ */
#define RCC_D3AMR_SRAM4AMEN 	0x20000000 //!< SRAM4 Autonomous mode enable
#define RCC_D3AMR_BKPRAMAMEN 	0x10000000 //!< Backup RAM Autonomous mode enable
#define RCC_D3AMR_ADC3AMEN 		0x01000000 //!< ADC3 Autonomous mode enable
#define RCC_D3AMR_SAI4AMEN 		0x00200000 //!< SAI4 Autonomous mode enable
#define RCC_D3AMR_CRCAMEN 		0x00080000 //!< CRC Autonomous mode enable
#define RCC_D3AMR_RTCAMEN 		0x00010000 //!< RTC Autonomous mode enable
#define RCC_D3AMR_VREFAMEN 		0x00008000 //!< VREF Autonomous mode enable
#define RCC_D3AMR_COMP12AMEN 	0x00004000 //!< COMP12 Autonomous mode enable
#define RCC_D3AMR_LPTIM5AMEN 	0x00001000 //!< LPTIM5 Autonomous mode enable
#define RCC_D3AMR_LPTIM4AMEN 	0x00000800 //!< LPTIM4 Autonomous mode enable
#define RCC_D3AMR_LPTIM3AMEN 	0x00000400 //!< LPTIM3 Autonomous mode enable
#define RCC_D3AMR_LPTIM2AMEN 	0x00000200 //!< LPTIM2 Autonomous mode enable
#define RCC_D3AMR_I2C4AMEN 		0x00000080 //!< I2C4 Autonomous mode enable
#define RCC_D3AMR_SPI6AMEN 		0x00000020 //!< SPI6 Autonomous mode enable
#define RCC_D3AMR_LPUART1AMEN 	0x00000008 //!< LPUART1 Autonomous mode enable
#define RCC_D3AMR_BDMAAMEN 		0x00000001 //!< BDMA and DMAMUX Autonomous mode enable
/** @} */






	//TODO USE THIS TEMPLATE FOR FILLING REGISTERS FASTER
/** @name RCC_:	(rcc Offset: 0x000) .........   */
/** @{ */
#define RCC__		0x00000000 //!<
/** @} */
	//TODO


/**
 * @brief In the following line adjust the value of External High Speed oscillator (HSE)
   used in your application

   Tip: To avoid modifying this file each time you need to use different HSE, you
        can define the HSE value in your toolchain compiler preprocessor.
  */
#ifndef HSE_VALUE
#define HSE_VALUE            25000000 /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) Startup
   Timeout value
   */
#ifndef HSE_STARTUP_TIMEOUT
#define HSE_STARTUP_TIMEOUT  0x0500   /*!< Time out for HSE start up */
#endif /* HSE_STARTUP_TIMEOUT */

#ifndef HSI_VALUE
#define HSI_VALUE            16000000 /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */


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
void RCCPeripheralLPEnable(unsigned int periph_id);
void RCCPeripheralLPDisable(unsigned int periph_id);

void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

#endif /* RCC_H7_H_ */

/** @} */
