/*
 * exti_drv.h
 *
 *  Created on: Nov 1, 2012
 *      Author: miro
 */

#ifndef EXTI_DRV_H_
#define EXTI_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>
#include <mcu_cpp.h>

/** EXTI Driver data structure **/
struct EXTI_DRIVER_DATA
{
	HANDLE 			waiting;		//!< List of waiting for locked read handles
#if ( USE_GPIO_EXPANDER !=	0)
	unsigned int	sreg_val;		//!< Data for virtual port pins
#endif
};

/** EXTI Driver Info structure **/
struct EXTI_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;		//!< standard driver info
	EXTI_TypeDef*	   	hw_base;	//!< EXTI base address
	unsigned int		int_mask;	//!< External lines mask
	EXTI_DRIVER_DATA * 	drv_data;	//!< GPIO data
};


#define PIOHND_IDLE			0x00	//!< Handle is idle, no interrupts pending
#define PIOHND_WAITING		0x01	//!< Handle is waiting for interrupt
#define PIOHND_INTPENDING	0x02	//!< Interrupt received, but handles is not busy

#define GPIO_IRQn EXTI0_IRQn		//!< Use this to open handles

void EXTI_DCR(EXTI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void EXTI_DSR(EXTI_DRIVER_INFO* drv_info, HANDLE hnd);
void EXTI_ISR(EXTI_DRIVER_INFO* drv_info );


#endif /* EXTI_DRV_H_ */
