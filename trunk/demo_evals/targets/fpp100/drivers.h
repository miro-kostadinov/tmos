/*
 * drivers.h
 *
 *  Created on: Nov 12, 2012
 *      Author: miro
 */

#ifndef DRIVERS_H_
#define DRIVERS_H_

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 target header files...
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <tmos.h>
#include <hardware_cpp.h>


extern "C" char __BUILD_VERSION;

#define LOCK_DEVICE 0
#define RELEASE 0

//----- Driver Priorities ----------------//
#define DRV_PRIORITY_KERNEL		0x01	// OS-aware ISRs  must have lower priority (higher number)
#define DRV_PRIORITY_UART0		0x02
#define DRV_PRIORITY_UART1		0x02
#define DRV_PRIORITY_UART2		0x02
#define DRV_PRIORITY_DMA1_CH0	0x03	// must be above all DMA clients
#define DRV_PRIORITY_DMA1_CH1	0x03
#define DRV_PRIORITY_DMA1_CH2	0x03
#define DRV_PRIORITY_DMA1_CH2	0x03
#define DRV_PRIORITY_DMA1_CH3	0x03
#define DRV_PRIORITY_DMA1_CH4	0x03
#define DRV_PRIORITY_DMA1_CH5   0x03
#define DRV_PRIORITY_DMA2_CH0	0x03
#define DRV_PRIORITY_DMA2_CH1	0x03
#define DRV_PRIORITY_DMA2_CH2	0x03
#define DRV_PRIORITY_DMA2_CH3	0x03
#define DRV_PRIORITY_DMA2_CH4	0x03
#define DRV_PRIORITY_DMA2_CH5   0x03
#define DRV_PRIORITY_EXTI0		0x04
#define DRV_PRIORITY_EXTI1		0x04
#define DRV_PRIORITY_EXTI2		0x04
#define DRV_PRIORITY_EXTI3		0x04
#define DRV_PRIORITY_EXTI4		0x04
#define DRV_PRIORITY_EXTI5		0x04
#define DRV_PRIORITY_EXTI10		0x04
#define DRV_PRIORITY_SPI1		0x04
#define DRV_PRIORITY_USB		0x04
#define DRV_PRIORITY_WDT		0x05
#define DRV_PRIORITY_DEFAULT	0x06


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 DRIVER INFO
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//subw	r0, pc, #4
//ldr.w	pc, [r0, #8]
#define DRIVER_INFO_STUB  0x0004f2af, 0xf008f8d0



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <usart_drv.h>

extern const USART_DRIVER_MODE uart_default_mode;

#define UART_TEST_DRIVER USART1_IRQn

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <gpio_stm32.h>
#include <exti_drv.h>

// Port A
#define PIN_UART4_TX	(PD_PA0  | PD_AF_UART4)									//!< UART 4
#define PIN_PA1			(PD_PA1  | PD_OUT)										//!< unused
#define PIN_ETH_MDIO	(PD_PA2  | PD_AF_ETH)									//!< eth
#define PIN_PRN2_TH		(PD_PA3  | PD_ANALOG)									//!< prn 2
#define PIN_VPRN		(PD_PA4  | PD_ANALOG)									//!< prn
#define PIN_SPI1_SCK	(PD_PA5  | PD_AF_SPI1)									//!< spi 1
#define PIN_SPI1_MISO	(PD_PA6  | PD_AF_SPI1)									//!< spi 1
#define PIN_ETH_CRS_DV	(PD_PA7  | PD_AF_ETH)									//!< eth
#define PIN_PA8			(PD_PA8  | PD_OUT)										//!< unused
#define PIN_UART1_TX	(PD_PA9  | PD_AF_USART1)								//!< usart 1
#define PIN_UART1_RX	(PD_PA10 | PD_AF_USART1)								//!< usart 1
#define PIN_PRN1_STB1	(PD_PA11 | PD_OUT)										//!< prn 1
#define PIN_PRN1_LATCH	(PD_PA12 | PD_OUT)										//!< prn 1
#define PIN_TMS			(PD_PA13 | PD_AF)										//!<
#define PIN_TCK			(PD_PA14 | PD_AF)										//!<
#define PIN_TDI			(PD_PA15 | PD_AF)										//!<

