/*
 * usb_hal.cpp
 *
 * 		USB Hardware API functions for AT91
 *
 *  Created on: 01.06.2011
 *      Author: Miroslav Kostadinov
 */


#include <tmos.h>
#include <usb_opt.h>
#include <usb_hal.h>

void usb_write_payload(word_reg *dst, HANDLE hnd,	unsigned int size)
{
    unsigned char *src;

    while(1)
    {
	    src = hnd->src.as_byteptr;
	    if (size > hnd->len)
	    {
	    	size -= hnd->len;
		    // Write packet in the FIFO buffer
		    while(hnd->len)
		    {
		    	hnd->len--;
		    	*dst = *src++;
		    }
		   	hnd->src.as_byteptr = src;
		   	hnd = hnd->next;
		   	if(!hnd)
		   		return;
	    }
	    else
	    {
		    hnd->len -= size;
		    while(size--)
		        *dst = *src++;
		   	hnd->src.as_byteptr = src;
	   		return;
	    }
    }
}

unsigned int usb_read_payload(unsigned char *src, HANDLE hnd, unsigned int size)
{
	size_t dwRead;
    unsigned char *dst;

    TRACE_USB("(%d)", size);

    while(size && hnd)
    {
    	dwRead = size;
    	if(dwRead > hnd->len)
    	{
    		dwRead = hnd->len;
    	}
    	hnd->len -= dwRead;
    	size -= dwRead;

    	dst = hnd->dst.as_byteptr;
    	while(dwRead--)
    	{
    	    *dst++ = *src;
    	}
    	hnd->dst.as_byteptr = dst;

    	if(!hnd->len)
    	{
    		hnd=NULL;

    	}
    }
    return (size);
}

void usb_hal_reset(USB_DRV_INFO drv_info)
{
   	drv_isr_disable(&drv_info->info);
    TRACE1_USB_DEBUG("\r\nUSBD_Init");
	/*
	    Function: USBD_HAL_Init
	        Initializes the USB HW Access driver.
	*/

    /* Must before USB & TXVC access! */
    drv_pmc_enable(&drv_info->info);

	drv_info->hw_base->UDP_GLB_STAT = 0;
    USBD_SUSPEND(drv_info);
//		    drv_data->install_mini = drv_info->install_mini;

    // Configure the pull-up on D+ and disconnect it
    USB_PULLUP_DISABLE(drv_info);
}

void usb_ack_packet(Udp* hw_base, Endpoint* endpoint, unsigned int eptnum)
{
    UDP_ClearRxFlag(&hw_base->UDP_CSR[eptnum], endpoint, eptnum);
}


void USBD_ISR(USBD_INFO drv_info)
{
	unsigned int status;
    USBD_DRIVER_DATA* drv_data = drv_info->drv_data;
	Udp* pUDP = drv_info->hw_base;

    // Get interrupt status
    // Some interrupts may get masked depending on the device state
    status = pUDP->UDP_ISR;
    status &= pUDP->UDP_IMR;

    if (drv_data->deviceState < USBD_STATE_POWERED)
    {
        status &= UDP_ICR_WAKEUP | UDP_ICR_RXRSM;
        pUDP->UDP_ICR = ~status;
    }

    /* Return immediately if there is no interrupt to service */
    if (status == 0)
    {
        TRACE1_USB(".\n\r");
        return;
    }

    /* Resume (Wakeup) */
    if ((status & (UDP_ISR_WAKEUP | UDP_ISR_RXRSM)) != 0)
    {
        TRACELN1_USB("Res ");
        /* Clear and disable resume interrupts */
        pUDP->UDP_ICR = UDP_ICR_WAKEUP | UDP_ICR_RXRSM | UDP_ICR_RXSUSP;
        pUDP->UDP_IDR = UDP_IDR_WAKEUP | UDP_IDR_RXRSM;

        /* Do resome operations */

        // --- USBD_ResumeHandler();
        /* Don't do anything if the device was not suspended */
        if (drv_data->deviceState == USBD_STATE_SUSPENDED)
        {
            /* Active the device */
            USBD_HAL_Activate(drv_info);
            drv_data->deviceState = drv_data->previousDeviceState;
//            if (deviceState >= USBD_STATE_DEFAULT)
//            {
//                /* Invoke the Resume callback */
//                if (USBDCallbacks_Resumed)
//                    USBDCallbacks_Resumed();
//            }
        }
        // ---
    }

    /* Suspend      This interrupt is always treated last (hence the '==') */
    if (status == UDP_ISR_RXSUSP)
    {

        TRACELN1_USB("Susp ");
        /* Enable wakeup */
        pUDP->UDP_IER = UDP_IER_WAKEUP | UDP_IER_RXRSM;
        /* Acknowledge interrupt */
        pUDP->UDP_ICR = UDP_ICR_RXSUSP;

        /* Do suspend operations */

        //--- USBD_SuspendHandler();
        /* Don't do anything if the device is already suspended */
        if (drv_data->deviceState != USBD_STATE_SUSPENDED)
        {
            /* Switch to the Suspended state */
        	drv_data->previousDeviceState = drv_data->deviceState;
        	drv_data->deviceState = USBD_STATE_SUSPENDED;

            /* Suspend HW interface */
            USBD_HAL_Suspend(drv_info);

            /* Invoke the User Suspended callback (Suspend System?) */
//            if (USBDCallbacks_Suspended)
//                USBDCallbacks_Suspended();
        }
        //--
    }
    /* End of bus reset */
    else if ((status & UDP_ISR_ENDBUSRES) != 0)
    {

        TRACELN1_USB("EoBRes ");
        /* Flush and enable the Suspend interrupt */
        pUDP->UDP_ICR = UDP_ICR_WAKEUP | UDP_ICR_RXRSM | UDP_ICR_RXSUSP;
        pUDP->UDP_IER = UDP_IER_RXSUSP;

        /* Do RESET operations */
        //--- USBD_ResetHandler();
        /* The device enters the Default state */
        drv_data->deviceState = USBD_STATE_DEFAULT;
        /* Active the USB HW */
        USBD_HAL_Activate(drv_info);
        /* Only EP0 enabled */
        USBD_HAL_ResetEPs(drv_info, 0xFFFFFFFF, USBD_STATUS_RESET, 0);
        USBD_ConfigureEndpoint(drv_info, NULL);
        /* Invoke the Reset callback */
//        if (USBDCallbacks_Reset)
//            USBDCallbacks_Reset();
        //---

        /* Acknowledge end of bus reset interrupt */
        pUDP->UDP_ICR = UDP_ICR_ENDBUSRES;
    }
    /* Endpoint interrupts */
    else
    {
        int32_t eptnum = 0;

        status &= ((1 << CHIP_USB_NUMENDPOINTS) - 1);
        while (status != 0)
        {
            /* Check if endpoint has a pending interrupt */
            if ((status & (1 << eptnum)) != 0)
            {
                UDP_EndpointHandler(pUDP, &drv_data->endpoints[eptnum], eptnum);
                status &= ~(1 << eptnum);

                if (status != 0) {

                	TRACE1_USB("\n\r  - ");
                }
            }
            eptnum++;
        }
    }

}
