/**************************************************************************//**
 * @ingroup	 hardware_lm3s
 * @defgroup lm3s_wdt Watchdog
 * Watchdog Timer (WDT) hardware.
 * @{
 * @file     hardware/lm3s/wdt_lm3s.h
 * @brief    Macros used when accessing the Watchdog Timer hardware
 * @version  V3.00
 * @date     25. October 2011
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/
#ifndef WDT_LM3S_H_
#define WDT_LM3S_H_


//==============================================================================
// C/C++ definitions
//==============================================================================
#ifndef __ASSEMBLY__
#ifdef __cplusplus
extern "C" {
#endif

#include <tmos_types.h>

/// Watchdog Timers Registers
typedef struct
{
    __IO uint32_t WDTLOAD; 			//!< (off 0x000) Watchdog Load
    __I  uint32_t WDTVALUE;         //!< (off 0x004) Watchdog Value
    __IO uint32_t WDTCTL;         	//!< (off 0x008) Watchdog Control
    __O  uint32_t WDTICR;           //!< (off 0x00C) Watchdog Interrupt Clear
    __I  uint32_t WDTRIS;           //!< (off 0x010) Watchdog Raw Interrupt Status
    __I  uint32_t WDTMIS;           //!< (off 0x014) Watchdog Masked Interrupt Status
         uint8_t  reserved0[1024];	//!< reserved
    __IO uint32_t WDTTEST;          //!< (off 0x418) Watchdog Test
         uint8_t  reserved1[2020];	//!< reserved
    __IO uint32_t WDTLOCK;          //!< (off 0xC00) Watchdog Lock
    	 uint8_t  reserved2[972];	//!< reserved
	__I  uint32_t WDTPeriphID4;     //!< (off 0xFD0) Watchdog Peripheral Identification 4
	__I  uint32_t WDTPeriphID5;     //!< (off 0xFD4) Watchdog Peripheral Identification 5
	__I  uint32_t WDTPeriphID6;     //!< (off 0xFD8) Watchdog Peripheral Identification 6
	__I  uint32_t WDTPeriphID7;     //!< (off 0xFDC) Watchdog Peripheral Identification 7
	__I  uint32_t WDTPeriphID0;     //!< (off 0xFE0) Watchdog Peripheral Identification 0
	__I  uint32_t WDTPeriphID1;     //!< (off 0xFE4) Watchdog Peripheral Identification 1
	__I  uint32_t WDTPeriphID2;     //!< (off 0xFE8) Watchdog Peripheral Identification 2
	__I  uint32_t WDTPeriphID3;     //!< (off 0xFEC) Watchdog Peripheral Identification 3
	__I  uint32_t WDTPCellID0;      //!< (off 0xFF0) Watchdog PrimeCell Identification 0
	__I  uint32_t WDTPCellID1;      //!< (off 0xFF4) Watchdog PrimeCell Identification 1
	__I  uint32_t WDTPCellID2;      //!< (off 0xFF8) Watchdog PrimeCell Identification 2
	__I  uint32_t WDTPCellID3;      //!< (off 0xFFC) Watchdog PrimeCell Identification 3
} WDT_Type;

#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */



/// @name off 0x008 WDTCTL: 	Watchdog Control
/** Write Complete
 * @note This bit is reserved for WDT0 and has a reset value of 0
 */
#define WDT_WDTCTL_WRC          0x80000000
#define WDT_WDTCTL_RESEN        0x00000002	//!< Enable reset output
/// @note once this bit is set, it can only be cleared by a hardware reset
#define WDT_WDTCTL_INTEN      	0x00000001	//!< Watchdog Interrupt Enable

/// @name off 0x00C WDTICR:		Watchdog Interrupt Clear
#define WDT_WDTICR_WDTICR		0x00000001	 //!< Watchdog Interrupt Clear

/// @name off 0x010 WDTRIS:		Watchdog Raw Interrupt Status
#define WDT_WDTRIS_WDTRIS       0x00000001  //!< Watchdog Raw Interrupt Status

/// @name off 0x014 WDTMIS:		Watchdog Masked Interrupt Status
#define WDT_WDTMIS_WDTMIS       0x00000001  //!< Watchdog Masked Interrupt Status

/// @name off 0x418 WDTTEST:	Watchdog Test
#define WDT_WDTTEST_STALL       0x00000100  //!< Watchdog stall enable

/// @name off 0xC00 WDTLOCK:	Watchdog Lock
#define WDT_WDTLOCK_UNLOCKED    0x00000000  //!< Watchdog timer is unlocked
#define WDT_WDTLOCK_LOCKED      0x00000001  //!< Watchdog timer is locked
#define WDT_WDTLOCK_UNLOCK      0x1ACCE551  //!< Unlocks the watchdog timer

#define WDT0         ((WDT_Type *)WATCHDOG0_BASE)	//!< Watchdog 0
#define WDT1         ((WDT_Type *)WATCHDOG1_BASE)	//!< Watchdog 0

#endif /* WDT_LM3S_H_ */
/** @} defgroup lm3s_wdt Watchdog */
/** @} defgroup lm3s_wdt Watchdog */
