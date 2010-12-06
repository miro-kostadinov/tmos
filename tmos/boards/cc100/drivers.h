#ifndef	DRIVERS_H
#define	DRIVERS_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 CC100 drivers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <tmos.h>
#include <cmsis_cpp.h>
#include <hardware_cpp.h>

#define SOFTWARE_VERSION 			0x0210
#define SOFTWARE_VERSION_STRING 	"2.10"

#define PLATFORM_MAIN			0x01

#define CURRENT_PLATFORM		PLATFORM_MAIN

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


//----- Driver Control Codes ----------------//
#define DCR_STOP			0x40


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SYSTICK DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <clock_drv.h>
#define OS_QUANTUM_PERIOD 1 // 1ms

extern const ClockDriver systick_driver;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <uart_driver.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <gpio_drv.h>



#if	HW_VER_11

// Port A
#define U0_RX			(PD_PIN_PA0_U0RX|PD_UART_PIN) //PD_PAD_TYPE_STD|PD_DIR_HW)
#define U0_TX			(PD_PIN_PA1_U0TX|PD_UART_PIN) //PD_PAD_TYPE_STD|PD_DIR_HW)
#define SSI0_CLK		(PD_PIN_PA2_SSI0CLK|PD_SSI_PIN)
#define SSI0_FSS		(PD_PIN_PA3_SSI0FSS|PD_SSI_PIN)
#define SSI0_RX			(PD_PIN_PA4_SSI0RX|PD_SSI_PIN)
#define SSI0_TX			(PD_PIN_PA5_SSI0TX|PD_SSI_PIN)
//#define U0_CTS			(PD_PIN_PA6|PD_IN_PIN|PD_INT_BOTH_EDGES|PD_PIN_INT_ENABLE)
//#define U0_RTS			(PD_PIN_PA7|PD_OUT_PIN)
// Port B
#define USB_ID			(PD_PIN_PB0|PD_USB_ANALOG)
#define USB_VBUS		(PD_PIN_PB1|PD_USB_ANALOG)
#define LCD_A0_PIN		(PD_PIN_PB2|PD_OUT_PIN)
#define LCD_CS			(PD_PIN_PB3|PD_OUT_PIN)
#define PFI_PIN			(PD_PIN_PB4|PD_ADC_PIN)
#define IO_CS			(PD_PIN_PB5|PD_OUT_PIN)
#define ON_3V3			(PD_PIN_PB6|PD_OUT_PIN)
#define UART_ON			(PD_PIN_PB7|PD_IN_PIN)
//gap
//#define BACKLIGHT_PIN		(PD_PIN_PB5|PD_OUT_PIN)
//#define LED0			(PD_PIN_PB6|PD_PAD_STRENGTH_8MA|PD_PAD_TYPE_STD|PD_DIR_OUT)
//#define LED1			(PD_PIN_PB7|PD_PAD_STRENGTH_8MA|PD_PAD_TYPE_STD|PD_DIR_OUT|PD_PIN_UNLOCK) // NMI pin
// Port C
#define U1_RX			(PD_PIN_PC6_U1RX|PD_UART_PIN) //PD_PAD_TYPE_STD|PD_DIR_HW)
#define U1_RX_OFF		(PD_PIN_PC6|PD_IN_PIN)
#define U1_TX			(PD_PIN_PC7_U1TX|PD_UART_PIN) //PD_PAD_TYPE_STD|PD_DIR_HW)
#define U1_TX_OFF		(PD_PIN_PC7|PD_IN_PIN)
// Port D
#define U2_RX			(PD_PIN_PD0_U2RX|PD_UART_PIN) //PD_PAD_TYPE_STD|PD_DIR_HW)
#define U2_TX			(PD_PIN_PD1_U2TX|PD_UART_PIN) //PD_PAD_TYPE_STD|PD_DIR_HW)
//#define U2_CTS			(PD_PIN_PD2|PD_IN_PIN|PD_INT_BOTH_EDGES|PD_PIN_INT_ENABLE)
//#define U2_RTS			(PD_PIN_PD3|PD_OUT_PIN)
#define GPIO0			(PD_PIN_PD4|PD_IN_PIN)  // ??
#define GPIO1			(PD_PIN_PD5|PD_IN_PIN)
#define GPIO2			(PD_PIN_PD6|PD_IN_PIN)
#define GPIO3			(PD_PIN_PD7|PD_IN_PIN)
// Port E
#define KEY_RD0			(PD_PIN_PE0|PD_PAD_STRENGTH_2MA | PD_PAD_TYPE_STD_WPU  | PD_DIR_IN | PD_INT_BOTH_EDGES)
#define KEY_RD1			(PD_PIN_PE1|PD_PAD_STRENGTH_2MA | PD_PAD_TYPE_STD_WPU  | PD_DIR_IN | PD_INT_BOTH_EDGES)
#define KEY_RD2			(PD_PIN_PE2|PD_PAD_STRENGTH_2MA | PD_PAD_TYPE_STD_WPU  | PD_DIR_IN | PD_INT_BOTH_EDGES)
#define KEY_RD3			(PD_PIN_PE3|PD_PAD_STRENGTH_2MA | PD_PAD_TYPE_STD_WPU  | PD_DIR_IN | PD_INT_BOTH_EDGES)
#define KEY_RD4			(PD_PIN_PE4|PD_PAD_STRENGTH_2MA | PD_PAD_TYPE_STD_WPU  | PD_DIR_IN | PD_INT_BOTH_EDGES)
#define KEY_RD5			(PD_PIN_PE5|PD_PAD_STRENGTH_2MA | PD_PAD_TYPE_STD_WPU  | PD_DIR_IN | PD_INT_BOTH_EDGES)
#define U1_CTS			(PD_PIN_PE6_U1CTS|PD_UART_PIN) //PD_PAD_TYPE_STD|PD_DIR_HW)
#define U1_CTS_OFF		(PD_PIN_PE6|PD_IN_PIN)
#define CHARGE_STAT		(PD_PIN_PE7|PD_DIR_IN|PD_PAD_STRENGTH_2MA)
// Port F
#define GSM_ONOFF_PIN	(PD_PIN_PF0|PD_OUT_PIN)
#define U1_RTS			(PD_PIN_PF1_U1RTS|PD_UART_PIN) //PD_PAD_TYPE_STD|PD_DIR_HW)
#define U1_RTS_OFF		(PD_PIN_PF1|PD_IN_PIN)
//#define ETH_LED1		(PD_PIN_PF2_LED1|PD_PAD_STRENGTH_8MA|PD_PAD_TYPE_STD|PD_DIR_HW)
//#define ETH_LED0		(PD_PIN_PF3_LED0|PD_PAD_STRENGTH_8MA|PD_PAD_TYPE_STD|PD_DIR_HW)
//#if defined(LM3S9B90) || defined(LM3S9B90_PATCH)
//#define LED2			(PD_PIN_PF4|PD_PAD_STRENGTH_8MA|PD_PAD_TYPE_STD|PD_DIR_OUT)
//#define LED3			(PD_PIN_PF5|PD_PAD_STRENGTH_8MA|PD_PAD_TYPE_STD|PD_DIR_OUT)
//#endif
//#if defined(LM3S5B91) || defined(LM3S5B91_PATCH)
//#define LED2			(PD_PIN_PF7|PD_PAD_STRENGTH_8MA|PD_PAD_TYPE_STD|PD_DIR_OUT)
//#define LED3			(PD_PIN_PG4|PD_PAD_STRENGTH_8MA|PD_PAD_TYPE_STD|PD_DIR_OUT)
//#endif
// Port G
//#define KEY_DRV0		(PD_PIN_PG0|PD_OUT_PIN)
//#define KEY_DRV1		(PD_PIN_PG1|PD_OUT_PIN)
//#define KEY_DRV7		(PD_PIN_PG7|PD_OUT_PIN)
// Port H
#define USB_PEN			PD_PIN_PH3_USB0EPEN
#define SSI1_CLK		(PD_PIN_PH4_SSI1CLK |PD_SSI_PIN)
#define SSI1_FSS		(PD_PIN_PH5_SSI1FSS |PD_OUT_PIN)
#define SSI1_RX			(PD_PIN_PH6_SSI1RX |PD_SSI_PIN)
#define SSI1_TX			(PD_PIN_PH7_SSI1TX |PD_SSI_PIN)
// Port J
#define GPIO4			(PD_PIN_PJ0|PD_IN_PIN)
#define GPIO5			(PD_PIN_PJ1|PD_IN_PIN)
// Port V SHIFT REGISTER
#define BACKLIGHT_PIN	PD_PIN_PV2
#define	LCD_RST_PIN		PD_PIN_PV3
#define LED0			PD_PIN_PV4
#define LED1			PD_PIN_PV5
#define LED2			PD_PIN_PV6
#define LED3			PD_PIN_PV7

#endif

extern   const GPIO_DRIVER_INFO * const GPIO_DRVS[9];
extern   GPIO_DRIVER_DATA gpio_data;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPI DRIVERS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <spi_drv.h>



extern   signed char const DRV_RESET_FIRST_TABLE[10];
extern 	 char* const DRV_TABLE[INALID_DRV_INDX+1];
#endif
