#ifndef	DRIVERS_H
#define	DRIVERS_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 target header files...
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <tmos.h>
#include <hardware_cpp.h>

//#include <mcu.h>
#include <handle.h>


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
// 		 UART DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <uart_driver.h>

#define UART_TEST_DRIVER UART2_IRQn
extern "C" const UART_DRIVER_MODE uart_default_mode;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <gpio_drv.h>




// Port A
#define U0_RX			(PD_PA0_U0RX    | PD_UART)								//!< UART FU (X8)
#define U0_TX			(PD_PA1_U0TX    | PD_UART)								//!< UART FU (X8)
#define SSI0_CLK		(PD_PA2_SSI0CLK | PD_PERIPHERAL_8MA_SC | PD_PULL_UP)	//!< SPI top (X10), ext SD card (X14)
#define SSI0_FSS		(PD_PA3         | PD_OUT)								//!< ext SD card (X14)
#define SSI0_RX			(PD_PA4_SSI0RX  | PD_PERIPHERAL_8MA_SC | PD_PULL_UP)	//!< ext SD card (X14)
#define SSI0_TX			(PD_PA5_SSI0TX  | PD_PERIPHERAL_8MA_SC | PD_PULL_UP)	//!< SPI top (X10), ext SD card (X14)

// Port B
#define USB_ID			(PD_PB0 | PD_USB_ANALOG)								//!< USB
#define USB_VBUS		(PD_PB1 | PD_USB_ANALOG)								//!< USB
#define LCD_A0_PIN		(PD_PB2 | PD_OUT)										//!< top (x10)
#define LCD1_CS			(PD_PB3 | PD_OUT)										//!< SPI top (X10)
#define PFI_PIN			(PD_PB4 | PD_ADC)										//!< Vin
#define IO_CS			(PD_PB5 | PD_OUT | PD_ACTIVE_LOW)						//!< SPI top (X10)
#define ON_3V3			(PD_PB6 | PD_OUT)										//!< keep 3v3 ON
#define UART_ON			(PD_PB7 | PD_IN | PD_PULL_DOWN | PD_LOCK) // NMI pin	//!< detect uarts or wake key

// Port C
#define U1_RX			(PD_PC6_U1RX | PD_UART | PD_PULL_UP)					//!< UART gsm
#define U1_RX_OFF		(PD_PC6      | PD_IN)									//!< UART gsm
#define U1_TX			(PD_PC7_U1TX | PD_UART)									//!< UART gsm
#define U1_TX_OFF		(PD_PC7      | PD_IN)									//!< UART gsm

// Port D
#define U2_RX			(PD_PD0_U2RX | PD_UART)									//!< UART PC (X9)
#define U2_TX			(PD_PD1_U2TX | PD_UART)									//!< UART PC (X9)
#define GPIO0			(PD_PD4 | PD_IN)										//!< jumper not placed
#define GPIO3			(PD_PD7 | PD_IN)										//!< jumper not placed

// Port E
#define KEY_RD0			(PD_PE0 | PD_IN | PD_PULL_UP | PD_BOTH_EDGES)			//!< key
#define KEY_RD1			(PD_PE1 | PD_IN | PD_PULL_UP | PD_BOTH_EDGES)			//!< key
#define KEY_RD2			(PD_PE2 | PD_IN | PD_PULL_UP | PD_BOTH_EDGES)			//!< key
#define KEY_RD3			(PD_PE3 | PD_IN | PD_PULL_UP | PD_BOTH_EDGES)			//!< key
#define KEY_RD4			(PD_PE4 | PD_IN | PD_PULL_UP | PD_BOTH_EDGES)			//!< key
#define KEY_RD5			(PD_PE5 | PD_IN | PD_PULL_UP | PD_BOTH_EDGES)			//!< key
#define U1_CTS			(PD_PE6_U1CTS | PD_UART)								//!< U1 cts - gsm RTS
#define U1_CTS_OFF		(PD_PE6 | PD_IN)
#define CHARGE_STAT		(PD_PE7 | PD_IN)										//!< bat charge status

// Port F
#define GSM_ONOFF_PIN	(PD_PF0 | PD_OUT | PD_ACTIVE_HIGH)										//!< GSM on mosfet
#define U1_RTS			(PD_PF1_U1RTS | PD_UART)								//!< U1 rts - gsm CTS
#define U1_RTS_OFF		(PD_PF1 | PD_IN)

// Port G
#define LCD2_CS			(PD_PG7 | PD_OUT)	// External LCD

// Port H
#define UART_INVALID	(PD_PH1 | PD_IN | PD_BOTH_EDGES)//PD_RISING_EDGE)										//!< uarts (0,2) invalid
#define USB_PEN			(PD_PH3_USB0EPEN | PD_PERIPHERAL)						//!< USB
#define SSI1_CLK		(PD_PH4_SSI1CLK  | PD_PERIPHERAL_8MA_SC | PD_PULL_UP)								//!< SPI int SD card (X11), data flash
#define SSI1_FSS		(PD_PH5 | PD_OUT)										//!< SPI data flash
#define SSI1_RX			(PD_PH6_SSI1RX   | PD_PERIPHERAL_8MA_SC | PD_PULL_UP)								//!< SPI int SD card (X11), data flash
#define SSI1_TX			(PD_PH7_SSI1TX   | PD_PERIPHERAL_8MA_SC | PD_PULL_UP)								//!< SPI int SD card (X11), data flash

// Port J
#define GSM_RESET_PIN	(PD_PJ0 | PD_OD_8MA | PD_ACTIVE_LOW)					//!< gsm reset
#define GPIO5			(PD_PJ1 | PD_IN)										//!< gpio
#define GSM_STATUS_PIN	(PD_PJ1 | PD_IN)										//!< gpio

// Port V SHIFT REGISTER
#define	LCD2_RST_PIN	PD_PV1   // External LCD
#define BACKLIGHT_PIN	PD_PV2
#define	LCD1_RST_PIN	PD_PV3
#define LED0			PD_PV4	//cc100 battery
#define LED1			PD_PV5  //g_fatal_error
#define LED2			PD_PV6	//gsm
#define LED3			PD_PV7

#define PIN_LED		LED0	// for LED test
#define PIN_BUTTON	KEY_RD0
#define GPIO_IRQn GPIOPortA_IRQn		//!< Use this to open handles

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPI DRIVERS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <spi_drv.h>
extern const SPI_DRIVER_INFO spi0_driver;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern const unsigned char key_codes[];
extern const unsigned int KEY_RD_PINS[];


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GUI DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <gui_drv.h>
extern const PIN_DESC lcd1pins[];
extern const PIN_DESC lcd2pins[];
extern const SPI_DRIVER_MODE lcd1_mode_stru;
extern const SPI_DRIVER_MODE lcd2_mode_stru;
extern const SPI_DRIVER_MODE lcdx_mode_stru;
extern GUI_DRIVER_INFO gui_driver;


extern const unsigned int usb_pins[];

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 WDT DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <wdt_drv.h>

extern const WDT_DRIVER_INFO wdt_driver;

extern "C" char* const DRV_TABLE[INALID_DRV_INDX+1];

#define USB_TEST_DRIVER USB0_IRQn
#define TEST_USB 1

#endif
