//////////////////////////////////////////////////////////////////////////
//
//			Drivers
//
//
//////////////////////////////////////////////////////////////////////////
#include <tmos.h>
#include <drivers.h>
#include <systick_drv.h>
#include <tim_drv.h>
#include <wifi_drv.h>
#include <esp8266.h>

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
// 		 GPIO / EXTI DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
EXTI_DRIVER_DATA gpio_data;
const EXTI_DRIVER_INFO exti0_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI0_1_IRQn,
			DRV_PRIORITY_EXTI0,
			ID_PERIPH_GPIOA
		},
		EXTI,
		EXTI_IMR_MR0 | EXTI_IMR_MR1,
		&gpio_data
};

//const EXTI_DRIVER_INFO exti1_driver =
//{
//		{
//			DRIVER_INFO_STUB,
//			(DRV_ISR)EXTI_ISR,
//			(DRV_DCR)EXTI_DCR,
//			(DRV_DSR)EXTI_DSR,
//			EXTI0_1_IRQn,
//			DRV_PRIORITY_EXTI1,
//			ID_NO_PERIPH
//		},
//		EXTI,
//		EXTI_IMR_MR1,
//		&gpio_data
//};

const EXTI_DRIVER_INFO exti2_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI2_3_IRQn,
			DRV_PRIORITY_EXTI2,
			ID_NO_PERIPH
		},
		EXTI,
		EXTI_IMR_MR2 | EXTI_IMR_MR3,
		&gpio_data
};

//const EXTI_DRIVER_INFO exti3_driver =
//{
//		{
//			DRIVER_INFO_STUB,
//			(DRV_ISR)EXTI_ISR,
//			(DRV_DCR)EXTI_DCR,
//			(DRV_DSR)EXTI_DSR,
//			EXTI2_3_IRQn,
//			DRV_PRIORITY_EXTI3,
//			ID_NO_PERIPH
//		},
//		EXTI,
//		EXTI_IMR_MR3,
//		&gpio_data
//};

const EXTI_DRIVER_INFO exti4_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI4_15_IRQn,
			DRV_PRIORITY_EXTI4,
			ID_NO_PERIPH
		},
		EXTI,
		EXTI_IMR_MR4 | EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7 | EXTI_IMR_MR8 | EXTI_IMR_MR9 | EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12 | EXTI_IMR_MR13 | EXTI_IMR_MR14 | EXTI_IMR_MR15,
		&gpio_data
};

//const EXTI_DRIVER_INFO exti5_driver =
//{
//		{
//			DRIVER_INFO_STUB,
//			(DRV_ISR)EXTI_ISR,
//			(DRV_DCR)EXTI_DCR,
//			(DRV_DSR)EXTI_DSR,
//			EXTI4_15_IRQn,
//			DRV_PRIORITY_EXTI5,
//			ID_NO_PERIPH
//		},
//		EXTI,
//		EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7 | EXTI_IMR_MR8 | EXTI_IMR_MR9,
//		&gpio_data
//};

//const EXTI_DRIVER_INFO exti10_driver =
//{
//		{
//			DRIVER_INFO_STUB,
//			(DRV_ISR)EXTI_ISR,
//			(DRV_DCR)EXTI_DCR,
//			(DRV_DSR)EXTI_DSR,
//			EXTI4_15_IRQn,
//			DRV_PRIORITY_EXTI10,
//			ID_NO_PERIPH
//		},
//		EXTI,
//		EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12 | EXTI_IMR_MR13 | EXTI_IMR_MR15 | EXTI_IMR_MR15,
//		&gpio_data
//};



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART 1 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const USART_DRIVER_MODE uart1_mode =
{
	115200,									//!< baudrate e.g. 9600
	USART_CR1_RE | USART_CR1_TE | USART_CR1_IDLEIE | USART_CR1_RXNEIE |
		USART_CR1_TXEIE | USART_CR1_UE,		//!< USART_CR1 register value
	USART_CR2_STOP_1b,						//!< USART_CR2 register value
	0										//!< USART_CR3 register value
};

