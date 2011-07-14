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

void usb_write_payload(volatile void* dst, HANDLE hnd,	unsigned int size)
{
    while(1)
    {
	    if (size > hnd->len)
	    {
	    	size -= hnd->len;
		    // Write packet in the FIFO buffer
		    while(hnd->len > 3)
		    {
		    	hnd->len-=4;
		    	*(volatile int*)dst = *hnd->src.as_intptr++;
		    }
		    while(hnd->len)
		    {
		    	hnd->len--;
		    	*(volatile char*)dst = *hnd->src.as_byteptr++;
		    }
		   	hnd = hnd->next;
		   	if(!hnd)
		   		return;
	    }
	    else
	    {
		    hnd->len -= size;
		    while(size>3)
		    {
		    	size -=4;
		    	*(volatile int*)dst = *hnd->src.as_intptr++;
		    }
		    while(size--)
		    	*(volatile char*)dst = *hnd->src.as_byteptr++;
	   		return;
	    }
    }
}

unsigned int usb_read_payload(volatile void* src, HANDLE hnd, unsigned int size)
{
	size_t dwRead;

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

    	while(dwRead>3)
    	{
    		dwRead -= 4;
    	    *hnd->dst.as_intptr++ = *(volatile int*)src;
    	}
    	while(dwRead--)
    	{
    	    *hnd->dst.as_byteptr++ = *(volatile char*)src;
    	}

    	if(!hnd->len)
    	{
    		hnd=NULL;

    	}
    }
    return size;
}

/** Reset on DCR_RESET
 *
 * @param drv_info
 */
void usb_hal_reset(USB_DRV_INFO drv_info)
{
    TRACE1_USB_DEBUG("\r\nUSBD_Init");
	NVIC->NVIC_SetPriority(drv_info->info.drv_index,
			drv_info->info.isr_priority);
}

void usb_hal_stall(USB_Type* hw_base, unsigned int eptnum, int is_in_dir)
{
    if (eptnum == EPT_0)
	{
		hw_base->DEVICE_EP[0].USBTXCSRL = USB_USBTXCSRL0_STALL;
	}
	else
	{
		if (is_in_dir)
		{
			hw_base->DEVICE_EP[eptnum].USBTXCSRL = USB_USBTXCSRL_STALL;
		}
		else
		{
			hw_base->DEVICE_EP[eptnum].USBRXCSRL = USB_USBRXCSRL_STALL;
		}
	}
}

void usb_hal_stall_clear(USB_Type* hw_base, unsigned int eptnum, int is_in_dir)
{
    if (eptnum == EPT_0)
	{
		hw_base->DEVICE_EP[0].USBTXCSRL &= ~USB_USBTXCSRL0_STALLED;
	}
	else
	{
		if (is_in_dir)
		{
	        // Clear the stall on an IN endpoint.
	        hw_base->DEVICE_EP[eptnum].USBTXCSRL &=
		            ~(USB_USBTXCSRL_STALL | USB_USBTXCSRL_STALLED);

	        // Reset the data toggle.
	        hw_base->DEVICE_EP[eptnum].USBTXCSRL |= USB_USBTXCSRL_CLRDT;

		}
		else
		{
	        // Clear the stall on an OUT endpoint.
			hw_base->DEVICE_EP[eptnum].USBRXCSRL &=
	            ~(USB_USBRXCSRL_STALL | USB_USBRXCSRL_STALLED);

	        // Reset the data toggle.
			hw_base->DEVICE_EP[eptnum].USBRXCSRL |= USB_USBRXCSRL_CLRDT;
		}
	}
}

/** PKTRDY must be set to enable transmission of the packet that was loaded
 */
void usb_hal_txpktrdy(USB_Type* hw_base, unsigned int eptnum, int len)
{
	unsigned int flags;

	if(eptnum)
	{
		flags = USB_USBTXCSRL_TXRDY;
	} else
	{
		if(len)
			flags = USB_USBTXCSRL0_TXRDY;
		else
			flags = USB_USBTXCSRL0_TXRDY | USB_USBTXCSRL0_DATAEND;
	}
    hw_base->DEVICE_EP[eptnum].USBTXCSRL = flags;

}

/** Configure as device
 *
 * @param drv_info
 */
