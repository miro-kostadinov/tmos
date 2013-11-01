/*
 * tim_drv.h
 *
 *  Created on: Jun 3, 2013
 *      Author: miro
 */

#ifndef TIM_DRV_H_
#define TIM_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>
#include <mcu_cpp.h>


/** Timer Driver mode structure **/
struct TIMER_DRIVER_MODE
{
	uint8_t		tim_indx;		//!< timer index (1..X)
	uint8_t		ch_indx;		//!< channel index (0=control, 1-4=channel)
	uint16_t	dier;			//!< dma/interrupt enable register (TIM_DIER)
	uint32_t	arr_or_ccr;		//!< auto reload or capture/compare register (TIM_ARR or TIM_CCRx)
};

/** Timer Channel mode structure **/
struct TIMER_CHANNEL_MODE	//: public TIMER_DRIVER_MODE
{
	uint8_t		tim_indx;		//!< timer index (1..X)
	uint8_t		ch_indx;		//!< channel index (0=control, 1-4=channel)
	uint16_t	dier;			//!< dma/interrupt enable register (TIM_DIER)
	uint32_t	arr_or_ccr;		//!< auto reload or capture/compare register (TIM_ARR or TIM_CCRx)

	PIN_DESC	ch_pin;			//!< channel pin configuration
	uint8_t 	ccmr;			//!< capture/compare mode register (TIM_CCMR1/2)
};

/** Timer Control mode structure **/
struct TIMER_CONTROL_MODE 	//: public TIMER_DRIVER_MODE
{
	uint8_t		tim_indx;		//!< timer index (1..X)
	uint8_t		ch_indx;		//!< channel index (0=control, 1-4=channel)
	uint16_t	dier;			//!< dma/interrupt enable register (TIM_DIER)
	uint32_t	arr_or_ccr;		//!< auto reload or capture/compare register (TIM_ARR or TIM_CCRx)

	uint16_t cr1;				//!< control register 1 (TIM_CR1)
	uint16_t cr2;				//!< control register 2 (TIM_CR2)
	uint16_t smcr;				//!< slave mode register (TIM_SMCR)
	uint16_t psc;				//!< prescaler (TIM_PSC)
	uint8_t  rcr;				//!< repetition counter	(TIM_RCR)
};


/** SDIO Driver data structure **/
struct TIMER_DRIVER_DATA
{
	unsigned int cnt;			//!< number of open handles
	HANDLE pending[5];			//!< 1-4 channels, 0 = control
};

/** SDIO Driver Info structure **/
struct TIMER_DRIVER_INFO
{
	DRIVER_INFO_Type   info;			//!< Standard driver info
	TIM_TypeDef* 	   hw_tmr_base;		//!< Timer Hardware registers for control
	TIM_TypeDef* 	   hw_ch_base;		//!< Timer Hardware registers for channels
	TIMER_DRIVER_DATA* drv_data_tbl;	//!< driver data for all timers
	unsigned char	   drv_tmr_idx;		//!< driver data idx for control
	unsigned char	   drv_ch_idx;		//!< driver data idx for channels
	unsigned short	   isr_tmr_mask;	//!< which TIM_SR flags to process
};

typedef const struct TIMER_DRIVER_INFO * TIM_DRV_INF;

void TIM_DCR(TIM_DRV_INF drv_info, unsigned int reason, HANDLE hnd);
void TIM_DSR(TIM_DRV_INF drv_info, HANDLE hnd);
void TIM_ISR(TIM_DRV_INF drv_info);


typedef RES_CODE(*TIMER_DRV_CBF)(HANDLE hnd, RES_CODE reason);


#endif /* TIM_DRV_H_ */
