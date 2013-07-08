/*
 * usb_hal.cpp
 *
 * 		USB Hardware API functions for LM3S
 *
 *  Created on: 01.06.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <usb_opt.h>
#include <usb_hal.h>
#include <gpio_drv.h>
#include <usb_drv.h>
#include <cmsis_cpp.h>
#include <tmos_atomic.h>


//-------------------  local static functions --------------------------------//
static void usb_write_payload(volatile void* dst, HANDLE hnd, unsigned int size)
{
	TRACE1_USB(" Wr");
	while (1)
	{
		if (size > hnd->len)
		{
			if (hnd->len)
			{
				size -= hnd->len;
				// Write packet in the FIFO buffer
				while (hnd->len > 3)
				{
					hnd->len -= 4;
					*(volatile int*) dst = *hnd->src.as_intptr++;
				}
				while (hnd->len)
				{
					hnd->len--;
					*(volatile char*) dst = *hnd->src.as_byteptr++;
				}
				hnd = hnd->next;
				if (!hnd)
					return;
			}
			else
			{
				hnd->res &= ~FLG_EOF;
				return;
			}
		}
		else
		{
			if (size == hnd->len)
				hnd->res |= FLG_EOF;
			hnd->len -= size;
			while (size > 3)
			{
				size -= 4;
				*(volatile int*) dst = *hnd->src.as_intptr++;
			}
			while (size--)
				*(volatile char*) dst = *hnd->src.as_byteptr++;
			return;
		}
	}
}

static uint32_t usb_read_payload(volatile void* src, HANDLE hnd, unsigned int size)
{
	size_t dwRead;

    TRACE_USB("(%d)", size);

	dwRead = size;
	if(dwRead > hnd->len)
	{
		dwRead = hnd->len;
	}
	hnd->len -= dwRead;
	size -= dwRead;

	while(dwRead>3)
	{
		dwRead -= 4;
		*hnd->dst.as_intptr++ = *(volatile int*)src;
	}
	while(dwRead--)
	{
		*hnd->dst.as_byteptr++ = *(volatile char*)src;
	}

	hnd->res = RES_BUSY | FLG_OK;

	if(size >0xff)
		hnd->mode1 = 0xff;
	else
		hnd->mode1 = size;

    return (size);
}

/** PKTRDY must be set to enable transmission of the packet that was loaded
 */
static void usb_txpktrdy(USB_Type* hw_base, unsigned int ept_indx, int len)
{
	unsigned int flags;

	if(ept_indx)
	{
		flags = USB_USBTXCSRL_TXRDY;
	} else
	{
		if(len)
			flags = USB_USBTXCSRL0_TXRDY;
		else
			flags = USB_USBTXCSRL0_TXRDY | USB_USBTXCSRL0_DATAEND;
	}
    hw_base->DEVICE_EP[ept_indx].USBTXCSRL = flags;

}


static unsigned int usb_ept0_status(USB_Type* hw_base)
{
	unsigned int status, clear;

	status = hw_base->DEVICE_EP[0].USBTXCSRL;
	clear = status;

    // clear SETEND
    if(status & USB_USBTXCSRL0_SETEND)
    {
    	clear |= USB_USBTXCSRL_SETENDC;
    }

    // clear STALLED
    if(status & USB_USBTXCSRL0_STALLED)
    {
    	clear &= ~USB_USBTXCSRL0_STALLED;
    }
    hw_base->DEVICE_EP[0].USBTXCSRL = clear & ~USB_USBTXCSRL0_DATAEND;

    return (status);
}

static unsigned int usb_ept_tx_status(USB_Type* hw_base, unsigned int ept_indx)
{
	unsigned int status;

	status = hw_base->DEVICE_EP[ept_indx].USBTXCSRL;

	hw_base->DEVICE_EP[ept_indx].USBTXCSRL = status & ~(USB_USBTXCSRL0_STALLED
			| USB_USBTXCSRL_UNDRN);
    return (status);
}

static unsigned int usb_ept_rx_status(USB_Type* hw_base, unsigned int ept_indx)
{
	unsigned int status;

	status = hw_base->DEVICE_EP[ept_indx].USBRXCSRL;

	hw_base->DEVICE_EP[ept_indx].USBRXCSRL = status & ~(USB_USBRXCSRL_STALLED
			| USB_USBRXCSRL_DATAERR | USB_USBRXCSRL_OVER);
    return (status);
}

static unsigned int round_to_fifosize(unsigned int& size)
{
	unsigned int new_size, size_code;

	new_size = 8;
    for(size_code = 0; size_code <= 8; size_code++)
    {
    	if(new_size >= size)
    	{
    		size = new_size;
    		return (size_code);
    	}
    	new_size <<= 1;
    }
	TRACELN_USB("ERROR: invalid endpoint size!");
	size = 64;
	return (3);
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
		drv_data->usb_state = USBST_DEVICE_SUSPENDED;
		break;

	case e_resume:
		if(drv_data->usb_state < USBST_DEVICE_DEFAULT)
			drv_data->usb_state = USBST_DEVICE_DEFAULT;
		break;

	case e_reset:
		drv_data->usb_state = USBST_DEVICE_DEFAULT;
		drv_data->frame_count = 0;
		break;

	default: ;
	break;
	}
}

#if USB_ENABLE_HOST
WEAK_C void usb_host_power(USB_DRV_INFO drv_info, bool enable)
{
	unsigned int reg;

	reg = drv_info->hw_base->USBEPC;
	reg &= ~(USB_USBEPC_EPENDE | USB_USBEPC_EPEN_Msk);
	if(enable)
	{
		TRACE1_USB(" USB pwr en");
		reg |= USB_USBEPC_EPENDE | USB_USBEPC_EPEN_HIGH/*USB_USBEPC_EPEN_VBHIGH*/;
		drv_info->hw_base->USBVDC = USB_USBVDC_VBDEN;
		drv_info->hw_base->USBVDCIM = USB_USBVDCIM_VD;
//		drv_info->hw_base->USBDRIM = USB_USBDRIM_RESUME;
	} else
	{
		TRACE1_USB(" USB pwr dis");
		drv_info->hw_base->USBVDC = 0;
		drv_info->hw_base->USBVDCIM = 0;
//		drv_info->hw_base->USBDRIM = 0;
	}
	drv_info->hw_base->USBEPC = reg;
}

