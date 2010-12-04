/**************************************************************************//**
 * @ingroup	 lm3s_uart
 * @{
 * @file     uart_lm3s.cpp
 * @brief    UART class methods
 * @version  V3.00
 * @date     15. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/
#include <tmos.h>
#include "fam_cpp.h"

/** The system clock divider defining the maximum baud rate supported by the UART.
 *
 */
#define UART_CLK_DIVIDER    ((CLASS_IS_SANDSTORM ||                     \
                            (CLASS_IS_FURY && REVISION_IS_A2) ||        \
                            (CLASS_IS_DUSTDEVIL && REVISION_IS_A0)) ?   \
                                 16 : 8)

/*****************************************************************************
 *
 *  Get UART peripheral value
 *
 *
 *
 *
 *  NOTE: Use Driver Info instead!!
 *
 *
 *
 *
 *
 *  return values that can be passed to the
 *			SysCtlPeripheralPresent(), SysCtlPeripheralEnable(),
 *			SysCtlPeripheralDisable(), and SysCtlPeripheralReset() APIs as the
 *			ulPeripheral parameter.
 *
 */
//unsigned long  UART_Type::UARTGetPeripheral(void)
//{
//	if( this == UART0 )
//		return SYSCTL_PERIPH_UART0;
//	if( this == UART1 )
//		return SYSCTL_PERIPH_UART1;
//	if( this == UART2 )
//		return SYSCTL_PERIPH_UART2;
//	return 0;
//}

/**
 * DMA_GetRxChannel
 * @return
 */
unsigned long UART_Type::DMA_GetRxChannel(void)
{
	if( this == UART0 )
		return UDMA_CHANNEL_UART0RX;
	if( this == UART1 )
		return UDMA_CHANNEL_UART1RX;
	if( this == UART2 )
		return UDMA_CHANNEL_UART2RX;
	return 0;
}

/**DMA_GetTxChannel
 *
 * @return
 */
unsigned long UART_Type::DMA_GetTxChannel(void)
{
	if( this == UART0 )
		return UDMA_CHANNEL_UART0TX;
	if( this == UART1 )
		return UDMA_CHANNEL_UART1TX;
	if( this == UART2 )
		return UDMA_CHANNEL_UART2TX;
	return 0;
}
/*
 *
 *  Reset UART peripheral
 *
 */

//void UART_Type::UARTReset(void)
//{
//	SYSCTL->SysCtlPeripheralReset(this->UARTGetPeripheral());
//}



/**
*
* Sets the type of parity.
*
* \param ulParity specifies the type of parity to use.
*
* Sets the type of parity to use for transmitting and expect when receiving.
* The \e ulParity parameter must be one of \b UART_CONFIG_PAR_NONE,
* \b UART_CONFIG_PAR_EVEN, \b UART_CONFIG_PAR_ODD, \b UART_CONFIG_PAR_ONE,
* or \b UART_CONFIG_PAR_ZERO.  The last two allow direct control of the
* parity bit; it will always be either be one or zero based on the mode.
*
* \return None.
*
*/
void UART_Type::UARTParityModeSet(unsigned long ulParity)
{
    ASSERT((ulParity == UART_CONFIG_PAR_NONE) ||
           (ulParity == UART_CONFIG_PAR_EVEN) ||
           (ulParity == UART_CONFIG_PAR_ODD) ||
           (ulParity == UART_CONFIG_PAR_ONE) ||
           (ulParity == UART_CONFIG_PAR_ZERO));

    this->LCRH = ((this->LCRH & ~(UART_LCRH_SPS|UART_LCRH_EPS|UART_LCRH_PEN)) | ulParity);
}

/**
*
* Gets the type of parity currently being used.
*
* This function gets the type of parity used for transmitting data, and
* expected when receiving data.
*
* \return Returns the current parity settings, specified as one of
* \b UART_CONFIG_PAR_NONE, \b UART_CONFIG_PAR_EVEN, \b UART_CONFIG_PAR_ODD,
* \b UART_CONFIG_PAR_ONE, or \b UART_CONFIG_PAR_ZERO.
*
*/
unsigned long UART_Type::UARTParityModeGet(void)
{
    return(this->LCRH & (UART_LCRH_SPS | UART_LCRH_EPS | UART_LCRH_PEN));
}

