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

/**
 * @ingroup	 hardware_sam3s
 * @defgroup  SAM3S_ADC Analog-to-Digital Converter (ADC)
 * Interface for configuration the Analog-to-Digital Converter (ADC) peripheral.
 * @{
 *
 *  \section Usage
 *
 *  -# Configurate the pins for ADC
 *  -# Initialize the ADC with ADC_Initialize().
 *  -# Select the active channel using ADC_EnableChannel()
 *  -# Start the conversion with ADC_StartConversion()
 *  -# Wait the end of the conversion by polling status with ADC_GetStatus()
 *  -# Finally, get the converted data using ADC_GetConvertedData()
 *
 * @section adc_module Working with ADC
 * The ADC driver provides the interface to configure and use the ADC peripheral.
 * \n
 *
 * It converts the analog input to digital format. The converted result could be
 * 12bit or 10bit. The ADC supports up to 16 analog lines.
 *
 * To Enable a ADC conversion,the user has to follow these few steps:
 * <ul>
 * <li> Select an appropriate reference voltage on ADVREF   </li>
 * <li> Configure the ADC according to its requirements and special needs,which
 * could be  broken down into several parts:
 * -#   Select the resolution by setting or clearing ADC_MR_LOWRES bit in
 *      ADC_MR (Mode Register)
 * -#   Set ADC clock by setting ADC_MR_PRESCAL bits in ADC_MR, the clock is
 *      calculated with ADCClock = MCK / ( (PRESCAL+1) * 2 )
 * -#   Set Startup Time,Tracking Clock cycles and Transfer Clock respectively
 *      in ADC_MR.
 </li>
 * <li> Start conversion by setting ADC_CR_START in ADC_CR. </li>
 * </ul>
 *
 * For more accurate information, please look at the ADC section of the
 * Datasheet.
 *
 * @file     hardware/at91/cortex-m3/sam3s/adc.h
 * @ingroup	 SAM3S_ADC
 * @brief    ADC header
 *
 * @}
*/
#ifndef _ADC_
#define _ADC_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
//#include "chip.h"

//#include <assert.h>
//#include <stdint.h>

/*------------------------------------------------------------------------------
 *         Definitions
 *------------------------------------------------------------------------------*/
/* SAM3S */
#define ADC_FREQ_MAX   20000000		//!< doc!
#define ADC_FREQ_MIN    1000000		//!< doc!

#define ADC_STARTUP_NORM     40		//!< doc!
#define ADC_STARTUP_FAST     12		//!< doc!

#define ADC_CHANNEL_0  0		//!< doc!
#define ADC_CHANNEL_1  1		//!< doc!
#define ADC_CHANNEL_2  2		//!< doc!
#define ADC_CHANNEL_3  3		//!< doc!
#define ADC_CHANNEL_4  4		//!< doc!
#define ADC_CHANNEL_5  5		//!< doc!
#define ADC_CHANNEL_6  6		//!< doc!
#define ADC_CHANNEL_7  7		//!< doc!
#define ADC_CHANNEL_8  8		//!< doc!
#define ADC_CHANNEL_9  9		//!< doc!
#define ADC_CHANNEL_10 10		//!< doc!
#define ADC_CHANNEL_11 11		//!< doc!
#define ADC_CHANNEL_12 12		//!< doc!
#define ADC_CHANNEL_13 13		//!< doc!
#define ADC_CHANNEL_14 14		//!< doc!
#define ADC_CHANNEL_15 15		//!< doc!