void usb_otg_clr_flags(USB_DRV_INFO drv_info, uint32_t flags)
{
    USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	if(flags & USB_OTG_FLG_HOST)
		flags |= USB_OTG_FLG_HOST_CON | USB_OTG_FLG_HOST_PWR
				| USB_OTG_FLG_HOST_RST | USB_OTG_FLG_HOST_OK;
	if(flags & USB_OTG_FLG_DEV)
		flags |= USB_OTG_FLG_DEV_CON | USB_OTG_FLG_DEV_OK;

	flags &= drv_data->otg_flags;
	drv_data->otg_flags &= ~flags;

	// host power
	if(flags & USB_OTG_FLG_HOST_PWR)
	{
		usb_host_power(drv_info, false);
	}

	// device con
	if(flags & USB_OTG_FLG_DEV_CON)
	{
        // Abort all transfers
    	for(int i= 1; i<USB_NUMENDPOINTS; i++)
    	{
    		usb_hal_ept_reset(drv_info, i);
    	}
	}

	// disconnect
	if(flags & (USB_OTG_FLG_HOST | USB_OTG_FLG_DEV | USB_OTG_FLG_HOST_PWR))
	{
		if(!(drv_data->otg_flags & (USB_OTG_FLG_HOST_OK | USB_OTG_FLG_DEV_OK)))
			drv_data->otg_state_cnt  |= USB_STATE_CNT_INVALID;
		if(drv_info->drv_data->pending)
		{
			HANDLE hnd;
			while( (hnd=drv_data->pending) )
			{
				drv_data->pending = hnd->next;
				usr_HND_SET_STATUS(hnd, FLG_SIGNALED | RES_FATAL);
			}
			atomic_clrex();
		}
	}

}

void usb_otg_set_flags(USB_DRV_INFO drv_info, uint32_t flags)
{
    USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	// do not set twice
	flags &= ~drv_data->otg_flags;
	if(flags)
	{
		if(flags >= USB_OTG_FLG_HOST )
		{
			// host flags
			if(drv_data->otg_flags & USB_OTG_FLG_HOST)
			{
				usb_otg_clr_flags(drv_info, USB_OTG_FLG_DEV);

				switch(flags)
				{
				case USB_OTG_FLG_HOST_PWR:
					usb_host_power(drv_info, true);
					flags |= USB_OTG_FLG_HOST_CON;
					break;

				case USB_OTG_FLG_HOST_RST:
					if(drv_data->helper_task)
					{
						int sig;
						do
						{
							sig = atomic_fetch((volatile int*)&drv_data->otg_h_sig);
							sig |= OTG_H_SIG_RST;
						} while(atomic_store((volatile int*)&drv_data->otg_h_sig, sig));
						usr_send_signal(drv_data->helper_task, USB_DRIVER_SIG);
					}
					break;

				case USB_OTG_FLG_HOST_OK:
					HANDLE hnd;
					__disable_irq();
					while( (hnd=drv_data->pending) )
					{
						drv_data->pending = hnd->next;
						__enable_irq();
						usr_HND_SET_STATUS(hnd, FLG_SIGNALED | RES_FATAL);
						__disable_irq();
					}

					drv_data->otg_state_cnt += drv_data->otg_state_cnt & USB_STATE_CNT_INVALID;
					break;

				}

				drv_info->drv_data->otg_flags |= flags;
				__enable_irq();
			}
		} else
		{
			//device flags
			if(drv_info->drv_data->otg_flags & USB_OTG_FLG_DEV)
			{
				usb_otg_clr_flags(drv_info, USB_OTG_FLG_HOST);

				switch(flags)
				{
				case USB_OTG_FLG_DEV_OK:
					drv_data->otg_state_cnt += drv_data->otg_state_cnt & USB_STATE_CNT_INVALID;
					break;

				case USB_OTG_FLG_DEV_CON:
					drv_info->hw_base->USBPOWER |= USB_USBPOWER_SOFTCONN;
					break;
				}

				if(drv_data->helper_task)
					usr_send_signal(drv_data->helper_task, USB_DRIVER_SIG);
				drv_info->drv_data->otg_flags |= flags;
			}
		}

	}

}

void usb_host_resume(USB_DRV_INFO drv_info)
{
	USB_Type* hw_base = drv_info->hw_base;

	hw_base->USBPOWER |= USB_USBPOWER_RESUME;
	tsk_sleep(20);
	hw_base->USBPOWER &= ~USB_USBPOWER_RESUME;

	hw_base->USBDRISC = USB_USBDRISC_RESUME;
	hw_base->USBDRIM = USB_USBDRIM_RESUME;
}

#endif

/**
 * Called once after boot (on DCR_RESET) to reset the driver
 * @param drv_info
 */
void usb_drv_reset(USB_DRV_INFO drv_info)
{
    TRACELN1_USB("USBD_Init");
	NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);

	drv_info->drv_data->helper_task = usr_task_create_dynamic("USBH",
			(TASK_FUNCTION) usbdrv_thread, 60, 400 + TRACE_SIZE);
	if(drv_info->drv_data->helper_task)
	{
		drv_info->drv_data->helper_task->sp->r0.as_cvoidptr = drv_info;
		svc_task_schedule(drv_info->drv_data->helper_task);
	}
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
    		if(!endpoint->pending && (endpoint->state & ENDPOINT_STATE_SENDING))
    		{
    			endpoint->state &= ~ENDPOINT_STATE_SENDING;
    			if(!endpoint->state)
    				endpoint->state = ENDPOINT_STATE_IDLE;
    		}

    	}
    }
}

/**
 * Start sending...
 *
 * LM3S logic:
 * 	1. Optionally write something in the endpoint FIFO
 * 	2. Set USB_USBTXCSRL0_TXRDY/USB_USBTXCSRL_TXRDY
 * 		for endpoint 0 also set USB_USBTXCSRL0_DATAEND on the last transfer
 * 	3. Core clears TXRDY and sets EPn bit in USBTXIS (endpoint tx interrupt)
 * 		when data is transmitted
 * 	4. In the interrupt when reading USBTXIS it is cleared,
 * 	   then usb_b_ept0_handler() / usb_b_ept_tx_handler() is called
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
    TRACE_USB(" Write%d(%d) ", ept_indx , hnd->len);
	endpoint = &(drv_info->drv_data->endpoints[ept_indx]);
    if( (prev=endpoint->pending) )
    {
    	while(prev->next)
    		prev = prev->next;
    	prev->next = hnd;
    } else
    {
	    if (endpoint->state != ENDPOINT_STATE_IDLE && endpoint->state != ENDPOINT_STATE_RECEIVING_OFF)
	    {
			svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
	    } else
	    {
		    endpoint->state = ENDPOINT_STATE_SENDING;
		    endpoint->pending = hnd;		//sending

#if USB_ENABLE_HOST
		    //host mode?
		    if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_CON)
		    {
		    	usb_remote_device* dev;
		    	USBHOSTFUNS_t* host_ept;

		    	dev = &drv_info->drv_data->host_bus.usb_device[hnd->mode0];
		    	host_ept = &drv_info->hw_base->HOST_EP[ept_indx];

		    	//set host transmit address
		    	host_ept->USBTXFUNCADDR = dev->dev_adress;

		    	//set hub transmit address
		    	host_ept->USBTXHUBADDR = (dev->hub_num << 8) | (dev->hub_port);

		    	//enable TX dir
		    	if(ept_indx)
		    		drv_info->hw_base->DEVICE_EP[ept_indx].USBTXCSRH |= USB_USBTXCSRH_MODE;
		    }
#endif

		    usb_write_payload(ENTPOINT_FIFO(drv_info->hw_base, ept_indx), hnd,
		    					endpoint->txfifo_size);
		    usb_txpktrdy(drv_info->hw_base, ept_indx, hnd->len);
	    }
    }
}

/**
 * Start receiving...
 * LM3S logic:
 * 	1. Core will trigger single interrupt when packet is received
 * 	2. Read entire packet
 * 	3. Acknowledge with xxx_RXRDYC
 *
 * @param drv_info
 * @param hnd
 */