/**
*
* Sets the FIFO level at which interrupts are generated.
*
* \param ulTxLevel is the transmit FIFO interrupt level, specified as one of
* \b UART_FIFO_TX1_8, \b UART_FIFO_TX2_8, \b UART_FIFO_TX4_8,
* \b UART_FIFO_TX6_8, or \b UART_FIFO_TX7_8.
* \param ulRxLevel is the receive FIFO interrupt level, specified as one of
* \b UART_FIFO_RX1_8, \b UART_FIFO_RX2_8, \b UART_FIFO_RX4_8,
* \b UART_FIFO_RX6_8, or \b UART_FIFO_RX7_8.
*
* This function sets the FIFO level at which transmit and receive interrupts
* will be generated.
*
* \return None.
*
*/
void UART_Type::UARTFIFOLevelSet(unsigned long ulTxLevel, unsigned long ulRxLevel)
{
    ASSERT((ulTxLevel == UART_FIFO_TX1_8) ||
           (ulTxLevel == UART_FIFO_TX2_8) ||
           (ulTxLevel == UART_FIFO_TX4_8) ||
           (ulTxLevel == UART_FIFO_TX6_8) ||
           (ulTxLevel == UART_FIFO_TX7_8));
    ASSERT((ulRxLevel == UART_FIFO_RX1_8) ||
           (ulRxLevel == UART_FIFO_RX2_8) ||
           (ulRxLevel == UART_FIFO_RX4_8) ||
           (ulRxLevel == UART_FIFO_RX6_8) ||
           (ulRxLevel == UART_FIFO_RX7_8));

    this->IFLS = ulTxLevel | ulRxLevel;
}

/**
*
* Gets the FIFO level at which interrupts are generated.
*
* \param pulTxLevel is a pointer to storage for the transmit FIFO level,
* returned as one of \b UART_FIFO_TX1_8, \b UART_FIFO_TX2_8,
* \b UART_FIFO_TX4_8, \b UART_FIFO_TX6_8, or UART_FIFO_TX7_8.
* \param pulRxLevel is a pointer to storage for the receive FIFO level,
* returned as one of \b UART_FIFO_RX1_8, \b UART_FIFO_RX2_8,
* \b UART_FIFO_RX4_8, \b UART_FIFO_RX6_8, or \b UART_FIFO_RX7_8.
*
* This function gets the FIFO level at which transmit and receive interrupts
* will be generated.
*
* \return None.
*
*/
void UART_Type::UARTFIFOLevelGet( unsigned long *pulTxLevel, unsigned long *pulRxLevel )
{
    unsigned long ulTemp;
    ulTemp = this->IFLS;
    *pulTxLevel = ulTemp & UART_IFLS_TX_M;
    *pulRxLevel = ulTemp & UART_IFLS_RX_M;
}

/**
*
* Sets the configuration of a UART.
*
* \param ulUARTClk is the rate of the clock supplied to the UART module.
* \param ulBaud is the desired baud rate.
* \param ulConfig is the data format for the port (number of data bits,
* number of stop bits, and parity).
*
* This function will configure the UART for operation in the specified data
* format.  The baud rate is provided in the \e ulBaud parameter and the data
* format in the \e ulConfig parameter.
*
* The \e ulConfig parameter is the logical OR of three values: the number of
* data bits, the number of stop bits, and the parity.  \b UART_CONFIG_WLEN_8,
* \b UART_CONFIG_WLEN_7, \b UART_CONFIG_WLEN_6, and \b UART_CONFIG_WLEN_5
* select from eight to five data bits per byte (respectively).
* \b UART_CONFIG_STOP_ONE and \b UART_CONFIG_STOP_TWO select one or two stop
* bits (respectively).  \b UART_CONFIG_PAR_NONE, \b UART_CONFIG_PAR_EVEN,
* \b UART_CONFIG_PAR_ODD, \b UART_CONFIG_PAR_ONE, and \b UART_CONFIG_PAR_ZERO
* select the parity mode (no parity bit, even parity bit, odd parity bit,
* parity bit always one, and parity bit always zero, respectively).
*
* The peripheral clock will be the same as the processor clock.  This will be
* the value returned by SysCtlClockGet(), or it can be explicitly hard coded
* if it is constant and known (to save the code/execution overhead of a call
* to SysCtlClockGet()).
*
* This function replaces the original UARTConfigSet() API and performs the
* same actions.  A macro is provided in <tt>uart.h</tt> to map the original
* API to this API.
*
* \return None.
*
*/
void UART_Type::UARTConfigSetExpClk(unsigned long ulUARTClk, unsigned long ulBaud, unsigned long ulConfig)
{
    unsigned long ulDiv;
    ASSERT(ulBaud != 0);
    ASSERT(ulUARTClk >= (ulBaud * UART_CLK_DIVIDER));

    this->UARTDisable();

    if((ulBaud * 16) > ulUARTClk)
    {
        this->CTL |= UART_CTL_HSE;
        ulBaud /= 2;
    }
    else
    {
        this->CTL &= ~(UART_CTL_HSE);
    }

    ulDiv = (((ulUARTClk * 8) / ulBaud) + 1) / 2;
    this->IBRD = ulDiv / 64;
    this->FBRD = ulDiv % 64;

    this->LCRH = ulConfig;

//    this->FR = 0;
    this->UARTEnable();
}

