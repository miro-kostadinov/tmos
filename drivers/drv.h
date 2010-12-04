/**************************************************************************//**
 * @ingroup	 drivers
 * @file     drv.h
 * @brief    Drivers header
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 *
 * @defgroup drivers Drivers
 * This module contains TMOS drivers for various peripherals
 *
 * @page page_drivers Driver System
 *
 * @section dcrs 		DCR control codes processing
 *
 * @subsection dcr_open DCR_OPEN
 * When a client tries to open a handle, the kernel calls the driver's DCR with
 * DCR_OPEN
 * The driver must set hnd->res to RES_OK if the handle can be open
 *
 * @subsection dcr_cancel DCR_CANCEL
 * This control is sent when the client wants to cancel the handle operation.
 * Usually the client first checks for FLG_BUSY and only if it is set it calls
 * cancel(). But there is a time between the check and the call in which an
 * interrupt can complete the operation. So when the driver receives DCR_CANCEL
 * the operation may or may not be completed.
 *
 * The driver should do nothing if there is no operation in progress (FLG_BUSY
 * can be checked if necessarily). The driver can also ignore this control code
 * if no cancellation mechanism is supported.
 * When it is possible to cancel the operation, the driver must set status with
 * FLG_SIGNALED. If there is some data transfered the FLG_OK must also be set.
 *
 *
 * @section dsrs D		SR processing
 *
 * The driver DSR() function is called when a client requests some handle
 * operation.
 *
 * @subsection hnd_ops Handle operations
 * Most common handle operations are simple read and write. These operations are
 * signaled with FLG_READ and FLG_WRITE respectively in the hnd->cmd field.
 * Some drivers may support simultaneous read and write (swap) operation.
 * Another simple operation is a Command that can be requested with FLG_COMMAND.
 * The command operation is a driver specific operation and not every driver can
 * support it.
 *
 * In addition to the basic operations, the client can set FLG_LOCK. This makes
 * the basic operation exclusive in some way. Typically, when a lock operation
 * is requested, the driver must lock to the this client and will not start
 * requests from other clients.
 *
 * The lock falg may also have side
 * effects. For example when a lock operations is requested from the SPI driver,
 * it "locks" to the client and
 *
 *
 ******************************************************************************/

#ifndef DRV_H_
#define DRV_H_


#endif /* DRV_H_ */