#ifdef __cplusplus
 extern "C" {
#endif

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Analog-to-digital Converter */
/* ============================================================================= */

#ifndef __ASSEMBLY__
/** \brief Adc hardware registers */
typedef struct {
WoReg ADC_CR;        //!< (Adc Offset: 0x00) Control Register */
RwReg ADC_MR;        //!< (Adc Offset: 0x04) Mode Register */
RwReg ADC_SEQR1;     //!< (Adc Offset: 0x08) Channel Sequence Register 1 */
RwReg ADC_SEQR2;     //!< (Adc Offset: 0x0C) Channel Sequence Register 2 */
WoReg ADC_CHER;      //!< (Adc Offset: 0x10) Channel Enable Register */
WoReg ADC_CHDR;      //!< (Adc Offset: 0x14) Channel Disable Register */
RoReg ADC_CHSR;      //!< (Adc Offset: 0x18) Channel Status Register */
RwReg Reserved1[1];	 //!< doc!
RoReg ADC_LCDR;      //!< (Adc Offset: 0x20) Last Converted Data Register */
WoReg ADC_IER;       //!< (Adc Offset: 0x24) Interrupt Enable Register */
WoReg ADC_IDR;       //!< (Adc Offset: 0x28) Interrupt Disable Register */
RoReg ADC_IMR;       //!< (Adc Offset: 0x2C) Interrupt Mask Register */
RoReg ADC_ISR;       //!< (Adc Offset: 0x30) Interrupt Status Register */
RwReg Reserved2[2];	 //!< doc!
RoReg ADC_OVER;      //!< (Adc Offset: 0x3C) Overrun Status Register */
RwReg ADC_EMR;       //!< (Adc Offset: 0x40) Extended Mode Register */
RwReg ADC_CWR;       //!< (Adc Offset: 0x44) Compare Window Register */
RwReg ADC_CGR;       //!< (Adc Offset: 0x48) Channel Gain Register */
RwReg ADC_COR;       //!< (Adc Offset: 0x4C) Channel Offset Register */
RoReg ADC_CDR[16];   //!< (Adc Offset: 0x50) Channel Data Register */
RwReg Reserved3[1];	 //!< doc!
RwReg ADC_ACR;       //!< (Adc Offset: 0x94) Analog Control Register */
RwReg Reserved4[19]; //!< doc!
RwReg ADC_WPMR;      //!< (Adc Offset: 0xE4) Write Protect Mode Register */
RoReg ADC_WPSR;      //!< (Adc Offset: 0xE8) Write Protect Status Register */
RwReg Reserved5[5];	 //!< doc!
RwReg ADC_RPR;       //!< (Adc Offset: 0x100) Receive Pointer Register */
RwReg ADC_RCR;       //!< (Adc Offset: 0x104) Receive Counter Register */
RwReg ADC_TPR;       //!< (Adc Offset: 0x108) Transmit Pointer Register */
RwReg ADC_TCR;       //!< (Adc Offset: 0x10C) Transmit Counter Register */
RwReg ADC_RNPR;      //!< (Adc Offset: 0x110) Receive Next Pointer Register */
RwReg ADC_RNCR;      //!< (Adc Offset: 0x114) Receive Next Counter Register */
RwReg ADC_TNPR;      //!< (Adc Offset: 0x118) Transmit Next Pointer Register */
RwReg ADC_TNCR;      //!< (Adc Offset: 0x11C) Transmit Next Counter Register */
WoReg ADC_PTCR;      //!< (Adc Offset: 0x120) Transfer Control Register */
RoReg ADC_PTSR;      //!< (Adc Offset: 0x124) Transfer Status Register */
} Adc;
#endif /* __ASSEMBLY__ */
/* -------- ADC_CR : (ADC Offset: 0x00) Control Register -------- */
#define ADC_CR_SWRST (0x1u << 0) //!< (ADC_CR) Software Reset */
#define ADC_CR_START (0x1u << 1) //!< (ADC_CR) Start Conversion */
/* -------- ADC_MR : (ADC Offset: 0x04) Mode Register -------- */
#define ADC_MR_TRGEN (0x1u << 0) //!< (ADC_MR) Trigger Enable */
#define   ADC_MR_TRGEN_DIS (0x0u << 0) //!< (ADC_MR) Hardware triggers are disabled. Starting a conversion is only possible by software. */
#define   ADC_MR_TRGEN_EN (0x1u << 0) //!< (ADC_MR) Hardware trigger selected by TRGSEL field is enabled. */
#define ADC_MR_TRGSEL_Pos 1	 //!< doc!
#define ADC_MR_TRGSEL_Msk (0x7u << ADC_MR_TRGSEL_Pos) //!< (ADC_MR) Trigger Selection */
#define   ADC_MR_TRGSEL_ADC_TRIG0 (0x0u << 1) //!< (ADC_MR) External trigger */
#define   ADC_MR_TRGSEL_ADC_TRIG1 (0x1u << 1) //!< (ADC_MR) TIO Output of the Timer Counter Channel 0 */
#define   ADC_MR_TRGSEL_ADC_TRIG2 (0x2u << 1) //!< (ADC_MR) TIO Output of the Timer Counter Channel 1 */
#define   ADC_MR_TRGSEL_ADC_TRIG3 (0x3u << 1) //!< (ADC_MR) TIO Output of the Timer Counter Channel 2 */
#define   ADC_MR_TRGSEL_ADC_TRIG4 (0x4u << 1) //!< (ADC_MR) PWM Event Line 0 */
#define   ADC_MR_TRGSEL_ADC_TRIG5 (0x5u << 1) //!< (ADC_MR) PWM Event Line 1 */
#define ADC_MR_LOWRES (0x1u << 4) //!< (ADC_MR) Resolution */
#define   ADC_MR_LOWRES_BITS_12 (0x0u << 4) //!< (ADC_MR) 12-bit resolution */
#define   ADC_MR_LOWRES_BITS_10 (0x1u << 4) //!< (ADC_MR) 10-bit resolution */
#define ADC_MR_SLEEP (0x1u << 5) //!< (ADC_MR) Sleep Mode */
#define   ADC_MR_SLEEP_NORMAL (0x0u << 5) //!< (ADC_MR) Normal Mode: The ADC Core and reference voltage circuitry are kept ON between conversions */
#define   ADC_MR_SLEEP_SLEEP (0x1u << 5) //!< (ADC_MR) Sleep Mode: The ADC Core and reference voltage circuitry are OFF between conversions */
#define ADC_MR_FWUP (0x1u << 6) //!< (ADC_MR) Fast Wake Up */
#define   ADC_MR_FWUP_OFF (0x0u << 6) //!< (ADC_MR) Normal Sleep Mode: The sleep mode is defined by the SLEEP bit */
#define   ADC_MR_FWUP_ON (0x1u << 6) //!< (ADC_MR) Fast Wake Up Sleep Mode: The Voltage reference is ON between conversions and ADC Core is OFF */
#define ADC_MR_FREERUN (0x1u << 7) //!< (ADC_MR) Free Run Mode */
#define   ADC_MR_FREERUN_OFF (0x0u << 7) //!< (ADC_MR) Normal Mode */
#define   ADC_MR_FREERUN_ON (0x1u << 7) //!< (ADC_MR) Free Run Mode: Never wait for any trigger. */
#define ADC_MR_PRESCAL_Pos 8	 //!< doc!
#define ADC_MR_PRESCAL_Msk (0xffu << ADC_MR_PRESCAL_Pos) //!< (ADC_MR) Prescaler Rate Selection */
#define ADC_MR_PRESCAL(value) ((ADC_MR_PRESCAL_Msk & ((value) << ADC_MR_PRESCAL_Pos)))	 //!< doc!
#define ADC_MR_STARTUP_Pos 16	 //!< doc!
#define ADC_MR_STARTUP_Msk (0xfu << ADC_MR_STARTUP_Pos) //!< (ADC_MR) Start Up Time */
#define   ADC_MR_STARTUP_SUT0 (0x0u << 16) //!< (ADC_MR) 0 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT8 (0x1u << 16) //!< (ADC_MR) 8 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT16 (0x2u << 16) //!< (ADC_MR) 16 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT24 (0x3u << 16) //!< (ADC_MR) 24 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT64 (0x4u << 16) //!< (ADC_MR) 64 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT80 (0x5u << 16) //!< (ADC_MR) 80 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT96 (0x6u << 16) //!< (ADC_MR) 96 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT112 (0x7u << 16) //!< (ADC_MR) 112 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT512 (0x8u << 16) //!< (ADC_MR) 512 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT576 (0x9u << 16) //!< (ADC_MR) 576 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT640 (0xAu << 16) //!< (ADC_MR) 640 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT704 (0xBu << 16) //!< (ADC_MR) 704 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT768 (0xCu << 16) //!< (ADC_MR) 768 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT832 (0xDu << 16) //!< (ADC_MR) 832 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT896 (0xEu << 16) //!< (ADC_MR) 896 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT960 (0xFu << 16) //!< (ADC_MR) 960 periods of ADCClock */
#define ADC_MR_SETTLING_Pos 20	 //!< doc!
#define ADC_MR_SETTLING_Msk (0x3u << ADC_MR_SETTLING_Pos) //!< (ADC_MR) Analog Settling Time */
#define ADC_MR_SETTLING(value) ((ADC_MR_SETTLING_Msk & ((value) << ADC_MR_SETTLING_Pos)))	 //!< doc!
#define ADC_MR_ANACH (0x1u << 23) //!< (ADC_MR) Analog Change */
#define   ADC_MR_ANACH_NONE (0x0u << 23) //!< (ADC_MR) No analog change on channel switching: DIFF0, GAIN0 and OFF0 are used for all channels */
#define   ADC_MR_ANACH_ALLOWED (0x1u << 23) //!< (ADC_MR) Allows different analog settings for each channel. See ADC_CGR and ADC_COR Registers */
#define ADC_MR_TRACKTIM_Pos 24	 //!< doc!
#define ADC_MR_TRACKTIM_Msk (0xfu << ADC_MR_TRACKTIM_Pos) //!< (ADC_MR) Tracking Time */
#define ADC_MR_TRACKTIM(value) ((ADC_MR_TRACKTIM_Msk & ((value) << ADC_MR_TRACKTIM_Pos)))	 //!< doc!
#define ADC_MR_TRANSFER_Pos 28	 //!< doc!
#define ADC_MR_TRANSFER_Msk (0x3u << ADC_MR_TRANSFER_Pos) //!< (ADC_MR)  */
#define ADC_MR_TRANSFER(value) ((ADC_MR_TRANSFER_Msk & ((value) << ADC_MR_TRANSFER_Pos)))	 //!< doc!
#define ADC_MR_USEQ (0x1u << 31) //!< (ADC_MR) Use Sequence Enable */
#define   ADC_MR_USEQ_NUM_ORDER (0x0u << 31) //!< (ADC_MR) Normal Mode: The controller converts channels in a simple numeric order. */
#define   ADC_MR_USEQ_REG_ORDER (0x1u << 31) //!< (ADC_MR) User Sequence Mode: The sequence respects what is defined in ADC_SEQR1 and ADC_SEQR2 registers. */
/* -------- ADC_SEQR1 : (ADC Offset: 0x08) Channel Sequence Register 1 -------- */
#define ADC_SEQR1_USCH1_Pos 0	 //!< doc!
#define ADC_SEQR1_USCH1_Msk (0x7u << ADC_SEQR1_USCH1_Pos) //!< (ADC_SEQR1) User Sequence Number 1 */
#define ADC_SEQR1_USCH1(value) ((ADC_SEQR1_USCH1_Msk & ((value) << ADC_SEQR1_USCH1_Pos)))	 //!< doc!
#define ADC_SEQR1_USCH2_Pos 4	 //!< doc!
#define ADC_SEQR1_USCH2_Msk (0x7u << ADC_SEQR1_USCH2_Pos) //!< (ADC_SEQR1) User Sequence Number 2 */
#define ADC_SEQR1_USCH2(value) ((ADC_SEQR1_USCH2_Msk & ((value) << ADC_SEQR1_USCH2_Pos)))	 //!< doc!
#define ADC_SEQR1_USCH3_Pos 8	 //!< doc!
#define ADC_SEQR1_USCH3_Msk (0x7u << ADC_SEQR1_USCH3_Pos) //!< (ADC_SEQR1) User Sequence Number 3 */
#define ADC_SEQR1_USCH3(value) ((ADC_SEQR1_USCH3_Msk & ((value) << ADC_SEQR1_USCH3_Pos)))	 //!< doc!
#define ADC_SEQR1_USCH4_Pos 12	 //!< doc!
#define ADC_SEQR1_USCH4_Msk (0x7u << ADC_SEQR1_USCH4_Pos) //!< (ADC_SEQR1) User Sequence Number 4 */
#define ADC_SEQR1_USCH4(value) ((ADC_SEQR1_USCH4_Msk & ((value) << ADC_SEQR1_USCH4_Pos)))	 //!< doc!
#define ADC_SEQR1_USCH5_Pos 16	 //!< doc!
#define ADC_SEQR1_USCH5_Msk (0x7u << ADC_SEQR1_USCH5_Pos) //!< (ADC_SEQR1) User Sequence Number 5 */
#define ADC_SEQR1_USCH5(value) ((ADC_SEQR1_USCH5_Msk & ((value) << ADC_SEQR1_USCH5_Pos)))	 //!< doc!
#define ADC_SEQR1_USCH6_Pos 20	 //!< doc!
#define ADC_SEQR1_USCH6_Msk (0x7u << ADC_SEQR1_USCH6_Pos) //!< (ADC_SEQR1) User Sequence Number 6 */
#define ADC_SEQR1_USCH6(value) ((ADC_SEQR1_USCH6_Msk & ((value) << ADC_SEQR1_USCH6_Pos)))	 //!< doc!
#define ADC_SEQR1_USCH7_Pos 24	 //!< doc!
#define ADC_SEQR1_USCH7_Msk (0x7u << ADC_SEQR1_USCH7_Pos) //!< (ADC_SEQR1) User Sequence Number 7 */
#define ADC_SEQR1_USCH7(value) ((ADC_SEQR1_USCH7_Msk & ((value) << ADC_SEQR1_USCH7_Pos)))	 //!< doc!
#define ADC_SEQR1_USCH8_Pos 28	 //!< doc!
#define ADC_SEQR1_USCH8_Msk (0x7u << ADC_SEQR1_USCH8_Pos) //!< (ADC_SEQR1) User Sequence Number 8 */
#define ADC_SEQR1_USCH8(value) ((ADC_SEQR1_USCH8_Msk & ((value) << ADC_SEQR1_USCH8_Pos)))	 //!< doc!
/* -------- ADC_SEQR2 : (ADC Offset: 0x0C) Channel Sequence Register 2 -------- */
#define ADC_SEQR2_USCH9_Pos 0	 //!< doc!
#define ADC_SEQR2_USCH9_Msk (0x7u << ADC_SEQR2_USCH9_Pos) //!< (ADC_SEQR2) User Sequence Number 9 */
#define ADC_SEQR2_USCH9(value) ((ADC_SEQR2_USCH9_Msk & ((value) << ADC_SEQR2_USCH9_Pos)))	 //!< doc!
#define ADC_SEQR2_USCH10_Pos 4	 //!< doc!
#define ADC_SEQR2_USCH10_Msk (0x7u << ADC_SEQR2_USCH10_Pos) //!< (ADC_SEQR2) User Sequence Number 10 */
#define ADC_SEQR2_USCH10(value) ((ADC_SEQR2_USCH10_Msk & ((value) << ADC_SEQR2_USCH10_Pos)))	 //!< doc!
#define ADC_SEQR2_USCH11_Pos 8	 //!< doc!
#define ADC_SEQR2_USCH11_Msk (0x7u << ADC_SEQR2_USCH11_Pos) //!< (ADC_SEQR2) User Sequence Number 11 */
#define ADC_SEQR2_USCH11(value) ((ADC_SEQR2_USCH11_Msk & ((value) << ADC_SEQR2_USCH11_Pos)))	 //!< doc!
#define ADC_SEQR2_USCH12_Pos 12	 //!< doc!
#define ADC_SEQR2_USCH12_Msk (0x7u << ADC_SEQR2_USCH12_Pos) //!< (ADC_SEQR2) User Sequence Number 12 */
#define ADC_SEQR2_USCH12(value) ((ADC_SEQR2_USCH12_Msk & ((value) << ADC_SEQR2_USCH12_Pos)))	 //!< doc!
#define ADC_SEQR2_USCH13_Pos 16	 //!< doc!
#define ADC_SEQR2_USCH13_Msk (0x7u << ADC_SEQR2_USCH13_Pos) //!< (ADC_SEQR2) User Sequence Number 13 */
#define ADC_SEQR2_USCH13(value) ((ADC_SEQR2_USCH13_Msk & ((value) << ADC_SEQR2_USCH13_Pos)))	 //!< doc!
#define ADC_SEQR2_USCH14_Pos 20	 //!< doc!
#define ADC_SEQR2_USCH14_Msk (0x7u << ADC_SEQR2_USCH14_Pos) //!< (ADC_SEQR2) User Sequence Number 14 */
#define ADC_SEQR2_USCH14(value) ((ADC_SEQR2_USCH14_Msk & ((value) << ADC_SEQR2_USCH14_Pos)))	 //!< doc!
#define ADC_SEQR2_USCH15_Pos 24	 //!< doc!
#define ADC_SEQR2_USCH15_Msk (0x7u << ADC_SEQR2_USCH15_Pos) //!< (ADC_SEQR2) User Sequence Number 15 */
#define ADC_SEQR2_USCH15(value) ((ADC_SEQR2_USCH15_Msk & ((value) << ADC_SEQR2_USCH15_Pos)))	 //!< doc!
#define ADC_SEQR2_USCH16_Pos 28	 //!< doc!
#define ADC_SEQR2_USCH16_Msk (0x7u << ADC_SEQR2_USCH16_Pos) //!< (ADC_SEQR2) User Sequence Number 16 */
#define ADC_SEQR2_USCH16(value) ((ADC_SEQR2_USCH16_Msk & ((value) << ADC_SEQR2_USCH16_Pos)))	 //!< doc!
/* -------- ADC_CHER : (ADC Offset: 0x10) Channel Enable Register -------- */
#define ADC_CHER_CH0 (0x1u << 0) //!< (ADC_CHER) Channel 0 Enable */
#define ADC_CHER_CH1 (0x1u << 1) //!< (ADC_CHER) Channel 1 Enable */
#define ADC_CHER_CH2 (0x1u << 2) //!< (ADC_CHER) Channel 2 Enable */
#define ADC_CHER_CH3 (0x1u << 3) //!< (ADC_CHER) Channel 3 Enable */
#define ADC_CHER_CH4 (0x1u << 4) //!< (ADC_CHER) Channel 4 Enable */
#define ADC_CHER_CH5 (0x1u << 5) //!< (ADC_CHER) Channel 5 Enable */
#define ADC_CHER_CH6 (0x1u << 6) //!< (ADC_CHER) Channel 6 Enable */
#define ADC_CHER_CH7 (0x1u << 7) //!< (ADC_CHER) Channel 7 Enable */
#define ADC_CHER_CH8 (0x1u << 8) //!< (ADC_CHER) Channel 8 Enable */
#define ADC_CHER_CH9 (0x1u << 9) //!< (ADC_CHER) Channel 9 Enable */
#define ADC_CHER_CH10 (0x1u << 10) //!< (ADC_CHER) Channel 10 Enable */
#define ADC_CHER_CH11 (0x1u << 11) //!< (ADC_CHER) Channel 11 Enable */
#define ADC_CHER_CH12 (0x1u << 12) //!< (ADC_CHER) Channel 12 Enable */
#define ADC_CHER_CH13 (0x1u << 13) //!< (ADC_CHER) Channel 13 Enable */
#define ADC_CHER_CH14 (0x1u << 14) //!< (ADC_CHER) Channel 14 Enable */
#define ADC_CHER_CH15 (0x1u << 15) //!< (ADC_CHER) Channel 15 Enable */
/* -------- ADC_CHDR : (ADC Offset: 0x14) Channel Disable Register -------- */
#define ADC_CHDR_CH0 (0x1u << 0) //!< (ADC_CHDR) Channel 0 Disable */
#define ADC_CHDR_CH1 (0x1u << 1) //!< (ADC_CHDR) Channel 1 Disable */
#define ADC_CHDR_CH2 (0x1u << 2) //!< (ADC_CHDR) Channel 2 Disable */
#define ADC_CHDR_CH3 (0x1u << 3) //!< (ADC_CHDR) Channel 3 Disable */
#define ADC_CHDR_CH4 (0x1u << 4) //!< (ADC_CHDR) Channel 4 Disable */
#define ADC_CHDR_CH5 (0x1u << 5) //!< (ADC_CHDR) Channel 5 Disable */
#define ADC_CHDR_CH6 (0x1u << 6) //!< (ADC_CHDR) Channel 6 Disable */
#define ADC_CHDR_CH7 (0x1u << 7) //!< (ADC_CHDR) Channel 7 Disable */
#define ADC_CHDR_CH8 (0x1u << 8) //!< (ADC_CHDR) Channel 8 Disable */
#define ADC_CHDR_CH9 (0x1u << 9) //!< (ADC_CHDR) Channel 9 Disable */
#define ADC_CHDR_CH10 (0x1u << 10) //!< (ADC_CHDR) Channel 10 Disable */
#define ADC_CHDR_CH11 (0x1u << 11) //!< (ADC_CHDR) Channel 11 Disable */
#define ADC_CHDR_CH12 (0x1u << 12) //!< (ADC_CHDR) Channel 12 Disable */
#define ADC_CHDR_CH13 (0x1u << 13) //!< (ADC_CHDR) Channel 13 Disable */
#define ADC_CHDR_CH14 (0x1u << 14) //!< (ADC_CHDR) Channel 14 Disable */
#define ADC_CHDR_CH15 (0x1u << 15) //!< (ADC_CHDR) Channel 15 Disable */
/* -------- ADC_CHSR : (ADC Offset: 0x18) Channel Status Register -------- */
#define ADC_CHSR_CH0 (0x1u << 0) //!< (ADC_CHSR) Channel 0 Status */
#define ADC_CHSR_CH1 (0x1u << 1) //!< (ADC_CHSR) Channel 1 Status */
#define ADC_CHSR_CH2 (0x1u << 2) //!< (ADC_CHSR) Channel 2 Status */
#define ADC_CHSR_CH3 (0x1u << 3) //!< (ADC_CHSR) Channel 3 Status */
#define ADC_CHSR_CH4 (0x1u << 4) //!< (ADC_CHSR) Channel 4 Status */
#define ADC_CHSR_CH5 (0x1u << 5) //!< (ADC_CHSR) Channel 5 Status */
#define ADC_CHSR_CH6 (0x1u << 6) //!< (ADC_CHSR) Channel 6 Status */
#define ADC_CHSR_CH7 (0x1u << 7) //!< (ADC_CHSR) Channel 7 Status */
#define ADC_CHSR_CH8 (0x1u << 8) //!< (ADC_CHSR) Channel 8 Status */
#define ADC_CHSR_CH9 (0x1u << 9) //!< (ADC_CHSR) Channel 9 Status */
#define ADC_CHSR_CH10 (0x1u << 10) //!< (ADC_CHSR) Channel 10 Status */
#define ADC_CHSR_CH11 (0x1u << 11) //!< (ADC_CHSR) Channel 11 Status */
#define ADC_CHSR_CH12 (0x1u << 12) //!< (ADC_CHSR) Channel 12 Status */
#define ADC_CHSR_CH13 (0x1u << 13) //!< (ADC_CHSR) Channel 13 Status */
#define ADC_CHSR_CH14 (0x1u << 14) //!< (ADC_CHSR) Channel 14 Status */
#define ADC_CHSR_CH15 (0x1u << 15) //!< (ADC_CHSR) Channel 15 Status */
/* -------- ADC_LCDR : (ADC Offset: 0x20) Last Converted Data Register -------- */
#define ADC_LCDR_LDATA_Pos 0	 //!< doc!
#define ADC_LCDR_LDATA_Msk (0xfffu << ADC_LCDR_LDATA_Pos) //!< (ADC_LCDR) Last Data Converted */
#define ADC_LCDR_CHNB_Pos 12	 //!< doc!
#define ADC_LCDR_CHNB_Msk (0xfu << ADC_LCDR_CHNB_Pos) //!< (ADC_LCDR) Channel Number */
/* -------- ADC_IER : (ADC Offset: 0x24) Interrupt Enable Register -------- */
#define ADC_IER_EOC0 (0x1u << 0) //!< (ADC_IER) End of Conversion Interrupt Enable 0 */
#define ADC_IER_EOC1 (0x1u << 1) //!< (ADC_IER) End of Conversion Interrupt Enable 1 */
#define ADC_IER_EOC2 (0x1u << 2) //!< (ADC_IER) End of Conversion Interrupt Enable 2 */
#define ADC_IER_EOC3 (0x1u << 3) //!< (ADC_IER) End of Conversion Interrupt Enable 3 */
#define ADC_IER_EOC4 (0x1u << 4) //!< (ADC_IER) End of Conversion Interrupt Enable 4 */
#define ADC_IER_EOC5 (0x1u << 5) //!< (ADC_IER) End of Conversion Interrupt Enable 5 */
#define ADC_IER_EOC6 (0x1u << 6) //!< (ADC_IER) End of Conversion Interrupt Enable 6 */
#define ADC_IER_EOC7 (0x1u << 7) //!< (ADC_IER) End of Conversion Interrupt Enable 7 */
#define ADC_IER_EOC8 (0x1u << 8) //!< (ADC_IER) End of Conversion Interrupt Enable 8 */
#define ADC_IER_EOC9 (0x1u << 9) //!< (ADC_IER) End of Conversion Interrupt Enable 9 */
#define ADC_IER_EOC10 (0x1u << 10) //!< (ADC_IER) End of Conversion Interrupt Enable 10 */
#define ADC_IER_EOC11 (0x1u << 11) //!< (ADC_IER) End of Conversion Interrupt Enable 11 */
#define ADC_IER_EOC12 (0x1u << 12) //!< (ADC_IER) End of Conversion Interrupt Enable 12 */
#define ADC_IER_EOC13 (0x1u << 13) //!< (ADC_IER) End of Conversion Interrupt Enable 13 */
#define ADC_IER_EOC14 (0x1u << 14) //!< (ADC_IER) End of Conversion Interrupt Enable 14 */
#define ADC_IER_EOC15 (0x1u << 15) //!< (ADC_IER) End of Conversion Interrupt Enable 15 */
#define ADC_IER_DRDY (0x1u << 24) //!< (ADC_IER) Data Ready Interrupt Enable */
#define ADC_IER_GOVRE (0x1u << 25) //!< (ADC_IER) General Overrun Error Interrupt Enable */
#define ADC_IER_COMPE (0x1u << 26) //!< (ADC_IER) Comparison Event Interrupt Enable */
#define ADC_IER_ENDRX (0x1u << 27) //!< (ADC_IER) End of Receive Buffer Interrupt Enable */
#define ADC_IER_RXBUFF (0x1u << 28) //!< (ADC_IER) Receive Buffer Full Interrupt Enable */
/* -------- ADC_IDR : (ADC Offset: 0x28) Interrupt Disable Register -------- */
#define ADC_IDR_EOC0 (0x1u << 0) //!< (ADC_IDR) End of Conversion Interrupt Disable 0 */
#define ADC_IDR_EOC1 (0x1u << 1) //!< (ADC_IDR) End of Conversion Interrupt Disable 1 */
#define ADC_IDR_EOC2 (0x1u << 2) //!< (ADC_IDR) End of Conversion Interrupt Disable 2 */
#define ADC_IDR_EOC3 (0x1u << 3) //!< (ADC_IDR) End of Conversion Interrupt Disable 3 */
#define ADC_IDR_EOC4 (0x1u << 4) //!< (ADC_IDR) End of Conversion Interrupt Disable 4 */
#define ADC_IDR_EOC5 (0x1u << 5) //!< (ADC_IDR) End of Conversion Interrupt Disable 5 */
#define ADC_IDR_EOC6 (0x1u << 6) //!< (ADC_IDR) End of Conversion Interrupt Disable 6 */
#define ADC_IDR_EOC7 (0x1u << 7) //!< (ADC_IDR) End of Conversion Interrupt Disable 7 */
#define ADC_IDR_EOC8 (0x1u << 8) //!< (ADC_IDR) End of Conversion Interrupt Disable 8 */
#define ADC_IDR_EOC9 (0x1u << 9) //!< (ADC_IDR) End of Conversion Interrupt Disable 9 */
#define ADC_IDR_EOC10 (0x1u << 10) //!< (ADC_IDR) End of Conversion Interrupt Disable 10 */
#define ADC_IDR_EOC11 (0x1u << 11) //!< (ADC_IDR) End of Conversion Interrupt Disable 11 */
#define ADC_IDR_EOC12 (0x1u << 12) //!< (ADC_IDR) End of Conversion Interrupt Disable 12 */
#define ADC_IDR_EOC13 (0x1u << 13) //!< (ADC_IDR) End of Conversion Interrupt Disable 13 */
#define ADC_IDR_EOC14 (0x1u << 14) //!< (ADC_IDR) End of Conversion Interrupt Disable 14 */
#define ADC_IDR_EOC15 (0x1u << 15) //!< (ADC_IDR) End of Conversion Interrupt Disable 15 */
#define ADC_IDR_DRDY (0x1u << 24) //!< (ADC_IDR) Data Ready Interrupt Disable */
#define ADC_IDR_GOVRE (0x1u << 25) //!< (ADC_IDR) General Overrun Error Interrupt Disable */
#define ADC_IDR_COMPE (0x1u << 26) //!< (ADC_IDR) Comparison Event Interrupt Disable */
#define ADC_IDR_ENDRX (0x1u << 27) //!< (ADC_IDR) End of Receive Buffer Interrupt Disable */
#define ADC_IDR_RXBUFF (0x1u << 28) //!< (ADC_IDR) Receive Buffer Full Interrupt Disable */
/* -------- ADC_IMR : (ADC Offset: 0x2C) Interrupt Mask Register -------- */
#define ADC_IMR_EOC0 (0x1u << 0) //!< (ADC_IMR) End of Conversion Interrupt Mask 0 */
#define ADC_IMR_EOC1 (0x1u << 1) //!< (ADC_IMR) End of Conversion Interrupt Mask 1 */
#define ADC_IMR_EOC2 (0x1u << 2) //!< (ADC_IMR) End of Conversion Interrupt Mask 2 */
#define ADC_IMR_EOC3 (0x1u << 3) //!< (ADC_IMR) End of Conversion Interrupt Mask 3 */
#define ADC_IMR_EOC4 (0x1u << 4) //!< (ADC_IMR) End of Conversion Interrupt Mask 4 */
#define ADC_IMR_EOC5 (0x1u << 5) //!< (ADC_IMR) End of Conversion Interrupt Mask 5 */
#define ADC_IMR_EOC6 (0x1u << 6) //!< (ADC_IMR) End of Conversion Interrupt Mask 6 */
#define ADC_IMR_EOC7 (0x1u << 7) //!< (ADC_IMR) End of Conversion Interrupt Mask 7 */
#define ADC_IMR_EOC8 (0x1u << 8) //!< (ADC_IMR) End of Conversion Interrupt Mask 8 */
#define ADC_IMR_EOC9 (0x1u << 9) //!< (ADC_IMR) End of Conversion Interrupt Mask 9 */
#define ADC_IMR_EOC10 (0x1u << 10) //!< (ADC_IMR) End of Conversion Interrupt Mask 10 */
#define ADC_IMR_EOC11 (0x1u << 11) //!< (ADC_IMR) End of Conversion Interrupt Mask 11 */
#define ADC_IMR_EOC12 (0x1u << 12) //!< (ADC_IMR) End of Conversion Interrupt Mask 12 */
#define ADC_IMR_EOC13 (0x1u << 13) //!< (ADC_IMR) End of Conversion Interrupt Mask 13 */
#define ADC_IMR_EOC14 (0x1u << 14) //!< (ADC_IMR) End of Conversion Interrupt Mask 14 */
#define ADC_IMR_EOC15 (0x1u << 15) //!< (ADC_IMR) End of Conversion Interrupt Mask 15 */
#define ADC_IMR_DRDY (0x1u << 24) //!< (ADC_IMR) Data Ready Interrupt Mask */
#define ADC_IMR_GOVRE (0x1u << 25) //!< (ADC_IMR) General Overrun Error Interrupt Mask */
#define ADC_IMR_COMPE (0x1u << 26) //!< (ADC_IMR) Comparison Event Interrupt Mask */
#define ADC_IMR_ENDRX (0x1u << 27) //!< (ADC_IMR) End of Receive Buffer Interrupt Mask */
#define ADC_IMR_RXBUFF (0x1u << 28) //!< (ADC_IMR) Receive Buffer Full Interrupt Mask */
/* -------- ADC_ISR : (ADC Offset: 0x30) Interrupt Status Register -------- */
#define ADC_ISR_EOC0 (0x1u << 0) //!< (ADC_ISR) End of Conversion 0 */
#define ADC_ISR_EOC1 (0x1u << 1) //!< (ADC_ISR) End of Conversion 1 */
#define ADC_ISR_EOC2 (0x1u << 2) //!< (ADC_ISR) End of Conversion 2 */
#define ADC_ISR_EOC3 (0x1u << 3) //!< (ADC_ISR) End of Conversion 3 */
#define ADC_ISR_EOC4 (0x1u << 4) //!< (ADC_ISR) End of Conversion 4 */
#define ADC_ISR_EOC5 (0x1u << 5) //!< (ADC_ISR) End of Conversion 5 */
#define ADC_ISR_EOC6 (0x1u << 6) //!< (ADC_ISR) End of Conversion 6 */
#define ADC_ISR_EOC7 (0x1u << 7) //!< (ADC_ISR) End of Conversion 7 */
#define ADC_ISR_EOC8 (0x1u << 8) //!< (ADC_ISR) End of Conversion 8 */
#define ADC_ISR_EOC9 (0x1u << 9) //!< (ADC_ISR) End of Conversion 9 */
#define ADC_ISR_EOC10 (0x1u << 10) //!< (ADC_ISR) End of Conversion 10 */
#define ADC_ISR_EOC11 (0x1u << 11) //!< (ADC_ISR) End of Conversion 11 */
#define ADC_ISR_EOC12 (0x1u << 12) //!< (ADC_ISR) End of Conversion 12 */
#define ADC_ISR_EOC13 (0x1u << 13) //!< (ADC_ISR) End of Conversion 13 */
#define ADC_ISR_EOC14 (0x1u << 14) //!< (ADC_ISR) End of Conversion 14 */
#define ADC_ISR_EOC15 (0x1u << 15) //!< (ADC_ISR) End of Conversion 15 */
#define ADC_ISR_DRDY (0x1u << 24) //!< (ADC_ISR) Data Ready */
#define ADC_ISR_GOVRE (0x1u << 25) //!< (ADC_ISR) General Overrun Error */
#define ADC_ISR_COMPE (0x1u << 26) //!< (ADC_ISR) Comparison Error */
#define ADC_ISR_ENDRX (0x1u << 27) //!< (ADC_ISR) End of RX Buffer */
#define ADC_ISR_RXBUFF (0x1u << 28) //!< (ADC_ISR) RX Buffer Full */
/* -------- ADC_OVER : (ADC Offset: 0x3C) Overrun Status Register -------- */
#define ADC_OVER_OVRE0 (0x1u << 0) //!< (ADC_OVER) Overrun Error 0 */
#define ADC_OVER_OVRE1 (0x1u << 1) //!< (ADC_OVER) Overrun Error 1 */
#define ADC_OVER_OVRE2 (0x1u << 2) //!< (ADC_OVER) Overrun Error 2 */
#define ADC_OVER_OVRE3 (0x1u << 3) //!< (ADC_OVER) Overrun Error 3 */
#define ADC_OVER_OVRE4 (0x1u << 4) //!< (ADC_OVER) Overrun Error 4 */
#define ADC_OVER_OVRE5 (0x1u << 5) //!< (ADC_OVER) Overrun Error 5 */
#define ADC_OVER_OVRE6 (0x1u << 6) //!< (ADC_OVER) Overrun Error 6 */
#define ADC_OVER_OVRE7 (0x1u << 7) //!< (ADC_OVER) Overrun Error 7 */
#define ADC_OVER_OVRE8 (0x1u << 8) //!< (ADC_OVER) Overrun Error 8 */
#define ADC_OVER_OVRE9 (0x1u << 9) //!< (ADC_OVER) Overrun Error 9 */
#define ADC_OVER_OVRE10 (0x1u << 10) //!< (ADC_OVER) Overrun Error 10 */
#define ADC_OVER_OVRE11 (0x1u << 11) //!< (ADC_OVER) Overrun Error 11 */
#define ADC_OVER_OVRE12 (0x1u << 12) //!< (ADC_OVER) Overrun Error 12 */
#define ADC_OVER_OVRE13 (0x1u << 13) //!< (ADC_OVER) Overrun Error 13 */
#define ADC_OVER_OVRE14 (0x1u << 14) //!< (ADC_OVER) Overrun Error 14 */
#define ADC_OVER_OVRE15 (0x1u << 15) //!< (ADC_OVER) Overrun Error 15 */
/* -------- ADC_EMR : (ADC Offset: 0x40) Extended Mode Register -------- */
#define ADC_EMR_CMPMODE_Pos 0	 //!< doc!
#define ADC_EMR_CMPMODE_Msk (0x3u << ADC_EMR_CMPMODE_Pos) //!< (ADC_EMR) Comparison Mode */
#define   ADC_EMR_CMPMODE_LOW (0x0u << 0) //!< (ADC_EMR) Generates an event when the converted data is lower than the low threshold of the window. */
#define   ADC_EMR_CMPMODE_HIGH (0x1u << 0) //!< (ADC_EMR) Generates an event when the converted data is higher than the high threshold of the window. */
#define   ADC_EMR_CMPMODE_IN (0x2u << 0) //!< (ADC_EMR) Generates an event when the converted data is in the comparison window. */
#define   ADC_EMR_CMPMODE_OUT (0x3u << 0) //!< (ADC_EMR) Generates an event when the converted data is out of the comparison window. */
#define ADC_EMR_CMPSEL_Pos 4	 //!< doc!
#define ADC_EMR_CMPSEL_Msk (0xfu << ADC_EMR_CMPSEL_Pos) //!< (ADC_EMR) Comparison Selected Channel */
#define ADC_EMR_CMPSEL(value) ((ADC_EMR_CMPSEL_Msk & ((value) << ADC_EMR_CMPSEL_Pos)))	 //!< doc!
#define ADC_EMR_CMPALL (0x1u << 9) //!< (ADC_EMR) Compare All Channels */
#define ADC_EMR_TAG (0x1u << 24) //!< (ADC_EMR) TAG of ADC_LDCR register */
/* -------- ADC_CWR : (ADC Offset: 0x44) Compare Window Register -------- */
#define ADC_CWR_LOWTHRES_Pos 0	 //!< doc!
#define ADC_CWR_LOWTHRES_Msk (0xfffu << ADC_CWR_LOWTHRES_Pos) //!< (ADC_CWR) Low Threshold */
#define ADC_CWR_LOWTHRES(value) ((ADC_CWR_LOWTHRES_Msk & ((value) << ADC_CWR_LOWTHRES_Pos)))	 //!< doc!
#define ADC_CWR_HIGHTHRES_Pos 16	 //!< doc!
#define ADC_CWR_HIGHTHRES_Msk (0xfffu << ADC_CWR_HIGHTHRES_Pos) //!< (ADC_CWR) High Threshold */
#define ADC_CWR_HIGHTHRES(value) ((ADC_CWR_HIGHTHRES_Msk & ((value) << ADC_CWR_HIGHTHRES_Pos)))	 //!< doc!
/* -------- ADC_CGR : (ADC Offset: 0x48) Channel Gain Register -------- */
#define ADC_CGR_GAIN0_Pos 0	 //!< doc!
#define ADC_CGR_GAIN0_Msk (0x3u << ADC_CGR_GAIN0_Pos) //!< (ADC_CGR) Gain for channel 0 */
#define ADC_CGR_GAIN0(value) ((ADC_CGR_GAIN0_Msk & ((value) << ADC_CGR_GAIN0_Pos)))	 //!< doc!
#define ADC_CGR_GAIN1_Pos 2	 //!< doc!
#define ADC_CGR_GAIN1_Msk (0x3u << ADC_CGR_GAIN1_Pos) //!< (ADC_CGR) Gain for channel 1 */
#define ADC_CGR_GAIN1(value) ((ADC_CGR_GAIN1_Msk & ((value) << ADC_CGR_GAIN1_Pos)))	 //!< doc!
#define ADC_CGR_GAIN2_Pos 4	 //!< doc!
#define ADC_CGR_GAIN2_Msk (0x3u << ADC_CGR_GAIN2_Pos) //!< (ADC_CGR) Gain for channel 2 */
#define ADC_CGR_GAIN2(value) ((ADC_CGR_GAIN2_Msk & ((value) << ADC_CGR_GAIN2_Pos)))	 //!< doc!
#define ADC_CGR_GAIN3_Pos 6	 //!< doc!
#define ADC_CGR_GAIN3_Msk (0x3u << ADC_CGR_GAIN3_Pos) //!< (ADC_CGR) Gain for channel 3 */
#define ADC_CGR_GAIN3(value) ((ADC_CGR_GAIN3_Msk & ((value) << ADC_CGR_GAIN3_Pos)))	 //!< doc!
#define ADC_CGR_GAIN4_Pos 8	 //!< doc!
#define ADC_CGR_GAIN4_Msk (0x3u << ADC_CGR_GAIN4_Pos) //!< (ADC_CGR) Gain for channel 4 */
#define ADC_CGR_GAIN4(value) ((ADC_CGR_GAIN4_Msk & ((value) << ADC_CGR_GAIN4_Pos)))	 //!< doc!
#define ADC_CGR_GAIN5_Pos 10	 //!< doc!
#define ADC_CGR_GAIN5_Msk (0x3u << ADC_CGR_GAIN5_Pos) //!< (ADC_CGR) Gain for channel 5 */
#define ADC_CGR_GAIN5(value) ((ADC_CGR_GAIN5_Msk & ((value) << ADC_CGR_GAIN5_Pos)))	 //!< doc!
#define ADC_CGR_GAIN6_Pos 12	 //!< doc!
#define ADC_CGR_GAIN6_Msk (0x3u << ADC_CGR_GAIN6_Pos) //!< (ADC_CGR) Gain for channel 6 */
#define ADC_CGR_GAIN6(value) ((ADC_CGR_GAIN6_Msk & ((value) << ADC_CGR_GAIN6_Pos)))	 //!< doc!
#define ADC_CGR_GAIN7_Pos 14	 //!< doc!
#define ADC_CGR_GAIN7_Msk (0x3u << ADC_CGR_GAIN7_Pos) //!< (ADC_CGR) Gain for channel 7 */
#define ADC_CGR_GAIN7(value) ((ADC_CGR_GAIN7_Msk & ((value) << ADC_CGR_GAIN7_Pos)))	 //!< doc!
#define ADC_CGR_GAIN8_Pos 16	 //!< doc!
#define ADC_CGR_GAIN8_Msk (0x3u << ADC_CGR_GAIN8_Pos) //!< (ADC_CGR) Gain for channel 8 */
#define ADC_CGR_GAIN8(value) ((ADC_CGR_GAIN8_Msk & ((value) << ADC_CGR_GAIN8_Pos)))	 //!< doc!
#define ADC_CGR_GAIN9_Pos 18	 //!< doc!
#define ADC_CGR_GAIN9_Msk (0x3u << ADC_CGR_GAIN9_Pos) //!< (ADC_CGR) Gain for channel 9 */
#define ADC_CGR_GAIN9(value) ((ADC_CGR_GAIN9_Msk & ((value) << ADC_CGR_GAIN9_Pos)))	 //!< doc!
#define ADC_CGR_GAIN10_Pos 20	 //!< doc!
#define ADC_CGR_GAIN10_Msk (0x3u << ADC_CGR_GAIN10_Pos) //!< (ADC_CGR) Gain for channel 10 */
#define ADC_CGR_GAIN10(value) ((ADC_CGR_GAIN10_Msk & ((value) << ADC_CGR_GAIN10_Pos)))	 //!< doc!
#define ADC_CGR_GAIN11_Pos 22	 //!< doc!
#define ADC_CGR_GAIN11_Msk (0x3u << ADC_CGR_GAIN11_Pos) //!< (ADC_CGR) Gain for channel 11 */
#define ADC_CGR_GAIN11(value) ((ADC_CGR_GAIN11_Msk & ((value) << ADC_CGR_GAIN11_Pos)))	 //!< doc!
#define ADC_CGR_GAIN12_Pos 24	 //!< doc!
#define ADC_CGR_GAIN12_Msk (0x3u << ADC_CGR_GAIN12_Pos) //!< (ADC_CGR) Gain for channel 12 */
#define ADC_CGR_GAIN12(value) ((ADC_CGR_GAIN12_Msk & ((value) << ADC_CGR_GAIN12_Pos)))	 //!< doc!
#define ADC_CGR_GAIN13_Pos 26	 //!< doc!
#define ADC_CGR_GAIN13_Msk (0x3u << ADC_CGR_GAIN13_Pos) //!< (ADC_CGR) Gain for channel 13 */
#define ADC_CGR_GAIN13(value) ((ADC_CGR_GAIN13_Msk & ((value) << ADC_CGR_GAIN13_Pos)))	 //!< doc!
#define ADC_CGR_GAIN14_Pos 28	 //!< doc!
#define ADC_CGR_GAIN14_Msk (0x3u << ADC_CGR_GAIN14_Pos) //!< (ADC_CGR) Gain for channel 14 */
#define ADC_CGR_GAIN14(value) ((ADC_CGR_GAIN14_Msk & ((value) << ADC_CGR_GAIN14_Pos)))	 //!< doc!
#define ADC_CGR_GAIN15_Pos 30	 //!< doc!
#define ADC_CGR_GAIN15_Msk (0x3u << ADC_CGR_GAIN15_Pos) //!< (ADC_CGR) Gain for channel 15 */
#define ADC_CGR_GAIN15(value) ((ADC_CGR_GAIN15_Msk & ((value) << ADC_CGR_GAIN15_Pos)))	 //!< doc!
/* -------- ADC_COR : (ADC Offset: 0x4C) Channel Offset Register -------- */
#define ADC_COR_OFF0 (0x1u << 0) //!< (ADC_COR) Offset for channel 0 */
#define ADC_COR_OFF1 (0x1u << 1) //!< (ADC_COR) Offset for channel 1 */
#define ADC_COR_OFF2 (0x1u << 2) //!< (ADC_COR) Offset for channel 2 */
#define ADC_COR_OFF3 (0x1u << 3) //!< (ADC_COR) Offset for channel 3 */
#define ADC_COR_OFF4 (0x1u << 4) //!< (ADC_COR) Offset for channel 4 */
#define ADC_COR_OFF5 (0x1u << 5) //!< (ADC_COR) Offset for channel 5 */
#define ADC_COR_OFF6 (0x1u << 6) //!< (ADC_COR) Offset for channel 6 */
#define ADC_COR_OFF7 (0x1u << 7) //!< (ADC_COR) Offset for channel 7 */
#define ADC_COR_OFF8 (0x1u << 8) //!< (ADC_COR) Offset for channel 8 */
#define ADC_COR_OFF9 (0x1u << 9) //!< (ADC_COR) Offset for channel 9 */
#define ADC_COR_OFF10 (0x1u << 10) //!< (ADC_COR) Offset for channel 10 */
#define ADC_COR_OFF11 (0x1u << 11) //!< (ADC_COR) Offset for channel 11 */
#define ADC_COR_OFF12 (0x1u << 12) //!< (ADC_COR) Offset for channel 12 */
#define ADC_COR_OFF13 (0x1u << 13) //!< (ADC_COR) Offset for channel 13 */
#define ADC_COR_OFF14 (0x1u << 14) //!< (ADC_COR) Offset for channel 14 */
#define ADC_COR_OFF15 (0x1u << 15) //!< (ADC_COR) Offset for channel 15 */
#define ADC_COR_DIFF0 (0x1u << 16) //!< (ADC_COR) Differential inputs for channel 0 */
#define ADC_COR_DIFF1 (0x1u << 17) //!< (ADC_COR) Differential inputs for channel 1 */
#define ADC_COR_DIFF2 (0x1u << 18) //!< (ADC_COR) Differential inputs for channel 2 */
#define ADC_COR_DIFF3 (0x1u << 19) //!< (ADC_COR) Differential inputs for channel 3 */
#define ADC_COR_DIFF4 (0x1u << 20) //!< (ADC_COR) Differential inputs for channel 4 */
#define ADC_COR_DIFF5 (0x1u << 21) //!< (ADC_COR) Differential inputs for channel 5 */
#define ADC_COR_DIFF6 (0x1u << 22) //!< (ADC_COR) Differential inputs for channel 6 */
#define ADC_COR_DIFF7 (0x1u << 23) //!< (ADC_COR) Differential inputs for channel 7 */
#define ADC_COR_DIFF8 (0x1u << 24) //!< (ADC_COR) Differential inputs for channel 8 */
#define ADC_COR_DIFF9 (0x1u << 25) //!< (ADC_COR) Differential inputs for channel 9 */
#define ADC_COR_DIFF10 (0x1u << 26) //!< (ADC_COR) Differential inputs for channel 10 */
#define ADC_COR_DIFF11 (0x1u << 27) //!< (ADC_COR) Differential inputs for channel 11 */
#define ADC_COR_DIFF12 (0x1u << 28) //!< (ADC_COR) Differential inputs for channel 12 */
#define ADC_COR_DIFF13 (0x1u << 29) //!< (ADC_COR) Differential inputs for channel 13 */
#define ADC_COR_DIFF14 (0x1u << 30) //!< (ADC_COR) Differential inputs for channel 14 */
#define ADC_COR_DIFF15 (0x1u << 31) //!< (ADC_COR) Differential inputs for channel 15 */
/* -------- ADC_CDR[16] : (ADC Offset: 0x50) Channel Data Register -------- */
#define ADC_CDR_DATA_Pos 0	 //!< doc!
#define ADC_CDR_DATA_Msk (0x3ffu << ADC_CDR_DATA_Pos) //!< (ADC_CDR[16]) Converted Data */
/* -------- ADC_ACR : (ADC Offset: 0x94) Analog Control Register -------- */
#define ADC_ACR_TSON (0x1u << 4) //!< (ADC_ACR) Temperature Sensor On */
#define ADC_ACR_IBCTL_Pos 8	 //!< doc!
#define ADC_ACR_IBCTL_Msk (0x3u << ADC_ACR_IBCTL_Pos) //!< (ADC_ACR) ADC Bias Current Control */
#define ADC_ACR_IBCTL(value) ((ADC_ACR_IBCTL_Msk & ((value) << ADC_ACR_IBCTL_Pos)))	 //!< doc!
/* -------- ADC_WPMR : (ADC Offset: 0xE4) Write Protect Mode Register -------- */
#define ADC_WPMR_WPEN (0x1u << 0) //!< (ADC_WPMR) Write Protect Enable */
#define ADC_WPMR_WPKEY_Pos 8	 //!< doc!
#define ADC_WPMR_WPKEY_Msk (0xffffffu << ADC_WPMR_WPKEY_Pos) //!< (ADC_WPMR) Write Protect KEY */
#define ADC_WPMR_WPKEY(value) ((ADC_WPMR_WPKEY_Msk & ((value) << ADC_WPMR_WPKEY_Pos)))	 //!< doc!
/* -------- ADC_WPSR : (ADC Offset: 0xE8) Write Protect Status Register -------- */
#define ADC_WPSR_WPVS (0x1u << 0) //!< (ADC_WPSR) Write Protect Violation Status */
#define ADC_WPSR_WPVSRC_Pos 8	 //!< doc!
#define ADC_WPSR_WPVSRC_Msk (0xffffu << ADC_WPSR_WPVSRC_Pos) //!< (ADC_WPSR) Write Protect Violation Source */
/* -------- ADC_RPR : (ADC Offset: 0x100) Receive Pointer Register -------- */
#define ADC_RPR_RXPTR_Pos 0	 //!< doc!
#define ADC_RPR_RXPTR_Msk (0xffffffffu << ADC_RPR_RXPTR_Pos) //!< (ADC_RPR) Receive Pointer Register */
#define ADC_RPR_RXPTR(value) ((ADC_RPR_RXPTR_Msk & ((value) << ADC_RPR_RXPTR_Pos)))	 //!< doc!
/* -------- ADC_RCR : (ADC Offset: 0x104) Receive Counter Register -------- */
#define ADC_RCR_RXCTR_Pos 0	 //!< doc!
#define ADC_RCR_RXCTR_Msk (0xffffu << ADC_RCR_RXCTR_Pos) //!< (ADC_RCR) Receive Counter Register */
#define ADC_RCR_RXCTR(value) ((ADC_RCR_RXCTR_Msk & ((value) << ADC_RCR_RXCTR_Pos)))	 //!< doc!
/* -------- ADC_TPR : (ADC Offset: 0x108) Transmit Pointer Register -------- */
#define ADC_TPR_TXPTR_Pos 0	 //!< doc!
#define ADC_TPR_TXPTR_Msk (0xffffffffu << ADC_TPR_TXPTR_Pos) //!< (ADC_TPR) Transmit Counter Register */
#define ADC_TPR_TXPTR(value) ((ADC_TPR_TXPTR_Msk & ((value) << ADC_TPR_TXPTR_Pos)))	 //!< doc!
/* -------- ADC_TCR : (ADC Offset: 0x10C) Transmit Counter Register -------- */
#define ADC_TCR_TXCTR_Pos 0	 //!< doc!
#define ADC_TCR_TXCTR_Msk (0xffffu << ADC_TCR_TXCTR_Pos) //!< (ADC_TCR) Transmit Counter Register */
#define ADC_TCR_TXCTR(value) ((ADC_TCR_TXCTR_Msk & ((value) << ADC_TCR_TXCTR_Pos)))	 //!< doc!
/* -------- ADC_RNPR : (ADC Offset: 0x110) Receive Next Pointer Register -------- */
#define ADC_RNPR_RXNPTR_Pos 0	 //!< doc!
#define ADC_RNPR_RXNPTR_Msk (0xffffffffu << ADC_RNPR_RXNPTR_Pos) //!< (ADC_RNPR) Receive Next Pointer */
#define ADC_RNPR_RXNPTR(value) ((ADC_RNPR_RXNPTR_Msk & ((value) << ADC_RNPR_RXNPTR_Pos)))	 //!< doc!
/* -------- ADC_RNCR : (ADC Offset: 0x114) Receive Next Counter Register -------- */
#define ADC_RNCR_RXNCTR_Pos 0	 //!< doc!
#define ADC_RNCR_RXNCTR_Msk (0xffffu << ADC_RNCR_RXNCTR_Pos) //!< (ADC_RNCR) Receive Next Counter */
#define ADC_RNCR_RXNCTR(value) ((ADC_RNCR_RXNCTR_Msk & ((value) << ADC_RNCR_RXNCTR_Pos)))	 //!< doc!
/* -------- ADC_TNPR : (ADC Offset: 0x118) Transmit Next Pointer Register -------- */
#define ADC_TNPR_TXNPTR_Pos 0	 //!< doc!
#define ADC_TNPR_TXNPTR_Msk (0xffffffffu << ADC_TNPR_TXNPTR_Pos) //!< (ADC_TNPR) Transmit Next Pointer */
#define ADC_TNPR_TXNPTR(value) ((ADC_TNPR_TXNPTR_Msk & ((value) << ADC_TNPR_TXNPTR_Pos)))	 //!< doc!
/* -------- ADC_TNCR : (ADC Offset: 0x11C) Transmit Next Counter Register -------- */
#define ADC_TNCR_TXNCTR_Pos 0	 //!< doc!
#define ADC_TNCR_TXNCTR_Msk (0xffffu << ADC_TNCR_TXNCTR_Pos) //!< (ADC_TNCR) Transmit Counter Next */
#define ADC_TNCR_TXNCTR(value) ((ADC_TNCR_TXNCTR_Msk & ((value) << ADC_TNCR_TXNCTR_Pos)))	 //!< doc!
/* -------- ADC_PTCR : (ADC Offset: 0x120) Transfer Control Register -------- */
#define ADC_PTCR_RXTEN (0x1u << 0) //!< (ADC_PTCR) Receiver Transfer Enable */
#define ADC_PTCR_RXTDIS (0x1u << 1) //!< (ADC_PTCR) Receiver Transfer Disable */
#define ADC_PTCR_TXTEN (0x1u << 8) //!< (ADC_PTCR) Transmitter Transfer Enable */
#define ADC_PTCR_TXTDIS (0x1u << 9) //!< (ADC_PTCR) Transmitter Transfer Disable */
/* -------- ADC_PTSR : (ADC Offset: 0x124) Transfer Status Register -------- */
#define ADC_PTSR_RXTEN (0x1u << 0) //!< (ADC_PTSR) Receiver Transfer Enable */
#define ADC_PTSR_TXTEN (0x1u << 8) //!< (ADC_PTSR) Transmitter Transfer Enable */

/** @} */

/*------------------------------------------------------------------------------
 *         Macros function of register access
 *------------------------------------------------------------------------------*/

#define ADC_GetModeReg( pAdc )       ((pAdc)->ADC_MR)		//!< doc!

#define ADC_StartConversion( pAdc )  ((pAdc)->ADC_CR = ADC_CR_START) //!< doc!


#define ADC_EnableChannel( pAdc, channel )    {\
            assert( channel < 16 ) ;\
            (pAdc)->ADC_CHER = (1 << (channel));\
        }		//!< doc!

