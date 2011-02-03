/**************************************************************************//**
 * @file     usbd_drv.cpp
 * @ingroup  DRV_AT91_USBD
 * @brief    USBD driver implementation
 * @version  V3.00
 * @date     26. December 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <usbd_drv.h>
#include <fam_cpp.h>
#include <platform_drv.h>
#include "USBRequests.h"
#include <USBDescriptors.h>

/** Bit mask for both banks of the UDP_CSR register. */
#define UDP_CSR_RXDATA_BK      (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1)

/**
 *  \section udp_csr_register_access_sec "UDP CSR register access"
 *
 *  This page lists the macros to access UDP CSR register.
 *
 *  \comment
 *  In a preemptive environment, set or clear the flag and wait for a time of
 *  1 UDPCK clock cycle and 1 peripheral clock cycle. However, RX_DATA_BK0,
 *  TXPKTRDY, RX_DATA_BK1 require wait times of 3 UDPCK clock cycles and
 *  5 peripheral clock cycles before accessing DPR.
 *  See datasheet
 *
 *  !Macros
 *  - CLEAR_CSR
 *  - SET_CSR
 */
#define nop()   __asm__ __volatile__ ( "nop" )

/**  Bitmap for all status bits in CSR. */
#define REG_NO_EFFECT_1_ALL      UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1 \
                                |UDP_CSR_STALLSENTISOERROR | UDP_CSR_RXSETUP \
                                |UDP_CSR_TXCOMP

/**
 *  Sets the specified bit(s) in the UDP_CSR register.
 *
 *  \param endpoint The endpoint number of the CSR to process.
 *  \param flags The bitmap to set to 1.
 */
void SET_CSR(RwReg* csr, uint32_t flags)
{
    uint32_t reg;

    reg = *csr;
    reg |= REG_NO_EFFECT_1_ALL;
    reg |= flags;
    *csr = reg;
    for(int32_t  nop_count=0; nop_count<15; nop_count++ )
    {
        nop();
    }
}

/**
 *  Clears the specified bit(s) in the UDP_CSR register.
 *
 *  \param endpoint The endpoint number of the CSR to process.
 *  \param flags The bitmap to clear to 0.
 */
void CLEAR_CSR(RwReg* csr, uint32_t flags)
{
    uint32_t reg;

    reg = *csr;
    reg |= REG_NO_EFFECT_1_ALL;
    reg &= ~flags;
    *csr = reg;
    for(int32_t  nop_count=0; nop_count<15; nop_count++ )
    {
        nop();
    }
}



/*---------------------------------------------------------------------------
 *      Internal Functions
 *---------------------------------------------------------------------------*/

/* Configure the pull-up on D+ and disconnect it */
void USB_PULLUP_DISABLE(USBD_INFO drv_info)
{
//	PIO_CfgInput(USB_STAT_PIN);
	drv_info->hw_base->UDP_TXVC &= ~(uint32_t)UDP_TXVC_PUON;
}

/* Configure the pull-up on D+ and enable it */
void USB_PULLUP_ENABLE(USBD_INFO drv_info)
{
	//PIO_CfgOutput0(USB_STAT_PIN);
	drv_info->hw_base->UDP_TXVC |= UDP_TXVC_PUON;
}


/**
 * Enables the clock of the UDP peripheral.
 * \return 1 if peripheral status changed.
 */
static inline uint8_t UDP_EnablePeripheralClock(uint32_t dwId)
{
    if (!PMC_IsPeriphEnabled(dwId))
    {
        PMC_EnablePeripheral(dwId);
        return 1;
    }
    return 0;
}

/**
 * Disables the UDP peripheral clock.
 */
static inline void UDP_DisablePeripheralClock(uint32_t dwId)
{
    PMC_DisablePeripheral(dwId);
}

/**
 * Enables the 48MHz USB clock.
 */
static inline void UDP_EnableUsbClock(void)
{
    REG_PMC_SCER = PMC_SCER_UDP;
}

/**
 *  Disables the 48MHz USB clock.
 */
static inline void UDP_DisableUsbClock(void)
{
    REG_PMC_SCDR = PMC_SCER_UDP;
}


/**
 * Enables the UDP transceiver.
 */
static inline void UDP_EnableTransceiver(Udp* pUDP)
{
    pUDP->UDP_TXVC &= ~(uint32_t)UDP_TXVC_TXVDIS;
}

/**
 * Disables the UDP transceiver.
 */
