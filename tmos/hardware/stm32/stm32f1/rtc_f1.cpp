/**************************************************************************//**
 * @ingroup	 hardware_stm32f1_rtc
 * @{
 * @file     hardware/stm32/stm32f1/rtc_f1.cpp
 * @brief    STM32F1 RTC
 * @version  V3.00
 * @date     22. February 2013
 * @author	 Miroslav Kostadinov
 * @}
 */

#include <tmos.h>
#include <fam_cpp.h>

bool rtc_enter_init_mode(RTC_TypeDef* rtc)
{
	/* Set the CNF flag to enter in the Configuration Mode */
	rtc->RTC_CRL |= RTC_CRL_CNF;
	return true;
}