/**
*
* Gets the current configuration of a UART.
*
* \param ulUARTClk is the rate of the clock supplied to the UART module.
* \param pulBaud is a pointer to storage for the baud rate.
* \param pulConfig is a pointer to storage for the data format.
*
* The baud rate and data format for the UART is determined, given an
* explicitly provided peripheral clock (hence the ExpClk suffix).  The
* returned baud rate is the actual baud rate; it may not be the exact baud
* rate requested or an ``official'' baud rate.  The data format returned in
* \e pulConfig is enumerated the same as the \e ulConfig parameter of
* UARTConfigSetExpClk().
*
* The peripheral clock will be the same as the processor clock.  This will be
* the value returned by SysCtlClockGet(), or it can be explicitly hard coded
* if it is constant and known (to save the code/execution overhead of a call
* to SysCtlClockGet()).
*
* This function replaces the original UARTConfigGet() API and performs the
* same actions.  A macro is provided in <tt>uart.h</tt> to map the original
* API to this API.
*
* \return None.
*
*/
void UART_Type::UARTConfigGetExpClk(unsigned long ulUARTClk, unsigned long *pulBaud, unsigned long *pulConfig)
{
    unsigned long ulInt, ulFrac;
    ulInt = this->IBRD;
    ulFrac = this->FBRD;
    *pulBaud = (ulUARTClk * 4) / ((64 * ulInt) + ulFrac);
    if(this->CTL & UART_CTL_HSE)
        *pulBaud *= 2;
    *pulConfig = (this->LCRH &
                  (UART_LCRH_SPS | UART_LCRH_WLEN_M | UART_LCRH_STP2 |
                   UART_LCRH_EPS | UART_LCRH_PEN));
}

/**
*
* Enables transmitting and receiving.
*
* Sets the UARTEN, TXE, and RXE bits, and enables the transmit and receive
* FIFOs.
*
* \return None.
*
*/
void UART_Type::UARTEnable(void)
{
    this->LCRH |= UART_LCRH_FEN;
    this->CTL |= (UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE);
}

/**
 * RxEnable
 */
void UART_Type::RxEnable(void)
{
    this->CTL |= UART_CTL_RXE;
}

/**
 * RxDisable
 */
void UART_Type::RxDisable(void)
{
    this->CTL &= ~UART_CTL_RXE;
}

/** TxEnable
 *
 */
void UART_Type::TxEnable(void)
{
    this->CTL |= UART_CTL_TXE;
}

/** TxDisable
 *
 */
void UART_Type::TxDisable(void)
{
    this->CTL &= ~UART_CTL_TXE;
}

/**
*
* Disables transmitting and receiving.
*
* Clears the UARTEN, TXE, and RXE bits, then waits for the end of
* transmission of the current character, and flushes the transmit FIFO.
*
* \return None.
*
*/
void UART_Type::UARTDisable(void)
{
    while(this->FR & UART_FR_BUSY)
    {
    }
    this->LCRH &= ~(UART_LCRH_FEN);
    this->CTL &= ~(UART_CTL_UARTEN | UART_CTL_TXE |UART_CTL_RXE);
}

/**
*
* Enables the transmit and receive FIFOs.
*
* This functions enables the transmit and receive FIFOs in the UART.
*
* \return None.
*
*/
void UART_Type::UARTFIFOEnable(void)
{
    this->LCRH |= UART_LCRH_FEN;
}

/**
*
* Disables the transmit and receive FIFOs.
*
* This functions disables the transmit and receive FIFOs in the UART.
*
* \return None.
*
*/
void UART_Type::UARTFIFODisable(void)
{
    this->LCRH &= ~(UART_LCRH_FEN);
}

