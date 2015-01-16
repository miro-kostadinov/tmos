/*
 * usart_f0.h
 *
 *  Created on: Oct 28, 2014
 *      Author: miro
 */

#ifndef USART_F0_H_
#define USART_F0_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Universal Synchronous Asynchronous Receiver Transmitter
 ******************************************************************************/
typedef struct
{
  __IO uint32_t USART_CR1;		//!< (usart Offset: 0x00) Control register 1
  __IO uint32_t USART_CR2;		//!< (usart Offset: 0x04) Control register 2
  __IO uint32_t USART_CR3;		//!< (usart Offset: 0x08) Control register 3
  __IO uint32_t USART_BRR;		//!< (usart Offset: 0x0C) Baud rate register
  __IO uint32_t USART_GTPR;		//!< (usart Offset: 0x10) Guard time and prescaler register
  __IO uint32_t USART_RTOR;		//!< (usart Offset: 0x14) Receiver timeout register
  __IO uint32_t USART_RQR;		//!< (usart Offset: 0x18) Request register
  __IO uint32_t USART_ISR;		//!< (usart Offset: 0x1C) Interrupt & status register
  __IO uint32_t USART_ICR;		//!< (usart Offset: 0x20) Interrupt flag clear register
  __IO uint32_t USART_RDR;		//!< (usart Offset: 0x24) Receive data register
  __IO uint32_t USART_TDR;		//!< (usart Offset: 0x28) Transmit data register
} USART_TypeDef;

/*******************************************************************************
 * @defgroup USART_regs_define
 * @{
 ******************************************************************************/

/** @defgroup USART_CR1:	(usart Offset: 0x00) Control register 1           */
#define USART_CR1_EOBIE             0x08000000 //!< End of Block interrupt enable
#define USART_CR1_RTOIE             0x04000000 //!< Receiver timeout interrupt enable
#define USART_CR1_DEAT              0x03E00000 //!< Driver Enable assertion time
#define USART_CR1_DEDT              0x001F0000 //!< Driver Enable deassertion time
#define USART_CR1_OVER8             0x00008000 //!< USART Oversmapling 8-bits
#define USART_CR1_CMIE              0x00004000 //!< Character match interrupt enable
#define USART_CR1_MME               0x00002000 //!< Mute mode enable
#define USART_CR1_M                 0x00001000 //!< Word length
#define USART_CR1_WAKE              0x00000800 //!< Wakeup method
#define USART_CR1_PCE               0x00000400 //!< Parity Control Enable
#define USART_CR1_PS                0x00000200 //!< Parity Selection
#define USART_CR1_PEIE              0x00000100 //!< PE Interrupt Enable
#define USART_CR1_TXEIE             0x00000080 //!< TXE Interrupt Enable
#define USART_CR1_TCIE              0x00000040 //!< Transmission Complete Interrupt Enable
#define USART_CR1_RXNEIE            0x00000020 //!< RXNE Interrupt Enable
#define USART_CR1_IDLEIE            0x00000010 //!< IDLE Interrupt Enable
#define USART_CR1_TE                0x00000008 //!< Transmitter Enable
#define USART_CR1_RE                0x00000004 //!< Receiver Enable
#define USART_CR1_UESM              0x00000002 //!< USART enable in Stop mode
#define USART_CR1_UE                0x00000001 //!< USART Enable
/** @} */