static inline void UDP_DisableTransceiver(Udp* pUDP)
{
	pUDP->UDP_TXVC |= UDP_TXVC_TXVDIS;
}


/**
 * Handles a completed transfer on the given endpoint, invoking the
 * configured callback if any.
 * \param bEndpoint Number of the endpoint for which the transfer has completed.
 * \param bStatus   Status code returned by the transfer operation
 */
static HANDLE UDP_EndOfTransfer(Endpoint *endpoint, unsigned int status)
{
    HANDLE hnd;

    hnd=endpoint->pending;

	if (hnd)
	{
		TRACE1_USB_DEBUG("EoT ");
		endpoint->pending = hnd->next;
		usr_HND_SET_STATUS(hnd, status);
	}

	return hnd;
}

/*
    Function: UDP_DisableEndpoints
        Disables all endpoints of the UDP peripheral except Control endpoint 0.
*/
static void UDP_DisableEndpoints(USBD_DRIVER_DATA drv_data)
{
    Endpoint *endpoint;
    unsigned int eptnum;

    endpoint = drv_data->endpoints;

    // Disable each endpoint, terminating any pending transfer
    for (eptnum = 1; eptnum < CHIP_USB_NUMENDPOINTS; eptnum++)
    {
        endpoint++;
        endpoint->state = UDP_ENDPOINT_DISABLED;
        UDP_EndOfTransfer(endpoint, USBD_STATUS_ABORTED);
    }
}


/*
    Function: UDP_WritePayload
        Writes a data payload into the current FIFO buffer of the UDP.

    Parameters:
        eptnum - Number of the endpoint which is sending data.
*/
static void UDP_WritePayload(RwReg *dst, HANDLE hnd, int size)
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

/*
    Function: UDP_ReadPayload
        Reads a data payload from the current FIFO buffer of an endpoint.

    Parameters:
        eptnum - Endpoint number.
        size - Size of the data to read.
*/
static bool UDP_ReadPayload(Endpoint *pEndpoint, RwReg *src, size_t size)
{
	HANDLE hnd;
	size_t dwRead;
    unsigned char *dst;

    TRACE_USB("%d ", size);


    while(size)
    {
    	hnd = pEndpoint->pending;
    	if(!hnd)
    		return true;

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
    		pEndpoint->pending = hnd->next;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
    	}
    }
    return false;

}

/**
 * Clears the correct reception flag (bank 0 or bank 1) of an endpoint
 * \param bEndpoint Index of endpoint
 */
static void UDP_ClearRxFlag(RwReg *pCSR, Endpoint *pEndpoint, unsigned int bEndpoint)
{
    // Clear flag and change banks
    if (pEndpoint->bank == 0)
    {
        CLEAR_CSR(pCSR, UDP_CSR_RX_DATA_BK0);
        // Swap bank if in dual-fifo mode
        if (CHIP_USB_ENDPOINTS_BANKS(bEndpoint) > 1)
        {
            pEndpoint->bank = 1;
        }
    }
    else
    {
        CLEAR_CSR(pCSR, UDP_CSR_RX_DATA_BK1);
        pEndpoint->bank = 0;
    }
}

/**
 * Received SETUP packet from endpoint 0 FIFO
 * \param pRequest Generic USB SETUP request sent over Control endpoints
 */
static void UDP_ReadRequest(USBGenericRequest *pRequest, RwReg* reg)
{
    uint8_t *pData = (uint8_t *)pRequest;

    // Copy packet
    for (uint32_t i = 0; i < 8; i++) {

        pData[i] = *reg;
    }
}

/**
 * Endpoint interrupt handler.
 * Handle IN/OUT transfers, received SETUP packets and STALLing
 * \param bEndpoint Index of endpoint
 */
