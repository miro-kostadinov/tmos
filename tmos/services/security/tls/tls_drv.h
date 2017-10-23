/*
 * tls_drv.h
 *
 *  Created on: Jun 6, 2017
 *      Author: miro
 */

#ifndef SERVICES_SECURITY_TLS_TLS_DRV_H_
#define SERVICES_SECURITY_TLS_TLS_DRV_H_

#include <tmos.h>
#include <csocket.h>

#ifndef TLS_MAX_CONNECTIONS
#define TLS_MAX_CONNECTIONS 2
#endif

#ifndef TLS_MAX_TIMEOUT
#define TLS_MAX_TIMEOUT	(120*1000)
#endif

#define TLS_CANCEL_SIGNAL 1
#define TLS_CLIENT_SIGNAL 2
#define TLS_CLOSE_SIGNAL  4
#define TLS_SIGNALS (TLS_CANCEL_SIGNAL | TLS_CLIENT_SIGNAL | TLS_CLOSE_SIGNAL)


struct TLS_DRIVER_DATA
{
	Task* 		tls_task[TLS_MAX_CONNECTIONS];
	int32_t		tls_clients;
};

struct TLS_DRIVER_INFO
{

	//--- required for all drivers -----///
	DRIVER_INFO_Type 	info;				//!< standard driver info, required from the OS


	//--- fields specific for this driver -----///
    TLS_DRIVER_DATA*	drv_data;			//!< pointer to the driver data (RAM) structure

};


void TLS_DCR(TLS_DRIVER_INFO * drv_info, unsigned int reason, HANDLE param);
void TLS_DSR(TLS_DRIVER_INFO * drv_info, HANDLE hnd);




#endif /* SERVICES_SECURITY_TLS_TLS_DRV_H_ */