/**
*
* Enables SIR (IrDA) mode on the specified UART.
*
* \param bLowPower indicates if SIR Low Power Mode is to be used.
*
* Enables the SIREN control bit for IrDA mode on the UART.  If the
* \e bLowPower flag is set, then SIRLP bit will also be set.
*
* \note SIR (IrDA) operation is not supported on Sandstorm-class devices.
*
* \return None.
*
*/
void UART_Type::UARTEnableSIR(int bLowPower)
{
    if(bLowPower)
    {
        this->CTL |= (UART_CTL_SIREN | UART_CTL_SIRLP);
    }
    else
    {
    	this->CTL |= (UART_CTL_SIREN);
    }
}

/**
*
* Disables SIR (IrDA) mode on the specified UART.
*
* Clears the SIREN (IrDA) and SIRLP (Low Power) bits.
*
* \note SIR (IrDA) operation is not supported on Sandstorm-class devices.
*
* \return None.
*
*/
void UART_Type::UARTDisableSIR(void)
{
    this->CTL &= ~(UART_CTL_SIREN | UART_CTL_SIRLP);
}

/**
*
* Enables ISO 7816 smart card mode on the specified UART.
*
* Enables the SMART control bit for ISO 7816 smart card mode on the UART.
* This call also sets 8 bit word length and even parity as required by ISO
* 7816.
*
* \note The availability of ISO 7816 smart card mode varies with the
* Stellaris part and UART in use.  Please consult the datasheet for the part
* you are using to determine whether this support is available.
*
* \return None.
*
*/
void UART_Type::UARTSmartCardEnable(void)
{
    unsigned long ulVal;

    ASSERT(!CLASS_IS_SANDSTORM && !CLASS_IS_FURY && !CLASS_IS_DUSTDEVIL);

    // Set 8 bit word length, even parity, 2 stop bits (even though the STP2
    // bit is ignored when in smartcard mode, this lets the caller read back
    // the actual setting in use).
    ulVal = this->LCRH;
    ulVal &= ~(UART_LCRH_SPS | UART_LCRH_EPS | UART_LCRH_PEN |
               UART_LCRH_WLEN_M);
    ulVal |= UART_LCRH_WLEN_8 | UART_LCRH_PEN | UART_LCRH_EPS | UART_LCRH_STP2;
    this->LCRH = ulVal;
    // Enable SMART mode.
    this->CTL |= UART_CTL_SMART;
}

/**
*
* Disables ISO 7816 smart card mode on the specified UART.
*
* Clears the SMART (ISO 7816 smart card) bits in the UART control register.
*
* \note The availability of ISO 7816 smart card mode varies with the
* Stellaris part and UART in use.  Please consult the datasheet for the part
* you are using to determine whether this support is available.
*
* \return None.
*
*/
void UART_Type::UARTSmartCardDisable(void)
{
    ASSERT(!CLASS_IS_SANDSTORM && !CLASS_IS_FURY && !CLASS_IS_DUSTDEVIL);

    this->CTL &= ~UART_CTL_SMART;
}

/**
*
* Sets the states of the DTR and/or RTS modem control signals.
*
* \param ulControl is a bit-mapped flag indicating which modem control bits
* should be set.
*
* Sets the states of the DTR or RTS modem handshake outputs from the UART.
*
* The \e ulControl parameter is the logical OR of any of the following:
*
* - \b UART_OUTPUT_DTR - The Modem Control DTR signal
* - \b UART_OUTPUT_RTS - The Modem Control RTS signal
*
* \note The availability of hardware modem handshake signals varies with the
* Stellaris part and UART in use.  Please consult the datasheet for the part
* you are using to determine whether this support is available.
*
* \return None.
*
*/
void UART_Type::UARTModemControlSet(unsigned long ulControl)
{
    unsigned long ulTemp;
    ASSERT(!CLASS_IS_SANDSTORM && !CLASS_IS_FURY && !CLASS_IS_DUSTDEVIL);
    ASSERT((ulControl & ~(UART_OUTPUT_RTS | UART_OUTPUT_DTR)) == 0);
    // Set the appropriate modem control output bits.
    ulTemp = this->CTL;
    ulTemp |= (ulControl & (UART_OUTPUT_RTS | UART_OUTPUT_DTR));
    this->CTL = ulTemp;
}

