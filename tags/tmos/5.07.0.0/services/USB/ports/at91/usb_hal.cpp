/*
 * usb_hal.cpp
 *
 * 		USB Hardware API functions for AT91
 *
 *  Created on: 01.06.2011
 *      Author: Miroslav Kostadinov
 */


#include <tmos.h>
#include <pmc_drv.h>
#include <usb_drv.h>
#include <usb_hal.h>

/** Bit mask for both banks of the UDP_CSR register. */
#define UDP_CSR_RXDATA_BK      (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1)
/**  Bitmap for all status bits in CSR. */
#define REG_NO_EFFECT_1_ALL      UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1 \
                                |UDP_CSR_STALLSENTISOERROR | UDP_CSR_RXSETUP \
                                |UDP_CSR_TXCOMP
/**
 *  Sets the specified bit(s) in the UDP_CSR register.
 *
 *  \param csr register.
 *  \param flags The bitmap to set to 1.
 */
void SET_CSR(RwReg* csr, uint32_t flags)
{
	uint32_t reg;

	reg = *csr;
	reg |= REG_NO_EFFECT_1_ALL;
	reg |= flags;
	*csr = reg;
	for (int32_t nop_count = 0; nop_count < 15; nop_count++)
	{
		__NOP();
	}
}


/**
 *  Clears the specified bit(s) in the UDP_CSR register.
 *
 *  \param csr register.
 *  \param flags The bitmap to clear to 0.
 */
void CLEAR_CSR(RwReg* csr, uint32_t flags)
{
	uint32_t reg;

	reg = *csr;
	reg |= REG_NO_EFFECT_1_ALL;
	reg &= ~flags;
	*csr = reg;
	for (int32_t nop_count = 0; nop_count < 15; nop_count++)
	{
		__NOP();
	}
}

//-------------------  local static functions --------------------------------//

/** Configure the pull-up on D+ and disconnect it **/
static void at91_pullup_disable(USB_DRV_INFO drv_info)
{
//  PIO_CfgInput(USB_STAT_PIN);
	drv_info->hw_base->UDP_TXVC &= ~(uint32_t) UDP_TXVC_PUON;
}

/** Configure the pull-up on D+ and enable it **/
void at91_pullup_enable(USB_DRV_INFO drv_info)
{
	//PIO_CfgOutput0(USB_STAT_PIN);
	drv_info->hw_base->UDP_TXVC |= UDP_TXVC_PUON;
}

/**
 * Suspend USB Device HW Interface
 *
 * -# Disable transceiver
 * -# Disable USB Clock
 * -# Disable USB Peripheral
 */
static void at91_usb_suspend(USB_DRV_INFO drv_info)
{
	// Disables the UDP transceiver
	drv_info->hw_base->UDP_TXVC |= UDP_TXVC_TXVDIS;

    // Disables the 48MHz USB clock
    REG_PMC_SCDR = PMC_SCER_UDP;

    // Disables the UDP peripheral clock
    PMC_DisablePeripheral(drv_info->info.peripheral_indx);

	usb_drv_event(drv_info, e_susppend);
}

/**
 * Activate USB Device HW Interface
 * -# Enable USB Peripheral
 * -# Enable USB Clock
 * -# Enable transceiver
 */
static void at91_usb_activate(USB_DRV_INFO drv_info)
{
	// Enables the clock of the UDP peripheral
	if (!PMC_IsPeriphEnabled(drv_info->info.peripheral_indx))
		PMC_EnablePeripheral(drv_info->info.peripheral_indx);

	// Enables the 48MHz USB clock
    REG_PMC_SCER = PMC_SCER_UDP;

    // Enables the UDP transceiver.
    drv_info->hw_base->UDP_TXVC &= ~(uint32_t)UDP_TXVC_TXVDIS;

    // Enable wakeup/resume/suspend
	drv_info->hw_base->UDP_IER = UDP_IER_WAKEUP | UDP_IER_RXRSM | UDP_IER_RXSUSP | UDP_IER_EP0INT ;
}


/**
 * Clears the correct reception flag (bank 0,bank 1 or setup) of an endpoint
 */
