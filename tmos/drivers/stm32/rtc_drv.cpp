/*
 * rtc_drv.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: miro
 */


#include <tmos.h>
#include <cmsis_cpp.h>
#include <rtc_drv.h>

/**
 * RTC Config
 *  After a reset, the backup domain (RTC registers, RTC backup data registers
 *  and backup SRAM) is protected against any possible unwanted write access.
 *
 *		To enable access to the backup domain, proceed as follows:
 *	# Access to the RTC and RTC backup registers
 *		1. Enable the power interface clock by setting the PWREN bits in the RCC
 *		   APB1 peripheral clock enable register (RCC_APB1ENR)
 *		2. Set the DBP bit in the PWR power control register (PWR_CR) to enable
 *		   access to the backup domain
 *		3. Select the RTC clock source: see Section 5.2.8: RTC/AWU clock
 *		4. Enable the RTC clock by programming the RTCEN [15] bit in the RCC
 *		   Backup domain control register (RCC_BDCR)
 *  # Access to the backup SRAM
 *  	1. Enable the power interface clock by setting the PWREN bits in the RCC
 *  	   APB1 peripheral clock enable register (RCC_APB1ENR)
 *  	2. Set the DBP bit in the PWR power control register (PWR_CR) to enable
 *  	   access to the backup domain
 *  	3. Enable the backup SRAM clock by setting BKPSRAMEN bit in the RCC AHB1
 *  	   peripheral clock register (RCC_AHB1ENR)
 *
 *
 * @param drv_info
 */
