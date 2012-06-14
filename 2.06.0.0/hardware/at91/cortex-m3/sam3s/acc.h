/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */
/**************************************************************************//**
 * @ingroup	 hardware_sam3s
 * @defgroup  SAM3S_ACC Analog Comparator Controller (ACC)
 * Interface for configuration the Analog-to-Digital Converter (ACC) peripheral.
 * @{
 * @section Usage
 *
 *  -# Configurate the pins for ACC
 *  -# Initialize the ACC with ACC_Initialize().
 *  -# Select the active channel using ACC_EnableChannel()
 *  -# Start the conversion with ACC_StartConversion()
 *  -# Wait the end of the conversion by polling status with ACC_GetStatus()
 *  -# Finally, get the converted data using ACC_GetConvertedData()
 *
 * @section acc_module Working with ACC
 * The ACC driver provides the interface to configure and use the ACC peripheral.\n
 *
 * It applies comparison on two inputs and gives a compare output.
 *
 * To Enable a ACC Comparison,the user has to follow these few steps:
 * <ul>
 * <li> Enable ACC peripheral clock by setting the corresponding bit in PMC_PCER1
 *      (PMC Peripheral Clock Enable Register 1)
 * </li>
 * <li> Reset the controller by asserting ACC_CR_SWRST in ACC_CR(ACC Control Register)
 </li>
 * <li> Configure the mode as following steps:  </li>
 * -#   Select inputs for SELMINUS and SELPLUS in ACC_MR (ACC Mode Register).
 * -#   Enable Analog Comparator by setting ACEN in ACC_MR.
 * -#   Configure Edge Type to detect different compare output.
 * </li>
 * <li> Wait until the automatic mask period expires by polling MASK bit in
 *      ACC_ISR.
 * </ul>
 *
 * For more accurate information, please look at the ACC section of the
 * Datasheet.
 *
 * @file     hardware/at91/cortex-m3/sam3s/acc.h
 * @ingroup	 SAM3S_ACC
 * @brief    ACC header
 *
 * @}
 */

#ifndef _ACC_
#define _ACC_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
//#include "chip.h"
//
//#include <stdint.h>
//#include <assert.h>

/*------------------------------------------------------------------------------
 *         Definitions
 *----------------------------------------------------------------------------*/
#define ACC_SELPLUS_AD12B0 0		//!< doc!
#define ACC_SELPLUS_AD12B1 1		//!< doc!
#define ACC_SELPLUS_AD12B2 2		//!< doc!
#define ACC_SELPLUS_AD12B3 3		//!< doc!
#define ACC_SELPLUS_AD12B4 4		//!< doc!
#define ACC_SELPLUS_AD12B5 5		//!< doc!
#define ACC_SELPLUS_AD12B6 6		//!< doc!
#define ACC_SELPLUS_AD12B7 7		//!< doc!
#define ACC_SELMINUS_TS 0			//!< doc!
#define ACC_SELMINUS_ADVREF 1		//!< doc!
#define ACC_SELMINUS_DAC0   2		//!< doc!
#define ACC_SELMINUS_DAC1   3		//!< doc!
#define ACC_SELMINUS_AD12B0 4		//!< doc!
#define ACC_SELMINUS_AD12B1 5		//!< doc!
#define ACC_SELMINUS_AD12B2 6		//!< doc!
#define ACC_SELMINUS_AD12B3 7		//!< doc!