void usb_drv_start_rx(USB_DRV_INFO drv_info, HANDLE hnd)
{
	uint32_t ept_indx;
	Endpoint *endpoint;
	USB_Type* hw_base;
    HANDLE prev;

	ept_indx = hnd->mode.as_bytes[0];
	endpoint = &drv_info->drv_data->endpoints[ept_indx];
	hw_base = drv_info->hw_base;

	if(endpoint->state & ENDPOINT_STATE_RECEIVING_OFF)
	{

		endpoint->rxfifo_cnt = usb_read_payload(ENTPOINT_FIFO(hw_base, ept_indx),
												hnd, endpoint->rxfifo_cnt);
        if (endpoint->rxfifo_cnt == 0)
        {
        	// acknowledge packet;
            if(ept_indx == 0)
            {
                // Clear RxPktRdy, and optionally DataEnd, on endpoint zero.
            	hw_base->DEVICE_EP[0].USBTXCSRL = USB_USBTXCSRL0_RXRDYC | USB_USBTXCSRL0_DATAEND;
            }
            else
            {
                // Clear RxPktRdy on all other endpoints.
            	hw_base->DEVICE_EP[ept_indx].USBRXCSRL &= ~(USB_USBRXCSRL_RXRDY);
            }
            endpoint->state &= ~ENDPOINT_STATE_RECEIVING_OFF;
            if(!endpoint->state)
            	endpoint->state = ENDPOINT_STATE_IDLE;
        }
    	svc_HND_SET_STATUS(hnd, RES_SIG_OK);
    	return;
	}

#if USB_ENABLE_HOST
	//host mode?
	if (drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_CON)
	{
    	usb_remote_device* dev;
    	USBHOSTFUNS_t* host_ept;

    	dev = &drv_info->drv_data->host_bus.usb_device[hnd->mode0];
    	host_ept = &drv_info->hw_base->HOST_EP[ept_indx];

    	//set host receive address
    	host_ept->USBRXFUNCADDR = dev->dev_adress;

    	//set hub receive address
    	host_ept->USBRXHUBADDR = (dev->hub_num << 8) | (dev->hub_port);

	    if( (prev=endpoint->pending) )	//receiving
	    {
	    	while(prev->next)
	    		prev = prev->next;
	    	prev->next = hnd;
	    } else
	    {
	    	endpoint->pending = hnd;	//receiving
			// request IN packet
	    	if(ept_indx)
	    	{
	        	drv_info->hw_base->DEVICE_EP[ept_indx].USBTXCSRH &= ~USB_USBTXCSRH_MODE;
				hw_base->DEVICE_EP[ept_indx].USBRXCSRL = USB_USBRXCSRL_REQPKT;
	    	}
	    	else
	    		hw_base->DEVICE_EP[ept_indx].USBTXCSRL = USB_USBRXCSRL_REQPKT;
	    }
		return;
	}
#endif
	if(hnd->len)
	{
	    if( (prev=endpoint->pending) )	//receiving
	    {
	    	while(prev->next)
	    		prev = prev->next;
	    	prev->next = hnd;
	    } else
	    {
	    	endpoint->pending = hnd;	//receiving
	    }

	} else
	{
    	svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}


/**
 * Cancel pending all handles for given endpoint
 * called on bus reset and endpoint halt
 * @param endpoint
 * @param status
 */
void usb_drv_end_transfers(Endpoint *endpoint, unsigned int status)
{
    HANDLE hnd;

	while( (hnd=endpoint->pending) )
	{
		TRACE1_USB(" EoT ");
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


void usb_hal_stall(USB_Type* hw_base, unsigned int ept_num)
{
	uint32_t ept_indx = ept_num & 0xF;

    if (ept_indx == EPT_0)
	{
		hw_base->DEVICE_EP[0].USBTXCSRL = USB_USBTXCSRL0_STALL;
	}
	else
	{
		if (ept_num & 0x80)
		{
			hw_base->DEVICE_EP[ept_indx].USBTXCSRL = USB_USBTXCSRL_STALL;
		}
		else
		{
			hw_base->DEVICE_EP[ept_indx].USBRXCSRL = USB_USBRXCSRL_STALL;
		}
	}
}

void usb_hal_stall_clear(USB_Type* hw_base, unsigned int ept_num)
{
	uint32_t ept_indx = ept_num & 0xF;

    if (ept_indx == EPT_0)
	{
		hw_base->DEVICE_EP[0].USBTXCSRL &= ~USB_USBTXCSRL0_STALLED;
	}
	else
	{
		if (ept_num & 0x80)
		{
	        // Clear the stall on an IN endpoint.
	        hw_base->DEVICE_EP[ept_indx].USBTXCSRL &=
		            ~(USB_USBTXCSRL_STALL | USB_USBTXCSRL_STALLED);

	        // Reset the data toggle.
	        hw_base->DEVICE_EP[ept_indx].USBTXCSRL |= USB_USBTXCSRL_CLRDT;

		}
		else
		{
	        // Clear the stall on an OUT endpoint.
			hw_base->DEVICE_EP[ept_indx].USBRXCSRL &=
	            ~(USB_USBRXCSRL_STALL | USB_USBRXCSRL_STALLED);

	        // Reset the data toggle.
			hw_base->DEVICE_EP[ept_indx].USBRXCSRL |= USB_USBRXCSRL_CLRDT;
		}
	}
}

static void usb_hal_start(USB_DRV_INFO drv_info, unsigned int ie)
{
	USB_Type* hw_base = drv_info->hw_base;

    // Enable Clocking to the USB controller.
	SysCtlPeripheralEnable(drv_info->info.peripheral_indx);
	SysCtlPeripheralReset(drv_info->info.peripheral_indx);

	// Enable USB related pins
	PIO_Cfg_List(drv_info->cfg->usb_pins);

    // Turn on USB Phy clock.
    SYSCTL->RCC2 &= ~SYSCTL_RCC2_USBPWRDN;


    // Clear interrupt statuses
    hw_base->USBIS;
    hw_base->USBTXIS;
    hw_base->USBRXIS;

	// Enable USB Interrupts.
    hw_base->USBIE = ie;

	hw_base->USBIDVIM = USB_USBIDVIM_ID;
	hw_base->USBTXIE = USB_USBTXIE_EP0;
	hw_base->USBRXIE = 0;

	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);
}

/** Configure as device
 *
 * @param drv_info
 */
void usb_hal_device_start(USB_DRV_INFO drv_info)
{
	usb_hal_start(drv_info, USB_USBIE_SESREQ
			| USB_USBIE_DISCON | USB_USBIE_CONN | USB_USBIE_RESET
			| USB_USBIE_RESUME | USB_USBIE_SUSPND);


	drv_info->hw_base->USBPOWER |= USB_USBPOWER_SOFTCONN;
}

#if USB_ENABLE_HOST
void usb_hal_host_start(USB_DRV_INFO drv_info)
{
	usb_hal_start(drv_info, USB_USBIE_VBUSERR | USB_USBIE_SESREQ
			| USB_USBIE_DISCON | USB_USBIE_CONN | USB_USBIE_RESET
			| USB_USBIE_RESUME | USB_USBIE_SUSPND);

	USB_Type* hw_base = drv_info->hw_base;


	// reset devices
	for(int i=0; i<= MAX_USB_DEVICES; i++)
	{
		usb_remote_device* dev;

		dev = &drv_info->drv_data->host_bus.usb_device[i];
		dev->dev_adress = 0;
		dev->dev_interface = 0;
		if(dev->config_descriptor)
		{
			tsk_free(dev->config_descriptor);
			dev->config_descriptor = NULL;
		}
		dev->dev_descriptor.bMaxPacketSize0 = 0;
		dev->dev_descriptor.as_generic.bLength = 0;
	}

	//configure endpoint 0
    Endpoint *pEndpoint;
    pEndpoint = &drv_info->drv_data->endpoints[0];
	usb_drv_end_transfers(pEndpoint, USBD_STATUS_RESET);
    pEndpoint->state = ENDPOINT_STATE_IDLE;
    pEndpoint->txfifo_size = 64;
	hw_base->DEVICE_EP[EPT_0].USBTXTYPE = USB_USBTXTYPE_SPEED_FULL;

	//enable all TX interrupts
	hw_base->USBTXIE = USB_USBTXIE_ALL;

	//enable receive interrupts
	hw_base->USBRXIE = USB_USBRXIE_ALL;

	// request session now
	hw_base->USBDEVCTL |= USB_USBDEVCTL_SESSION;
	TRACELN1_USB("USB sh ses");

}
#endif

void usb_hal_ept_reset(USB_DRV_INFO drv_info, unsigned int eptnum)
{
	USB_Type* hw_base = drv_info->hw_base;
	unsigned int mask;

	mask = ~(1 << eptnum);
	if(eptnum == 0)
	{
		// - clear ept0 status and flush
		if( usb_ept0_status(hw_base) & USB_USBTXCSRL0_RXRDY)
			hw_base->DEVICE_EP[0].USBTXCSRH = USB_USBTXCSRH0_FLUSH;

	} else
	{
		// - disable interrupts
		hw_base->USBRXIE &= mask;
		hw_base->USBTXIE &= mask;

		// - clear ept1-15 status and flush
		if( usb_ept_rx_status(hw_base, eptnum) & USB_USBRXCSRL_RXRDY)
		{
			hw_base->DEVICE_EP[eptnum].USBRXCSRL |= USB_USBRXCSRL_FLUSH;
		}

		if (usb_ept_tx_status(hw_base, eptnum) & USB_USBTXCSRL_TXRDY)
		{
			hw_base->DEVICE_EP[eptnum].USBTXCSRL |= USB_USBTXCSRL_FLUSH;
		}
	}

    Endpoint *pEndpoint;
    pEndpoint = &drv_info->drv_data->endpoints[eptnum];
	usb_drv_end_transfers(pEndpoint, USBD_STATUS_RESET);
    pEndpoint->state = ENDPOINT_STATE_DISABLED;
}

/**
 * Configures an endpoint according to its Endpoint Descriptor.
 * \param pDescriptor Pointer to an Endpoint descriptor.
 */
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor, unsigned int rindx)
{
    Endpoint *pEndpoint;
    unsigned int eptnum, reg;
    EndpointAttribute_TransferType ept_type;
    EndpointDirection ept_dir = ENDPOINT_DIRECTION_OUT;
    USB_Type* hw_base = drv_info->hw_base;

	switch(pDescriptor->bDescriptorType)
	{
	case DEVICE_DESCRIPTOR:
        eptnum = 0;
        pEndpoint = &drv_info->drv_data->endpoints[eptnum];
        pEndpoint->txfifo_size = ((USBDeviceDescriptor *)pDescriptor)->bMaxPacketSize0;
        break;

	case ENDPOINT_DESCRIPTOR:
		const USBEndpointDescriptor* descriptor;
		descriptor = (const USBEndpointDescriptor*)pDescriptor;

		eptnum = descriptor->GetNumber();
		ept_type = descriptor->GetType();
		ept_dir = descriptor->GetDirection();
	    pEndpoint = &drv_info->drv_data->endpoints[eptnum];
        if(ept_dir == ENDPOINT_DIRECTION_IN)
        {
            pEndpoint->txfifo_size = descriptor->GetMaxPacketSize();

        } else
        {
            pEndpoint->rxfifo_size = descriptor->GetMaxPacketSize();
        }
		break;

	default:
		TRACELN1_USB("Invalid descriptor!");
		return;
	}


    /* Abort the current transfer is the endpoint was configured and in
	 Write or Read state */
	usb_hal_ept_reset(drv_info, eptnum);
    pEndpoint->state = ENDPOINT_STATE_IDLE;


    /* Configure endpoint */
    if(eptnum >= USB_NUMENDPOINTS)
    {
		TRACELN1_USB("Invalid endpoint descriptor!");
    } else if(eptnum == 0)
    {
    	//Endpoint 0 has nothing to configure, only to enable interrupts
		hw_base->USBTXIE |= USB_USBTXIE_EP0;

    } else
    {
        // Enable isochronous mode if requested.
        if(ept_type == ENDPOINT_TYPE_ISOCHRONOUS)
        {
            reg = USB_USBTXCSRH_ISO;
        } else
            reg = 0;

        // Determine if a transmit or receive endpoint is being configured.
        //
        if(ept_dir == ENDPOINT_DIRECTION_IN)
        {
#if USB_ENABLE_HOST
        	if(rindx)
        		hw_base->DEVICE_EP[eptnum].USBTXTYPE = (ept_type << 4) | rindx;
#endif

            // Set the maximum packet size.
            hw_base->DEVICE_EP[eptnum].USBTXMAXP = pEndpoint->txfifo_size;

            // Write the transmit control value.
            hw_base->DEVICE_EP[eptnum].USBTXCSRH = reg;

            // Reset the Data toggle to zero.
            hw_base->DEVICE_EP[eptnum].USBTXCSRL = USB_USBTXCSRL_CLRDT;

    		hw_base->USBTXIE |= 1 << eptnum;

        }
        else
        {
#if USB_ENABLE_HOST
        	if(rindx)
        		hw_base->DEVICE_EP[eptnum].USBRXTYPE = (ept_type << 4) | rindx;
#endif
            // Set the MaxPacketSize.
            hw_base->DEVICE_EP[eptnum].USBRXMAXP = pEndpoint->rxfifo_size;

            // Write the receive control value.
            hw_base->DEVICE_EP[eptnum].USBRXCSRH = reg;

            // Reset the Data toggle to zero.
            hw_base->DEVICE_EP[eptnum].USBRXCSRL = USB_USBRXCSRL_CLRDT;

    		hw_base->USBRXIE |= 1 << eptnum;

        }
    }

    TRACE_USB(" CfgEp(%x)", eptnum + (ept_dir <<7));
}

void usb_hal_config_fifo(USB_DRV_INFO drv_info)
{
	unsigned int size, limit, address, size_code;
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	USB_Type* hw_base = drv_info->hw_base;

	TRACELN1_USB("USB FIFOS:");

	// Loop and try the best limit
	limit = 1024;
	do
	{
		//ept1 starts at 64
		address = 64;
		for(int eptnum=1; eptnum<USB_NUMENDPOINTS; eptnum++)
		{
			size = drv_data->endpoints[eptnum].rxfifo_size;
			if(size)
			{
				size_code = round_to_fifosize(size);

				if((size + address)> 4095)
				{
					address += size;
					break;
				}
				TRACE_USB(" ept%d [rx %d]=", eptnum, address);
			    // Set the index.
				hw_base->USBEPIDX = eptnum;
				// set the address
				hw_base->USBRXFIFOADD = address >> 3;
				address += size;
				//set the size
				if((size > limit) && ((size + address) > 4095))
				{
					hw_base->USBRXFIFOSZ = size_code;
					TRACE_USB(" %d", size);
				} else
				{
					hw_base->USBRXFIFOSZ = USB_USBRXFIFOSZ_DPB | size_code;
					address += size;
					hw_base->USBRXDPKTBUFDIS |= (1<<eptnum);
					TRACE_USB(" 2*%d", size);
				}

			}
			size = drv_data->endpoints[eptnum].txfifo_size;
			if(size)
			{
				size_code = round_to_fifosize(size);

				if((size + address)> 4095)
				{
					address += size;
					break;
				}
				TRACE_USB(" ept%d [tx %d]=", eptnum, address);
			    // Set the index.
				hw_base->USBEPIDX = eptnum;
				// set the address
				hw_base->USBTXFIFOADD = address >> 3;
				address += size;
				//set the size
				if((size > limit) && ((size + address) > 4095))
				{
					hw_base->USBTXFIFOSZ = size_code;
					TRACE_USB(" %d", size);
				} else
				{
					hw_base->USBTXFIFOSZ = USB_USBTXFIFOSZ_DPB | size_code;
					address += size;
					hw_base->USBTXDPKTBUFDIS |= (1<<eptnum);
					TRACE_USB(" 2*%d", size);
				}

			}
		}
		limit >>= 1;
	} while ((limit >= 16) && (address > 4095));

}

/** Handle USB bus reset
 *
 * @param drv_info
 */
static void usb_handle_bus_reset(USB_DRV_INFO drv_info)
{
	const USBDDriverDescriptors * dev_descriptors;
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	//Step 1. enable clocks etc.

	//Step 2. reset endpoints
	for(int i= 0; i<USB_NUMENDPOINTS; i++)
	{
		usb_hal_ept_reset(drv_info, i);
	}

	dev_descriptors = drv_data->device.pDescriptors;

	if(dev_descriptors)
	{
		usb_hal_ept_config(drv_info, &dev_descriptors->pFsDevice->as_generic);
	}

#if USB_ENABLE_OTG
	//Step 3. Wakeup clients..
	usb_otg_set_flags(drv_info, USB_OTG_FLG_DEV_OK);
	if(drv_info->drv_data->pending)
	{
		HANDLE hnd;
		while( (hnd=drv_data->pending) )
		{
			drv_data->pending = hnd->next;
			usr_HND_SET_STATUS(hnd, RES_SIG_IDLE);
		}
		atomic_clrex();
	}
#endif

}

void usb_b_ept0_handler(USB_DRV_INFO drv_info)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	USB_Type* hw_base = drv_info->hw_base;
    Endpoint *endpoint;
    unsigned int status;
    HANDLE hnd;

    status = usb_ept0_status(hw_base);
    TRACE_USB("%02x]", status);
    endpoint = &drv_data->endpoints[0];

	if (status & USB_USBTXCSRL0_STALLED)
	{
		if(endpoint->state == ENDPOINT_STATE_STALL)
		{
	    	endpoint->state = ENDPOINT_STATE_IDLE;
		} else
		{
	    	TRACE_USB(" Stl0 state=%d", endpoint->state);
		}
	}
#if USB_ENABLE_HOST
	if (status & USB_USBTXCSRL0_ERROR)
	{
	    //host mode?
	    if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST)
	    {
			while((hnd=endpoint->pending) )
			{
		   		// End of transfer ?
		    	TRACE1_USB(" err!");
				endpoint->pending = hnd->next;
				usr_HND_SET_STATUS(hnd, RES_SIG_ERROR);
			}
			endpoint->state = ENDPOINT_STATE_IDLE;
	    }
	}
