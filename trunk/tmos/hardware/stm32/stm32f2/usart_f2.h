/*
 * usart_f2.h
 *
 *  Created on: Nov 6, 2012
 *      Author: miro
 */

#ifndef USART_F2_H_
#define USART_F2_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Universal Synchronous Asynchronous Receiver Transmitter
 ******************************************************************************/
typedef struct
{
  __IO uint32_t USART_SR;		//!< (usart Offset: 0x00) Status register
  __IO uint32_t USART_DR;		//!< (usart Offset: 0x04) Data register
  __IO uint32_t USART_BRR;		//!< (usart Offset: 0x08) Baud rate register
  __IO uint32_t USART_CR1;		//!< (usart Offset: 0x0C) Control register 1
  __IO uint32_t USART_CR2;		//!< (usart Offset: 0x10) Control register 2
  __IO uint32_t USART_CR3;		//!< (usart Offset: 0x14) Control register 3
  __IO uint32_t USART_GTPR;		//!< (usart Offset: 0x18) Guard time and prescaler register
} USART_TypeDef;

/*******************************************************************************
 * @defgroup USART_regs_define
 * @{
 ******************************************************************************/

/** @defgroup USART_SR:		(usart Offset: 0x00) Status register				*/
#define USART_SR_PE          	    0x0001 //!< Parity Error
#define USART_SR_FE                 0x0002 //!< Framing Error
#define USART_SR_NE                 0x0004 //!< Noise Error Flag
#define USART_SR_ORE                0x0008 //!< OverRun Error
#define USART_SR_IDLE               0x0010 //!< IDLE line detected
#define USART_SR_RXNE               0x0020 //!< Read Data Register Not Empty
#define USART_SR_TC                 0x0040 //!< Transmission Complete
#define USART_SR_TXE                0x0080 //!< Transmit Data Register Empty
#define USART_SR_LBD                0x0100 //!< LIN Break Detection Flag
#define USART_SR_CTS                0x0200 //!< CTS Flag
/** @} */

/** @defgroup USART_DR:		(usart Offset: 0x04) Data register					*/
#define USART_DR_DR                 0x01FF //!< Data value */
/** @} */

/** @defgroup USART_BRR:	(usart Offset: 0x08) Baud rate register				*/
#define USART_BRR_DIV_Fraction      	0x000F 	   //!< Fraction of USARTDIV
#define USART_BRR_DIV_Fraction_Msk      0x000F 	   //!< Pin selection mask
#define USART_BRR_DIV_Fraction_Set(x)   (x)        //!< Pin selection set
#define USART_BRR_DIV_Fraction_Get(x)   ((x)& 0xF) //!< Pin selection get

#define USART_BRR_DIV_Mantissa    		0xFFF0 //!< Mantissa of USARTDIV
#define USART_BRR_DIV_Mantissa_Msk      0xFFF0 	   		  //!< Pin selection mask
#define USART_BRR_DIV_Mantissa_Set(x)   ((x)<<4)          //!< Pin selection set
#define USART_BRR_DIV_Mantissa_Get(x)   (((x)>>4)& 0xFFF) //!< Pin selection get
/** @} */

/** @defgroup USART_CR1:	(usart Offset: 0x0C) Control register 1				*/
#define USART_CR1_SBK               0x0001 //!< Send Break
#define USART_CR1_RWU               0x0002 //!< Receiver wakeup
#define USART_CR1_RE                0x0004 //!< Receiver Enable
#define USART_CR1_TE                0x0008 //!< Transmitter Enable
#define USART_CR1_IDLEIE            0x0010 //!< IDLE Interrupt Enable
#define USART_CR1_RXNEIE            0x0020 //!< RXNE Interrupt Enable
#define USART_CR1_TCIE              0x0040 //!< Transmission Complete Interrupt Enable
#define USART_CR1_TXEIE             0x0080 //!< TXE Interrupt Enable
#define USART_CR1_PEIE              0x0100 //!< PE Interrupt Enable
#define USART_CR1_PS                0x0200 //!< Parity Selection
#define USART_CR1_PCE               0x0400 //!< Parity Control Enable
#define USART_CR1_WAKE              0x0800 //!< Wakeup method
#define USART_CR1_M                 0x1000 //!< Word length
#define USART_CR1_UE                0x2000 //!< USART Enable
#define USART_CR1_OVER8             0x8000 //!< USART Oversmapling 8-bits
/** @} */

/** @defgroup USART_CR2:	(usart Offset: 0x10) Control register 2				*/
#define USART_CR2_ADD               0x000F //!< Address of the USART node
#define USART_CR2_ADD_Set(x)  		 (x)   //!< Address set
#define USART_CR2_ADD_Get(x)   	((x)& 0xF) //!< Address get

