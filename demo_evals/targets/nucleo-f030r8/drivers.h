#ifndef	DRIVERS_H
#define	DRIVERS_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 target header files...
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <tmos.h>
#include <hardware_cpp.h>

//#include <mcu.h>
#include <handle.h>

//#define SOFTWARE_VERSION 			0x0226
//#define SOFTWARE_VERSION_STRING 	"2.26.8"
extern "C" char __BUILD_VERSION;

#define LOCK_DEVICE 0
#define RELEASE 0

//#define RF_TEST 1

//----- Driver Priorities ----------------//
#define DRV_PRIORITY_KERNEL		NVIC_PIORITY(0x01)	// OS-aware ISRs  must have lower priority (higher number)
#define DRV_PRIORITY_UART0		NVIC_PIORITY(0x02)
#define DRV_PRIORITY_UART1		NVIC_PIORITY(0x02)
#define DRV_PRIORITY_UART2		NVIC_PIORITY(0x02)
#define DRV_PRIORITY_EXTI0		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_EXTI1		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_EXTI2		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_EXTI3		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_EXTI4		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_EXTI5		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_EXTI10		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_SPI1		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_USB		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_TIMER		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_ADC		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_WIFI		NVIC_PIORITY(0x03)
#define DRV_PRIORITY_WDT		NVIC_PIORITY(0x04)


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 DRIVER INFO
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef const struct DRIVER_INFO_Type * DRIVER_INFO;
typedef void (*DRV_DCR)(DRIVER_INFO drv_info, unsigned int reason, void *param);
typedef void (*DRV_DSR)(DRIVER_INFO drv_info, HANDLE hnd);
typedef void (*DRV_ISR)(DRIVER_INFO drv_info);





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <usart_drv.h>

extern const USART_DRIVER_MODE uart_default_mode;

#define UART_TEST_DRIVER USART2_IRQn

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPI DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <spi_drv.h>

#define USE_RFM73 0
#define RFM73_SPI_DRIVER SPI1_IRQn

extern const SPI_DRIVER_MODE g_rfm73_mode;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <gpio_stm32.h>
#include <exti_drv.h>

// Port A
#define PIN_WAKE_UP		(PD_PA0  | PD_IN | PD_INT_BE)							//!< CN8.1	CN7.28	ADC_IN0
#define PIN_UART2_RTS	(PD_PA1  | PD_IN)										//!< CN8.2	CN7.30	ADC_IN1
#define PIN_UART2_TX	(PD_PA2  | PD_AF_USART2)								//!< CN9.2	CN10.35	USART2_TX
#define PIN_UART2_RX	(PD_PA3  | PD_AF_USART2)								//!< CN9.1	CN10.37	USART2_RX
#define PIN_SPI1_NSS	(PD_PA4  | PD_OUT | PD_ACTIVE_LOW)						//!< CN8.3	CN7.32	ADC_IN4
#define PIN_LED			(PD_PA5  | PD_OUT | PD_ACTIVE_HIGH)						//!< CN5.6	CN10.11	SPI1_SCK
#define PIN_A6			(PD_PA6  | PD_IN)										//!< CN5.5	CN10.13	SPI1_MISO
#define PIN_A7			(PD_PA7  | PD_IN)										//!< CN5.4	CN10.15	TIM17_CH1 or SPI1_MOSI
#define PIN_RFM_IRQ		(PD_PA8  | PD_IN | PD_INT_BE)							//!< CN9.8	CN10.23
#define PIN_UART1_TX	(PD_PA9  | PD_AF1)										//!< CN5.1	CN10.21
#define PIN_UART1_RX	(PD_PA10 | PD_AF1)										//!< CN9.3
#define PIN_A11			(PD_PA11 | PD_AF)										//!< CN10.14
#define PIN_A12			(PD_PA12 | PD_AF)										//!< CN10.12
#define PIN_TMS			(PD_PA13 | PD_AF)										//!< CN7.13			TMS
#define PIN_TCK			(PD_PA14 | PD_AF)										//!< CN7.15			TCK
#define PIN_TDI			(PD_PA15 | PD_AF)										//!<