#endif

	if ( (endpoint->state == ENDPOINT_STATE_SENDING)  )
	{
		//check if we have pending write
		while((hnd=endpoint->pending) && ( hnd->len == 0) && !(hnd->res & FLG_EOF))
		{
	   		// End of transfer ?
	    	TRACE1_USB(" Wr!");
			endpoint->pending = hnd->next;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
		}

		//check if we have more to write
		if(hnd)
		{
			if( !(status & USB_USBTXCSRL0_TXRDY) )
			{
	            // Send next packet
			    usb_write_payload( ENTPOINT_FIFO(hw_base, 0), hnd, endpoint->txfifo_size);
			    usb_txpktrdy(drv_info->hw_base, 0, hnd->len);
			}
		} else
		{
			endpoint->state = ENDPOINT_STATE_IDLE;
		}

	}

	if (status & USB_USBTXCSRL0_RXRDY)
	{
		// there is a packet

		if(endpoint->state == ENDPOINT_STATE_IDLE)
		{
			unsigned int size;
			size = hw_base->DEVICE_EP[0].USBRXCOUNT;
	    	TRACE_USB(" Rd0 %d", size);

	    	while(size && (hnd = endpoint->pending))
	    	{
	    		endpoint->pending = hnd->next;
	    		size = usb_read_payload(ENTPOINT_FIFO(hw_base, 0), hnd, size);
				usr_HND_SET_STATUS(hnd, RES_SIG_OK);
	    	}
	    	if(size)
			{
	            // mark that we have data
	        	endpoint->state = ENDPOINT_STATE_RECEIVING_OFF;
	    		endpoint->rxfifo_cnt = size;
	        } else
	        {
#if USB_ENABLE_HOST
				//host mode?
				if (drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_CON)
				{
			    	if((hnd = endpoint->pending) && !hnd->len)
			    	{
			    		hnd->mode1 = 0;
			    		endpoint->pending = hnd->next;
			    	    TRACE_USB("(%d)", size);
						usr_HND_SET_STATUS(hnd, RES_SIG_OK);
			    	}
				}
#endif
	        	hw_base->DEVICE_EP[0].USBTXCSRL = USB_USBTXCSRL0_RXRDYC ;
	        }
		} else
		{
	    	TRACE_USB(" error state=%d", endpoint->state);
		}
	}
}

