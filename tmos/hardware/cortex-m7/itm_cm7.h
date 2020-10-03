/**************************************************************************//**
 * @ingroup	hardware_cm7_itm
 * @file	hardware/cortex-m7/itm_cm7.h
 * @brief  	Cortex-M7 ITM
 * @date    18. December 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_cm7_itm  Instrumentation Trace Macrocell (ITM)
 * @ingroup	 hardware_cm7
 * Source and definitions for Cortex-M7  Instrumentation Trace Macrocell (ITM)
 * @{
 *
 ******************************************************************************/

#ifndef ITM_CM7_H_
#define ITM_CM7_H_

#include <tmos_types.h>

/***************************************************************************//**
 *  Instrumentation Trace Macrocell (ITM) 	base 0xE0000000
 ******************************************************************************/
#ifndef __ASSEMBLY__
struct ITM_Type
{
  __O  ureg_t	ITM_PORT [32];  //!< (ITM Offset: 0x000) ITM Stimulus Port Registers
	   uint32_t reserved0[864]; //!< (ITM Offset: 0x080)
  __IO uint32_t ITM_TER;        //!< (ITM Offset: 0xE00) ITM Trace Enable Register
	   uint32_t reserved1[15];	//!< (ITM Offset: 0xE04)
  __IO uint32_t ITM_TPR;        //!< (ITM Offset: 0xE40) ITM Trace Privilege Register
	   uint32_t reserved2[15];	//!< (ITM Offset: 0xE44)
  __IO uint32_t ITM_TCR;        //!< (ITM Offset: 0xE80) ITM Trace Control Register
	   uint32_t reserved3[29];	//!< (ITM Offset: 0xE84)
  __IO uint32_t ITM_IWR;        //!< (ITM Offset: 0xEF8) ITM Integration Write Register
  __IO uint32_t ITM_IRR;        //!< (ITM Offset: 0xEFC) ITM Integration Read Register
  __IO uint32_t ITM_IMCR;       //!< (ITM Offset: 0xF00) ITM Integration Mode Control Register
	   uint32_t reserved4[43];	//!< (ITM Offset: 0xF04)
  __IO uint32_t ITM_LAR;        //!< (ITM Offset: 0xFB0) ITM Lock Access Register
  __IO uint32_t ITM_LSR;        //!< (ITM Offset: 0xFB4) ITM Lock Status Register
	   uint32_t reserved5[6];	//!< (ITM Offset: 0xFB8)
  __I  uint32_t ITM_PID4;       //!< (ITM Offset: 0xFD0) ITM Peripheral Identification Register #4
  __I  uint32_t ITM_PID5;       //!< (ITM Offset: 0xFD4) ITM Peripheral Identification Register #5
  __I  uint32_t ITM_PID6;       //!< (ITM Offset: 0xFD8) ITM Peripheral Identification Register #6
  __I  uint32_t ITM_PID7;       //!< (ITM Offset: 0xFDC) ITM Peripheral Identification Register #7
  __I  uint32_t ITM_PID0;       //!< (ITM Offset: 0xFE0) ITM Peripheral Identification Register #0
  __I  uint32_t ITM_PID1;       //!< (ITM Offset: 0xFE4) ITM Peripheral Identification Register #1
  __I  uint32_t ITM_PID2;       //!< (ITM Offset: 0xFE8) ITM Peripheral Identification Register #2
  __I  uint32_t ITM_PID3;       //!< (ITM Offset: 0xFEC) ITM Peripheral Identification Register #3
  __I  uint32_t ITM_CID0;       //!< (ITM Offset: 0xFF0) ITM Component  Identification Register #0
  __I  uint32_t ITM_CID1;       //!< (ITM Offset: 0xFF4) ITM Component  Identification Register #1
  __I  uint32_t ITM_CID2;       //!< (ITM Offset: 0xFF8) ITM Component  Identification Register #2
  __I  uint32_t ITM_CID3;       //!< (ITM Offset: 0xFFC) ITM Component  Identification Register #3
};
#endif /* __ASSEMBLY__ */

/***************************************************************************//**
 * @relates ITM_Type
 * @{
 ******************************************************************************/

/** @name ITM_PORT [32]: (ITM Offset: 0x000) ITM Stimulus Port Registers	  */
/** @{ */
#define ITM_PORT_full			0x00000000 //!< Stimulus port FIFO status is full
#define ITM_PORT_not_full		0x00000001 //!< Stimulus port FIFO status is not full
/** @} */

/** @name ITM_TER:       (ITM Offset: 0xE00) ITM Trace Enable Register        */
/** @{ */
#define ITM_TER_STIMENA 		0xFFFFFFFF //!< Bit mask to enable tracing on ITM stimulus ports. One bit per stimulus port
/** @} */