/** @defgroup USART_CR2:	(usart Offset: 0x04) Control register 2           */
#define USART_CR2_ADD               0xFF000000 //!< Address of the USART node
#define USART_CR2_ADD_Set(x)  		((x)<<24)  //!<  Address set
#define USART_CR2_ADD_Get(x)   		((x)>>24)  //!<  Address get
#define USART_CR2_RTOEN             0x00800000 //!< Receiver timeout enable
#define USART_CR2_ABRMOD            0x00600000 //!< Auto baud rate mode
#define USART_CR2_ABREN             0x00100000 //!< Auto baud rate enable
#define USART_CR2_MSBFIRST          0x00080000 //!< Most significant bit first
#define USART_CR2_DATAINV           0x00040000 //!< Binary data inversion
#define USART_CR2_TXINV             0x00020000 //!< TX pin active level inversion
#define USART_CR2_RXINV             0x00010000 //!< RX pin active level inversion
#define USART_CR2_SWAP              0x00008000 //!< Swap TX/RX pins
#define USART_CR2_LINEN             0x00004000 //!< LIN mode enable
#define USART_CR2_STOP              0x00003000 //!< STOP bits
#define USART_CR2_STOP_1b           0x00000000 //!<  1 Stop bit
#define USART_CR2_STOP_2b           0x00002000 //!<  2 Stop bits
#define USART_CR2_STOP_1b5          0x00003000 //!<  1.5 Stop bit
#define USART_CR2_CLKEN             0x00000800 //!< Clock Enable
#define USART_CR2_CPOL              0x00000400 //!< Clock Polarity
#define USART_CR2_CPHA              0x00000200 //!< Clock Phase
#define USART_CR2_LBCL              0x00000100 //!< Last Bit Clock pulse
#define USART_CR2_LBDIE             0x00000040 //!< LIN Break Detection Interrupt Enable
#define USART_CR2_LBDL              0x00000020 //!< LIN Break Detection Length
#define USART_CR2_ADDM7             0x00000010 //!< 7-bit Address Detection/4-bit Address Detection
/** @} */

/** @defgroup USART_CR3:	(usart Offset: 0x08) Control register 3           */
#define USART_CR3_WUFIE             0x00400000 //!< Wakeup from Stop mode interrupt enable
#define USART_CR3_WUS               0x00300000 //!< Wakeup from Stop mode interrupt flag selection
#define USART_CR3_WUS_ADD           0x00000000 //!<  WUF active on address match (as defined by ADD[7:0] and ADDM7)
#define USART_CR3_WUS_START         0x00200000 //!<  WUF active on Start bit detection
#define USART_CR3_WUS_RXNE          0x00300000 //!<  WUF active on RXNE
#define USART_CR3_SCARCNT           0x000E0000 //!< Smartcard auto-retry count
#define USART_CR3_DEP	            0x00008000 //!< Driver enable polarity selection
#define USART_CR3_DEM               0x00004000 //!< Driver enable mode
#define USART_CR3_DDRE              0x00002000 //!< DMA Disable on Reception Error
#define USART_CR3_OVRDIS            0x00001000 //!< Overrun Disable
#define USART_CR3_ONEBIT            0x00000800 //!< One Bit method
#define USART_CR3_CTSIE             0x00000400 //!< CTS Interrupt Enable
#define USART_CR3_CTSE              0x00000200 //!< CTS Enable
#define USART_CR3_RTSE              0x00000100 //!< RTS Enable
#define USART_CR3_DMAT              0x00000080 //!< DMA Enable Transmitter
#define USART_CR3_DMAR              0x00000040 //!< DMA Enable Receiver
#define USART_CR3_SCEN              0x00000020 //!< Smartcard mode enable
#define USART_CR3_NACK              0x00000010 //!< Smartcard NACK enable
#define USART_CR3_HDSEL             0x00000008 //!< Half-Duplex Selection
#define USART_CR3_IRLP              0x00000004 //!< IrDA Low-Power
#define USART_CR3_IREN              0x00000002 //!< IrDA mode Enable
#define USART_CR3_EIE               0x00000001 //!< Error Interrupt Enable
/** @} */

/** @defgroup USART_BRR:	(usart Offset: 0x0C) Baud rate register           */
/** @} */

/** @defgroup USART_GTPR:	(usart Offset: 0x10) Guard time and prescaler register */
#define USART_GTPR_PSC              0x00FF //!< PSC[7:0] bits (Prescaler value) */
#define USART_GTPR_PSC_Set(x)  		 (x)   //!< Prescaler value set
#define USART_GTPR_PSC_Get(x)  ((x)& 0xFF) //!< Prescaler value get
#define USART_GTPR_GT               0xFF00 //!< Guard time value */
#define USART_GTPR_GT_Set(x)  	  ((x)<<8) //!< Guard time value set
#define USART_GTPR_GT_Get(x) (((x)>>8)&0xFF) //!< Guard time value get
/** @} */

