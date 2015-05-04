/*
 * board_setup.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: miro
 */
#include <tmos.h>
#include <drivers.h>
#include <hardware_cpp.h>

/*!< Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source)

   IMPORTANT NOTE:
   ==============
   1. After each device reset the HSI is used as System clock source.

   2. Please make sure that the selected System clock doesn't exceed your device's
      maximum frequency.

   3. If none of the define below is enabled, the HSI is used as System clock
    source.

   4. The System clock configuration functions provided within this file assume that:
        - For Low, Medium and High density Value line devices an external 8MHz
          crystal is used to drive the System clock.
        - For Low, Medium and High density devices an external 8MHz crystal is
          used to drive the System clock.
        - For Connectivity line devices an external 25MHz crystal is used to drive
          the System clock.
     If you are using different crystal you have to adapt those functions accordingly.
    */


/**
 * This WEAK function is called immediately after reset
 */
extern "C" void LowLevelInit( void )
{

	//-- Enable HSI
	RCC->RCC_CR |=  RCC_CR_HSION;

	while(!(RCC->RCC_CR & RCC_CR_HSIRDY))
	{
	}

	//--disable the PLL
	RCC->RCC_CFGR &= ~RCC_CFGR_SW;		//select HSI as system clock
	while((RCC->RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
	{
	}
	RCC->RCC_CR &= ~(RCC_CR_HSEON | RCC_CR_PLLON);
	while(RCC->RCC_CR & RCC_CR_PLLRDY)
	{
	}

	//-- Configure the pll
	RCC->RCC_CFGR2 = 0;
	RCC->RCC_CFGR &= ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC); //PLLSRS = HSI/2
	RCC->RCC_CFGR |= RCC_CFGR_PLLMUL_Set(12-2);

	//--enable the PLL
	RCC->RCC_CR |= RCC_CR_PLLON;
	while(!(RCC->RCC_CR & RCC_CR_PLLRDY))
	{
	}

	//-- set flash wait states
	FLASH->FLASH_ACR = (FLASH->FLASH_ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_1WS;

	//-- HCLK = system clock, PCLK = system clock
	RCC->RCC_CFGR = (RCC->RCC_CFGR  & ~(RCC_CFGR_HPRE|RCC_CFGR_PPRE)) | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE_DIV1;

	//-- switch to pll
	RCC->RCC_CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
	{
	}


	system_clock_frequency = 48000000;

	if(exception_record.record_crc != exception_crc((const unsigned int*)&exception_record))
	{
		memclr((void *)&exception_record,sizeof(exception_record));
	}

	exception_record.reset_cause = LowLevelResetCause();
	if(exception_record.reset_cause & RCC_CSR_PORRSTF)
	{
		memclr((void *)&exception_record,sizeof(exception_record));
	}
	else
	{
		if(!exception_record.restart_cause && exception_record.reset_cause)
			exception_record.restart_cause = 0xff;
	}
}



extern "C" void app_init(void)
{
	TRACELN1("App init");
}



