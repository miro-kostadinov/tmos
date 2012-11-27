/*
 * drivers.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: miro
 */
#include <tmos.h>
#include <drivers.h>
#include <systick_drv.h>

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
		DRV_PRIORITY_DEFAULT,
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
			EXTI0_IRQn,
			DRV_PRIORITY_EXTI0,
			ID_PERIPH_SYSC
		},
		EXTI,
		EXTI_IMR_MR0,
		&gpio_data
};

const EXTI_DRIVER_INFO exti1_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI1_IRQn,
			DRV_PRIORITY_EXTI1,
			ID_NO_PERIPH
		},
		EXTI,
		EXTI_IMR_MR1,
		&gpio_data
};

const EXTI_DRIVER_INFO exti2_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI2_IRQn,
			DRV_PRIORITY_EXTI2,
			ID_NO_PERIPH
		},
		EXTI,
		EXTI_IMR_MR2,
		&gpio_data
};

const EXTI_DRIVER_INFO exti3_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI3_IRQn,
			DRV_PRIORITY_EXTI3,
			ID_NO_PERIPH
		},
		EXTI,
		EXTI_IMR_MR3,
		&gpio_data
};

const EXTI_DRIVER_INFO exti4_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI4_IRQn,
			DRV_PRIORITY_EXTI4,
			ID_NO_PERIPH
		},
		EXTI,
		EXTI_IMR_MR4,
		&gpio_data
};

const EXTI_DRIVER_INFO exti5_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI9_5_IRQn,
			DRV_PRIORITY_EXTI5,
			ID_NO_PERIPH
		},
		EXTI,
		EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7 | EXTI_IMR_MR8 | EXTI_IMR_MR9,
		&gpio_data
};

const EXTI_DRIVER_INFO exti10_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)EXTI_ISR,
			(DRV_DCR)EXTI_DCR,
			(DRV_DSR)EXTI_DSR,
			EXTI15_10_IRQn,
			DRV_PRIORITY_EXTI10,
			ID_NO_PERIPH
		},
		EXTI,
		EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12 | EXTI_IMR_MR13 | EXTI_IMR_MR15 | EXTI_IMR_MR15,
		&gpio_data
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 DMA 1 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DMA_DRIVER_DATA dma1_drv_data;
DMA_DRIVER_DATA dma2_drv_data;

DMA_CHANNEL_DATA dma1_ch0_data;
const DMA_DRIVER_INFO dma1_ch0_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)DMA_ISR,
			(DRV_DCR)DMA_DCR,
			(DRV_DSR)DMA_DSR,
			DMA1_Stream0_IRQn,
			DRV_PRIORITY_DMA1_CH0,
			ID_PERIPH_DMA1
		},
		DMA1,
		&dma1_drv_data,
		&dma1_ch0_data,
		0								// channel
};

DMA_CHANNEL_DATA dma1_ch1_data;
const DMA_DRIVER_INFO dma1_ch1_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)DMA_ISR,
			(DRV_DCR)DMA_DCR,
			(DRV_DSR)DMA_DSR,
			DMA1_Stream1_IRQn,
			DRV_PRIORITY_DMA1_CH1,
			ID_PERIPH_DMA1
		},
		DMA1,
		&dma1_drv_data,
		&dma1_ch1_data,
		1								// channel
};

DMA_CHANNEL_DATA dma1_ch2_data;
const DMA_DRIVER_INFO dma1_ch2_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)DMA_ISR,
			(DRV_DCR)DMA_DCR,
			(DRV_DSR)DMA_DSR,
			DMA1_Stream2_IRQn,
			DRV_PRIORITY_DMA1_CH2,
			ID_PERIPH_DMA1
		},
		DMA1,
		&dma1_drv_data,
		&dma1_ch2_data,
		2								// channel
};

DMA_CHANNEL_DATA dma2_ch0_data;
const DMA_DRIVER_INFO dma2_ch0_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)DMA_ISR,
			(DRV_DCR)DMA_DCR,
			(DRV_DSR)DMA_DSR,
			DMA2_Stream0_IRQn,
			DRV_PRIORITY_DMA2_CH0,
			ID_PERIPH_DMA2
		},
		DMA2,
		&dma2_drv_data,
		&dma2_ch0_data,
		0								// channel
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
		{(PIN_DESC)PIN_UART1_RX, (PIN_DESC)PIN_UART1_TX, 0, 0, 0},      // pin Rx , pin Tx, pin RTS, pin CTS
		&uart1_drv_data
};



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPI 1 DRIVER
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


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 USB DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const unsigned int KEY_RD_PINS[]=
{
	PIN_KEY0,
	PIN_KEY1,
	PIN_KEY2,
	PIN_KEY3,
	0
};

const unsigned char key_codes[] = "DUKC";

