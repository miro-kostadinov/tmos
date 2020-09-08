/**************************************************************************//**
 * @ingroup	hardware_stm32h7_pwr
 * @file	hardware/stm32/stm32h7/pwr_h7.h
 * @brief  	STM32H7 PWR
 * @date    7. March 2020
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_pwr   Power control (PWR)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7  Power control (PWR)
 * @{
 *
 ******************************************************************************/

#ifndef PWR_H7_H_
#define PWR_H7_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Power control (PWR)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t PWR_CR1;     		//!< (pwr Offset: 0x00) PWR control register 1
  __IO uint32_t PWR_CSR1;      		//!< (pwr Offset: 0x04) PWR control status register 1
  __IO uint32_t PWR_CR2;       		//!< (pwr Offset: 0x08) PWR control register 2
  __IO uint32_t PWR_CR3;       		//!< (pwr Offset: 0x0C) PWR control register 3
  __IO uint32_t PWR_CPUCR;     		//!< (pwr Offset: 0x10) PWR CPU control register
  __IO uint32_t reserved1; 			//!< (pwr Offset: 0x14) reserved
  __IO uint32_t PWR_D3CR;      		//!< (pwr Offset: 0x18) PWR D3 domain control register
  __IO uint32_t reserved2; 			//!< (pwr Offset: 0x1C) reserved
  __IO uint32_t PWR_WKUPCR;    		//!< (pwr Offset: 0x20) PWR wakeup clear register
  __IO uint32_t PWR_WKUPFR;    		//!< (pwr Offset: 0x24) PWR wakeup flag register
  __IO uint32_t PWR_WKUPEPR;   		//!< (pwr Offset: 0x28) PWR wakeup enable and polarity register
} PWR_TypeDef;


/***************************************************************************//**
 * @relates PWR_TypeDef
 * @{
 ******************************************************************************/

/** @name PWR_CR1:     		(pwr Offset: 0x00) PWR control register 1		   */
/** @{ */
#define PWR_CR1_ALS				0x00060000 //!< Analog voltage detector level selection
#define PWR_CR1_ALS_1v7			0x00000000 //!<  1.7 V
#define PWR_CR1_ALS_2v1			0x00020000 //!<  2.1 V
#define PWR_CR1_ALS_2v5			0x00040000 //!<  2.4 V
#define PWR_CR1_ALS_2v8			0x00060000 //!<  2.8 V
#define PWR_CR1_AVDEN			0x00010000 //!< Peripheral voltage monitor on V DDA enable
#define PWR_CR1_SVOS			0x0000C000 //!< System Stop mode voltage scaling selection
#define PWR_CR1_SVOS_5			0x00004000 //!<  SVOS5 Scale 5
#define PWR_CR1_SVOS_4			0x00008000 //!<  SVOS4 Scale 4
#define PWR_CR1_SVOS_3			0x0000C000 //!<  SVOS3 Scale 3 (default)
#define PWR_CR1_FLPS			0x00000200 //!< Flash low-power mode in DStop mode
#define PWR_CR1_DBP				0x00000100 //!< Disable backup domain write protection
#define PWR_CR1_PLS				0x000000E0 //!< Programmable voltage detector level selection
#define PWR_CR1_PLS_1v95		0x00000000 //!<  1.95 V
#define PWR_CR1_PLS_2v1			0x00000020 //!<  2.1  V
#define PWR_CR1_PLS_2v25		0x00000040 //!<  2.25 V
#define PWR_CR1_PLS_2v4			0x00000060 //!<  2.4  V
#define PWR_CR1_PLS_2v55		0x00000080 //!<  2.55 V
#define PWR_CR1_PLS_2v7			0x000000A0 //!<  2.7  V
#define PWR_CR1_PLS_2v85		0x000000C0 //!<  2.85 V
#define PWR_CR1_PLS_vref		0x000000E0 //!<  External voltage level on PVD_IN pin, compared to internal V REFINT level.
#define PWR_CR1_PVDE			0x00000010 //!< Programmable voltage detector enable
#define PWR_CR1_LPDS			0x00000001 //!< Low-power Deepsleep with SVOS3
/** @} */

