/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
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

/**
 * @ingroup	 hardware_at91
 * @defgroup  AT91_PWM Pulse Width Modulation Controller (PWM) peripheral
 * Interface for PWM
 * @{
 *
 *  \section Purpose
 *
 *  Interface for configuration the Pulse Width Modulation Controller (PWM)
 *  peripheral.
 *
 *  \section Usage
 *
 *    -# Configures PWM clocks A & B to run at the given frequencies using
 *       PWMC_ConfigureClocks().
 *    -# Configure PWMC channel using PWMC_ConfigureChannel(), PWMC_ConfigureChannelExt()
 *       PWMC_SetPeriod(), PWMC_SetDutyCycle() and PWMC_SetDeadTime().
 *    -# Enable & disable channel using  PWMC_EnableChannel() and
 *        PWMC_DisableChannel().
 *    -# Enable & disable the period interrupt for the given PWM channel using
 *       PWMC_EnableChannelIt() and PWMC_DisableChannelIt().
 *    -# Enable & disable the selected interrupts sources on a PWMC peripheral
 *       using  PWMC_EnableIt() and PWMC_DisableIt().
 *    -# Control syncronous channel using PWMC_ConfigureSyncChannel(),
 *       PWMC_SetSyncChannelUpdatePeriod() and PWMC_SetSyncChannelUpdateUnlock().
 *    -# Control PWM override output using PWMC_SetOverrideValue(),
 *       PWMC_EnableOverrideOutput() and PWMC_DisableOverrideOutput().
 *    -# Send data through the transmitter using PWMC_WriteBuffer().
 *
 * @file     hardware/at91/pwmc.h
 * @ingroup	 AT91_PWM
 * @brief    PWM header
 *
 * @}
*/

#ifndef PWMC_H_
#define PWMC_H_

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/
#include <fam_inc.h>
#include <at91_types.h>


