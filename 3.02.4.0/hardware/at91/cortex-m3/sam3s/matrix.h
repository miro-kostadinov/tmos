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

#ifndef MATRIX_H_
#define MATRIX_H_

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/
#include <fam_inc.h>
#include <at91_types.h>


#ifdef __cplusplus
 extern "C" {
#endif


/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR AHB Bus Matrix */
/* ============================================================================= */
/** \addtogroup SAM3S_MATRIX AHB Bus Matrix */
/*@{*/

#ifndef __ASSEMBLY__
/** \brief Matrix hardware registers */
typedef struct {
  RwReg MATRIX_MCFG[4]; //!< (Matrix Offset: 0x0000) Master Configuration Register */
  RwReg Reserved1[12];	//!< reserved
  RwReg MATRIX_SCFG[5]; //!< (Matrix Offset: 0x0040) Slave Configuration Register */
  RwReg Reserved2[11];	//!< reserved
  RwReg MATRIX_PRAS0;   //!< (Matrix Offset: 0x0080) Priority Register A for Slave 0 */
  RwReg Reserved3[1];	//!< reserved
  RwReg MATRIX_PRAS1;   //!< (Matrix Offset: 0x0088) Priority Register A for Slave 1 */
  RwReg Reserved4[1];	//!< reserved
  RwReg MATRIX_PRAS2;   //!< (Matrix Offset: 0x0090) Priority Register A for Slave 2 */
  RwReg Reserved5[1];	//!< reserved
  RwReg MATRIX_PRAS3;   //!< (Matrix Offset: 0x0098) Priority Register A for Slave 3 */
  RwReg Reserved6[1];	//!< reserved
  RwReg MATRIX_PRAS4;   //!< (Matrix Offset: 0x00A0) Priority Register A for Slave 4 */
  RwReg Reserved7[1];	//!< reserved
  RwReg Reserved8[27];	//!< reserved
  RwReg CCFG_SYSIO;     //!< (Matrix Offset: 0x0114) System I/O Configuration register */
  RwReg Reserved9[1];	//!< reserved
  RwReg CCFG_SMCNFCS;   //!< (Matrix Offset: 0x011C) SMC Chip Select NAND Flash Assignment Register */
  RwReg Reserved10[49];	//!< reserved
  RwReg MATRIX_WPMR;    //!< (Matrix Offset: 0x1E4) Write Protect Mode Register */
  RoReg MATRIX_WPSR;    //!< (Matrix Offset: 0x1E8) Write Protect Status Register */
} Matrix;
#endif /* __ASSEMBLY__ */
/* -------- MATRIX_MCFG[4] : (MATRIX Offset: 0x0000) Master Configuration Register -------- */
#define MATRIX_MCFG_ULBT_Pos 0	//!< ULBT bit position
#define MATRIX_MCFG_ULBT_Msk (0x7u << MATRIX_MCFG_ULBT_Pos) //!< (MATRIX_MCFG[4]) Undefined Length Burst Type */
/// set macro
#define MATRIX_MCFG_ULBT(value) ((MATRIX_MCFG_ULBT_Msk & ((value) << MATRIX_MCFG_ULBT_Pos)))
/* -------- MATRIX_SCFG[5] : (MATRIX Offset: 0x0040) Slave Configuration Register -------- */
#define MATRIX_SCFG_SLOT_CYCLE_Pos 0	//!< SLOT_CYCLE bit position
#define MATRIX_SCFG_SLOT_CYCLE_Msk (0xffu << MATRIX_SCFG_SLOT_CYCLE_Pos) //!< (MATRIX_SCFG[5]) Maximum Number of Allowed Cycles for a Burst */
/// set macro
#define MATRIX_SCFG_SLOT_CYCLE(value) ((MATRIX_SCFG_SLOT_CYCLE_Msk & ((value) << MATRIX_SCFG_SLOT_CYCLE_Pos)))
#define MATRIX_SCFG_DEFMSTR_TYPE_Pos 16	//!< DEFMSTR_TYPE bit position
#define MATRIX_SCFG_DEFMSTR_TYPE_Msk (0x3u << MATRIX_SCFG_DEFMSTR_TYPE_Pos) //!< (MATRIX_SCFG[5]) Default Master Type */
/// set macro
#define MATRIX_SCFG_DEFMSTR_TYPE(value) ((MATRIX_SCFG_DEFMSTR_TYPE_Msk & ((value) << MATRIX_SCFG_DEFMSTR_TYPE_Pos)))
#define MATRIX_SCFG_FIXED_DEFMSTR_Pos 18	//!< FIXED_DEFMSTR bit position
#define MATRIX_SCFG_FIXED_DEFMSTR_Msk (0x7u << MATRIX_SCFG_FIXED_DEFMSTR_Pos) //!< (MATRIX_SCFG[5]) Fixed Default Master */
/// set macro
#define MATRIX_SCFG_FIXED_DEFMSTR(value) ((MATRIX_SCFG_FIXED_DEFMSTR_Msk & ((value) << MATRIX_SCFG_FIXED_DEFMSTR_Pos)))
#define MATRIX_SCFG_ARBT_Pos 24	//!< ARBT bit position
#define MATRIX_SCFG_ARBT_Msk (0x3u << MATRIX_SCFG_ARBT_Pos) //!< (MATRIX_SCFG[5]) Arbitration Type */
/// set macro
#define MATRIX_SCFG_ARBT(value) ((MATRIX_SCFG_ARBT_Msk & ((value) << MATRIX_SCFG_ARBT_Pos)))
/* -------- MATRIX_PRAS0 : (MATRIX Offset: 0x0080) Priority Register A for Slave 0 -------- */
#define MATRIX_PRAS0_M0PR_Pos 0	//!< M0PR bit position
#define MATRIX_PRAS0_M0PR_Msk (0x3u << MATRIX_PRAS0_M0PR_Pos) //!< (MATRIX_PRAS0) Master 0 Priority */
/// set macro
#define MATRIX_PRAS0_M0PR(value) ((MATRIX_PRAS0_M0PR_Msk & ((value) << MATRIX_PRAS0_M0PR_Pos)))
#define MATRIX_PRAS0_M1PR_Pos 4	//!< M1PR bit position
#define MATRIX_PRAS0_M1PR_Msk (0x3u << MATRIX_PRAS0_M1PR_Pos) //!< (MATRIX_PRAS0) Master 1 Priority */
/// set macro
#define MATRIX_PRAS0_M1PR(value) ((MATRIX_PRAS0_M1PR_Msk & ((value) << MATRIX_PRAS0_M1PR_Pos)))
#define MATRIX_PRAS0_M2PR_Pos 8	//!< M2PR bit position
#define MATRIX_PRAS0_M2PR_Msk (0x3u << MATRIX_PRAS0_M2PR_Pos) //!< (MATRIX_PRAS0) Master 2 Priority */
/// set macro
#define MATRIX_PRAS0_M2PR(value) ((MATRIX_PRAS0_M2PR_Msk & ((value) << MATRIX_PRAS0_M2PR_Pos)))
#define MATRIX_PRAS0_M3PR_Pos 12	//!< M3PR bit position
#define MATRIX_PRAS0_M3PR_Msk (0x3u << MATRIX_PRAS0_M3PR_Pos) //!< (MATRIX_PRAS0) Master 3 Priority */
/// set macro
#define MATRIX_PRAS0_M3PR(value) ((MATRIX_PRAS0_M3PR_Msk & ((value) << MATRIX_PRAS0_M3PR_Pos)))
#define MATRIX_PRAS0_M4PR_Pos 16	//!< M4PR bit position
#define MATRIX_PRAS0_M4PR_Msk (0x3u << MATRIX_PRAS0_M4PR_Pos) //!< (MATRIX_PRAS0) Master 4 Priority */
/// set macro
#define MATRIX_PRAS0_M4PR(value) ((MATRIX_PRAS0_M4PR_Msk & ((value) << MATRIX_PRAS0_M4PR_Pos)))
/* -------- MATRIX_PRAS1 : (MATRIX Offset: 0x0088) Priority Register A for Slave 1 -------- */
#define MATRIX_PRAS1_M0PR_Pos 0	//!< M0PR bit position
#define MATRIX_PRAS1_M0PR_Msk (0x3u << MATRIX_PRAS1_M0PR_Pos) //!< (MATRIX_PRAS1) Master 0 Priority */
/// set macro
#define MATRIX_PRAS1_M0PR(value) ((MATRIX_PRAS1_M0PR_Msk & ((value) << MATRIX_PRAS1_M0PR_Pos)))
#define MATRIX_PRAS1_M1PR_Pos 4	//!< M1PR bit position
#define MATRIX_PRAS1_M1PR_Msk (0x3u << MATRIX_PRAS1_M1PR_Pos) //!< (MATRIX_PRAS1) Master 1 Priority */
/// set macro
#define MATRIX_PRAS1_M1PR(value) ((MATRIX_PRAS1_M1PR_Msk & ((value) << MATRIX_PRAS1_M1PR_Pos)))
#define MATRIX_PRAS1_M2PR_Pos 8	//!< M2PR bit position
#define MATRIX_PRAS1_M2PR_Msk (0x3u << MATRIX_PRAS1_M2PR_Pos) //!< (MATRIX_PRAS1) Master 2 Priority */
/// set macro
#define MATRIX_PRAS1_M2PR(value) ((MATRIX_PRAS1_M2PR_Msk & ((value) << MATRIX_PRAS1_M2PR_Pos)))
#define MATRIX_PRAS1_M3PR_Pos 12	//!< M3PR bit position
#define MATRIX_PRAS1_M3PR_Msk (0x3u << MATRIX_PRAS1_M3PR_Pos) //!< (MATRIX_PRAS1) Master 3 Priority */
/// set macro
#define MATRIX_PRAS1_M3PR(value) ((MATRIX_PRAS1_M3PR_Msk & ((value) << MATRIX_PRAS1_M3PR_Pos)))
#define MATRIX_PRAS1_M4PR_Pos 16	//!< M4PR bit position
#define MATRIX_PRAS1_M4PR_Msk (0x3u << MATRIX_PRAS1_M4PR_Pos) //!< (MATRIX_PRAS1) Master 4 Priority */
/// set macro
#define MATRIX_PRAS1_M4PR(value) ((MATRIX_PRAS1_M4PR_Msk & ((value) << MATRIX_PRAS1_M4PR_Pos)))
/* -------- MATRIX_PRAS2 : (MATRIX Offset: 0x0090) Priority Register A for Slave 2 -------- */
#define MATRIX_PRAS2_M0PR_Pos 0	//!< M0PR bit position
#define MATRIX_PRAS2_M0PR_Msk (0x3u << MATRIX_PRAS2_M0PR_Pos) //!< (MATRIX_PRAS2) Master 0 Priority */
/// set macro
#define MATRIX_PRAS2_M0PR(value) ((MATRIX_PRAS2_M0PR_Msk & ((value) << MATRIX_PRAS2_M0PR_Pos)))
#define MATRIX_PRAS2_M1PR_Pos 4	//!< M1PR bit position
#define MATRIX_PRAS2_M1PR_Msk (0x3u << MATRIX_PRAS2_M1PR_Pos) //!< (MATRIX_PRAS2) Master 1 Priority */
/// set macro
#define MATRIX_PRAS2_M1PR(value) ((MATRIX_PRAS2_M1PR_Msk & ((value) << MATRIX_PRAS2_M1PR_Pos)))
#define MATRIX_PRAS2_M2PR_Pos 8	//!< M2PR bit position
#define MATRIX_PRAS2_M2PR_Msk (0x3u << MATRIX_PRAS2_M2PR_Pos) //!< (MATRIX_PRAS2) Master 2 Priority */
/// set macro
#define MATRIX_PRAS2_M2PR(value) ((MATRIX_PRAS2_M2PR_Msk & ((value) << MATRIX_PRAS2_M2PR_Pos)))
#define MATRIX_PRAS2_M3PR_Pos 12	//!< M3PR bit position
#define MATRIX_PRAS2_M3PR_Msk (0x3u << MATRIX_PRAS2_M3PR_Pos) //!< (MATRIX_PRAS2) Master 3 Priority */
/// set macro
#define MATRIX_PRAS2_M3PR(value) ((MATRIX_PRAS2_M3PR_Msk & ((value) << MATRIX_PRAS2_M3PR_Pos)))
#define MATRIX_PRAS2_M4PR_Pos 16	//!< M4PR bit position
#define MATRIX_PRAS2_M4PR_Msk (0x3u << MATRIX_PRAS2_M4PR_Pos) //!< (MATRIX_PRAS2) Master 4 Priority */
/// set macro
#define MATRIX_PRAS2_M4PR(value) ((MATRIX_PRAS2_M4PR_Msk & ((value) << MATRIX_PRAS2_M4PR_Pos)))
/* -------- MATRIX_PRAS3 : (MATRIX Offset: 0x0098) Priority Register A for Slave 3 -------- */
#define MATRIX_PRAS3_M0PR_Pos 0	//!< M0PR bit position
#define MATRIX_PRAS3_M0PR_Msk (0x3u << MATRIX_PRAS3_M0PR_Pos) //!< (MATRIX_PRAS3) Master 0 Priority */
/// set macro
#define MATRIX_PRAS3_M0PR(value) ((MATRIX_PRAS3_M0PR_Msk & ((value) << MATRIX_PRAS3_M0PR_Pos)))
#define MATRIX_PRAS3_M1PR_Pos 4	//!< M1PR bit position
#define MATRIX_PRAS3_M1PR_Msk (0x3u << MATRIX_PRAS3_M1PR_Pos) //!< (MATRIX_PRAS3) Master 1 Priority */
/// set macro
#define MATRIX_PRAS3_M1PR(value) ((MATRIX_PRAS3_M1PR_Msk & ((value) << MATRIX_PRAS3_M1PR_Pos)))
#define MATRIX_PRAS3_M2PR_Pos 8	//!< M2PR bit position
#define MATRIX_PRAS3_M2PR_Msk (0x3u << MATRIX_PRAS3_M2PR_Pos) //!< (MATRIX_PRAS3) Master 2 Priority */
/// set macro
#define MATRIX_PRAS3_M2PR(value) ((MATRIX_PRAS3_M2PR_Msk & ((value) << MATRIX_PRAS3_M2PR_Pos)))
#define MATRIX_PRAS3_M3PR_Pos 12	//!< M3PR bit position
#define MATRIX_PRAS3_M3PR_Msk (0x3u << MATRIX_PRAS3_M3PR_Pos) //!< (MATRIX_PRAS3) Master 3 Priority */
/// set macro
#define MATRIX_PRAS3_M3PR(value) ((MATRIX_PRAS3_M3PR_Msk & ((value) << MATRIX_PRAS3_M3PR_Pos)))
#define MATRIX_PRAS3_M4PR_Pos 16	//!< M4PR bit position
#define MATRIX_PRAS3_M4PR_Msk (0x3u << MATRIX_PRAS3_M4PR_Pos) //!< (MATRIX_PRAS3) Master 4 Priority */
/// set macro
#define MATRIX_PRAS3_M4PR(value) ((MATRIX_PRAS3_M4PR_Msk & ((value) << MATRIX_PRAS3_M4PR_Pos)))
/* -------- MATRIX_PRAS4 : (MATRIX Offset: 0x00A0) Priority Register A for Slave 4 -------- */
#define MATRIX_PRAS4_M0PR_Pos 0	//!< M0PR bit position
#define MATRIX_PRAS4_M0PR_Msk (0x3u << MATRIX_PRAS4_M0PR_Pos) //!< (MATRIX_PRAS4) Master 0 Priority */
/// set macro
#define MATRIX_PRAS4_M0PR(value) ((MATRIX_PRAS4_M0PR_Msk & ((value) << MATRIX_PRAS4_M0PR_Pos)))
#define MATRIX_PRAS4_M1PR_Pos 4	//!< M1PR bit position
#define MATRIX_PRAS4_M1PR_Msk (0x3u << MATRIX_PRAS4_M1PR_Pos) //!< (MATRIX_PRAS4) Master 1 Priority */
/// set macro
#define MATRIX_PRAS4_M1PR(value) ((MATRIX_PRAS4_M1PR_Msk & ((value) << MATRIX_PRAS4_M1PR_Pos)))
#define MATRIX_PRAS4_M2PR_Pos 8	//!< M2PR bit position
#define MATRIX_PRAS4_M2PR_Msk (0x3u << MATRIX_PRAS4_M2PR_Pos) //!< (MATRIX_PRAS4) Master 2 Priority */
/// set macro
#define MATRIX_PRAS4_M2PR(value) ((MATRIX_PRAS4_M2PR_Msk & ((value) << MATRIX_PRAS4_M2PR_Pos)))
#define MATRIX_PRAS4_M3PR_Pos 12	//!< M3PR bit position
#define MATRIX_PRAS4_M3PR_Msk (0x3u << MATRIX_PRAS4_M3PR_Pos) //!< (MATRIX_PRAS4) Master 3 Priority */
/// set macro
#define MATRIX_PRAS4_M3PR(value) ((MATRIX_PRAS4_M3PR_Msk & ((value) << MATRIX_PRAS4_M3PR_Pos)))
#define MATRIX_PRAS4_M4PR_Pos 16	//!< M4PR bit position
#define MATRIX_PRAS4_M4PR_Msk (0x3u << MATRIX_PRAS4_M4PR_Pos) //!< (MATRIX_PRAS4) Master 4 Priority */
/// set macro
#define MATRIX_PRAS4_M4PR(value) ((MATRIX_PRAS4_M4PR_Msk & ((value) << MATRIX_PRAS4_M4PR_Pos)))
/* -------- CCFG_SYSIO : (MATRIX Offset: 0x0114) System I/O Configuration register -------- */
#define CCFG_SYSIO_SYSIO4 (0x1u << 4) //!< (CCFG_SYSIO) PB4 or TDI Assignment */
#define CCFG_SYSIO_SYSIO5 (0x1u << 5) //!< (CCFG_SYSIO) PB5 or TDO/TRACESWO Assignment */
#define CCFG_SYSIO_SYSIO6 (0x1u << 6) //!< (CCFG_SYSIO) PB6 or TMS/SWDIO Assignment */
#define CCFG_SYSIO_SYSIO7 (0x1u << 7) //!< (CCFG_SYSIO) PB7 or TCK/SWCLK Assignment */
#define CCFG_SYSIO_SYSIO10 (0x1u << 10) //!< (CCFG_SYSIO) PB10 or DDM Assignment */
#define CCFG_SYSIO_SYSIO11 (0x1u << 11) //!< (CCFG_SYSIO) PB11 or DDP Assignment */
#define CCFG_SYSIO_SYSIO12 (0x1u << 12) //!< (CCFG_SYSIO) PB12 or ERASE Assignment */
/* -------- CCFG_SMCNFCS : (MATRIX Offset: 0x011C) SMC Chip Select NAND Flash Assignment Register -------- */
#define CCFG_SMCNFCS_SMC_NFCS0 (0x1u << 0) //!< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 0 Assignment */
#define CCFG_SMCNFCS_SMC_NFCS1 (0x1u << 1) //!< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 1 Assignment */
#define CCFG_SMCNFCS_SMC_NFCS2 (0x1u << 2) //!< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 2 Assignment */
#define CCFG_SMCNFCS_SMC_NFCS3 (0x1u << 3) //!< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 3 Assignment */
/* -------- MATRIX_WPMR : (MATRIX Offset: 0x1E4) Write Protect Mode Register -------- */
#define MATRIX_WPMR_WPEN (0x1u << 0) //!< (MATRIX_WPMR) Write Protect ENable */
#define MATRIX_WPMR_WPKEY_Pos 8	//!< WPKEY bit position
#define MATRIX_WPMR_WPKEY_Msk (0xffffffu << MATRIX_WPMR_WPKEY_Pos) //!< (MATRIX_WPMR) Write Protect KEY (Write-only) */
/// set macro
#define MATRIX_WPMR_WPKEY(value) ((MATRIX_WPMR_WPKEY_Msk & ((value) << MATRIX_WPMR_WPKEY_Pos)))
/* -------- MATRIX_WPSR : (MATRIX Offset: 0x1E8) Write Protect Status Register -------- */
#define MATRIX_WPSR_WPVS (0x1u << 0) //!< (MATRIX_WPSR) Write Protect Violation Status */
#define MATRIX_WPSR_WPVSRC_Pos 8	//!< WPVSRC bit position
#define MATRIX_WPSR_WPVSRC_Msk (0xffffu << MATRIX_WPSR_WPVSRC_Pos) //!< (MATRIX_WPSR) Write Protect Violation Source */

/*@}*/





#ifdef __cplusplus
}
#endif

#endif /* MATRIX_H_ */