#ifdef __cplusplus
 extern "C" {
#endif

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR Analog Comparator Controller */
/* ========================================================================== */

#ifndef __ASSEMBLY__
 /** \brief Acc hardware registers */
 typedef struct {
   WoReg ACC_CR;        //!< (Acc Offset: 0x00) Control Register
   RwReg ACC_MR;        //!< (Acc Offset: 0x04) Mode Register
   RwReg Reserved1[7];  //!< doc!
   WoReg ACC_IER;       //!< (Acc Offset: 0x24) Interrupt Enable Register
   WoReg ACC_IDR;       //!< (Acc Offset: 0x28) Interrupt Disable Register
   RoReg ACC_IMR;       //!< (Acc Offset: 0x2C) Interrupt Mask Register
   RoReg ACC_ISR;       //!< (Acc Offset: 0x30) Interrupt Status Register
   RwReg Reserved2[24]; //!< doc!
   RwReg ACC_ACR;       //!< (Acc Offset: 0x94) Analog Control Register
   RwReg Reserved3[19]; //!< doc!
   RwReg ACC_WPMR;      //!< (Acc Offset: 0xE4) Write Protect Mode Register
   RoReg ACC_WPSR;      //!< (Acc Offset: 0xE8) Write Protect Status Register
 } Acc;
#endif /* __ASSEMBLY__ */


/* -------- ACC_CR : (ACC Offset: 0x00) Control Register -------- */
#define ACC_CR_SWRST (0x1u << 0) //!< (ACC_CR) SoftWare ReSeT */
/* -------- ACC_MR : (ACC Offset: 0x04) Mode Register -------- */
#define ACC_MR_SELMINUS_Pos 0  //!< doc!
#define ACC_MR_SELMINUS_Msk (0x7u << ACC_MR_SELMINUS_Pos)
 	 	 	 	 	 	 //!< (ACC_MR) SELection for MINUS comparator input
#define   ACC_MR_SELMINUS_TS (0x0u << 0) 	//!< (ACC_MR) SelectTS
#define   ACC_MR_SELMINUS_ADVREF (0x1u << 0)//!< (ACC_MR) Select ADVREF
#define   ACC_MR_SELMINUS_DAC0 (0x2u << 0)	//!< (ACC_MR) Select DAC0
#define   ACC_MR_SELMINUS_DAC1 (0x3u << 0)	//!< (ACC_MR) Select DAC1
#define   ACC_MR_SELMINUS_AD0 (0x4u << 0)	//!< (ACC_MR) Select AD0
#define   ACC_MR_SELMINUS_AD1 (0x5u << 0)	//!< (ACC_MR) Select AD1
#define   ACC_MR_SELMINUS_AD2 (0x6u << 0)	//!< (ACC_MR) Select AD2
#define   ACC_MR_SELMINUS_AD3 (0x7u << 0)	//!< (ACC_MR) Select AD3
#define ACC_MR_SELPLUS_Pos 4 //!< doc!
#define ACC_MR_SELPLUS_Msk (0x7u << ACC_MR_SELPLUS_Pos)
 	 	 	 	 	 	 	//!< (ACC_MR) SELection for PLUS comparator input
#define   ACC_MR_SELPLUS_AD0 (0x0u << 4) //!< (ACC_MR) Select AD0
#define   ACC_MR_SELPLUS_AD1 (0x1u << 4) //!< (ACC_MR) Select AD1
#define   ACC_MR_SELPLUS_AD2 (0x2u << 4) //!< (ACC_MR) Select AD2
#define   ACC_MR_SELPLUS_AD3 (0x3u << 4) //!< (ACC_MR) Select AD3
#define   ACC_MR_SELPLUS_AD4 (0x4u << 4) //!< (ACC_MR) Select AD4
#define   ACC_MR_SELPLUS_AD5 (0x5u << 4) //!< (ACC_MR) Select AD5
#define   ACC_MR_SELPLUS_AD6 (0x6u << 4) //!< (ACC_MR) Select AD6
#define   ACC_MR_SELPLUS_AD7 (0x7u << 4) //!< (ACC_MR) Select AD7
#define ACC_MR_ACEN (0x1u << 8) //!< (ACC_MR) Analog Comparator ENable
#define   ACC_MR_ACEN_DIS (0x0u << 8) //!< (ACC_MR) Analog Comparator Disabled.
#define   ACC_MR_ACEN_EN (0x1u << 8) //!< (ACC_MR) Analog Comparator Enabled.
#define ACC_MR_EDGETYP_Pos 9	 //!< doc!
#define ACC_MR_EDGETYP_Msk (0x3u << ACC_MR_EDGETYP_Pos) //!< (ACC_MR) EDGE TYPe
#define   ACC_MR_EDGETYP_RISING (0x0u << 9) //!< (ACC_MR) only rising edge of comparator output
#define   ACC_MR_EDGETYP_FALLING (0x1u << 9) //!< (ACC_MR) falling edge of comparator output
#define   ACC_MR_EDGETYP_ANY (0x2u << 9) //!< (ACC_MR) any edge of comparator output
#define ACC_MR_INV (0x1u << 12) //!< (ACC_MR) INVert comparator output
#define   ACC_MR_INV_DIS (0x0u << 12) //!< (ACC_MR) Analog Comparator output is directly processed.
#define   ACC_MR_INV_EN (0x1u << 12) //!< (ACC_MR) Analog Comparator output is inverted prior to being processed.
#define ACC_MR_SELFS (0x1u << 13) //!< (ACC_MR) SELection of Fault Source
#define   ACC_MR_SELFS_CF (0x0u << 13) //!< (ACC_MR) the CF flag is used to drive the FAULT output.
#define   ACC_MR_SELFS_OUTPUT (0x1u << 13) //!< (ACC_MR) the output of the Analog Comparator flag is used to drive the FAULT output.
#define ACC_MR_FE (0x1u << 14) //!< (ACC_MR) Fault Enable
#define   ACC_MR_FE_DIS (0x0u << 14) //!< (ACC_MR) the FAULT output is tied to 0.
#define   ACC_MR_FE_EN (0x1u << 14) //!< (ACC_MR) the FAULT output is driven by the signal defined by SELFS.
/* -------- ACC_IER : (ACC Offset: 0x24) Interrupt Enable Register -------- */
#define ACC_IER_CE (0x1u << 0) //!< (ACC_IER) Comparison Edge
/* -------- ACC_IDR : (ACC Offset: 0x28) Interrupt Disable Register -------- */
#define ACC_IDR_CE (0x1u << 0) //!< (ACC_IDR) Comparison Edge
/* -------- ACC_IMR : (ACC Offset: 0x2C) Interrupt Mask Register -------- */
#define ACC_IMR_CE (0x1u << 0) //!< (ACC_IMR) Comparison Edge
/* -------- ACC_ISR : (ACC Offset: 0x30) Interrupt Status Register -------- */
#define ACC_ISR_CE (0x1u << 0) //!< (ACC_ISR) Comparison Edge
#define ACC_ISR_SCO (0x1u << 1) //!< (ACC_ISR) Synchronized Comparator Output
#define ACC_ISR_MASK (0x1u << 31) //!< (ACC_ISR)
/* -------- ACC_ACR : (ACC Offset: 0x94) Analog Control Register -------- */
#define ACC_ACR_ISEL (0x1u << 0) //!< (ACC_ACR) Current SELection
#define   ACC_ACR_ISEL_LOPW (0x0u << 0) //!< (ACC_ACR) low power option.
#define   ACC_ACR_ISEL_HISP (0x1u << 0) //!< (ACC_ACR) high speed option.
#define ACC_ACR_HYST_Pos 1		 //!< doc!
#define ACC_ACR_HYST_Msk (0x3u << ACC_ACR_HYST_Pos) //!< (ACC_ACR) HYSTeresis selection
#define ACC_ACR_HYST(value) ((ACC_ACR_HYST_Msk & ((value) << ACC_ACR_HYST_Pos)))
 	 	 	 	 	 	 	 //!< doc!
/* -------- ACC_WPMR : (ACC Offset: 0xE4) Write Protect Mode Register -------- */
#define ACC_WPMR_WPEN (0x1u << 0) //!< (ACC_WPMR) Write Protect Enable
#define ACC_WPMR_WPKEY_Pos 8  //!< doc!
#define ACC_WPMR_WPKEY_Msk (0xffffffu << ACC_WPMR_WPKEY_Pos) //!< (ACC_WPMR) Write Protect KEY
#define ACC_WPMR_WPKEY(value) ((ACC_WPMR_WPKEY_Msk & ((value) << ACC_WPMR_WPKEY_Pos)))
 	 	 	 	 	 	 	 //!< doc!
/* -------- ACC_WPSR : (ACC Offset: 0xE8) Write Protect Status Register -------- */
#define ACC_WPSR_WPROTERR (0x1u << 0) //!< (ACC_WPSR) Write PROTection ERRor


/*------------------------------------------------------------------------------
 *         Macros function of register access
 *----------------------------------------------------------------------------*/
#define ACC_CfgModeReg(pAcc, mode)  { \
            (pAcc)->ACC_MR = (mode);\
        }		//!< doc!