static void at91_clear_rx_flag(RwReg *pCSR, Endpoint *endpoint, unsigned int ept_indx)
{
	switch(endpoint->bank)
	{
	case 0:
        CLEAR_CSR(pCSR, UDP_CSR_RX_DATA_BK0);
        // Swap bank if in dual-fifo mode
        if (CHIP_USB_ENDPOINTS_BANKS(ept_indx) > 1)
        {
            endpoint->bank = 1;
        }
        break;

	case 1:
        CLEAR_CSR(pCSR, UDP_CSR_RX_DATA_BK1);
        endpoint->bank = 0;
        break;

	case 2: //control endpoint
		SET_CSR(pCSR, UDP_CSR_DIR);
		// continue...

	case 3:
		CLEAR_CSR(pCSR, UDP_CSR_RXSETUP);
        endpoint->bank = 0;
		break;
	}
}


/**
 * Resets all the endpoints of the UDP peripheral.
 * @param drv_info
 */
static void at91_reset_endpoints(USB_DRV_INFO drv_info)
{
	for (int i = 0; i < USB_NUMENDPOINTS; i++)
	{
		usb_hal_ept_reset(drv_info, i);
	}
}


static void usb_write_payload(RwReg* dst, HANDLE hnd, unsigned int size)
{
    unsigned char *src;

	TRACE1_USB(" Wr");
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


static uint32_t usb_read_payload(RwReg* src, HANDLE hnd, unsigned int size)
{
	size_t dwRead;

	dwRead = size;
	if(dwRead > hnd->len)
	{
		dwRead = hnd->len;
	}
	hnd->len -= dwRead;
	size -= dwRead;

    TRACE_USB("(%d)", dwRead);
	while(dwRead--)
	{
		*hnd->dst.as_byteptr++ = *src;
	}
    return size;
}





//---------------  USB driver related functions ------------------------------//

WEAK_C void usb_drv_event(USB_DRV_INFO drv_info, USB_EVENT event)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	switch(event)
	{
//	case e_powered:
//		drv_data->usb_state = USB_STATE_POWERED;
//		break;

	case e_susppend:
		if(drv_data->usb_state > USBST_DEVICE_SUSPENDED)
		{
			drv_data->usb_previous_state = drv_data->usb_state;
			drv_data->usb_state = USBST_DEVICE_SUSPENDED;
		}
		break;

	case e_resume:
		if (drv_data->usb_state == USBST_DEVICE_SUSPENDED)
		{
			drv_data->usb_state = drv_data->usb_previous_state;
		}
		if(drv_data->usb_state < USBST_DEVICE_DEFAULT)
			drv_data->usb_state = USBST_DEVICE_DEFAULT;
		break;

	case e_reset:
		drv_data->usb_state = USBST_DEVICE_DEFAULT;
		break;

	default: ;
	break;
	}
}

void usb_drv_reset(USB_DRV_INFO drv_info)
{
	Task* task;

   	drv_isr_disable(&drv_info->info);
    TRACELN1_USB("USBD_Init");
	NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);

	// create usb helper thread
	task = usr_task_create_dynamic("USBH", (TASK_FUNCTION)usbdrv_thread,
			60,	400+TRACE_SIZE);
	if(task)
	{
		svc_task_schedule(task);
       	task->sp->r0.as_cvoidptr = drv_info;
	}

    // Must before USB & TXVC access!
    drv_pmc_enable(&drv_info->info);

	drv_info->hw_base->UDP_GLB_STAT = 0;

	// Reset endpoint structures
	at91_reset_endpoints(drv_info);

	at91_usb_suspend(drv_info);

    // Configure the pull-up on D+ and disconnect it
    at91_pullup_disable(drv_info);
}

void usb_drv_cancel_hnd(USB_DRV_INFO drv_info, HANDLE hnd)
{
	uint32_t ept_indx;
    Endpoint *endpoint;

    if(hnd->res & FLG_BUSY)
    {
    	if(hnd->cmd & FLAG_WRITE)
    		ept_indx = hnd->mode.as_bytes[1];
    	else
    		ept_indx = hnd->mode.as_bytes[0];

    	endpoint = &(drv_info->drv_data->endpoints[ept_indx]);
    	if(hnd->list_remove(endpoint->pending))
    	{
    		svc_HND_SET_STATUS(hnd, FLG_SIGNALED);
//   		TRACE1_USB("Can!");
    		if(!endpoint->pending && endpoint->state == ENDPOINT_STATE_SENDING)
    			endpoint->state = ENDPOINT_STATE_IDLE;

    	}
    }
}

