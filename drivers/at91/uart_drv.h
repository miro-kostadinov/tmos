/**
 * @ingroup	 drv_at91
 * @defgroup DRV_AT91_UART Universal Asynchronous Receiver Transmitter Driver
 * Interface for Universal Asynchronous Receiver Transmitter (UART) driver.
 * @{
 * This is ported from DBGU driver, here are the original comments:
 * Date:          2008-06-01
 * Description:   TMOS DBU driver
 *
 *	Implements serial communication over the DBGU module
 *  Hardware usage:
 *		- DBGU, interrupt is checked in the SYS_DRV
 *		- DTXD and RXD pin
 *	Problems:
 *
 *		if 2 handles are pending because of the double buffering we can't
 *		be certain which one was active when OVRE, PAR or FRAME occurred
 *
 * 		when we see 1 error we have to clear it, but we can clear only
 *		all errors including the ones we don't know yet ... So we may miss
 *		some error.
 *
 * HANDLE update:
 * len - decremented with the transfered bytes
 * src/dst - incremented with the transfered bytes
 *
 *
 * @file     drivers/at91/uart_drv.h
 * @ingroup	 DRV_AT91_UART
 * @brief    UART driver header
 *
 * @date     02. December 2010
 * @author	 Miroslav Kostadinov
 * @}
*/

#ifndef UART_DRV_H_
#define UART_DRV_H_


#include <tmos.h>
#include <uart.h>

struct UART_DRIVER_DATA_STRU
{
    unsigned short	cnt;		//signal flags for pending hanles (low-byte is oldest)
    unsigned char 	state;
    unsigned char	drv_indx;
    HANDLE			hnd_rcv;
    HANDLE			hnd_snd;
    unsigned int 	mode;
    unsigned int 	baudrate;
    unsigned int  	rtout;
    unsigned char * rx_ptr;
    unsigned char	rx_buf[];
};
typedef UART_DRIVER_DATA_STRU* UART_DRIVER_DATA;

#ifdef __cplusplus
 extern "C" {
#endif


struct DRV_UART_MODE_STRU
{
    unsigned int	baudrate;
    unsigned int	mode;
    unsigned short  rtout;
    unsigned short  ttgr;
};
typedef const DRV_UART_MODE_STRU * DRV_UART_MODE;

struct UART_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Uart *				hw_base;		//!< pointer to the hardware peripheral
	UART_DRIVER_DATA 	drv_data;
	size_t				buf_size;
};
/** UART Driver Info */
typedef const UART_DRIVER_INFO* UART_INFO;

void UART_DCR(UART_INFO drv_info, unsigned int reason, HANDLE param);
void UART_DSR(UART_INFO drv_info, HANDLE hnd);
void UART_ISR(UART_INFO drv_info );


#ifdef __cplusplus
}
#endif


#endif /* UART_DRV_H_ */