#define ACC_GetModeReg( pAcc )      ((pAcc)->ACC_MR)		//!< doc!

#define ACC_StartConversion( pAcc ) ((pAcc)->ACC_CR = ACC_CR_START)		//!< doc!

#define ACC_SoftReset( pAcc )       ((pAcc)->ACC_CR = ACC_CR_SWRST)		//!< doc!

#define ACC_EnableChannel( pAcc, dwChannel )    {\
            assert( dwChannel < 16 ) ;\
            (pAcc)->ACC_CHER = (1 << (dwChannel));\
        }		//!< doc!

#define ACC_DisableChannel( pAcc, dwChannel )  {\
            assert( dwChannel < 16 ) ;\
            (pAcc)->ACC_CHDR = (1 << (dwChannel));\
        }		//!< doc!

#define ACC_EnableIt( pAcc, dwMode )            {\
            assert( ((dwMode)&0xFFF00000)== 0 ) ;\
            (pAcc)->ACC_IER = (dwMode);\
        }		//!< doc!

#define ACC_DisableIt( pAcc, dwMode )           {\
            assert( ((dwMode)&0xFFF00000)== 0 ) ;\
            (pAcc)->ACC_IDR = (dwMode);\
        }		//!< doc!

#define ACC_EnableDataReadyIt( pAcc )      ((pAcc)->ACC_IER = AT91C_ACC_DRDY)//!< doc!