/**
*
* Clears the states of the DTR and/or RTS modem control signals.
*
* \param ulControl is a bit-mapped flag indicating which modem control bits
* should be set.
*
* Clears the states of the DTR or RTS modem handshake outputs from the UART.
*
* The \e ulControl parameter is the logical OR of any of the following:
*
* - \b UART_OUTPUT_DTR - The Modem Control DTR signal
* - \b UART_OUTPUT_RTS - The Modem Control RTS signal
*
* \note The availability of hardware modem handshake signals varies with the
* Stellaris part and UART in use.  Please consult the datasheet for the part
* you are using to determine whether this support is available.
*
* \return None.
*
*/
void UART_Type::UARTModemControlClear(unsigned long ulControl)
{
    unsigned long ulTemp;
    ASSERT(!CLASS_IS_SANDSTORM && !CLASS_IS_FURY && !CLASS_IS_DUSTDEVIL);
    ASSERT((ulControl & ~(UART_OUTPUT_RTS | UART_OUTPUT_DTR)) == 0);
    // Clear the appropriate modem control output bits.
    ulTemp = this->CTL;
    ulTemp &= ~(ulControl & (UART_OUTPUT_RTS | UART_OUTPUT_DTR));
    this->CTL = ulTemp;
}

/**
*
* Gets the states of the DTR and RTS modem control signals.
*
* Returns the current states of each of the two UART modem control signals,
* DTR and RTS.
*
* \note The availability of hardware modem handshake signals varies with the
* Stellaris part and UART in use.  Please consult the datasheet for the part
* you are using to determine whether this support is available.
*
* \return Returns the states of the handshake output signals.  This will be a
* logical logical OR combination of values \b UART_OUTPUT_RTS and
* \b UART_OUTPUT_DTR where the presence of each flag indicates that the
* associated signal is asserted.
*
*/
unsigned long UART_Type::UARTModemControlGet(void)
{
    ASSERT(!CLASS_IS_SANDSTORM && !CLASS_IS_FURY && !CLASS_IS_DUSTDEVIL);

    return(this->CTL & (UART_OUTPUT_RTS | UART_OUTPUT_DTR));
}

/**
*
* Gets the states of the RI, DCD, DSR and CTS modem status signals.
*
* Returns the current states of each of the four UART modem status signals,
* RI, DCD, DSR and CTS.
*
* \note The availability of hardware modem handshake signals varies with the
* Stellaris part and UART in use.  Please consult the datasheet for the part
* you are using to determine whether this support is available.
*
* \return Returns the states of the handshake output signals.  This will be a
* logical logical OR combination of values \b UART_INPUT_RI, \b
* UART_INPUT_DCD, \b UART_INPUT_CTS and \b UART_INPUT_DSR where the
* presence of each flag indicates that the associated signal is asserted.
*
*/
unsigned long UART_Type::UARTModemStatusGet(void)
{
    ASSERT(!CLASS_IS_SANDSTORM && !CLASS_IS_FURY && !CLASS_IS_DUSTDEVIL);

    return(this->FR & (UART_INPUT_RI | UART_INPUT_DCD | UART_INPUT_CTS | UART_INPUT_DSR));
}

/**
*
* Sets the UART hardware flow control mode to be used.
*
* \param ulMode indicates the flow control modes to be used.  This is a
* logical OR combination of values \b UART_FLOWCONTROL_TX and \b
* UART_FLOWCONTROL_RX to enable hardware transmit (CTS) and receive (RTS)
* flow control or \b UART_FLOWCONTROL_NONE to disable hardware flow control.
*
* Sets the required hardware flow control modes.  If \e ulMode contains
* flag \b UART_FLOWCONTROL_TX, data is only transmitted if the incoming CTS
* signal is asserted. If \e ulMode contains flag \b UART_FLOWCONTROL_RX,
* the RTS output is controlled by the hardware and is asserted only when
* there is space available in the receive FIFO.  If no hardware flow control
* is required, UART_FLOWCONTROL_NONE should be passed.
*
* \note The availability of hardware flow control varies with the Stellaris
* part and UART in use.  Please consult the datasheet for the part you are
* using to determine whether this support is available.
*
* \return None.
*
*/
void UART_Type::UARTFlowControlSet(unsigned long ulMode)
{
    ASSERT(!CLASS_IS_SANDSTORM && !CLASS_IS_FURY && !CLASS_IS_DUSTDEVIL);
    ASSERT((ulMode & ~(UART_FLOWCONTROL_TX | UART_FLOWCONTROL_RX)) == 0);
    // Set the flow control mode as requested.
    this->CTL = ((this->CTL & ~(UART_FLOWCONTROL_TX |UART_FLOWCONTROL_RX)) | ulMode);
}

