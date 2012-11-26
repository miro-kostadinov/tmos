//////////////////////////////////////////////////////////////////////////
//
//			Drivers
//
//
//////////////////////////////////////////////////////////////////////////
#include <tmos.h>
#include <drivers.h>
#include <systick_drv.h>
#include <usb_drv.h>
#include <usb_setup.h>

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
		SYSCTL_NO_PERIPH
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
			SYSCTL_NO_PERIPH
		},
		1									//!< OS_QUANTUM_PERIOD = 1 ms
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART 0 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

UART_DRIVER_DATA uart_0_vars;
const UART_DRIVER_INFO uart_0 =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_SerilaDriver,
			(DRV_DCR)dcr_SerialDriver,
			(DRV_DSR)dsr_SerialDriver,
			UART0_IRQn,
			DRV_PRIORITY_UART0,
			SYSCTL_PERIPH_UART0
		},
		UART0,
		{U0_RX, U0_TX, 0, 0, 0},           // pin Rx , pin Tx, pin RTS, pin CTS
		&uart_0_vars
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART 1 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UART_DRIVER_DATA uart_1_vars;
const UART_DRIVER_INFO uart_1 =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_SerilaDriver,
			(DRV_DCR)dcr_SerialDriver,
			(DRV_DSR)dsr_SerialDriver,
			UART1_IRQn,
			DRV_PRIORITY_UART1,
			SYSCTL_PERIPH_UART1
		},
		UART1,
		{U1_RX, U1_TX, U1_RTS, U1_CTS, 0}, // pin Rx , pin Tx, pin RTS, pin CTS
		&uart_1_vars
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 UART 2 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UART_DRIVER_DATA uart_2_vars;
const UART_DRIVER_INFO uart_2 =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_SerilaDriver,
			(DRV_DCR)dcr_SerialDriver,
			(DRV_DSR)dsr_SerialDriver,
			UART2_IRQn,
			DRV_PRIORITY_UART2,
			SYSCTL_PERIPH_UART2
		},
		UART2,
		{U2_RX, U2_TX, 0, 0, 0},           // pin Rx , pin Tx, pin RTS, pin CTS
		&uart_2_vars
};

extern "C" const UART_DRIVER_MODE uart_default_mode = {115200, //19200,  //19200, /*9600,*/
									UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE,
									1/*UART_RX_TIMEOUT*/, 0/*UART_HW_FLOWCTRL*/};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
GPIO_DRIVER_DATA gpio_data;
const GPIO_DRIVER_INFO gpio_a =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortA_IRQn,
			DRV_PRIORITY_GPIOA,
			SYSCTL_PERIPH_GPIOA
		},
		GPIOA,
		0,
		&gpio_data
};

const GPIO_DRIVER_INFO gpio_b =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortB_IRQn,
			DRV_PRIORITY_GPIOB,
			SYSCTL_PERIPH_GPIOB
		},
		GPIOB,
		1,
		&gpio_data
};

const GPIO_DRIVER_INFO gpio_c =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortC_IRQn,
			DRV_PRIORITY_GPIOC,
			SYSCTL_PERIPH_GPIOC
		},
		GPIOC,
		2,
		&gpio_data
};

const GPIO_DRIVER_INFO gpio_d =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortD_IRQn,
			DRV_PRIORITY_GPIOD,
			SYSCTL_PERIPH_GPIOD
		},
		GPIOD,
		3,
		&gpio_data
};

const GPIO_DRIVER_INFO gpio_e =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortE_IRQn,
			DRV_PRIORITY_GPIOE,
			SYSCTL_PERIPH_GPIOE
		},
		GPIOE,
		4,
		&gpio_data
};

const GPIO_DRIVER_INFO gpio_f =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortF_IRQn,
			DRV_PRIORITY_GPIOF,
			SYSCTL_PERIPH_GPIOF
		},
		GPIOF,
		5,
		&gpio_data
};

const GPIO_DRIVER_INFO gpio_g =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortG_IRQn,
			DRV_PRIORITY_GPIOG,
			SYSCTL_PERIPH_GPIOG
		},
		GPIOG,
		6,
		&gpio_data
};

const GPIO_DRIVER_INFO gpio_h =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortH_IRQn,
			DRV_PRIORITY_GPIOH,
			SYSCTL_PERIPH_GPIOH
		},
		GPIOH,
		7,
		&gpio_data
};