/** @defgroup USART_RTOR:	(usart Offset: 0x14) Receiver timeout register    */
/** @} */

/** @defgroup USART_RQR:	(usart Offset: 0x18) Request register             */
#define USART_RQR_TXFRQ             0x00000010 //!< Transmit data flush request
#define USART_RQR_RXFRQ             0x00000008 //!< Receive data flush request
#define USART_RQR_MMRQ              0x00000004 //!< Mute mode request
#define USART_RQR_SBKRQ             0x00000002 //!< Send break request
#define USART_RQR_ABRRQ             0x00000001 //!< Auto baud rate request
/** @} */

/** @defgroup USART_ISR:	(usart Offset: 0x1C) Interrupt & status register  */
#define USART_ISR_REACK             0x00400000 //!< Receive enable acknowledge flag
#define USART_ISR_TEACK             0x00200000 //!< Transmit enable acknowledge flag
#define USART_ISR_WUF               0x00100000 //!< Wakeup from Stop mode flag
#define USART_ISR_RWU               0x00080000 //!< Receiver wakeup from Mute mode
#define USART_ISR_SBKF              0x00040000 //!< Send break flag
#define USART_ISR_CMF               0x00020000 //!< Character match flag
#define USART_ISR_BUSY              0x00010000 //!< Busy flag
#define USART_ISR_ABRF              0x00008000 //!< Auto baud rate flag
#define USART_ISR_ABRE              0x00004000 //!< Auto baud rate error
#define USART_ISR_EOBF              0x00001000 //!< End of block flag
#define USART_ISR_RTOF              0x00000800 //!< Receiver timeout
#define USART_ISR_CTS               0x00000400 //!< CTS Flag
#define USART_ISR_CTSI              0x00000200 //!< CTS interrupt flag
#define USART_ISR_LBD               0x00000100 //!< LIN Break Detection Flag
#define USART_ISR_TXE               0x00000080 //!< Transmit Data Register Empty
#define USART_ISR_TC                0x00000040 //!< Transmission Complete
#define USART_ISR_RXNE              0x00000020 //!< Read Data Register Not Empty
#define USART_ISR_IDLE              0x00000010 //!< IDLE line detected
#define USART_ISR_ORE               0x00000008 //!< OverRun Error
#define USART_ISR_NF                0x00000004 //!< Noise Error Flag
#define USART_ISR_FE                0x00000002 //!< Framing Error
#define USART_ISR_PE          	    0x00000001 //!< Parity Error
/** @} */

/** @defgroup USART_ICR:	(usart Offset: 0x20) Interrupt flag clear register*/
#define USART_ICR_WUCF              0x00100000 //!< Wakeup from Stop mode clear flag
#define USART_ICR_CMCF              0x00020000 //!< Character match clear flag
#define USART_ICR_EOBCF             0x00001000 //!< End of timeout clear flag
#define USART_ICR_RTOCF             0x00000800 //!< Receiver timeout clear flag
#define USART_ICR_CTSCF             0x00000200 //!< CTS clear flag
#define USART_ICR_LBDCF             0x00000100 //!< LIN break detection clear flag
#define USART_ICR_TCCF              0x00000040 //!< Transmission complete clear flag
#define USART_ICR_IDLECF            0x00000010 //!< Idle line detected clear flag
#define USART_ICR_ORECF             0x00000008 //!< Overrun error clear flag
#define USART_ICR_NCF               0x00000004 //!< Noise detected clear flag
#define USART_ICR_FECF              0x00000002 //!< Framing error clear flag
#define USART_ICR_PECF         	    0x00000001 //!< Parity error clear flag
/** @} */

/** @defgroup USART_RDR:	(usart Offset: 0x24) Receive data register        */
#define USART_RDR_RDR               0x01FF //!< Data value */
/** @} */

/** @defgroup USART_TDR:	(usart Offset: 0x28) Transmit data register       */
/** @} */

/** @} */ // @defgroup USART_regs_define