/**
 * Start sending...
 *
 * AT91 logic:
 * 	1. Optionally write a packet in the endpoint FIFO
 * 	2. Set UDP_CSR_TXPKTRDY
 * 	3. If dual banks are available, fill another packet and set TXPKTRDY
 * 	4. Enable endpoint interrupt
 * 	5. On TXCOMP continue with more packets or disable the interrupt
 *
 * @param drv_info
 * @param hnd
 */
void usb_drv_start_tx(USB_DRV_INFO drv_info, HANDLE hnd)
{
	Endpoint *endpoint;
	uint32_t ept_indx;
	HANDLE prev;

	ept_indx = hnd->mode.as_bytes[1];
    TRACE_USB(" Write%d(%d)", ept_indx , hnd->len);
	endpoint = &(drv_info->drv_data->endpoints[ept_indx]);
    if( (prev=endpoint->pending) )
    {
    	while(prev->next)
    		prev = prev->next;
    	prev->next = hnd;
    } else
    {
	    if (endpoint->state != ENDPOINT_STATE_IDLE)
	    {
			svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
	    } else
	    {
			endpoint->state = ENDPOINT_STATE_SENDING;
			endpoint->pending = hnd; //sending
			usb_write_payload(&drv_info->hw_base->UDP_FDR[ept_indx], hnd,
					endpoint->size);
			SET_CSR(&drv_info->hw_base->UDP_CSR[ept_indx], UDP_CSR_TXPKTRDY);
			// If double buffering is enabled and there is data remaining,
			// prepare another packet
			if ((CHIP_USB_ENDPOINTS_BANKS(ept_indx) > 1) && hnd->len)
			{
				usb_write_payload(&drv_info->hw_base->UDP_FDR[ept_indx], hnd,
						endpoint->size);
			}

			// Enable interrupt on endpoint
			drv_info->hw_base->UDP_IER |= 1 << ept_indx;

	    }
    }
}

/**
 * Start receiving...
 * AT91 logic:
 * 	1. Core will trigger RXDATA or RXSETUP interrupt when packet is received
 * 	2. Read entire packet
 * 	3. Acknowledge with at91_cleat_rx_flag()
 *
 * @param drv_info
 * @param hnd
 */
void usb_drv_start_rx(USB_DRV_INFO drv_info, HANDLE hnd)
{
	uint32_t ept_indx;
	Endpoint *endpoint;

	ept_indx = hnd->mode.as_bytes[0];
	endpoint = &drv_info->drv_data->endpoints[ept_indx];

	if(endpoint->state == ENDPOINT_STATE_RECEIVING_OFF)
	{
		Udp* pUDP = drv_info->hw_base;
		if (!usb_read_payload(&pUDP->UDP_FDR[ept_indx], hnd, endpoint->rxfifo_cnt))
		{
			at91_clear_rx_flag(&pUDP->UDP_CSR[ept_indx], endpoint, ept_indx);
			endpoint->state = ENDPOINT_STATE_IDLE;
			pUDP->UDP_IER = (1 << ept_indx);
			TRACE1_USB("<");
		}
	}

	if(hnd->len)
	{
	    HANDLE prev;

	    if( (prev=endpoint->pending) )	//receiving
	    {
	    	while(prev->next)
	    		prev = prev->next;
	    	prev->next = hnd;
	    } else
	    	endpoint->pending = hnd;	//receiving

	} else
	{
    	svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}


/**
 * Handles a completed transfer on the given endpoint, invoking the
 * configured callback if any.
 * \param endpoint the endpoint for which the transfer has completed.
 * \param status   Status code returned by the transfer operation
 */
HANDLE usb_drv_end_transfers(Endpoint *endpoint, unsigned int status)
{
	HANDLE hnd;

	hnd = endpoint->pending;

	if (hnd)
	{
		TRACE1_USB(" EoT");
		endpoint->pending = hnd->next;
		if (__get_CONTROL() & 2)
		{
			usr_HND_SET_STATUS(hnd, status);
		}
		else
		{
			svc_HND_SET_STATUS(hnd, status);
		}
	}

	return (hnd);
}


/**
 * Reset and disable endpoint
 * @param drv_info
 * @param ept_indx
 */
void usb_hal_ept_reset(USB_DRV_INFO drv_info, unsigned int ept_indx)
{
	Udp* pUDP = drv_info->hw_base;

    // Kill pending TXPKTREADY
    CLEAR_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_TXPKTRDY);

	// Reset Endpoint Fifos
	pUDP->UDP_RST_EP |= (1 << ept_indx);
	pUDP->UDP_RST_EP &= ~(1 << ept_indx);
	pUDP->UDP_IDR = (1 << ept_indx);

	// Configure endpoint
	CLEAR_CSR(&pUDP->UDP_CSR[ept_indx],  UDP_CSR_EPEDS | UDP_CSR_EPTYPE_Msk | UDP_CSR_DIR);


    Endpoint *endpoint;
    endpoint = &drv_info->drv_data->endpoints[ept_indx];
	usb_drv_end_transfers(endpoint, USBD_STATUS_RESET);
    endpoint->state = ENDPOINT_STATE_DISABLED;
    endpoint->bank = 0;
}



