/**************************************************************************//**
 * @ingroup	hardware_stm32h7_usart
 * @file	hardware/stm32/stm32h7/usart_h7.h
 * @brief  	STM32H7 USART
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_usart  Universal Synchronous Asynchronous Receiver Transmitter (USART)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 Universal Synchronous Asynchronous Receiver Transmitter (USART
 * @{
 *
 ******************************************************************************/

#ifndef USART_H7_H_
#define USART_H7_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Universal Synchronous Asynchronous Receiver Transmitter
 ******************************************************************************/
typedef struct
{
  __IO uint32_t USART_CR1;    //!< (usart Offset: 0x00) USART Control register 1
  __IO uint32_t USART_CR2;    //!< (usart Offset: 0x04) USART Control register 2
  __IO uint32_t USART_CR3;    //!< (usart Offset: 0x08) USART Control register 3
  __IO uint32_t USART_BRR;    //!< (usart Offset: 0x0C) USART Baud rate register
  __IO uint32_t USART_GTPR;   //!< (usart Offset: 0x10) USART Guard time and prescaler register
  __IO uint32_t USART_RTOR;   //!< (usart Offset: 0x14) USART Receiver Time Out register
  __IO uint32_t USART_RQR;    //!< (usart Offset: 0x18) USART Request register
  __IO uint32_t USART_ISR;    //!< (usart Offset: 0x1C) USART Interrupt and status register
  __IO uint32_t USART_ICR;    //!< (usart Offset: 0x20) USART Interrupt flag Clear register
  __IO uint32_t USART_RDR;    //!< (usart Offset: 0x24) USART Receive Data register
  __IO uint32_t USART_TDR;    //!< (usart Offset: 0x28) USART Transmit Data register
  __IO uint32_t USART_PRESC;  //!< (usart Offset: 0x2C) USART clock Prescaler register
} USART_TypeDef;

/*******************************************************************************
 * @relates USART_TypeDef
 * @{
 ******************************************************************************/

/** @name USART_CR1:	(usart Offset: 0x00) Control register 1 	          */
/** @{ */
#define USART_CR1_RXFFIE 			0x80000000 //!< RXFIFO Full interrupt enable
#define USART_CR1_TXFEIE 			0x40000000 //!< TXFIFO empty interrupt enable
#define USART_CR1_FIFOEN  			0x20000000 //!< FIFO mode enable
#define USART_CR1_M1 				0x10000000 //!< Word length
#define USART_CR1_EOBIE 		 	0x08000000 //!< End of Block interrupt enable
#define USART_CR1_RTOIE 			0x04000000 //!< Receiver timeout interrupt enable
#define USART_CR1_DEAT				0x03E00000 //!< Driver Enable assertion time
#define USART_CR1_DEDT				0x001F0000 //!< Driver Enable deassertion time
#define USART_CR1_OVER8 			0x00008000 //!< Oversampling mode
#define USART_CR1_CMIE				0x00004000 //!< Character match interrupt enable
#define USART_CR1_MME 				0x00002000 //!< Mute mode enable
#define USART_CR1_M0 				0x00001000 //!< Word length
#define USART_CR1_WAKE 				0x00000800 //!< Receiver wakeup method
#define USART_CR1_PCE 				0x00000400 //!< Parity control enable
#define USART_CR1_PS 				0x00000200 //!< Parity selection
#define USART_CR1_PEIE 				0x00000100 //!< PE interrupt enable
#define USART_CR1_TXFNFIE 			0x00000080 //!< TXFIFO not full interrupt enable
#define USART_CR1_TCIE 				0x00000040 //!< Transmission complete interrupt enable
#define USART_CR1_RXFNEIE 			0x00000020 //!< RXFIFO not empty interrupt enable
#define USART_CR1_IDLEIE  			0x00000010 //!< IDLE interrupt enable
#define USART_CR1_TE 				0x00000008 //!< Transmitter enable
#define USART_CR1_RE 				0x00000004 //!< Receiver enable
#define USART_CR1_UESM 				0x00000002 //!< USART enable in low-power mode
#define USART_CR1_UE 				0x00000001 //!< USART enable

