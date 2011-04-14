#ifndef CAN_DRV_H_
#define CAN_DRV_H_


#include <can_lm3s.h>

struct CAN_MODE_STRU
{
};

struct CAN_DRIVER_DATA
{
    HANDLE			waiting;	//!< Receive queue
    HANDLE			helper;		//!< Helper task's handle
    unsigned char	cnt;		//!< number of open handles
};

struct CAN_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	CAN_Type *			hw_base;		//!< pointer to the hardware peripheral
	CAN_DRIVER_DATA *	drv_data;		//!< pointer to the driver data
	GPIO_STRU 			CAN_pins;
};
/** CAN Driver Info */
typedef const CAN_DRIVER_INFO* CAN_INFO;

void CAN_DCR(CAN_INFO drv_info, unsigned int reason, HANDLE param);
void CAN_DSR(CAN_INFO drv_info, HANDLE hnd);
void CAN_ISR(CAN_INFO drv_info );


#endif /* CAN_DRV_H_ */