#define ACC_GetStatus( pAcc )              ((pAcc)->ACC_ISR)		//!< doc!

#define ACC_GetChannelStatus( pAcc )       ((pAcc)->ACC_CHSR)		//!< doc!

#define ACC_GetInterruptMaskStatus( pAcc ) ((pAcc)->ACC_IMR)		//!< doc!

#define ACC_GetLastConvertedData( pAcc )   ((pAcc)->ACC_LCDR)		//!< doc!

#define ACC_CfgAnalogCtrlReg( pAcc, dwMode )     {\
            assert( ((dwMode) & 0xFFFCFF3C) == 0 ) ;\
            (pAcc)->ACC_ACR = (dwMode);\
        }		//!< doc!

#define ACC_CfgExtModeReg( pAcc, extmode )    {\
            assert( ((extmode) & 0xFF00FFFE) == 0 ) ;\
            (pAcc)->ACC_EMR = (extmode);\
        }		//!< doc!

#define ACC_GetAnalogCtrlReg( pAcc )          ((pAcc)->ACC_ACR)		//!< doc!

/*------------------------------------------------------------------------------
 *         Exported functions
 *------------------------------------------------------------------------------*/
extern void ACC_Configure( Acc *pAcc, uint8_t idAcc, uint8_t ucSelplus, uint8_t ucSelminus,
                           uint16_t wAc_en, uint16_t wEdge, uint16_t wInvert ) ;

extern void ACC_SetComparisionPair( Acc *pAcc, uint8_t ucSelplus, uint8_t ucSelminus ) ;

extern uint32_t ACC_GetComparisionResult( Acc* pAcc, uint32_t dwStatus ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _ACC_ */
