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

//----- Driver Priorities ----------------//
#define DRV_PRIORITY_KERNEL		0x01	// OS-aware ISRs  must have lower priority (higher number)
#define DRV_PRIORITY_UART0		0x02
#define DRV_PRIORITY_UART1		0x02
#define DRV_PRIORITY_UART2		0x02
#define DRV_PRIORITY_GPIOA		0x02
#define DRV_PRIORITY_GPIOB		0x02
#define DRV_PRIORITY_GPIOC		0x02
#define DRV_PRIORITY_GPIOD		0x02
#define DRV_PRIORITY_GPIOE		0x02
#define DRV_PRIORITY_GPIOF		0x02
#define DRV_PRIORITY_GPIOG		0x02
#define DRV_PRIORITY_GPIOH		0x02
#define DRV_PRIORITY_GPIOJ		0x02
#define DRV_PRIORITY_SPI0		0x02
#define DRV_PRIORITY_USB		0x02
#define DRV_PRIORITY_WDT		0x02



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 DRIVER INFO
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef const struct DRIVER_INFO_Type * DRIVER_INFO;
typedef void (*DRV_DCR)(DRIVER_INFO drv_info, unsigned int reason, void *param);
typedef void (*DRV_DSR)(DRIVER_INFO drv_info, HANDLE hnd);
typedef void (*DRV_ISR)(DRIVER_INFO drv_info);

//subw	r0, pc, #4
//ldr.w	pc, [r0, #8]
#define DRIVER_INFO_STUB  0x0004f2af, 0xf008f8d0



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <gpio_stm32.h>

// Port A
#define PIN_WAKE_UP		(PD_PA0  | PD_IN)										//!< Wake-up button
#define PIN_UART2_RTS	(PD_PA1  | PD_OUT)										//!< UART 2
#define PIN_UART2_TX	(PD_PA2  | PD_AF)										//!< UART 2
#define PIN_UART2_RX	(PD_PA3  | PD_AF)										//!< UART 2
#define PIN_SPI1_NSS	(PD_PA4  | PD_OUT | PD_ACTIVE_LOW)						//!< UEXT con
#define PIN_SPI1_SCK	(PD_PA5  | PD_AF)										//!< UEXT con
#define PIN_SPI1_MISO	(PD_PA6  | PD_AF)										//!< UEXT con
#define PIN_SPI1_MOSI	(PD_PA7  | PD_AF)										//!< UEXT con
#define PIN_PA8			(PD_PA8  | PD_IN)
#define PIN_UART1_TX	(PD_PA9  | PD_AF)										//!< UEXT con
#define PIN_UART1_RX	(PD_PA10 | PD_AF)										//!< UEXT con
#define PIN_USBDM		(PD_PA11 | PD_AF)										//!< USB
#define PIN_USBDP		(PD_PA12 | PD_AF)										//!< USB
#define PIN_TMS			(PD_PA13 | PD_AF)										//!< jtag
#define PIN_TCK			(PD_PA14 | PD_AF)										//!< jtag
#define PIN_TDI			(PD_PA15 | PD_AF)										//!< jtag

// Port B
#define PIN_PB0			(PD_PB0  | PD_IN)										//!<
#define PIN_PB1			(PD_PB1  | PD_IN)										//!<
#define PIN_PB2 		(PD_PB2  | PD_IN)										//!<
#define PIN_TDO			(PD_PB3  | PD_AF)										//!< jtag
#define PIN_TSRT		(PD_PB4  | PD_AF)										//!< jtag
#define PIN_PB5			(PD_PB5  | PD_IN)										//!<
#define PIN_I2C1_SCL	(PD_PB6  | PD_AF)										//!< UEXT con
#define PIN_I2C1_SDA	(PD_PB7  | PD_AF) 										//!< UEXT con
#define PIN_CAN_RX		(PD_PB8  | PD_AF)										//!< CAN
#define PIN_CAN_TX		(PD_PB9  | PD_AF)										//!< CAN
#define PIN_PB10		(PD_PB10 | PD_IN)
#define PIN_PB11		(PD_PB11 | PD_IN)
#define PIN_SPI2_NSS	(PD_PB12 | PD_OUT | PD_ACTIVE_LOW)						//!< SD
#define PIN_SPI2_SCK	(PD_PB13 | PD_AF)										//!< SD
#define PIN_SPI2_MISO	(PD_PB14 | PD_AF)										//!< SD
#define PIN_SPI2_MOSI	(PD_PB15 | PD_AF)										//!< SD

// Port C
#define PIN_PC0			(PD_PC0  | PD_IN)										//!<
#define PIN_PC1			(PD_PC1  | PD_IN)										//!<
#define PIN_PC2 		(PD_PC2  | PD_IN)										//!<
#define PIN_PC3			(PD_PC3  | PD_IN)										//!<
#define PIN_USB_P 		(PD_PC4  | PD_IN)										//!< USB 5V
#define PIN_PC5			(PD_PC5  | PD_IN)										//!<
#define PIN_SD_WP		(PD_PC6  | PD_IN)										//!< SD card write protect
#define PIN_SD_CP		(PD_PC7  | PD_IN)						 				//!< SD card detect
#define PIN_PC8			(PD_PC8  | PD_IN)                                       //!<
#define PIN_PC9			(PD_PC9  | PD_IN)                                       //!<
#define PIN_CAN_CNTRL	(PD_PC10 | PD_IN)                                       //!<
#define PIN_USB_PUP		(PD_PC11 | PD_OUT | PD_ACTIVE_LOW)                      //!< USB pullup disable
#define PIN_LED 		(PD_PC12 | PD_OUT | PD_ACTIVE_LOW)                      //!< red LED
#define PIN_PC13		(PD_PC13 | PD_IN)                                       //!<
#define PIN_PC14		(PD_PC14 | PD_IN)                                       //!<
#define PIN_PC15		(PD_PC15 | PD_IN)                                       //!<

// Port D
#define PIN_OSC_IN		(PD_PD0 | PD_AF)										//!< 8MHz Osc
#define PIN_OSC_OUT		(PD_PD1 | PD_AF)										//!< 8MHz Osc
#define PIN_PD2 		(PD_PD2 | PD_IN)										//!<



extern   signed char const DRV_RESET_FIRST_TABLE[];
extern "C" char* const DRV_TABLE[INALID_DRV_INDX+1];

#endif
