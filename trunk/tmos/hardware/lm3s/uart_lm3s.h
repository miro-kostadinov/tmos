/**************************************************************************//**
 * @ingroup	 lm3s_uart
 * @{
 * @file     uart_lm3s.h
 * @brief    UART header
 * @version  V3.00
 * @date     15. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/
#ifndef UART_LM3S_H_
#define UART_LM3S_H_

#ifdef __cplusplus
extern "C"
{
#endif
	/*----------       Asynchronous Serial (UART)                     -----------*/
	// <g> Asynchronous Serial (UART)
	struct UART_Type
	{
	    __IO uint32_t DR;               //!< Data Register
	    __IO uint32_t RSR;              //!< Receive Status Register (read)
	#define ECR RSR                     //!< Error Clear Register (write)
	         uint8_t  RESERVED1[16];	//!< doc!
	    __I  uint32_t FR;               //!< Flag Register (read only)
	         uint8_t  RESERVED2[4];		//!< doc!
	    __IO uint32_t ILPR;             //!< UART IrDA Low-Power Register
	    __IO uint32_t IBRD;             //!< Integer Baud Rate Divisor Reg
	    __IO uint32_t FBRD;             //!< Fractional Baud Rate Divisor Reg
	    __IO uint32_t LCRH;             //!< UART Line Control
	    __IO uint32_t CTL;              //!< Control Register
	    __IO uint32_t IFLS;             //!< Interrupt FIFO Level Select Reg
	    __IO uint32_t IM;               //!< Interrupt Mask Set/Clear Reg
	    __I  uint32_t RIS;              //!< Raw Interrupt Status Register
	    __I  uint32_t MIS;              //!< Masked Interrupt Status Register
	    __O  uint32_t ICR;              //!< Interrupt Clear Register
	    __IO uint32_t DMACTL;           //!< UART DMA Control
	         uint8_t  RESERVED3[68];	//!< doc!
	    __IO uint32_t LCTL;             //!< UART LIN Control
	    __I  uint32_t LSS;              //!< UART LIN Snap Shot
	    __I  uint32_t LTIM;             //!< UART LIN Timer
	    void UARTParityModeSet(unsigned long ulParity);
	    unsigned long UARTParityModeGet(void);
	    void UARTFIFOLevelSet(unsigned long ulTxLevel, unsigned long ulRxLevel);
	    void UARTFIFOLevelGet( unsigned long *pulTxLevel, unsigned long *pulRxLevel );
	    void UARTConfigSetExpClk(unsigned long ulUARTClk, unsigned long ulBaud, unsigned long ulConfig);
	    void UARTConfigGetExpClk(unsigned long ulUARTClk, unsigned long *pulBaud, unsigned long *pulConfig);
	    void UARTEnable(void);
	    void UARTDisable(void);
	    void UARTFIFODisable(void);
	    void UARTFIFOEnable(void);
	    void UARTEnableSIR(int bLowPower);
	    void UARTDisableSIR(void);
	    void UARTSmartCardEnable(void);
	    void UARTSmartCardDisable(void);
	    void UARTModemControlSet(unsigned long ulControl);
	    void UARTModemControlClear(unsigned long ulControl);
	    unsigned long UARTModemControlGet(void);
	    unsigned long UARTModemStatusGet(void);
	    void UARTFlowControlSet(unsigned long ulMode);
	    unsigned long UARTFlowControlGet(void);
	    void UARTTxIntModeSet(unsigned long ulMode);
	    unsigned long UARTTxIntModeGet(void);
	    int UARTCharsAvail(void);
	    int UARTSpaceAvail(void);
	    long UARTCharGetNonBlocking(void);
	    long UARTCharGet(void);
	    int UARTCharPutNonBlocking(unsigned char ucData);
	    void UARTCharPut(unsigned char ucData);
	    void UARTBreakCtl(int bBreakState);
	    int UARTBusy(void);
	    void UARTIntEnable(unsigned long ulIntFlags);
	    void UARTIntDisable(unsigned long ulIntFlags);
	    unsigned long UARTIntStatus(int bMasked);
	    void UARTIntClear(unsigned long ulIntFlags);
	    void UARTDMAEnable(unsigned long ulDMAFlags);
	    void UARTDMADisable(unsigned long ulDMAFlags);
	    unsigned long UARTRxErrorGet(void);
	    void UARTRxErrorClear(void);
//	    void UARTReset(void);
//	    unsigned long UARTGetPeripheral(void);
	    void RxEnable(void);
	    void RxDisable(void);
	    void TxEnable(void);
	    void TxDisable(void);
	    unsigned long DMA_GetRxChannel(void);
	    unsigned long DMA_GetTxChannel(void);
	};
	// </g>
//*****************************************************************************
//
// Values that can be passed to UARTIntEnable, UARTIntDisable, and UARTIntClear
// as the ulIntFlags parameter, and returned from UARTIntStatus.
//
//*****************************************************************************
#define UART_INT_OE             0x400       //!< Overrun Error Interrupt Mask
#define UART_INT_BE             0x200       //!< Break Error Interrupt Mask
#define UART_INT_PE             0x100       //!< Parity Error Interrupt Mask
#define UART_INT_FE             0x080       //!< Framing Error Interrupt Mask
#define UART_INT_RT             0x040       //!< Receive Timeout Interrupt Mask
#define UART_INT_TX             0x020       //!< Transmit Interrupt Mask
#define UART_INT_RX             0x010       //!< Receive Interrupt Mask
#define UART_INT_DSR            0x008       //!< DSR Modem Interrupt Mask
#define UART_INT_DCD            0x004       //!< DCD Modem Interrupt Mask
#define UART_INT_CTS            0x002       //!< CTS Modem Interrupt Mask
#define UART_INT_RI             0x001       //!< RI Modem Interrupt Mask

