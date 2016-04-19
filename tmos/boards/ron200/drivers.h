#ifndef	DRIVERS_H
#define	DRIVERS_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 RON200 drivers...
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <tmos.h>
#include <hardware_cpp.h>


//----- Driver Priorities ----------------//
#define DRV_PRIORITY_KERNEL		NVIC_PIORITY(0x01)	// OS-aware ISRs  must have lower priority (higher number)
#define DRV_PRIORITY_UART0		NVIC_PIORITY(0x02)
#define DRV_PRIORITY_UART1		NVIC_PIORITY(0x02)
#define DRV_PRIORITY_UART2		NVIC_PIORITY(0x02)
#define DRV_PRIORITY_SPI0		NVIC_PIORITY(0x02)



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define KEY_POLLING_TIME	10
#define KEY_DEBOUNCE_TIME 	50 //30 //300
#define KEY_REPEAT_TIME 	300//150 //1000
#define KEY_COUNT		3


extern 	 char* const DRV_TABLE[INALID_DRV_INDX+1];
#endif