/**
*
* Returns the UART hardware flow control mode currently in use.
*
* Returns the current hardware flow control mode.
*
* \note The availability of hardware flow control varies with the Stellaris
* part and UART in use.  Please consult the datasheet for the part you are
* using to determine whether this support is available.
*
* \return Returns the current flow control mode in use.  This is a
* logical OR combination of values \b UART_FLOWCONTROL_TX if transmit
* (CTS) flow control is enabled and \b UART_FLOWCONTROL_RX if receive (RTS)
* flow control is in use.  If hardware flow control is disabled, \b
* UART_FLOWCONTROL_NONE will be returned.
*
*/
unsigned long UART_Type::UARTFlowControlGet(void)
{
    ASSERT(!CLASS_IS_SANDSTORM && !CLASS_IS_FURY && !CLASS_IS_DUSTDEVIL);

    return(this->CTL & (UART_FLOWCONTROL_TX | UART_FLOWCONTROL_RX));
}

/**
*
* Sets the operating mode for the UART transmit interrupt.
*
* \param ulMode is the operating mode for the transmit interrupt.  It may be
* \b UART_TXINT_MODE_EOT to trigger interrupts when the transmitter is idle
* or \b UART_TXINT_MODE_FIFO to trigger based on the current transmit FIFO
* level.
*
* This function allows the mode of the UART transmit interrupt to be set.  By
* default, the transmit interrupt is asserted when the FIFO level falls past
* a threshold set via a call to UARTFIFOLevelSet().  Alternatively, if this
* function is called with \e ulMode set to \b UART_TXINT_MODE_EOT, the
* transmit interrupt will only be asserted once the transmitter is completely
* idle - the transmit FIFO is empty and all bits, including any stop bits,
* have cleared the transmitter.
*
* \note The availability of end-of-transmission mode varies with the
* Stellaris part in use.  Please consult the datasheet for the part you are
* using to determine whether this support is available.
*
* \return None.
*
*/
void UART_Type::UARTTxIntModeSet(unsigned long ulMode)
{
    ASSERT((ulMode == UART_TXINT_MODE_EOT) ||
           (ulMode == UART_TXINT_MODE_FIFO));
    this->CTL = ((this->CTL & ~(UART_TXINT_MODE_EOT | UART_TXINT_MODE_FIFO)) | ulMode);
}

/**
*
* Returns the current operating mode for the UART transmit interrupt.
*
* This function returns the current operating mode for the UART transmit
* interrupt.  The return value will be \b UART_TXINT_MODE_EOT if the
* transmit interrupt is currently set to be asserted once the transmitter is
* completely idle - the transmit FIFO is empty and all bits, including any
* stop bits, have cleared the transmitter.  The return value will be \b
* UART_TXINT_MODE_FIFO if the interrupt is set to be asserted based upon the
* level of the transmit FIFO.
*
* \note The availability of end-of-transmission mode varies with the
* Stellaris part in use.  Please consult the datasheet for the part you are
* using to determine whether this support is available.
*
* \return Returns \b UART_TXINT_MODE_FIFO or \b UART_TXINT_MODE_EOT.
*
*/
unsigned long UART_Type::UARTTxIntModeGet(void)
{
    return(this->CTL & (UART_TXINT_MODE_EOT | UART_TXINT_MODE_FIFO));
}

/**
*
* Determines if there are any characters in the receive FIFO.
*
* This function returns a flag indicating whether or not there is data
* available in the receive FIFO.
*
* \return Returns \b true if there is data in the receive FIFO, and \b false
* if there is no data in the receive FIFO.
*
*/
int UART_Type::UARTCharsAvail(void)
{
    return((this->FR & UART_FR_RXFE) ? false : true);
}

/**
*
* Determines if there is any space in the transmit FIFO.
*
* This function returns a flag indicating whether or not there is space
* available in the transmit FIFO.
*
* \return Returns \b true if there is space available in the transmit FIFO,
* and \b false if there is no space available in the transmit FIFO.
*
*/
int UART_Type::UARTSpaceAvail(void)
{
    return((this->FR & UART_FR_TXFF) ? false : true);
}

/**
*
* Receives a character from the specified port.
*
* Gets a character from the receive FIFO for the specified port.
*
* This function replaces the original UARTCharNonBlockingGet() API and
* performs the same actions.  A macro is provided in <tt>uart.h</tt> to map
* the original API to this API.
*
* \return Returns the character read from the specified port, cast as a
* \e long.  A \b -1 will be returned if there are no characters present in
* the receive FIFO.  The UARTCharsAvail() function should be called before
* attempting to call this function.
*
*/
long UART_Type::UARTCharGetNonBlocking(void)
{
    if(!(this->FR & UART_FR_RXFE))
        return(this->DR);
    return(-1);
}

