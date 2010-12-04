//////////////////////////////////////////////////////////////////////////
//
//			Drivers
//
//
//////////////////////////////////////////////////////////////////////////

#include "drivers.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		 DEFAULT DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void dcr_DefaultDriver(DRIVER_INFO_Type* driver, unsigned int reason, HANDLE hnd)
{
}

void dsr_DefaultDriver(DRIVER_INFO_Type* driver, HANDLE hnd)
{
}
void isr_DefaultDriver(DRIVER_INFO_Type* driver )
{
	while(1);
}

const DRIVER_INFO_Type DefaultDriver =
{
		DRIVER_INFO_STUB,
		(DRV_ISR)isr_DefaultDriver,
		(DRV_DCR)dcr_DefaultDriver,
		(DRV_DSR)dsr_DefaultDriver,
		INALID_DRV_INDX
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 SYSTICK DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const ClockDriver systick_driver =
{
		{
			DRIVER_INFO_STUB,
			(DRV_ISR)isr_ClockDriver,
			(DRV_DCR)dcr_ClockDriver,
			(DRV_DSR)dsr_ClockDriver,
			SysTick_IRQn,
			DRV_PRIORITY_KERNEL,
			SYSCTL_NO_PERIPH
		}
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
#if HW_VER_10
		{U0_RTS,U0_CTS,U0_RX,U0_TX,0}, // pin RTS, pin CTS, pin Rx , pin Tx
#endif
#if HW_VER_11
		{0,0,U0_RX,U0_TX,0}, // pin RTS, pin CTS, pin Rx , pin Tx
#endif
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
		{U1_RTS,U1_CTS,U1_RX,U1_TX,0}, // pin RTS, pin CTS, pin Rx , pin Tx
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
#ifdef HW_VER_10
		{U2_RTS,U2_CTS,U2_RX,U2_TX,0}, // pin RTS, pin CTS, pin Rx , pin Tx
#endif
#ifdef HW_VER_11
		{0,0,U2_RX,U2_TX,0}, // pin RTS, pin CTS, pin Rx , pin Tx
#endif
		&uart_2_vars
};

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

const GPIO_DRIVER_INFO * const GPIO_DRVS[9] = {
		&gpio_a, &gpio_b, &gpio_c, &gpio_d, &gpio_e, &gpio_f, &gpio_g, &gpio_h, &gpio_j
};

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




signed char const DRV_RESET_FIRST_TABLE[10] =
{
	SysTick_IRQn,
	GPIOPortA_IRQn, GPIOPortB_IRQn, GPIOPortC_IRQn, GPIOPortD_IRQn, GPIOPortE_IRQn,
	GPIOPortF_IRQn, GPIOPortG_IRQn, GPIOPortH_IRQn,
	GPIOPortJ_IRQn
};

//=================== DRV_TABLE ==========================================
// All drivers in the system must be listed in this table
// To open a driver, only the driver index is required (defined in drivers.h)
// The driver index must match with the driver position in this table
char * const DRV_TABLE[INALID_DRV_INDX+1] __attribute__ ((section (".ExceptionVectors")))  =
{
	1+ (char * const)&gpio_a.info, // 0
	1+ (char * const)&gpio_b.info, // 1
	1+ (char * const)&gpio_c.info, // 2
	1+ (char * const)&gpio_d.info, // 3
	1+ (char * const)&gpio_e.info, // 4
	1+ (char * const)&uart_0.info,	 // 5
	1+ (char * const)&DefaultDriver,	 // 6
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
#if defined(LM3S9B90_PATCH) || defined(LM3S5B91_PATCH)
	1+ (char * const)&FlashDriver, // 29
#endif

#if	defined(LM3S9B90) || defined(LM3S5B91)
	1+ (char * const)&DefaultDriver, // 29
#endif
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
	1+ (char * const)&DefaultDriver, // 44
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
	1+ (char * const)&DefaultDriver,	 // 56
	1+ (char * const)&DefaultDriver,	 // 57
	1+ (char * const)&DefaultDriver,	 // 58
   NULL				//null terminated list
};