void usb_hal_configure(USB_DRV_INFO drv_info)
{
	USB_CONTROLLER* hw_base = drv_info->hw_base;
//	unsigned int status;

    // Enable Clocking to the USB controller.
	SysCtlPeripheralEnable(drv_info->info.peripheral_indx);
	SysCtlPeripheralReset(drv_info->info.peripheral_indx);

	// Enable USB related pins
	PIO_Cfg_List((PIN_DESC*)usb_pins);

    // Turn on USB Phy clock.
    SYSCTL->RCC2 &= ~SYSCTL_RCC2_USBPWRDN;


    // Clear interrupt statuses
    hw_base->USBIS;
    hw_base->USBTXIS;
    hw_base->USBRXIS;

	// Enable USB Interrupts.
    hw_base->USBIE |= USB_USBIE_RESET | USB_USBIE_DISCON | USB_USBIE_SESREQ
			| USB_USBIE_CONN | USB_USBIE_RESUME | USB_USBIE_SUSPND
			/*| USB_USBIE_SOF*/;

	hw_base->USBIDVIM = USB_USBIDVIM_ID;
	hw_base->USBTXIE = USB_USBTXIE_EP0;
	hw_base->USBRXIE = 0;
	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);

	drv_info->hw_base->USBPOWER |= USB_USBPOWER_SOFTCONN;
	//hw_base->USBGPCS = USB_USBGPCS_DEVMOD | USB_USBGPCS_DEVMODOTG;
}

static unsigned int usb_hal_ept0_status(USB_CONTROLLER* hw_base)
{
	unsigned int status, clear;

	status = hw_base->DEVICE_EP[0].USBTXCSRL;
	clear = status;

	// clear RXRDY
//    if(status & USB_USBTXCSRL0_RXRDY)
//    {
//    	clear |= USB_USBTXCSRL0_RXRDYC;
//    }

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
    hw_base->DEVICE_EP[0].USBTXCSRL = clear;

    return status;
}

static unsigned int usb_hal_ept_tx_status(USB_CONTROLLER* hw_base, unsigned int eptnum)
{
	unsigned int status;

	status = hw_base->DEVICE_EP[eptnum].USBTXCSRL;

	hw_base->DEVICE_EP[eptnum].USBTXCSRL = status & ~(USB_USBTXCSRL0_STALLED
			| USB_USBTXCSRL_UNDRN);
    return status;
}

static unsigned int usb_hal_ept_rx_status(USB_CONTROLLER* hw_base, unsigned int eptnum)
{
	unsigned int status;

	status = hw_base->DEVICE_EP[eptnum].USBRXCSRL;

	hw_base->DEVICE_EP[eptnum].USBRXCSRL = status & ~(USB_USBRXCSRL_STALLED
			| USB_USBRXCSRL_DATAERR | USB_USBRXCSRL_OVER);
    return status;
}

/*void USBFIFOFlush(USB_CONTROLLER* hw_base, unsigned long eptnum,
             unsigned long ulFlags)
{
    // Endpoint zero has a different register set for FIFO flushing.
    if(eptnum == USB_EP_0)
    {
        // Nothing in the FIFO if neither of these bits are set.
    	if(hw_base->DEVICE_EP[0].USBTXCSRL  &(USB_USBTXCSRL0_RXRDY | USB_USBTXCSRL0_TXRDY))
    	{
    		hw_base->DEVICE_EP[0].USBTXCSRH = USB_USBTXCSRH0_FLUSH;
    	}
    }
    else
    {
        // Only reset the IN or OUT FIFO.
        if(ulFlags & (USB_EP_HOST_OUT | USB_EP_DEV_IN))
        {
            // Make sure the FIFO is not empty.
        	if(hw_base->DEVICE_EP[eptnum].USBTXCSRL & USB_USBTXCSRL_TXRDY)
        	{
        		hw_base->DEVICE_EP[eptnum].USBTXCSRL |= USB_USBTXCSRL_FLUSH;
        	}
        }
        else
        {
            // Make sure that the FIFO is not empty.
        	if(hw_base->DEVICE_EP[eptnum].USBRXCSRL & USB_USBRXCSRL_RXRDY)
        	{
        		hw_base->DEVICE_EP[eptnum].USBRXCSRL |= USB_USBRXCSRL_FLUSH;
        	}
        }
    }
}*/