static void UDP_EndpointHandler(Udp* pUDP, Endpoint *pEndpoint, uint8_t bEndpoint)
{
    uint32_t status;
    USBGenericRequest request;
    HANDLE hnd;

    status = pUDP->UDP_CSR[bEndpoint];

    TRACE_USB("E%d ", bEndpoint);
    TRACE_USB("st:0x%X ", status);

    // Handle interrupts
    // IN packet sent
    if ((status & UDP_CSR_TXCOMP) != 0)
    {
    	TRACE_USB("Wr ");

		//check if we have pending write
		while((hnd=pEndpoint->pending) && ( hnd->len == 0) )
		{
	   		// End of transfer ?
			pEndpoint->pending = hnd->next;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
		}

		//check if we have more to write
		if(hnd)
		{
            // Send next packet
            if (CHIP_USB_ENDPOINTS_BANKS(bEndpoint) == 1)
            {
                // No double buffering
                UDP_WritePayload(&pUDP->UDP_FDR[bEndpoint], hnd, pEndpoint->size);
                SET_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_TXPKTRDY);
                // Acknowledge interrupt
                CLEAR_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_TXCOMP);
            }
            else
            {
                // Double buffering
                SET_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_TXPKTRDY);
                // Acknowledge interrupt
                CLEAR_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_TXCOMP);
                UDP_WritePayload(&pUDP->UDP_FDR[bEndpoint], hnd, pEndpoint->size);
            }
		} else
		{
			pEndpoint->state = UDP_ENDPOINT_IDLE;
            // Disable interrupt if this is not a control endpoint
            if ((status & UDP_CSR_EPTYPE_Msk) != UDP_CSR_EPTYPE_CTRL)
            {
                pUDP->UDP_IDR |= 1 << bEndpoint;
            }
            // Acknowledge interrupt
            CLEAR_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_TXCOMP);
		}
    }


    // OUT packet received
    if ((status & UDP_CSR_RXDATA_BK) != 0)
    {
    	TRACE_USB("Rd ");

		//check if we have pending read
    	hnd = pEndpoint->pending;

		if(!hnd)
		{
            // Check if an ACK has been received on a Control endpoint
            if (((status & UDP_CSR_EPTYPE_Msk) == UDP_CSR_EPTYPE_CTRL)
                && ((status & UDP_CSR_RXBYTECNT_Msk) == 0))
            {
                // Acknowledge the data and finish the current transfer
            	TRACE_USB("Ack ");
                UDP_ClearRxFlag(&pUDP->UDP_CSR[bEndpoint], pEndpoint, bEndpoint);
            }
            // Check if the data has been STALLed
            else if ((status & UDP_CSR_FORCESTALL) != 0)
            {
                // Discard STALLed data
            	TRACE_USB("Disc ");
                UDP_ClearRxFlag(&pUDP->UDP_CSR[bEndpoint], pEndpoint, bEndpoint);
            }
            // NAK the data
            else
            {
            	TRACE_USB("Nak ");
                pUDP->UDP_IDR |= 1 << bEndpoint;
            }
        }
        // Endpoint is in Read state
        else
        {
            // Retrieve data and store it into the current transfer buffer
            if (UDP_ReadPayload(pEndpoint, &pUDP->UDP_FDR[bEndpoint], status >> 16))
            {
	            // Check if the transfer is finished
                // Disable interrupt if this is not a control endpoint
                if ((status & UDP_CSR_EPTYPE_Msk) != UDP_CSR_EPTYPE_CTRL)
                {
                    pUDP->UDP_IDR |= 1 << bEndpoint;
                }
                pEndpoint->state = UDP_ENDPOINT_IDLE;
            }
	        UDP_ClearRxFlag(&pUDP->UDP_CSR[bEndpoint], pEndpoint, bEndpoint);
        }
    }


    // STALL sent
    if ((status & UDP_CSR_STALLSENTISOERROR) != 0)
    {
    	TRACE_USB("Sta ");

        CLEAR_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_STALLSENTISOERROR);

        if (   (status & UDP_CSR_EPTYPE_Msk) == UDP_CSR_EPTYPE_ISO_IN
            || (status & UDP_CSR_EPTYPE_Msk) == UDP_CSR_EPTYPE_ISO_OUT ) {

        	TRACE_USB("Isoe [%d] ", bEndpoint);
            while(UDP_EndOfTransfer(pEndpoint, USBD_STATUS_ABORTED))
            {

            }
            pEndpoint->state = UDP_ENDPOINT_IDLE;
        }
        else
        {
            TRACE_WARNING("Sta 0x%X [%d] ", (int)status, bEndpoint);

            // If the endpoint is not halted, clear the STALL condition
            if (pEndpoint->state != UDP_ENDPOINT_HALTED)
            {
                TRACE_WARNING( "_ " );
                CLEAR_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_FORCESTALL);
            }
        }
    }

    // SETUP packet received
    if ((status & UDP_CSR_RXSETUP) != 0)
    {
    	TRACE_USB("Stp ");

        // If a transfer was pending, complete it
        // Handles the case where during the status phase of a control write
        // transfer, the host receives the device ZLP and ack it, but the ack
        // is not received by the device
        if (pEndpoint->state & UDP_ENDPOINT_BUSY)
		{
            while( (hnd=pEndpoint->pending) && !(hnd->cmd & FLAG_LOCK))
			{
            	UDP_EndOfTransfer(pEndpoint, USBD_STATUS_SUCCESS);
			}
            pEndpoint->state = UDP_ENDPOINT_IDLE;
        }

        UDP_ReadRequest(&request, &pUDP->UDP_FDR[bEndpoint]);

        if( (hnd=pEndpoint->pending) && (hnd->cmd == (CMD_READ | FLAG_LOCK)))
        {
        	unsigned int len;
        	unsigned char * src;

        	pEndpoint->pending = hnd->next;
        	len = hnd->len;
        	if(len >8)
        		len = 8;
        	hnd->len -= len;
        	src = (unsigned char *)&request;
        	while(len--)
        		*hnd->dst.as_byteptr++ = *src++;
			usr_HND_SET_STATUS(hnd, RES_OK);
//        	UDP_ReadPayload((HANDLE)endpoint, &pUDP->UDP_FDR[eptnum], 8, 9);
        }


        // Set the DIR bit before clearing RXSETUP in Control IN sequence
        if (USBGenericRequest_GetDirection(&request) == USBGenericRequest_IN)
        {
            SET_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_DIR);
        }
        CLEAR_CSR(&pUDP->UDP_CSR[bEndpoint], UDP_CSR_RXSETUP);

        // Forward the request to the upper layer
        //USBDCallbacks_RequestReceived(&request);
    }
}

