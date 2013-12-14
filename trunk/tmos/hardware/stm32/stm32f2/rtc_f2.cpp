/*
 * rtc_f2.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: miro
 */


#include <tmos.h>
#include <fam_cpp.h>
#include <cmsis_cpp.h>

#define SYNCHRO_TIMEOUT     0x00008000
#define INITMODE_TIMEOUT	0x00010000

bool rtc_wait_sync(RTC_TypeDef* rtc)
{
	bool res=false;
	// Disable the write protection for RTC registers
	rtc->RTC_WPR = 0xCA;
	rtc->RTC_WPR = 0x53;

	// Clear RSF flag
	rtc->RTC_ISR &= ~(RTC_ISR_INIT | RTC_ISR_RSF);

	/* Wait the registers to be synchronised */
	for(int i=0; i < SYNCHRO_TIMEOUT; i++)
	{
		if(rtc->RTC_ISR & RTC_ISR_RSF)
		{
			res = true;
			break;
		}
	}

	// Enable the write protection for RTC registers
	rtc->RTC_WPR = 0xFF;

	return (res);

}

bool rtc_enter_init_mode(RTC_TypeDef* rtc)
{
	bool res;

	// Disable the write protection for RTC registers
	rtc->RTC_WPR = 0xCA;
	rtc->RTC_WPR = 0x53;

	// Enter Initialization mode is set
	res = rtc->RTC_ISR & RTC_ISR_INITF;
	if ( !res )
	{
		RTC->RTC_ISR = 0xFFFFFFFF; // INIT bit is rw, others are w_c0

		// Wait for INITF
		for(int retry=0; retry < INITMODE_TIMEOUT; retry++)
			if (RTC->RTC_ISR & RTC_ISR_INITF)
			{
				res = true;
				break;
			}
	}
	return (res);
}

void rtc_exit_init_mode(RTC_TypeDef* rtc)
{
    // Exit Initialization mode
    rtc->RTC_ISR &= ~RTC_ISR_INIT;

    // Enable write protection
    rtc->RTC_WPR = 0xFF;
}

#if USE_TIME
static uint32_t rtc_bcd2byte(uint32_t val)
{
	return (val & 0xF) + ((val >> 4)*10);
}

static uint32_t rtc_byte2bcd(uint32_t val)
{
	return ( ((val/10)<<4) + (val %10));
}

void rtc_get_time(RTC_TypeDef* rtc, time_t* time)
{
	uint32_t tr;

	// Get the RTC_TR register, reading RTC_TR locks the values in the
	// higher-order calendar shadow registers until RTC_DR is read.
	tr	= rtc->RTC_TR;

	time->hour = rtc_bcd2byte( (tr>>16) & 0x3F );
	if(tr & RTC_TR_PM)
		time->hour += 12;
	time->minute= rtc_bcd2byte( (tr>>8) & 0x7F );
	time->second = rtc_bcd2byte( tr & 0x7F );

	// Get the RTC_TR register
	tr	= rtc->RTC_DR;

	time->year = 2000 + rtc_bcd2byte( (tr>>16) & 0xFF );
	time->mon = rtc_bcd2byte( (tr>>8) & 0x1F );
	time->mday  =rtc_bcd2byte( tr & 0x3F );

}

bool rtc_set_time(RTC_TypeDef* rtc, const time_t* time)
{
	uint32_t tr, dr;

	tr =  (rtc_byte2bcd(time->hour) << 16) | (rtc_byte2bcd(time->minute) << 8)
			| (rtc_byte2bcd(time->second));

	dr = (rtc_byte2bcd(time->year % 100) << 16) |(rtc_byte2bcd(time->mon) << 8)
			| (rtc_byte2bcd(time->mday)) | (time->get_week_day() << 13);

    // Enter Initialization mode
	if(rtc_enter_init_mode(rtc))
	{
	    rtc->RTC_DR = dr;
	    rtc->RTC_TR = tr;

	    rtc_exit_init_mode(rtc);
	    return rtc_wait_sync(rtc);
	}
	return false;
}
#endif

bool rtc_set_wut(RTC_TypeDef* rtc, uint32_t period)
{
	uint32_t cr;
	bool res=false;


	// Disable the RTC registers Write protection
	rtc->RTC_WPR = 0xCA;
	rtc->RTC_WPR = 0x53;

	// Disable the wakeup timer.
	cr = rtc->RTC_CR;
	if(cr & RTC_CR_WUTE)
	{

		cr &= ~(RTC_CR_WUTE | RTC_CR_WUTIE);
		rtc->RTC_CR = cr;

		// Ensure access to Wakeup auto-reload counter and bits WUCKSEL[2:0] is allowed
		for(int i=0; i < INITMODE_TIMEOUT; i++)
		{
			if(rtc->RTC_ISR & RTC_ISR_WUTF)
			{
				res = true;
				break;
			}
		}

	}
	// Program the value into the wakeup timer
	rtc->RTC_WUTR = period & RTC_WUTR_WUT;

	// Select the desired clock source
	cr &= ~RTC_CR_WUCKSEL;
	if(period > RTC_WUTR_WUT)
		cr |= RTC_CR_WUCKSEL_CK_SPRE2;
	else
		cr |= RTC_CR_WUCKSEL_CK_SPRE;
	rtc->RTC_CR = cr;

	// clear wutf
	rtc->RTC_ISR &= ~RTC_ISR_WUTF;

	// Re-enable the wakeup timer
	rtc->RTC_CR = cr | RTC_CR_WUTE;

	// Enable wakeup interrupt
	rtc->RTC_CR |= RTC_CR_WUTIE;

	// Enable the RTC registers Write protection
    rtc->RTC_WPR = 0xFF;

    return res;
}
