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
 * @ingroup	 AT91_SPI
 * @file
 * @brief    Implementation of Serial Peripheral Interface (SPI) controller.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include <tmos.h>
#include "fam_cpp.h"
//#include "chip.h"
//#include <stdint.h>

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Enables a SPI peripheral.
 *
 * \param spi  Pointer to an Spi instance.
 */
extern void SPI_Enable( Spi* spi )
{
    spi->SPI_CR = SPI_CR_SPIEN ;
}

/**
 * \brief Disables a SPI peripheral.
 *
 * \param spi  Pointer to an Spi instance.
 */
extern void SPI_Disable( Spi* spi )
{
    spi->SPI_CR = SPI_CR_SPIDIS ;
}

/**
 * \brief Enables one or more interrupt sources of a SPI peripheral.
 *
 * \param spi  Pointer to an Spi instance.
 * \param dwSources Bitwise OR of selected interrupt sources.
 */
extern void SPI_EnableIt( Spi* spi, uint32_t dwSources )
{
    spi->SPI_IER = dwSources ;
}

/**
 * \brief Disables one or more interrupt sources of a SPI peripheral.
 *
 * \param spi  Pointer to an Spi instance.
 * \param dwSources Bitwise OR of selected interrupt sources.
 */
extern void SPI_DisableIt( Spi* spi, uint32_t dwSources )
{
    spi->SPI_IDR = dwSources ;
}

/**
 * \brief Configures a SPI peripheral as specified. The configuration can be computed
 * using several macros (see \ref spi_configuration_macros).
 *
 * \param spi  Pointer to an Spi instance.
 * \param dwId   Peripheral ID of the SPI.
 * \param dwConfiguration  Value of the SPI configuration register.
 */
extern void SPI_Configure( Spi* spi, uint32_t dwId, uint32_t dwConfiguration )
{
    PMC_EnablePeripheral( dwId ) ;
    spi->SPI_CR = SPI_CR_SPIDIS ;

    /* Execute a software reset of the SPI twice */
    spi->SPI_CR = SPI_CR_SWRST ;
    spi->SPI_CR = SPI_CR_SWRST ;
    spi->SPI_MR = dwConfiguration ;
}


/**
 * \brief Configures a chip select of a SPI peripheral. The chip select configuration
 * is computed using several macros (see \ref spi_configuration_macros).
 *
 * \param spi   Pointer to an Spi instance.
 * \param dwNpcs  Chip select to configure (0, 1, 2 or 3).
 * \param dwConfiguration  Desired chip select configuration.
 */
void SPI_ConfigureNPCS( Spi* spi, uint32_t dwNpcs, uint32_t dwConfiguration )
{
    spi->SPI_CSR[dwNpcs] = dwConfiguration ;
}

/**
 * \brief Get the current status register of the given SPI peripheral.
 * \note This resets the internal value of the status register, so further
 * read may yield different values.
 * \param spi   Pointer to a Spi instance.
 * \return  SPI status register.
 */
extern uint32_t SPI_GetStatus( Spi* spi )
{
    return spi->SPI_SR ;
}

/**
 * \brief Reads and returns the last word of data received by a SPI peripheral. This
 * method must be called after a successful SPI_Write call.
 *
 * \param spi  Pointer to an Spi instance.
 *
 * \return readed data.
 */
extern uint32_t SPI_Read( Spi* spi )
{
    while ( (spi->SPI_SR & SPI_SR_RDRF) == 0 ) ;

    return spi->SPI_RDR & 0xFFFF ;
}

/**
 * \brief Sends data through a SPI peripheral. If the SPI is configured to use a fixed
 * peripheral select, the npcs value is meaningless. Otherwise, it identifies
 * the component which shall be addressed.
 *
 * \param spi   Pointer to an Spi instance.
 * \param dwNpcs  Chip select of the component to address (0, 1, 2 or 3).
 * \param wData  Word of data to send.
 */
extern void SPI_Write( Spi* spi, uint32_t dwNpcs, uint16_t wData )
{
    /* Send data */
    while ( (spi->SPI_SR & SPI_SR_TXEMPTY) == 0 ) ;
    spi->SPI_TDR = wData | SPI_PCS( dwNpcs ) ;
    while ( (spi->SPI_SR & SPI_SR_TDRE) == 0 ) ;
}

/**
 * \brief Check if SPI transfer finish.
 *
 * \param spi  Pointer to an Spi instance.
 *
 * \return Returns 1 if there is no pending write operation on the SPI; otherwise
 * returns 0.
 */
extern uint32_t SPI_IsFinished( Spi* spi )
{
    return ((spi->SPI_SR & SPI_SR_TXEMPTY) != 0) ;
}