#define USART_STATUS_TC 	USART_ISR_TC	//!< TC flag for F0 family
#define USART_STATUS_TXE 	USART_ISR_TXE	//!< TXE flag for F0 family
#define USART_STATUS_RXNE 	USART_ISR_RXNE	//!< RXNE flag for F0 family
#define USART_STATUS_IDLE 	USART_ISR_IDLE	//!< IDLE flag for F0 family
#define USART_STATUS_ORE 	USART_ISR_ORE	//!< Overrun flag for F2 family

#define USART_STATUS_RXNEIE USART_CR1_RXNEIE	//!< RXNE enable flag for F0 family
#define USART_STATUS_IDLEIE USART_CR1_IDLEIE	//!< IDLE enableflag for F0 family

unsigned int set_usart_baudrate(USART_TypeDef* usart, uint32_t periph_id, uint32_t rate);

#define USART_CR1_EOBIE             0x08000000 //!< End of Block interrupt enable
#define USART_CR1_RTOIE             0x04000000 //!< Receiver timeout interrupt enable
#define USART_CR1_DEAT              0x03E00000 //!< Driver Enable assertion time
#define USART_CR1_DEDT              0x001F0000 //!< Driver Enable deassertion time
#define USART_CR1_OVER8             0x00008000 //!< USART Oversmapling 8-bits
#define USART_CR1_CMIE              0x00004000 //!< Character match interrupt enable
#define USART_CR1_MME               0x00002000 //!< Mute mode enable
#define USART_CR1_M                 0x00001000 //!< Word length
#define USART_CR1_WAKE              0x00000800 //!< Wakeup method
#define USART_CR1_PCE               0x00000400 //!< Parity Control Enable
#define USART_CR1_PS                0x00000200 //!< Parity Selection
#define USART_CR1_PEIE              0x00000100 //!< PE Interrupt Enable
#define USART_CR1_TXEIE             0x00000080 //!< TXE Interrupt Enable
#define USART_CR1_TCIE              0x00000040 //!< Transmission Complete Interrupt Enable
#define USART_CR1_RXNEIE            0x00000020 //!< RXNE Interrupt Enable
#define USART_CR1_IDLEIE            0x00000010 //!< IDLE Interrupt Enable
#define USART_CR1_TE                0x00000008 //!< Transmitter Enable
#define USART_CR1_RE                0x00000004 //!< Receiver Enable
#define USART_CR1_UESM              0x00000002 //!< USART enable in Stop mode
#define USART_CR1_UE                0x00000001 //!< USART Enable
/// Maskable status bits
#define USART_SR_MASKABLE ( USART_CR1_EOBIE | USART_CR1_RTOIE | USART_CR1_TXEIE \
		| USART_CR1_TCIE | USART_CR1_RXNEIE | USART_CR1_IDLEIE)

/// Nonmaskable status bits
#define USART_SR_NOMASKABLE (USART_ISR_FE | USART_ISR_NF | USART_ISR_ORE)

/// ERROR status bits
#define USART_SR_ERRORS (USART_ISR_PE | USART_ISR_FE | USART_ISR_NF | USART_ISR_ORE)

#define get_usart_sr(uart) (uart->USART_ISR)	//!< interrupt status for F0 family

#define get_usart_imr(uart) \
	((uart->USART_CR1 & USART_SR_MASKABLE) | \
	 ((uart->USART_CR1 >> 8) & 1) | \
     USART_SR_NOMASKABLE )	//!< interrupt mask for F0 family

#define get_usart_tdr(uart) (uart->USART_TDR)	//!< transmit data register for F2
#define get_usart_rdr(uart) (uart->USART_RDR)	//!< receive data register for F2
#define clr_usart_idle(uart) uart->USART_ICR=USART_ICR_IDLECF	//!< clear IDLE flag

#define enable_usart_drv_ints(uart, f) (uart->USART_CR1 |= (f))		//!< enable usart interrupts
#define disable_usart_drv_ints(uart, f) (uart->USART_CR1 &= ~(f)) 	//!< disable usart interrupts

#endif /* USART_F0_H_ */
