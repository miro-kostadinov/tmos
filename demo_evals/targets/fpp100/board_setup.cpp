/*
 * board_setup.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <drivers.h>
#include <hardware_cpp.h>

/**
  *=============================================================================
  *        Supported STM32F2xx device revision    | Rev B and Y
  *-----------------------------------------------------------------------------
  *        System Clock source                    | PLL (HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 120000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 120000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 4
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 2
  *-----------------------------------------------------------------------------
  *        HSE Frequency(Hz)                      | 25000000
  *-----------------------------------------------------------------------------
  *        PLL_M                                  | 25
  *-----------------------------------------------------------------------------
  *        PLL_N                                  | 240
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 5
  *-----------------------------------------------------------------------------
  *        PLLI2S_N                               | NA
  *-----------------------------------------------------------------------------
  *        PLLI2S_R                               | NA
  *-----------------------------------------------------------------------------
  *        I2S input clock                        | NA
  *-----------------------------------------------------------------------------
  *        VDD(V)                                 | 3.3
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 3
  *-----------------------------------------------------------------------------
  *        Prefetch Buffer                        | ON
  *-----------------------------------------------------------------------------
  *        Instruction cache                      | ON
  *-----------------------------------------------------------------------------
  *        Data cache                             | ON
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Enabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================
  */
#define DEF_PLL_M   RCC_PLLCFGR_PLLM_Set(25)		//see above table
#define DEF_PLL_N   RCC_PLLCFGR_PLLN_Set(240)
#define DEF_PLL_P   RCC_PLLCFGR_PLLP_DIV2
#define DEF_PLL_Q 	RCC_PLLCFGR_PLLQ_Set(5)
/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors,
  *         AHB/APBx prescalers and Flash settings
  * @Note   This function should be called only once the RCC clock configuration
  *         is reset to the default reset state (done in SystemInit() function).
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
	/******************************************************************************/
	/*            PLL (clocked by HSE) used as System clock source                */
	/******************************************************************************/
	__IO uint32_t StartUpCounter = 0, HSEStatus;

	/* Enable HSE */
	RCC->RCC_CR |= RCC_CR_HSEON;

	/* Wait till HSE is ready and if Time out is reached exit */
	do
	{
		HSEStatus = RCC->RCC_CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if (RCC->RCC_CR & RCC_CR_HSERDY)
	{
		/* HCLK = SYSCLK / 1*/
		RCC->RCC_CFGR |= RCC_CFGR_HPRE_DIV1;

		/* PCLK2 = HCLK / 2*/
		RCC->RCC_CFGR |= RCC_CFGR_PPRE2_DIV2;

		/* PCLK1 = HCLK / 4*/
		RCC->RCC_CFGR |= RCC_CFGR_PPRE1_DIV4;

		/* Configure the main PLL */
		RCC->RCC_PLLCFGR = DEF_PLL_M | DEF_PLL_N | DEF_PLL_P | DEF_PLL_Q
				| RCC_PLLCFGR_PLLSRC_HSE ;

		/* Enable the main PLL */
		RCC->RCC_CR |= RCC_CR_PLLON;

		/* Wait till the main PLL is ready */
		while ((RCC->RCC_CR & RCC_CR_PLLRDY) == 0)
		{
		}

		/* Configure Flash prefetch, Instruction cache, Data cache and wait state */
		FLASH->FLASH_ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN
						| FLASH_ACR_LATENCY_3WS;

		/* Select the main PLL as system clock source */
		RCC->RCC_CFGR &= ~(RCC_CFGR_SW);
		RCC->RCC_CFGR |= RCC_CFGR_SW_PLL;

		/* Wait till the main PLL is used as system clock source */
		while ((RCC->RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
		{
		}
	}
	else
	{ /* If HSE fails to start-up, the application will have wrong clock
	 configuration. User can add here some code to deal with this error */
		TRACELN1("Failed to start HSE");
	}

}


/**
 * This WEAK function is called immediately after reset
 */
extern "C" void LowLevelInit( void )
{
	/* Reset the RCC clock configuration to the default reset state ------------*/
	RCC->RCC_CR |= RCC_CR_HSION;

	/* Reset CFGR register */
	RCC->RCC_CFGR = 0x00000000;

	/* Reset HSEON, CSSON and PLLON bits */
	RCC->RCC_CR &= ~(RCC_CR_PLLON | RCC_CR_HSEON | RCC_CR_CSSON);

	/* Reset PLLCFGR register */
	RCC->RCC_PLLCFGR =  0x24003010;

	/* Reset HSEBYP bit */
	RCC->RCC_CR &= (uint32_t)0xFFFBFFFF;

	/* Disable all interrupts */
	RCC->RCC_CIR = 0x00000000;


	/* Configure the System clock source, PLL Multiplier and Divider factors,
	   AHB/APBx prescalers and Flash settings ----------------------------------*/
	SetSysClock();



	DBGMCU->DBGMCU_CR = DBGMCU_CR_TRACE_IOEN;
//    if(!(CoreDebug->DHCSR&1))
//     return;

	//SYSCTL->SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC	| SYSCTL_SYSDIV_2);
	system_clock_frequency =120000000; //SYSCTL->SysCtlClockGet();

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




