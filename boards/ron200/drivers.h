#ifndef	DRIVERS_H
#define	DRIVERS_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 RON200 drivers...
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <tmos.h>
#include <cmsis_cpp.h>
#include <hardware_cpp.h>


//----- Driver Priorities ----------------//
#define DRV_PRIORITY_KERNEL		0x01	// OS-aware ISRs  must have lower priority (higher number)
#define DRV_PRIORITY_UART0		0x02
#define DRV_PRIORITY_UART1		0x02
#define DRV_PRIORITY_UART2		0x02
#define DRV_PRIORITY_SPI0		0x02






extern   signed char const DRV_RESET_FIRST_TABLE[1];
extern 	 char* const DRV_TABLE[INALID_DRV_INDX+1];
#endif