const GPIO_DRIVER_INFO gpio_j =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_GPIO_driver,
			(DRV_DCR)dcr_GPIO_driver,
			(DRV_DSR)dsr_GPIO_driver,
			GPIOPortJ_IRQn,
			DRV_PRIORITY_GPIOJ,
			SYSCTL_PERIPH_GPIOJ
		},
		GPIOJ,
		8,
		&gpio_data
};

// Virtual GPIO Port functions

#if ( USE_GPIO_EXPANDER !=	0)

const SPI_DRIVER_MODE sreg_mode_stru =
{
	IO_CS,
    SSI_CR0_SPH | SSI_CR0_SPO | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8 | (2 << 8)		// clock rate
//    SPI_CLOCK_DIV(2) |	SPI_MODE_1		//	0x00000200
};

extern "C" void VPIO_Write(pio_def pio, unsigned int val )
{
	CHandle sreg;

	sreg.tsk_safe_open(SSI0_IRQn, &sreg_mode_stru);
	pio = PD_PINS_Get(pio);
	gpio_data.sreg_val = (gpio_data.sreg_val & ~pio) | (val & pio);
	sreg.tsk_write(&gpio_data.sreg_val, 1 );
}

extern "C" pio_set VPIO_Read(pio_def pio)
{
	return (PD_PINS_Get(pio) & gpio_data.sreg_val);
}

#endif



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPIO DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SPI_DRIVER_DATA spi0_drv_data;
const SPI_DRIVER_INFO spi0_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_SPI_driver,
			(DRV_DCR)dcr_SPI_driver,
			(DRV_DSR)dsr_SPI_driver,
			SSI0_IRQn,
			DRV_PRIORITY_SPI0,
			SYSCTL_PERIPH_SSI0
		},
		SSI0,
		&spi0_drv_data,
		{
			SSI0_CLK,
			SSI0_RX,
			SSI0_TX,
			0
		}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SPI1 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SPI_DRIVER_DATA spi1_drv_data;
const SPI_DRIVER_INFO spi1_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_SPI_driver,
			(DRV_DCR)dcr_SPI_driver,
			(DRV_DSR)dsr_SPI_driver,
			SSI1_IRQn,
			DRV_PRIORITY_SPI0,
			SYSCTL_PERIPH_SSI1
		},
		SSI1,
		&spi1_drv_data,
		{
			SSI1_CLK,
			SSI1_RX,
			SSI1_TX,
			0
		}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const unsigned int KEY_RD_PINS[]=
{
	KEY_RD0 ,
	KEY_RD1 ,
	KEY_RD2 ,
	KEY_RD3 ,
	KEY_RD4 ,
	KEY_RD5 ,
	0
};

//const unsigned char key_codes[] = "D654NCFKU3210987O";
const unsigned char key_codes[] = "ODUNKC";

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 GUI DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const PIN_DESC lcd1pins[] =
{
	BACKLIGHT_PIN,				// BKLT_PIN_INDX
	LCD1_RST_PIN,				// RST_PIN_INDX
	LCD1_CS,					// CSX_PIN_INDX
	PD_PA2 | PD_OUT,			// SCL_PIN_INDX
	PD_PA5 | PD_OUT | PD_PULL_UP //SDA_PIN_INDX
};

const PIN_DESC lcd2pins[] =
{
	BACKLIGHT_PIN,					// BKLT_PIN_INDX
	LCD2_RST_PIN,					// RST_PIN_INDX
	LCD2_CS,						// CSX_PIN_INDX
	PD_PA2 | PD_OUT,				// SCL_PIN_INDX
	PD_PA5 | PD_OUT | PD_PULL_UP 	//SDA_PIN_INDX
};

const SPI_DRIVER_MODE lcd1_mode_stru =
{
	LCD1_CS,
    SSI_CR0_SPH | SSI_CR0_SPO | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_9 | (1 << 8)		// clock rate
//    SPI_CLOCK_DIV(2) |	SPI_MODE_1		//	0x00000200
};

const SPI_DRIVER_MODE lcd2_mode_stru =
{
	LCD2_CS,
    SSI_CR0_SPH | SSI_CR0_SPO | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_9 | (1 << 8)		// clock rate
//    SPI_CLOCK_DIV(2) |	SPI_MODE_1		//	0x00000200
};

