/**************************************************************************//**
 * @ingroup	 ports
 * @defgroup ports_cortex Cortex M3 port
 * @{
 * @file     cortex-m3/port_cpp.h
 * @brief    Cortex M3 kernel CPP header
 * @version  V3.00
 * @date     08. November 2010
 * @author	 Miroslav Kostadinov
 *
 * @}
 *
 ******************************************************************************/

#ifndef PORT_CPP_H_
#define PORT_CPP_H_

#if USE_TMOS_STDLIB
#include <tmos_stdlib.h>
#endif

#if USE_TMOS_STRING
#include <tmos_string.h>
#endif

#include <tasks.h>
#include <swi_tab.h>
#include <tmos_swi.h>
#include <services.h>
#include <tmos_stdio.h>

#ifdef __cplusplus

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//          NOTE: Drivers are supported only in C++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <handle.h>



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 DRIVER INFO
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * Pointer type to driver info structure
 */
typedef const struct DRIVER_INFO_Type * DRIVER_INFO;

/**
 * Driver DCR() function type
 *
 * DCR() is called from the kernel with a reason code
 *
 * @param drv_info	- pointer to driver info
 * @param reason	- reason code (DCR_XXXX type)
 * @param param		- optional parameter
 */
typedef void (*DRV_DCR)(DRIVER_INFO drv_info, unsigned int reason, void *param);

/**
 * Driver DSR() function type
 *
 * DSR() is called from the kernel with a handle which the driver must process
 *
 * @param drv_info	- pointer to driver info
 * @param hnd		- handle
 */
typedef void (*DRV_DSR)(DRIVER_INFO drv_info, HANDLE hnd);

/**
 * Driver ISR() function type
 *
 * @param drv_info
 */
typedef void (*DRV_ISR)(DRIVER_INFO drv_info);

struct DRIVER_INFO_Type {
	unsigned int op0; /**< ASM instruction */
	unsigned int op4; /**< ASM instruction */

	DRV_ISR isr; /**< pointer to the driver's ISR() */
	DRV_DCR dcr; /**< pointer to the driver's DCR() */
	DRV_DSR dsr; /**< pointer to the driver's DSR() */

	DRIVER_INDEX drv_index; /**< driver index in DRV_TABLE */
	unsigned char isr_priority; /**< driver ISR priority */
	unsigned char peripheral_indx; /**< peripheral index  */
};

/**
 *  Cortex M3 instructions ...?
 */
#define DRIVER_INFO_STUB  0x0004f2af, 0xf008f8d0


/**
 *  Pure virtual function ...?
 */
extern "C" void __cxa_pure_virtual(void);

#endif	/* end C++ */



void usr_task_init_static(TASK_DESCRIPTION const * desc, int bStart);

/**
 * Macro to allocate a task signal
 */
#define ALLOCATE_SIGNAL(sig) CURRENT_TASK->aloc_sig |= sig

/**
 * Macro which test the task for signal
 */
#define tsk_test_signal(sig) (CURRENT_TASK->signals & sig)


#endif /* PORT_CPP_H_ */