#define USART_CR1_M 				USART_CR1_M0 //!< Legacy (>8 bit)
/** @} */

/** @name USART_CR2:	(usart Offset: 0x04) Control register 2 	          */
/** @{ */
#define USART_CR2_ADD               0xFF000000 //!< Address of the USART node
#define USART_CR2_ADD_Set(x)  		((x)<<24)  //!<  Address set
#define USART_CR2_ADD_Get(x)   		((x)>>24)  //!<  Address get
#define USART_CR2_RTOEN 			0x00800000 //!< Receiver timeout enable
#define USART_CR2_ABRMOD 			0x00600000 //!< Auto baud rate mode
#define USART_CR2_ABREN 			0x00100000 //!< Auto baud rate enable
#define USART_CR2_MSBFIRST 			0x00080000 //!< Most significant bit first
#define USART_CR2_DATAINV 			0x00040000 //!< Binary data inversion
#define USART_CR2_TXINV 			0x00020000 //!< TX pin active level inversion
#define USART_CR2_RXINV 			0x00010000 //!< RX pin active level inversion
#define USART_CR2_SWAP 				0x00008000 //!< Swap TX/RX pins
#define USART_CR2_LINEN 			0x00004000 //!< LIN mode enable
#define USART_CR2_STOP 				0x00003000 //!< stop bits
#define USART_CR2_CLKEN 			0x00000800 //!< Clock enable
#define USART_CR2_CPOL 				0x00000400 //!< Clock polarity
#define USART_CR2_CPHA 				0x00000200 //!< Clock phase
#define USART_CR2_LBCL 				0x00000100 //!< Last bit clock pulse
#define USART_CR2_LBDIE 			0x00000040 //!< LIN break detection interrupt enable
#define USART_CR2_LBDL 				0x00000020 //!< LIN break detection length
#define USART_CR2_ADDM7 			0x00000010 //!< 7-bit Address Detection/4-bit Address Detection
#define USART_CR2_DIS_NSS    		0x00000008 //!<
#define USART_CR2_SLVEN    			0x00000001 //!< Synchronous Slave mode enable
/** @} */

/** @name USART_CR3:	(usart Offset: 0x08) Control register 3    			  */
/** @{ */
#define USART_CR3_TXFTCFG			0xE0000000 //!< TXFIFO threshold configuration
#define USART_CR3_RXFTIE 			0x10000000 //!< RXFIFO threshold interrupt enable
#define USART_CR3_RXFTCFG			0x0E000000 //!< Receive FIFO threshold configuration
#define USART_CR3_TCBGTIE  			0x01000000 //!< Transmission Complete before guard time, interrupt enable
#define USART_CR3_TXFTIE  			0x00800000 //!< TXFIFO threshold interrupt enable
#define USART_CR3_WUFIE  			0x00400000 //!< Wakeup from low-power mode interrupt enable
#define USART_CR3_WUS				0x00300000 //!< Wakeup from low-power mode interrupt flag selection
#define USART_CR3_SCARCNT	 		0x000E0000 //!< Smartcard auto-retry count

#define USART_CR3_DEP  				0x00008000 //!< Driver enable polarity selection
#define USART_CR3_DEM  		 		0x00004000 //!< Driver enable mode
#define USART_CR3_DDRE  			0x00002000 //!< DMA Disable on Reception Error
#define USART_CR3_OVRDIS  			0x00001000 //!< Overrun Disable
#define USART_CR3_ONEBIT  			0x00000800 //!< One sample bit method enable
#define USART_CR3_CTSIE  			0x00000400 //!< CTS interrupt enable
#define USART_CR3_CTSE  			0x00000200 //!< CTS enable
#define USART_CR3_RTSE  			0x00000100 //!< RTS enable
#define USART_CR3_DMAT  			0x00000080 //!< DMA enable transmitter
#define USART_CR3_DMAR  			0x00000040 //!< DMA enable receiver
#define USART_CR3_SCEN  			0x00000020 //!< Smartcard mode enable
#define USART_CR3_NACK  			0x00000010 //!< Smartcard NACK enable
#define USART_CR3_HDSEL  			0x00000008 //!< Half-duplex selection
#define USART_CR3_IRLP  			0x00000004 //!< IrDA low-power
#define USART_CR3_IREN  			0x00000002 //!< IrDA mode enable
#define USART_CR3_EIE  			 	0x00000001 //!< Error interrupt enable
/** @} */