USART_DRIVER_DATA uart1_drv_data;
const USART_DRIVER_INFO uart1_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)USART_ISR,
			(DRV_DCR)USART_DCR,
			(DRV_DSR)USART_DSR,
			USART1_IRQn,
			DRV_PRIORITY_UART1,
			ID_PERIPH_USART1
		},
		USART1,
		{PIN_UART1_RX, PIN_UART1_TX, 0, 0, 0},      // pin Rx , pin Tx, pin RTS, pin CTS
		&uart1_drv_data
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART 2 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const USART_DRIVER_MODE uart_default_mode =
{
	115200,									//!< baudrate e.g. 9600
	USART_CR1_RE | USART_CR1_TE | USART_CR1_IDLEIE | USART_CR1_RXNEIE |
		USART_CR1_TXEIE | USART_CR1_UE,		//!< USART_CR1 register value
	USART_CR2_STOP_1b,						//!< USART_CR2 register value
	0										//!< USART_CR3 register value
};

const USART_DRIVER_MODE uart_9600_mode =
{
	9600,									//!< baudrate e.g. 9600
	USART_CR1_RE | USART_CR1_TE | USART_CR1_IDLEIE | USART_CR1_RXNEIE |
		USART_CR1_TXEIE | USART_CR1_UE,		//!< USART_CR1 register value
	USART_CR2_STOP_1b,						//!< USART_CR2 register value
	0										//!< USART_CR3 register value
};

USART_DRIVER_DATA uart2_drv_data;
const USART_DRIVER_INFO uart2_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)USART_ISR,
			(DRV_DCR)USART_DCR,
			(DRV_DSR)USART_DSR,
			USART2_IRQn,
			DRV_PRIORITY_UART2,
			ID_PERIPH_USART2
		},
		USART2,
		{PIN_UART2_RX, PIN_UART2_TX, 0, 0, 0},      // pin Rx , pin Tx, pin RTS, pin CTS
		&uart2_drv_data
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 TIMER DRIVERs
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//TIMER_DRIVER_DATA timer_drv_data_table[1];	//TIM2
//
//const TIMER_DRIVER_INFO t2cci_driver =
//{
//		{
//			DRIVER_INFO_STUB,
//			(DRV_ISR)TIM_ISR,
//			(DRV_DCR)TIM_DCR,
//			(DRV_DSR)TIM_DSR,
//			TIM2_IRQn,
//			DRV_PRIORITY_TIMER,
//			ID_PERIPH_TIM2
//		},
//		TIM2,					//!< Timer Hardware registers for control
//		TIM2,					//!< Timer Hardware registers for channels
//		timer_drv_data_table,	//!< driver data for all timers
//		0,						//!< driver data idx for control
//		0,						//!< driver data idx for channels
//		0						//!< which TIM_SR flags to process
//};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPI 1-2 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SPI_DRIVER_DATA spi1_drv_data;
const SPI_DRIVER_INFO spi1_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)SPI_ISR,
			(DRV_DCR)SPI_DCR,
			(DRV_DSR)SPI_DSR,
			SPI1_IRQn,
			DRV_PRIORITY_SPI1,
			ID_PERIPH_SPI1
		},
		SPI1,
		&spi1_drv_data,
		{
			PIN_SPI1_SCK,
			PIN_SPI1_MISO,
			PIN_SPI1_MOSI,
			0
		}
};

const SPI_DRIVER_MODE g_rfm73_mode =
{
		PIN_RFM_CS,		//CS
		// SPI_CR1 register value
		SPI_CR1_BR_DIV16 | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSBFIRST |
		SPI_CR1_MSTR | SPI_CR1_CPHA | SPI_CR1_CPOL,
		// SPI_CR2 register value (FRF and SSOE only)
		SPI_CR2_DS_8bit | SPI_CR2_FRF_MOTO
};




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 WIFI DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

WIFI_DRIVER_DATA wifi_drv_data;