const SPI_DRIVER_MODE lcdx_mode_stru =
{
	0,
    SSI_CR0_SPH | SSI_CR0_SPO | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_9 | (1 << 8)		// clock rate
//    SPI_CLOCK_DIV(2) |	SPI_MODE_1		//	0x00000200
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
			DRV_PRIORITY_KERNEL,
			0
		},
		&gui_drv_data,
		{
				NULL, // lcd_module will be set from AppInit()
				NULL
		}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 USB DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const unsigned int usb_pins[]=
{
	USB_ID,
	USB_VBUS,
	USB_PEN,
	0
};

USB_DRIVER_DATA usb_drv_data;
const USB_DRIVER_INFO usb_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)USB_B_ISR,
			(DRV_DCR)USB_DCR,
			(DRV_DSR)USB_DSR,
			USB0_IRQn,
			DRV_PRIORITY_USB,
			SYSCTL_PERIPH_USB0
		},
		USB0,
		&usb_drv_data,
		&cc500_descriptors
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
			Watchdog_IRQn,
			DRV_PRIORITY_WDT,
			SYSCTL_PERIPH_WDOG1
		},
		WDT1,					//hw_base
		80 * 1000 * 1000,		//load
		50						//sleep
};


extern signed char const DRV_RESET_FIRST_TABLE[] =
{
	SysTick_IRQn,
	GPIOPortA_IRQn, GPIOPortB_IRQn, GPIOPortC_IRQn, GPIOPortD_IRQn, GPIOPortE_IRQn,
	GPIOPortF_IRQn, GPIOPortG_IRQn, GPIOPortH_IRQn,
	GPIOPortJ_IRQn, INALID_DRV_INDX
};

//=================== DRV_TABLE ==========================================
// All drivers in the system must be listed in this table
// To open a driver, only the driver index is required (defined in drivers.h)
// The driver index must match with the driver position in this table
extern "C" char * const DRV_TABLE[INALID_DRV_INDX+1] __attribute__ ((section (".DriverTable")))  =
{
	1+ (char * const)&gpio_a.info, // 0
	1+ (char * const)&gpio_b.info, // 1
	1+ (char * const)&gpio_c.info, // 2
	1+ (char * const)&gpio_d.info, // 3
	1+ (char * const)&gpio_e.info, // 4
	1+ (char * const)&uart_0.info,	 // 5
	1+ (char * const)&uart_1,		 // 6
	1+ (char * const)&spi0_driver,	 // 7
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
	1+ (char * const)&DefaultDriver, // 18 wdt_driver
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
	1+ (char * const)&gpio_f.info, // 30
	1+ (char * const)&gpio_g.info, // 31
	1+ (char * const)&gpio_h.info, // 32
	1+ (char * const)&uart_2.info, // 33
	1+ (char * const)&spi1_driver, // 34
	1+ (char * const)&DefaultDriver, // 35
	1+ (char * const)&DefaultDriver, // 36
	1+ (char * const)&DefaultDriver, // 37
	1+ (char * const)&DefaultDriver, // 38
	1+ (char * const)&DefaultDriver, // 39
	1+ (char * const)&DefaultDriver, // 40
	1+ (char * const)&DefaultDriver, // 41
	1+ (char * const)&DefaultDriver, // 42
	1+ (char * const)&DefaultDriver, // 43
	1+ (char * const)&usb_driver, 	 // 44
	1+ (char * const)&DefaultDriver, // 45
	1+ (char * const)&DefaultDriver, // 46
	1+ (char * const)&DefaultDriver, // 47
	1+ (char * const)&DefaultDriver, // 48
	1+ (char * const)&DefaultDriver, // 49
	1+ (char * const)&DefaultDriver, // 50
	1+ (char * const)&DefaultDriver, // 51
	1+ (char * const)&DefaultDriver, // 52
	1+ (char * const)&DefaultDriver, // 53
	1+ (char * const)&gpio_j.info, 	 // 54
	1+ (char * const)&DefaultDriver,	 // 55
	1+ (char * const)&gui_driver,	 // 56
	1+ (char * const)&DefaultDriver,	 // 57
	1+ (char * const)&DefaultDriver, // 58
	1+ (char * const)&DefaultDriver,	 // 61
   NULL				//null terminated list
};





