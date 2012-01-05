/*
 * uart_driver.h
 *
 *  Created on: 2010-3-19
 *
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

//#include "gpio_drv.h"
#include <uart_lm3s.h>
#include <gpio_lm3s.h>

#define RX_PIN	0
#define TX_PIN	1
#define RTS_PIN	2
#define CTS_PIN	3

#define UART_RX_TIMEOUT			  1
#define UART_HW_FLOWCTRL		  2

/** UART Driver mode structure **/
struct UART_DRIVER_MODE
{
	unsigned int baudrate;		//!< doc!
	unsigned int mode;			//!< doc!
	unsigned short int rx_tout;	//!< doc!
	unsigned short int hw_flow;	//!< doc!
};

#define RX_BUF_SIZE	(256)

/** UART Driver data structure **/
struct UART_DRIVER_DATA
{
	unsigned int cnt;			//!< doc!
	HANDLE hnd_rcv;				//!< doc!
	HANDLE hnd_snd;				//!< doc!
	unsigned char * rx_ptr;		//!< doc!
	unsigned char * rx_wrptr;	//!< doc!
	unsigned char rx_buf[RX_BUF_SIZE];	//!< doc!
	unsigned int rx_remaining;	//!< doc!
	struct UART_DRIVER_MODE mode;	//!< doc!
};

/** UART Driver Info structure **/
struct UART_DRIVER_INFO
{
	DRIVER_INFO_Type  info;			//!< Standard driver info
	UART_Type *	   	  hw_base;		//!< UART Hardware registers
	PIN_DESC	 	  uart_pins[5]; //!< pins: Rx, Tx, RTS, CTS, 0
	UART_DRIVER_DATA* drv_data;		//!< driver data
};

void dcr_SerialDriver(UART_DRIVER_INFO* driver, unsigned int reason, HANDLE hnd);
void dsr_SerialDriver(UART_DRIVER_INFO* driver, HANDLE hnd);
void isr_SerilaDriver(UART_DRIVER_INFO* driver );

#endif /* UART_DRIVER_H_ */
