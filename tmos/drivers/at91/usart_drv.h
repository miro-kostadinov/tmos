/**
 * @ingroup	 drv_at91
 * @defgroup DRV_AT91_USART Universal Synchronous Asynchronous (USART) Driver
 * Interface for Universal Synchronous Asynchronous Receiver Transceiver (USART)
 *  driver.
 * @{
 *
 *
 * @file     drivers/at91/usart_drv.h
 * @ingroup	 DRV_AT91_USART
 * @brief    USART driver header
 *
 * @date     17. December 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef USART_DRV_H_
#define USART_DRV_H_

#include <tmos.h>
#include <usart.h>
#include <pio_drv.h>
#include <uart_drv.h>

#ifdef __cplusplus
 extern "C" {
#endif

/** USART Driver Info structure **/
struct USART_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Usart *				hw_base;		//!< pointer to the hardware peripheral
	UART_DRIVER_DATA 	drv_data;		//!< pointer to the driver data
	GPIO_STRU			pins;			//!< RX/TX pins
	unsigned short		buf_size;		//!< size of the rx_buf
};
/** USART Driver Info */
typedef const USART_DRIVER_INFO* USART_INFO;

void USART_DCR(USART_INFO drv_info, unsigned int reason, HANDLE param);
void USART_DSR(USART_INFO drv_info, HANDLE hnd);
void USART_ISR(USART_INFO drv_info );

#ifdef __cplusplus
}
#endif


#endif /* USART_DRV_H_ */