/** @name USART_BRR:	(usart Offset: 0x0C) Baud rate register 	          */
/** @{ */
#define USART_BRR_BRR	 			0x0000FFFF //!< USART baud rate
/** @} */

/** @name USART_GTPR:	(usart Offset: 0x10) Guard time and prescaler register */
/** @{ */

#define USART_GTPR_GT               0x0000FF00 //!< Guard time value */
#define USART_GTPR_GT_Set(x)  	  	((x)<<8) //!< Guard time value set
#define USART_GTPR_GT_Get(x) (((x)>>8)&0xFF) //!< Guard time value get
#define USART_GTPR_PSC              0x000000FF //!< PSC[7:0] bits (Prescaler value) */
#define USART_GTPR_PSC_Set(x)  		 (x)   //!< Prescaler value set
#define USART_GTPR_PSC_Get(x)  ((x)& 0xFF) //!< Prescaler value get
/** @} */

/** @name USART_RTOR:	(usart Offset: 0x14) Receiver timeout register		  */
/** @{ */
#define USART_RTOR_BLEN 	0xFF000000 //!< Block Length
#define USART_RTOR_RTO1 	0x00FF0000 //!< Receiver timeout value
#define USART_RTOR_RTO2 	0x0000FFFF //!< Receiver timeout value

/** @} */

/** @name USART_RQR:	(usart Offset: 0x18) Request register     	          */
/** @{ */
#define USART_RQR_TXFRQ             0x00000010 //!< Transmit data flush request
#define USART_RQR_RXFRQ             0x00000008 //!< Receive data flush request
#define USART_RQR_MMRQ              0x00000004 //!< Mute mode request
#define USART_RQR_SBKRQ             0x00000002 //!< Send break request
#define USART_RQR_ABRRQ             0x00000001 //!< Auto baud rate request
/** @} */

/** @name USART_ISR:	(usart Offset: 0x1C) Interrupt & status register 	  */
/** @{ */
#define USART_ISR_TXFT 				0x08000000 //!< TXFIFO threshold flag
#define USART_ISR_RXFT  			0x04000000 //!< RXFIFO threshold flag
#define USART_ISR_TCBGT  			0x02000000 //!< Transmission complete before guard time flag
#define USART_ISR_RXFF  			0x01000000 //!< RXFIFO full
#define USART_ISR_TXFE  		 	0x00800000 //!< TXFIFO empty
#define USART_ISR_REACK  		 	0x00400000 //!< Receive enable acknowledge flag
#define USART_ISR_TEACK  			0x00200000 //!< Transmit enable acknowledge flag
#define USART_ISR_WUF  				0x00100000 //!< Wakeup from low-power mode flag
#define USART_ISR_RWU   			0x00080000 //!< Receiver wakeup from Mute mode
#define USART_ISR_SBKF  			0x00040000 //!< Send break flag
#define USART_ISR_CMF  				0x00020000 //!< Character match flag
#define USART_ISR_BUSY  			0x00010000 //!< Busy flag
#define USART_ISR_ABRF  			0x00008000 //!< Auto baud rate flag
#define USART_ISR_ABRE  			0x00004000 //!< Auto baud rate error
#define USART_ISR_UDR  				0x00002000 //!< SPI slave underrun error flag
#define USART_ISR_EOBF  			0x00001000 //!< End of block flag
#define USART_ISR_RTOF  			0x00000800 //!< Receiver timeout
#define USART_ISR_CTS  				0x00000400 //!< CTS flag
#define USART_ISR_CTSIF  			0x00000200 //!< CTS interrupt flag
#define USART_ISR_LBDF  			0x00000100 //!< LIN break detection flag
#define USART_ISR_TXFNF  			0x00000080 //!< TXFIFO not full
#define USART_ISR_TC  				0x00000040 //!< Transmission complete
#define USART_ISR_RXFNE  			0x00000020 //!< RXFIFO not empty
#define USART_ISR_IDLE  			0x00000010 //!< Idle line detected
#define USART_ISR_ORE  				0x00000008 //!< Overrun error
#define USART_ISR_NE  				0x00000004 //!< Noise detection flag
#define USART_ISR_FE  				0x00000002 //!< Framing error
#define USART_ISR_PE  				0x00000001 //!< Parity error
/** @} */


