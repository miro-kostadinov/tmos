//////////////////////////////////////////////////////////////////////////
//
//			Drivers
//
//
//////////////////////////////////////////////////////////////////////////

#include <tmos.h>
#include <drivers.h>
#include <systick_drv.h>
#include <supc_drv.h>
#include <rstc_drv.h>
#include <rtc_drv.h>
#include <rtt_drv.h>
#include <wdt_drv.h>
#include <pmc_drv.h>
#include <usart_drv.h>
#include <adc_drv.h>
#include <dacc_drv.h>
#include <usbd_drv.h>
#include <key2_drv.h>

const char restart_on_exception =0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		 DEFAULT DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DEFAULT_DRIVER_DCR(DRIVER_INFO drv_info, unsigned int reason, void * param)
{
}

void DEFAULT_DRIVER_DSR(DRIVER_INFO drv_info, HANDLE hnd)
{
}

void DEFAULT_DRIVER_ISR(DRIVER_INFO drv_info)
{
	while(1);
}

const DRIVER_INFO_Type DefaultDriver =
{
		DRIVER_INFO_STUB,
		DEFAULT_DRIVER_ISR,
		DEFAULT_DRIVER_DCR,
		DEFAULT_DRIVER_DSR,
		INALID_DRV_INDX,
		DRV_PRIORITY_KERNEL,
		ID_NO_PERIPHERAL
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SYSTICK DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern "C" const SYSTICK_DRIVER_INFO systick_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_ClockDriver,
			(DRV_DCR)SYSTICK_DCR,
			(DRV_DSR)DEFAULT_DRIVER_DSR,	//!< cannot be used!
			SysTick_IRQn,
			DRV_PRIORITY_KERNEL,
			ID_NO_PERIPHERAL
		},
		1									//!< OS_QUANTUM_PERIOD = 1 ms
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SYSTICK DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const SUPC_DRIVER_INFO supc_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)SUPC_ISR,
			(DRV_DCR)SUPC_DCR,
			(DRV_DSR)SUPC_DSR,
			SUPC_IRQn,
			DRV_PRIORITY_KERNEL,
			ID_SUPC
		},
		SUPC
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 RSTC DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const RSTC_DRIVER_INFO rstc_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)RSTC_ISR,
			(DRV_DCR)RSTC_DCR,
			(DRV_DSR)RSTC_DSR,
			RSTC_IRQn,
			DRV_PRIORITY_KERNEL,
			ID_RSTC
		},
		RSTC
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 RTC DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const RTC_DRIVER_INFO rtc_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)RTC_ISR,
		(DRV_DCR)RTC_DCR,
		(DRV_DSR)RTC_DSR,
		RTC_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_RTC
	},
	RTC
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 RTT DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const RTT_DRIVER_INFO rtt_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)RTT_ISR,
		(DRV_DCR)RTT_DCR,
		(DRV_DSR)RTT_DSR,
		RTT_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_RTT
	},
	RTT
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 WDT DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const WDT_DRIVER_INFO wdt_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)WDT_ISR,
		(DRV_DCR)WDT_DCR,
		(DRV_DSR)WDT_DSR,
		WDT_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_WDT
	},
	WDT
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 (5) PMC DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define MHz(x) (x * 1000000)
extern "C" const PMC_DRIVER_INFO pmc_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)PMC_ISR,
		(DRV_DCR)PMC_DCR,
		(DRV_DSR)PMC_DSR,
		PMC_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_PMC
	},
	PMC,
	CKGR_MOR_CFDEN | CKGR_MOR_MOSCSEL | CKGR_MOR_KEY(0x37) |
	CKGR_MOR_MOSCXTST(255) |
	CKGR_MOR_MOSCXTEN,				//CFG_CKGR_MOR

//	CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCRCF_4MHZ | CKGR_MOR_KEY(0x37) |
//		CKGR_MOR_CFDEN,				//CFG_CKGR_MOR


	//PCK = 12000000 * (7+1) / 2  = 48 MHz
	//PCK = 12000000 * (7+1) / 1  = 96 MHz
//	CKGR_PLLAR_STUCKTO1 | CKGR_PLLAR_MULA(7) | CKGR_PLLAR_PLLACOUNT(0x3f)
//		| CKGR_PLLAR_DIVA(1),


	//PCK = 12000000 * (15+1) / 3 = 64 MHz
	CKGR_PLLAR_STUCKTO1 | CKGR_PLLAR_MULA(15) | CKGR_PLLAR_PLLACOUNT(0x3f)
		| CKGR_PLLAR_DIVA(3),



	{
		32768,		//freq for slow clk
		MHz(12),	// freq for main clk
		MHz(64),	//freq for PLLA
		MHz(96)		//freq for PLLB
	},


	PMC_MCKR_CSS_MAIN_CLK | PMC_MCKR_PRES_CLK
//	PMC_MCKR_CSS_PLLA_CLK | PMC_MCKR_PRES_CLK
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 (11) PIOA DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
PIO_DRIVER_DATA_STRU pioA_driver_data;

const PIO_DRIVER_INFO pioA_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)PIO_ISR,
		(DRV_DCR)PIO_DCR,
		(DRV_DSR)PIO_DSR,
		PIOA_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_PIOA
	},
	PIOA,
	&pioA_driver_data
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 (12) PIOB DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
PIO_DRIVER_DATA_STRU pioB_driver_data;

const PIO_DRIVER_INFO pioB_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)PIO_ISR,
		(DRV_DCR)PIO_DCR,
		(DRV_DSR)PIO_DSR,
		PIOB_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_PIOB
	},
	PIOB,
	&pioB_driver_data
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 (15) USART1 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define CFG_USART1_BUF_SIZE 16
int usart1_data[(sizeof(UART_DRIVER_DATA_STRU)+CFG_USART1_BUF_SIZE +3)/4];

