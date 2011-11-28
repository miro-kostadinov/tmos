/*
 * cdc_data_inf.cpp
 *
 *  Created on: 13.07.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <usb_opt.h>
#include <cdc_data_inf.h>
#include <usb_svc.h>

void usb_cdc_data_interface::Initialize(const USBInterfaceDescriptor* ds,
		int size)
{

}

void usb_cdc_data_interface::RequestHandler(const void* drv,
		const USBGenericRequest *pRequest, HANDLE hnd)
{
	TRACE_USB(" cdc data reques(code %d)",
			pRequest->bRequest);
    usb_svc_stall(hnd);

}