/** @name PWR_CSR1:      	(pwr Offset: 0x04) PWR control status register 1   */
/** @{ */
#define PWR_CSR1_AVDO			0x00010000 //!< Analog voltage detector output on V DDA
#define PWR_CSR1_ACTVOS			0x0000C000 //!< VOS currently applied for V CORE voltage scaling selection
#define PWR_CSR1_ACTVOSRDY		0x00002000 //!< Voltage levels ready bit for currently used VOS
#define PWR_CSR1_PVDO			0x00000010 //!< Programmable voltage detect output
/** @} */

/** @name PWR_CR2:       	(pwr Offset: 0x08) PWR control register 2	 	   */
/** @{ */
#define PWR_CR2_TEMPH			0x00800000 //!< Temperature level monitoring versus high threshold
#define PWR_CR2_TEMPL			0x00400000 //!< Temperature level monitoring versus low threshold
#define PWR_CR2_VBATH			0x00200000 //!< V BAT level monitoring versus high threshold
#define PWR_CR2_VBATL			0x00100000 //!< V BAT level monitoring versus low threshold
#define PWR_CR2_BRRDY			0x00010000 //!< Backup regulator ready
#define PWR_CR2_MONEN			0x00000010 //!< V BAT and temperature monitoring enable
#define PWR_CR2_BREN			0x00000001 //!< Backup regulator enable
/** @} */

/** @name PWR_CR3:       	(pwr Offset: 0x0C) PWR control register 3	   	   */
/** @{ */
#define PWR_CR3_USB33RDY		0x04000000 //!< USB supply ready
#define PWR_CR3_USBREGEN		0x02000000 //!< USB regulator enable
#define PWR_CR3_3DEN			0x01000000 //!< VDD33USB voltage level detector enable
#define PWR_CR3_VBRS			0x00000200 //!< VBAT charging resistor selection
#define PWR_CR3_VBE				0x00000100 //!< VBAT charging enable
#define PWR_CR3_SCUEN			0x00000004 //!< Supply configuration update enable
#define PWR_CR3_LDOEN			0x00000002 //!< Low drop-out regulator enable
#define PWR_CR3_BYPASS			0x00000001 //!< Power management unit bypass
/** @} */

/** @name PWR_CPUCR:     	(pwr Offset: 0x10) PWR CPU control register	 	   */
/** @{ */
#define PWR_CPUCR_				0x00000000 //!<
#define PWR_CPUCR_				0x00000000 //!<
#define PWR_CPUCR_				0x00000000 //!<
#define PWR_CPUCR_				0x00000000 //!<
#define PWR_CPUCR_				0x00000000 //!<
#define PWR_CPUCR_				0x00000000 //!<
#define PWR_CPUCR_				0x00000000 //!<
#define PWR_CPUCR_				0x00000000 //!<
#define PWR_CPUCR_				0x00000000 //!<
/** @} */

/** @name PWR_D3CR:      	(pwr Offset: 0x18) PWR D3 domain control register  */
/** @{ */
#define PWR_D3CR_				0x00000000 //!<
/** @} */

/** @name PWR_WKUPCR:    	(pwr Offset: 0x20) PWR wakeup clear register	   */
/** @{ */
#define PWR_WKUPCR_				0x00000000 //!<
/** @} */

/** @name PWR_WKUPFR:    	(pwr Offset: 0x24) PWR wakeup flag register	 	   */
/** @{ */
#define PWR_WKUPFR_				0x00000000 //!<
/** @} */

/** @name PWR_WKUPEPR:   	(pwr Offset: 0x28) PWR wakeup enable and polarity register */
/** @{ */
#define PWR_WKUPEPR_			0x00000000 //!<
/** @} */


/** @} */ // @relates PWR_TypeDef



#endif /* PWR_H7_H_ */

/** @} */