/**
*
* Waits for a character from the specified port.
*
* Gets a character from the receive FIFO for the specified port.  If there
* are no characters available, this function will wait until a character is
* received before returning.
*
* \return Returns the character read from the specified port, cast as an
* \e long.
*
*/
long UART_Type::UARTCharGet(void)
{
    while(this->FR & UART_FR_RXFE)
    {
    }
    return(this->DR);
}

/**
*
* Sends a character to the specified port.
*
* \param ucData is the character to be transmitted.
*
* Writes the character \e ucData to the transmit FIFO for the specified port.
* This function does not block, so if there is no space available, then a
* \b false is returned, and the application will have to retry the function
* later.
*
* This function replaces the original UARTCharNonBlockingPut() API and
* performs the same actions.  A macro is provided in <tt>uart.h</tt> to map
* the original API to this API.
*
* \return Returns \b true if the character was successfully placed in the
* transmit FIFO, and \b false if there was no space available in the transmit
* FIFO.

*/
int UART_Type::UARTCharPutNonBlocking(unsigned char ucData)
{
    if(!(this->FR & UART_FR_TXFF))
    {
        this->DR = ucData;
        return(true);
    }
    return(false);
}

/**
*
* Waits to send a character from the specified port.
*
* \param ucData is the character to be transmitted.
*
* Sends the character \e ucData to the transmit FIFO for the specified port.
* If there is no space available in the transmit FIFO, this function will
* wait until there is space available before returning.
*
* \return None.
*
*/
void UART_Type::UARTCharPut(unsigned char ucData)
{
    while(this->FR & UART_FR_TXFF)
    {
    }
    this->DR = ucData;
}

/**
*
* Causes a BREAK to be sent.
*
* \param bBreakState controls the output level.
*
* Calling this function with \e bBreakState set to \b true will assert a
* break condition on the UART.  Calling this function with \e bBreakState set
* to \b false will remove the break condition.  For proper transmission of a
* break command, the break must be asserted for at least two complete frames.
*
* \return None.
*
*/
void UART_Type::UARTBreakCtl(int bBreakState)
{
    this->LCRH = (bBreakState ?
         (this->LCRH | UART_LCRH_BRK) :
         (this->LCRH & ~(UART_LCRH_BRK)));
}

/**
*
* Determines whether the UART transmitter is busy or not.
*
* Allows the caller to determine whether all transmitted bytes have cleared
* the transmitter hardware.  If \b false is returned, the transmit FIFO is
* empty and all bits of the last transmitted character, including all stop
* bits, have left the hardware shift register.
*
* \return Returns \b true if the UART is transmitting or \b false if all
* transmissions are complete.
*
*/
int UART_Type::UARTBusy(void)
{
    return((this->FR & UART_FR_BUSY) ? true : false);
}

/**
*
* Enables individual UART interrupt sources.
*
* \param ulIntFlags is the bit mask of the interrupt sources to be enabled.
*
* Enables the indicated UART interrupt sources.  Only the sources that are
* enabled can be reflected to the processor interrupt; disabled sources have
* no effect on the processor.
*
* The \e ulIntFlags parameter is the logical OR of any of the following:
*
* - \b UART_INT_OE - Overrun Error interrupt
* - \b UART_INT_BE - Break Error interrupt
* - \b UART_INT_PE - Parity Error interrupt
* - \b UART_INT_FE - Framing Error interrupt
* - \b UART_INT_RT - Receive Timeout interrupt
* - \b UART_INT_TX - Transmit interrupt
* - \b UART_INT_RX - Receive interrupt
* - \b UART_INT_DSR - DSR interrupt
* - \b UART_INT_DCD - DCD interrupt
* - \b UART_INT_CTS - CTS interrupt
* - \b UART_INT_RI - RI interrupt
*
* \return None.
*
*/
void UART_Type::UARTIntEnable(unsigned long ulIntFlags)
{
     this->IM |= ulIntFlags;
}

/**
*
* Disables individual UART interrupt sources.
*
* \param ulIntFlags is the bit mask of the interrupt sources to be disabled.
*
* Disables the indicated UART interrupt sources.  Only the sources that are
* enabled can be reflected to the processor interrupt; disabled sources have
* no effect on the processor.
*
* The \e ulIntFlags parameter has the same definition as the \e ulIntFlags
* parameter to UARTIntEnable().
*
* \return None.
*
*/
void UART_Type::UARTIntDisable(unsigned long ulIntFlags)
{
    this->IM &= ~(ulIntFlags);
}

