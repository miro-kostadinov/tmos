/**************************************************************************//**
 * @ingroup	hardware_cm3_tpiu
 * @file	hardware/cortex-m3/tpiu_cm3.h
 * @brief  	Cortex-M3 TPIU
 * @date    18. December 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_cm3_tpiu  Trace Port Interface Unit (TPIU)
 * @ingroup	 hardware_cm3
 * Source and definitions for Cortex-M3  Trace Port Interface Unit (TPIU)
 * @{
 *
 ******************************************************************************/

#ifndef TPIU_CM3_H_
#define TPIU_CM3_H_

#include <tmos_types.h>

/***************************************************************************//**
 *  Trace Port Interface Unit (TPIU)
 ******************************************************************************/
#ifndef __ASSEMBLY__
struct TPIU_Type
{
  __I  uint32_t TPIU_SSPSR;		//!< (TPIU Offset: 0x000) Supported Sync Port Sizes Register
  __IO uint32_t TPIU_CSPSR;		//!< (TPIU Offset: 0x004) Current Sync Port Size Register
       uint32_t reserved1[2];	//!< (TPIU Offset: 0x008)
  __IO uint32_t TPIU_ACPR;		//!< (TPIU Offset: 0x010) Async Clock Prescaler Register
       uint32_t reserved2[55];	//!< (TPIU Offset: 0x014)
  __IO uint32_t TPIU_SPPR;		//!< (TPIU Offset: 0x0F0) Selected Pin Protocol Register
  	   uint32_t reserved3[131];	//!< (TPIU Offset: 0x014)
  __I  uint32_t TPIU_FFSR;		//!< (TPIU Offset: 0x300) Formatter and Flush Status Register
  __IO uint32_t TPIU_FFCR;		//!< (TPIU Offset: 0x304) Formatter and Flush Control Register
  __I  uint32_t TPIU_FSCR;		//!< (TPIU Offset: 0x308) Formatter Synchronization Counter Register
	   uint32_t reserved4[759];	//!< (TPIU Offset: 0x30C)
  __I  uint32_t TPIU_TRIGGER; 	//!< (TPIU Offset: 0xEE8) Integration Register: TRIGGER
  __I  uint32_t TPIU_FIFO0; 	//!< (TPIU Offset: 0xEEC) Integration register : FIFO data 0
  __I  uint32_t TPIU_ITATBCTR2;	//!< (TPIU Offset: 0xEF0) Integration Register: ITATBCTR2
  	   uint32_t reserved5;		//!< (TPIU Offset: 0xEF4)
  __I  uint32_t TPIU_ITATBCTR0; //!< (TPIU Offset: 0xEF8) Integration Register: ITATBCTR0
  __I  uint32_t TPIU_FIFO1;  	//!< (TPIU Offset: 0xEFC) Integration register : FIFO data 1
  __IO uint32_t TPIU_IMCR; 		//!< (TPIU Offset: 0xF00) Integration Mode Control Register
	   uint32_t reserved6[39];	//!< (TPIU Offset: 0xF04)
  __IO uint32_t TPIU_CTS;  		//!< (TPIU Offset: 0xFA0) Claim tag set register
  __IO uint32_t TPIU_CTC; 		//!< (TPIU Offset: 0xFA4) Claim tag clear register
  	   uint32_t reserved7[30];	//!< (TPIU Offset: 0xF08)
  __I  uint32_t TPIU_DEVID; 	//!< (TPIU Offset: 0xFC8) Device ID register
};
#endif /* __ASSEMBLY__ */

/***************************************************************************//**
 * @relates TPIU_Type
 * @{
 ******************************************************************************/

/** @name TPIU_SSPSR: //!< (TPIU Offset: 0x000) Supported Sync Port Sizes Register */
/** @{ */
#define TPIU_SSPSR_WORD			   0x00000008 //!< port size 4
#define TPIU_SSPSR_HALFWORD		   0x00000002 //!< port size 2
#define TPIU_SSPSR_BYTE			   0x00000001 //!< port size 1
/** @} */

/** @name TPIU_CSPSR: //!< (TPIU Offset: 0x004) Current Sync Port Size Register */
/** @{ */
#define TPIU_CSPSR_WORD			   0x00000008 //!< port size 4
#define TPIU_CSPSR_HALFWORD		   0x00000002 //!< port size 2
#define TPIU_CSPSR_BYTE			   0x00000001 //!< port size 1
/** @} */

/** @name TPIU_ACPR:  //!< (TPIU Offset: 0x010) Async Clock Prescaler Register */
/** @{ */
#define TPIU_ACPR_PRESCALER	 	   0x00001FFF //!< Divisor for TRACECLKIN is Prescaler + 1.
/** @} */

/** @name TPIU_SPPR:  //!< (TPIU Offset: 0x0F0) Selected Pin Protocol Register */
/** @{ */
#define TPIU_SPPR_PROTOCOL	 	   0x00000003 //!< Protocol to use for trace output
#define TPIU_SPPR_SYNC		 	   0x00000000 //!< 	TracePort mode
#define TPIU_SPPR_ASYNC_MANCHESTER 0x00000001 //!< 	SerialWire Output (Manchester)
#define TPIU_SPPR_ASYNC_NRZ 	   0x00000002 //!< 	SerialWire Output (NRZ)
/** @} */

/** @name TPIU_FFSR:  //!< (TPIU Offset: 0x300) Formatter and Flush Status Register */
/** @{ */
#define TPIU_FFSR_FTNONSTOP		   0x00000008 //!< Formatter cannot be stopped
#define TPIU_FFSR_TCPRESENT		   0x00000004 //!< This bit always reads zero
#define TPIU_FFSR_FTSTOPPED		   0x00000002 //!< This bit always reads zero
#define TPIU_FFSR_FLINPROG		   0x00000001 //!< This bit always reads zero
/** @} */

/** @name TPIU_FFCR:  //!< (TPIU Offset: 0x304) Formatter and Flush Control Register */
/** @{ */
#define TPIU_FFCR_StopTrig 		   0x00002000 //!< Stop the formatter after a Trigger Event is observed
#define TPIU_FFCR_StopFI 		   0x00001000 //!< Stop the formatter after a flush completes
#define TPIU_FFCR_TrigFI 		   0x00000400 //!< Indicates a trigger on Flush completion
#define TPIU_FFCR_TrigEVT 		   0x00000200 //!< Indicate a trigger on a Trigger Event
#define TPIU_FFCR_TrigIN 		   0x00000100 //!< Indicate a trigger on TRIGIN being asserted
#define TPIU_FFCR_FOnMan 		   0x00000040 //!< Manually generate a flush of the system
#define TPIU_FFCR_FOnTrig 		   0x00000020 //!< Generate flush using Trigger event
#define TPIU_FFCR_FOnFlln 		   0x00000010 //!< Generate flush using the FLUSHIN interface
#define TPIU_FFCR_EnFCont 		   0x00000002 //!< Continuous Formatting, no TRACECTL. This bit is set on reset
#define TPIU_FFCR_EnFTC 		   0x00000001 //!< Enable Formatting. Because TRACECTL is never present, this bit reads as zero
/** @} */

/** @name TPIU_FSCR:  //!< (TPIU Offset: 0x308) Formatter Synchronization Counter Register */
/** @{ */
/** @} */





/** @} */ // @relates TPIU_Type



#endif /* TPIU_CM3_H_ */

/** @} */