void usb_b_ept_rx_handler(USB_DRV_INFO drv_info, unsigned int eptnum)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	USB_Type* hw_base = drv_info->hw_base;
    Endpoint *endpoint;
    unsigned int status;
    HANDLE hnd;

    status = usb_ept_rx_status(hw_base, eptnum);
    TRACE_USB("%02x]", status);
    endpoint = &drv_data->endpoints[eptnum];

	if (status & USB_USBRXCSRL_DATAERR)
	{
    	TRACE_USB(" DATAERR state=%d", endpoint->state);
	}

	if (status & USB_USBRXCSRL_OVER)
	{
#if USB_ENABLE_HOST
		if( drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_CON)
		{
			status |= USB_USBRXCSRL_RXRDY;
	    	TRACE1_USB(" RXERR");
		}
		else
#endif
    	TRACE1_USB(" OVER");
	}

	if (status & USB_USBRXCSRL_STALLED)
	{
		if(endpoint->state == ENDPOINT_STATE_STALL)
		{
	    	endpoint->state = ENDPOINT_STATE_IDLE;
		} else
		{
	    	TRACE_USB(" Stl state=%d", endpoint->state);
		}
	}


	if (status & USB_USBRXCSRL_RXRDY)
	{
		// there is a packet

#if USB_ENABLE_HOST
		//host mode?
		if( (endpoint->state == ENDPOINT_STATE_IDLE) ||
				(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_CON) )
#else
		if(endpoint->state == ENDPOINT_STATE_IDLE)
#endif
		{
			unsigned int size;
			size = hw_base->DEVICE_EP[eptnum].USBRXCOUNT;
	    	TRACE_USB(" Rd %d", size);

	    	while(size && (hnd = endpoint->pending))
	    	{
	    		endpoint->pending = hnd->next;
	    		size = usb_read_payload(ENTPOINT_FIFO(hw_base, eptnum), hnd, size);
				usr_HND_SET_STATUS(hnd, RES_SIG_OK);
	    	}
	    	if(size)
			{
	            // mark that we have data
	    		endpoint->state &= ~ENDPOINT_STATE_IDLE;
	        	endpoint->state |= ENDPOINT_STATE_RECEIVING_OFF;
	    		endpoint->rxfifo_cnt = size;
	        } else
	        {
	        	status &= ~(USB_USBRXCSRL_STALLED | USB_USBRXCSRL_DATAERR
						| USB_USBRXCSRL_OVER | USB_USBRXCSRL_RXRDY);
	        	hw_base->DEVICE_EP[eptnum].USBRXCSRL = status;
	        }
		} else
		{
	    	TRACE_USB(" error state=%d", endpoint->state);
		}
	}
}

