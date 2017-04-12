/**************************************************************************//**
 * @ingroup	hardware_stm32f2_pwr
 * @file	hardware/stm32/stm32f2/pwr_f2.h
 * @brief  	STM32F2 PWR
 * @date    15. February 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f2_pwr   Power control (PWR)
 * @ingroup	 hardware_stm32f2
 * Source and definitions for STM32F2  Power control (PWR)
 * @{
 *
 ******************************************************************************/

#ifndef PWR_F2_H_
#define PWR_F2_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Power control (PWR)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t PWR_CR;   		   	//!< (pwr Offset: 0x00) PWR power control register
  __IO uint32_t PWR_CSR;      		//!< (pwr Offset: 0x04) PWR power control/status register
} PWR_TypeDef;

/** bit-banding version */
typedef struct
{
  __IO uint32_t PWR_CR_LPDS_BB;  	//!< Bit 0 LPDS: Low-power deep sleep
  __IO uint32_t PWR_CR_PDDS_BB;  	//!< Bit 1 PDDS: Power down deepsleep
  __IO uint32_t PWR_CR_CWUF_BB;  	//!< Bit 2 CWUF: Clear wakeup flag
  __IO uint32_t PWR_CR_CSBF_BB;  	//!< Bit 3 CSBF: Clear standby flag
  __IO uint32_t PWR_CR_PVDE_BB;  	//!< Bit 4 PVDE: Power voltage detector enable
  __IO uint32_t PWR_CR_PLS_0_BB;  	//!< Bits 7:5 PLS[2:0]: PVD level selection
  __IO uint32_t PWR_CR_PLS_1_BB;  	//!< Bits 7:5 PLS[2:0]: PVD level selection
  __IO uint32_t PWR_CR_PLS_2_BB;  	//!< Bits 7:5 PLS[2:0]: PVD level selection
  __IO uint32_t PWR_CR_DBP_BB;  	//!< Bit 8 DBP: Disable backup domain write protection
  __IO uint32_t PWR_CR_FPDS_BB;  	//!< Bit 9 FPDS: Flash power down in Stop mode
  __IO uint32_t reserved1[22];  	//!< Bits 31:10 reserved

  __IO uint32_t PWR_CSR_WUF_BB;  	//!< Bit 0 WUF: Wakeup flag
  __IO uint32_t PWR_CSR_SBF_BB;  	//!< Bit 1 SBF: Standby flag
  __IO uint32_t PWR_CSR_PVDO_BB;  	//!< Bit 2 PVDO: PVD output
  __IO uint32_t PWR_CSR_BRR_BB;  	//!< Bit 3 BRR: Backup regulator ready
  __IO uint32_t reserved2[4]; 	 	//!< Bits 7:4 reserved
  __IO uint32_t PWR_CSR_EWUP_BB;  	//!< Bit 8 EWUP: Enable WKUP pin
  __IO uint32_t PWR_CSR_BRE_BB;  	//!< Bit 9 BRE: Backup regulator enable

} PWR_BB_TypeDef;

/***************************************************************************//**
 * @relates PWR_BB_TypeDef
 * @{
 ******************************************************************************/

/** @name PWR_CR:      	(pwr Offset: 0x00) PWR power control register		  */
/** @{ */
#define PWR_CR_FPDS         0x0200 //!< Bit 9 FPDS: Flash power down in Stop mode
#define PWR_CR_DBP          0x0100 //!< Bit 8 DBP: Disable backup domain write protection
#define PWR_CR_PLS          0x00E0 //!< Bits 7:5 PLS[2:0]: PVD level selection
#define PWR_CR_PLS_2_0V     0x0000 //!<  000: 2.0 V
#define PWR_CR_PLS_2_1V     0x0020 //!<  001: 2.1 V
#define PWR_CR_PLS_2_3V     0x0040 //!<  010: 2.3 V
#define PWR_CR_PLS_2_5V     0x0060 //!<  011: 2.5 V
#define PWR_CR_PLS_2_6V     0x0080 //!<  100: 2.6 V
#define PWR_CR_PLS_2_7V     0x00A0 //!<  101: 2.7 V
#define PWR_CR_PLS_2_8V     0x00C0 //!<  110: 2.8 V
#define PWR_CR_PLS_2_9V     0x00E0 //!<  111: 2.9 V
#define PWR_CR_PVDE         0x0010 //!< Bit 4 PVDE: Power voltage detector enable
#define PWR_CR_CSBF         0x0008 //!< Bit 3 CSBF: Clear standby flag
#define PWR_CR_CWUF         0x0004 //!< Bit 2 CWUF: Clear wakeup flag
#define PWR_CR_PDDS         0x0002 //!< Bit 1 PDDS: Power down deepsleep
#define PWR_CR_LPDS         0x0001 //!< Bit 0 LPDS: Low-power deep sleep
/** @} */

/** @name PWR_CSR:      (pwr Offset: 0x04) PWR power control/status register  */
/** @{ */
#define PWR_CSR_BRE         0x0200 //!< Bit 9 BRE: Backup regulator enable
#define PWR_CSR_EWUP        0x0100 //!< Bit 8 EWUP: Enable WKUP pin
#define PWR_CSR_BRR         0x0008 //!< Bit 3 BRR: Backup regulator ready
#define PWR_CSR_PVDO        0x0004 //!< Bit 2 PVDO: PVD output
#define PWR_CSR_SBF         0x0002 //!< Bit 1 SBF: Standby flag
#define PWR_CSR_WUF         0x0001 //!< Bit 0 WUF: Wakeup flag
/** @} */



/** @} */ // @relates PWR_BB_TypeDef



#endif /* PWR_F2_H_ */

/** @} */