const WIFI_DRIVER_INFO wifi_driver =
{
		{
			DRIVER_INFO_STUB,
			DEFAULT_DRIVER_ISR,
			(DRV_DCR)WIFI_DCR,
			(DRV_DSR)WIFI_DSR,
			WIFI_DRV_INDX,
			DRV_PRIORITY_WIFI,
			ID_NO_PERIPH
		},
		&wifi_drv_data,
		USART1_IRQn,
		{
			&uart_default_mode,
			&uart_9600_mode
		}
};

esp8266_module g_esp8266(&wifi_driver);

extern "C" wifi_module_type* wifi_detect(WIFI_DRIVER_INFO* drv_info)
{
	return &g_esp8266;
}


//=================== DRV_TABLE ==========================================
// All drivers in the system must be listed in this table
// To open a driver, only the driver index is required (defined in drivers.h)
// The driver index must match with the driver position in this table
extern "C" char * const DRV_TABLE[INALID_DRV_INDX+1] __attribute__ ((section (".DriverTable")))  =
{
	1+ (char * const)&DefaultDriver,    /*!< 0 Window WatchDog Interrupt */
	1+ (char * const)&DefaultDriver,  	/*!< 1 Empty */
	1+ (char * const)&DefaultDriver,    /*!< 2 RTC Interrupt through EXTI Lines 17, 19 and 20                  */
	1+ (char * const)&DefaultDriver,    /*!< 3 FLASH global Interrupt                                          */
	1+ (char * const)&DefaultDriver,    /*!< 4 RCC global Interrupt                                            */
	1+ (char * const)&exti0_driver,     /*!< 5 EXTI Line 0 and 1 Interrupts                                    */
	1+ (char * const)&exti2_driver,   	/*!< 6 EXTI Line 2 and 3 Interrupts                                    */
	1+ (char * const)&exti4_driver,     /*!< 7 EXTI Line 4 to 15 Interrupts  */
	1+ (char * const)&DefaultDriver,    /*!< 8 Empty */
	1+ (char * const)&DefaultDriver,    /*!< 9 DMA1 Channel 1 Interrupt                                        */
	1+ (char * const)&DefaultDriver,    /*!< 10 DMA1 Channel 2 and Channel 3 Interrupts                         */
	1+ (char * const)&DefaultDriver,    /*!< 11 DMA1 Channel 4 and Channel 5 Interrupts                         */
	1+ (char * const)&DefaultDriver,    /*!< 12 ADC1 global Interrupt                                           */
	1+ (char * const)&DefaultDriver,    /*!< 13 TIM1 Break, Update, Trigger and Commutation Interrupts          */
	1+ (char * const)&DefaultDriver,    /*!< 14 TIM1 Capture Compare Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!< 15 Empty */
	1+ (char * const)&DefaultDriver,    /*!< 16 TIM3 global Interrupt                                           */
	1+ (char * const)&DefaultDriver,    /*!< 17 TIM6 global Interrupt */
	1+ (char * const)&DefaultDriver, 	/*!< 18 Empty */
	1+ (char * const)&DefaultDriver,    /*!< 19 TIM14 global Interrupt                                          */
	1+ (char * const)&DefaultDriver,    /*!< 20 TIM15 global Interrupt                                          */
	1+ (char * const)&DefaultDriver,    /*!< 21 TIM16 global Interrupt                                          */
	1+ (char * const)&DefaultDriver,    /*!< 22 TIM17 global Interrupt                                          */
	1+ (char * const)&DefaultDriver,    /*!< 23 I2C1 Event Interrupt                                            */
	1+ (char * const)&DefaultDriver,    /*!< 24 I2C2 Event Interrupt                                            */
	1+ (char * const)&spi1_driver,      /*!< 25 SPI1 global Interrupt                                           */
	1+ (char * const)&DefaultDriver,    /*!< 26 SPI2 global Interrupt                                           */
	1+ (char * const)&uart1_driver,     /*!< 27 USART1 global Interrupt                                         */
	1+ (char * const)&uart2_driver,     /*!< 28 USART2 global Interrupt										    */
	1+ (char * const)&wifi_driver,      /*!< 29 WiFi Driver													    */

    NULL				//null terminated list
};