void usb_b_ept_tx_handler(USB_DRV_INFO drv_info, unsigned int eptnum)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	USB_Type* hw_base = drv_info->hw_base;
    Endpoint *endpoint;
    unsigned int status;
    HANDLE hnd;

    status = usb_ept_tx_status(hw_base, eptnum);
    TRACE_USB("%02x]", status);
    endpoint = &drv_data->endpoints[eptnum];

	if (status & USB_USBTXCSRL_STALLED)
	{
		if(endpoint->state == ENDPOINT_STATE_STALL)
		{
	    	endpoint->state = ENDPOINT_STATE_IDLE;
		} else
		{
	    	TRACE_USB(" Stl state=%d", endpoint->state);
		}
	}

	if (status & USB_USBTXCSRL_UNDRN)
	{
    	TRACE1_USB(" UNDRN");
	}

	if ( (endpoint->state & ENDPOINT_STATE_SENDING)  )
	{
		//check if we have pending write
		while((hnd=endpoint->pending) && ( hnd->len == 0) && !(hnd->res & FLG_EOF))
		{
	   		// End of transfer ?
	    	TRACE1_USB(" Wr!");
			endpoint->pending = hnd->next;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
		}

		//check if we have more to write
		if(hnd)
		{
			if( !(status & USB_USBTXCSRL_TXRDY) )
			{
	            // Send next packet
			    usb_write_payload( ENTPOINT_FIFO(hw_base, eptnum), hnd, endpoint->txfifo_size);
			    hw_base->DEVICE_EP[eptnum].USBTXCSRL = USB_USBTXCSRL_TXRDY;
			}
		} else
		{
			endpoint->state &= ~ENDPOINT_STATE_SENDING;
			if(!endpoint->state)
				endpoint->state = ENDPOINT_STATE_IDLE;
		}

	}
}