// Port B
#define PIN_LCD_CS0		(PD_PB0  | PD_OUT | PD_ACTIVE_LOW)						//!< lcd
#define PIN_LCD_CS1		(PD_PB1  | PD_OUT | PD_ACTIVE_LOW)						//!< lcd
#define PIN_SDIO_CS		(PD_PB2  | PD_OUT)										//!< sdio
#define PIN_TDO			(PD_PB3  | PD_AF)										//!<
#define PIN_IO_CS		(PD_PB4  | PD_OUT)										//!< io
#define PIN_SPI1_MOSI	(PD_PB5  | PD_AF_SPI1)									//!< spi 1
#define PIN_PRN1_PH1	(PD_PB6  | PD_OUT)										//!< prn 1
#define PIN_PRN1_PH2	(PD_PB7  | PD_OUT) 										//!< prn 1
#define PIN_PRN1_PH3	(PD_PB8  | PD_OUT)										//!< prn 1
#define PIN_PRN1_PH4	(PD_PB9  | PD_OUT)										//!< prn 1
#define PIN_SPI2_SCK	(PD_PB10 | PD_AF_SPI2)									//!< spi 2
#define PIN_ETH_TX_EN	(PD_PB11 | PD_AF_ETH)									//!< eth
#define PIN_PB12		(PD_PB12 | PD_OUT)										//!< unused
#define PIN_USB_VBUS	(PD_PB13 | PD_IN)										//!< usb
#define PIN_USB_DM		(PD_PB14 | PD_AF12)										//!< usb
#define PIN_USB_DP		(PD_PB15 | PD_AF12)										//!< usb

// Port C
#define PIN_PRN2_PS		(PD_PC0  | PD_IN)										//!< prn 2
#define PIN_ETH_MDC		(PD_PC1  | PD_IN)										//!< eth
#define PIN_PWR_ADC		(PD_PC2  | PD_ANALOG)									//!< pwr
#define PIN_DI			(PD_PC3  | PD_AF_SPI2)									//!< spi 2
#define PIN_ETH_RXD0	(PD_PC4  | PD_AF_ETH)									//!< eth
#define PIN_ETH_RXD1	(PD_PC5  | PD_AF_ETH)									//!< eth
#define PIN_USART6_TX	(PD_PC6  | PD_AF_USART6)								//!< usart 6
#define PIN_USART6_RX	(PD_PC7  | PD_AF_USART6)				 				//!< usart 6
#define PIN_LCD_BL		(PD_PC8  | PD_OUT)                                      //!< lcd
#define PIN_GSM_ON		(PD_PC9  | PD_OUT | PD_ACTIVE_HIGH)                     //!< gsm
#define PIN_SPI3_SCK	(PD_PC10 | PD_AF_SPI3)                                  //!< spi 3
#define PIN_UART4_RX	(PD_PC11 | PD_AF_UART4) 			                    //!< uart 4
#define PIN_SPI3_MOSI 	(PD_PC12 | PD_AF_SPI3) 				                    //!< spi 3
#define PIN_JP3			(PD_PC13 | PD_IN)                                       //!< jp3
#define PIN_OSC32_IN	(PD_PC14 | PD_AF)                                       //!< osc 32khz
#define PIN_OSC32_OUT	(PD_PC15 | PD_AF)                                       //!< osc 32khz

// Port D
#define PIN_M_D2		(PD_PD0  | PD_AF_FSMC)									//!< bus
#define PIN_M_D3		(PD_PD1  | PD_AF_FSMC)									//!< bus
#define PIN_DOPEN 		(PD_PD2  | PD_IN)										//!< dopen?
#define PIN_PFO			(PD_PD3  | PD_OUT)										//!< scard
#define PIN_M_OE		(PD_PD4  | PD_AF_FSMC)									//!< bus
#define PIN_M_WE		(PD_PD5  | PD_AF_FSMC)									//!< bus
#define PIN_BOX			(PD_PD6  | PD_OUT | PD_ACTIVE_HIGH)						//!< box/service mode in
#define PIN_MCS0		(PD_PD7  | PD_AF_FSMC)					 				//!< bus
#define PIN_SC_IO		(PD_PD8  | PD_AF_USART3)                                //!< scard
#define PIN_SC_RST		(PD_PD9  | PD_OUT)       			          			//!< scard
#define PIN_SC_CLK		(PD_PD10 | PD_AF_USART3)                                //!< scard
#define PIN_M_A16		(PD_PD11 | PD_AF_FSMC)				                    //!< bus
#define PIN_M_A17	 	(PD_PD12 | PD_AF_FSMC)				                    //!< bus
#define PIN_M_A18		(PD_PD13 | PD_AF_FSMC)                                  //!< bus
#define PIN_M_D0		(PD_PD14 | PD_AF_FSMC)                                  //!< bus
#define PIN_M_D1		(PD_PD15 | PD_AF_FSMC)                                  //!< bus

// Port E
#define PIN_KEY0		(PD_PE0  | PD_IN | PD_PULL_UP | PD_INT_BE)				//!< key
#define PIN_KEY1		(PD_PE1  | PD_IN | PD_PULL_UP | PD_INT_BE)				//!< key
#define PIN_PRN2_LATCH	(PD_PE2  | PD_OUT)										//!< prn 2
#define PIN_M_A19		(PD_PE3  | PD_AF_FSMC)									//!< bus
#define PIN_M_A20		(PD_PE4  | PD_AF_FSMC)									//!< bus
#define PIN_KEY2		(PD_PE5  | PD_IN | PD_PULL_UP | PD_INT_BE)				//!< key
#define PIN_KEY3		(PD_PE6  | PD_IN | PD_PULL_UP | PD_INT_BE)				//!< key
#define PIN_M_D4		(PD_PE7  | PD_AF_FSMC)					 				//!< bus
#define PIN_M_D5		(PD_PE8  | PD_AF_FSMC)                                  //!< bus
#define PIN_M_D6		(PD_PE9  | PD_AF_FSMC)         					        //!< bus
#define PIN_M_D7		(PD_PE10 | PD_AF_FSMC)                                  //!< bus
#define PIN_PRN2_PPH1	(PD_PE11 | PD_OUT) 					                    //!< prn 2
#define PIN_BUZ0	 	(PD_PE12 | PD_AF) 					                    //!< buz
#define PIN_BUZ1		(PD_PE13 | PD_AF)                                       //!< buz
#define PIN_PRN2_PPH2	(PD_PE14 | PD_OUT)                                      //!< prn 2
#define PIN_PRN2_STB1	(PD_PE15 | PD_OUT)                                      //!< prn 2