/*
    Function: UDP_ResetEndpoints
        Resets all the endpoints of the UDP peripheral.
*/
static void USBD_ResetEndpoints(USBD_DRIVER_DATA drv_data)
{
    Endpoint *endpoint;
    unsigned int eptnum;

    endpoint = drv_data->endpoints;
    // Reset the transfer descriptor of every endpoint
    for (eptnum = 0; eptnum < CHIP_USB_NUMENDPOINTS; eptnum++)
    {
        // Reset endpoint state
        endpoint->bank = 0;
        endpoint->state = UDP_ENDPOINT_DISABLED;

        // Reset endpoint transfer descriptor
        while(UDP_EndOfTransfer(endpoint, USBD_STATUS_RESET))
        {

        }

        endpoint++;
    }
}

/**
 * Configures an endpoint according to its Endpoint Descriptor.
 * \param pDescriptor Pointer to an Endpoint descriptor.
 */
static void USBD_ConfigureEndpoint(USBD_INFO drv_info, const USBEndpointDescriptor *pDescriptor)
{
    Endpoint *pEndpoint;
    uint8_t bEndpoint;
    uint8_t bType;
    uint8_t bEndpointDir;
	Udp* pUDP = drv_info->hw_base;

    /* NULL descriptor -> Control endpoint 0 in default */
    if (pDescriptor == 0)
    {
        bEndpoint = 0;
        pEndpoint = &drv_info->drv_data->endpoints[bEndpoint];
        bType= USBEndpointDescriptor_CONTROL;
        bEndpointDir = 0;
        pEndpoint->size = CHIP_USB_ENDPOINTS_MAXPACKETSIZE(0);
    }
    /* Device descriptor -> Specific Control EP */
    else if (pDescriptor->bDescriptorType == USBGenericDescriptor_DEVICE)
    {
        bEndpoint = 0;
        pEndpoint = &drv_info->drv_data->endpoints[bEndpoint];
        bType = USBEndpointDescriptor_CONTROL;
        bEndpointDir = 0;
        pEndpoint->size = ((USBDeviceDescriptor *)pDescriptor)->bMaxPacketSize0;
    }
    /* Not endpoint descriptor, ERROR! */
    else if (pDescriptor->bDescriptorType != USBGenericDescriptor_ENDPOINT)
    {
        return;
    }
    else
    {
        bEndpoint = pDescriptor->bEndpointAddress & 0xF; //USBEndpointDescriptor_GetNumber()
        pEndpoint = &drv_info->drv_data->endpoints[bEndpoint];
        bType = pDescriptor->bmAttributes & 0x3; // USBEndpointDescriptor_GetType()
        bEndpointDir = (pDescriptor->bEndpointAddress & 0x80)? //USBEndpointDescriptor_GetDirection()
        		USBEndpointDescriptor_IN : USBEndpointDescriptor_OUT;
        pEndpoint->size = pDescriptor->wMaxPacketSize; //USBEndpointDescriptor_GetMaxPacketSize()
    }

    /* Abort the current transfer is the endpoint was configured and in
       Write or Read state */
    if (pEndpoint->state & UDP_ENDPOINT_BUSY)
    {
        UDP_EndOfTransfer(pEndpoint, USBD_STATUS_RESET);
    }
    pEndpoint->state = UDP_ENDPOINT_IDLE;

    /* Reset Endpoint Fifos */
    pUDP->UDP_RST_EP |= (1 << bEndpoint);
    pUDP->UDP_RST_EP &= ~(1 << bEndpoint);

    /* Configure endpoint */
    SET_CSR(&pUDP->UDP_CSR[bEndpoint], (uint32_t)UDP_CSR_EPEDS
                       | (bType << 8) | (bEndpointDir << 10));
    if (bType != USBEndpointDescriptor_CONTROL)
    {

    }
    else
    {

        pUDP->UDP_IER = (1 << bEndpoint);
    }

    TRACE_USB("CfgEp%d ", bEndpoint);
}