//*****************************************************************************
//
// Values that can be passed to UARTConfigSetExpClk as the ulConfig parameter
// and returned by UARTConfigGetExpClk in the pulConfig parameter.
// Additionally, the UART_CONFIG_PAR_* subset can be passed to
// UARTParityModeSet as the ulParity parameter, and are returned by
// UARTParityModeGet.
//
//*****************************************************************************
#define UART_CONFIG_WLEN_MASK   0x00000060  //!< Mask for extracting word length
#define UART_CONFIG_WLEN_8      0x00000060  //!< 8 bit data
#define UART_CONFIG_WLEN_7      0x00000040  //!< 7 bit data
#define UART_CONFIG_WLEN_6      0x00000020  //!< 6 bit data
#define UART_CONFIG_WLEN_5      0x00000000  //!< 5 bit data
#define UART_CONFIG_STOP_MASK   0x00000008  //!< Mask for extracting stop bits
#define UART_CONFIG_STOP_ONE    0x00000000  //!< One stop bit
#define UART_CONFIG_STOP_TWO    0x00000008  //!< Two stop bits
#define UART_CONFIG_PAR_MASK    0x00000086  //!< Mask for extracting parity
#define UART_CONFIG_PAR_NONE    0x00000000  //!< No parity
#define UART_CONFIG_PAR_EVEN    0x00000006  //!< Even parity
#define UART_CONFIG_PAR_ODD     0x00000002  //!< Odd parity
#define UART_CONFIG_PAR_ONE     0x00000082  //!< Parity bit is one
#define UART_CONFIG_PAR_ZERO    0x00000086  //!< Parity bit is zero

//*****************************************************************************
//
// Values that can be passed to UARTFIFOLevelSet as the ulTxLevel parameter and
// returned by UARTFIFOLevelGet in the pulTxLevel.
//
//*****************************************************************************
#define UART_FIFO_TX1_8         0x00000000  //!< Transmit interrupt at 1/8 Full
#define UART_FIFO_TX2_8         0x00000001  //!< Transmit interrupt at 1/4 Full
#define UART_FIFO_TX4_8         0x00000002  //!< Transmit interrupt at 1/2 Full
#define UART_FIFO_TX6_8         0x00000003  //!< Transmit interrupt at 3/4 Full
#define UART_FIFO_TX7_8         0x00000004  //!< Transmit interrupt at 7/8 Full

//*****************************************************************************
//
// Values that can be passed to UARTFIFOLevelSet as the ulRxLevel parameter and
// returned by UARTFIFOLevelGet in the pulRxLevel.
//
//*****************************************************************************
#define UART_FIFO_RX1_8         0x00000000  //!< Receive interrupt at 1/8 Full
#define UART_FIFO_RX2_8         0x00000008  //!< Receive interrupt at 1/4 Full
#define UART_FIFO_RX4_8         0x00000010  //!< Receive interrupt at 1/2 Full
#define UART_FIFO_RX6_8         0x00000018  //!< Receive interrupt at 3/4 Full
#define UART_FIFO_RX7_8         0x00000020  //!< Receive interrupt at 7/8 Full

//*****************************************************************************
//
// Values that can be passed to UARTDMAEnable() and UARTDMADisable().
//
//*****************************************************************************
#define UART_DMA_ERR_RXSTOP     0x00000004  //!< Stop DMA receive if UART error
#define UART_DMA_TX             0x00000002  //!< Enable DMA for transmit
#define UART_DMA_RX             0x00000001  //!< Enable DMA for receive

//*****************************************************************************
//
// Values returned from UARTRxErrorGet().
//
//*****************************************************************************
#define UART_RXERROR_OVERRUN    0x00000008	//!< doc!
#define UART_RXERROR_BREAK      0x00000004	//!< doc!
#define UART_RXERROR_PARITY     0x00000002	//!< doc!
#define UART_RXERROR_FRAMING    0x00000001	//!< doc!

//*****************************************************************************
//
// Values that can be passed to UARTHandshakeOutputsSet() or returned from
// UARTHandshakeOutputGet().
//
//*****************************************************************************
#define UART_OUTPUT_RTS         0x00000800	//!< doc!
#define UART_OUTPUT_DTR         0x00000400	//!< doc!

//*****************************************************************************
//
// Values that can be returned from UARTHandshakeInputsGet().
//
//*****************************************************************************
#define UART_INPUT_RI           0x00000100	//!< doc!
#define UART_INPUT_DCD          0x00000004	//!< doc!
#define UART_INPUT_DSR          0x00000002	//!< doc!
#define UART_INPUT_CTS          0x00000001	//!< doc!

//*****************************************************************************
//
// Values that can be passed to UARTFlowControl() or returned from
// UARTFlowControlGet().
//
//*****************************************************************************
#define UART_FLOWCONTROL_TX     0x00008000	//!< doc!
#define UART_FLOWCONTROL_RX     0x00004000	//!< doc!
#define UART_FLOWCONTROL_NONE   0x00000000	//!< doc!

//*****************************************************************************
//
// Values that can be passed to UARTTxIntModeSet() or returned from
// UARTTxIntModeGet().
//
//*****************************************************************************
#define UART_TXINT_MODE_FIFO    0x00000000	//!< doc!
#define UART_TXINT_MODE_EOT     0x00000010	//!< doc!

#define UART0         ((UART_Type *)UART0_BASE)	//!< doc!
#define UART1         ((UART_Type *)UART1_BASE)	//!< doc!
#define UART2         ((UART_Type *)UART2_BASE)	//!< doc!

#ifdef __cplusplus
}
#endif

#endif //  UART_LM3S_H_
/** @} ingroup	 lm3s_uart */