#define USART_CR2_LBDL              0x0020 //!< LIN Break Detection Length
#define USART_CR2_LBDIE             0x0040 //!< LIN Break Detection Interrupt Enable
#define USART_CR2_LBCL              0x0100 //!< Last Bit Clock pulse
#define USART_CR2_CPHA              0x0200 //!< Clock Phase
#define USART_CR2_CPOL              0x0400 //!< Clock Polarity
#define USART_CR2_CLKEN             0x0800 //!< Clock Enable

#define USART_CR2_STOP              0x3000 //!< STOP[1:0] bits (STOP bits)
#define USART_CR2_STOP_1b           0x0000 //!< 1 Stop bit
#define USART_CR2_STOP_0b5          0x1000 //!< 0.5 Stop bit (not available for UART4 & UART5)
#define USART_CR2_STOP_2b           0x2000 //!< 2 Stop bits
#define USART_CR2_STOP_1b5          0x3000 //!< 1.5 Stop bit (not available for UART4 & UART5)

#define USART_CR2_LINEN             0x4000 //!< LIN mode enable
/** @} */

/** @defgroup USART_CR3:	(usart Offset: 0x14) Control register 3				*/
#define USART_CR3_EIE               0x0001 //!< Error Interrupt Enable
#define USART_CR3_IREN              0x0002 //!< IrDA mode Enable
#define USART_CR3_IRLP              0x0004 //!< IrDA Low-Power
#define USART_CR3_HDSEL             0x0008 //!< Half-Duplex Selection
#define USART_CR3_NACK              0x0010 //!< Smartcard NACK enable
#define USART_CR3_SCEN              0x0020 //!< Smartcard mode enable
#define USART_CR3_DMAR              0x0040 //!< DMA Enable Receiver
#define USART_CR3_DMAT              0x0080 //!< DMA Enable Transmitter
#define USART_CR3_RTSE              0x0100 //!< RTS Enable
#define USART_CR3_CTSE              0x0200 //!< CTS Enable
#define USART_CR3_CTSIE             0x0400 //!< CTS Interrupt Enable
#define USART_CR3_ONEBIT            0x0800 //!< One Bit method
/** @} */

/** @defgroup USART_GTPR:	(usart Offset: 0x18) Guard time and prescaler register */
#define USART_GTPR_PSC              0x00FF //!< PSC[7:0] bits (Prescaler value) */
#define USART_GTPR_PSC_Set(x)  		 (x)   //!< Prescaler value set
#define USART_GTPR_PSC_Get(x)  ((x)& 0xFF) //!< Prescaler value get


#define USART_GTPR_GT               0xFF00 //!< Guard time value */
#define USART_GTPR_GT_Set(x)  	  ((x)<<8) //!< Guard time value set
#define USART_GTPR_GT_Get(x) (((x)>>8)&0xFF) //!< Guard time value get
/** @} */


/** @} */ // @defgroup USART_regs_define

#define USART_STATUS_TC 	USART_SR_TC		//!< TC flag for F2 family
#define USART_STATUS_TXE 	USART_SR_TXE	//!< TXE flag for F2 family
#define USART_STATUS_RXNE 	USART_SR_RXNE	//!< RXNE flag for F2 family
#define USART_STATUS_IDLE 	USART_SR_IDLE	//!< IDLE flag for F2 family
#define USART_STATUS_ORE 	USART_SR_ORE	//!< Overrun flag for F2 family

#define USART_STATUS_RXNEIE USART_CR1_RXNEIE	//!< RXNE enable flag for F2 family
#define USART_STATUS_IDLEIE USART_CR1_IDLEIE	//!< IDLE enableflag for F2 family

unsigned int get_usart_source_clk(unsigned int periph_id);

/// Maskable status bits
#define USART_SR_MASKABLE (USART_CR1_IDLEIE | USART_CR1_RXNEIE | USART_CR1_TCIE | USART_CR1_TXEIE)

/// Nonmaskable status bits
#define USART_SR_NOMASKABLE (USART_SR_FE | USART_SR_NE | USART_SR_ORE)

/// ERROR status bits
#define USART_SR_ERRORS (USART_SR_PE | USART_SR_FE | USART_SR_NE | USART_SR_ORE)

#define get_usart_sr(uart) (uart->USART_SR)		//!< interrupt status for F2 family

#define get_usart_imr(uart) \
	((uart->USART_CR1 & USART_SR_MASKABLE) | \
	 ((uart->USART_CR1 >> 8) & 1) | \
     USART_SR_NOMASKABLE )	//!< interrupt mask for F2 family

#define get_usart_tdr(uart) (uart->USART_DR)	//!< transmit data register for F2
#define get_usart_rdr(uart) (uart->USART_DR)	//!< receive data register for F2

#define enable_usart_drv_ints(uart, f) (uart->USART_CR1 |= (f))		//!< enable usart interrupts
#define disable_usart_drv_ints(uart, f) (uart->USART_CR1 &= ~(f)) 	//!< disable usart interrupts

#endif /* USART_F2_H_ */