/*
    Function: USBD_Stall
        Causes the given endpoint to acknowledge the next packet it receives
        with a STALL handshake.

    Parameters:
        eptnum - Endpoint number.

    Returns:
        USBD_STATUS_SUCCESS or USBD_STATUS_LOCKED.
*/
void USBD_Stall(HANDLE hnd)
{
	// note: Stall will be performed for the last endpoint used by this handle!!

	hnd->dst.as_int = DCR_USB_STALL;
	hnd->hcontrol(DCR_HANDLE);
}

void USBD_SUSPEND(USBD_INFO drv_info)
{
	TRACE1_USB_DEBUG("\r\nSUSPEND!\r\n");
    USBD_DRIVER_DATA drv_data = drv_info->drv_data;

    // Reset endpoint structures
    USBD_ResetEndpoints(drv_data);

    // Configure the pull-up on D+ and disconnect it
//    USB_PULLUP_CONFIGURE(drv_info);

    UDP_DisableTransceiver(drv_info->hw_base);

    UDP_DisablePeripheralClock(drv_info->info.peripheral_indx);
    UDP_DisableUsbClock();

    drv_data->previousDeviceState = drv_data->deviceState;
    drv_data->deviceState = USBD_STATE_SUSPENDED;
}


/**
 * \brief Reset endpoints and disable them.
 * -# Terminate transfer if there is any, with given status;
 * -# Reset the endpoint & disable it.
 * \param bmEPs    Bitmap for endpoints to reset.
 * \param bStatus  Status passed to terminate transfer on endpoint.
 * \param bKeepCfg 1 to keep old endpoint configuration.
 * \note Use USBD_HAL_ConfigureEP() to configure and enable endpoint
         if not keeping old configuration.
 * \sa USBD_HAL_ConfigureEP().
 */
void USBD_HAL_ResetEPs(USBD_INFO drv_info, uint32_t bmEPs, uint8_t bStatus, uint8_t bKeepCfg)
{
    Endpoint *pEndpoint;
    uint32_t tmp = bmEPs & ((1<<CHIP_USB_NUMENDPOINTS)-1);
    uint8_t  ep;
    uint32_t epBit, epCfg;
	Udp* pUDP = drv_info->hw_base;

    for (ep = 0, epBit = 1; ep < CHIP_USB_NUMENDPOINTS; ep ++)
    {
        if (tmp & epBit)
        {
            /* Disable ISR */
            pUDP->UDP_IDR = epBit;
            /* Kill pending TXPKTREADY */
            CLEAR_CSR(&pUDP->UDP_CSR[ep], UDP_CSR_TXPKTRDY);

            /* Reset transfer information */
            pEndpoint = &drv_info->drv_data->endpoints[ep];
            /* Reset endpoint state */
            pEndpoint->bank = 0;
            /* Endpoint configure */
            epCfg = pUDP->UDP_CSR[ep];
            /* Reset endpoint */
            pUDP->UDP_RST_EP |=  epBit;
            pUDP->UDP_RST_EP &= ~epBit;
            /* Restore configure */
            if (bKeepCfg)
            {
                //SET_CSR(ep, pEndpoint->CSR);
                SET_CSR(&pUDP->UDP_CSR[ep], epCfg);
            }
            else {
                //pEndpoint->CSR = 0;
                pEndpoint->state = UDP_ENDPOINT_DISABLED;
            }

            /* Terminate transfer on this EP */
            while(UDP_EndOfTransfer(pEndpoint, bStatus))
            {

            }
        }
        epBit <<= 1;
    }
    /* Reset EPs */
    // UDP->UDP_RST_EP |=  bmEPs;
    // UDP->UDP_RST_EP &= ~bmEPs;
}

