/**************************************************************************//**
 * @ingroup	 lm3s_ssi
 * @{
 * @file     ssi_lm3s.h
 * @brief    SSI class header
 * @version  V3.00
 * @date     18. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/
#ifndef SSI_LM3S_H_
#define SSI_LM3S_H_

#ifdef __cplusplus
extern "C"
{
#endif
/*----------       Synchronous Serial Interface (SSI)             -----------*/
// <g> Synchronous Serial Interface (SSI)
struct SSI_Type
{
	__IO uint32_t CR0;              //!< Control register 0
	__IO uint32_t CR1;              //!< Control register 1
	__IO uint32_t DR;               //!< Data register
	__I  uint32_t SR;               //!< Status register
	__IO uint32_t CPSR;             //!< Clock prescale register
	__IO uint32_t IM;               //!< Int mask set and clear register
	__I  uint32_t RIS;              //!< Raw interrupt register
	__I  uint32_t MIS;              //!< Masked interrupt register
	__O  uint32_t ICR;              //!< Interrupt clear register
	__IO uint32_t DMACTL;           //!< SSI DMA Control

	void SSIConfigSetExpClk(unsigned long ulSSIClk,
						    unsigned long ulProtocol, unsigned long ulMode,
						    unsigned long ulBitRate, unsigned long ulDataWidth);
	void SSIEnable(void);
	void SSIDisable(void);
	void SSIIntEnable(unsigned long ulIntFlags);
	void SSIIntDisable(unsigned long ulIntFlags);
	unsigned long SSIIntStatus(int bMasked);
	void SSIIntClear(unsigned long ulIntFlags);
	void SSIDataPut(unsigned long ulData);
	long SSIDataPutNonBlocking( unsigned long ulData);
	void SSIDataGet(unsigned long *pulData);
	long SSIDataGetNonBlocking( unsigned long *pulData);
	void SSIDMAEnable(unsigned long ulDMAFlags);
	void SSIDMADisable(unsigned long ulDMAFlags);
	int SSIBusy(void);


};
// </g>

//*****************************************************************************
//
// Values that can be passed to SSIIntEnable, SSIIntDisable, and SSIIntClear
// as the ulIntFlags parameter, and returned by SSIIntStatus.
//
//*****************************************************************************
#define SSI_TXFF                0x00000008  //!< TX FIFO half empty or less
#define SSI_RXFF                0x00000004  //!< RX FIFO half full or less
#define SSI_RXTO                0x00000002  //!< RX timeout
#define SSI_RXOR                0x00000001  //!< RX overrun

//*****************************************************************************
//
// Values that can be passed to SSIConfigSetExpClk.
//
//*****************************************************************************
#define SSI_FRF_MOTO_MODE_0     0x00000000  //!< Moto fmt, polarity 0, phase 0
#define SSI_FRF_MOTO_MODE_1     0x00000002  //!< Moto fmt, polarity 0, phase 1
#define SSI_FRF_MOTO_MODE_2     0x00000001  //!< Moto fmt, polarity 1, phase 0
#define SSI_FRF_MOTO_MODE_3     0x00000003  //!< Moto fmt, polarity 1, phase 1
#define SSI_FRF_TI              0x00000010  //!< TI frame format
#define SSI_FRF_NMW             0x00000020  //!< National MicroWire frame format

#define SSI_MODE_MASTER         0x00000000  //!< SSI master
#define SSI_MODE_SLAVE          0x00000001  //!< SSI slave
#define SSI_MODE_SLAVE_OD       0x00000002  //!< SSI slave with output disabled

//*****************************************************************************
//
// Values that can be passed to SSIDMAEnable() and SSIDMADisable().
//
//*****************************************************************************
#define SSI_DMA_TX              0x00000002  //!< Enable DMA for transmit
#define SSI_DMA_RX              0x00000001  //!< Enable DMA for receive

#define SSI0       ((SSI_Type *)SSI0_BASE)	//!< doc!
#define SSI1       ((SSI_Type *)SSI1_BASE)	//!< doc!

#ifdef __cplusplus
}
#endif

#endif // SSI_LM3S_H_
/** @} ingroup	 lm3s_ssi */
