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
	__IO uint32_t reserved2; /*!< reserved,                                                                Address offset: 0x24  */
	__IO uint32_t RCC_PLLCKSELR; /*!< RCC PLLs Clock Source Selection Register,                                Address offset: 0x28  */
	__IO uint32_t RCC_PLLCFGR; /*!< RCC PLLs  Configuration Register,                                        Address offset: 0x2C  */
	__IO uint32_t RCC_PLL1DIVR; /*!< RCC PLL1 Dividers Configuration Register,                                Address offset: 0x30  */
	__IO uint32_t RCC_PLL1FRACR; /*!< RCC PLL1 Fractional Divider Configuration Register,                      Address offset: 0x34  */
	__IO uint32_t RCC_PLL2DIVR; /*!< RCC PLL2 Dividers Configuration Register,                                Address offset: 0x38  */
	__IO uint32_t RCC_PLL2FRACR; /*!< RCC PLL2 Fractional Divider Configuration Register,                      Address offset: 0x3C  */
	__IO uint32_t RCC_PLL3DIVR; /*!< RCC PLL3 Dividers Configuration Register,                                Address offset: 0x40  */
	__IO uint32_t RCC_PLL3FRACR; /*!< RCC PLL3 Fractional Divider Configuration Register,                      Address offset: 0x44  */
	__IO uint32_t reserved3; /*!< Reserved,                                                                Address offset: 0x48  */
	__IO uint32_t RCC_D1CCIPR; /*!< RCC Domain 1 Kernel Clock Configuration Register                         Address offset: 0x4C  */
	__IO uint32_t RCC_D2CCIP1R; /*!< RCC Domain 2 Kernel Clock Configuration Register                         Address offset: 0x50  */
	__IO uint32_t RCC_D2CCIP2R; /*!< RCC Domain 2 Kernel Clock Configuration Register                         Address offset: 0x54  */
	__IO uint32_t RCC_D3CCIPR; /*!< RCC Domain 3 Kernel Clock Configuration Register                         Address offset: 0x58  */
	__IO uint32_t reserved4; /*!< Reserved,                                                                Address offset: 0x5C  */
	__IO uint32_t RCC_CIER; /*!< RCC Clock Source Interrupt Enable Register                               Address offset: 0x60  */
	__IO uint32_t RCC_CIFR; /*!< RCC Clock Source Interrupt Flag Register                                 Address offset: 0x64  */
	__IO uint32_t RCC_CICR; /*!< RCC Clock Source Interrupt Clear Register                                Address offset: 0x68  */
	__IO uint32_t reserved5; /*!< Reserved,                                                                Address offset: 0x6C  */
	__IO uint32_t RCC_BDCR; /*!< RCC Vswitch Backup Domain Control Register,                              Address offset: 0x70  */
	__IO uint32_t RCC_CSR; /*!< RCC clock control & status register,                                     Address offset: 0x74  */
	__IO uint32_t reserved6; /*!< Reserved,                                                                Address offset: 0x78  */
	__IO uint32_t RCC_AHB3RSTR; /*!< RCC AHB3 peripheral reset register,                                      Address offset: 0x7C  */
	__IO uint32_t RCC_AHB1RSTR; /*!< RCC AHB1 peripheral reset register,                                      Address offset: 0x80  */
	__IO uint32_t RCC_AHB2RSTR; /*!< RCC AHB2 peripheral reset register,                                      Address offset: 0x84  */
	__IO uint32_t RCC_AHB4RSTR; /*!< RCC AHB4 peripheral reset register,                                      Address offset: 0x88  */
	__IO uint32_t RCC_APB3RSTR; /*!< RCC APB3 peripheral reset register,                                      Address offset: 0x8C  */
	__IO uint32_t RCC_APB1LRSTR; /*!< RCC APB1 peripheral reset Low Word register,                             Address offset: 0x90  */
	__IO uint32_t RCC_APB1HRSTR; /*!< RCC APB1 peripheral reset High Word register,                            Address offset: 0x94  */
	__IO uint32_t RCC_APB2RSTR; /*!< RCC APB2 peripheral reset register,                                      Address offset: 0x98  */
	__IO uint32_t RCC_APB4RSTR; /*!< RCC APB4 peripheral reset register,                                      Address offset: 0x9C  */
	__IO uint32_t RCC_GCR; /*!< RCC RCC Global Control  Register,                                        Address offset: 0xA0  */
	__IO uint32_t reserved8; /*!< Reserved,                                                                Address offset: 0xA4  */
	__IO uint32_t RCC_D3AMR; /*!< RCC Domain 3 Autonomous Mode Register,                                   Address offset: 0xA8  */
	__IO uint32_t reserved11[9]; /*!< Reserved, 0xAC-0xCC                                                      Address offset: 0xAC  */
	__IO uint32_t RCC_RSR; /*!< RCC Reset status register,                                               Address offset: 0xD0  */
	__IO uint32_t RCC_AHB3ENR; /*!< RCC AHB3 peripheral clock  register,                                     Address offset: 0xD4  */
	__IO uint32_t RCC_AHB1ENR; /*!< RCC AHB1 peripheral clock  register,                                     Address offset: 0xD8  */
	__IO uint32_t RCC_AHB2ENR; /*!< RCC AHB2 peripheral clock  register,                                     Address offset: 0xDC  */
	__IO uint32_t RCC_AHB4ENR; /*!< RCC AHB4 peripheral clock  register,                                     Address offset: 0xE0  */
	__IO uint32_t RCC_APB3ENR; /*!< RCC APB3 peripheral clock  register,                                     Address offset: 0xE4  */
	__IO uint32_t RCC_APB1LENR; /*!< RCC APB1 peripheral clock  Low Word register,                            Address offset: 0xE8  */
	__IO uint32_t RCC_APB1HENR; /*!< RCC APB1 peripheral clock  High Word register,                           Address offset: 0xEC  */
	__IO uint32_t RCC_APB2ENR; /*!< RCC APB2 peripheral clock  register,                                     Address offset: 0xF0  */
	__IO uint32_t RCC_APB4ENR; /*!< RCC APB4 peripheral clock  register,                                     Address offset: 0xF4  */
	__IO uint32_t reserved12; /*!< Reserved,                                                                Address offset: 0xF8  */
	__IO uint32_t RCC_AHB3LPENR; /*!< RCC AHB3 peripheral sleep clock  register,                               Address offset: 0xFC  */
	__IO uint32_t RCC_AHB1LPENR; /*!< RCC AHB1 peripheral sleep clock  register,                               Address offset: 0x100 */
	__IO uint32_t RCC_AHB2LPENR; /*!< RCC AHB2 peripheral sleep clock  register,                               Address offset: 0x104 */
	__IO uint32_t RCC_AHB4LPENR; /*!< RCC AHB4 peripheral sleep clock  register,                               Address offset: 0x108 */
	__IO uint32_t RCC_APB3LPENR; /*!< RCC APB3 peripheral sleep clock  register,                               Address offset: 0x10C */
	__IO uint32_t RCC_APB1LLPENR; /*!< RCC APB1 peripheral sleep clock  Low Word register,                      Address offset: 0x110 */
	__IO uint32_t RCC_APB1HLPENR; /*!< RCC APB1 peripheral sleep clock  High Word register,                     Address offset: 0x114 */
	__IO uint32_t RCC_APB2LPENR; /*!< RCC APB2 peripheral sleep clock  register,                               Address offset: 0x118 */
	__IO uint32_t RCC_APB4LPENR; /*!< RCC APB4 peripheral sleep clock  register,                               Address offset: 0x11C */
	__IO uint32_t reserved13[4]; /*!< Reserved, 0x120-0x12C                                                    Address offset: 0x120 */

} RCC_TypeDef;