// Port F
#define PIN_M_A0		(PD_PF0  | PD_AF_FSMC)									//!< bus
#define PIN_M_A1		(PD_PF1  | PD_AF_FSMC)									//!< bus
#define PIN_M_A2	 	(PD_PF2  | PD_AF_FSMC)									//!< bus
#define PIN_M_A3		(PD_PF3  | PD_AF_FSMC)									//!< bus
#define PIN_M_A4		(PD_PF4  | PD_AF_FSMC)									//!< bus
#define PIN_M_A5		(PD_PF5  | PD_AF_FSMC)									//!< bus
#define PIN_PRN1_HS		(PD_PF6  | PD_ANALOG)									//!< prn 1
#define PIN_PRN1_PS		(PD_PF7  | PD_ANALOG)						 			//!< prn 1
#define PIN_PRN1_CS		(PD_PF8  | PD_OUT)                                      //!< prn 1
#define PIN_PRN1_TH		(PD_PF9  | PD_ANALOG)				                    //!< prn 1
#define PIN_PRN2_HS		(PD_PF10 | PD_ANALOG)                                   //!< prn 2
#define PIN_LCD_RST		(PD_PF11 | PD_OUT) 					                    //!< lcd
#define PIN_M_A6	 	(PD_PF12 | PD_AF_FSMC)				                    //!< bus
#define PIN_M_A7		(PD_PF13 | PD_AF_FSMC)                                  //!< bus
#define PIN_M_A8		(PD_PF14 | PD_AF_FSMC)                                  //!< bus
#define PIN_M_A9		(PD_PF15 | PD_AF_FSMC)                                  //!< bus

// Port G
#define PIN_M_A10		(PD_PG0  | PD_AF_FSMC)									//!< bus
#define PIN_M_A11		(PD_PG1  | PD_AF_FSMC)									//!< bus
#define PIN_M_A12		(PD_PG2  | PD_AF_FSMC)									//!< bus
#define PIN_M_A13		(PD_PG3  | PD_AF_FSMC)									//!< bus
#define PIN_M_A14		(PD_PG4  | PD_AF_FSMC)									//!< bus
#define PIN_M_A15		(PD_PG5  | PD_AF_FSMC)									//!< bus
#define PIN_LED0		(PD_PG6  | PD_OUT | PD_ACTIVE_HIGH)						//!< led
#define PIN_LED1		(PD_PG7  | PD_OUT | PD_ACTIVE_HIGH)						//!< led
#define PIN_LED2		(PD_PG8  | PD_OUT | PD_ACTIVE_HIGH)                     //!< led
#define PIN_M_CS1		(PD_PG9  | PD_AF_FSMC)				                    //!< bus
#define PIN_LED3		(PD_PG10 | PD_OUT | PD_ACTIVE_HIGH)                     //!< led
#define PIN_ETH_IRQ		(PD_PG11 | PD_IN) 					                    //!< eth
#define PIN_GSM_CTS 	(PD_PG12 | PD_AF_USART6)			                    //!< gsm (uart6 rts)
#define PIN_ETH_TXD0	(PD_PG13 | PD_AF_ETH)                                   //!< eth
#define PIN_ETH_TXD1	(PD_PG14 | PD_AF_ETH)                                   //!< eth
#define PIN_GSM_RTS		(PD_PG15 | PD_AF_USART6)                                //!< gsm (usart6 cts)

#define PIN_LED		PIN_LED0	// for LED test
#define PIN_BUTTON	PIN_KEY0

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 DMA DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <dma_drv.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPI DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <spi_drv.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern const unsigned char key_codes[];
extern const unsigned int KEY_RD_PINS[];

#include <key_drv.h>
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GUI DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <gui_drv.h>

extern const PIN_DESC lcd1pins[];
extern const PIN_DESC lcd2pins[];
extern const SPI_DRIVER_MODE lcd1_mode_stru;
extern const SPI_DRIVER_MODE lcd2_mode_stru;




extern   signed char const DRV_RESET_FIRST_TABLE[];
extern "C" char* const DRV_TABLE[INALID_DRV_INDX+1];


#define TEST_MEM2MEM_DMA 0


#endif /* DRIVERS_H_ */
