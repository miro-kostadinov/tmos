/*
 * rtc_drv.h
 *
 * RTC driver for F2 family (could be ported for F4 etc).
 *
 *  Created on: Feb 14, 2013
 *      Author: miro
 */

#ifndef RTC2_DRV_H_
#define RTC2_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>
#include <tmos_time.h>

/** RTC Driver data structure **/
struct RTC_DRIVER_DATA
{
	time_t 	 rtc_time;
	uint16_t rtc_state;
};

#define RTC_CFG_RTCSEL 		0x00000300 //!< RTCSEL[1:0]: RTC clock source selection
#define RTC_CFG_HSEPRE 		0x001F0000 //!< HSE division factor for RTC clock

/** RTC Driver Info structure **/
struct RTC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	RTC_TypeDef*		hw_base;		//!< RTC peripheral
	RTC_DRIVER_DATA* 	drv_data;		//!< driver data
	uint32_t			rtc_cfg;		//!< rtc clksel + rtc HSEPRE
	uint32_t			rtc_prer;		//!< rtc PRER
};

void RTC_DCR(RTC_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void RTC_DSR(RTC_DRIVER_INFO* drv_info, HANDLE hnd);

void RTC_ALARM_ISR(RTC_DRIVER_INFO* drv_info);
void RTC_TAMP_ISR(RTC_DRIVER_INFO* drv_info);
void RTC_WKUP_ISR(RTC_DRIVER_INFO* drv_info);



#endif /* RTC2_DRV_H_ */
