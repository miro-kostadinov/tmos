/**
 * @ingroup	 drv_at91
 * @defgroup DRV_AT91_PIO Parallel Input/Output Controller Driver
 * Interface for Parallel Input/Output Controller (PIO) driver.
 * @{
 * The GPIOs lines can be accessed with or without driver. The PIO driver is
 * useful when a task has to scan for IO line changes.
 * No interrupt interception is required. A locked read blocks the transaction
 * until IO line level change occurs.
 *
 *  *
 * HANDLE update:
 * len - decremented with the transfered bytes
 * src/dst - incremented with the transfered bytes
 *
 *
 * @file     drivers/at91/pio_drv.h
 * @ingroup	 DRV_AT91_PIO
 * @brief    PIO driver header
 *
 * @date     09. December 2010
 * @author	 Miroslav Kostadinov
 * @}
*/


#ifndef PIO_DRV_H_
#define PIO_DRV_H_

#include <tmos.h>
#include <pio.h>

// port value
#define	PORT_A			0
#define	PORT_B			1
#define	PORT_C			2

// pio mode
#define PIOMODE_PER		0x01
#define PIOMODE_OER		0x02
#define PIOMODE_IFER	0x04
#define PIOMODE_SODR	0x08
#define PIOMODE_IER		0x10
#define PIOMODE_MDER	0x20
#define PIOMODE_PPUER	0x40
#define PIOMODE_BSR		0x80

struct GPIO_STRU
{
	unsigned int 	mask;
	unsigned short 	mode;
	unsigned char	port;
};
typedef const GPIO_STRU* PIN_DESC;

void PIO_CfgOutput(PIN_DESC pins);
void PIO_CfgOutput0(PIN_DESC pins);
void PIO_CfgOutput1(PIN_DESC pins);
void PIO_CfgInput(PIN_DESC pins);
void PIO_CfgInputPull(PIN_DESC pins);
void PIO_CfgPeriph(PIN_DESC pins);
void PIO_CfgPeriphB(PIN_DESC pins);
void PIO_CfgOD(PIN_DESC pins);
void PIO_SetOutput(PIN_DESC pins);
void PIO_ClrOutput(PIN_DESC pins);
unsigned int PIO_Read(PIN_DESC pins);



struct PIO_DRIVER_DATA_STRU
{
    unsigned char	cnt;		//!< number of open handles
    HANDLE			waiting;	//!< Receive queue
};
/** PIO DRIVER DATA pointer type */
typedef PIO_DRIVER_DATA_STRU* PIO_DRIVER_DATA;

struct PIO_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Pio *				hw_base;		//!< pointer to the hardware peripheral
	PIO_DRIVER_DATA 	drv_data;		//!< pointer to the driver data
};
/** PIO Driver Info */
typedef const PIO_DRIVER_INFO* PIO_INFO;

void PIO_DCR(PIO_INFO drv_info, unsigned int reason, HANDLE param);
void PIO_DSR(PIO_INFO drv_info, HANDLE hnd);
void PIO_ISR(PIO_INFO drv_info );


#endif /* PIO_DRV_H_ */
