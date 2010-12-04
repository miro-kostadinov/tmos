/**************************************************************************//**
 * @defgroup ports TMOS Kernel
 * @{
 * @file     tmos_inc.h
 * @brief    TMOS Kernel common defines
 * @version  V3.00
 * @date     05. May 2008
 * @author	 Miroslav Kostadinov
 *
 * @defgroup libs TMOS libraries
 *
 * @}
 *
 ******************************************************************************/
#ifndef TMOS_INC_H_
#define TMOS_INC_H_

#include <port_inc.h>

//----- Driver Control Codes ----------------//

#define DCR_RESET			0x01	//!< unconditional driver reset
#define DCR_SHUTDOWN		0x02	//!< entering low power/frequency mode
#define DCR_CLOCK			0x03	//!< clock change
#define DCR_OPEN			0x04	//!< open handle
#define DCR_CLOSE			0x05	//!< close handle
#define DCR_CANCEL			0x06	//!< cancel handle operation
#define DCR_SIGNAL			0x07	//!< signal from handle that is owned from a driver
#define DCR_GET_WAITING		0x08	//!< return waiting list to helper task
#define DCR_PARAMS			0x09	//!< set driver parameters
#define DCR_ISR				0x0a	//!< can be used when ISR needs to call DCR
#define DCR_HANDLE			0x0b	//!< DCR call with a handle
#define DCR_BAUDRATE		0x10	//!< Baudrate change request


#endif /* TMOS_INC_H_ */
