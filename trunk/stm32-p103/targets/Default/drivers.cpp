//////////////////////////////////////////////////////////////////////////
//
//			Drivers
//
//
//////////////////////////////////////////////////////////////////////////
#include <tmos.h>
#include <drivers.h>
#include <systick_drv.h>

const char restart_on_exception =1;



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
		ID_NO_PERIPH
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
			ID_NO_PERIPH
		},
		1									//!< OS_QUANTUM_PERIOD = 1 ms
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART 0 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 USB DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


extern signed char const DRV_RESET_FIRST_TABLE[] =
{
	SysTick_IRQn,
	INALID_DRV_INDX
};

//=================== DRV_TABLE ==========================================
// All drivers in the system must be listed in this table
// To open a driver, only the driver index is required (defined in drivers.h)
// The driver index must match with the driver position in this table
extern "C" char * const DRV_TABLE[INALID_DRV_INDX+1] __attribute__ ((section (".DriverTable")))  =
{
	1+ (char * const)&DefaultDriver, 	/*!<  0 Window WatchDog Interrupt       */
	1+ (char * const)&DefaultDriver, 	/*!<  1 PVD through EXTI Line detection Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!<  2 Tamper Interrupt                */
	1+ (char * const)&DefaultDriver, 	/*!<  3 RTC global Interrupt            */
	1+ (char * const)&DefaultDriver, 	/*!<  4 FLASH global Interrupt          */
	1+ (char * const)&DefaultDriver, 	/*!<  5 RCC global Interrupt            */
	1+ (char * const)&DefaultDriver, 	/*!<  6 EXTI Line0 Interrupt            */
	1+ (char * const)&DefaultDriver, 	/*!<  7 EXTI Line1 Interrupt            */
	1+ (char * const)&DefaultDriver, 	/*!<  8 EXTI Line2 Interrupt            */
	1+ (char * const)&DefaultDriver, 	/*!<  9 EXTI Line3 Interrupt            */
	1+ (char * const)&DefaultDriver, 	/*!< 10 EXTI Line4 Interrupt            */
	1+ (char * const)&DefaultDriver, 	/*!< 11 DMA1 Channel 1 global Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!< 12 DMA1 Channel 2 global Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!< 13 DMA1 Channel 3 global Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!< 14 DMA1 Channel 4 global Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!< 15 DMA1 Channel 5 global Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!< 16 DMA1 Channel 6 global Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!< 17 DMA1 Channel 7 global Interrupt */

	1+ (char * const)&DefaultDriver, 	/*!< 18 ADC1 and ADC2 global Interrupt       */
	1+ (char * const)&DefaultDriver, 	/*!< 19 USB Device High Priority or CAN1 TX Interrupts */
    1+ (char * const)&DefaultDriver, 	/*!< 20 SB Device Low Priority or CAN1 RX0 Interrupts */
    1+ (char * const)&DefaultDriver, 	/*!< 21 AN1 RX1 Interrupt                   */
    1+ (char * const)&DefaultDriver, 	/*!< 22 AN1 SCE Interrupt                   */
    1+ (char * const)&DefaultDriver, 	/*!< 23 xternal Line[9:5] Interrupts        */
    1+ (char * const)&DefaultDriver, 	/*!< 24 IM1 Break Interrupt                 */
    1+ (char * const)&DefaultDriver, 	/*!< 25 IM1 Update Interrupt                */
    1+ (char * const)&DefaultDriver, 	/*!< 26 IM1 Trigger and Commutation Interrupt */
    1+ (char * const)&DefaultDriver, 	/*!< 27 IM1 Capture Compare Interrupt       */
    1+ (char * const)&DefaultDriver, 	/*!< 28 IM2 global Interrupt                */
    1+ (char * const)&DefaultDriver, 	/*!< 29 IM3 global Interrupt                */
    1+ (char * const)&DefaultDriver, 	/*!< 30 IM4 global Interrupt                */
    1+ (char * const)&DefaultDriver, 	/*!< 31 2C1 Event Interrupt                 */
    1+ (char * const)&DefaultDriver, 	/*!< 32 2C1 Error Interrupt                 */
    1+ (char * const)&DefaultDriver, 	/*!< 33 2C2 Event Interrupt                 */
    1+ (char * const)&DefaultDriver, 	/*!< 34 2C2 Error Interrupt                 */
    1+ (char * const)&DefaultDriver, 	/*!< 35 PI1 global Interrupt                */
    1+ (char * const)&DefaultDriver, 	/*!< 36 PI2 global Interrupt                */
    1+ (char * const)&DefaultDriver, 	/*!< 37 SART1 global Interrupt              */
    1+ (char * const)&DefaultDriver, 	/*!< 38 SART2 global Interrupt              */
    1+ (char * const)&DefaultDriver, 	/*!< 39 SART3 global Interrupt              */
    1+ (char * const)&DefaultDriver, 	/*!< 40 xternal Line[15:10] Interrupts      */
    1+ (char * const)&DefaultDriver, 	/*!< 41 TC Alarm through EXTI Line Interrupt */
    1+ (char * const)&DefaultDriver, 	/*!< 42 SB Device WakeUp from suspend through EXTI Line Interrupt */
   NULL				//null terminated list
};