#ifdef USB_A_ENABLED
static void usb_a_usbis_handler(USB_DRV_INFO drv_info, unsigned int status)
{
    //
    // If device initialization has not been performed then just disconnect
    // from the USB bus and return from the handler.
    //
    if(g_psUSBDevice[0].psInfo == 0)
    {
        USBDevDisconnect(USB0_BASE);
    } else
    {
        //
        // Received a reset from the host.
        //
        if(status & USB_USBIS_RESET)
        {
            USBDeviceEnumResetHandler(&g_psUSBDevice[0]);
        }

        //
        // Suspend was signaled on the bus.
        //
        if(status & USB_USBIS_SUSPEND)
        {
            //
            // Call the SuspendHandler() if it was specified.
            //
            if(psInfo->sCallbacks.pfnSuspendHandler)
            {
                psInfo->sCallbacks.pfnSuspendHandler(pvInstance);
            }
        }

        //
        // Resume was signaled on the bus.
        //
        if(status & USB_USBIS_RESUME)
        {
            //
            // Call the ResumeHandler() if it was specified.
            //
            if(psInfo->sCallbacks.pfnResumeHandler)
            {
                psInfo->sCallbacks.pfnResumeHandler(pvInstance);
            }
        }

        //
        // USB device was disconnected.
        //
        if(status & USB_USBIS_DISCON)
        {
            //
            // Call the DisconnectHandler() if it was specified.
            //
            if(psInfo->sCallbacks.pfnDisconnectHandler)
            {
                psInfo->sCallbacks.pfnDisconnectHandler(pvInstance);
            }
        }

        //
        // Start of Frame was received.
        //
        if(status & USB_USBIS_SOF)
        {
            //
            // Increment the global Start of Frame counter.
            //
            g_ulUSBSOFCount++;

            //
            // Increment our SOF divider.
            //
            ulSOFDivide++;

            //
            // Handle resume signaling if required.
            //
            USBDeviceResumeTickHandler(0);

            //
            // Have we counted enough SOFs to allow us to call the tick function?
            //
            if(ulSOFDivide == USB_SOF_TICK_DIVIDE)
            {
                //
                // Yes - reset the divider and call the SOF tick handler.
                //
                ulSOFDivide = 0;
                InternalUSBStartOfFrameTick(USB_SOF_TICK_DIVIDE);
            }
        }
    }

}
#endif /*USB_A_ENABLED*/

static void usb_b_usbis_handler(USB_DRV_INFO drv_info, unsigned int status)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
    //
    // If device initialization has not been performed then just disconnect
    // from the USB bus and return from the handler.
    //
    if(drv_data->usb_state == USBST_ALL_DOWN)
    {
    	//Disconnect
		TRACELN1_USB("USB: session end");
    	drv_info->hw_base->USBPOWER &= ~(USB_USBPOWER_SOFTCONN);
    } else
    {
        // Received a reset from the host.
        if(status & USB_USBIS_RESET)
        {
    		TRACE1_USB(" reset");
        	usb_drv_event(drv_info, e_reset);
        	usb_handle_bus_reset(drv_info);
        }

        // Suspend was signaled on the bus.
        if(status & USB_USBIS_SUSPEND)
        {
    		TRACE1_USB(" suspend");
        	usb_drv_event(drv_info, e_susppend);

        }

        // Resume was signaled on the bus.
        if(status & USB_USBIS_RESUME)
        {
    		TRACE1_USB(" usb resume");
        	usb_drv_event(drv_info, e_resume);
        }

        // USB device was disconnected.
        if(status & USB_USBIS_DISCON)
        {
    		TRACE1_USB(" usb disconnect");
        	usb_drv_event(drv_info, e_disconnect);
        }

        //
        // Start of Frame was received.
        //
        if(status & USB_USBIS_SOF)
        {
        }
    }

}

#ifdef USB_A_ENABLED
void usb_a_int_handler(USB_DRV_INFO drv_info, unsigned int status)
{
    static unsigned long ulSOFDivide = 0;
    tDeviceInfo *psInfo;
    void *pvInstance;


    psInfo = g_psUSBDevice[0].psInfo;
    pvInstance = g_psUSBDevice[0].pvInstance;


    //
    // Get the controller interrupt status.
    //
    status = USBIntStatusEndpoint(USB0_BASE);

    //
    // Handle end point 0 interrupts.
    //
    if(status & USB_INTEP_0)
    {
        USBDeviceEnumHandler(&g_psUSBDevice[0]);
    }

    //
    // Because there is no way to detect if a uDMA interrupt has occurred,
    // check for an endpoint callback and call it if it is available.
    //
    if(psInfo->sCallbacks.pfnEndpointHandler)
    {
        psInfo->sCallbacks.pfnEndpointHandler(pvInstance, status);
    }

}
#endif /*USB_A_ENABLED*/

#ifdef USB_A_ENABLED
void USB_A_ISR(USB_DRV_INFO drv_info)
{
	unsigned int status;
	USB_Type* hw_base = drv_info->hw_base;

    //----- Process USBIS interrupts
    status = hw_base->USBIS;
    if(status)
    {
    	usb_b_usbis_handler(drv_info, status);
    }

    //----- Process USBEPCISC interrupt
    status = hw_base->USBEPCISC & USB_USBEPCISC_PF;
    if(status)
    {
    	//Power Fault status has been detected
    	hw_base->USBEPCISC = status;

        //
        // Indicate that a power fault has occurred.
        //
        g_ulUSBHIntEvents |= INT_EVENT_POWER_FAULT;

        //
        // Turn off power to the bus.
        //
        USBHostPwrDisable(USB0_BASE);

        //
        // Disable USB interrupts.
        //
        IntDisable(INT_USB0);

        return;
    }

    //----- Process USBIDVISC interrupt
    status = hw_base->USBIDVISC & USB_USBIDVISC_ID;
    if(status)
    {
    	//ID Valid Detect Interrupt
    	hw_base->USBIDVISC = status;
    	status |= USB_INTSTATUS_MODE_DETECT;
    }


    //process service statuses first
    if (status & )
    usb_b_svcint_handler(drv_info, status);
}
#endif /*USB_A_ENABLED*/

