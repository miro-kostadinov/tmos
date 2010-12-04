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
 * @ingroup	 HW_SAM3S_WDT
 * @file
 * @brief    Implementation of Watchdog Timer (WDT) controller.
 *
 */

/*---------------------------------------------------------------------------
 *        Headers
 *---------------------------------------------------------------------------*/

//#include "chip.h"
//#include <stdint.h>

#include <tmos.h>
#include "platform_cpp.h"

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Enable watchdog with given mode.
 *
 * \note The Watchdog Mode Register (WDT_MR) can be written only once.
 * Only a processor reset resets it.
 *
 * @param pWDT
 * \param dwMode   WDT mode to be set
 */
extern void WDT_Enable( Wdt* pWDT, uint32_t dwMode )
{
    pWDT->WDT_MR = dwMode ;
}

/**
 * \brief Disable watchdog.
 *
 * \note The Watchdog Mode Register (WDT_MR) can be written only once.
 * Only a processor reset resets it.
 *
 * @param pWDT
 */
extern void WDT_Disable( Wdt* pWDT )
{
    pWDT->WDT_MR = WDT_MR_WDDIS;
}

/**
 * \brief Watchdog restart.
 */
extern void WDT_Restart( Wdt* pWDT )
{
    pWDT->WDT_CR = 0xA5000001;
}

/**
 * \brief Watchdog get status.
 */
extern uint32_t WDT_GetStatus( Wdt* pWDT )
{
    return (pWDT->WDT_SR & 0x3) ;
}

/**
 * \brief Watchdog get period.
 *
 * \param dwMs   desired watchdog period in millisecond.
 */
extern uint32_t WDT_GetPeriod( uint32_t dwMs )
{
    if ( (dwMs < 4) || (dwMs > 16000) )
    {
        return 0 ;
    }
    return ((dwMs << 8) / 1000) ;
}
