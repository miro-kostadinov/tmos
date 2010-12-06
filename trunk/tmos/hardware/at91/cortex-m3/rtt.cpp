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
 * @ingroup	 HW_SAM3S_RTT
 * @file
 * @brief    Implementation of Real Time Timer (RTT) controller.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
//#include "chip.h"
//#include <assert.h>
#include <tmos.h>
#include "platform_cpp.h"

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Changes the prescaler value of the given RTT and restarts it.
 *
 * \note This function disables RTT interrupt sources.
 *
 * \param rtt  Pointer to a Rtt instance.
 * \param prescaler  Prescaler value for the RTT.
 */
void RTT_SetPrescaler(Rtt *rtt, uint16_t prescaler)
{
    rtt->RTT_MR = (prescaler |  RTT_MR_RTTRST);
}

/**
 * \brief Returns the current value of the RTT timer value.
 *
 * \param rtt  Pointer to a Rtt instance.
 */
uint32_t RTT_GetTime(Rtt *rtt)
{
    return rtt->RTT_VR;
}

/**
 * \brief Enables the specified RTT interrupt sources.
 *
 * \param rtt  Pointer to a Rtt instance.
 * \param sources  Bitmask of interrupts to enable.
 */
void RTT_EnableIT(Rtt *rtt, uint32_t sources)
{
    ASSERT( (sources & 0x0004FFFF) == 0 ) ;
    rtt->RTT_MR |= sources;
}

/**
 * \brief Returns the status register value of the given RTT.
 *
 * \param rtt  Pointer to an Rtt instance.
 */
uint32_t RTT_GetStatus(Rtt *rtt)
{
    return rtt->RTT_SR;
}

/**
 * \brief Configures the RTT to generate an alarm at the given time.
 *
 * \param pRtt  Pointer to an Rtt instance.
 * \param time  Alarm time.
 */
void RTT_SetAlarm(Rtt *pRtt, uint32_t time)
{
    ASSERT(time > 0);

    pRtt->RTT_AR = time - 1;
}