/**
 * \brief Enable Spi PDC transmit
 * \param spi  Pointer to an Spi instance.
*/
extern void SPI_PdcEnableTx( Spi* spi )
{
    spi->SPI_PTCR = SPI_PTCR_TXTEN ;
}

/**
 * \brief Disable Spi PDC transmit
 * \param spi  Pointer to an Spi instance.
*/
extern void SPI_PdcDisableTx( Spi* spi )
{
    spi->SPI_PTCR = SPI_PTCR_TXTDIS ;
}

/**
 * \brief Enable Spi PDC receive
 * \param spi  Pointer to an Spi instance.
*/
extern void SPI_PdcEnableRx( Spi* spi )
{
    spi->SPI_PTCR = SPI_PTCR_RXTEN ;
}

/**
 * \brief Disable Spi PDC receive
 * \param spi  Pointer to an Spi instance.
*/
extern void SPI_PdcDisableRx( Spi* spi )
{
    spi->SPI_PTCR = SPI_PTCR_RXTDIS ;
}

/**
 * \brief Set PDC transmit and next transmit buffer address and size.
 *
 * \param spi    Pointer to an Spi instance.
 * \param pvTxBuf  PDC transmit buffer address.
 * \param dwTxCount  Length in bytes of the transmit buffer.
 * \param pvTxNextBuf  PDC next transmit buffer address.
 * \param dwTxNextCount  Length in bytes of the next transmit buffer.
 */
extern void SPI_PdcSetTx( Spi* spi, void* pvTxBuf, uint32_t dwTxCount, void* pvTxNextBuf, uint32_t dwTxNextCount )
{
    spi->SPI_TPR = (uint32_t)pvTxBuf ;
    spi->SPI_TCR = dwTxCount ;
    spi->SPI_TNPR = (uint32_t)pvTxNextBuf ;
    spi->SPI_TNCR = dwTxNextCount ;
}

/**
 * \brief Set PDC receive and next receive buffer address and size.
 *
 * \param spi    Pointer to an Spi instance.
 * \param pvRxBuf  PDC receive buffer address.
 * \param dwRxCount  Length in bytes of the receive buffer.
 * \param pvRxNextBuf  PDC next receive buffer address.
 * \param dwRxNextCount  Length in bytes of the next receive buffer.
 */
extern void SPI_PdcSetRx( Spi* spi, void* pvRxBuf, uint32_t dwRxCount, void* pvRxNextBuf, uint32_t dwRxNextCount )
{
    spi->SPI_RPR = (uint32_t)pvRxBuf ;
    spi->SPI_RCR = dwRxCount ;
    spi->SPI_RNPR = (uint32_t)pvRxNextBuf ;
    spi->SPI_RNCR = dwRxNextCount ;
}

/**
 * \brief Sends the contents of buffer through a SPI peripheral, using the PDC to
 * take care of the transfer.
 *
 * \param spi     Pointer to an Spi instance.
 * \param pvBuffer  Data buffer to send.
 * \param dwLength  Length of the data buffer.
 */
extern uint32_t SPI_WriteBuffer( Spi* spi, void* pvBuffer, uint32_t dwLength )
{
    /* Check if first bank is free */
    if ( spi->SPI_TCR == 0 )
    {
        spi->SPI_TPR = (uint32_t)pvBuffer ;
        spi->SPI_TCR = dwLength ;
        spi->SPI_PTCR = PERIPH_PTCR_TXTEN ;

        return 1 ;
    }
    /* Check if second bank is free */
    else
    {
        if ( spi->SPI_TNCR == 0 )
        {
            spi->SPI_TNPR = (uint32_t)pvBuffer ;
            spi->SPI_TNCR = dwLength ;

            return 1 ;
        }
    }

    /* No free banks */
    return 0 ;
}

/**
 * \brief Reads data from a SPI peripheral until the provided buffer is filled. This
 * method does NOT need to be called after SPI_Write or SPI_WriteBuffer.
 *
 * \param spi     Pointer to an Spi instance.
 * \param pvBuffer  Data buffer to store incoming bytes.
 * \param dwLength  Length in bytes of the data buffer.
 */
extern uint32_t SPI_ReadBuffer( Spi* spi, void *pvBuffer, uint32_t dwLength )
{
    /* Check if the first bank is free */
    if ( spi->SPI_RCR == 0 )
    {
        spi->SPI_RPR = (uint32_t)pvBuffer ;
        spi->SPI_RCR = dwLength ;
        spi->SPI_PTCR = PERIPH_PTCR_RXTEN ;

        return 1 ;
    }
    /* Check if second bank is free */
    else
    {
        if ( spi->SPI_RNCR == 0 )
        {
            spi->SPI_RNPR = (uint32_t)pvBuffer ;
            spi->SPI_RNCR = dwLength ;
            return 1 ;
        }
    }

    /* No free bank */
    return 0 ;
}


