/*
 * rtc_f1.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: miro
 */


#include <tmos.h>
#include <fam_cpp.h>

bool rtc_enter_init_mode(RTC_TypeDef* rtc)
{
	/* Set the CNF flag to enter in the Configuration Mode */
	rtc->RTC_CRL |= RTC_CRL_CNF;
	return true;
}

