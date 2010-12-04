/*
 * spi_drv.h
 *
 *  Created on: 2010-4-21
 *      Author: Diado
 */

#ifndef SPI_DRV_H_
#define SPI_DRV_H_

struct SPI_DRIVER_MODE
{
    unsigned int	cs_pin;		//!< doc!
    unsigned int	cr0_reg;	//!< doc!
};


struct SPI_DRIVER_DATA
{
    HANDLE			waiting;	//!< waiting to be processed
    HANDLE			pending;	//!< currently processed handle, or list of handles with locked access.
    Task *		 	locker;		//!< Task (or other client) which have locked the driver for multiple transfers whithin single CS
	unsigned short 	cnt;		//!< doc!
	unsigned short 	buffered;	//!< doc!
};

struct SPI_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< doc!
	SSI_Type *			hw_base;		//!< doc!
	SPI_DRIVER_DATA * 	drv_data;		//!< doc!
	unsigned int	 	spi_pins[4];	//!< doc!

};

void dcr_SPI_driver(SPI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void dsr_SPI_driver(SPI_DRIVER_INFO* drv_info, HANDLE hnd);
void isr_SPI_driver(SPI_DRIVER_INFO* drv_info );



#endif /* SPI_DRV_H_ */