//------------------  HAL related functions ----------------------------------//

/**
 * Get endpoint status
 * @param drv_info
 * @param ept_num
 * @param data
 * @return
 */
bool usb_hal_get_ep_status(USB_DRV_INFO drv_info, uint8_t ept_num, uint16_t* data)
{
	ept_num &= 0xF;

	// Check if the endpoint exists
    if (ept_num  > USB_NUMENDPOINTS)
    {
    	return false;
    }
    else
    {
        // Check if the endpoint if currently halted
        if (drv_info->drv_data->endpoints[ept_num].state  == ENDPOINT_STATE_HALTED)
        {
            *data = 1;
        } else
        {
        	*data = 0;
        }

    }
    return true;
}

void usb_hal_stall(Udp* hw_base, unsigned int ept_num)
{
	SET_CSR(&hw_base->UDP_CSR[ept_num & 0xF], UDP_CSR_FORCESTALL);
}

void usb_hal_stall_clear(Udp* hw_base, unsigned int ept_num)
{
	CLEAR_CSR(&hw_base->UDP_CSR[ept_num & 0xF], UDP_CSR_FORCESTALL);
}

/**
 * Poweron
 * @param drv_info
 */
void usb_hal_device_start(USB_DRV_INFO drv_info)
{
	TRACELN1_USB("POWERON!\r\n");

	at91_usb_activate(drv_info);

	// Reset endpoint structures
	at91_reset_endpoints(drv_info);

	drv_enable_isr(&drv_info->info);

	at91_pullup_enable(drv_info);
}

/**
 * Configures an endpoint according to its Endpoint Descriptor.
 * @param drv_info
 * @param pDescriptor Pointer to an Endpoint descriptor.
 */
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor)
{
	Endpoint *endpoint;
	uint8_t ept_indx;
	uint8_t bType;
	uint8_t bEndpointDir;
	Udp* pUDP = drv_info->hw_base;

	bType = ENDPOINT_TYPE_CONTROL;

	// Device descriptor -> Specific Control EP
	if (pDescriptor->bDescriptorType == DEVICE_DESCRIPTOR)
	{
		ept_indx = 0;
		endpoint = &drv_info->drv_data->endpoints[ept_indx];
		bEndpointDir = 0;
		endpoint->size = ((USBDeviceDescriptor *) pDescriptor)->bMaxPacketSize0;
	}
	// Not endpoint descriptor, ERROR!
	else if (pDescriptor->bDescriptorType != ENDPOINT_DESCRIPTOR)
	{
		return;
	}
	else
	{
		const USBEndpointDescriptor* descriptor;
		descriptor = (const USBEndpointDescriptor*)pDescriptor;

		ept_indx = descriptor->GetNumber();
		endpoint = &drv_info->drv_data->endpoints[ept_indx];
		bType = descriptor->GetType();
		bEndpointDir = descriptor->GetDirection();
		endpoint->size = descriptor->GetMaxPacketSize();
	}

	// Abort the current transfer is the endpoint was configured
	if (endpoint->state > ENDPOINT_STATE_IDLE)
	{
		usb_drv_end_transfers(endpoint, USBD_STATUS_RESET);
	}
	endpoint->state = ENDPOINT_STATE_IDLE;

	// Reset Endpoint Fifos
	pUDP->UDP_RST_EP |= (1 << ept_indx);
	pUDP->UDP_RST_EP &= ~(1 << ept_indx);

	// Configure endpoint
	SET_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_EPEDS | (bType << 8) | (bEndpointDir << 10));

	//enable endpoint interrupt
	pUDP->UDP_IER = (1 << ept_indx);

	TRACE_USB(" CfgEp%d", ept_indx);
}



