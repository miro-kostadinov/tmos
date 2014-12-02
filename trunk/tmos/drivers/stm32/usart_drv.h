/*
 * usart_drv.h
 *
 *  Created on: Nov 6, 2012
 *      Author: miro
 */

#ifndef USART_DRV_H_
#define USART_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>
#include <mcu_cpp.h>

#ifndef USART_DRV_RX_BUF_SIZE
#define USART_DRV_RX_BUF_SIZE	(256)
#endif

/** UART Driver mode structure **/
struct USART_DRIVER_MODE
{
	uint32_t baudrate;		//!< baudrate e.g. 9600, For M0 USART1 two MSB = clock source
	uint16_t mode_cr1;		//!< USART_CR1 register value
	uint16_t mode_cr2;		//!< USART_CR2 register value
	uint16_t mode_cr3;		//!< USART_CR3 register value
};

/** USART Driver data structure **/
struct USART_DRIVER_DATA
{
	unsigned int cnt;					//!< number of open handles
	HANDLE hnd_rcv;						//!< doc!
	HANDLE hnd_snd;						//!< doc!
	unsigned char * rx_ptr;				//!< doc!
	unsigned char * rx_wrptr;			//!< doc!
	unsigned int rx_remaining;			//!< doc!
	unsigned int usart_err;
	unsigned char rx_buf[USART_DRV_RX_BUF_SIZE];	//!< doc!
	struct USART_DRIVER_MODE mode;		//!< copy of the current USART mode
};

/** UART Driver Info structure **/
struct USART_DRIVER_INFO
{
	DRIVER_INFO_Type   info;			//!< Standard driver info
	USART_TypeDef* 	   hw_base;			//!< USART Hardware registers
	PIN_DESC	 	   uart_pins[5]; 	//!< pins: Rx, Tx, RTS, CTS, 0
	USART_DRIVER_DATA* drv_data;		//!< driver data
};

void USART_DCR(USART_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void USART_DSR(USART_DRIVER_INFO* drv_info, HANDLE hnd);
void USART_ISR(USART_DRIVER_INFO* drv_info);


#endif /* USART_DRV_H_ */