void usb_ept_reset(USB_DRV_INFO drv_info, unsigned int eptnum)
{
	USB_CONTROLLER* hw_base = drv_info->hw_base;
	unsigned int mask, status;

	mask = ~(1 << eptnum);
	if(eptnum == 0)
	{
		// - disable interrupts
		hw_base->USBTXIE &= mask;

		// - clear ept0 status and flush
		status = hw_base->DEVICE_EP[0].USBTXCSRL;
		if( status & USB_USBTXCSRL0_RXRDY)
			hw_base->DEVICE_EP[0].USBTXCSRH = USB_USBTXCSRH0_FLUSH;
		usb_hal_ept0_status(hw_base);
	} else
	{
		// - disable interrupts
		hw_base->USBRXIE &= mask;
		hw_base->USBTXIE &= mask;

		// - clear ept1-15 status and flush
		status = hw_base->DEVICE_EP[eptnum].USBRXCSRL;
		if( status & USB_USBRXCSRL_RXRDY)
		{
			hw_base->DEVICE_EP[eptnum].USBRXCSRL |= USB_USBRXCSRL_FLUSH;
			// do it again
			status = hw_base->DEVICE_EP[eptnum].USBRXCSRL;
			if( status & USB_USBRXCSRL_RXRDY)
				hw_base->DEVICE_EP[eptnum].USBRXCSRL |= USB_USBRXCSRL_FLUSH;
		}
		usb_hal_ept_rx_status(hw_base, eptnum);


		status = hw_base->DEVICE_EP[eptnum].USBTXCSRL;
		if (status & USB_USBTXCSRL_TXRDY)
		{
			hw_base->DEVICE_EP[eptnum].USBTXCSRL |= USB_USBTXCSRL_FLUSH;
			// do it again
			status = hw_base->DEVICE_EP[eptnum].USBTXCSRL;
			if (status & USB_USBTXCSRL_TXRDY)
			{
				hw_base->DEVICE_EP[eptnum].USBTXCSRL |= USB_USBTXCSRL_FLUSH;

			}
		}
		usb_hal_ept_tx_status(hw_base, eptnum);
	}

    Endpoint *pEndpoint;
    pEndpoint = &drv_info->drv_data->endpoints[eptnum];
	usb_end_transfers(pEndpoint, USBD_STATUS_RESET);
    pEndpoint->state = ENDPOINT_STATE_DISABLED;
}

/**
 * Configures an endpoint according to its Endpoint Descriptor.
 * \param pDescriptor Pointer to an Endpoint descriptor.
 */
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor)
{
    Endpoint *pEndpoint;
    unsigned int eptnum, reg;
    EndpointAttribute_TransferType ept_type;
    EndpointDirection ept_dir;
	USB_CONTROLLER* hw_base = drv_info->hw_base;

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
	usb_ept_reset(drv_info, eptnum);
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
        reg = 0;

        // Determine if a transmit or receive endpoint is being configured.
        //
        if(ept_dir == ENDPOINT_DIRECTION_IN)
        {
            // Set the maximum packet size.
            hw_base->DEVICE_EP[eptnum].USBTXMAXP = pEndpoint->txfifo_size;


            // Allow auto setting of TxPktRdy when max packet size has been loaded
            // into the FIFO.
/*
            if(ulFlags & USB_EP_AUTO_SET)
            {
                reg |= USB_USBTXCSRH_AUTOSET;
            }
*/


            // Enable isochronous mode if requested.
            if(ept_type == ENDPOINT_TYPE_ISOCHRONOUS)
            {
                reg |= USB_USBTXCSRH_ISO;
            }

            // Write the transmit control value.
            hw_base->DEVICE_EP[eptnum].USBTXCSRH = reg;

            // Reset the Data toggle to zero.
            hw_base->DEVICE_EP[eptnum].USBTXCSRL = USB_USBTXCSRL_CLRDT;

    		hw_base->USBTXIE |= 1 << eptnum;

        }
        else
        {
            // Set the MaxPacketSize.
            hw_base->DEVICE_EP[eptnum].USBRXMAXP = pEndpoint->rxfifo_size;

            // Allow auto clearing of RxPktRdy when packet of size max packet
            // has been unloaded from the FIFO.
/*
            if(ulFlags & USB_EP_AUTO_CLEAR)
            {
                reg = USB_USBRXCSRH_AUTOCL;
            }
*/

            //
            // Enable isochronous mode if requested.
            //
            if(ept_type == ENDPOINT_TYPE_ISOCHRONOUS)
            {
                reg |= USB_USBRXCSRH_ISO;
            }

            // Write the receive control value.
            hw_base->DEVICE_EP[eptnum].USBRXCSRH = reg;


            // Reset the Data toggle to zero.
            hw_base->DEVICE_EP[eptnum].USBRXCSRL = USB_USBRXCSRL_CLRDT;

    		hw_base->USBRXIE |= 1 << eptnum;

        }
    }

    TRACE_USB(" CfgEp(%d)", eptnum);
}