/** @name USART_ISR:	(usart Offset: 0x1C) USART interrupt and status register [alternate] (USART_ISR) 	  */
/** @{ */
#define USART_ISR_TXFT 				0x08000000 //!< TXFIFO threshold flag
#define USART_ISR_RXFT 				0x04000000 //!< RXFIFO threshold flag
#define USART_ISR_TCBGT 			0x02000000 //!< Transmission complete before guard time flag
#define USART_ISR_RXFF 				0x01000000 //!< RXFIFO full
#define USART_ISR_TXFE 				0x00800000 //!< TXFIFO empty
#define USART_ISR_REACK 			0x00400000 //!< Receive enable acknowledge flag
#define USART_ISR_TEACK 			0x00200000 //!< Transmit enable acknowledge flag
#define USART_ISR_WUF 				0x00100000 //!< Wakeup from low-power mode flag
#define USART_ISR_RWU 				0x00080000 //!< Receiver wakeup from Mute mode
#define USART_ISR_SBKF 				0x00040000 //!< Send break flag
#define USART_ISR_CMF 				0x00020000 //!< Character match flag
#define USART_ISR_BUSY 				0x00010000 //!< Busy flag
#define USART_ISR_ABRF 				0x00008000 //!< Auto baud rate flag
#define USART_ISR_ABRE 				0x00004000 //!< Auto baud rate error
#define USART_ISR_UDR 				0x00002000 //!< SPI slave underrun error flag
#define USART_ISR_EOBF 				0x00001000 //!< End of block flag
#define USART_ISR_RTOF 				0x00000800 //!< Receiver timeout
#define USART_ISR_CTS 				0x00000400 //!< CTS flag
#define USART_ISR_CTSIF 			0x00000200 //!< CTS interrupt flag
#define USART_ISR_LBDF 				0x00000100 //!< LIN break detection flag
#define USART_ISR_TXFNF 			0x00000080 //!< TXFIFO not full
#define USART_ISR_TC 				0x00000040 //!< Transmission complete
#define USART_ISR_RXFNE 			0x00000020 //!< RXFIFO not empty
#define USART_ISR_IDLE 				0x00000010 //!< Idle line detected
#define USART_ISR_ORE 				0x00000008 //!< Overrun error
#define USART_ISR_NE 				0x00000004 //!< Noise detection flag
#define USART_ISR_FE 				0x00000002 //!< Framing error
#define USART_ISR_PE 				0x00000001 //!< Parity error
/** @} */