/**
 * Endpoint interrupt handler.
 * Handle IN/OUT transfers, received SETUP packets and STALLing
 * @param pUDP
 * @param endpoint
 * @param ept_indx Index of endpoint
 */
static void usb_b_ept_handler(Udp* pUDP, Endpoint *endpoint, uint32_t ept_indx)
{
	uint32_t status;
	HANDLE hnd;
	unsigned int size;

	status = pUDP->UDP_CSR[ept_indx];

	TRACE_USB(" E%d", ept_indx);

	// IN packet sent
	if (status & UDP_CSR_TXCOMP)
	{
		//check if we have pending write
		while ((hnd = endpoint->pending) && (hnd->len == 0))
		{
			// End of transfer ?
	    	TRACE1_USB(" Wr!");
			endpoint->pending = hnd->next;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
		}

		//check if we have more to write
		if (hnd)
		{
			// Send next packet
			if (CHIP_USB_ENDPOINTS_BANKS(ept_indx) == 1)
			{
				// No double buffering
				usb_write_payload(&pUDP->UDP_FDR[ept_indx], hnd, endpoint->size);
				SET_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_TXPKTRDY);
				// Acknowledge interrupt
				CLEAR_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_TXCOMP);
			}
			else
			{
				// Double buffering
				SET_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_TXPKTRDY);
				// Acknowledge interrupt
				CLEAR_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_TXCOMP);
				usb_write_payload(&pUDP->UDP_FDR[ept_indx], hnd, endpoint->size);
			}
		}
		else
		{
			endpoint->state = ENDPOINT_STATE_IDLE;
			// Disable interrupt if this is not a control endpoint
			if ((status & UDP_CSR_EPTYPE_Msk) != UDP_CSR_EPTYPE_CTRL)
			{
				pUDP->UDP_IDR |= 1 << ept_indx;
			}
			// Acknowledge interrupt
			CLEAR_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_TXCOMP);
		}
	}

	// STALL sent
	if ( status & UDP_CSR_STALLSENTISOERROR )
	{
		TRACE1_USB(" Sta");

		CLEAR_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_STALLSENTISOERROR);

		if ((status & UDP_CSR_EPTYPE_Msk) == UDP_CSR_EPTYPE_ISO_IN
				|| (status & UDP_CSR_EPTYPE_Msk) == UDP_CSR_EPTYPE_ISO_OUT)
		{

			TRACE_USB(" Isoe [%d]", ept_indx);
			while (usb_drv_end_transfers(endpoint, USBD_STATUS_ABORTED))
			{

			}
			endpoint->state = ENDPOINT_STATE_IDLE;
		}
		else
		{
			TRACE1_USB(" Sta_ack");

			// If the endpoint is not halted, clear the STALL condition
			if (endpoint->state != ENDPOINT_STATE_HALTED)
			{
				TRACE1_USB( "!");
				CLEAR_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_FORCESTALL);
			}
		}
	}

	// OUT packet received
	if ( status & UDP_CSR_RXDATA_BK )
	{
		size = status >> 16;
    	TRACE_USB(" Rd%d", size);

		if(endpoint->state == ENDPOINT_STATE_IDLE)
		{
	    	while(size && (hnd = endpoint->pending))
	    	{
	    		endpoint->pending = hnd->next;
	    		size = usb_read_payload(&pUDP->UDP_FDR[ept_indx], hnd, size);
				usr_HND_SET_STATUS(hnd, RES_SIG_OK);
	    	}
	    	if(size)
			{
				// Disable interrupt
				pUDP->UDP_IDR |= 1 << ept_indx;
				endpoint->rxfifo_cnt = size;
				endpoint->state = ENDPOINT_STATE_RECEIVING_OFF;
				TRACE1_USB(">");
	        } else
	        {
	        	at91_clear_rx_flag(&pUDP->UDP_CSR[ept_indx], endpoint, ept_indx);
	        }
		} else
		{
	    	TRACE_USB(" error %x state=%d", pUDP->UDP_CSR[ept_indx], endpoint->state);
	    	pUDP->UDP_RST_EP |= (1 << ept_indx);
	    	pUDP->UDP_RST_EP &= ~(1 << ept_indx);
	        CLEAR_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);

		}
	}

	// SETUP packet received
	if ( status & UDP_CSR_RXSETUP )
	{
		USBGenericRequest* request=NULL;

		size = status >> 16;
    	TRACE_USB(" Stp%d", size);

		if(endpoint->state == ENDPOINT_STATE_IDLE)
		{
			endpoint->bank = 3;
	    	while(size && (hnd = endpoint->pending))
	    	{
	    		if(!request)
	        		request = (USBGenericRequest*)hnd->dst.as_voidptr;
	    		endpoint->pending = hnd->next;
	    		size = usb_read_payload(&pUDP->UDP_FDR[ept_indx], hnd, size);
				usr_HND_SET_STATUS(hnd, RES_SIG_OK);
	    	}
    		if(request && request->GetDirection() == USB_REQUEST_DIR_IN)
    			endpoint->bank = 2;
	    	if(size)
			{
				// Disable interrupt
				pUDP->UDP_IDR |= 1 << ept_indx;
				endpoint->rxfifo_cnt = size;
				endpoint->state = ENDPOINT_STATE_RECEIVING_OFF;
				TRACE1_USB(">");
	        } else
	        {
	        	at91_clear_rx_flag(&pUDP->UDP_CSR[ept_indx], endpoint, ept_indx);
	        }
		} else
		{
	    	TRACE_USB(" error %x state=%d", pUDP->UDP_CSR[ept_indx], endpoint->state);
	    	pUDP->UDP_RST_EP |= (1 << ept_indx);
	    	pUDP->UDP_RST_EP &= ~(1 << ept_indx);
			CLEAR_CSR(&pUDP->UDP_CSR[ept_indx], UDP_CSR_RXSETUP);
		}
	}
}