void USB_ISR(USB_DRV_INFO drv_info)
{
	unsigned int status, eptnum;
	USB_Type* hw_base = drv_info->hw_base;
	bool irq_found = false;


    //----- Process USBIDVISC interrupt
    status = hw_base->USBIDVISC & USB_USBIDVISC_ID;
    if(status)
    {
    	irq_found = true;
    	//ID Valid Detect Interrupt
    	hw_base->USBIDVISC = status;
    	status = hw_base->USBDEVCTL & (USB_USBDEVCTL_DEV | USB_USBDEVCTL_HOST
				| USB_USBDEVCTL_SESSION | USB_USBDEVCTL_VBUS_Msk);
    	TRACE_USB("\r\nUSB:[id=%02x]", status);
#if USB_ENABLE_HOST

		switch (status)
		{
		case USB_USBDEVCTL_SESSION:								//host
		case USB_USBDEVCTL_SESSION | USB_USBDEVCTL_HOST:
		case USB_USBDEVCTL_SESSION | USB_USBDEVCTL_VBUS_AVALID:
			//enable usb power
			usb_otg_set_flags(drv_info, USB_OTG_FLG_HOST_PWR);
			if (drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST)
			{
				hw_base->USBDEVCTL &= ~USB_USBDEVCTL_SESSION;
				hw_base->USBDEVCTL |= USB_USBDEVCTL_SESSION;
				TRACE1_USB(" ses");
			}
//			drv_info->hw_base->USBEPC |= USB_USBEPC_EPEN_VBHIGH;
			break;

		case USB_USBDEVCTL_SESSION | USB_USBDEVCTL_VBUS_VALID:	// host, vbus valid
			if(drv_info->drv_data->helper_task)
			{
				int sig;
				do
				{
					sig = atomic_fetch((volatile int*)&drv_info->drv_data->otg_h_sig);
					sig |= OTG_H_SIG_CON;
				} while(atomic_store((volatile int*)&drv_info->drv_data->otg_h_sig, sig));
				usr_send_signal(drv_info->drv_data->helper_task, USB_DRIVER_SIG);
			}
			break;

		// device mode
		case USB_USBDEVCTL_SESSION | USB_USBDEVCTL_DEV | USB_USBDEVCTL_VBUS_VALID:
			usb_otg_set_flags(drv_info, USB_OTG_FLG_DEV_CON);
			break;

		case USB_USBDEVCTL_SESSION | USB_USBDEVCTL_VBUS_SEND:
			TRACE1_USB(" (send < vbus < valid)");
			hw_base->USBDEVCTL &= ~USB_USBDEVCTL_SESSION;
			hw_base->USBDEVCTL |= USB_USBDEVCTL_SESSION;
			TRACE1_USB(" ses");
			break;

		// disconnect
		default:
			usb_otg_clr_flags(drv_info, USB_OTG_FLG_ALL);
			break;
		}
#endif
    }


    //Process VBUS Droop
#if USB_ENABLE_HOST
    status = hw_base->USBVDCISC;
	if(status)
	{
    	irq_found = true;
		hw_base->USBVDCISC = status;
		TRACE1_USB(" vdc");
		// disconnect
		usb_otg_clr_flags(drv_info, USB_OTG_FLG_ALL);
	}
#endif

    //----- Process USBIS interrupts
    status = hw_base->USBIS;
    if(status & USB_USBIS_SOF)
    {
    	irq_found = true;
    	status &=  ~USB_USBIS_SOF;
    }
    if(status)
    {
    	irq_found = true;
   		TRACELN_USB("USB:[is=%02x]", status);

#if USB_ENABLE_HOST
    	// VBUS error
    	if(status & USB_USBIS_VBUSERR )
    	{
       		TRACE_USB(" droop");
			if (drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST)
       		{
				hw_base->USBDEVCTL |= USB_USBDEVCTL_SESSION;
				TRACE1_USB(" ses");
       		}
    	}

    	// disconnect
    	if(status & USB_USBIS_DISCON)
    	{
    		TRACE1_USB(" DISCON");
			usb_otg_clr_flags(drv_info, USB_OTG_FLG_ALL);
    	}

		//connect
		if(status & USB_USBIS_CONN)
		{
    		TRACE1_USB(" CON");
			usb_otg_set_flags(drv_info, USB_OTG_FLG_HOST_RST);
		}
		status &=  ~(USB_USBIS_VBUSERR | USB_USBIS_DISCON | USB_USBIS_CONN);


		if(status == USB_USBIS_SUSPEND)
		{
			if(drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_PWR)
			{
	    		TRACE1_USB(" discon");
				usb_otg_clr_flags(drv_info, USB_OTG_FLG_HOST);

			}
			if( !(drv_info->drv_data->otg_flags & USB_OTG_FLG_DEV_CON) )
				status = 0;
		}


    	// Bus reset
    	if(status & USB_USBIS_RESET)
    	{
    		uint32_t babble;

    		babble = drv_info->drv_data->otg_flags & USB_OTG_FLG_HOST_CON;

    		// reset, we must switch to otg device
			usb_otg_clr_flags(drv_info, USB_OTG_FLG_HOST);
			if(babble)
				status = 0; // the only way to recover is to close and reopen the session
			else
				usb_otg_set_flags(drv_info, USB_OTG_FLG_DEV_CON);
    	}

    	if(status)
#endif
    	usb_b_usbis_handler(drv_info, status);
    }

    //----- Process endpoint transmit interrupts
    status = hw_base->USBTXIS;
    if(status & USB_USBTXIS_EP0)
    {
    	irq_found = true;
    	TRACELN1_USB("USB:[E0=");
    	usb_b_ept0_handler(drv_info);
    }
	eptnum=1;
    while( (status>>=1) )
    {
		if(status & 1)
		{
	    	irq_found = true;
	    	TRACELN_USB("USB:[T%x=", eptnum);
   			usb_b_ept_tx_handler(drv_info, eptnum);
		}
		eptnum++;
    }

    //----- Process endpoint receive interrupts
    status = hw_base->USBRXIS;
    if(status)
    {
    	irq_found = true;
    	eptnum=0;
    	do
    	{
    		if(status & 1)
    		{
    	    	TRACELN_USB("USB:[R%x=", eptnum);
    			usb_b_ept_rx_handler(drv_info, eptnum);
    		}
    		eptnum++;
    		status >>= 1;
    	} while(status);
    }

    //----- Process USBEPCISC interrupt
    status = hw_base->USBEPCISC & USB_USBEPCISC_PF;
    if(status)
    {
    	irq_found = true;
    	//Power Fault status has been detected
    	hw_base->USBEPCISC = status;

    	TRACELN_USB("USB:[pc=%02x]", status);
    }

    //----- Process USBDRISC interrupt
//    status = hw_base->USBDRISC & USB_USBDRISC_RESUME;
//    if(status)
//    {
//    	irq_found = true;
//    	//Power Fault status has been detected
//		hw_base->USBDRIM = 0;
//    	hw_base->USBDRISC = status;
//#if USB_ENABLE_OTG
//
//		if(drv_info->drv_data->helper_task)
//		{
//			int sig;
//			do
//			{
//				sig = atomic_fetch((volatile int*)&drv_info->drv_data->otg_h_sig);
//				sig |= OTG_H_SIG_RESUME;
//			} while(atomic_store((volatile int*)&drv_info->drv_data->otg_h_sig, sig));
//			usr_send_signal(drv_info->drv_data->helper_task, USB_DRIVER_SIG);
//		}
//#endif
//    	TRACELN_USB("USB:[rm=%02x]", status);
//    }

    if (!irq_found)
    	TRACELN1_USB("USB: dummy ISR");


}