#define ADC_DisableChannel(pAdc, channel)  {\
            assert( (channel) < 16 ) ;\
            (pAdc)->ADC_CHDR = (1 << (channel));\
        }		//!< doc!

#define ADC_EnableIt(pAdc, dwMode)            {\
            (pAdc)->ADC_IER = (dwMode);\
        }		//!< doc!

#define ADC_DisableIt(pAdc, dwMode)           {\
            (pAdc)->ADC_IDR = (dwMode);\
        }		//!< doc!

#define ADC_EnableTS(pAdc,dwMode)             {\
            (pAdc)->ADC_ACR |= dwMode;\
        }		//!< doc!

#define ADC_EnableDataReadyIt(pAdc)         ((pAdc)->ADC_IER = AT91C_ADC_DRDY)		//!< doc!

#define ADC_GetStatus(pAdc)                 ((pAdc)->ADC_ISR)		//!< doc!

#define ADC_GetCompareMode(pAdc)            (((pAdc)->ADC_EMR)& (ADC_EMR_CMPMODE_Msk))		//!< doc!

#define ADC_GetChannelStatus(pAdc)          ((pAdc)->ADC_CHSR)		//!< doc!

#define ADC_GetInterruptMaskStatus(pAdc)    ((pAdc)->ADC_IMR)		//!< doc!