/***************************************************************************//**
 * @relates RCC_TypeDef
 * @{
 ******************************************************************************/

/** @name RCC_CR: 		(rcc Offset: 0x000) RCC Source Control Register		   */
/** @{ */
/** @} */

#if STM32_H7_CPU_REVISION == STM32_H7_CPU_REVISION_V
/** @name RCC_HSICFGR:	(rcc Offset: 0x004) RCC HSI configuration register	   */
/** @{ */
/** @} */

#else
/** @name RCC_ICSCR:	(rcc Offset: 0x004) RCC Internal Clock Source Calibration Register */
/** @{ */
/** @} */

#endif
/** @name RCC_CRRCR:	(rcc Offset: 0x008) Clock Recovery RC Register		   */
/** @{ */
/** @} */

#if STM32_H7_CPU_REVISION == STM32_H7_CPU_REVISION_V
/** @name RCC_CSICFGR:	(rcc Offset: 0x00C) CSI Clock Calibration Register	   */
/** @{ */
/** @} */

#endif
/** @name RCC_CFGR:		(rcc Offset: 0x010) RCC clock configuration register  */
/** @{ */
/** @} */

/** @name RCC_D1CFGR:	(rcc Offset: 0x018) RCC Domain 1 configuration register */
/** @{ */
/** @} */

/** @name RCC_D2CFGR:	(rcc Offset: 0x01C) RCC Domain 2 configuration register */
/** @{ */
#define RCC_D2CFGR_D2PPRE2 			0x00000700 //!< D2 domain APB2 prescaler
#define RCC_D2CFGR_D2PPRE2_hclk1	0x00000000 //!<  rcc_pclk2 = rcc_hclk1 (default after reset)
#define RCC_D2CFGR_D2PPRE2_hclk1_2	0x00000400 //!<  rcc_pclk2 = rcc_hclk1 / 2
#define RCC_D2CFGR_D2PPRE2_hclk1_4	0x00000500 //!<  rcc_pclk2 = rcc_hclk1 / 4
#define RCC_D2CFGR_D2PPRE2_hclk1_8	0x00000600 //!<  rcc_pclk2 = rcc_hclk1 / 8
#define RCC_D2CFGR_D2PPRE2_hclk1_16	0x00000700 //!<  rcc_pclk2 = rcc_hclk1 /16
#define RCC_D2CFGR_        			0x00000000 //!<
/** @} */

/** @name RCC_D3CFGR:	(rcc Offset: 0x020) RCC Domain 3 configuration register */
/** @{ */
#define RCC_D3CFGR_        			0x00000000 //!<
/** @} */










/** @} */ // @relates RCC_TypeDef

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
