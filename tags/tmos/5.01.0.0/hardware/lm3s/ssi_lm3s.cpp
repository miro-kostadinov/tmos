/**************************************************************************//**
 * @ingroup	 lm3s_ssi
 * @{
 * @file     ssi_lm3s.cpp
 * @brief    SSI class functions
 * @version  V3.00
 * @date     18. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#include <tmos.h>
#include "fam_cpp.h"

//*****************************************************************************
//
//! Configures the synchronous serial interface.
//!
//! \param ulSSIClk is the rate of the clock supplied to the SSI module.
//! \param ulProtocol specifies the data transfer protocol.
//! \param ulMode specifies the mode of operation.
//! \param ulBitRate specifies the clock rate.
//! \param ulDataWidth specifies number of bits transferred per frame.
//!
//! This function configures the synchronous serial interface.  It sets
//! the SSI protocol, mode of operation, bit rate, and data width.
//!
//! The \e ulProtocol parameter defines the data frame format.  The
//! \e ulProtocol parameter can be one of the following values:
//! \b SSI_FRF_MOTO_MODE_0, \b SSI_FRF_MOTO_MODE_1, \b SSI_FRF_MOTO_MODE_2,
//! \b SSI_FRF_MOTO_MODE_3, \b SSI_FRF_TI, or \b SSI_FRF_NMW.  The Motorola
//! frame formats imply the following polarity and phase configurations:
//!
//! <pre>
//! Polarity Phase       Mode
//!   0       0   SSI_FRF_MOTO_MODE_0
//!   0       1   SSI_FRF_MOTO_MODE_1
//!   1       0   SSI_FRF_MOTO_MODE_2
//!   1       1   SSI_FRF_MOTO_MODE_3
//! </pre>
//!
//! The \e ulMode parameter defines the operating mode of the SSI module.  The
//! SSI module can operate as a master or slave; if a slave, the SSI can be
//! configured to disable output on its serial output line.  The \e ulMode
//! parameter can be one of the following values: \b SSI_MODE_MASTER,
//! \b SSI_MODE_SLAVE, or \b SSI_MODE_SLAVE_OD.
//!
//! The \e ulBitRate parameter defines the bit rate for the SSI.  This bit rate
//! must satisfy the following clock ratio criteria:
//!
//! - FSSI >= 2 * bit rate (master mode)
//! - FSSI >= 12 * bit rate (slave modes)
//!
//! where FSSI is the frequency of the clock supplied to the SSI module.
//!
//! The \e ulDataWidth parameter defines the width of the data transfers, and
//! can be a value between 4 and 16, inclusive.
//!
//! The peripheral clock will be the same as the processor clock.  This will be
//! the value returned by SysCtlClockGet(), or it can be explicitly hard coded
//! if it is constant and known (to save the code/execution overhead of a call
//! to SysCtlClockGet()).
//!
//! This function replaces the original SSIConfig() API and performs the same
//! actions.  A macro is provided in <tt>ssi.h</tt> to map the original API to
//! this API.
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIConfigSetExpClk(unsigned long ulSSIClk,
					    unsigned long ulProtocol, unsigned long ulMode,
					    unsigned long ulBitRate, unsigned long ulDataWidth)
{
    unsigned long ulMaxBitRate;
    unsigned long ulRegVal;
    unsigned long ulPreDiv;
    unsigned long ulSCR;
    unsigned long ulSPH_SPO;

    ASSERT((ulProtocol == SSI_FRF_MOTO_MODE_0) ||
           (ulProtocol == SSI_FRF_MOTO_MODE_1) ||
           (ulProtocol == SSI_FRF_MOTO_MODE_2) ||
           (ulProtocol == SSI_FRF_MOTO_MODE_3) ||
           (ulProtocol == SSI_FRF_TI) ||
           (ulProtocol == SSI_FRF_NMW));
    ASSERT((ulMode == SSI_MODE_MASTER) ||
           (ulMode == SSI_MODE_SLAVE) ||
           (ulMode == SSI_MODE_SLAVE_OD));
    ASSERT(((ulMode == SSI_MODE_MASTER) && (ulBitRate <= (ulSSIClk / 2))) ||
           ((ulMode != SSI_MODE_MASTER) && (ulBitRate <= (ulSSIClk / 12))));
    ASSERT((ulSSIClk / ulBitRate) <= (254 * 256));
    ASSERT((ulDataWidth >= 4) && (ulDataWidth <= 16));

    // Set the mode.
    ulRegVal = (ulMode == SSI_MODE_SLAVE_OD) ? SSI_CR1_SOD : 0;
    ulRegVal |= (ulMode == SSI_MODE_MASTER) ? 0 : SSI_CR1_MS;
    this->CR1 = ulRegVal;
    // Set the clock predivider.
    ulMaxBitRate = ulSSIClk / ulBitRate;
    ulPreDiv = 0;
    do
    {
        ulPreDiv += 2;
        ulSCR = (ulMaxBitRate / ulPreDiv) - 1;
    }
    while(ulSCR > 255);
    this->CPSR = ulPreDiv;
    // Set protocol and clock rate.
    ulSPH_SPO = (ulProtocol & 3) << 6;
    ulProtocol &= SSI_CR0_FRF_M;
    ulRegVal = (ulSCR << 8) | ulSPH_SPO | ulProtocol | (ulDataWidth - 1);
    this->CR0 = ulRegVal;
}

//*****************************************************************************
//
//! Enables the synchronous serial interface.
//!
//!
//! This will enable operation of the synchronous serial interface.  It must be
//! configured before it is enabled.
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIEnable(void)
{
    this->CR1 |= SSI_CR1_SSE;
}

//*****************************************************************************
//
//! Disables the synchronous serial interface.
//!
//!
//! This will disable operation of the synchronous serial interface.
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIDisable(void)
{
    this->CR1 &= ~(SSI_CR1_SSE);
}

//*****************************************************************************
//
//! Enables individual SSI interrupt sources.
//!
//! \param ulIntFlags is a bit mask of the interrupt sources to be enabled.
//!
//! Enables the indicated SSI interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.  The \e ulIntFlags parameter can be any of the
//! \b SSI_TXFF, \b SSI_RXFF, \b SSI_RXTO, or \b SSI_RXOR values.
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIIntEnable(unsigned long ulIntFlags)
{
    this->IM |= ulIntFlags;
}

//*****************************************************************************
//
//! Disables individual SSI interrupt sources.
//!
//! \param ulIntFlags is a bit mask of the interrupt sources to be disabled.
//!
//! Disables the indicated SSI interrupt sources.  The \e ulIntFlags parameter
//! can be any of the \b SSI_TXFF, \b SSI_RXFF, \b SSI_RXTO, or \b SSI_RXOR
//! values.
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIIntDisable(unsigned long ulIntFlags)
{
    this->IM &= ~(ulIntFlags);
}

//*****************************************************************************
//
//! Gets the current interrupt status.
//!
//! \param bMasked is \b false if the raw interrupt status is required and
//! \b true if the masked interrupt status is required.
//!
//! This returns the interrupt status for the SSI module.  Either the raw
//! interrupt status or the status of interrupts that are allowed to reflect to
//! the processor can be returned.
//!
//! \return The current interrupt status, enumerated as a bit field of
//! \b SSI_TXFF, \b SSI_RXFF, \b SSI_RXTO, and \b SSI_RXOR.
//
//*****************************************************************************
unsigned long SSI_Type::SSIIntStatus(int bMasked)
{
    if(bMasked)
        return(this->MIS);
    return(this->RIS);
}

//*****************************************************************************
//
//! Clears SSI interrupt sources.
//!
//! \param ulIntFlags is a bit mask of the interrupt sources to be cleared.
//!
//! The specified SSI interrupt sources are cleared, so that
//! they no longer assert.  This must be done in the interrupt handler to
//! keep it from being called again immediately upon exit.
//! The \e ulIntFlags parameter can consist of either or both the \b SSI_RXTO
//! and \b SSI_RXOR values.
//!
//! \note Since there is a write buffer in the Cortex-M3 processor, it may take
//! several clock cycles before the interrupt source is actually cleared.
//! Therefore, it is recommended that the interrupt source be cleared early in
//! the interrupt handler (as opposed to the very last action) to avoid
//! returning from the interrupt handler before the interrupt source is
//! actually cleared.  Failure to do so may result in the interrupt handler
//! being immediately reentered (since NVIC still sees the interrupt source
//! asserted).
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIIntClear(unsigned long ulIntFlags)
{
    this->ICR = ulIntFlags;
}

//*****************************************************************************
//
//! Puts a data element into the SSI transmit FIFO.
//!
//! \param ulData data to be transmitted over the SSI interface.
//!
//! This function will place the supplied data into the transmit FIFO of
//! the specified SSI module.
//!
//! \note The upper 32 - N bits of the \e ulData will be discarded by the
//! hardware, where N is the data width as configured by SSIConfigSetExpClk().
//! For example, if the interface is configured for 8-bit data width, the upper
//! 24 bits of \e ulData will be discarded.
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIDataPut(unsigned long ulData)
{
    ASSERT((ulData & (0xfffffffe << (this->CR0 & SSI_CR0_DSS_M))) == 0);

    while(!(this->SR & SSI_SR_TNF))
    {
    }
    this->DR = ulData;
}

//*****************************************************************************
//
//! Puts a data element into the SSI transmit FIFO.
//!
//! \param ulData data to be transmitted over the SSI interface.
//!
//! This function will place the supplied data into the transmit FIFO of
//! the specified SSI module.  If there is no space in the FIFO, then this
//! function will return a zero.
//!
//! This function replaces the original SSIDataNonBlockingPut() API and
//! performs the same actions.  A macro is provided in <tt>ssi.h</tt> to map
//! the original API to this API.
//!
//! \note The upper 32 - N bits of the \e ulData will be discarded by the
//! hardware, where N is the data width as configured by SSIConfigSetExpClk().
//! For example, if the interface is configured for 8-bit data width, the upper
//! 24 bits of \e ulData will be discarded.
//!
//! \return Returns the number of elements written to the SSI transmit FIFO.
//
//*****************************************************************************
long SSI_Type::SSIDataPutNonBlocking( unsigned long ulData)
{
    ASSERT((ulData & (0xfffffffe << (this->CR0 & SSI_CR0_DSS_M))) == 0);

    if(this->SR & SSI_SR_TNF)
    {
        this->DR = ulData;
        return(true);
    }
    else
    {
        return(false);
    }
}

//*****************************************************************************
//
//! Gets a data element from the SSI receive FIFO.
//!
//! \param pulData pointer to a storage location for data that was received
//! over the SSI interface.
//!
//! This function will get received data from the receive FIFO of the specified
//! SSI module, and place that data into the location specified by the
//! \e pulData parameter.
//!
//! \note Only the lower N bits of the value written to \e pulData will contain
//! valid data, where N is the data width as configured by
//! SSIConfigSetExpClk().  For example, if the interface is configured for
//! 8-bit data width, only the lower 8 bits of the value written to \e pulData
//! will contain valid data.
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIDataGet(unsigned long *pulData)
{
    while(!(this->SR & SSI_SR_RNE))
    {
    }
    *pulData = this->DR;
}

//*****************************************************************************
//
//! Gets a data element from the SSI receive FIFO.
//!
//! \param pulData pointer to a storage location for data that was received
//! over the SSI interface.
//!
//! This function will get received data from the receive FIFO of
//! the specified SSI module, and place that data into the location specified
//! by the \e ulData parameter.  If there is no data in the FIFO, then this
//! function will return a zero.
//!
//! This function replaces the original SSIDataNonBlockingGet() API and
//! performs the same actions.  A macro is provided in <tt>ssi.h</tt> to map
//! the original API to this API.
//!
//! \note Only the lower N bits of the value written to \e pulData will contain
//! valid data, where N is the data width as configured by
//! SSIConfigSetExpClk().  For example, if the interface is configured for
//! 8-bit data width, only the lower 8 bits of the value written to \e pulData
//! will contain valid data.
//!
//! \return Returns the number of elements read from the SSI receive FIFO.
//
//*****************************************************************************
long SSI_Type::SSIDataGetNonBlocking( unsigned long *pulData)
{
    if(this->SR & SSI_SR_RNE)
    {
        *pulData = this->DR;
        return(true);
    }
    return(false);
}

//*****************************************************************************
//
//! Enable SSI DMA operation.
//!
//! \param ulDMAFlags is a bit mask of the DMA features to enable.
//!
//! The specified SSI DMA features are enabled.  The SSI can be
//! configured to use DMA for transmit and/or receive data transfers.
//! The \e ulDMAFlags parameter is the logical OR of any of the following
//! values:
//!
//! - SSI_DMA_RX - enable DMA for receive
//! - SSI_DMA_TX - enable DMA for transmit
//!
//! \note The uDMA controller must also be set up before DMA can be used
//! with the SSI.
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIDMAEnable(unsigned long ulDMAFlags)
{
    this->DMACTL |= ulDMAFlags;
}

//*****************************************************************************
//
//! Disable SSI DMA operation.
//!
//! \param ulDMAFlags is a bit mask of the DMA features to disable.
//!
//! This function is used to disable SSI DMA features that were enabled
//! by SSIDMAEnable().  The specified SSI DMA features are disabled.  The
//! \e ulDMAFlags parameter is the logical OR of any of the following values:
//!
//! - SSI_DMA_RX - disable DMA for receive
//! - SSI_DMA_TX - disable DMA for transmit
//!
//! \return None.
//
//*****************************************************************************
void SSI_Type::SSIDMADisable(unsigned long ulDMAFlags)
{
    this->DMACTL &= ~ulDMAFlags;
}

//*****************************************************************************
//
//! Determines whether the SSI transmitter is busy or not.
//!
//!
//! Allows the caller to determine whether all transmitted bytes have cleared
//! the transmitter hardware.  If \b false is returned, then transmit FIFO is
//! empty and all bits of the last transmitted word have left the hardware
//! shift register.
//!
//! \return Returns \b true if the SSI is transmitting or \b false if all
//! transmissions are complete.
//
//*****************************************************************************
int SSI_Type::SSIBusy(void)
{
    return((this->SR & SSI_SR_BSY) ? true : false);
}

/** @} ingroup	 lm3s_ssi */