#define ADC_GetLastConvertedData(pAdc)      ((pAdc)->ADC_LCDR)		//!< doc!

/*------------------------------------------------------------------------------
 *         Exported functions
 *------------------------------------------------------------------------------*/
extern void ADC_Initialize( Adc* pAdc, uint32_t idAdc );
extern void ADC_CfgTiming( Adc* pAdc, uint32_t tracking, uint32_t settling, uint32_t transfer );
extern void ADC_cfgFrequency( Adc* pAdc, uint32_t startup, uint32_t prescal );
extern void ADC_CfgTrigering( Adc* pAdc, uint32_t trgEn, uint32_t trgSel, uint32_t freeRun );
extern void ADC_CfgLowRes( Adc* pAdc, uint32_t resolution );
extern void ADC_CfgPowerSave( Adc* pAdc, uint32_t sleep, uint32_t fwup );
extern void ADC_CfgChannelMode( Adc* pAdc, uint32_t useq, uint32_t anach );
extern void ADC_check( Adc* pAdc, uint32_t mck_freq );

extern uint32_t ADC_GetConvertedData( Adc* pAdc, uint32_t dwChannel ) ;
extern void ADC_SetCompareChannel( Adc* pAdc, uint32_t dwChannel ) ;
extern void ADC_SetCompareMode( Adc* pAdc, uint32_t dwMode ) ;
extern void ADC_SetComparisonWindow( Adc* pAdc, uint32_t dwHi_Lo ) ;
extern uint32_t ADC_IsInterruptMasked( Adc* pAdc, uint32_t dwFlag ) ;
extern uint32_t ADC_IsStatusSet( Adc* pAdc, uint32_t dwFlag ) ;
extern uint32_t ADC_IsChannelInterruptStatusSet( uint32_t adc_sr, uint32_t dwChannel ) ;
extern uint32_t ADC_ReadBuffer( Adc* pADC, int16_t *pwBuffer, uint32_t dwSize ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _ADC_ */
