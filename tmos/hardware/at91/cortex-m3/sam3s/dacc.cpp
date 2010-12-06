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
 * @ingroup	 SAM3S_DACC
 * @file
 * @brief    Implementation of Digital-to-Analog Converter Controller (DACC).
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include <tmos.h>
#include "series_cpp.h"
//#include "chip.h"
//#include <stdint.h>
//#include <assert.h>

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
  * \brief Initialize the DACC controller
  * \param pDACC Pointer to an DACC instance.
  * \param idDACC identifier of DAC peripheral
  * \param trgEn trigger mode, free running mode or external Hardware trigger
  * \param word transfer size,word or half word
  * \param trgSel hardware trigger selection
  * \param sleepMode sleep mode selection
  * \param mck value of MCK in Hz
  * \param refresh refresh period
  * \param user_sel user channel selection ,0 or 1
  * \param tag_mode tag for channel number
  * \param startup value of the start up time (in DACCClock) (see datasheet)
*/
extern void DACC_Initialize( Dacc* pDACC,
                             uint8_t idDACC,
                             uint8_t trgEn,
                             uint8_t trgSel,
                             uint8_t word,
                             uint8_t sleepMode,
                             uint32_t mck,
                             uint8_t refresh,    /* refresh period */
                             uint8_t user_sel,   /* user channel selection */
                             uint32_t tag_mode,  /* using tag for channel number */
                             uint32_t startup
                            )
{
    assert( 1024*refresh*1000/(mck>>1) < 20 ) ;

    /* Enable peripheral clock*/
    PMC->PMC_PCER0 = 1 << idDACC;

    /*  Reset the controller */
    DACC_SoftReset(pDACC);

    /*  Write to the MR register */
    DACC_CfgModeReg( pDACC,
          ( (trgEn<<0) & DACC_MR_TRGEN)
        |   DACC_MR_TRGSEL(trgSel)
        | ( (word<<4) & DACC_MR_WORD)
        | ( (sleepMode<<5) & DACC_MR_SLEEP)
        |   DACC_MR_REFRESH(refresh)
        | ( (user_sel<<DACC_MR_USER_SEL_Pos)& DACC_MR_USER_SEL_Msk)
        | ( (tag_mode<<20) &  DACC_MR_TAG)
        | ( (startup<<DACC_MR_STARTUP_Pos) & DACC_MR_STARTUP_Msk));
}


/**
 * Set the Conversion Data
 * \param pDACC Pointer to an Dacc instance.
 * \param dwData  date to be converted.
 */
extern void DACC_SetConversionData( Dacc* pDACC, uint32_t dwData )
{
    uint32_t dwMR = pDACC->DACC_MR ;

    if ( dwMR & DACC_MR_WORD )
    {
    	pDACC->DACC_CDR = dwData ;
    }
    else
    {
    	pDACC->DACC_CDR = (dwData&0xFFFF) ;
    }
}


/**
  * \brief Write converted data through PDC channel
  * \param pDACC the pointer of DACC peripheral
  * \param pwBuffer the destination buffer
  * \param dwSize the size of the buffer
*/
extern uint32_t DACC_WriteBuffer( Dacc* pDACC, uint16_t *pwBuffer, uint32_t dwSize )
{

    /* Check if the first PDC bank is free*/
    if ( (pDACC->DACC_TCR == 0) && (pDACC->DACC_TNCR == 0) )
	{
        pDACC->DACC_TPR = (uint32_t)pwBuffer ;
        pDACC->DACC_TCR = dwSize ;
        pDACC->DACC_PTCR = DACC_PTCR_TXTEN ;

        return 1 ;
    }
    /* Check if the second PDC bank is free*/
    else
	{
	    if (pDACC->DACC_TNCR == 0)
	    {
            pDACC->DACC_TNPR = (uint32_t)pwBuffer ;
            pDACC->DACC_TNCR = dwSize ;

            return 1 ;
        }
        else
		{
            return 0 ;
		}
    }

}