/**
 * Suspend USB Device HW Interface
 *
 * -# Disable transceiver
 * -# Disable USB Clock
 * -# Disable USB Peripheral
 */
static void USBD_HAL_Suspend(USBD_INFO drv_info)
{
    /* The device enters the Suspended state */
    UDP_DisableTransceiver(drv_info->hw_base);
    UDP_DisableUsbClock();
    UDP_DisablePeripheralClock(drv_info->info.peripheral_indx);
}

/**
 * Activate USB Device HW Interface
 * -# Enable USB Peripheral
 * -# Enable USB Clock
 * -# Enable transceiver
 */
static void USBD_HAL_Activate(USBD_INFO drv_info)
{
    UDP_EnablePeripheralClock(drv_info->info.peripheral_indx);
    UDP_EnableUsbClock();
    UDP_EnableTransceiver(drv_info->hw_base);
}


void USBD_POWERON(USBD_INFO drv_info)
{
	USBD_DRIVER_DATA drv_data = drv_info->drv_data;

    TRACE1_USB_DEBUG("\r\nPOWERON!\r\n");
    if( drv_data->deviceState < USBD_STATE_POWERED)
	{
	    // Reset endpoint structures
    	USBD_ResetEndpoints(drv_data);

	    // Device is in the Attached state
	    drv_data->previousDeviceState = drv_data->deviceState;
	    drv_data->deviceState = USBD_STATE_POWERED;
	    UDP_EnablePeripheralClock(drv_info->info.peripheral_indx);
	    UDP_EnableUsbClock();
	    drv_info->hw_base->UDP_IER = UDP_IER_WAKEUP;

		// Install minidriver
//		drv_info->drv_data->install_mini(drv_info);

		drv_isr_enable(&drv_info->info);

		USB_PULLUP_ENABLE(drv_info);
	}
}


bool try_cancel_transfer(HANDLE * list, HANDLE hnd)
{
	HANDLE prev, next;

    if((prev = *list))
    {
    	if( prev == hnd )
    	{
    		*list = hnd->next;
			svc_HND_SET_STATUS(hnd, RES_SIG_IDLE);
			return true;
    	}
    	while((next=prev->next))
    	{
	    	if( next == hnd )
	    	{
	    		prev->next = hnd->next;
				svc_HND_SET_STATUS(hnd, RES_SIG_IDLE);
				return true;
	    	}
	    	prev = next;
    	}
    }
    return 0;
}


