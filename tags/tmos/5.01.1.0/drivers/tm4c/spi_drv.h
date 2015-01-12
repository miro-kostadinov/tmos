/*
 * spi_drv.h
 *
 *  Created on: Sep 4, 2014
 *      Author: miro
 */

#ifndef SPI_DRV_H_
#define SPI_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>
#include <mcu_cpp.h>

/** SPI Driver mode structure **/
struct SPI_DRIVER_MODE
{
    unsigned int	cs_pin;		//!< doc!
    unsigned int	cr0_reg;	//!< doc!
};

/** SPI Driver data structure **/
struct SPI_DRIVER_DATA
{
    HANDLE			waiting;	//!< waiting to be processed
    HANDLE			pending;	//!< currently processed handle, or list of handles with locked access.
    Task *		 	locker;		//!< Task (or other client) which have locked the driver for multiple transfers whithin single CS
	unsigned short 	cnt;		//!< doc!
	unsigned short 	buffered;	//!< doc!
};

/** SPI Driver Info structure **/
struct SPI_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< doc!
	QSSI_TypeDef*		hw_base;		//!< doc!
	SPI_DRIVER_DATA* 	drv_data;		//!< doc!
	PIN_DESC		 	spi_pins[4];	//!< zero terminated PIN_DESC list

};

void SPI_DCR(const SPI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void SPI_DSR(const SPI_DRIVER_INFO* drv_info, HANDLE hnd);
void SPI_ISR(const SPI_DRIVER_INFO* drv_info );




#endif /* SPI_DRV_H_ */