/**
*
* Gets the current interrupt status.
*
* \param bMasked is false if the raw interrupt status is required and true
* if the masked interrupt status is required.
*
* This returns the interrupt status for the specified UART.  Either the raw
* interrupt status or the status of interrupts that are allowed to reflect to
* the processor can be returned.
*
* \return Returns the current interrupt status, enumerated as a bit field of
* values described in UARTIntEnable().
*
*/
unsigned long UART_Type::UARTIntStatus(int bMasked)
{
    if(bMasked)
        return(this->MIS);
    return(this->RIS);
}

/**
*
* Clears UART interrupt sources.
*
* \param ulIntFlags is a bit mask of the interrupt sources to be cleared.
*
* The specified UART interrupt sources are cleared, so that they no longer
* assert.  This must be done in the interrupt handler to keep it from being
* called again immediately upon exit.
*
* The \e ulIntFlags parameter has the same definition as the \e ulIntFlags
* parameter to UARTIntEnable().
*
* \note Since there is a write buffer in the Cortex-M3 processor, it may take
* several clock cycles before the interrupt source is actually cleared.
* Therefore, it is recommended that the interrupt source be cleared early in
* the interrupt handler (as opposed to the very last action) to avoid
* returning from the interrupt handler before the interrupt source is
* actually cleared.  Failure to do so may result in the interrupt handler
* being immediately reentered (since NVIC still sees the interrupt source
* asserted).
*
* \return None.
*
*/
void UART_Type::UARTIntClear(unsigned long ulIntFlags)
{
    this->ICR = ulIntFlags;
}

/**
*
* Enable UART DMA operation.
*
* \param ulDMAFlags is a bit mask of the DMA features to enable.
*
* The specified UART DMA features are enabled.  The UART can be
* configured to use DMA for transmit or receive, and to disable
* receive if an error occurs.  The \e ulDMAFlags parameter is the
* logical OR of any of the following values:
*
* - UART_DMA_RX - enable DMA for receive
* - UART_DMA_TX - enable DMA for transmit
* - UART_DMA_ERR_RXSTOP - disable DMA receive on UART error
*
* \note The uDMA controller must also be set up before DMA can be used
* with the UART.
*
* \return None.
*
*/
void UART_Type::UARTDMAEnable(unsigned long ulDMAFlags)
{
    this->DMACTL |= ulDMAFlags;
}

/**
*
* Disable UART DMA operation.
*
* \param ulDMAFlags is a bit mask of the DMA features to disable.
*
* This function is used to disable UART DMA features that were enabled
* by UARTDMAEnable().  The specified UART DMA features are disabled.  The
* \e ulDMAFlags parameter is the logical OR of any of the following values:
*
* - UART_DMA_RX - disable DMA for receive
* - UART_DMA_TX - disable DMA for transmit
* - UART_DMA_ERR_RXSTOP - do not disable DMA receive on UART error
*
* \return None.
*
*/
void UART_Type::UARTDMADisable(unsigned long ulDMAFlags)
{
    this->DMACTL &= ~ulDMAFlags;
}

/**
*
* Gets current receiver errors.
*
* This function returns the current state of each of the 4 receiver error
* sources.  The returned errors are equivalent to the four error bits
* returned via the previous call to UARTCharGet() or UARTCharGetNonBlocking()
* with the exception that the overrun error is set immediately the overrun
* occurs rather than when a character is next read.
*
* \return Returns a logical OR combination of the receiver error flags,
* \b UART_RXERROR_FRAMING, \b UART_RXERROR_PARITY, \b UART_RXERROR_BREAK
* and \b UART_RXERROR_OVERRUN.
*
*/
unsigned long UART_Type::UARTRxErrorGet(void)
{
    return(this->RSR & 0x0000000F);
}

/**
*
* Clears all reported receiver errors.
*
* This function is used to clear all receiver error conditions reported via
* UARTRxErrorGet().  If using the overrun, framing error, parity error or
* break interrupts, this function must be called after clearing the interrupt
* to ensure that later errors of the same type trigger another interrupt.
*
* \return None.
*
*/
void UART_Type::UARTRxErrorClear(void)
{
    this->ECR = 0;
}



/** @} ingroup	 lm3s_uart */