#ifdef __cplusplus
 extern "C" {
#endif


/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Pulse Width Modulation Controller */
/* ============================================================================= */
/** \addtogroup SAM3S_PWM Pulse Width Modulation Controller */
/*@{*/

#ifndef __ASSEMBLY__
/** \brief PwmCh_num hardware registers */
typedef struct {
#ifdef	 CFG_SAM7
  RwReg      PWM_CMR;       //!< (PwmCh_num Offset: 0x0) PWM Channel Mode Register */
  RwReg      PWM_CDTY;      //!< (PwmCh_num Offset: 0x4) PWM Channel Duty Cycle Register */
  RwReg      PWM_CPRD;      //!< (PwmCh_num Offset: 0x8) PWM Channel Period Register */
  RoReg      PWM_CCNT;      //!< (PwmCh_num Offset: 0xC) PWM Channel Counter Register */
  WoReg      PWM_CUPD;      //!< (PwmCh_num Offset: 0x10) PWM Channel Update Register */
  RwReg		 PWM_CRSV[3];
#else
  RwReg      PWM_CMR;       //!< (PwmCh_num Offset: 0x0) PWM Channel Mode Register */
  RwReg      PWM_CDTY;      //!< (PwmCh_num Offset: 0x4) PWM Channel Duty Cycle Register */
  RwReg      PWM_CDTYUPD;   //!< (PwmCh_num Offset: 0x8) PWM Channel Duty Cycle Update Register */
  RwReg      PWM_CPRD;      //!< (PwmCh_num Offset: 0xC) PWM Channel Period Register */
  RwReg      PWM_CPRDUPD;   //!< (PwmCh_num Offset: 0x10) PWM Channel Period Update Register */
  RwReg      PWM_CCNT;      //!< (PwmCh_num Offset: 0x14) PWM Channel Counter Register */
  RwReg      PWM_DT;        //!< (PwmCh_num Offset: 0x18) PWM Channel Dead Time Register */
  RwReg      PWM_DTUPD;     //!< (PwmCh_num Offset: 0x1C) PWM Channel Dead Time Update Register */
#endif /* CFG_SAM7 */
} PwmCh_num;

/** \brief PwmCmp hardware registers */
typedef struct {
  RwReg      PWM_CMPxV;     //!< (PwmCmp Offset: 0x0) PWM Comparison 0 Value Register */
  RwReg      PWM_CMPxVUPD;  //!< (PwmCmp Offset: 0x4) PWM Comparison 0 Value Update Register */
  RwReg      PWM_CMPxM;     //!< (PwmCmp Offset: 0x8) PWM Comparison 0 Mode Register */
  RwReg      PWM_CMPxMUPD;  //!< (PwmCmp Offset: 0xC) PWM Comparison 0 Mode Update Register */
} PwmCmp;

/** \brief Pwm hardware registers */
typedef struct {
  RwReg      PWM_CLK;       //!< (Pwm Offset: 0x00) PWM Clock Register */
  WoReg      PWM_ENA;       //!< (Pwm Offset: 0x04) PWM Enable Register */
  WoReg      PWM_DIS;       //!< (Pwm Offset: 0x08) PWM Disable Register */
  RoReg      PWM_SR;        //!< (Pwm Offset: 0x0C) PWM Status Register */
  WoReg      PWM_IER1;      //!< (Pwm Offset: 0x10) PWM Interrupt Enable Register 1 */
  WoReg      PWM_IDR1;      //!< (Pwm Offset: 0x14) PWM Interrupt Disable Register 1 */
  RoReg      PWM_IMR1;      //!< (Pwm Offset: 0x18) PWM Interrupt Mask Register 1 */
  RoReg      PWM_ISR1;      //!< (Pwm Offset: 0x1C) PWM Interrupt Status Register 1 */
#ifndef	 CFG_SAM7
  RwReg      PWM_SCM;       //!< (Pwm Offset: 0x20) PWM Sync Channels Mode Register */
  RwReg      Reserved1[1];	//!< reserved
  RwReg      PWM_SCUC;      //!< (Pwm Offset: 0x28) PWM Sync Channels Update Control Register */
  RwReg      PWM_SCUP;      //!< (Pwm Offset: 0x2C) PWM Sync Channels Update Period Register */
  WoReg      PWM_SCUPUPD;   //!< (Pwm Offset: 0x30) PWM Sync Channels Update Period Update Register */
  WoReg      PWM_IER2;      //!< (Pwm Offset: 0x34) PWM Interrupt Enable Register 2 */
  WoReg      PWM_IDR2;      //!< (Pwm Offset: 0x38) PWM Interrupt Disable Register 2 */
  RoReg      PWM_IMR2;      //!< (Pwm Offset: 0x3C) PWM Interrupt Mask Register 2 */
  RoReg      PWM_ISR2;      //!< (Pwm Offset: 0x40) PWM Interrupt Status Register 2 */
  RwReg      PWM_OOV;       //!< (Pwm Offset: 0x44) PWM Output Override Value Register */
  RwReg      PWM_OS;        //!< (Pwm Offset: 0x48) PWM Output Selection Register */
  WoReg      PWM_OSS;       //!< (Pwm Offset: 0x4C) PWM Output Selection Set Register */
  WoReg      PWM_OSC;       //!< (Pwm Offset: 0x50) PWM Output Selection Clear Register */
  WoReg      PWM_OSSUPD;    //!< (Pwm Offset: 0x54) PWM Output Selection Set Update Register */
  WoReg      PWM_OSCUPD;    //!< (Pwm Offset: 0x58) PWM Output Selection Clear Update Register */
  RwReg      PWM_FMR;       //!< (Pwm Offset: 0x5C) PWM Fault Mode Register */
  RoReg      PWM_FSR;       //!< (Pwm Offset: 0x60) PWM Fault Status Register */
  WoReg      PWM_FCR;       //!< (Pwm Offset: 0x64) PWM Fault Clear Register */
  RwReg      PWM_FPV;       //!< (Pwm Offset: 0x68) PWM Fault Protection Value Register */
  RwReg      PWM_FPE;       //!< (Pwm Offset: 0x6C) PWM Fault Protection Enable Register */
  RwReg      Reserved2[3];	//!< reserved
  RwReg      PWM_ELxMR[2];  //!< (Pwm Offset: 0x7C) PWM Event Line 0 Mode Register */
  RwReg      Reserved3[11];	//!< reserved
  RwReg      PWM_SMMR;      //!< (Pwm Offset: 0xB0) PWM Stepper Motor Mode Register */
  RwReg      Reserved4[12]; //!< reserved
  WoReg      PWM_WPCR;      //!< (Pwm Offset: 0xE4) PWM Write Protect Control Register */
  RoReg      PWM_WPSR;      //!< (Pwm Offset: 0xE8) PWM Write Protect Status Register */
  RwReg      Reserved5[5];	//!< reserved
  RwReg      PWM_RPR;       //!< (Pwm Offset: 0x100) Receive Pointer Register */
  RwReg      PWM_RCR;       //!< (Pwm Offset: 0x104) Receive Counter Register */
  RwReg      PWM_TPR;       //!< (Pwm Offset: 0x108) Transmit Pointer Register */
  RwReg      PWM_TCR;       //!< (Pwm Offset: 0x10C) Transmit Counter Register */
  RwReg      PWM_RNPR;      //!< (Pwm Offset: 0x110) Receive Next Pointer Register */
  RwReg      PWM_RNCR;      //!< (Pwm Offset: 0x114) Receive Next Counter Register */
  RwReg      PWM_TNPR;      //!< (Pwm Offset: 0x118) Transmit Next Pointer Register */
  RwReg      PWM_TNCR;      //!< (Pwm Offset: 0x11C) Transmit Next Counter Register */
  WoReg      PWM_PTCR;      //!< (Pwm Offset: 0x120) Transfer Control Register */
  RoReg      PWM_PTSR;      //!< (Pwm Offset: 0x124) Transfer Status Register */
  RwReg      Reserved6[2];	//!< reserved
  PwmCmp     PWM_CMP[8];    //!< (Pwm Offset: 0x130) cmp = 0 .. 7 */
  RwReg      Reserved7[20]; //!< reserved
#else
  RwReg		Reserved8[(0x200 - 0x20)/4];
#endif /* CFG_SAM7 */
  PwmCh_num  PWM_CH_NUM[4]; //!< (Pwm Offset: 0x200) ch_num = 0 .. 3 */
} Pwm;
#endif /* __ASSEMBLY__ */
/* -------- PWM_CLK : (PWM Offset: 0x00) PWM Clock Register -------- */
#define PWM_CLK_DIVA_Pos 0	//!< DIVA bit position
#define PWM_CLK_DIVA_Msk (0xffu << PWM_CLK_DIVA_Pos) //!< (PWM_CLK) CLKA, CLKB Divide Factor */
/// set macro
#define PWM_CLK_DIVA(value) ((PWM_CLK_DIVA_Msk & ((value) << PWM_CLK_DIVA_Pos)))
#define PWM_CLK_PREA_Pos 8	//!< PREA bit position
#define PWM_CLK_PREA_Msk (0xfu << PWM_CLK_PREA_Pos) //!< (PWM_CLK) CLKA, CLKB Source Clock Selection */
/// set macro
#define PWM_CLK_PREA(value) ((PWM_CLK_PREA_Msk & ((value) << PWM_CLK_PREA_Pos)))
#define PWM_CLK_DIVB_Pos 16	//!< DIVB bit position
#define PWM_CLK_DIVB_Msk (0xffu << PWM_CLK_DIVB_Pos) //!< (PWM_CLK) CLKA, CLKB Divide Factor */
/// set macro
#define PWM_CLK_DIVB(value) ((PWM_CLK_DIVB_Msk & ((value) << PWM_CLK_DIVB_Pos)))
#define PWM_CLK_PREB_Pos 24	//!< PREB bit position
#define PWM_CLK_PREB_Msk (0xfu << PWM_CLK_PREB_Pos) //!< (PWM_CLK) CLKA, CLKB Source Clock Selection */
/// set macro
#define PWM_CLK_PREB(value) ((PWM_CLK_PREB_Msk & ((value) << PWM_CLK_PREB_Pos)))
/* -------- PWM_ENA : (PWM Offset: 0x04) PWM Enable Register -------- */
#define PWM_ENA_CHID0 (0x1u << 0) //!< (PWM_ENA) Channel ID */
#define PWM_ENA_CHID1 (0x1u << 1) //!< (PWM_ENA) Channel ID */
#define PWM_ENA_CHID2 (0x1u << 2) //!< (PWM_ENA) Channel ID */
#define PWM_ENA_CHID3 (0x1u << 3) //!< (PWM_ENA) Channel ID */
/* -------- PWM_DIS : (PWM Offset: 0x08) PWM Disable Register -------- */
#define PWM_DIS_CHID0 (0x1u << 0) //!< (PWM_DIS) Channel ID */
#define PWM_DIS_CHID1 (0x1u << 1) //!< (PWM_DIS) Channel ID */
#define PWM_DIS_CHID2 (0x1u << 2) //!< (PWM_DIS) Channel ID */
#define PWM_DIS_CHID3 (0x1u << 3) //!< (PWM_DIS) Channel ID */
/* -------- PWM_SR : (PWM Offset: 0x0C) PWM Status Register -------- */
#define PWM_SR_CHID0 (0x1u << 0) //!< (PWM_SR) Channel ID */
#define PWM_SR_CHID1 (0x1u << 1) //!< (PWM_SR) Channel ID */
#define PWM_SR_CHID2 (0x1u << 2) //!< (PWM_SR) Channel ID */
#define PWM_SR_CHID3 (0x1u << 3) //!< (PWM_SR) Channel ID */
/* -------- PWM_IER1 : (PWM Offset: 0x10) PWM Interrupt Enable Register 1 -------- */
#define PWM_IER1_CHID0 (0x1u << 0) //!< (PWM_IER1) Counter Event on Channel 0 Interrupt Enable */
#define PWM_IER1_CHID1 (0x1u << 1) //!< (PWM_IER1) Counter Event on Channel 1 Interrupt Enable */
#define PWM_IER1_CHID2 (0x1u << 2) //!< (PWM_IER1) Counter Event on Channel 2 Interrupt Enable */
#define PWM_IER1_CHID3 (0x1u << 3) //!< (PWM_IER1) Counter Event on Channel 3 Interrupt Enable */
#define PWM_IER1_FCHID0 (0x1u << 16) //!< (PWM_IER1) Fault Protection Trigger on Channel 0 Interrupt Enable */
#define PWM_IER1_FCHID1 (0x1u << 17) //!< (PWM_IER1) Fault Protection Trigger on Channel 1 Interrupt Enable */
#define PWM_IER1_FCHID2 (0x1u << 18) //!< (PWM_IER1) Fault Protection Trigger on Channel 2 Interrupt Enable */
#define PWM_IER1_FCHID3 (0x1u << 19) //!< (PWM_IER1) Fault Protection Trigger on Channel 3 Interrupt Enable */
/* -------- PWM_IDR1 : (PWM Offset: 0x14) PWM Interrupt Disable Register 1 -------- */
#define PWM_IDR1_CHID0 (0x1u << 0) //!< (PWM_IDR1) Counter Event on Channel 0 Interrupt Disable */
#define PWM_IDR1_CHID1 (0x1u << 1) //!< (PWM_IDR1) Counter Event on Channel 1 Interrupt Disable */
#define PWM_IDR1_CHID2 (0x1u << 2) //!< (PWM_IDR1) Counter Event on Channel 2 Interrupt Disable */
#define PWM_IDR1_CHID3 (0x1u << 3) //!< (PWM_IDR1) Counter Event on Channel 3 Interrupt Disable */
#define PWM_IDR1_FCHID0 (0x1u << 16) //!< (PWM_IDR1) Fault Protection Trigger on Channel 0 Interrupt Disable */
#define PWM_IDR1_FCHID1 (0x1u << 17) //!< (PWM_IDR1) Fault Protection Trigger on Channel 1 Interrupt Disable */
#define PWM_IDR1_FCHID2 (0x1u << 18) //!< (PWM_IDR1) Fault Protection Trigger on Channel 2 Interrupt Disable */
#define PWM_IDR1_FCHID3 (0x1u << 19) //!< (PWM_IDR1) Fault Protection Trigger on Channel 3 Interrupt Disable */
/* -------- PWM_IMR1 : (PWM Offset: 0x18) PWM Interrupt Mask Register 1 -------- */
#define PWM_IMR1_CHID0 (0x1u << 0) //!< (PWM_IMR1) Counter Event on Channel 0 Interrupt Mask */
#define PWM_IMR1_CHID1 (0x1u << 1) //!< (PWM_IMR1) Counter Event on Channel 1 Interrupt Mask */
#define PWM_IMR1_CHID2 (0x1u << 2) //!< (PWM_IMR1) Counter Event on Channel 2 Interrupt Mask */
#define PWM_IMR1_CHID3 (0x1u << 3) //!< (PWM_IMR1) Counter Event on Channel 3 Interrupt Mask */
#define PWM_IMR1_FCHID0 (0x1u << 16) //!< (PWM_IMR1) Fault Protection Trigger on Channel 0 Interrupt Mask */
#define PWM_IMR1_FCHID1 (0x1u << 17) //!< (PWM_IMR1) Fault Protection Trigger on Channel 1 Interrupt Mask */
#define PWM_IMR1_FCHID2 (0x1u << 18) //!< (PWM_IMR1) Fault Protection Trigger on Channel 2 Interrupt Mask */
#define PWM_IMR1_FCHID3 (0x1u << 19) //!< (PWM_IMR1) Fault Protection Trigger on Channel 3 Interrupt Mask */
/* -------- PWM_ISR1 : (PWM Offset: 0x1C) PWM Interrupt Status Register 1 -------- */
#define PWM_ISR1_CHID0 (0x1u << 0) //!< (PWM_ISR1) Counter Event on Channel 0 */
#define PWM_ISR1_CHID1 (0x1u << 1) //!< (PWM_ISR1) Counter Event on Channel 1 */
#define PWM_ISR1_CHID2 (0x1u << 2) //!< (PWM_ISR1) Counter Event on Channel 2 */
#define PWM_ISR1_CHID3 (0x1u << 3) //!< (PWM_ISR1) Counter Event on Channel 3 */
#define PWM_ISR1_FCHID0 (0x1u << 16) //!< (PWM_ISR1) Fault Protection Trigger on Channel 0 */
#define PWM_ISR1_FCHID1 (0x1u << 17) //!< (PWM_ISR1) Fault Protection Trigger on Channel 1 */
#define PWM_ISR1_FCHID2 (0x1u << 18) //!< (PWM_ISR1) Fault Protection Trigger on Channel 2 */
#define PWM_ISR1_FCHID3 (0x1u << 19) //!< (PWM_ISR1) Fault Protection Trigger on Channel 3 */
/* -------- PWM_SCM : (PWM Offset: 0x20) PWM Sync Channels Mode Register -------- */
#define PWM_SCM_SYNC0 (0x1u << 0) //!< (PWM_SCM) Synchronous Channel 0 */
#define PWM_SCM_SYNC1 (0x1u << 1) //!< (PWM_SCM) Synchronous Channel 1 */
#define PWM_SCM_SYNC2 (0x1u << 2) //!< (PWM_SCM) Synchronous Channel 2 */
#define PWM_SCM_SYNC3 (0x1u << 3) //!< (PWM_SCM) Synchronous Channel 3 */
#define PWM_SCM_UPDM_Pos 16		  //!< UPDM bit position
#define PWM_SCM_UPDM_Msk (0x3u << PWM_SCM_UPDM_Pos) //!< (PWM_SCM) Synchronous Channels Update Mode */
#define   PWM_SCM_UPDM_MODE0 (0x0u << 16) //!< (PWM_SCM) Manual write of double buffer registers and manual update of synchronous channels. The update occurs at the begin- ning of the next PWM period, when the bit UPDULOCK in "PWM Sync Channels Update Control Register" on page 55 is set. */
#define   PWM_SCM_UPDM_MODE1 (0x1u << 16) //!< (PWM_SCM) Manual write of double buffer registers and automatic update of synchronous channels. The update occurs when the Update Period is elapsed. */
#define   PWM_SCM_UPDM_MODE2 (0x2u << 16) //!< (PWM_SCM) Automatic write of duty-cycle update registers by the PDC and automatic update of synchronous channels. The update occurs when the Update Period is elapsed. */
#define PWM_SCM_PTRM (0x1u << 20) //!< (PWM_SCM) PDC Transfer Request Mode */
#define PWM_SCM_PTRCS_Pos 21	//!< PTRCS bit position
#define PWM_SCM_PTRCS_Msk (0x7u << PWM_SCM_PTRCS_Pos) //!< (PWM_SCM) PDC Transfer Request Comparison Selection */
/// set macro
#define PWM_SCM_PTRCS(value) ((PWM_SCM_PTRCS_Msk & ((value) << PWM_SCM_PTRCS_Pos)))
/* -------- PWM_SCUC : (PWM Offset: 0x28) PWM Sync Channels Update Control Register -------- */
#define PWM_SCUC_UPDULOCK (0x1u << 0) //!< (PWM_SCUC) Synchronous Channels Update Unlock */
/* -------- PWM_SCUP : (PWM Offset: 0x2C) PWM Sync Channels Update Period Register -------- */
#define PWM_SCUP_UPR_Pos 0	//!< UPR bit position
#define PWM_SCUP_UPR_Msk (0xfu << PWM_SCUP_UPR_Pos) //!< (PWM_SCUP) Update Period */
/// set macro
#define PWM_SCUP_UPR(value) ((PWM_SCUP_UPR_Msk & ((value) << PWM_SCUP_UPR_Pos)))
#define PWM_SCUP_UPRCNT_Pos 4	//!< UPRCNT bit position
#define PWM_SCUP_UPRCNT_Msk (0xfu << PWM_SCUP_UPRCNT_Pos) //!< (PWM_SCUP) Update Period Counter */
/// set macro
#define PWM_SCUP_UPRCNT(value) ((PWM_SCUP_UPRCNT_Msk & ((value) << PWM_SCUP_UPRCNT_Pos)))
/* -------- PWM_SCUPUPD : (PWM Offset: 0x30) PWM Sync Channels Update Period Update Register -------- */
#define PWM_SCUPUPD_UPRUPD_Pos 0 //!< UPRUPD bit position
#define PWM_SCUPUPD_UPRUPD_Msk (0xfu << PWM_SCUPUPD_UPRUPD_Pos) //!< (PWM_SCUPUPD) Update Period Update */
/// set macro
#define PWM_SCUPUPD_UPRUPD(value) ((PWM_SCUPUPD_UPRUPD_Msk & ((value) << PWM_SCUPUPD_UPRUPD_Pos)))
/* -------- PWM_IER2 : (PWM Offset: 0x34) PWM Interrupt Enable Register 2 -------- */
#define PWM_IER2_WRDY (0x1u << 0) //!< (PWM_IER2) Write Ready for Synchronous Channels Update Interrupt Enable */
#define PWM_IER2_ENDTX (0x1u << 1) //!< (PWM_IER2) PDC End of TX Buffer Interrupt Enable */
#define PWM_IER2_TXBUFE (0x1u << 2) //!< (PWM_IER2) PDC TX Buffer Empty Interrupt Enable */
#define PWM_IER2_UNRE (0x1u << 3) //!< (PWM_IER2) Synchronous Channels Update Underrun Error Interrupt Enable */
#define PWM_IER2_CMPM0 (0x1u << 8) //!< (PWM_IER2) Comparison 0 Match Interrupt Enable */
#define PWM_IER2_CMPM1 (0x1u << 9) //!< (PWM_IER2) Comparison 1 Match Interrupt Enable */
#define PWM_IER2_CMPM2 (0x1u << 10) //!< (PWM_IER2) Comparison 2 Match Interrupt Enable */
#define PWM_IER2_CMPM3 (0x1u << 11) //!< (PWM_IER2) Comparison 3 Match Interrupt Enable */
#define PWM_IER2_CMPM4 (0x1u << 12) //!< (PWM_IER2) Comparison 4 Match Interrupt Enable */
#define PWM_IER2_CMPM5 (0x1u << 13) //!< (PWM_IER2) Comparison 5 Match Interrupt Enable */
#define PWM_IER2_CMPM6 (0x1u << 14) //!< (PWM_IER2) Comparison 6 Match Interrupt Enable */
#define PWM_IER2_CMPM7 (0x1u << 15) //!< (PWM_IER2) Comparison 7 Match Interrupt Enable */
#define PWM_IER2_CMPU0 (0x1u << 16) //!< (PWM_IER2) Comparison 0 Update Interrupt Enable */
#define PWM_IER2_CMPU1 (0x1u << 17) //!< (PWM_IER2) Comparison 1 Update Interrupt Enable */
#define PWM_IER2_CMPU2 (0x1u << 18) //!< (PWM_IER2) Comparison 2 Update Interrupt Enable */
#define PWM_IER2_CMPU3 (0x1u << 19) //!< (PWM_IER2) Comparison 3 Update Interrupt Enable */
#define PWM_IER2_CMPU4 (0x1u << 20) //!< (PWM_IER2) Comparison 4 Update Interrupt Enable */
#define PWM_IER2_CMPU5 (0x1u << 21) //!< (PWM_IER2) Comparison 5 Update Interrupt Enable */
#define PWM_IER2_CMPU6 (0x1u << 22) //!< (PWM_IER2) Comparison 6 Update Interrupt Enable */
#define PWM_IER2_CMPU7 (0x1u << 23) //!< (PWM_IER2) Comparison 7 Update Interrupt Enable */
/* -------- PWM_IDR2 : (PWM Offset: 0x38) PWM Interrupt Disable Register 2 -------- */
#define PWM_IDR2_WRDY (0x1u << 0) //!< (PWM_IDR2) Write Ready for Synchronous Channels Update Interrupt Disable */
#define PWM_IDR2_ENDTX (0x1u << 1) //!< (PWM_IDR2) PDC End of TX Buffer Interrupt Disable */
#define PWM_IDR2_TXBUFE (0x1u << 2) //!< (PWM_IDR2) PDC TX Buffer Empty Interrupt Disable */
#define PWM_IDR2_UNRE (0x1u << 3) //!< (PWM_IDR2) Synchronous Channels Update Underrun Error Interrupt Disable */
#define PWM_IDR2_CMPM0 (0x1u << 8) //!< (PWM_IDR2) Comparison 0 Match Interrupt Disable */
#define PWM_IDR2_CMPM1 (0x1u << 9) //!< (PWM_IDR2) Comparison 1 Match Interrupt Disable */
#define PWM_IDR2_CMPM2 (0x1u << 10) //!< (PWM_IDR2) Comparison 2 Match Interrupt Disable */
#define PWM_IDR2_CMPM3 (0x1u << 11) //!< (PWM_IDR2) Comparison 3 Match Interrupt Disable */
#define PWM_IDR2_CMPM4 (0x1u << 12) //!< (PWM_IDR2) Comparison 4 Match Interrupt Disable */
#define PWM_IDR2_CMPM5 (0x1u << 13) //!< (PWM_IDR2) Comparison 5 Match Interrupt Disable */
#define PWM_IDR2_CMPM6 (0x1u << 14) //!< (PWM_IDR2) Comparison 6 Match Interrupt Disable */
#define PWM_IDR2_CMPM7 (0x1u << 15) //!< (PWM_IDR2) Comparison 7 Match Interrupt Disable */
#define PWM_IDR2_CMPU0 (0x1u << 16) //!< (PWM_IDR2) Comparison 0 Update Interrupt Disable */
#define PWM_IDR2_CMPU1 (0x1u << 17) //!< (PWM_IDR2) Comparison 1 Update Interrupt Disable */
#define PWM_IDR2_CMPU2 (0x1u << 18) //!< (PWM_IDR2) Comparison 2 Update Interrupt Disable */
#define PWM_IDR2_CMPU3 (0x1u << 19) //!< (PWM_IDR2) Comparison 3 Update Interrupt Disable */
#define PWM_IDR2_CMPU4 (0x1u << 20) //!< (PWM_IDR2) Comparison 4 Update Interrupt Disable */
#define PWM_IDR2_CMPU5 (0x1u << 21) //!< (PWM_IDR2) Comparison 5 Update Interrupt Disable */
#define PWM_IDR2_CMPU6 (0x1u << 22) //!< (PWM_IDR2) Comparison 6 Update Interrupt Disable */
#define PWM_IDR2_CMPU7 (0x1u << 23) //!< (PWM_IDR2) Comparison 7 Update Interrupt Disable */
/* -------- PWM_IMR2 : (PWM Offset: 0x3C) PWM Interrupt Mask Register 2 -------- */
#define PWM_IMR2_WRDY (0x1u << 0) //!< (PWM_IMR2) Write Ready for Synchronous Channels Update Interrupt Mask */
#define PWM_IMR2_ENDTX (0x1u << 1) //!< (PWM_IMR2) PDC End of TX Buffer Interrupt Mask */
#define PWM_IMR2_TXBUFE (0x1u << 2) //!< (PWM_IMR2) PDC TX Buffer Empty Interrupt Mask */
#define PWM_IMR2_UNRE (0x1u << 3) //!< (PWM_IMR2) Synchronous Channels Update Underrun Error Interrupt Mask */
#define PWM_IMR2_CMPM0 (0x1u << 8) //!< (PWM_IMR2) Comparison 0 Match Interrupt Mask */
#define PWM_IMR2_CMPM1 (0x1u << 9) //!< (PWM_IMR2) Comparison 1 Match Interrupt Mask */
#define PWM_IMR2_CMPM2 (0x1u << 10) //!< (PWM_IMR2) Comparison 2 Match Interrupt Mask */
#define PWM_IMR2_CMPM3 (0x1u << 11) //!< (PWM_IMR2) Comparison 3 Match Interrupt Mask */
#define PWM_IMR2_CMPM4 (0x1u << 12) //!< (PWM_IMR2) Comparison 4 Match Interrupt Mask */
#define PWM_IMR2_CMPM5 (0x1u << 13) //!< (PWM_IMR2) Comparison 5 Match Interrupt Mask */
#define PWM_IMR2_CMPM6 (0x1u << 14) //!< (PWM_IMR2) Comparison 6 Match Interrupt Mask */
#define PWM_IMR2_CMPM7 (0x1u << 15) //!< (PWM_IMR2) Comparison 7 Match Interrupt Mask */
#define PWM_IMR2_CMPU0 (0x1u << 16) //!< (PWM_IMR2) Comparison 0 Update Interrupt Mask */
#define PWM_IMR2_CMPU1 (0x1u << 17) //!< (PWM_IMR2) Comparison 1 Update Interrupt Mask */
#define PWM_IMR2_CMPU2 (0x1u << 18) //!< (PWM_IMR2) Comparison 2 Update Interrupt Mask */
#define PWM_IMR2_CMPU3 (0x1u << 19) //!< (PWM_IMR2) Comparison 3 Update Interrupt Mask */
#define PWM_IMR2_CMPU4 (0x1u << 20) //!< (PWM_IMR2) Comparison 4 Update Interrupt Mask */
#define PWM_IMR2_CMPU5 (0x1u << 21) //!< (PWM_IMR2) Comparison 5 Update Interrupt Mask */
#define PWM_IMR2_CMPU6 (0x1u << 22) //!< (PWM_IMR2) Comparison 6 Update Interrupt Mask */
#define PWM_IMR2_CMPU7 (0x1u << 23) //!< (PWM_IMR2) Comparison 7 Update Interrupt Mask */
/* -------- PWM_ISR2 : (PWM Offset: 0x40) PWM Interrupt Status Register 2 -------- */
#define PWM_ISR2_WRDY (0x1u << 0) //!< (PWM_ISR2) Write Ready for Synchronous Channels Update */
#define PWM_ISR2_ENDTX (0x1u << 1) //!< (PWM_ISR2) PDC End of TX Buffer */
#define PWM_ISR2_TXBUFE (0x1u << 2) //!< (PWM_ISR2) PDC TX Buffer Empty */
#define PWM_ISR2_UNRE (0x1u << 3) //!< (PWM_ISR2) Synchronous Channels Update Underrun Error */
#define PWM_ISR2_CMPM0 (0x1u << 8) //!< (PWM_ISR2) Comparison 0 Match */
#define PWM_ISR2_CMPM1 (0x1u << 9) //!< (PWM_ISR2) Comparison 1 Match */
#define PWM_ISR2_CMPM2 (0x1u << 10) //!< (PWM_ISR2) Comparison 2 Match */
#define PWM_ISR2_CMPM3 (0x1u << 11) //!< (PWM_ISR2) Comparison 3 Match */
#define PWM_ISR2_CMPM4 (0x1u << 12) //!< (PWM_ISR2) Comparison 4 Match */
#define PWM_ISR2_CMPM5 (0x1u << 13) //!< (PWM_ISR2) Comparison 5 Match */
#define PWM_ISR2_CMPM6 (0x1u << 14) //!< (PWM_ISR2) Comparison 6 Match */
#define PWM_ISR2_CMPM7 (0x1u << 15) //!< (PWM_ISR2) Comparison 7 Match */
#define PWM_ISR2_CMPU0 (0x1u << 16) //!< (PWM_ISR2) Comparison 0 Update */
#define PWM_ISR2_CMPU1 (0x1u << 17) //!< (PWM_ISR2) Comparison 1 Update */
#define PWM_ISR2_CMPU2 (0x1u << 18) //!< (PWM_ISR2) Comparison 2 Update */
#define PWM_ISR2_CMPU3 (0x1u << 19) //!< (PWM_ISR2) Comparison 3 Update */
#define PWM_ISR2_CMPU4 (0x1u << 20) //!< (PWM_ISR2) Comparison 4 Update */
#define PWM_ISR2_CMPU5 (0x1u << 21) //!< (PWM_ISR2) Comparison 5 Update */
#define PWM_ISR2_CMPU6 (0x1u << 22) //!< (PWM_ISR2) Comparison 6 Update */
#define PWM_ISR2_CMPU7 (0x1u << 23) //!< (PWM_ISR2) Comparison 7 Update */
/* -------- PWM_OOV : (PWM Offset: 0x44) PWM Output Override Value Register -------- */
#define PWM_OOV_OOVH0 (0x1u << 0) //!< (PWM_OOV) Output Override Value for PWMH output of the channel 0 */
#define PWM_OOV_OOVH1 (0x1u << 1) //!< (PWM_OOV) Output Override Value for PWMH output of the channel 1 */
#define PWM_OOV_OOVH2 (0x1u << 2) //!< (PWM_OOV) Output Override Value for PWMH output of the channel 2 */
#define PWM_OOV_OOVH3 (0x1u << 3) //!< (PWM_OOV) Output Override Value for PWMH output of the channel 3 */
#define PWM_OOV_OOVL0 (0x1u << 16) //!< (PWM_OOV) Output Override Value for PWML output of the channel 0 */
#define PWM_OOV_OOVL1 (0x1u << 17) //!< (PWM_OOV) Output Override Value for PWML output of the channel 1 */
#define PWM_OOV_OOVL2 (0x1u << 18) //!< (PWM_OOV) Output Override Value for PWML output of the channel 2 */
#define PWM_OOV_OOVL3 (0x1u << 19) //!< (PWM_OOV) Output Override Value for PWML output of the channel 3 */
/* -------- PWM_OS : (PWM Offset: 0x48) PWM Output Selection Register -------- */
#define PWM_OS_OSH0 (0x1u << 0) //!< (PWM_OS) Output Selection for PWMH output of the channel 0 */
#define PWM_OS_OSH1 (0x1u << 1) //!< (PWM_OS) Output Selection for PWMH output of the channel 1 */
#define PWM_OS_OSH2 (0x1u << 2) //!< (PWM_OS) Output Selection for PWMH output of the channel 2 */
#define PWM_OS_OSH3 (0x1u << 3) //!< (PWM_OS) Output Selection for PWMH output of the channel 3 */
#define PWM_OS_OSL0 (0x1u << 16) //!< (PWM_OS) Output Selection for PWML output of the channel 0 */
#define PWM_OS_OSL1 (0x1u << 17) //!< (PWM_OS) Output Selection for PWML output of the channel 1 */
#define PWM_OS_OSL2 (0x1u << 18) //!< (PWM_OS) Output Selection for PWML output of the channel 2 */
#define PWM_OS_OSL3 (0x1u << 19) //!< (PWM_OS) Output Selection for PWML output of the channel 3 */
/* -------- PWM_OSS : (PWM Offset: 0x4C) PWM Output Selection Set Register -------- */
#define PWM_OSS_OSSH0 (0x1u << 0) //!< (PWM_OSS) Output Selection Set for PWMH output of the channel 0 */
#define PWM_OSS_OSSH1 (0x1u << 1) //!< (PWM_OSS) Output Selection Set for PWMH output of the channel 1 */
#define PWM_OSS_OSSH2 (0x1u << 2) //!< (PWM_OSS) Output Selection Set for PWMH output of the channel 2 */
#define PWM_OSS_OSSH3 (0x1u << 3) //!< (PWM_OSS) Output Selection Set for PWMH output of the channel 3 */
#define PWM_OSS_OSSL0 (0x1u << 16) //!< (PWM_OSS) Output Selection Set for PWML output of the channel 0 */
#define PWM_OSS_OSSL1 (0x1u << 17) //!< (PWM_OSS) Output Selection Set for PWML output of the channel 1 */
#define PWM_OSS_OSSL2 (0x1u << 18) //!< (PWM_OSS) Output Selection Set for PWML output of the channel 2 */
#define PWM_OSS_OSSL3 (0x1u << 19) //!< (PWM_OSS) Output Selection Set for PWML output of the channel 3 */
/* -------- PWM_OSC : (PWM Offset: 0x50) PWM Output Selection Clear Register -------- */
#define PWM_OSC_OSCH0 (0x1u << 0) //!< (PWM_OSC) Output Selection Clear for PWMH output of the channel 0 */
#define PWM_OSC_OSCH1 (0x1u << 1) //!< (PWM_OSC) Output Selection Clear for PWMH output of the channel 1 */
#define PWM_OSC_OSCH2 (0x1u << 2) //!< (PWM_OSC) Output Selection Clear for PWMH output of the channel 2 */
#define PWM_OSC_OSCH3 (0x1u << 3) //!< (PWM_OSC) Output Selection Clear for PWMH output of the channel 3 */
#define PWM_OSC_OSCL0 (0x1u << 16) //!< (PWM_OSC) Output Selection Clear for PWML output of the channel 0 */
#define PWM_OSC_OSCL1 (0x1u << 17) //!< (PWM_OSC) Output Selection Clear for PWML output of the channel 1 */
#define PWM_OSC_OSCL2 (0x1u << 18) //!< (PWM_OSC) Output Selection Clear for PWML output of the channel 2 */
#define PWM_OSC_OSCL3 (0x1u << 19) //!< (PWM_OSC) Output Selection Clear for PWML output of the channel 3 */
/* -------- PWM_OSSUPD : (PWM Offset: 0x54) PWM Output Selection Set Update Register -------- */
#define PWM_OSSUPD_OSSUPH0 (0x1u << 0) //!< (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 0 */
#define PWM_OSSUPD_OSSUPH1 (0x1u << 1) //!< (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 1 */
#define PWM_OSSUPD_OSSUPH2 (0x1u << 2) //!< (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 2 */
#define PWM_OSSUPD_OSSUPH3 (0x1u << 3) //!< (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 3 */
#define PWM_OSSUPD_OSSUPL0 (0x1u << 16) //!< (PWM_OSSUPD) Output Selection Set for PWML output of the channel 0 */
#define PWM_OSSUPD_OSSUPL1 (0x1u << 17) //!< (PWM_OSSUPD) Output Selection Set for PWML output of the channel 1 */
#define PWM_OSSUPD_OSSUPL2 (0x1u << 18) //!< (PWM_OSSUPD) Output Selection Set for PWML output of the channel 2 */
#define PWM_OSSUPD_OSSUPL3 (0x1u << 19) //!< (PWM_OSSUPD) Output Selection Set for PWML output of the channel 3 */
/* -------- PWM_OSCUPD : (PWM Offset: 0x58) PWM Output Selection Clear Update Register -------- */
#define PWM_OSCUPD_OSCUPH0 (0x1u << 0) //!< (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 0 */
#define PWM_OSCUPD_OSCUPH1 (0x1u << 1) //!< (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 1 */
#define PWM_OSCUPD_OSCUPH2 (0x1u << 2) //!< (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 2 */
#define PWM_OSCUPD_OSCUPH3 (0x1u << 3) //!< (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 3 */
#define PWM_OSCUPD_OSCUPL0 (0x1u << 16) //!< (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 0 */
#define PWM_OSCUPD_OSCUPL1 (0x1u << 17) //!< (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 1 */
#define PWM_OSCUPD_OSCUPL2 (0x1u << 18) //!< (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 2 */
#define PWM_OSCUPD_OSCUPL3 (0x1u << 19) //!< (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 3 */
/* -------- PWM_FMR : (PWM Offset: 0x5C) PWM Fault Mode Register -------- */
#define PWM_FMR_FPOL_Pos 0	//!< FPOL bit position
#define PWM_FMR_FPOL_Msk (0xffu << PWM_FMR_FPOL_Pos) //!< (PWM_FMR) Fault Polarity (fault input bit varies from 0 to 5) */
/// set macro
#define PWM_FMR_FPOL(value) ((PWM_FMR_FPOL_Msk & ((value) << PWM_FMR_FPOL_Pos)))
#define PWM_FMR_FMOD_Pos 8 //!< FMOD bit position
#define PWM_FMR_FMOD_Msk (0xffu << PWM_FMR_FMOD_Pos) //!< (PWM_FMR) Fault Activation Mode (fault input bit varies from 0 to 5) */
/// set macro
#define PWM_FMR_FMOD(value) ((PWM_FMR_FMOD_Msk & ((value) << PWM_FMR_FMOD_Pos)))
#define PWM_FMR_FFIL_Pos 16	//!< FFIL bit position
#define PWM_FMR_FFIL_Msk (0xffu << PWM_FMR_FFIL_Pos) //!< (PWM_FMR) Fault Filtering (fault input bit varies from 0 to 5) */
/// set macro
#define PWM_FMR_FFIL(value) ((PWM_FMR_FFIL_Msk & ((value) << PWM_FMR_FFIL_Pos)))
/* -------- PWM_FSR : (PWM Offset: 0x60) PWM Fault Status Register -------- */
#define PWM_FSR_FIV_Pos 0	//!< FIV bit position
#define PWM_FSR_FIV_Msk (0xffu << PWM_FSR_FIV_Pos) //!< (PWM_FSR) Fault Input Value (fault input bit varies from 0 to 5) */
#define PWM_FSR_FS_Pos 8	//!< FS bit position
#define PWM_FSR_FS_Msk (0xffu << PWM_FSR_FS_Pos) //!< (PWM_FSR) Fault Status (fault input bit varies from 0 to 5) */
/* -------- PWM_FCR : (PWM Offset: 0x64) PWM Fault Clear Register -------- */
#define PWM_FCR_FCLR_Pos 0	//!< FCLR bit position
#define PWM_FCR_FCLR_Msk (0xffu << PWM_FCR_FCLR_Pos) //!< (PWM_FCR) Fault Clear (fault input bit varies from 0 to 5) */
/// set macro
#define PWM_FCR_FCLR(value) ((PWM_FCR_FCLR_Msk & ((value) << PWM_FCR_FCLR_Pos)))
/* -------- PWM_FPV : (PWM Offset: 0x68) PWM Fault Protection Value Register -------- */
#define PWM_FPV_FPVH0 (0x1u << 0) //!< (PWM_FPV) Fault Protection Value for PWMH output on channel 0 */
#define PWM_FPV_FPVH1 (0x1u << 1) //!< (PWM_FPV) Fault Protection Value for PWMH output on channel 1 */
#define PWM_FPV_FPVH2 (0x1u << 2) //!< (PWM_FPV) Fault Protection Value for PWMH output on channel 2 */
#define PWM_FPV_FPVH3 (0x1u << 3) //!< (PWM_FPV) Fault Protection Value for PWMH output on channel 3 */
#define PWM_FPV_FPVL0 (0x1u << 16) //!< (PWM_FPV) Fault Protection Value for PWML output on channel 0 */
#define PWM_FPV_FPVL1 (0x1u << 17) //!< (PWM_FPV) Fault Protection Value for PWML output on channel 1 */
#define PWM_FPV_FPVL2 (0x1u << 18) //!< (PWM_FPV) Fault Protection Value for PWML output on channel 2 */
#define PWM_FPV_FPVL3 (0x1u << 19) //!< (PWM_FPV) Fault Protection Value for PWML output on channel 3 */
/* -------- PWM_FPE : (PWM Offset: 0x6C) PWM Fault Protection Enable Register -------- */
#define PWM_FPE_FPE0_Pos 0	//!< FPE0 bit position
#define PWM_FPE_FPE0_Msk (0xffu << PWM_FPE_FPE0_Pos) //!< (PWM_FPE) Fault Protection Enable for channel 0 (fault input bit varies from 0 to 5) */
/// set macro
#define PWM_FPE_FPE0(value) ((PWM_FPE_FPE0_Msk & ((value) << PWM_FPE_FPE0_Pos)))
#define PWM_FPE_FPE1_Pos 8	//!< FPE1 bit position
#define PWM_FPE_FPE1_Msk (0xffu << PWM_FPE_FPE1_Pos) //!< (PWM_FPE) Fault Protection Enable for channel 1 (fault input bit varies from 0 to 5) */
/// set macro
#define PWM_FPE_FPE1(value) ((PWM_FPE_FPE1_Msk & ((value) << PWM_FPE_FPE1_Pos)))
#define PWM_FPE_FPE2_Pos 16	//!< FPE2 bit position
#define PWM_FPE_FPE2_Msk (0xffu << PWM_FPE_FPE2_Pos) //!< (PWM_FPE) Fault Protection Enable for channel 2 (fault input bit varies from 0 to 5) */
/// set macro
#define PWM_FPE_FPE2(value) ((PWM_FPE_FPE2_Msk & ((value) << PWM_FPE_FPE2_Pos)))
#define PWM_FPE_FPE3_Pos 24	//!< FPE3 bit position
#define PWM_FPE_FPE3_Msk (0xffu << PWM_FPE_FPE3_Pos) //!< (PWM_FPE) Fault Protection Enable for channel 3 (fault input bit varies from 0 to 5) */
/// set macro
#define PWM_FPE_FPE3(value) ((PWM_FPE_FPE3_Msk & ((value) << PWM_FPE_FPE3_Pos)))
/* -------- PWM_EL0MR : (PWM Offset: 0x7C) PWM Event Line 0 Mode Register -------- */
#define PWM_EL0MR_CSEL0 (0x1u << 0) //!< (PWM_EL0MR) Comparison 0 Selection */
#define PWM_EL0MR_CSEL1 (0x1u << 1) //!< (PWM_EL0MR) Comparison 1 Selection */
#define PWM_EL0MR_CSEL2 (0x1u << 2) //!< (PWM_EL0MR) Comparison 2 Selection */
#define PWM_EL0MR_CSEL3 (0x1u << 3) //!< (PWM_EL0MR) Comparison 3 Selection */
#define PWM_EL0MR_CSEL4 (0x1u << 4) //!< (PWM_EL0MR) Comparison 4 Selection */
#define PWM_EL0MR_CSEL5 (0x1u << 5) //!< (PWM_EL0MR) Comparison 5 Selection */
#define PWM_EL0MR_CSEL6 (0x1u << 6) //!< (PWM_EL0MR) Comparison 6 Selection */
#define PWM_EL0MR_CSEL7 (0x1u << 7) //!< (PWM_EL0MR) Comparison 7 Selection */
/* -------- PWM_EL1MR : (PWM Offset: 0x80) PWM Event Line 1 Mode Register -------- */
#define PWM_EL1MR_CSEL0 (0x1u << 0) //!< (PWM_EL1MR) Comparison 0 Selection */
#define PWM_EL1MR_CSEL1 (0x1u << 1) //!< (PWM_EL1MR) Comparison 1 Selection */
#define PWM_EL1MR_CSEL2 (0x1u << 2) //!< (PWM_EL1MR) Comparison 2 Selection */
#define PWM_EL1MR_CSEL3 (0x1u << 3) //!< (PWM_EL1MR) Comparison 3 Selection */
#define PWM_EL1MR_CSEL4 (0x1u << 4) //!< (PWM_EL1MR) Comparison 4 Selection */
#define PWM_EL1MR_CSEL5 (0x1u << 5) //!< (PWM_EL1MR) Comparison 5 Selection */
#define PWM_EL1MR_CSEL6 (0x1u << 6) //!< (PWM_EL1MR) Comparison 6 Selection */
#define PWM_EL1MR_CSEL7 (0x1u << 7) //!< (PWM_EL1MR) Comparison 7 Selection */
/* -------- PWM_SMMR : (PWM Offset: 0xB0) PWM Stepper Motor Mode Register -------- */
#define PWM_SMMR_GCEN0 (0x1u << 0) //!< (PWM_SMMR) Gray Count ENable */
#define PWM_SMMR_GCEN1 (0x1u << 1) //!< (PWM_SMMR) Gray Count ENable */
#define PWM_SMMR_DOWN0 (0x1u << 16) //!< (PWM_SMMR) DOWN Count */
#define PWM_SMMR_DOWN1 (0x1u << 17) //!< (PWM_SMMR) DOWN Count */
/* -------- PWM_WPCR : (PWM Offset: 0xE4) PWM Write Protect Control Register -------- */
#define PWM_WPCR_WPCMD_Pos 0	//!< WPCMD bit position
#define PWM_WPCR_WPCMD_Msk (0x3u << PWM_WPCR_WPCMD_Pos) //!< (PWM_WPCR) Write Protect Command */
/// set macro
#define PWM_WPCR_WPCMD(value) ((PWM_WPCR_WPCMD_Msk & ((value) << PWM_WPCR_WPCMD_Pos)))
#define PWM_WPCR_WPRG0 (0x1u << 2) //!< (PWM_WPCR) Write Protect Register Group 0 */
#define PWM_WPCR_WPRG1 (0x1u << 3) //!< (PWM_WPCR) Write Protect Register Group 1 */
#define PWM_WPCR_WPRG2 (0x1u << 4) //!< (PWM_WPCR) Write Protect Register Group 2 */
#define PWM_WPCR_WPRG3 (0x1u << 5) //!< (PWM_WPCR) Write Protect Register Group 3 */
#define PWM_WPCR_WPRG4 (0x1u << 6) //!< (PWM_WPCR) Write Protect Register Group 4 */
#define PWM_WPCR_WPRG5 (0x1u << 7) //!< (PWM_WPCR) Write Protect Register Group 5 */
#define PWM_WPCR_WPKEY_Pos 8	//!< WPKEY bit position
#define PWM_WPCR_WPKEY_Msk (0xffffffu << PWM_WPCR_WPKEY_Pos) //!< (PWM_WPCR) Write Protect Key */
/// set macro
#define PWM_WPCR_WPKEY(value) ((PWM_WPCR_WPKEY_Msk & ((value) << PWM_WPCR_WPKEY_Pos)))
/* -------- PWM_WPSR : (PWM Offset: 0xE8) PWM Write Protect Status Register -------- */
#define PWM_WPSR_WPSWS0 (0x1u << 0) //!< (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS1 (0x1u << 1) //!< (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS2 (0x1u << 2) //!< (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS3 (0x1u << 3) //!< (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS4 (0x1u << 4) //!< (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS5 (0x1u << 5) //!< (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPVS (0x1u << 7) //!< (PWM_WPSR) Write Protect Violation Status */
#define PWM_WPSR_WPHWS0 (0x1u << 8) //!< (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS1 (0x1u << 9) //!< (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS2 (0x1u << 10) //!< (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS3 (0x1u << 11) //!< (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS4 (0x1u << 12) //!< (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS5 (0x1u << 13) //!< (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPVSRC_Pos 16 	//!< WPVSRC bit position
#define PWM_WPSR_WPVSRC_Msk (0xffffu << PWM_WPSR_WPVSRC_Pos) //!< (PWM_WPSR) Write Protect Violation Source */
/* -------- PWM_RPR : (PWM Offset: 0x100) Receive Pointer Register -------- */
#define PWM_RPR_RXPTR_Pos 0	//!< RXPTR bit position
#define PWM_RPR_RXPTR_Msk (0xffffffffu << PWM_RPR_RXPTR_Pos) //!< (PWM_RPR) Receive Pointer Register */
/// set macro
#define PWM_RPR_RXPTR(value) ((PWM_RPR_RXPTR_Msk & ((value) << PWM_RPR_RXPTR_Pos)))
/* -------- PWM_RCR : (PWM Offset: 0x104) Receive Counter Register -------- */
#define PWM_RCR_RXCTR_Pos 0	//!< RXCTR bit position
#define PWM_RCR_RXCTR_Msk (0xffffu << PWM_RCR_RXCTR_Pos) //!< (PWM_RCR) Receive Counter Register */
/// set macro
#define PWM_RCR_RXCTR(value) ((PWM_RCR_RXCTR_Msk & ((value) << PWM_RCR_RXCTR_Pos)))
/* -------- PWM_TPR : (PWM Offset: 0x108) Transmit Pointer Register -------- */
#define PWM_TPR_TXPTR_Pos 0	//!< TXPTR bit position
#define PWM_TPR_TXPTR_Msk (0xffffffffu << PWM_TPR_TXPTR_Pos) //!< (PWM_TPR) Transmit Counter Register */
/// set macro
#define PWM_TPR_TXPTR(value) ((PWM_TPR_TXPTR_Msk & ((value) << PWM_TPR_TXPTR_Pos)))
/* -------- PWM_TCR : (PWM Offset: 0x10C) Transmit Counter Register -------- */
#define PWM_TCR_TXCTR_Pos 0	//!< TXCTR bit position
#define PWM_TCR_TXCTR_Msk (0xffffu << PWM_TCR_TXCTR_Pos) //!< (PWM_TCR) Transmit Counter Register */
/// set macro
#define PWM_TCR_TXCTR(value) ((PWM_TCR_TXCTR_Msk & ((value) << PWM_TCR_TXCTR_Pos)))
/* -------- PWM_RNPR : (PWM Offset: 0x110) Receive Next Pointer Register -------- */
#define PWM_RNPR_RXNPTR_Pos 0	//!< RXNPTR bit position
#define PWM_RNPR_RXNPTR_Msk (0xffffffffu << PWM_RNPR_RXNPTR_Pos) //!< (PWM_RNPR) Receive Next Pointer */
/// set macro
#define PWM_RNPR_RXNPTR(value) ((PWM_RNPR_RXNPTR_Msk & ((value) << PWM_RNPR_RXNPTR_Pos)))
/* -------- PWM_RNCR : (PWM Offset: 0x114) Receive Next Counter Register -------- */
#define PWM_RNCR_RXNCTR_Pos 0	//!< RXNCTR bit position
#define PWM_RNCR_RXNCTR_Msk (0xffffu << PWM_RNCR_RXNCTR_Pos) //!< (PWM_RNCR) Receive Next Counter */
/// set macro
#define PWM_RNCR_RXNCTR(value) ((PWM_RNCR_RXNCTR_Msk & ((value) << PWM_RNCR_RXNCTR_Pos)))
/* -------- PWM_TNPR : (PWM Offset: 0x118) Transmit Next Pointer Register -------- */
#define PWM_TNPR_TXNPTR_Pos 0	//!< TXNPTR bit position
#define PWM_TNPR_TXNPTR_Msk (0xffffffffu << PWM_TNPR_TXNPTR_Pos) //!< (PWM_TNPR) Transmit Next Pointer */
/// set macro
#define PWM_TNPR_TXNPTR(value) ((PWM_TNPR_TXNPTR_Msk & ((value) << PWM_TNPR_TXNPTR_Pos)))
/* -------- PWM_TNCR : (PWM Offset: 0x11C) Transmit Next Counter Register -------- */
#define PWM_TNCR_TXNCTR_Pos 0	//!< TXNCTR bit position
#define PWM_TNCR_TXNCTR_Msk (0xffffu << PWM_TNCR_TXNCTR_Pos) //!< (PWM_TNCR) Transmit Counter Next */
/// set macro
#define PWM_TNCR_TXNCTR(value) ((PWM_TNCR_TXNCTR_Msk & ((value) << PWM_TNCR_TXNCTR_Pos)))
/* -------- PWM_PTCR : (PWM Offset: 0x120) Transfer Control Register -------- */
#define PWM_PTCR_RXTEN (0x1u << 0) //!< (PWM_PTCR) Receiver Transfer Enable */
#define PWM_PTCR_RXTDIS (0x1u << 1) //!< (PWM_PTCR) Receiver Transfer Disable */
#define PWM_PTCR_TXTEN (0x1u << 8) //!< (PWM_PTCR) Transmitter Transfer Enable */
#define PWM_PTCR_TXTDIS (0x1u << 9) //!< (PWM_PTCR) Transmitter Transfer Disable */
/* -------- PWM_PTSR : (PWM Offset: 0x124) Transfer Status Register -------- */
#define PWM_PTSR_RXTEN (0x1u << 0) //!< (PWM_PTSR) Receiver Transfer Enable */
#define PWM_PTSR_TXTEN (0x1u << 8) //!< (PWM_PTSR) Transmitter Transfer Enable */
/* -------- PWM_CMPxV : (PWM Offset: N/A) PWM Comparison 0 Value Register -------- */
#define PWM_CMPxV_CV_Pos 0	//!< CMPxV_CV bit position
#define PWM_CMPxV_CV_Msk (0xffffffu << PWM_CMPxV_CV_Pos) //!< (PWM_CMPxV) Comparison x Value */
/// set macro
#define PWM_CMPxV_CV(value) ((PWM_CMPxV_CV_Msk & ((value) << PWM_CMPxV_CV_Pos)))
#define PWM_CMPxV_CVM (0x1u << 24) //!< (PWM_CMPxV) Comparison x Value Mode */
/* -------- PWM_CMPxVUPD : (PWM Offset: N/A) PWM Comparison 0 Value Update Register -------- */
/// set macro
#define PWM_CMPxVUPD_CVUPD_Pos 0
#define PWM_CMPxVUPD_CVUPD_Msk (0xffffffu << PWM_CMPxVUPD_CVUPD_Pos) //!< (PWM_CMPxVUPD) Comparison x Value Update */
/// set macro
#define PWM_CMPxVUPD_CVUPD(value) ((PWM_CMPxVUPD_CVUPD_Msk & ((value) << PWM_CMPxVUPD_CVUPD_Pos)))
#define PWM_CMPxVUPD_CVMUPD (0x1u << 24) //!< (PWM_CMPxVUPD) Comparison x Value Mode Update */
/* -------- PWM_CMPxM : (PWM Offset: N/A) PWM Comparison 0 Mode Register -------- */
#define PWM_CMPxM_CEN (0x1u << 0) //!< (PWM_CMPxM) Comparison x Enable */
#define PWM_CMPxM_CTR_Pos 4	//!< CTR bit position
#define PWM_CMPxM_CTR_Msk (0xfu << PWM_CMPxM_CTR_Pos) //!< (PWM_CMPxM) Comparison x Trigger */
/// set macro
#define PWM_CMPxM_CTR(value) ((PWM_CMPxM_CTR_Msk & ((value) << PWM_CMPxM_CTR_Pos)))
#define PWM_CMPxM_CPR_Pos 8	//!< CPR bit position
#define PWM_CMPxM_CPR_Msk (0xfu << PWM_CMPxM_CPR_Pos) //!< (PWM_CMPxM) Comparison x Period */
/// set macro
#define PWM_CMPxM_CPR(value) ((PWM_CMPxM_CPR_Msk & ((value) << PWM_CMPxM_CPR_Pos)))
#define PWM_CMPxM_CPRCNT_Pos 12	//!< CPRCNT bit position
#define PWM_CMPxM_CPRCNT_Msk (0xfu << PWM_CMPxM_CPRCNT_Pos) //!< (PWM_CMPxM) Comparison x Period Counter */
/// set macro
#define PWM_CMPxM_CPRCNT(value) ((PWM_CMPxM_CPRCNT_Msk & ((value) << PWM_CMPxM_CPRCNT_Pos)))
#define PWM_CMPxM_CUPR_Pos 16	//!< CUPR bit position
#define PWM_CMPxM_CUPR_Msk (0xfu << PWM_CMPxM_CUPR_Pos) //!< (PWM_CMPxM) Comparison x Update Period */
/// set macro
#define PWM_CMPxM_CUPR(value) ((PWM_CMPxM_CUPR_Msk & ((value) << PWM_CMPxM_CUPR_Pos)))
#define PWM_CMPxM_CUPRCNT_Pos 20	//!< CUPRCNT bit position
#define PWM_CMPxM_CUPRCNT_Msk (0xfu << PWM_CMPxM_CUPRCNT_Pos) //!< (PWM_CMPxM) Comparison x Update Period Counter */
/// set macro
#define PWM_CMPxM_CUPRCNT(value) ((PWM_CMPxM_CUPRCNT_Msk & ((value) << PWM_CMPxM_CUPRCNT_Pos)))
/* -------- PWM_CMPxMUPD : (PWM Offset: N/A) PWM Comparison 0 Mode Update Register -------- */
#define PWM_CMPxMUPD_CENUPD (0x1u << 0) //!< (PWM_CMPxMUPD) Comparison x Enable Update */
#define PWM_CMPxMUPD_CTRUPD_Pos 4	//!< CTRUPD bit position
#define PWM_CMPxMUPD_CTRUPD_Msk (0xfu << PWM_CMPxMUPD_CTRUPD_Pos) //!< (PWM_CMPxMUPD) Comparison x Trigger Update */
/// set macro
#define PWM_CMPxMUPD_CTRUPD(value) ((PWM_CMPxMUPD_CTRUPD_Msk & ((value) << PWM_CMPxMUPD_CTRUPD_Pos)))
#define PWM_CMPxMUPD_CPRUPD_Pos 8	//!< CPRUPD bit position
#define PWM_CMPxMUPD_CPRUPD_Msk (0xfu << PWM_CMPxMUPD_CPRUPD_Pos) //!< (PWM_CMPxMUPD) Comparison x Period Update */
/// set macro
#define PWM_CMPxMUPD_CPRUPD(value) ((PWM_CMPxMUPD_CPRUPD_Msk & ((value) << PWM_CMPxMUPD_CPRUPD_Pos)))
#define PWM_CMPxMUPD_CUPRUPD_Pos 16	//!< CUPRUPD bit position
#define PWM_CMPxMUPD_CUPRUPD_Msk (0xfu << PWM_CMPxMUPD_CUPRUPD_Pos) //!< (PWM_CMPxMUPD) Comparison x Update Period Update */
/// set macro
#define PWM_CMPxMUPD_CUPRUPD(value) ((PWM_CMPxMUPD_CUPRUPD_Msk & ((value) << PWM_CMPxMUPD_CUPRUPD_Pos)))
/* -------- PWM_CMR : (PWM Offset: N/A) PWM Channel Mode Register -------- */
#define PWM_CMR_CPRE_Pos 0	//!< CPRE bit position
#define PWM_CMR_CPRE_Msk (0xfu << PWM_CMR_CPRE_Pos) //!< (PWM_CMR) Channel Pre-scaler */
#define   PWM_CMR_CPRE_MCK (0x0u << 0) //!< (PWM_CMR) MCK */
#define   PWM_CMR_CPRE_MCK_DIV_2 (0x1u << 0) //!< (PWM_CMR) MCK/2 */
#define   PWM_CMR_CPRE_MCK_DIV_4 (0x2u << 0) //!< (PWM_CMR) MCK/4 */
#define   PWM_CMR_CPRE_MCK_DIV_8 (0x3u << 0) //!< (PWM_CMR) MCK/8 */
#define   PWM_CMR_CPRE_MCK_DIV_16 (0x4u << 0) //!< (PWM_CMR) MCK/16 */
#define   PWM_CMR_CPRE_MCK_DIV_32 (0x5u << 0) //!< (PWM_CMR) MCK/32 */
#define   PWM_CMR_CPRE_MCK_DIV_64 (0x6u << 0) //!< (PWM_CMR) MCK/64 */
#define   PWM_CMR_CPRE_MCK_DIV_128 (0x7u << 0) //!< (PWM_CMR) MCK/128 */
#define   PWM_CMR_CPRE_MCK_DIV_256 (0x8u << 0) //!< (PWM_CMR) MCK/256 */
#define   PWM_CMR_CPRE_MCK_DIV_512 (0x9u << 0) //!< (PWM_CMR) MCK/512 */
#define   PWM_CMR_CPRE_MCK_DIV_1024 (0xAu << 0) //!< (PWM_CMR) MCK/1024 */
#define   PWM_CMR_CPRE_CKA (0xBu << 0) //!< (PWM_CMR) CLKA */
#define   PWM_CMR_CPRE_CKB (0xCu << 0) //!< (PWM_CMR) CLKB */
#define PWM_CMR_CALG (0x1u << 8) //!< (PWM_CMR) Channel Alignment */
#define PWM_CMR_CPOL (0x1u << 9) //!< (PWM_CMR) Channel Polarity */
#define PWM_CMR_CES (0x1u << 10) //!< (PWM_CMR) Counter Event Selection */
#define PWM_CMR_DTE (0x1u << 16) //!< (PWM_CMR) Dead-Time Generator Enable */
#define PWM_CMR_DTHI (0x1u << 17) //!< (PWM_CMR) Dead-Time PWMHx Output Inverted */
#define PWM_CMR_DTLI (0x1u << 18) //!< (PWM_CMR) Dead-Time PWMLx Output Inverted */
/* -------- PWM_CDTY : (PWM Offset: N/A) PWM Channel Duty Cycle Register -------- */
#define PWM_CDTY_CDTY_Pos 0	//!< CDTY bit position
#define PWM_CDTY_CDTY_Msk (0xffffffu << PWM_CDTY_CDTY_Pos) //!< (PWM_CDTY) Channel Duty-Cycle */
/// set macro
#define PWM_CDTY_CDTY(value) ((PWM_CDTY_CDTY_Msk & ((value) << PWM_CDTY_CDTY_Pos)))
/* -------- PWM_CDTYUPD : (PWM Offset: N/A) PWM Channel Duty Cycle Update Register -------- */
#define PWM_CDTYUPD_CDTYUPD_Pos 0	//!< CDTYUPD bit position
#define PWM_CDTYUPD_CDTYUPD_Msk (0xffffffu << PWM_CDTYUPD_CDTYUPD_Pos) //!< (PWM_CDTYUPD) Channel Duty-Cycle Update */
/// set macro
#define PWM_CDTYUPD_CDTYUPD(value) ((PWM_CDTYUPD_CDTYUPD_Msk & ((value) << PWM_CDTYUPD_CDTYUPD_Pos)))
/* -------- PWM_CPRD : (PWM Offset: N/A) PWM Channel Period Register -------- */
#define PWM_CPRD_CPRD_Pos 0	//!< CPRD bit position
#define PWM_CPRD_CPRD_Msk (0xffffffu << PWM_CPRD_CPRD_Pos) //!< (PWM_CPRD) Channel Period */
/// set macro
#define PWM_CPRD_CPRD(value) ((PWM_CPRD_CPRD_Msk & ((value) << PWM_CPRD_CPRD_Pos)))
/* -------- PWM_CPRDUPD : (PWM Offset: N/A) PWM Channel Period Update Register -------- */
#define PWM_CPRDUPD_CPRDUPD_Pos 0	//!< CPRDUPD bit position
#define PWM_CPRDUPD_CPRDUPD_Msk (0xffffffu << PWM_CPRDUPD_CPRDUPD_Pos) //!< (PWM_CPRDUPD) Channel Period Update */
/// set macro
#define PWM_CPRDUPD_CPRDUPD(value) ((PWM_CPRDUPD_CPRDUPD_Msk & ((value) << PWM_CPRDUPD_CPRDUPD_Pos)))
/* -------- PWM_CCNT : (PWM Offset: N/A) PWM Channel Counter Register -------- */
#define PWM_CCNT_CNT_Pos 0	//!< CNT bit position
#define PWM_CCNT_CNT_Msk (0xffffffu << PWM_CCNT_CNT_Pos) //!< (PWM_CCNT) Channel Counter Register */
/* -------- PWM_DT : (PWM Offset: N/A) PWM Channel Dead Time Register -------- */
#define PWM_DT_DTH_Pos 0	//!< DTH bit position
#define PWM_DT_DTH_Msk (0xffffu << PWM_DT_DTH_Pos) //!< (PWM_DT) Dead-Time Value for PWMHx Output */
/// set macro
#define PWM_DT_DTH(value) ((PWM_DT_DTH_Msk & ((value) << PWM_DT_DTH_Pos)))
#define PWM_DT_DTL_Pos 16	//!< DTL bit position
#define PWM_DT_DTL_Msk (0xffffu << PWM_DT_DTL_Pos) //!< (PWM_DT) Dead-Time Value for PWMLx Output */
/// set macro
#define PWM_DT_DTL(value) ((PWM_DT_DTL_Msk & ((value) << PWM_DT_DTL_Pos)))
/* -------- PWM_DTUPD : (PWM Offset: N/A) PWM Channel Dead Time Update Register -------- */
#define PWM_DTUPD_DTHUPD_Pos 0	//!< DTHUPD bit position
#define PWM_DTUPD_DTHUPD_Msk (0xffffu << PWM_DTUPD_DTHUPD_Pos) //!< (PWM_DTUPD) Dead-Time Value Update for PWMHx Output */
/// set macro
#define PWM_DTUPD_DTHUPD(value) ((PWM_DTUPD_DTHUPD_Msk & ((value) << PWM_DTUPD_DTHUPD_Pos)))
#define PWM_DTUPD_DTLUPD_Pos 16	//!< DTLUPD bit position
#define PWM_DTUPD_DTLUPD_Msk (0xffffu << PWM_DTUPD_DTLUPD_Pos) //!< (PWM_DTUPD) Dead-Time Value Update for PWMLx Output */
/// set macro
#define PWM_DTUPD_DTLUPD(value) ((PWM_DTUPD_DTLUPD_Msk & ((value) << PWM_DTUPD_DTLUPD_Pos)))


#ifdef __cplusplus
}
#endif

 /* @} addtogroup SAM3S_PWM */

#endif /* PWMC_H_ */