void USBD_DCR(USBD_INFO drv_info, unsigned int reason, HANDLE param)
{
    USBD_DRIVER_DATA drv_data = drv_info->drv_data;
	Udp* pUDP = drv_info->hw_base;

	switch(reason)
    {
        case DCR_RESET:
        	/* Clock may not be correct, disable USB anyway..*/
           	drv_isr_disable(&drv_info->info);
            TRACE1_USB_DEBUG("\r\nUSBD_Init");
			/*
			    Function: USBD_HAL_Init
			        Initializes the USB HW Access driver.
			*/

            /* Must before USB & TXVC access! */
            drv_pmc_enable(&drv_info->info);

  	        pUDP->UDP_GLB_STAT = 0;
		    drv_data->deviceState = USBD_STATE_ATTACHED;
		    USBD_SUSPEND(drv_info);
//		    drv_data->install_mini = drv_info->install_mini;

		    // Configure the pull-up on D+ and disconnect it
		    USB_PULLUP_DISABLE(drv_info);

            break;

        case DCR_PARAMS:
        	switch ((unsigned int)param)
        	{
				case USBD_STATE_SUSPENDED:
					USBD_SUSPEND(drv_info);
					break;
				case USBD_STATE_POWERED:
					USBD_POWERON(drv_info);
					break;
        	}
        	break;

        case DCR_HANDLE:
        	{
        		switch(param->dst.as_int)
        		{
        			case DCR_USB_STALL:		//Causes the given endpoint to acknowledge
        									// the next packet it receives with a STALL handshake.
        			{
						unsigned char eptnum = param->mode0;
					    Endpoint *endpoint = &drv_data->endpoints[eptnum];

					    // Check that endpoint is in Idle state
					    if (endpoint->state != UDP_ENDPOINT_IDLE)
					    {
					    	TRACE_USB_WARNING("USBD_Stall: Endpoint%d locked\r\n", eptnum);
					    } else
					    {
						    TRACE_USB_DEBUG("Stall%d ", eptnum);
						    SET_CSR(&pUDP->UDP_CSR[eptnum], UDP_CSR_FORCESTALL);
					    }
        			}
				    break;

        			case DCR_USB_SET_ADDRESS:	// Sets the device address to the given value.
        			{

    					unsigned char address = param->src.as_int;

    					TRACE_USB_DEBUG("SetAddr(%d) ", address);

					    // Set address
					    pUDP->UDP_FADDR = UDP_FADDR_FEN | address;

					    // If the address is 0, the device returns to the Default state
					    if (address == 0)
					    {

					        pUDP->UDP_GLB_STAT = 0;
					        drv_data->deviceState = USBD_STATE_DEFAULT;
					    }
					    // If the address is non-zero, the device enters the Address state
					    else
					    {

					        pUDP->UDP_GLB_STAT = UDP_GLB_STAT_FADDEN;
					        drv_data->deviceState = USBD_STATE_ADDRESS;
					    }
        			}
        			break;

        			case DCR_USB_SET_CONFIGURATION:							// Sets the current device configuration.
        			{
						unsigned char cfgnum = param->src.as_int;

						TRACE_USB_DEBUG("SetCfg(%d) ", cfgnum);

					    // If the configuration number if non-zero, the device enters the
					    // Configured state
					    if (cfgnum != 0)
					    {
					        drv_data->deviceState = USBD_STATE_CONFIGURED;
					        pUDP->UDP_GLB_STAT |= UDP_GLB_STAT_CONFG;
					    }
					    // If the configuration number is zero, the device goes back to the Address
					    // state
					    else
					    {
					        drv_data->deviceState = USBD_STATE_ADDRESS;
					        pUDP->UDP_GLB_STAT = UDP_GLB_STAT_FADDEN;

					        // Abort all transfers
					        UDP_DisableEndpoints(drv_data);
					    }
					}
        			break;

					case DCR_USB_CONFIGUREENDPOINT:							// Calls USBD_ConfigureEndpoint
					{
						USBD_ConfigureEndpoint(drv_info, (const USBEndpointDescriptor *)param->src.as_voidptr);
					}
					break;

					case DCR_USB_HALT:										// Calls USBD_Halt
					{
						unsigned char eptnum = param->src.as_int;
					    Endpoint *endpoint = &drv_data->endpoints[eptnum];

					    // Check that endpoint is enabled and not already in Halt state
					    if ((endpoint->state != UDP_ENDPOINT_DISABLED)
					        && (endpoint->state != UDP_ENDPOINT_HALTED))
					   	{

					    	TRACE_USB_DEBUG("Halt%d ", eptnum);

					        // Abort the current transfer if necessary
					    	UDP_EndOfTransfer(endpoint, USBD_STATUS_ABORTED);

					        // Put endpoint into Halt state
					        SET_CSR(&pUDP->UDP_CSR[eptnum], UDP_CSR_FORCESTALL);
					        endpoint->state = UDP_ENDPOINT_HALTED;

					        // Enable the endpoint interrupt
					        pUDP->UDP_IER |= 1 << eptnum;
					    }
					}
					break;

					case DCR_USB_UNHALT:									// Calls USBD_Unhalt
					{
						unsigned char eptnum = param->src.as_int;
					    Endpoint *endpoint = &drv_data->endpoints[eptnum];

					    // Check if the endpoint is enabled
//					    if (endpoint->state != UDP_ENDPOINT_DISABLED)
					    if (endpoint->state == UDP_ENDPOINT_HALTED)
					    {
					    	TRACE_USB_DEBUG("Unhalt%d ", eptnum);

					        // Return endpoint to Idle state
					        endpoint->state = UDP_ENDPOINT_IDLE;

					        // Clear FORCESTALL flag
					        CLEAR_CSR(&pUDP->UDP_CSR[eptnum], UDP_CSR_FORCESTALL);

					        // Reset Endpoint Fifos, beware this is a 2 steps operation
					        pUDP->UDP_RST_EP |= 1 << eptnum;
					        pUDP->UDP_RST_EP &= ~(1 << eptnum);
					    }
					}
					break;

        		}

        	}
        	break;

    	case DCR_CLOSE:
        case DCR_CANCEL:
	        {
				unsigned char eptnum = param->mode0;
			    Endpoint *endpoint = &drv_data->endpoints[eptnum];

			    if(!param->list_remove(endpoint->pending))
					    	break;
			    endpoint->state = UDP_ENDPOINT_IDLE;
	        }
        	break;

        case DCR_OPEN:
		  {

			param->mode0 = param->mode.as_int;	// set last used endpoit to read (for stall)
			param->res = RES_OK;
			break;
		  }



    }
}

