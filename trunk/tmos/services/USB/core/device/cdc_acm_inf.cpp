/*
 * cdc_acm_inf.cpp
 *
 *  Created on: 13.07.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <usb_opt.h>
#include <cdc_acm_inf.h>
#include <usb_svc.h>

void usb_cdc_acm_interface::Initialize(const USBInterfaceDescriptor* ds,
		int size)
{
	descriptor = ds;
}

void usb_cdc_acm_interface::RequestHandler(const void* drv,
		const USBGenericRequest *pRequest, HANDLE hnd)
{
    /* Check request code */
    switch (pRequest->bRequest)
    {
    case CDCRequest_SET_LINE_CODING:

    	TRACE1_USB(" sLineCoding");

    	if (hnd->tsk_read(&lineCoding, sizeof(lineCoding), USB_SETUP_READ_TOUT)==RES_OK)
		{
			//read OK
			TRACE_USB(" rate=%d", lineCoding.dwDTERate);
			set_line_coding(drv);
	        usb_svc_send_control_status(hnd);
		}
        break;

    case CDCRequest_GET_LINE_CODING:

    	TRACE1_USB(" gLineCoding");

    	hnd->tsk_write(&lineCoding, sizeof(lineCoding), USB_SETUP_WRITE_TOUT);
        break;

    case CDCRequest_SET_CONTROL_LINE_STATE:
    {
        TRACE_USB("sControlLineState(%02x) ", pRequest->wValue);

        bControlLineState = (uint8_t)pRequest->wValue;

        set_control_line_state(drv);

        usb_svc_send_control_status(hnd);
        break;
    }

    default:

    	TRACE_USB(" Unknown cdc acm request(%d)", pRequest->bRequest);
        usb_svc_stall(hnd);
        break;

    }

}
