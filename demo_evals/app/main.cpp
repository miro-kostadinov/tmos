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

/**
 * example how to drive pins directly...
 *
 * Note:
 * 	There are target specific things like the PIN_DESC type and it's value.
 * 	So these things are defined either in the OS or in the target directory of
 * 	the project.
 * 	The code here is supposed to be universal. We just want to flash a led,
 * 	we don't care if the actual board is using Atmel, ST or Luminary CPUs.
 * 	We don't care if the pin is PA0 or PB1. We don't care if the pin needs to be
 * 	asserted low or high. These are target specific things.
 * 	All we need here is to have a pin that can drive a led. We assume the pin
 * 	definition is called PIN_LED and we ask assertion or deassertion of this pin.
 * 	If you want to change the pin, just go in drivers.h in the target's directory
 * 	and change the definition.
 *
 */
void led_thread(void)
{
	PIN_DESC led_pin = PIN_LED;

	PIO_Cfg(led_pin);
	while(1)
	{
		PIO_Assert(led_pin);
		tsk_sleep(100);
		PIO_Deassert(led_pin);
		tsk_sleep(500);
	}
}
TASK_DECLARE_STATIC(led_task, "LEDT", led_thread, 0, 20+TRACE_SIZE);

#define DEBOUNCE_TIME 40		//40 ms debounce

void button_thread(void)
{
	/* We always need a handle to work with drivers	 */
	CHandle hnd;
	/* As a mode structure we must provide a zero/null terminated array of pin
	 * descriptions (many pins can be read/write at once).*/
	PIN_DESC pin_definitions[] = {PIN_WAKE_UP, 0};
	pio_set read_values[1];	// here we will read the pin values.

	/* If we want to open a handle for gpio operations we can use GPIO_IRQn as
	 * the driver index.
	 * The tsk_open() will also configure the pins according to the descriptions */
	if(hnd.tsk_open(GPIO_IRQn, &pin_definitions))
	{
		while(1)
		{
			/* Reading from the handle will return immediately with the pin state(s).
			 * The same can be done with PIO_Read() without handles and drivers.
			 *
			 * Locked read will return when at least one pin from the set triggers
			 * an interrupt. In our case we have one pin, so tsk_read_locked() will
			 * return as soon as active edge is detected on this pin.
			 * Make sure the definition includes rasing/falling edge flag. Otherwise
			 * the function will block forever.
			 */
			if(hnd.tsk_read_locked(read_values, sizeof(read_values)) == RES_OK)
			{
				// The pin changed once...

				//to debounce keep reading while it is changing too fast
				int oscilations =0;
				while(hnd.tsk_read_locked(read_values, sizeof(read_values), DEBOUNCE_TIME) == RES_OK)
				{
					// we have a change within less the DEBOUNCE time - ignore it!
					oscilations++;
				}

				// tsk_read_locked() timed out... the pin has settled at last.
				TRACELN("PIN change: %04X (%u)", read_values[0], oscilations);
			}
		}
	}
}
TASK_DECLARE_STATIC(button_task, "LEDT", button_thread, 5, 50+TRACE_SIZE);

void uart_thread(void)
{
	CHandle uart_hnd;
	char buf[8];
	RES_CODE res;

	if(uart_hnd.tsk_open(UART_TEST_DRIVER, &uart_default_mode))
	{
		//transmit something..
		res = uart_hnd.tsk_write("hello\r\n");
		TRACELN("uart tsk send %x", res);

		//loopback
		while(1)
		{
			res = uart_hnd.tsk_read(buf, sizeof(buf));
			TRACELN("uart tsk rcv %x", res);
			if(res <= RES_OK)
			{
				unsigned int dwRead;

				dwRead = sizeof(buf) - uart_hnd.len;
				TRACE(" %u bytes", dwRead);
				if(dwRead)
				{
					res = uart_hnd.tsk_write(buf, dwRead);
					TRACELN("uart tsk send %x", res);
				}
			}
		}
	}
}
TASK_DECLARE_STATIC(uart_task, "UART", uart_thread, 5, 100+TRACE_SIZE);


volatile unsigned int cpu_usage;

static unsigned int get_clocks_per200ms(void)
{
	unsigned int res;

	asm volatile (
		"1: \n\t"
		"	movs			%0, #0				\n\t"
		"	mov				r1, r10				\n\t"	//r1 = REG_MTASK
		"	movs			r2, #200			\n\t"	//r2 = mS
		"	ldr				r3, [r1, %1]		\n\t"	//r3 = CURRENT_TIME
		"2:										\n\t"
		"	adds			%0, #6				\n\t"	// r0 += [6]		[1]
		"	ldr				r4, [r1, %1]		\n\t"	//					[2]
		"	cmp				r3, r4				\n\t"	//					[1]
		"	beq				2b 					\n\t"	// do while (r2==r4)[2]
		"	bhi				1b 					\n\t"

		"	adds			r3, #1				\n\t"
		"	subs			r2, #1				\n\t"
		"	bne				2b 					\n\t"

	  : "=r"(res)
	  : "I" (tsk_time)
	  : "r1", "r2", "r3", "r4", "memory"
	  );
	return res;
}

int main(void)
{
	unsigned int clock_freq;


	//start other tasks
    usr_task_init_static(&led_task_desc, true);
    usr_task_init_static(&button_task_desc, true);
    usr_task_init_static(&uart_task_desc, true);

    //clocks in 250mS
    clock_freq = system_clock_frequency >> 2;

    for(;;)
	{
		unsigned int clocks;
		clocks = (125 * get_clocks_per200ms()) / clock_freq;
		if(clocks >= 100)
			clocks =1;
		else
			clocks = 100 - clocks;

		if(cpu_usage > clocks)
			cpu_usage -= (cpu_usage - clocks) >> 3 ;
		else
			cpu_usage += (clocks -cpu_usage) >> 3 ;

	}

    //codan!
    return 0;
}
TASK_DECLARE_STATIC(main_task, "MAIN", (void (*)(void))main, 0, 20+TRACE_SIZE);
