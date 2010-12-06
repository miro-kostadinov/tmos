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
// 		 PMC DRIVER
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
	CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCRCF_4MHZ | CKGR_MOR_KEY(0x37) |
		CKGR_MOR_CFDEN,				//CFG_CKGR_MOR
	//PCK = 12000000 * (15+1) / 3 = 64 MHz
	//PCK = 12000000 * (7+1) / 2  = 48 MHz
	//PCK = 12000000 * (7+1) / 1  = 96 MHz
	CKGR_PLLAR_STUCKTO1 | CKGR_PLLAR_MULA(7) | CKGR_PLLAR_PLLACOUNT(0x3f)
		| CKGR_PLLAR_DIVA(1),
	{
		32768,		//freq for slow clk
		MHz(4),		// freq for main clk
		MHz(96),	//freq for PLLA
		MHz(96)		//freq for PLLB
	},
	PMC_MCKR_CSS_MAIN_CLK | PMC_MCKR_PRES_CLK
};

signed char const DRV_RESET_FIRST_TABLE[1] =
{
	SysTick_IRQn
};

//=================== DRV_TABLE ==========================================
// All drivers in the system must be listed in this table
// To open a driver, only the driver index is required (defined in drivers.h)
// The driver index must match with the driver position in this table
char * const DRV_TABLE[INALID_DRV_INDX+1] __attribute__ ((section (".ExceptionVectors")))  =
{
	1+ (char * const)&supc_driver, 	 // 0
	1+ (char * const)&rstc_driver,	 // 1
	1+ (char * const)&rtc_driver,	 // 2
	1+ (char * const)&rtt_driver,	 // 3
	1+ (char * const)&wdt_driver, 	 // 4
	1+ (char * const)&pmc_driver,	 // 5
	1+ (char * const)&DefaultDriver, // 6
	1+ (char * const)&DefaultDriver, // 7
	1+ (char * const)&DefaultDriver, // 8
	1+ (char * const)&DefaultDriver, // 9
	1+ (char * const)&DefaultDriver, // 10
	1+ (char * const)&DefaultDriver, // 11
	1+ (char * const)&DefaultDriver, // 12
	1+ (char * const)&DefaultDriver, // 13
	1+ (char * const)&DefaultDriver, // 14
	1+ (char * const)&DefaultDriver, // 15
	1+ (char * const)&DefaultDriver, // 16
	1+ (char * const)&DefaultDriver, // 17
	1+ (char * const)&DefaultDriver, // 18
	1+ (char * const)&DefaultDriver, // 19
	1+ (char * const)&DefaultDriver, // 20
	1+ (char * const)&DefaultDriver, // 21
	1+ (char * const)&DefaultDriver, // 22
	1+ (char * const)&DefaultDriver, // 23
	1+ (char * const)&DefaultDriver, // 24
	1+ (char * const)&DefaultDriver, // 25
	1+ (char * const)&DefaultDriver, // 26
	1+ (char * const)&DefaultDriver, // 27
	1+ (char * const)&DefaultDriver, // 28
	1+ (char * const)&DefaultDriver, // 29
	1+ (char * const)&DefaultDriver, // 30
	1+ (char * const)&DefaultDriver, // 31
   NULL				//null terminated list
};