const USART_DRIVER_INFO usart1_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)USART_ISR,
		(DRV_DCR)USART_DCR,
		(DRV_DSR)USART_DSR,
		USART1_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_USART1
	},
	USART1,
	(UART_DRIVER_DATA)usart1_data,
	{ //GPIO_STRU
		PIO_PA22A_TXD1 | PIO_PA21A_RXD1,
		0,
		PORT_A
	},
	CFG_USART1_BUF_SIZE
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		(29) ADC DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ADC_DRIVER_DATA_STRU adc_driver_data;

const ADC_DRIVER_INFO adc_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)ADC_ISR,
		(DRV_DCR)ADC_DCR,
		(DRV_DSR)ADC_DSR,
		ADC_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_ADC
	},
	ADC,
	&adc_driver_data,
	ADC_MR_TRANSFER(1) | ADC_MR_TRACKTIM(7) | ADC_MR_SETTLING(3) | ADC_MR_STARTUP_SUT96//CFG_ADC_MR
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		(30) DACC DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DACC_DRIVER_DATA_STRU dacc_driver_data;

const DACC_DRIVER_INFO dacc_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)DACC_ISR,
		(DRV_DCR)DACC_DCR,
		(DRV_DSR)DACC_DSR,
		DACC_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_DACC
	},
	DACC,
	&dacc_driver_data
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		(34) USB DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
USBD_DRIVER_DATA_STRU usb_driver_data;

const USBD_DRIVER_INFO usb_driver =
{
	{
		DRIVER_INFO_STUB,
		(DRV_ISR)USBD_ISR,
		(DRV_DCR)USBD_DCR,
		(DRV_DSR)USBD_DSR,
		UDP_IRQn,
		DRV_PRIORITY_KERNEL,
		ID_UDP
	},
	UDP,
	&usb_driver_data
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		(35) KEY2 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

KEY2_DRIVER_DATA key2_driver_data;

const KEY2_DRIVER_INFO key2_driver =
{
	{
		DRIVER_INFO_STUB,
		DEFAULT_DRIVER_ISR,
		(DRV_DCR)KEY2_DCR,
		(DRV_DSR)KEY2_DSR,
		KEY_DRV_INDX,
		DRV_PRIORITY_KERNEL,
		ID_NO_PERIPHERAL
	},
	&key2_driver_data,
	{ //GPIO_STRU
		PIO_PA26 | PIO_PA25 | PIO_PA11,
		PIOMODE_IER | PIOMODE_IFER,
		PORT_A
	},
};


signed char const DRV_RESET_FIRST_TABLE[] =
{
	SysTick_IRQn, INALID_DRV_INDX
};

//=================== DRV_TABLE ==========================================
// All drivers in the system must be listed in this table
// To open a driver, only the driver index is required (defined in drivers.h)
// The driver index must match with the driver position in this table
char * const DRV_TABLE[INALID_DRV_INDX+1] __attribute__ ((section (".ExceptionVectors")))  =
{
	1+ (char * const)&supc_driver, 	 // 0 Supply Controller
	1+ (char * const)&rstc_driver,	 // 1 Reset Controller
	1+ (char * const)&rtc_driver,	 // 2 Real Time Clock
	1+ (char * const)&rtt_driver,	 // 3 Real Time Timer
	1+ (char * const)&wdt_driver, 	 // 4 Watchdog Timer
	1+ (char * const)&pmc_driver,	 // 5 PMC
	1+ (char * const)&DefaultDriver, // 6 EEFC
	1+ (char * const)&DefaultDriver, // 7 Reserved
	1+ (char * const)&DefaultDriver, // 8 UART0
	1+ (char * const)&DefaultDriver, // 9 UART1
	1+ (char * const)&DefaultDriver, // 10 SMC
	1+ (char * const)&pioA_driver,	 // 11 Parallel IO Controller A
	1+ (char * const)&pioB_driver,	 // 12 Parallel IO Controller B
	1+ (char * const)&DefaultDriver, // 13 Parallel IO Controller C
	1+ (char * const)&DefaultDriver, // 14 USART 0
	1+ (char * const)&usart1_driver, // 15 USART 1
	1+ (char * const)&DefaultDriver, // 16 Reserved
	1+ (char * const)&DefaultDriver, // 17 Reserved
	1+ (char * const)&DefaultDriver, // 18 MCI
	1+ (char * const)&DefaultDriver, // 19 TWI 0
	1+ (char * const)&DefaultDriver, // 20 TWI 1
	1+ (char * const)&DefaultDriver, // 21 SPI
	1+ (char * const)&DefaultDriver, // 22 SSC
	1+ (char * const)&DefaultDriver, // 23 Timer Counter 0
	1+ (char * const)&DefaultDriver, // 24 Timer Counter 1
	1+ (char * const)&DefaultDriver, // 25 Timer Counter 2
	1+ (char * const)&DefaultDriver, // 26 Timer Counter 3
	1+ (char * const)&DefaultDriver, // 27 Timer Counter 4
	1+ (char * const)&DefaultDriver, // 28 Timer Counter 5
	1+ (char * const)&adc_driver,	 // 29 ADC controller
	1+ (char * const)&dacc_driver,	 // 30 DAC controller
	1+ (char * const)&DefaultDriver, // 31 PWM
	1+ (char * const)&DefaultDriver, // 32 CRC Calculation Unit
	1+ (char * const)&DefaultDriver, // 33 Analog Comparator
	1+ (char * const)&DefaultDriver, // 34 USB Device Port
	1+ (char * const)&key2_driver,	 // 35 Key2 driver
   NULL				//null terminated list
};





