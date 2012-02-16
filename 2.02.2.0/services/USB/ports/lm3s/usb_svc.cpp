/*
 * usb_svc.cpp
 *
 *  Created on: 07.07.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <usb_svc.h>
#include <usb_hal.h>

void usb_svc_stall_hook(USB_DRV_INFO drv_info, HANDLE hnd)
{
	unsigned char eptnum = hnd->mode0;
    Endpoint *endpoint = &drv_info->drv_data->endpoints[eptnum];

    // Check that endpoint is in Idle state
    if (endpoint->state != ENDPOINT_STATE_IDLE)
    {
    	TRACE_USB(" Stall(%d): locked", eptnum);
    } else
    {
	    TRACE_USB(" Stall(%d)", eptnum);
	    usb_hal_stall(drv_info->hw_base, eptnum, ENDPOINT_DIRECTION_IN);
    }

}

/**
 * Causes the given endpoint to acknowledge the next packet it receives
 * with a STALL handshake
*/
void usb_svc_stall(HANDLE hnd)
{
	// note: Stall will be performed for the last endpoint used by this handle!!
	hnd->dst.as_voidptr = (void*)usb_svc_stall_hook;
	hnd->hcontrol(DCR_HANDLE);
}

//-----------------------------------------------------------------------------
void usb_svc_setconfiguration_hook(USB_DRV_INFO drv_info, HANDLE hnd)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	unsigned char cfgnum = hnd->src.as_int;

	TRACE_USB(" SetCfg(%d)", cfgnum);

    // If the configuration number if non-zero, the device enters the
    // Configured state
    if (cfgnum != 0)
    {
        drv_data->usb_state = USB_STATE_CONFIGURED;
    }
    // If the configuration number is zero, the device goes back to the Address
    // state
    else
    {
        drv_data->usb_state = USB_STATE_ADDRESS;

        // Abort all transfers
    	for(int i= 1; i<USB_NUMENDPOINTS; i++)
    	{
    		usb_ept_reset(drv_info, i);
    	}
    }
	drv_data->device.cfgnum = cfgnum;
}

/**
 * Sets the current device configuration
*/
void usb_svc_setconfiguration(HANDLE hnd, unsigned int cfg)
{
	// note: Stall will be performed for the last endpoint used by this handle!!
	hnd->dst.as_voidptr = (void*)usb_svc_setconfiguration_hook;
	hnd->src.as_int = cfg;
	hnd->hcontrol(DCR_HANDLE);
}

//-----------------------------------------------------------------------------
void usb_svc_configendpoints_hook(USB_DRV_INFO drv_info, HANDLE hnd)
{
	const USBGenericDescriptor* endpoint;


	endpoint = (const USBGenericDescriptor*)hnd->src.as_int;

	usb_hal_ept_config(drv_info, endpoint);
}

/**
 * Sets the current device configuration
*/
void usb_svc_configendpoint(HANDLE hnd, const USBGenericDescriptor* ds)
{
	// note: Stall will be performed for the last endpoint used by this handle!!
	hnd->dst.as_voidptr = (void*)usb_svc_configendpoints_hook;
	hnd->src.as_cvoidptr = ds;
	hnd->hcontrol(DCR_HANDLE);
}

//-----------------------------------------------------------------------------
void usb_svc_setaddress_hook(USB_DRV_INFO drv_info, HANDLE hnd)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	unsigned char address = hnd->src.as_int;

	TRACE_USB(" SetAddr(%d)", address);

    // If the address is 0, the device returns to the Default state
    if (address == 0)
    {

        drv_data->usb_state = USB_STATE_DEFAULT;
    }
    // If the address is non-zero, the device enters the Address state
    else
    {
        drv_data->usb_state = USB_STATE_ADDRESS;
    }
    drv_info->hw_base->USBFADDR = address;
}

/**
 * Sets the current device configuration
*/
void usb_svc_setaddress(HANDLE hnd, unsigned int adr)
{
	// note: Stall will be performed for the last endpoint used by this handle!!
	hnd->dst.as_voidptr = (void*)usb_svc_setaddress_hook;
	hnd->src.as_int = adr;
	hnd->hcontrol(DCR_HANDLE);
}
//-----------------------------------------------------------------------------
void usb_svc_halt_hook(USB_DRV_INFO drv_info, HANDLE hnd)
{
	unsigned int eptnum = hnd->src.as_int;
    Endpoint *endpoint = &drv_info->drv_data->endpoints[eptnum];

	TRACE_USB(" Halt(%d)", eptnum);

    // Check that endpoint is enabled and not already in Halt state
    if ((endpoint->state != ENDPOINT_STATE_DISABLED)
        && (endpoint->state != ENDPOINT_STATE_HALTED))
   	{
        // Abort the current transfer if necessary
    	usb_end_transfers(endpoint, USBD_STATUS_ABORTED);

        endpoint->state = ENDPOINT_STATE_HALTED;

        // Put endpoint into Halt state
    	usb_hal_stall(drv_info->hw_base, eptnum, hnd->len);

        // Enable the endpoint interrupt
        eptnum = 1 << eptnum;
        if(hnd->len)
        	drv_info->hw_base->USBRXIE |= eptnum;
        else
        	drv_info->hw_base->USBTXIE |= eptnum;
    }
}

/**
 * Sets the current device configuration
*/
void usb_svc_halt(HANDLE hnd, unsigned int eptnum, int is_in_dir)
{
	// note: Stall will be performed for the last endpoint used by this handle!!
	hnd->dst.as_voidptr = (void*)usb_svc_halt_hook;
	hnd->src.as_int = eptnum;
	hnd->len = is_in_dir;
	hnd->hcontrol(DCR_HANDLE);
}

//-----------------------------------------------------------------------------
void usb_svc_unhalt_hook(USB_DRV_INFO drv_info, HANDLE hnd)
{
	unsigned char eptnum = hnd->src.as_int;
    Endpoint *endpoint = &drv_info->drv_data->endpoints[eptnum];

	TRACE_USB(" Unhalt(%d)", eptnum);

    if (endpoint->state == ENDPOINT_STATE_HALTED)
    {
        // Return endpoint to Idle state
        endpoint->state = ENDPOINT_STATE_IDLE;

        // Clear FORCESTALL flag
        usb_hal_stall_clear(drv_info->hw_base, eptnum, hnd->len);
    }
}

/**
 * Sets the current device configuration
*/
void usb_svc_unhalt(HANDLE hnd, unsigned int eptnum, int is_in_dir)
{
	// note: Stall will be performed for the last endpoint used by this handle!!
	hnd->dst.as_voidptr = (void*)usb_svc_unhalt_hook;
	hnd->src.as_int = eptnum;
	hnd->len = is_in_dir;
	hnd->hcontrol(DCR_HANDLE);
}