/** @name USART_ICR:	(usart Offset: 0x20) Interrupt flag clear register	  */
/** @{ */
#define USART_ICR_WUCF 				0x00100000 //!< Wakeup from low-power mode clear flag
#define USART_ICR_CMCF  			0x00020000 //!< Character match clear flag
#define USART_ICR_UDRCF 			0x00002000 //!< SPI slave underrun clear flag
#define USART_ICR_EOBCF  			0x00001000 //!< End of block clear flag
#define USART_ICR_RTOCF 			0x00000800 //!< Receiver timeout clear flag
#define USART_ICR_CTSCF 			0x00000200 //!< CTS clear flag
#define USART_ICR_LBDCF  			0x00000100 //!< LIN break detection clear flag
#define USART_ICR_TCBGTCF  	 		0x00000080 //!< Transmission complete before Guard time clear flag
#define USART_ICR_TCCF  			0x00000040 //!< Transmission complete clear flag
#define USART_ICR_TXFECF  			0x00000020 //!< TXFIFO empty clear flag
#define USART_ICR_IDLECF  			0x00000010 //!< Idle line detected clear flag
#define USART_ICR_ORECF  			0x00000008 //!< Overrun error clear flag
#define USART_ICR_NECF   			0x00000004 //!< Noise detected clear flag
#define USART_ICR_FECF  			0x00000002 //!< Framing error clear flag
#define USART_ICR_PECF  			0x00000001 //!< Parity error clear flag
/** @} */

/** @name USART_RDR:	(usart Offset: 0x24) Receive data register    	      */
/** @{ */
#define USART_RDR_RDR               0x000001FF //!< Data value */
/** @} */

/** @name USART_TDR:	(usart Offset: 0x28) Transmit data register  	      */
/** @{ */
#define USART_TDR_TDR 	0x000001FF //!< Transmit data value
/** @} */

/** @name USART_PRESC:  (usart Offset: 0x2C) USART clock Prescaler register   */
#define USART_PRESC_PRESCALER		0x0000000F //!< Clock prescaler
/** @} */ // @relates USART_TypeDef

#define USART_STATUS_TC 	USART_ISR_TC		//!< TC flag for H7 family
#define USART_STATUS_TXE 	USART_ISR_TXFNF		//!< TXE flag for H7 family
#define USART_STATUS_RXNE 	USART_ISR_RXFNE		//!< RXNE flag for H7 family
#define USART_STATUS_IDLE 	USART_ISR_IDLE		//!< IDLE flag for H7 family
#define USART_STATUS_ORE 	USART_ISR_ORE		//!< Overrun flag for H7 family

#define USART_STATUS_RXNEIE USART_CR1_RXFNEIE	//!< RXNE enable flag for H7 family
#define USART_STATUS_IDLEIE USART_CR1_IDLEIE	//!< IDLE enableflag for H7 family

unsigned int set_usart_baudrate(USART_TypeDef* usart, uint32_t periph_id, uint32_t rate);

/// Maskable status bits
#define USART_SR_MASKABLE (USART_CR1_IDLEIE | USART_CR1_RXFNEIE | USART_CR1_TCIE | USART_CR1_TXFEIE)

/// Nonmaskable status bits
#define USART_SR_NOMASKABLE (USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE)

/// ERROR status bits
#define USART_SR_ERRORS (USART_SR_PE | USART_SR_FE | USART_SR_NE | USART_SR_ORE)

#define get_usart_sr(uart) (uart->USART_ISR)		//!< interrupt status for H7 family

#define USART_CR1_TXEIE USART_CR1_TXFEIE

#define get_usart_imr(uart) \
	((uart->USART_CR1 & USART_SR_MASKABLE) | \
	 ((uart->USART_CR1 >> 8) & 1) | \
     USART_SR_NOMASKABLE )	//!< interrupt mask for H7 family

#define get_usart_tdr(uart) (uart->USART_TDR)	//!< transmit data register for H7
#define get_usart_rdr(uart) (uart->USART_RDR)	//!< receive data register for H7
#define clr_usart_idle(uart) uart->USART_ICR=USART_ICR_IDLECF	//!< clear IDLE flag
#define clr_usart_over(uart) uart->USART_ICR=USART_ICR_ORECF	//!< Overrun error clear

#define enable_usart_drv_ints(uart, f) (uart->USART_CR1 |= (f))		//!< enable usart interrupts
#define disable_usart_drv_ints(uart, f) (uart->USART_CR1 &= ~(f)) 	//!< disable usart interrupts

#endif /* USART_H7_H_ */

/** @} */