void USBD_ISR(USB_DRV_INFO drv_info)
{
	unsigned int status;
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	Udp* pUDP = drv_info->hw_base;

	// Get interrupt status
	// Some interrupts may get masked depending on the device state
	status = pUDP->UDP_ISR;
	if(status & UDP_ISR_SOFINT)
		pUDP->UDP_ICR = UDP_ICR_SOFINT;

	status &= pUDP->UDP_IMR;
	TRACELN_USB("USB:%x[%04x]", drv_data->usb_state, status);

	/* Return immediately if there is no interrupt to service */
	if (status == 0)
	{
		TRACE_USB("-[%04x]", pUDP->UDP_ISR);
		return;
	}

	/* Resume (Wakeup) */
	if ( status & (UDP_ISR_WAKEUP | UDP_ISR_RXRSM))
	{
		TRACE1_USB(" Rsm");
		/* Clear and disable resume interrupts */
		pUDP->UDP_ICR = status & (UDP_ISR_WAKEUP | UDP_ISR_RXRSM);

		/* Active the device */
		at91_usb_activate(drv_info);
    	usb_drv_event(drv_info, e_resume);
	}

	/* Suspend      This interrupt is always treated last (hence the '==') */
	if (status == UDP_ISR_RXSUSP)
	{
		/* Acknowledge interrupt */
		pUDP->UDP_ICR = UDP_ICR_RXSUSP;

		TRACE1_USB(" Susp");

		/* Don't do anything if the device is already suspended */
		if (drv_data->usb_state != USBST_DEVICE_SUSPENDED)
		{
			/* Suspend HW interface */
			at91_usb_suspend(drv_info);
		}
	}
	/* End of bus reset */
	else if (status & UDP_ISR_ENDBUSRES)
	{
		const USBDDriverDescriptors * dev_descriptors;

		pUDP->UDP_ICR = UDP_ISR_ENDBUSRES;

		TRACE1_USB(" EoBRes");

    	usb_drv_event(drv_info, e_reset);

		// Active the USB HW
		at91_usb_activate(drv_info);

		// reset endpoints
		at91_reset_endpoints(drv_info);

		// Configure device
		dev_descriptors = drv_info->drv_data->device.pDescriptors;
		if(dev_descriptors)
		{
			usb_hal_ept_config(drv_info, &dev_descriptors->pFsDevice->as_generic);
		}
	}
	/* Endpoint interrupts */
	else
	{
		int32_t eptnum = 0;

		status &= ((1 << CHIP_USB_NUMENDPOINTS) - 1);
		while (status)
		{
			/* Check if endpoint has a pending interrupt */
			if ( status & 1 )
			{
				usb_b_ept_handler(pUDP, &drv_data->endpoints[eptnum], eptnum);
			}
			status >>= 1;
			eptnum++;
		}
	}

}
