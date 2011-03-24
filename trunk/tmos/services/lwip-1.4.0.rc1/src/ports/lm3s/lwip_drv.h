/*
 * lwip_drv.h
 *
 *  Created on: 22.03.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LWIP_DRV_H_
#define LWIP_DRV_H_

#include <tmos.h>
#include <ethernet_lm3s.h>

struct LWIP_DRIVER_DATA
{
	unsigned char	deviceState;	//!< LWIP_STATE_XXX
};

struct LWIP_DRIVER_MODE
{
	unsigned int	baudrate;	//!< baudrate for the mode
};

struct LWIP_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;		//!< standard driver info
	MAC_Type *			hw_base;	//!< pointer to the hardware peripheral
	LWIP_DRIVER_DATA* 	drv_data;	//!< pointer to the driver data
};


void LWIP_DCR(LWIP_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param);
void LWIP_DSR(LWIP_DRIVER_INFO* drv_info, HANDLE hnd);
void LWIP_ISR(LWIP_DRIVER_INFO* drv_info );

#endif /* LWIP_DRV_H_ */
