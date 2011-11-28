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

//{@ port value
#define	PORT_A			0	//!< port A
#define	PORT_B			1	//!< port B
#define	PORT_C			2	//!< port C
//@}

//{@ pio mode
#define PIOMODE_PER		0x01	//!< doc!
#define PIOMODE_OER		0x02	//!< output enable
#define PIOMODE_IFER	0x04	//!< doc!
#define PIOMODE_SODR	0x08	//!< doc!
#define PIOMODE_IER		0x10	//!< doc!
#define PIOMODE_MDER	0x20	//!< doc!
#define PIOMODE_PPUER	0x40	//!< doc!
#define PIOMODE_BSR1	0x80	//!< doc!
#define PIOMODE_BSR2	0x100	//!< doc!
//@}

/** GPIO struct **/
struct GPIO_STRU
{
	unsigned int 	mask;	//!< doc!
	unsigned short 	mode;	//!< doc!
	unsigned char	port;	//!< doc!
};
/** PIN Description **/
typedef const GPIO_STRU* PIN_DESC;

void PIO_CfgOutput(PIN_DESC pins);
void PIO_CfgOutput0(PIN_DESC pins);
void PIO_CfgOutput1(PIN_DESC pins);
void PIO_CfgInput(PIN_DESC pins);
void PIO_CfgInputPull(PIN_DESC pins);
void PIO_CfgPeriph(PIN_DESC pins);
void PIO_CfgPeriphA(PIN_DESC pins);
void PIO_CfgPeriphB(PIN_DESC pins);
void PIO_CfgPeriphC(PIN_DESC pins);
void PIO_CfgPeriphD(PIN_DESC pins);
void PIO_CfgOD(PIN_DESC pins);
void PIO_SetOutput(PIN_DESC pins);
void PIO_ClrOutput(PIN_DESC pins);
unsigned int PIO_Read(PIN_DESC pins);

bool pio_open(HANDLE hnd, PIN_DESC pins);

/** PIO Driver Data structure **/
struct PIO_DRIVER_DATA_STRU
{
    unsigned char	cnt;		//!< number of open handles
    HANDLE			waiting;	//!< Receive queue
};
/** PIO DRIVER DATA pointer type */
typedef PIO_DRIVER_DATA_STRU* PIO_DRIVER_DATA;

/** PIO Driver Info structure **/
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
