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

#if defined (STM32F10X_LD_VL) || (defined STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
/* #define SYSCLK_FREQ_HSE    HSE_VALUE */
 #define SYSCLK_FREQ_24MHz  24000000
#else
/* #define SYSCLK_FREQ_HSE    HSE_VALUE */
/* #define SYSCLK_FREQ_24MHz  24000000 */
/* #define SYSCLK_FREQ_36MHz  36000000 */
/* #define SYSCLK_FREQ_48MHz  48000000 */
/* #define SYSCLK_FREQ_56MHz  56000000 */
#define SYSCLK_FREQ_72MHz  72000000
#endif

/**
  * @brief  Sets System clock frequency to 72MHz and configure HCLK, PCLK2
  *          and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
static void SetSysClockTo72(void)
{
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;

	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
	/* Enable HSE */
	RCC->RCC_CR |=  RCC_CR_HSEON;

	/* Wait till HSE is ready and if Time out is reached exit */
	do
	{
		HSEStatus = RCC->RCC_CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if ( !(RCC->RCC_CR & RCC_CR_HSERDY))
	{
		HSEStatus =  0x01;
	}
	else
	{
		HSEStatus =  0x00;
	}

	if (HSEStatus == 0x01)
	{
		/* Enable Prefetch Buffer */
		FLASH->FLASH_ACR |= FLASH_ACR_PRFTBE;

		/* Flash 2 wait state */
		FLASH->FLASH_ACR &= ~FLASH_ACR_LATENCY;
		FLASH->FLASH_ACR |= FLASH_ACR_LATENCY_2WS;

		/* HCLK = SYSCLK */
		RCC->RCC_CFGR |= RCC_CFGR_HPRE_DIV1;

		/* PCLK2 = HCLK */
		RCC->RCC_CFGR |= RCC_CFGR_PPRE2_DIV1;

		/* PCLK1 = HCLK */
		RCC->RCC_CFGR |= RCC_CFGR_PPRE1_DIV2;

#ifdef STM32F10X_CL
		/* Configure PLLs ------------------------------------------------------*/
		/* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
		/* PREDIV1 configuration: PREDIV1CLK = PLL2 / 5 = 8 MHz */

		RCC->RCC_CFGR2 &= ~(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
				RCC_CFGR2_PREDIV1 | RCC_CFGR2_PREDIV1SRC);
		RCC->RCC_CFGR2 |= (RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
				RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV5);

		/* Enable PLL2 */
		RCC->RCC_CR |= RCC_CR_PLL2ON;
		/* Wait till PLL2 is ready */
		while((RCC->RCC_CR & RCC_CR_PLL2RDY) == 0)
		{
		}

		/* PLL configuration: PLLCLK = PREDIV1 * 9 = 72 MHz */
		RCC->RCC_CFGR &= ~(RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
		RCC->RCC_CFGR |= (RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLSRC_PREDIV1 |
				RCC_CFGR_PLLMULL9);
#else
		/*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
		RCC->RCC_CFGR &=  ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
		RCC->RCC_CFGR |= (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
#endif /* STM32F10X_CL */

		/* Enable PLL */
		RCC->RCC_CR |= RCC_CR_PLLON;

		/* Wait till PLL is ready */
		while ((RCC->RCC_CR & RCC_CR_PLLRDY) == 0)
		{
		}

		/* Select PLL as system clock source */
		RCC->RCC_CFGR &= ~(RCC_CFGR_SW);
		RCC->RCC_CFGR |= RCC_CFGR_SW_PLL;

		/* Wait till PLL is used as system clock source */
		while ((RCC->RCC_CFGR & RCC_CFGR_SWS) != 0x08)
		{
		}
	}
	else
	{ /* If HSE fails to start-up, the application will have wrong clock
	 configuration. User can add here some code to deal with this error */
	}
}

/**
  * @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
#ifdef SYSCLK_FREQ_HSE
  SetSysClockToHSE();
#elif defined SYSCLK_FREQ_24MHz
  SetSysClockTo24();
#elif defined SYSCLK_FREQ_36MHz
  SetSysClockTo36();
#elif defined SYSCLK_FREQ_48MHz
  SetSysClockTo48();
#elif defined SYSCLK_FREQ_56MHz
  SetSysClockTo56();
#elif defined SYSCLK_FREQ_72MHz
  SetSysClockTo72();
#endif

 /* If none of the define above is enabled, the HSI is used as System clock
    source (default after reset) */
}


/**
 * This WEAK function is called immediately after reset
 */
extern "C" void LowLevelInit( void )
{
	  /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
	  /* Set HSION bit */
	  RCC->RCC_CR |= 0x00000001;

	  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
	#ifndef STM32F10X_CL
	  RCC->RCC_CFGR &= 0xF8FF0000;
	#else
	  RCC->RCC_CFGR &= 0xF0FF0000;
	#endif /* STM32F10X_CL */

	  /* Reset HSEON, CSSON and PLLON bits */
	  RCC->RCC_CR &= 0xFEF6FFFF;

	  /* Reset HSEBYP bit */
	  RCC->RCC_CR &= 0xFFFBFFFF;

	  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
	  RCC->RCC_CFGR &= 0xFF80FFFF;

	#ifdef STM32F10X_CL
	  /* Reset PLL2ON and PLL3ON bits */
	  RCC->RCC_CR &= 0xEBFFFFFF;

	  /* Disable all interrupts and clear pending bits  */
	  RCC->RCC_CIR = 0x00FF0000;

	  /* Reset CFGR2 register */
	  RCC->RCC_CFGR2 = 0x00000000;
	#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
	  /* Disable all interrupts and clear pending bits  */
	  RCC->RCC_CIR = 0x009F0000;

	  /* Reset CFGR2 register */
	  RCC->RCC_CFGR2 = 0x00000000;
	#else
	  /* Disable all interrupts and clear pending bits  */
	  RCC->RCC_CIR = 0x009F0000;
	#endif /* STM32F10X_CL */

	#if defined (STM32F10X_HD) || (defined STM32F10X_XL) || (defined STM32F10X_HD_VL)
	  #ifdef DATA_IN_ExtSRAM
	    SystemInit_ExtMemCtl();
	  #endif /* DATA_IN_ExtSRAM */
	#endif

	/* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
	/* Configure the Flash Latency cycles and enable prefetch buffer */
	SetSysClock();

	DBGMCU->DBGMCU_CR = DBGMCU_CR_TRACE_IOEN;
//    if(!(CoreDebug->DHCSR&1))
//     return;

	//SYSCTL->SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC	| SYSCTL_SYSDIV_2);
	system_clock_frequency = 72000000; //SYSCTL->SysCtlClockGet();

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



