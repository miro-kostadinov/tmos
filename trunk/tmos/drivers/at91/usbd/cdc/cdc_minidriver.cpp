#include <tmos.h>
#include <USBDDriver.h>
#include <cdc_serial_port.h>
#include <usbd_drv.h>
#include <cdc_driver.h>


__attribute__ ((weak)) void CDC_SETLINECODING(CDCDSerialPort *)
{

}

__attribute__ ((weak)) void CDC_SETCONTROLLINESTATE(CDCDSerialPort *)
{

}

__attribute__ ((weak)) uint8_t CDCDSerial_LineCodingIsToChange(CDCLineCoding * pLineCoding)
{
    /* Nothing to do */
	return USBRC_SUCCESS;
}

__attribute__  ((weak)) void CDCDSerial_ControlLineStateChanged(uint8_t DTR,uint8_t RTS)
{
    /* Nothing to do */
}

/**
 * USB CDC Serial Port Event Handler.
 * \param event Event code.
 * \param param Event parameter.
 */
static uint32_t CDCDSerial_EventHandler(uint32_t event, uint32_t param)
{
	switch (event)
	{
	case CDCDSerialPortEvent_SETCONTROLLINESTATE:
	{
		if (CDCDSerial_ControlLineStateChanged)
		{
			CDCDSerial_ControlLineStateChanged(
					(param & CDCControlLineState_DTR) > 0, (param
							& CDCControlLineState_RTS) > 0);
		}
	}
		break;

	case CDCDSerialPortEvent_SETLINECODING:
	{
		if (CDCDSerial_LineCodingIsToChange)
		{
			event = CDCDSerial_LineCodingIsToChange((CDCLineCoding*) param);
			if (event != USBRC_SUCCESS)
				return event;
		}
	}
		break;

	default:
		return USBRC_SUCCESS;
	}

	return USBRC_SUCCESS;
}

//*----------------------------------------------------------------------------
//*			CDC task
//*----------------------------------------------------------------------------

/*
 * Standard Request logic
 *
 * 	UDP_EndpointHandler() calls
 * 		USBD_RequestHandler() calls (if)
 * 			USBDCallbacks_RequestReceived
 *
 *  USBDCallbacks_RequestReceived for cdc calls
 * 		CDCDSerialDriver_RequestHandler
 * 			calls CDCDSerial_RequestHandler() or USBDDriver_RequestHandler()
 *
 *  CDCDSerial_RequestHandler calls CDCDSerialPort_RequestHandler
 *
 */

void cdc_thread(USBD_INFO drv_info, uint8_t bInterfaceNb)
// USBDDriverDescriptors *cdcDescriptors
{
    CHandle handle;
    CDCDSerialPort cdcdSerialDriver;
    USBGenericRequest request;

    handle.tsk_safe_open(drv_info->info.drv_index, 0); //Endpoint 0

	TRACE1_USB_DEBUG("CDCDSerialDriver_Initialize\r\n");


    /* Initialize serial port function */
    CDCDSerialPort_Initialize(
                      &cdcdSerialDriver, &drv_info->drv_data->usbdDriver,
                      (CDCDSerialPortEventHandler)CDCDSerial_EventHandler,
                      0,
                      bInterfaceNb, 2);


//    cdcdriver->lineCoding = CDCLineCoding__SETLINECODING;
//    // Initialize Abstract Control Model attributes
//    CDCLineCoding_Initialize(cdcdriver,
//                             115200,
//                             CDCLineCoding_ONESTOPBIT,
//                             CDCLineCoding_NOPARITY,
//                             8);
//    cdcdriver->isCarrierActivated = 0;
//    cdcdriver->serialState = 0;

    for(;;)
    {
    	// locket read returns setup transactions only
    	if(handle.tsk_read_locked(&request, sizeof(request))!=RES_OK)
    	{
    		TRACE1_USB_DEBUG(".");
    		 tsk_sleep(10); //deadlock when not connected
    	}
    	else
    	{
//		    trace_LOG(trace_INFO, "\r\nNewReq ");

    	    /* Validate interface */
    	    if (request.wIndex >= cdcdSerialDriver.bInterfaceNdx &&
    	        request.wIndex < cdcdSerialDriver.bInterfaceNdx + cdcdSerialDriver.bNumInterface)
    	    {
		    // Handle the request
		    switch (request.bRequest)
		    {
		        case CDCGenericRequest_SETLINECODING:

		        	TRACE1_USB_DEBUG("\r\nsLineCoding");

		        	if (handle.tsk_read(&cdcdSerialDriver.lineCoding, sizeof(CDCLineCoding), USB_SETUP_READ_TOUT)==RES_OK)
					{
						//read OK
						TRACE_USB_DEBUG(" %d", cdcdSerialDriver.lineCoding.dwDTERate);
						CDC_SETLINECODING(&cdcdSerialDriver);
						handle.tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
					}
		            break;

		        case CDCGenericRequest_GETLINECODING:

		        	TRACE1_USB_DEBUG("\r\ngLineCoding");

		        	handle.tsk_write(&cdcdSerialDriver.lineCoding, sizeof(CDCLineCoding), USB_SETUP_WRITE_TOUT);
		            break;

		        case CDCGenericRequest_SETCONTROLLINESTATE:
		        {
		            uint8_t DTR, RTS;

		            DTR = ((request.wValue & CDCControlLineState_DTR) > 0);
		            RTS = ((request.wValue & CDCControlLineState_RTS) > 0);
		            TRACE_USB_DEBUG("sControlLineState(%d, %d) ", DTR, RTS);

		            cdcdSerialDriver.bControlLineState = (uint8_t)request.wValue;

		            CDC_SETCONTROLLINESTATE(&cdcdSerialDriver);
		            handle.tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);

		            break;
		        }

		        default:

		            USBDDriver_RequestHandler(cdcdSerialDriver.pUsbd, &request, &handle);
		            break;
		    }
    	    }
    	}

    }
}


//------------------------------------------------------------------------------
//         Installing CDC...
//
//	The CDC minidriver requires a task.
//	If for some reason multiple CDCs are required then a task shoould be created
//	for each CDC. All tasks can share the same thread routine.
//  Every cdc task takes as parameter the corresponding USB driver index and unique
//  CDCSerialdriver and descriptors.
//------------------------------------------------------------------------------

TASK_DECLARE_STATIC(cdc_task, "CDCT", (TASK_FUNCTION)cdc_thread, 130, 150); //min stack size is 1 word

/*
    Variable: cdcdSerialDriver
        Static instance of the CDC serial driver.
*/
//CDCDSerialDriver cdcdSerialDriver;


WEAK_C void usb_install_minidrv(USBD_INFO drv_info)
{

	//passing the parameters
	usr_task_init_static(&cdc_task_desc, true);
	cdc_task.sp->r0.as_voidptr = (void*)drv_info;
	cdc_task.sp->r1.as_int	= CDCDSerialDriver_CC_INTERFACE;

}
