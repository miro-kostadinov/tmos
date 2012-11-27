/*
 * spi_drv.h
 *
 *  Created on: Nov 20, 2012
 *      Author: miro
 */

#ifndef SPI_DRV_H_
#define SPI_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>
#include <mcu_cpp.h>

#ifndef USE_SPI_DMA_DRIVER
#define USE_SPI_DMA_DRIVER 1		//Enable DMA by defaulr
#endif

/** SPI Driver mode structure **/
struct SPI_DRIVER_MODE
{
    PIN_DESC cs_pin;		//!< chip select pin
	uint16_t spi_cr1;		//!< SPI_CR1 register value
	uint16_t spi_cr2;		//!< SPI_CR2 register value (FRF and SSOE only)
};

/** SPI Driver data structure **/
struct SPI_DRIVER_DATA
{
    HANDLE		waiting;	//!< waiting to be processed
    HANDLE		pending;	//!< currently processed handle, or list of handles with locked access.
    Task *	 	locker;		//!< Task (or other client) which have locked the driver for multiple transfers whithin single CS
	uint16_t 	cnt;		//!< Number of open handles
#if USE_SPI_DMA_DRIVER
	CHandle		rx_dma_hnd;
	CHandle		tx_dma_hnd;
#endif
};


/** SPI Driver Info structure **/
struct SPI_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	SPI_TypeDef*		hw_base;		//!< SPI peripheral
	SPI_DRIVER_DATA* 	drv_data;		//!< driver data
	PIN_DESC		 	spi_pins[4];	//!< zero terminated PIN_DESC list
#if USE_SPI_DMA_DRIVER
	DMA_DRIVER_MODE		rx_dma_mode;
	DMA_DRIVER_MODE		tx_dma_mode;
#endif
};

void SPI_DCR(SPI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void SPI_DSR(SPI_DRIVER_INFO* drv_info, HANDLE hnd);
void SPI_ISR(SPI_DRIVER_INFO* drv_info);


#endif /* SPI_DRV_H_ */