/** @name ITM_TPR:       (ITM Offset: 0xE40) ITM Trace Privilege Register	  */
/** @{ */
#define ITM_TPR_PRIVMASK		0x0000000F //!< Bit mask to enable tracing on ITM stimulus ports
/** @} */

/** @name ITM_TCR:       (ITM Offset: 0xE80) ITM Trace Control Register	      */
/** @{ */
#define ITM_TCR_BUSY        	0x00800000 //!< Set when ITM events present and being drained
#define ITM_TCR_ATBID       	0x007F0000 //!< ATB ID for CoreSight system
#define ITM_TCR_TSPrescale  	0x00000300 //!< Timestamp prescaler
#define ITM_TCR_TSPrescale_1  	0x00000000 //!< 	no prescaling
#define ITM_TCR_TSPrescale_4  	0x00000100 //!< 	divide by 4
#define ITM_TCR_TSPrescale_16  	0x00000200 //!< 	divide by 16
#define ITM_TCR_TSPrescale_64  	0x00000300 //!< 	divide by 64
#define ITM_TCR_SWOENA      	0x00000010 //!< Enable SWV behavior – count on TPIUACTV and TPIUBAUD
#define ITM_TCR_DWTENA      	0x00000008 //!< Enables the DWT stimulus
#define ITM_TCR_SYNCENA     	0x00000004 //!< Enables sync packets for TPIU
#define ITM_TCR_TSENA       	0x00000002 //!< Enables differential timestamps
#define ITM_TCR_ITMENA      	0x00000001 //!< Enable ITM
/** @} */

/** @name ITM_IWR:       (ITM Offset: 0xEF8) ITM Integration Write Register   */
/** @{ */
#define ITM_IWR_ATVALIDM        0x00000001 //!< integration mode is set
/** @} */

/** @name ITM_IRR:       (ITM Offset: 0xEFC) ITM Integration Read Register    */
/** @{ */
#define ITM_IRR_ATREADYM        0x00000001 //!< Value on ATREADYM
/** @} */

/** @name ITM_IMCR:      (ITM Offset: 0xF00) ITM Integration Mode Control Register */
/** @{ */
#define ITM_IMCR_INTEGRATION  	0x00000001 //!< ATVALIDM driven from Integration Write Register
/** @} */

/** @name ITM_LAR:       (ITM Offset: 0xFB0) ITM Lock Access Register              */
/** @{ */
#define ITM_LAR_KEY		        0xC5ACCE55 //!< A privileged write of 0xC5ACCE55 enables more write access to Control Register 0xE00::0xFFC. An invalid write removes write access.
/** @} */

/** @name ITM_LSR:       (ITM Offset: 0xFB4) ITM Lock Status Register              */
/** @{ */
#define ITM_LSR_ByteAcc         0x00000004 //!< You cannot implement 8-bit lock accesses
#define ITM_LSR_Access       	0x00000002 //!< Write access to component is blocked. All writes are ignored, reads are permitted
#define ITM_LSR_Present         0x00000001 //!< Indicates that a lock mechanism exists for this component
/** @} */

/** @name ITM_PID4:      (ITM Offset: 0xFD0) ITM Peripheral Identification Register #4 */
/** @{ */
/** @} */

/** @name ITM_PID5:      (ITM Offset: 0xFD4) ITM Peripheral Identification Register #5 */
/** @{ */
/** @} */

/** @name ITM_PID6:      (ITM Offset: 0xFD8) ITM Peripheral Identification Register #6 */
/** @{ */
/** @} */

/** @name ITM_PID7:      (ITM Offset: 0xFDC) ITM Peripheral Identification Register #7 */
/** @{ */
/** @} */

/** @name ITM_PID0:      (ITM Offset: 0xFE0) ITM Peripheral Identification Register #0 */
/** @{ */
/** @} */

/** @name ITM_PID1:      (ITM Offset: 0xFE4) ITM Peripheral Identification Register #1 */
/** @{ */
/** @} */

/** @name ITM_PID2:      (ITM Offset: 0xFE8) ITM Peripheral Identification Register #2 */
/** @{ */
/** @} */

/** @name ITM_PID3:      (ITM Offset: 0xFEC) ITM Peripheral Identification Register #3 */
/** @{ */
/** @} */

/** @name ITM_CID0:      (ITM Offset: 0xFF0) ITM Component  Identification Register #0 */
/** @{ */
/** @} */

/** @name ITM_CID1:      (ITM Offset: 0xFF4) ITM Component  Identification Register #1 */
/** @{ */
/** @} */

/** @name ITM_CID2:      (ITM Offset: 0xFF8) ITM Component  Identification Register #2 */
/** @{ */
/** @} */

/** @name ITM_CID3:      (ITM Offset: 0xFFC) ITM Component  Identification Register #3 */
/** @{ */
/** @} */


/** @} */ // @relates ITM_Type

#endif /* ITM_CM7_H_ */

/** @} */