// Port B
#define PIN_PB0			(PD_PB0  | PD_IN)										//!< CN8.4	CN7.34	ADC_IN8
#define PIN_PB1			(PD_PB1  | PD_IN)										//!< 		CN10.24
#define PIN_PB2 		(PD_PB2  | PD_IN)										//!< 		CN10.22
#define PIN_SPI1_SCK	(PD_PB3  | PD_AF_SPI1)									//!< CN9.4	CN10.31
#define PIN_SPI1_MISO	(PD_PB4  | PD_AF_SPI1)									//!< CN9.6	CN10.27	TIM3_CH1
#define PIN_SPI1_MOSI	(PD_PB5  | PD_AF_SPI1)									//!< CN9.5	CN10.29
#define PIN_I2C1_SCL	(PD_PB6  | PD_AF)										//!< CN5.3	CN10.17	TIM16_CH1N or SPI1_CS
#define PIN_I2C1_SDA	(PD_PB7  | PD_AF) 										//!< CN7.21
#define PIN_CAN_RX		(PD_PB8  | PD_AF)										//!< CN5.10	CN10.3	I2C1_SCL
#define PIN_CAN_TX		(PD_PB9  | PD_AF)										//!< CN5.9	CN10.5	I2C1_SDA
#define PIN_RFM_CS		(PD_PB10 | PD_OUT | PD_ACTIVE_LOW)						//!< CN9.7	CN10.25
#define PIN_PB11		(PD_PB11 | PD_IN)										//!< CN10.18
#define PIN_SPI2_NSS	(PD_PB12 | PD_OUT | PD_ACTIVE_LOW)						//!< CN10.16
#define PIN_SPI2_SCK	(PD_PB13 | PD_AF)										//!< 		CN10.30
#define PIN_SPI2_MISO	(PD_PB14 | PD_AF)										//!< 		CN10.28
#define PIN_SPI2_MOSI	(PD_PB15 | PD_AF)										//!< 		CN10.26

// Port C
#define PIN_PC0			(PD_PC0  | PD_IN)										//!< CN8.6	CN7.38	ADC_IN10
#define PIN_PC1			(PD_PC1  | PD_IN)										//!< CN8.5	CN7.36	ADC_IN11
#define PIN_PC2 		(PD_PC2  | PD_IN)										//!< CN7.35
#define PIN_PC3			(PD_PC3  | PD_IN)										//!< CN7.37
#define PIN_USB_P 		(PD_PC4  | PD_IN)										//!< CN10.34
#define PIN_PC5			(PD_PC5  | PD_IN)										//!< CN10.6
#define PIN_SD_WP		(PD_PC6  | PD_IN)										//!< CN10.4
#define PIN_RFM_CE		(PD_PA10 | PD_OUT | PD_ACTIVE_HIGH)		 				//!< CN5.2	CN10.19	TIM3_CH2
#define PIN_PC8			(PD_PC8  | PD_OUT | PD_ACTIVE_HIGH)                     //!< CN10.2
#define PIN_PC9			(PD_PC9  | PD_IN)                                       //!< CN10.1
#define PIN_CAN_CNTRL	(PD_PC10 | PD_IN)                                       //!< CN7.1
#define PIN_USB_PUP		(PD_PC11 | PD_OUT | PD_ACTIVE_LOW)                      //!< CN7.2
#define PIN_PC12 		(PD_PC12 | PD_OUT | PD_ACTIVE_LOW)                      //!< CN7.3
#define PIN_PC13		(PD_PC13 | PD_IN)                                       //!< CN7.23 swd
#define PIN_PC14		(PD_PC14 | PD_IN)                                       //!< CN7.25 swd
#define PIN_PC15		(PD_PC15 | PD_IN)                                       //!< CN7.27

// Port D
#define PIN_OSC_IN		(PD_PD0 | PD_AF)										//!<
#define PIN_OSC_OUT		(PD_PD1 | PD_AF)										//!<
#define PIN_PD2 		(PD_PD2 | PD_IN | PD_INT_BE)							//!< CN7.4

// Port F
#define PIN_PF0			(PD_PF0 | PD_IN)                                        //!< CN7.29
#define PIN_PF1			(PD_PF1 | PD_IN)                                        //!< CN7.31
#define PIN_PF4			(PD_PF4 | PD_IN)                                        //!< CN10.38
#define PIN_PF5			(PD_PF5 | PD_IN)                                        //!< CN10.36
#define PIN_PF6			(PD_PF6 | PD_IN)                                        //!< CN7.9
#define PIN_PF7			(PD_PF7 | PD_IN)                                        //!< CN7.11

#define PIN_BUTTON PIN_WAKE_UP

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 ADC DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define TEST_ADC	1
#define ADC_TEST_DRV ADC1_IRQn
#include <adc1_drv.h>
extern const ADC_DRIVER_MODE adc_test_mode;


extern "C" char* const DRV_TABLE[INALID_DRV_INDX+1];

#endif
