/*
 * gpio_drv.h
 *
 *  Created on: 2010-4-22
 *      Author: stanly
 */

#ifndef GPIO_DRV_H_
#define GPIO_DRV_H_

#include <gpio_lm3s.h>

/** GPIO Driver data structure **/
struct GPIO_DRIVER_DATA
{
	HANDLE 			waiting;		//!< doc!
#if ( USE_GPIO_EXPANDER !=	0)
	unsigned int	sreg_val;		//!< doc!
#endif
};

/** GPIO Driver Info structure **/
struct GPIO_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;		//!< doc!
	GPIO_Type *	   	 	hw_base;	//!< doc!
	unsigned int		port_num;	//!< doc!
	GPIO_DRIVER_DATA * 	drv_data;	//!< doc!
};


#define PIOHND_WAITING		0x01
#define PIOHND_INTPENDING	0x02

void dcr_GPIO_driver(GPIO_DRIVER_INFO * drv_info, unsigned int reason, HANDLE hnd);
void dsr_GPIO_driver(GPIO_DRIVER_INFO * drv_info, HANDLE hnd);
void isr_GPIO_driver(GPIO_DRIVER_INFO * drv_info );


#endif /* GPIO_DRV_H_ */