void USBD_DSR(USBD_INFO drv_info, HANDLE hnd)
{
	unsigned char eptnum;
    Endpoint *endpoint;
    HANDLE prev;
    USBD_DRIVER_DATA drv_data = drv_info->drv_data;

	hnd->next = NULL;
	hnd->res = RES_BUSY;

	if (hnd->cmd & FLAG_LOCK)
	{
		eptnum = hnd->mode0 = hnd->mode.as_int >>16;
		endpoint = &(drv_data->endpoints[eptnum]);
		if (hnd->cmd & FLAG_READ)
		{
			if( (!endpoint->pending) || (endpoint->pending == hnd) )
			{
				endpoint->pending = hnd;
				return;
			}
		}
		svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
		return;
	}

	if (hnd->cmd & FLAG_WRITE)
    {
		eptnum = hnd->mode0 = hnd->mode.as_int >>8;
	    TRACE_USB("Write%d(%d) ", eptnum , hnd->len);
		endpoint = &(drv_data->endpoints[eptnum]);
	    if( (prev=endpoint->pending) )
	    {
	    	while(prev->next)
	    		prev = prev->next;
	    	prev->next = hnd;
	    } else
	    {
		    if (endpoint->state != UDP_ENDPOINT_IDLE)
		    {
				svc_HND_SET_STATUS(hnd, RES_ERROR);
				return;
		    }
		    endpoint->state = UDP_ENDPOINT_SENDING;
		    endpoint->pending = hnd;		//sending
		    UDP_WritePayload(&drv_info->hw_base->UDP_FDR[eptnum], hnd, endpoint->size);
		    SET_CSR(&drv_info->hw_base->UDP_CSR[eptnum], UDP_CSR_TXPKTRDY);
		    // If double buffering is enabled and there is data remaining,
		    // prepare another packet
		    if ((CHIP_USB_ENDPOINTS_BANKS(eptnum) > 1) && hnd->len)
		    {

			    UDP_WritePayload(&drv_info->hw_base->UDP_FDR[eptnum], hnd, endpoint->size);
		    }

		    // Enable interrupt on endpoint
		    drv_info->hw_base->UDP_IER |= 1 << eptnum;
	    }

	    return;
    }


	if (hnd->cmd & FLAG_READ)
    {
		eptnum = hnd->mode0 = hnd->mode.as_int;
		endpoint = &(drv_data->endpoints[eptnum]);
	    // Return if the endpoint is not in IDLE state
	    if (endpoint->state != UDP_ENDPOINT_IDLE)
	    {
			svc_HND_SET_STATUS(hnd, RES_ERROR);
			return;
	    }

	    TRACE_USB("Read%d(%d) ", eptnum, hnd->len);

	    // Endpoint enters Receiving state
	    endpoint->state = UDP_ENDPOINT_RECEIVING;

	    if( (prev=endpoint->pending) )	//receiving
	    {
	    	while(prev->next)
	    		prev = prev->next;
	    	prev->next = hnd;
	    } else
	    	endpoint->pending = hnd;	//receiving

	    // Enable interrupt on endpoint
	    drv_info->hw_base->UDP_IER |= 1 << eptnum;
	    return;
    }


	svc_HND_SET_STATUS(hnd, RES_ERROR);
}

void USBD_ISR(USBD_INFO drv_info)
{
	unsigned int status;
    USBD_DRIVER_DATA drv_data = drv_info->drv_data;
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
        TRACE_USB(".\n\r");
        return;
    }

    /* Resume (Wakeup) */
    if ((status & (UDP_ISR_WAKEUP | UDP_ISR_RXRSM)) != 0)
    {
        TRACE_USB("Res ");
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

        TRACE_USB("Susp ");
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

        TRACE_USB("EoBRes ");
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

                	TRACE_USB("\n\r  - ");
                }
            }
            eptnum++;
        }
    }

}
