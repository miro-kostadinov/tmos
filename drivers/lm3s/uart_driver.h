/*
 * uart_driver.h
 *
 *  Created on: 2010-3-19
 *
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "gpio_drv.h"

#define UART_LIST_ALL_PINS 0
#define RTS_PIN	0
#define CTS_PIN	1

#define UART_LIST_RX_TX_PINS	  2
#define RX_PIN	2
#define TX_PIN	3

#define UART_RX_TIMEOUT			  1
#define UART_HW_FLOWCTRL		  2

struct UART_DRIVER_MODE
{
	unsigned int baudrate;		//!< doc!
	unsigned int mode;			//!< doc!
	unsigned short int rx_tout;	//!< doc!
	unsigned short int hw_flow;	//!< doc!
};

#define RX_BUF_SIZE	(256)

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

struct UART_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;	//!< doc!
	UART_Type *	   	 	hw_base;	//!< doc!

	unsigned int 		uart_pins[5]; //!< pin RTS, pin CTS, pin Rx , pin Tx,
	UART_DRIVER_DATA * 	drv_data;	//!< doc!
};

void dcr_SerialDriver(UART_DRIVER_INFO* driver, unsigned int reason, HANDLE hnd);
void dsr_SerialDriver(UART_DRIVER_INFO* driver, HANDLE hnd);
void isr_SerilaDriver(UART_DRIVER_INFO* driver );

#endif /* UART_DRIVER_H_ */