KEY_DRIVER_DATA key_drv_data;
const KEY_DRIVER_INFO key_driver =
{
		{
			DRIVER_INFO_STUB,
			DEFAULT_DRIVER_ISR,
			(DRV_DCR)KEY_DCR,
			(DRV_DSR)KEY_DSR,
			KEY_DRV_INDX,
			DRV_PRIORITY_KERNEL,
			0
		},
		&key_drv_data
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GUI DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


const PIN_DESC lcd1pins[] =
{
	PIN_LCD_BL,						// BKLT_PIN_INDX
	PIN_LCD_RST,					// RST_PIN_INDX
	PIN_SPI1_MISO,					// A9_PIN_INDX
	0
};

const PIN_DESC lcd2pins[] =
{
	PIN_LCD_BL,						// BKLT_PIN_INDX
	PIN_LCD_CS1,					// fake reset (lcd1 will reset)
	PIN_SPI1_MISO,					// A9_PIN_INDX
	0
};

const SPI_DRIVER_MODE lcd1_mode_stru =
{
	PIN_LCD_CS0 ,
	// SPI_CR1 register value
	SPI_CR1_DFF_8bit | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSBFIRST |
	SPI_CR1_BR_DIV256 | SPI_CR1_MSTR | SPI_CR1_CPHA | SPI_CR1_CPOL,
	// SPI_CR2 register value (FRF and SSOE only)
	SPI_CR2_FRF_MOTO
};

const SPI_DRIVER_MODE lcd2_mode_stru =
{
	PIN_LCD_CS1,
	// SPI_CR1 register value
	SPI_CR1_DFF_8bit | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSBFIRST |
	SPI_CR1_BR_DIV256 | SPI_CR1_MSTR | SPI_CR1_CPHA | SPI_CR1_CPOL,
	// SPI_CR2 register value (FRF and SSOE only)
	SPI_CR2_FRF_MOTO
};



GUI_DRIVER_DATA gui_drv_data;

GUI_DRIVER_INFO gui_driver =
{
	{
		DRIVER_INFO_STUB,
		DEFAULT_DRIVER_ISR,
		(DRV_DCR)GUI_DCR,
		(DRV_DSR)GUI_DSR,
		GUI_DRV_INDX,
		DRV_PRIORITY_DEFAULT,
		ID_NO_PERIPH
	},
	&gui_drv_data,
	{
			NULL, // lcd_module will be set from AppInit()
			NULL
	}
};


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
    /******  STM32 F2 specific Interrupt Numbers ***********************************************************/
    1+ (char * const)&DefaultDriver, 	/*!< 0  Window WatchDog Interrupt                                         */
    1+ (char * const)&DefaultDriver, 	/*!< 1  PVD through EXTI Line detection Interrupt                         */
    1+ (char * const)&DefaultDriver, 	/*!< 2  Tamper and TimeStamp interrupts through the EXTI line             */
    1+ (char * const)&DefaultDriver, 	/*!< 3  RTC Wakeup interrupt through the EXTI line                        */
    1+ (char * const)&DefaultDriver, 	/*!< 4  FLASH global Interrupt                                            */
    1+ (char * const)&DefaultDriver, 	/*!< 5  RCC global Interrupt                                              */
    1+ (char * const)&exti0_driver, 	/*!< 6  EXTI Line0 Interrupt                                              */
    1+ (char * const)&exti1_driver, 	/*!< 7  EXTI Line1 Interrupt                                              */
    1+ (char * const)&exti2_driver, 	/*!< 8  EXTI Line2 Interrupt                                              */
    1+ (char * const)&exti3_driver, 	/*!< 9  EXTI Line3 Interrupt                                              */
    1+ (char * const)&exti4_driver, 	/*!< 10 EXTI Line4 Interrupt                                              */
    1+ (char * const)&dma1_ch0_driver, 	/*!< 11 DMA1 Stream 0 global Interrupt                                    */
    1+ (char * const)&dma1_ch1_driver, 	/*!< 12 DMA1 Stream 1 global Interrupt                                    */
    1+ (char * const)&dma1_ch2_driver, 	/*!< 13 DMA1 Stream 2 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 14 DMA1 Stream 3 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 15 DMA1 Stream 4 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 16 DMA1 Stream 5 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 17 DMA1 Stream 6 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 18 ADC1, ADC2 and ADC3 global Interrupts                             */
    1+ (char * const)&DefaultDriver, 	/*!< 19 CAN1 TX Interrupt                                                 */
    1+ (char * const)&DefaultDriver, 	/*!< 20 CAN1 RX0 Interrupt                                                */
    1+ (char * const)&DefaultDriver, 	/*!< 21 CAN1 RX1 Interrupt                                                */
    1+ (char * const)&DefaultDriver, 	/*!< 22 CAN1 SCE Interrupt                                                */
    1+ (char * const)&exti5_driver, 	/*!< 23 External Line[9:5] Interrupts                                     */
    1+ (char * const)&DefaultDriver, 	/*!< 24 TIM1 Break interrupt and TIM9 global interrupt                    */
    1+ (char * const)&DefaultDriver, 	/*!< 25 TIM1 Update Interrupt and TIM10 global interrupt                  */
    1+ (char * const)&DefaultDriver, 	/*!< 26 TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
    1+ (char * const)&DefaultDriver, 	/*!< 27 TIM1 Capture Compare Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 28 TIM2 global Interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 29 TIM3 global Interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 30 TIM4 global Interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 31 I2C1 Event Interrupt                                              */
    1+ (char * const)&DefaultDriver, 	/*!< 32 I2C1 Error Interrupt                                              */
    1+ (char * const)&DefaultDriver, 	/*!< 33 I2C2 Event Interrupt                                              */
    1+ (char * const)&DefaultDriver, 	/*!< 34 I2C2 Error Interrupt                                              */
    1+ (char * const)&spi1_driver, 		/*!< 35 SPI1 global Interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 36 SPI2 global Interrupt                                             */
    1+ (char * const)&uart1_driver, 	/*!< 37 USART1 global Interrupt                                           */
    1+ (char * const)&DefaultDriver, 	/*!< 38 USART2 global Interrupt                                           */
    1+ (char * const)&DefaultDriver, 	/*!< 39 USART3 global Interrupt                                           */
    1+ (char * const)&exti10_driver, 	/*!< 40 External Line[15:10] Interrupts                                   */
    1+ (char * const)&DefaultDriver, 	/*!< 41 RTC Alarm (A and B) through EXTI Line Interrupt                   */
    1+ (char * const)&DefaultDriver, 	/*!< 42 USB OTG FS Wakeup through EXTI line interrupt                     */
    1+ (char * const)&DefaultDriver, 	/*!< 43 TIM8 Break Interrupt and TIM12 global interrupt                   */
    1+ (char * const)&DefaultDriver, 	/*!< 44 TIM8 Update Interrupt and TIM13 global interrupt                  */
    1+ (char * const)&DefaultDriver, 	/*!< 45 TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
    1+ (char * const)&DefaultDriver, 	/*!< 46 TIM8 Capture Compare Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 47 DMA1 Stream7 Interrupt                                            */
    1+ (char * const)&DefaultDriver, 	/*!< 48 FSMC global Interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 49 SDIO global Interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 50 TIM5 global Interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 51 SPI3 global Interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 52 UART4 global Interrupt                                            */
    1+ (char * const)&DefaultDriver, 	/*!< 53 UART5 global Interrupt                                            */
    1+ (char * const)&DefaultDriver, 	/*!< 54 TIM6 global and DAC1&2 underrun error  interrupts                 */
    1+ (char * const)&DefaultDriver, 	/*!< 55 TIM7 global interrupt                                             */
    1+ (char * const)&dma2_ch0_driver, 	/*!< 56 DMA2 Stream 0 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 57 DMA2 Stream 1 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 58 DMA2 Stream 2 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 59 DMA2 Stream 3 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 60 DMA2 Stream 4 global Interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 61 Ethernet global Interrupt                                         */
    1+ (char * const)&DefaultDriver, 	/*!< 62 Ethernet Wakeup through EXTI line Interrupt                       */
    1+ (char * const)&DefaultDriver, 	/*!< 63 CAN2 TX Interrupt                                                 */
    1+ (char * const)&DefaultDriver, 	/*!< 64 CAN2 RX0 Interrupt                                                */
    1+ (char * const)&DefaultDriver, 	/*!< 65 CAN2 RX1 Interrupt                                                */
    1+ (char * const)&DefaultDriver, 	/*!< 66 CAN2 SCE Interrupt                                                */
    1+ (char * const)&DefaultDriver, 	/*!< 67 USB OTG FS global Interrupt                                       */
    1+ (char * const)&DefaultDriver, 	/*!< 68 DMA2 Stream 5 global interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 69 DMA2 Stream 6 global interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 70 DMA2 Stream 7 global interrupt                                    */
    1+ (char * const)&DefaultDriver, 	/*!< 71 USART6 global interrupt                                           */
    1+ (char * const)&DefaultDriver, 	/*!< 72 I2C3 event interrupt                                              */
    1+ (char * const)&DefaultDriver, 	/*!< 73 I2C3 error interrupt                                              */
    1+ (char * const)&DefaultDriver, 	/*!< 74 USB OTG HS End Point 1 Out global interrupt                       */
    1+ (char * const)&DefaultDriver, 	/*!< 75 USB OTG HS End Point 1 In global interrupt                        */
    1+ (char * const)&DefaultDriver, 	/*!< 76 USB OTG HS Wakeup through EXTI interrupt                          */
    1+ (char * const)&DefaultDriver, 	/*!< 77 USB OTG HS global interrupt                                       */
    1+ (char * const)&DefaultDriver, 	/*!< 78 DCMI global interrupt                                             */
    1+ (char * const)&DefaultDriver, 	/*!< 79 CRYP crypto global interrupt                                      */
    1+ (char * const)&DefaultDriver, 	/*!< 80 Hash and Rng global interrupt                                     */
    1+ (char * const)&key_driver, 		//81 key drv
    1+ (char * const)&gui_driver, 		//82 gui drv


   NULL				//null terminated list
};