static void RTC_CONFIG(RTC_DRIVER_INFO* drv_info)
{
	uint32_t temp;

	RTC_TypeDef* rtc = drv_info->hw_base;

    // 1. Enable the PWR clock
	RCCPeripheralEnable(drv_info->info.peripheral_indx);

    // 2. Allow access to RTC
	PWR_BB->PWR_CR_DBP_BB = 1;

	if( !(rtc->RTC_ISR & RTC_ISR_INITS) )
	{
		// RTC is not running...

		temp = drv_info->rtc_cfg & RTC_CFG_RTCSEL;
		switch (temp)
		{
		case RCC_BDCR_RTCSEL_LSI:
			// start LSI
			RCC->RCC_CSR |= RCC_CSR_LSION;
			for(int retry=0; retry < 1000; retry++)
				if (RCC->RCC_CSR & RCC_CSR_LSIRDY)
					break;
			break;

		case RCC_BDCR_RTCSEL_LSE:
			// start LSE
			RCC->RCC_BDCR |= RCC_BDCR_LSEON;
			for(int retry=0; retry < 1000; retry++)
				if (RCC->RCC_BDCR & RCC_BDCR_LSERDY)
					break;
			break;

		case RCC_BDCR_RTCSEL_HSE: //externa high frequency quartz
			RCC->RCC_CFGR = (RCC->RCC_CFGR & ~RCC_CFGR_RTCPRE)
							| (drv_info->rtc_cfg & RTC_CFG_HSEPRE);

			break;

		default:
			//no clock
			break;
		}

		//change clock selection
		temp |= RCC->RCC_BDCR & ~RCC_BDCR_RTCSEL;
		RCC->RCC_BDCR = temp;

		// enable rtc
		RCC->RCC_BDCR = temp | RCC_BDCR_RTCEN;

	    // Enter Initialization mode
		rtc_enter_init_mode(rtc);

	    // Clear RTC CR FMT Bit
	    rtc->RTC_CR &= ~RTC_CR_FMT;

	    /* Configure the RTC PRER */
	    temp = rtc->RTC_PRER;
	    temp &= ~RTC_PRER_PREDIV_S;
	    temp |= drv_info->rtc_prer & RTC_PRER_PREDIV_S;
	    rtc->RTC_PRER = temp;
	    temp &= ~RTC_PRER_PREDIV_A;
	    temp |= drv_info->rtc_prer & RTC_PRER_PREDIV_A;
	    rtc->RTC_PRER = temp;

	    // Exit Initialization mode
		rtc_exit_init_mode(rtc);

	}
    /* Wait for RTC APB registers synchronisation */
	rtc_wait_sync(rtc);

	rtc_get_time(rtc, &drv_info->drv_data->rtc_time);

	// Enable periodic auto wake-up
	exti_enable_irq(EXTI, EXTI_Line22, PD_INT_RE);

	NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);
	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);

	rtc_set_wut(rtc, 0); // 1s
}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void RTC_DCR(RTC_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{

	switch(reason)
    {
        case DCR_RESET:
        	RTC_CONFIG(drv_info);
        	break;

        case DCR_OPEN:
			hnd->res = RES_OK;	// allow open
        	break;

   }
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void RTC_DSR(RTC_DRIVER_INFO* drv_info, HANDLE hnd)
{
	RES_CODE res;

	res = RES_SIG_ERROR;
	if(hnd->len >= 8)
	{
		// write from time_t source...
		if(hnd->cmd & FLAG_WRITE)
	    {
			time_t* time = (time_t*)hnd->src.as_voidptr;

			if(time->is_valid())
			{
				// Note: set time can be very slow...
				if(rtc_set_time(drv_info->hw_base, time))
				{
					res = RES_SIG_OK;
				}
			}
	    }

		// read to time_t ...
		if(hnd->cmd & FLAG_READ)
		{
			rtc_get_time(drv_info->hw_base, (time_t*)hnd->dst.as_voidptr);
			res = RES_SIG_OK;
		}
	}

	svc_HND_SET_STATUS(hnd, res);
}


//*----------------------------------------------------------------------------
//*			ISR functions
//*----------------------------------------------------------------------------
/**
 * RTC Alarm interrupts on EXTI line 17
 * @param drv_info
 */
void RTC_ALARM_ISR(RTC_DRIVER_INFO* drv_info)
{
	uint32_t status;

	// Check External interrupt line
	status = EXTI->EXTI_PR & EXTI_Line17;
	if(status)
	{
		RTC_TypeDef* rtc = drv_info->hw_base;

		// Clear exti pending
		EXTI->EXTI_PR = status;

		status = rtc->RTC_ISR;

		//check Alarm B flag
		if(status & RTC_ISR_ALRBF)
		{
			// clear Alarm B flag only
			rtc->RTC_ISR = status ^ RTC_ISR_ALRBF;
		}

		//check AlarmA
		if(status & RTC_ISR_ALRAF)
		{
			// clear Alarm A flag only
			rtc->RTC_ISR = status ^ RTC_ISR_ALRAF;
		}
	}
}

/**
 * RTC Tamper and TimeStamp interrupt on EXTI line 21
 * @param drv_info
 */
void RTC_TAMP_ISR(RTC_DRIVER_INFO* drv_info)
{
	uint32_t status;

	// Check External interrupt line
	status = EXTI->EXTI_PR & EXTI_Line21;
	if(status)
	{
		RTC_TypeDef* rtc = drv_info->hw_base;

		// Clear exti pending
		EXTI->EXTI_PR = status;

		status = rtc->RTC_ISR;

		//check Tamper flag
		if(status & RTC_ISR_TAMP1F)
		{
			// clear tamper flag only
			rtc->RTC_ISR = status ^ RTC_ISR_TAMP1F;
		}

		//check TimeStamp
		if(status & RTC_ISR_TSF)
		{
			// clear TimeStamp flag only
			rtc->RTC_ISR = status ^ RTC_ISR_TSF;

			//check TimeStamp overflow
			if(status & RTC_ISR_TSOVF)
			{
				// clear TimeStamp overflow flag only
				rtc->RTC_ISR = status ^ RTC_ISR_TSOVF;
			}
		}
	}
}

/**
 * RTC Wakepp interrupt on EXTI line 22
 * @param drv_info
 */
void RTC_WKUP_ISR(RTC_DRIVER_INFO* drv_info)
{
	uint32_t status;

	// Check External interrupt line
	status = EXTI->EXTI_PR & EXTI_Line22;
	if(status)
	{
		RTC_TypeDef* rtc = drv_info->hw_base;

		// Clear exti pending
		EXTI->EXTI_PR = status;

		//check wakeup flag
		status = rtc->RTC_ISR;
		if(status & RTC_ISR_WUTF)
		{
			// clear wakeup flag only
			rtc->RTC_ISR = status ^ RTC_ISR_WUTF;
			rtc_get_time(rtc, &drv_info->drv_data->rtc_time);
		}
	}
}
