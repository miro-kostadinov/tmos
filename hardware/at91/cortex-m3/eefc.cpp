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
 * @ingroup	 AT91_EEFC
 * @file
 * @brief    Implementation of Enhanced Embedded Flash Controller (EEFC).
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
 * \brief Enables the flash ready interrupt source on the EEFC peripheral.
 *
 * \param efc  Pointer to a Efc instance
 */
extern void EFC_EnableFrdyIt( Efc* efc )
{
    efc->EEFC_FMR |= EEFC_FMR_FRDY ;
}

/**
 * \brief Disables the flash ready interrupt source on the EEFC peripheral.
 *
 * \param efc  Pointer to a Efc instance
 */

extern void EFC_DisableFrdyIt( Efc* efc )
{
    efc->EEFC_FMR &= ~((uint32_t)EEFC_FMR_FRDY) ;
}


/**
 * \brief Set read/write wait state on the EEFC perpherial.
 *
 * \param efc  Pointer to a Efc instance
 * \param ucCycles  the number of wait states in cycle.
 */

extern void EFC_SetWaitState( Efc* efc, uint8_t ucCycles )
{
    uint32_t dwValue ;

    dwValue = efc->EEFC_FMR ;
    dwValue &= ~((uint32_t)EEFC_FMR_FWS_Msk) ;
    dwValue |= EEFC_FMR_FWS(ucCycles);
    efc->EEFC_FMR = dwValue ;
}

/**
 * \brief Returns the current status of the EEFC.
 *
 * \note Keep in mind that this function clears the value of some status bits (LOCKE, PROGE).
 *
 * \param efc  Pointer to a Efc instance
 */
extern uint32_t EFC_GetStatus( Efc* efc )
{
    return efc->EEFC_FSR ;
}

/**
 * \brief Returns the result of the last executed command.
 *
 * \param efc  Pointer to a Efc instance
 */
extern uint32_t EFC_GetResult( Efc* efc )
{
    return efc->EEFC_FRR ;
}

/**
 * \brief Translates the given address page and offset values.
 * \note The resulting values are stored in the provided variables if they are not null.
 *
 * \param ppEfc  Pointer to a Efc instance
 * \param dwAddress  Address to translate.
 * \param pwPage  First page accessed.
 * \param pwOffset  Byte offset in first page.
 */
extern void EFC_TranslateAddress( Efc** ppEfc, uint32_t dwAddress, uint16_t* pwPage, uint16_t* pwOffset )
{
    Efc *pEfc ;
    uint16_t wPage ;
    uint16_t wOffset ;

    ASSERT( dwAddress >= IFLASH_ADDR ) ;
    ASSERT( dwAddress <= (IFLASH_ADDR + IFLASH_SIZE) ) ;

    pEfc = EFC ;
    wPage = (dwAddress - IFLASH_ADDR) / IFLASH_PAGE_SIZE;
    wOffset = (dwAddress - IFLASH_ADDR) % IFLASH_PAGE_SIZE;

    TRACE_DEBUG( "Translated 0x%08X to page=%d and offset=%d\n\r", dwAddress, wPage, wOffset ) ;
    /* Store values */
    if ( pEfc )
    {
        *ppEfc = pEfc ;
    }

    if ( pwPage )
    {
        *pwPage = wPage ;
    }

    if ( pwOffset )
    {
        *pwOffset = wOffset ;
    }
}

/**
 * \brief Computes the address of a flash access given the page and offset.
 *
 * \param efc  Pointer to a Efc instance
 * \param wPage  Page number.
 * \param wOffset  Byte offset inside page.
 * \param pdwAddress  Computed address (optional).
 */
extern void EFC_ComputeAddress( Efc *efc, uint16_t wPage, uint16_t wOffset, uint32_t *pdwAddress )
{
    uint32_t dwAddress ;
	
    ASSERT( efc ) ;
    ASSERT( wPage <= IFLASH_NB_OF_PAGES ) ;
    ASSERT( wOffset < IFLASH_PAGE_SIZE ) ;
	
    /* Compute address */
    dwAddress = IFLASH_ADDR + wPage * IFLASH_PAGE_SIZE + wOffset ;
	
    /* Store result */
    if ( pdwAddress != NULL )
	{
        *pdwAddress = dwAddress ;
    }
}

/**
 * \brief Starts the executing the given command on the EEFC and returns as soon as the command is started.
 *
 * \note It does NOT set the FMCN field automatically.
 * \param efc  Pointer to a Efc instance
 * \param dwCommand  Command to execute.
 * \param dwArgument  Command argument (should be 0 if not used).
 */
extern void EFC_StartCommand( Efc* efc, uint32_t dwCommand, uint32_t dwArgument )
{
    /* Check command & argument */
    switch ( dwCommand )
    {
        case EFC_FCMD_WP:
        case EFC_FCMD_WPL:
        case EFC_FCMD_EWP:
        case EFC_FCMD_EWPL:
        case EFC_FCMD_SLB:
        case EFC_FCMD_CLB:
            ASSERT( dwArgument < IFLASH_NB_OF_PAGES ) ;
        break ;

        case EFC_FCMD_SFB:
        case EFC_FCMD_CFB:
            ASSERT( dwArgument < 2 ) ;
        break;

        case EFC_FCMD_GETD:
        case EFC_FCMD_EA:
        case EFC_FCMD_GLB:
        case EFC_FCMD_GFB:
        case EFC_FCMD_STUI:
            ASSERT( dwArgument == 0 ) ;
        break;

        default: ASSERT( 0 ) ;
    }

    /* Start command Embedded flash */
    ASSERT( (efc->EEFC_FSR & EEFC_FMR_FRDY) == EEFC_FMR_FRDY ) ;
    efc->EEFC_FCR = EEFC_FCR_FKEY(0x5A) | EEFC_FCR_FARG(dwArgument) | EEFC_FCR_FCMD(dwCommand) ;
}

/**
 * \brief Performs the given command and wait until its completion (or an error).
 *
 * \param efc  Pointer to a Efc instance
 * \param dwCommand  Command to perform.
 * \param dwArgument  Optional command argument.
 * \param dwUseIAP
 *
 * \return 0 if successful, otherwise returns an error code.
 */

extern uint32_t EFC_PerformCommand( Efc* efc, uint32_t dwCommand, uint32_t dwArgument, uint32_t dwUseIAP )
{
    if ( dwUseIAP != 0 )
    {
        /* Pointer on IAP function in ROM */
        static uint32_t (*IAP_PerformCommand)( uint32_t, uint32_t ) ;

        IAP_PerformCommand = (uint32_t (*)( uint32_t, uint32_t )) *((uint32_t*)CHIP_FLASH_IAP_ADDRESS ) ;
        IAP_PerformCommand( 0, EEFC_FCR_FKEY(0x5A) | EEFC_FCR_FARG(dwArgument) | EEFC_FCR_FCMD(dwCommand) ) ;

        return (efc->EEFC_FSR & (EEFC_FSR_FLOCKE | EEFC_FSR_FCMDE)) ;
    }
    else
    {
        uint32_t dwStatus ;

        efc->EEFC_FCR = EEFC_FCR_FKEY(0x5A) | EEFC_FCR_FARG(dwArgument) | EEFC_FCR_FCMD(dwCommand) ;
        do
        {
            dwStatus = efc->EEFC_FSR ;
        }
        while ( (dwStatus & EEFC_FSR_FRDY) != EEFC_FSR_FRDY ) ;

        return ( dwStatus & (EEFC_FSR_FLOCKE | EEFC_FSR_FCMDE) ) ;
    }
}