/** USB bus reset
 *
 * @param drv_info
 */
void usb_hal_bus_reset(USB_DRV_INFO drv_info)
{
	const USBDDriverDescriptors * dev_descriptors;

	//Step 1. enable clocks etc.

	//Step 2. reset endpoints
	for(int i= 0; i<USB_NUMENDPOINTS; i++)
	{
		usb_ept_reset(drv_info, i);
	}

	dev_descriptors = drv_info->drv_data->device.pDescriptors;

	if(dev_descriptors)
	{
		usb_hal_ept_config(drv_info, &dev_descriptors->pFsDevice->as_generic);
	}
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
    		return size_code;
    	}
    	new_size <<= 1;
    }
	TRACELN_USB("ERROR: invalid endpoint size!");
	size = 64;
	return 3;
}

void usb_hal_config_fifo(USB_DRV_INFO drv_info)
{
	unsigned int size, limit, address, size_code;
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	USB_CONTROLLER* hw_base = drv_info->hw_base;

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

void usb_ack_packet(USB_Type* hw_base, Endpoint* endpoint, unsigned int eptnum)
{
    if(eptnum == 0)
    {
        // Clear RxPktRdy, and optionally DataEnd, on endpoint zero.
    	hw_base->DEVICE_EP[0].USBTXCSRL = USB_USBTXCSRL0_RXRDYC
				| (endpoint->rxfifo_cnt ? 0 : USB_USBTXCSRL0_DATAEND);
    }
    else
    {
        // Clear RxPktRdy on all other endpoints.
        hw_base->DEVICE_EP[eptnum].USBRXCSRL &= ~(USB_USBRXCSRL_RXRDY);
    }
}


void usb_b_ept0_handler(USB_DRV_INFO drv_info)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;
	USB_CONTROLLER* hw_base = drv_info->hw_base;
    Endpoint *endpoint;
    unsigned int status;
    HANDLE hnd;

    status = usb_hal_ept0_status(hw_base);
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

	if ( (endpoint->state == ENDPOINT_STATE_SENDING)  )
	{
		//check if we have pending write
		while((hnd=endpoint->pending) && ( hnd->len == 0) )
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
		    	TRACE1_USB(" Wr");
			    usb_write_payload( ENTPOINT_FIFO(hw_base, 0), hnd, endpoint->txfifo_size);
			    usb_hal_txpktrdy(drv_info->hw_base, 0, hnd->len);
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
	    		hw_base->USBTXIE &= USB_USBTXIE_EP0;
	        } else
	        {
	        	hw_base->DEVICE_EP[0].USBTXCSRL = USB_USBTXCSRL0_RXRDYC;
//	        	usb_ack_packet(drv_info->hw_base, endpoint, 0);
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
	USB_CONTROLLER* hw_base = drv_info->hw_base;
    Endpoint *endpoint;
    unsigned int status;
    HANDLE hnd;

    status = usb_hal_ept_rx_status(hw_base, eptnum);
    TRACE_USB("%02x]", status);
    endpoint = &drv_data->endpoints[eptnum];

	if (status & USB_USBRXCSRL_DATAERR)
	{
    	TRACE_USB(" DATAERR state=%d", endpoint->state);
	}

	if (status & USB_USBRXCSRL_OVER)
	{
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

		if(endpoint->state == ENDPOINT_STATE_IDLE)
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
	        	endpoint->state = ENDPOINT_STATE_RECEIVING_OFF;
	    		endpoint->rxfifo_cnt = size;
	    		hw_base->USBRXIE &= 1 << eptnum;
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
	USB_CONTROLLER* hw_base = drv_info->hw_base;
    Endpoint *endpoint;
    unsigned int status;
    HANDLE hnd;

    status = usb_hal_ept_tx_status(hw_base, eptnum);
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

	if ( (endpoint->state == ENDPOINT_STATE_SENDING)  )
	{
		//check if we have pending write
		while((hnd=endpoint->pending) && ( hnd->len == 0) )
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
		    	TRACE1_USB(" Wr");
			    usb_write_payload( ENTPOINT_FIFO(hw_base, eptnum), hnd, endpoint->txfifo_size);
			    hw_base->DEVICE_EP[eptnum].USBTXCSRL = USB_USBTXCSRL_TXRDY;
			    //usb_hal_txpktrdy(drv_info->hw_base, eptnum, hnd->len);
			}
		} else
		{
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
    if(drv_data->usb_state == USB_STATE_DOWN)
    {
    	//Disconnect
		TRACELN1_USB("USB: session end");
    	drv_info->hw_base->USBPOWER &= ~(USB_USBPOWER_SOFTCONN);
    } else
    {
    	//Received Session request
    	if(status & USB_USBIS_SESREQ)
    	{
    		TRACE1_USB(" session start");
        	if(drv_data->usb_device_mode == USB_MODE_ENABLED)
        	{
        		drv_data->usb_device_mode = USB_MODE_STARTING;
            	drv_info->hw_base->USBPOWER |= USB_USBPOWER_SOFTCONN;

        	} else
        	{
        		// restart maybe?
        	}
    	}

        // Received a reset from the host.
        if(status & USB_USBIS_RESET)
        {
    		TRACE1_USB(" reset");
        	drv_data->usb_event(e_reset);
        	usb_hal_bus_reset(drv_info);
        }

        // Suspend was signaled on the bus.
        if(status & USB_USBIS_SUSPEND)
        {
    		TRACE1_USB(" suspend");
        	drv_data->usb_event(e_susppend);
	        drv_data->usb_state = USB_STATE_SUSPENDED;

        }

        // Resume was signaled on the bus.
        if(status & USB_USBIS_RESUME)
        {
    		TRACE1_USB(" usb resume");
        	drv_data->usb_event(e_resume);
        }

        // USB device was disconnected.
        if(status & USB_USBIS_DISCON)
        {
    		TRACE1_USB(" usb disconnect");
        	drv_data->usb_event(e_disconnect);
        }

        //
        // Start of Frame was received.
        //
        if(status & USB_USBIS_SOF)
        {
        	drv_data->frame_count++;

        	if( drv_data->usb_power & USB_POWER_REMOTE_WAKE_ACTIVE)
        	{
        		drv_data->wake_count++;
        		if(drv_data->wake_count == 10)
        			drv_info->hw_base->USBPOWER &= USB_USBPOWER_RESUME;
        		if(drv_data->wake_count >= 20)
        		{
        			//Wakeup complete
        			drv_data->usb_power &= ~USB_POWER_REMOTE_WAKE_ACTIVE;
                	drv_data->usb_event(e_wakeup);
        		}
        	}
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
	USB_CONTROLLER* hw_base = drv_info->hw_base;

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

void USB_B_ISR(USB_DRV_INFO drv_info)
{
	unsigned int status, eptnum;
	USB_CONTROLLER* hw_base = drv_info->hw_base;

    //----- Process USBIS interrupts
    status = hw_base->USBIS;
    if(status)
    {
    	if(status != 8 )
    		TRACELN_USB("USB:[is=%02x]", status);
    	usb_b_usbis_handler(drv_info, status);
    }

    //----- Process endpoint transmit interrupts
    status = hw_base->USBTXIS;
    if(status & USB_USBTXIS_EP0)
    {
    	TRACELN1_USB("USB:[E0=");
    	usb_b_ept0_handler(drv_info);
    }
	eptnum=1;
    while( (status>>=1) )
    {
		if(status & 1)
		{
	    	TRACELN_USB("USB:[T%x=", eptnum);
   			usb_b_ept_tx_handler(drv_info, eptnum);
		}
		eptnum++;
    }

    //----- Process endpoint receive interrupts
    status = hw_base->USBRXIS;
    if(status)
    {
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
    	//Power Fault status has been detected
    	hw_base->USBEPCISC = status;

    	TRACE_USB("\r\nUSB:[pc=%02x]", status);
    }

    //----- Process USBIDVISC interrupt
    status = hw_base->USBIDVISC & USB_USBIDVISC_ID;
    if(status)
    {
    	//ID Valid Detect Interrupt
    	hw_base->USBIDVISC = status;
    	TRACE_USB("\r\nUSB:[id=%02x]", status);
    }


}

