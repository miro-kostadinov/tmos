/*
 * usb_hal.cpp
 *
 *  Created on: Dec 3, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <usb_hal.h>
#include <cmsis_cpp.h>

//-------------------  local static functions --------------------------------//

/**
 * Reads a packet from the Rx FIFO
 * @param drv_info
 * @param endpoint
 * @param len
 */
static bool stm_read_payload(USB_DRV_INFO drv_info,	uint32_t status)
{
    Endpoint* endpoint;
    uint32_t bcnt, top_rx_cnt;
    HANDLE hnd;
    __IO uint32_t* fifo = drv_info->hw_base->DFIFO[0].DFIFO;

	endpoint = &drv_info->drv_data->endpoints[status & OTG_GRXSTSP_EPNUM_Msk];
	bcnt = OTG_GRXSTSP_BCNT_Get(status);
	top_rx_cnt = 0;
	TRACE_USB(" que%x(%ub)", status & OTG_GRXSTSP_EPNUM_Msk, bcnt);

	if(endpoint->epd_out.epd_state == ENDPOINT_STATE_IDLE)
	{
		if(bcnt)
		{
			// loop pending handles
			hnd = endpoint->epd_out.epd_pending;
			while( hnd )
			{
				if(!top_rx_cnt)
				{
					// try whole words
					while(hnd->len > 3 && bcnt > 3)
					{
						*hnd->dst.as_intptr++ = *fifo;
						hnd->len -= 4;
						bcnt -= 4;
					}

					if(!bcnt)
					{
						// nothing left in fifo or top
			    		endpoint->epd_out.epd_pending = hnd->next;
						usr_HND_SET_STATUS(hnd, RES_SIG_OK);

						return true;
					}

					// bcnt is >0 and hnd->len is <4
					if(hnd->len)
					{
						// len !=0 -> we must pad
						endpoint->top_rx_word = *fifo;
						top_rx_cnt = (bcnt>3)?4:bcnt;
						bcnt -= top_rx_cnt;
					}
				}

				//process the top word
				while(hnd->len && top_rx_cnt)
				{
					*hnd->dst.as_byteptr++ = endpoint->top_rx_word;
					endpoint->top_rx_word >>= 8;
					top_rx_cnt--;
					hnd->len--;
				}

				if(hnd->len == 0)
				{
		    		endpoint->epd_out.epd_pending = hnd->next;
					usr_HND_SET_STATUS(hnd, RES_SIG_OK);
					hnd = endpoint->epd_out.epd_pending;
				}
			}

			if(bcnt + top_rx_cnt)
			{
				endpoint->epd_out.epd_state = ENDPOINT_STATE_RECEIVING_OFF;
				endpoint->rxfifo_cnt = bcnt;
				endpoint->top_rx_cnt = top_rx_cnt;
				return false;
			}
		}
	} else
	{
		TRACE_USB("usb drop %X %u bytes ep state=%u", status, bcnt, endpoint->epd_out.epd_state);
		bcnt = (bcnt+3)/4;
		while(bcnt--)
			status = *fifo;
	}
	return true;
}

/**
 * Try to load next packet in the FIFO
 * aka DCD_WriteEmptyTxFifo
 * @param drv_info
 * @param ept_indx
 */
static void stm_write_payload(USB_DRV_INFO drv_info, uint32_t ept_indx)
{
    ep_dir_state_t* epdir;
    HANDLE hnd;
	uint32_t len, len32b, txsize;
    __IO uint32_t *fifo;

	epdir = &(drv_info->drv_data->endpoints[ept_indx].epd_in);
	hnd = epdir->epd_pending;
	if(hnd)
	{
		fifo = drv_info->hw_base->DFIFO[ept_indx].DFIFO;
		txsize = drv_info->hw_base->in_ept_regs[ept_indx].DTXFSTS & DTXFSTS_INEPTFSAV_Msk;

		TRACE_USB(" Wr%d(%d) ", ept_indx , hnd->len);
		while( (len = hnd->len) )
		{
			if (len > epdir->epd_fifo_sz)
				len = epdir->epd_fifo_sz;

			len32b = (len + 3) / 4;
			if (txsize <= len32b)
				break;
			txsize -= len32b;

			while(len32b--)
				fifo[0] = *hnd->src.as_intptr++;
			hnd->len -= len;
		}

		if(len)
		{
			if(epdir->epd_type != ENDPOINT_TYPE_ISOCHRONOUS)
			{
				drv_info->hw_base->device_regs.DIEPEMPMSK |= OTG_DIEPEMPMSK_INEPTXFEM(ept_indx);
			}
			return;
		}
	} else
	{
		TRACELN1_USB("usb tx emp !h");
	}
	drv_info->hw_base->device_regs.DIEPEMPMSK &= ~OTG_DIEPEMPMSK_INEPTXFEM(ept_indx);
}

static void stm_start_tx(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t eptnum, ep_dir_state_t* epdir)
{
	uint32_t reg;
	USB_TypeDef* otg = drv_info->hw_base;

	// 1. Program the transfer size...
	if (hnd->len == 0)
	{
		reg = otg->in_ept_regs[eptnum].DIEPTSIZ;
		reg &= OTG_DIEPTSIZ_MCNT_Msk;
		reg |= OTG_DIEPTSIZ_PKTCNT(1);
	} else
	{
		/* Program the transfer size and packet count
		 * as follows: xfersize = N * maxpacket +
		 * short_packet pktcnt = N + (short_packet
		 * exist ? 1 : 0)
		 */
		reg = hnd->len;
		reg |= OTG_DIEPTSIZ_PKTCNT((hnd->len + epdir->epd_fifo_sz -1)/epdir->epd_fifo_sz);

		if (epdir->epd_type == ENDPOINT_TYPE_ISOCHRONOUS)
		{
			reg |= OTG_DIEPTSIZ_MCNT(1);
		}
	}
	otg->in_ept_regs[eptnum].DIEPTSIZ = reg;

	// 2. Enable endpoint, clear NAK etc..
	reg = otg->in_ept_regs[eptnum].DIEPCTL;
	if (epdir->epd_type == ENDPOINT_TYPE_ISOCHRONOUS)
	{
		if (otg->device_regs.DSTS & OTG_DSTS_FNSOF_ODD)
		{
			reg |= OTG_DIEPCTL_SD0PID;
		}
		else
		{
			reg |= OTG_DIEPCTL_SD1PID;
		}
	}
	otg->in_ept_regs[eptnum].DIEPCTL = reg | OTG_DIEPCTL_CNAK | OTG_DIEPCTL_EPENA;

	// 3. Write some data
	stm_write_payload(drv_info, eptnum);
}

static void stm_start_rx(USB_DRV_INFO drv_info, uint32_t ept_indx, Endpoint *endpoint)
{
	HANDLE hnd;

	hnd = endpoint->epd_out.epd_pending;
	if(hnd && (!ept_indx || drv_info->drv_data->usb_state >= USBST_DEVICE_ADDRESS))
	{
    	OTG_OUT_EPT_REGS* otg_regs;
    	uint32_t depctl, deptsiz, maxpacket;

    	//start xfer
    	otg_regs = &drv_info->hw_base->out_ept_regs[ept_indx];
    	depctl = otg_regs->DOEPCTL;
    	if( !( depctl & OTG_DOEPCTL_EPENA) )
    	{
    		deptsiz = otg_regs->DOEPTSIZ;
    		TRACELN_USB("usb sr%u sz=%08x", ept_indx, deptsiz);
    		deptsiz &= OTG_DOEPTSIZ_STUPCNT_Msk;

    	    /* Program the transfer size and packet count as follows:
    	    * pktcnt = N
    	    * xfersize = N * maxpacket
    	    */
    		maxpacket = endpoint->epd_out.epd_fifo_sz;
    		if(!maxpacket)
    			maxpacket = 64;
    	    if (hnd->len == 0)
    	    {
    	    	deptsiz |= OTG_DOEPTSIZ_PKTCNT(1) | OTG_DOEPTSIZ_XFRSIZ(maxpacket);
    	    }
    	    else
    	    {
    	    	uint32_t pktcnt;
    	      	pktcnt = (hnd->len + (maxpacket - 1)) / maxpacket;
    	    	deptsiz |= OTG_DOEPTSIZ_PKTCNT(pktcnt) | OTG_DOEPTSIZ_XFRSIZ(pktcnt*maxpacket);
    	    }
    	    depctl |= OTG_DOEPCTL_EPENA | OTG_DOEPCTL_CNAK;
    		otg_regs->DOEPTSIZ = deptsiz;
	    	otg_regs->DOEPCTL = depctl;

    	} else
    	{
    		TRACELN_USB("usb rr%u sz=%08x", ept_indx, otg_regs->DOEPTSIZ);
    	}
	}
}

static void stm_uDelay (uint32_t usec)
{
	usec ++;
	usec *= 16;
	while(usec--)
		__NOP();
}

/** Initializes the common interrupts, used in both device and modes
 *
 * @param otg
 * @param cfg
 */
static void stm_enable_common_int(USB_TypeDef* otg, uint32_t cfg)
{
	uint32_t mask;

	/* Clear any pending USB_OTG Interrupts */
    if( cfg & CFG_STM32_OTG_MODE)
    {
    	otg->core_regs.GOTGINT = 0xFFFFFFFF;
    }

    /* Clear any pending interrupts */
	otg->core_regs.GINTSTS = 0xBFFFFFFF;

	mask = OTG_GINTMSK_WKUM | OTG_GINTMSK_USBSUSPM;
    if( cfg & CFG_STM32_OTG_MODE)
    {
    	mask |= OTG_GINTMSK_OTGM | OTG_GINTMSK_SRQM | OTG_GINTMSK_CIDSCHGM;
    }
	otg->core_regs.GINTMSK = mask;
}

/**
* Soft reset of the core
*/
static void stm_otg_core_reset(USB_TypeDef* otg)
{
	uint32_t count = 0;

	/* Wait for AHB master IDLE state. */
	while (!(otg->core_regs.GRSTCTL & OTG_GRSTCTL_AHBIDL))
	{
		if (++count > 200000)
			break;
		stm_uDelay(3);
	}

	/* Core Soft Reset */
	otg->core_regs.GRSTCTL |= OTG_GRSTCTL_CSRST;
	count = 0;
	while (otg->core_regs.GRSTCTL & OTG_GRSTCTL_CSRST)
	{
		if (++count > 200000)
			break;
	}

	/* Wait for 3 PHY Clocks*/
	stm_uDelay(3);
}

static void stm_otg_core_init(USB_TypeDef* otg, uint32_t cfg)
{
	uint32_t reg;

	if(cfg & CFG_STM32_OTG_ULPI)
	{

		reg = otg->core_regs.GCCFG;

	    reg &= ~OTG_GCCFG_PWRDWN;	// Transceiver Power down active

	    if (cfg & CFG_STM32_OTG_SOF_OUTPUT)
	    {
			reg |= OTG_GCCFG_SOFOUTEN;
	    }
		otg->core_regs.GCCFG = reg;

	    /* Init The ULPI Interface */
		reg = otg->core_regs.GUSBCFG;

		reg &= ~OTG_GUSBCFG_PHYSEL; /* HS Interface */
		reg |= OTG_GUSBCFG_PHYSEL_ULPI;

		if(cfg & CFG_STM32_OTG_INTERNAL_VBUS)
			reg &= ~OTG_GUSBCFG_ULPIEVBUSD; /* Use internal VBUS */
		else
			reg |= OTG_GUSBCFG_ULPIEVBUSD;  /* Use external VBUS */

		reg &= ~OTG_GUSBCFG_TSDPS;     /* Data line pulsing using utmi_txvalid */

		reg &= ~OTG_GUSBCFG_ULPIFSLS;
		reg &= ~OTG_GUSBCFG_ULPICSM;
		otg->core_regs.GUSBCFG = reg;

	    /* Reset after a PHY select  */
		stm_otg_core_reset(otg);
	} else
	{
		otg->core_regs.GUSBCFG |= OTG_GUSBCFG_PHYSEL_FSSER; /* FS Interface */

		/* Reset after a PHY select and set Host mode */
		stm_otg_core_reset(otg);

		/* Deactivate the power down*/
		reg = OTG_GCCFG_PWRDWN | OTG_GCCFG_VBUSBSEN | OTG_GCCFG_VBUSASEN;

		if (!(cfg & CFG_STM32_OTG_VBUS_SENS))
			reg |= OTG_GCCFG_NOVBUSSENS;

		if (cfg & CFG_STM32_OTG_SOF_OUTPUT)
		{
			reg |= OTG_GCCFG_SOFOUTEN;
		}
		otg->core_regs.GCCFG = reg;

	}

	/* initialize OTG features */
    if( cfg & CFG_STM32_OTG_MODE)
    {
    	otg->core_regs.GUSBCFG |= OTG_GUSBCFG_HNPCAP | OTG_GUSBCFG_SRPCAP;
    	stm_enable_common_int(otg, cfg);
    }
}

/**
 *
 * @param otg
 * @param mode - select OTG_GUSBCFG_FDMOD or OTG_GUSBCFG_FHMOD
 */
static void stm_set_current_mode(USB_CONTROLLER* otg , uint32_t mode)
{
	uint32_t reg;

	reg = otg->core_regs.GUSBCFG;
	reg &= ~(OTG_GUSBCFG_FDMOD | OTG_GUSBCFG_FHMOD);
	otg->core_regs.GUSBCFG = reg | mode;

	tsk_sleep(50);
}

/** USB_OTG_InitDevSpeed :Initializes the DevSpd field of DCFG register
 * 	depending the PHY type and the enumeration speed of the device.
 * @param otg
 * @param speed = OTG_DCFG_DSPD_HS or OTG_DCFG_DSPD_HS_FS or OTG_DCFG_DSPD_FS11
 */
static void stm_init_dev_speed(USB_TypeDef* otg, uint32_t speed)
{
	uint32_t reg;

	reg = otg->device_regs.DCFG;
	reg &= ~OTG_DCFG_DSPD_Msk;
	otg->device_regs.DCFG = reg | speed;
}

/** Flush a Tx FIFO
 *
 * @param otg
 * @param num - fifo
 */
static void stm_flush_tx_fifo(USB_TypeDef* otg, uint32_t num )
{
	uint32_t count = 0;

	otg->core_regs.GRSTCTL = OTG_GRSTCTL_TXFFLSH | OTG_GRSTCTL_TXFNUM(num);
	while(otg->core_regs.GRSTCTL & OTG_GRSTCTL_TXFFLSH)
	{
		if (++count > 200000)
			break;
	}
	/* Wait for 3 PHY Clocks*/
	stm_uDelay(3);
}

/**
 * Flush a Rx FIFO
 * @param otg
 */
static void stm_flush_rx_fifo( USB_TypeDef* otg )
{
	uint32_t count = 0;

	otg->core_regs.GRSTCTL = OTG_GRSTCTL_RXFFLSH;
	while(otg->core_regs.GRSTCTL & OTG_GRSTCTL_RXFFLSH)
	{
		if (++count > 200000)
			break;
	}

	/* Wait for 3 PHY Clocks*/
	stm_uDelay(3);
}

/**
 * Enables the Device mode interrupts
 * @param otg
 * @param cfg
 */
static void stm_enable_dev_int(USB_TypeDef* otg, uint32_t cfg)
{
	uint32_t mask;

	/* Disable all interrupts. */
	otg->core_regs.GINTMSK = 0;

	/* Clear any pending interrupts */
	otg->core_regs.GINTSTS = 0xBFFFFFFF;

	/* Enable the common interrupts */
	stm_enable_common_int(otg, cfg);

	/* Enable interrupts matching to the Device mode ONLY */
	mask = OTG_GINTMSK_USBSUSPM | OTG_GINTMSK_USBRSTM | OTG_GINTMSK_ENUMDNEM |
			OTG_GINTMSK_IEPM | OTG_GINTMSK_OEPM | OTG_GINTMSK_SOFM |
			OTG_GINTMSK_IISOOXFRM | OTG_GINTMSK_IISOIXFRM | OTG_GINTMSK_RXFLVLM;

    if(cfg & CFG_STM32_OTG_VBUS_SENS)
    	mask |= OTG_GINTMSK_SRQM | OTG_GINTMSK_OTGM;

	otg->core_regs.GINTMSK = mask;
}



static void stm_otg_core_init_dev(USB_TypeDef* otg, const usb_config_t* cfg)
{
	uint32_t reg, adr;
	uint32_t dev_endpoints;

	/* Restart the Phy Clock */
	otg->PCGCCTL = 0;

	/* Device configuration register */
	reg = otg->device_regs.DCFG;
	reg &= ~OTG_DCFG_PFIVL_Msk;
	reg |= OTG_DCFG_PFIVL_80;
	otg->device_regs.DCFG = reg;


	if(cfg->stm32_otg & CFG_STM32_OTG_HS_CORE)
	{
		// HS core

		/* Set High speed phy */
		if(cfg->stm32_otg & CFG_STM32_OTG_ULPI)
		{
			stm_init_dev_speed(otg, OTG_DCFG_DSPD_HS);
		}
		else /* set High speed phy in Full speed mode */
		{
			stm_init_dev_speed(otg, OTG_DCFG_DSPD_FS11);
		}

	    dev_endpoints    = 6-1;
	} else
	{
		// FS core
		/* Set Full speed phy */
		stm_init_dev_speed(otg, OTG_DCFG_DSPD_FS11);

	    dev_endpoints    = 4-1;
	}

	adr = cfg->rx_fifo_sz;
	/* set Rx FIFO size */
	otg->core_regs.GRXFSIZ = adr;

	/* EP0 TX*/
	otg->core_regs.DIEPTXF0_HNPTXFSIZ = adr + OTG_DIEPTXF_INEPTXFD(cfg->tx_fifo_sz[0]);

	/* EP1 - EP5 TX*/
	for(uint32_t i=0; i<dev_endpoints; i++)
	{
		adr += cfg->tx_fifo_sz[i];
		otg->core_regs.DIEPTXF[i] = adr + OTG_DIEPTXF_INEPTXFD(cfg->tx_fifo_sz[i+1]);
	}


	/* Flush the FIFOs */
	stm_flush_tx_fifo(otg, 0x10); /* all Tx FIFOs */
	stm_flush_rx_fifo(otg);
	/* Clear all pending Device Interrupts */
	otg->device_regs.DAINT		= 0xFFFFFFFF;
	otg->device_regs.DAINTMSK	= 0;

	for (uint32_t i = 0; i < dev_endpoints; i++)
	{
		reg = otg->in_ept_regs[i].DIEPCTL;
		if(reg & OTG_DIEPCTL_EPENA)
		{
			reg = OTG_DIEPCTL_EPDIS | OTG_DIEPCTL_SNAK;
		} else
		{
			reg = 0;
		}
		otg->in_ept_regs[i].DIEPCTL = reg;
		otg->in_ept_regs[i].DIEPINT = 0xFF;

		reg = otg->out_ept_regs[i].DOEPCTL;
		if(reg & OTG_DOEPCTL_EPENA)
		{
			reg = OTG_DOEPCTL_EPDIS | OTG_DOEPCTL_SNAK;
		} else
		{
			reg = 0;
		}
		otg->out_ept_regs[i].DOEPCTL = reg;
		otg->out_ept_regs[i].DOEPINT = 0xFF;
	}
	otg->device_regs.DIEPMSK = OTG_DIEPMSK_BIM | OTG_DIEPMSK_TXFURM |
			OTG_DIEPMSK_TOCM | OTG_DIEPMSK_EPDM |OTG_DIEPMSK_XFRCM;
#if USB_STM32_DEDICATED_EP1
	otg->device_regs.DINEP1MSK = OTG_DIEPMSK_BIM | OTG_DIEPMSK_TXFURM |
			OTG_DIEPMSK_TOCM | OTG_DIEPMSK_EPDM | OTG_DIEPMSK_XFRCM;
#endif
	otg->device_regs.DOEPMSK = OTG_DOEPMSK_BOIM | OTG_DOEPMSK_OPEM |
			OTG_DOEPMSK_B2BSTUP | OTG_DOEPMSK_EPDM;
#if USB_STM32_DEDICATED_EP1
	otg->device_regs.DOUTEP1MSK = OTG_DOEPMSK_BOIM | OTG_DOEPMSK_OPEM |
			OTG_DOEPMSK_B2BSTUP | OTG_DOEPMSK_EPDM;
#endif
	stm_enable_dev_int(otg, cfg->stm32_otg);
}

/**
 * active USB Core clock
 * @param otg
 * @param cfg
 */
static void stm_otg_ungate_clock(USB_TypeDef* otg, uint32_t cfg)
{
	if(cfg & CFG_STM32_OTG_LOW_POWER)
	{
		if(otg->device_regs.DSTS & OTG_DSTS_SUSPSTS)
		{
			/* un-gate USB Core clock */
			otg->PCGCCTL &= ~(OTG_PCGCCTL_GATEHCLK | OTG_PCGCCTL_STPPCLK);
		}
	}
}

static void stm_ept_config(USB_DRV_INFO drv_info, uint32_t ept_num,
		const USBGenericDescriptor* pDescriptor)
{
	ep_dir_state_t* epdir;
	uint32_t ept_indx = ept_num & 0xF;
	USB_TypeDef* otg = drv_info->hw_base;
	__IO uint32_t* depctl;
	uint32_t reg, ept_type, fifo_sz;

    if(ept_indx >= USB_NUMENDPOINTS)
    {
		TRACELN1_USB("Invalid endpoint descriptor!");

    } else
    {
        TRACE_USB(" CfgEp(%x)", ept_num);

        /* Abort the current transfer is the endpoint was configured and in
    	 Write or Read state */
    	usb_hal_ept_reset(drv_info, ept_num);

        // set fifo size
    	ept_type = ENDPOINT_TYPE_CONTROL;
    	switch(pDescriptor->bDescriptorType)
    	{
    	case DEVICE_DESCRIPTOR:
        	fifo_sz = ((USBDeviceDescriptor *)pDescriptor)->bMaxPacketSize0;
        	break;

    	case ENDPOINT_DESCRIPTOR:
    		fifo_sz = ((const USBEndpointDescriptor*)pDescriptor)->wMaxPacketSize;
    		ept_type = ((const USBEndpointDescriptor*)pDescriptor)->GetType();
    		break;

    	default:
    		fifo_sz = 0;
        }

        if(ept_num & 0x80)
        {
            epdir = &drv_info->drv_data->endpoints[ept_indx].epd_in;
#if USB_STM32_DEDICATED_EP1
            if(ept_indx == EPT_1 && drv_info->cfg->stm32_otg & CFG_STM32_OTG_DEDICATED_EP1)
            	otg->device_regs.DEACHINTMSK |= OTG_DEACHINTMSK_IEP1INT;
            else
#endif
            otg->device_regs.DAINTMSK |= OTG_DAINTMSK_IEPM(ept_indx);

        	depctl = &otg->in_ept_regs[ept_indx].DIEPCTL;
        	reg = *depctl;
        	if(!(reg & OTG_DIEPCTL_USBAEP))
        	{
        		reg &= ~(OTG_DIEPCTL_TXFNUM_Msk | OTG_DIEPCTL_EPTYP_Msk |
        				OTG_DIEPCTL_MPSIZ_Msk);
        		reg |= OTG_DIEPCTL_SD0PID | OTG_DIEPCTL_TXFNUM(ept_indx) |
        				OTG_DIEPCTL_EPTYP(ept_type) | OTG_DIEPCTL_USBAEP |
        				OTG_DIEPCTL_MPSIZ(fifo_sz);
            	*depctl = reg;
        	}
        } else
        {
            epdir = &drv_info->drv_data->endpoints[ept_indx].epd_out;
#if USB_STM32_DEDICATED_EP1
            if(ept_indx == EPT_1 && drv_info->cfg->stm32_otg & CFG_STM32_OTG_DEDICATED_EP1)
            	otg->device_regs.DEACHINTMSK |= OTG_DEACHINTMSK_OEP1INTM;
            else
#endif
           	otg->device_regs.DAINTMSK |= OTG_DAINTMSK_OEPM(ept_indx);
        	depctl = &otg->out_ept_regs[ept_indx].DOEPCTL;
        	reg = *depctl;
        	if(!(reg & OTG_DOEPCTL_USBAEP))
        	{
        		reg &= ~( OTG_DOEPCTL_EPTYP_Msk | OTG_DOEPCTL_MPSIZ_Msk);
        		reg |= OTG_DOEPCTL_SD0PID | OTG_DOEPCTL_EPTYP(ept_type) |
        				OTG_DOEPCTL_USBAEP | OTG_DOEPCTL_MPSIZ(fifo_sz);
            	*depctl = reg;
        	}
        }

		epdir->epd_fifo_sz = fifo_sz;
    	epdir->epd_state = ENDPOINT_STATE_IDLE;

#if USB_STM32_DEDICATED_EP1
    	if(drv_info->cfg->stm32_otg & CFG_STM32_OTG_DEDICATED_EP1)
    	{
    		switch(ept_num)
    		{
    		case 0x01:
    			NVIC->NVIC_EnableIRQ(OTG_HS_EP1_OUT_IRQn);
    			break;
    		case 0x81:
    			NVIC->NVIC_EnableIRQ(OTG_HS_EP1_IN_IRQn);
    			break;
    		}

    	}
#endif
    }
}



//---------------  USB driver related functions ------------------------------//
WEAK_C void usb_drv_event(USB_DRV_INFO drv_info, USB_EVENT event)
{
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;

	switch(event)
	{
	case e_powered:
		drv_data->usb_state = USBST_DEVICE_POWERED;
		break;

	case e_susppend:
		if(drv_data->usb_state > USBST_DEVICE_SUSPENDED)
		{
			drv_data->usb_previous_state = drv_data->usb_state;
			drv_data->usb_state = USBST_DEVICE_SUSPENDED;
		}
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

/**
 * Called once after boot (on DCR_RESET) to reset the driver
 * @param drv_info
 */
void usb_drv_reset(USB_DRV_INFO drv_info)
{
    TRACELN1_USB("USBD_Init");
	NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);
	if(drv_info->info.peripheral_indx != ID_NO_PERIPH)
	{
    	Task* task;

		task = usr_task_create_dynamic("USBH", (TASK_FUNCTION)usbdrv_thread,
				60,	400+TRACE_SIZE);
		if(task)
		{
			svc_task_schedule(task);
           	task->sp->r0.as_cvoidptr = drv_info;
		}
	}
}

void usb_drv_cancel_hnd(USB_DRV_INFO drv_info, HANDLE hnd)
{
    ep_dir_state_t* epdir;

    if(hnd->res & FLG_BUSY)
    {
    	if(hnd->cmd & FLAG_WRITE)
    	{
    		epdir = &(drv_info->drv_data->endpoints[hnd->mode.as_bytes[1]].epd_in);
    	} else
    	{
    		epdir = &(drv_info->drv_data->endpoints[hnd->mode.as_bytes[0]].epd_out);
    	}
		if(hnd->list_remove(epdir->epd_pending))
		{
			svc_HND_SET_STATUS(hnd, FLG_SIGNALED | (hnd->res & FLG_OK));
			TRACE1_USB("Can!");
			if(!epdir->epd_pending && epdir->epd_state == ENDPOINT_STATE_SENDING)
				epdir->epd_state = ENDPOINT_STATE_IDLE;

		}
    }
}

/**
 * Start sending...
 *
 * STM32 logic:
 * 	1. Program the transfer size...
 * 	2. Enable endpoint
 * 	3. write some payload data
 * 	4. On tx fifo empty (TXFE) continue to write
 * 	5. On transf. complete (XFRC) signal the client
 *
 * @param drv_info
 * @param hnd
 */
void usb_drv_start_tx(USB_DRV_INFO drv_info, HANDLE hnd)
{
	ep_dir_state_t* epdir;
	uint32_t eptnum;
	HANDLE prev;

	eptnum = hnd->mode.as_bytes[1];
//    TRACE_USB(" Write%d(%d) ", eptnum , hnd->len);
	epdir = &(drv_info->drv_data->endpoints[eptnum].epd_in);
    if( (prev=epdir->epd_pending) )
    {
    	while(prev->next)
    		prev = prev->next;
    	prev->next = hnd;
    } else
    {
	    if (epdir->epd_state != ENDPOINT_STATE_IDLE)
	    {
			svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
	    } else
	    {
	    	epdir->epd_state = ENDPOINT_STATE_SENDING;
		    epdir->epd_pending = hnd;		//sending

		    stm_start_tx(drv_info, hnd, eptnum, epdir);
	    }
    }
}

void usb_drv_start_rx(USB_DRV_INFO drv_info, HANDLE hnd)
{
	uint32_t eptnum;
	Endpoint *endpoint;

	eptnum = hnd->mode.as_bytes[0];
	endpoint = &drv_info->drv_data->endpoints[eptnum];

	if(hnd->len)
	{
		if(endpoint->epd_out.epd_state == ENDPOINT_STATE_RECEIVING_OFF)
		{
		    uint32_t bcnt, top_rx_cnt;
		    __IO uint32_t* fifo = drv_info->hw_base->DFIFO[0].DFIFO;

			bcnt = endpoint->rxfifo_cnt;
			top_rx_cnt = endpoint->top_rx_cnt;
			while(hnd->len)
			{
				if(!top_rx_cnt)
				{
					// try whole words
					while(hnd->len > 3 && bcnt > 3)
					{
						*hnd->dst.as_intptr++ = *fifo;
						hnd->len -= 4;
						bcnt -= 4;
					}

					if(!bcnt)
					{
						// nothing left in fifo or top
						break;
					}

					// bcnt is >0 and hnd->len is <4
					if(hnd->len)
					{
						// len !=0 -> we must pad
						endpoint->top_rx_word = *fifo;
						top_rx_cnt = (bcnt>3)?4:bcnt;
						bcnt -= top_rx_cnt;
					}
				}

				//process the top word
				while(hnd->len && top_rx_cnt)
				{
					*hnd->dst.as_byteptr++ = endpoint->top_rx_word;
					endpoint->top_rx_word >>= 8;
					top_rx_cnt--;
					hnd->len--;
				}
			}
			endpoint->rxfifo_cnt = bcnt;
			endpoint->top_rx_cnt = top_rx_cnt;
			if( !(bcnt + top_rx_cnt) )
			{
	        	endpoint->epd_out.epd_state = ENDPOINT_STATE_IDLE;
				drv_info->hw_base->core_regs.GINTMSK |= OTG_GINTMSK_RXFLVLM;
			}
		} else
		{
		    HANDLE prev;

		    if( (prev=endpoint->epd_out.epd_pending) )	//receiving
		    {
		    	while(prev->next)
		    		prev = prev->next;
		    	prev->next = hnd;
		    } else
		    {
		    	endpoint->epd_out.epd_pending = hnd;	//receiving
		    	stm_start_rx(drv_info, eptnum, endpoint);
		    }
		    return;
		}
	}
	svc_HND_SET_STATUS(hnd, RES_SIG_OK);

}

void usb_drv_end_transfers(ep_dir_state_t* epdir, unsigned int status)
{
    HANDLE hnd;

	while( (hnd =  epdir->epd_pending) )
	{
		TRACE1_USB(" EoT ");
		epdir->epd_pending = hnd->next;
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

bool usb_hal_get_ep_status(USB_DRV_INFO drv_info, uint8_t ept_num, uint16_t* data)
{
	uint32_t ept_indx = ept_num & 0xF;

	// Check if the endpoint exists
    if (ept_indx  > USB_NUMENDPOINTS)
    {
    	return false;
    }
    else
    {
        // Check if the endpoint if currently halted
    	if(ept_num & 0x80)
    	{
    		// IN direction
    		ept_indx = drv_info->drv_data->endpoints[ept_indx].epd_in.epd_state;
    	} else
    	{
    		ept_indx = drv_info->drv_data->endpoints[ept_indx].epd_out.epd_state;
    	}
        if (ept_indx  == ENDPOINT_STATE_HALTED)
        {
            *data = 1;
        } else
        {
        	*data = 0;
        }

    }
    return true;
}


void usb_hal_stall(USB_TypeDef* hw_base, unsigned int ept_num)
{
	uint32_t ept_indx = ept_num & 0xF;

	if (ept_num & 0x80)
	{
		uint32_t value;
		__IO uint32_t* reg;

		reg = &hw_base->in_ept_regs[ept_indx].DIEPCTL;

		value = *reg;
		if(value & OTG_DIEPCTL_EPENA)
			value |= OTG_DIEPCTL_EPDIS;
		value |= OTG_DIEPCTL_STALL;

		*reg = value;
	} else
	{
		hw_base->out_ept_regs[ept_indx].DOEPCTL |= OTG_DOEPCTL_STALL;
	}

}

void usb_hal_stall_clear(USB_TypeDef* hw_base, unsigned int ept_num)
{
	uint32_t value;
	__IO uint32_t* reg;
	uint32_t ept_indx = ept_num & 0xF;

	if (ept_num & 0x80)
	{
		reg = &hw_base->in_ept_regs[ept_indx].DIEPCTL;
	} else
	{
		reg = &hw_base->out_ept_regs[ept_indx].DOEPCTL;

	}
	value = *reg;
	value &= ~OTG_DIEPCTL_STALL;

	if(ept_num >0) // this should be for interrupt/bulk
		value |= OTG_DIEPCTL_SD0PID; // set endpoint data PID to DATA0

	*reg = value;
}


/** Configure as device
 * called from the usbdrv thread
 * @param drv_info
 */
void usb_hal_device_start(USB_DRV_INFO drv_info)
{
	USB_CONTROLLER* otg = drv_info->hw_base;

    // Enable Clocking to the USB controller.
	RCCPeripheralEnable(drv_info->info.peripheral_indx);
	RCCPeripheralReset(drv_info->info.peripheral_indx);


	// Enable USB related pins
	PIO_Cfg_List(drv_info->cfg->usb_pins);

	// disable global interrupts
	otg->core_regs.GAHBCFG &= ~OTG_GAHBCFG_GINTMSK;

	// init core
	stm_otg_core_init(otg, drv_info->cfg->stm32_otg);

	// force device mode
	stm_set_current_mode(otg, OTG_GUSBCFG_FDMOD);

	// init device
	stm_otg_core_init_dev(otg, drv_info->cfg);

	// enable global ints
	otg->core_regs.GAHBCFG |= OTG_GAHBCFG_GINTMSK;

	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);
}



void usb_hal_ept_reset(USB_DRV_INFO drv_info, unsigned int ept_num)
{
	USB_TypeDef* otg = drv_info->hw_base;
	uint32_t ept_indx = ept_num & 0xF;
	ep_dir_state_t* epdir;

	// - clear status and flush
	if(ept_num & 0x80)
	{
        epdir = &drv_info->drv_data->endpoints[ept_indx].epd_in;
		otg->in_ept_regs[ept_indx].DIEPINT = 0xFF;
		stm_flush_tx_fifo(otg, ept_indx);

		// - disable ints...
		otg->device_regs.DAINT = OTG_DAINT_IEPINT(ept_indx);
		otg->device_regs.DAINTMSK &= ~OTG_DAINTMSK_IEPM(ept_indx);
	} else
	{
        epdir = &drv_info->drv_data->endpoints[ept_indx].epd_out;
		otg->out_ept_regs[ept_indx].DOEPINT = 0xFF;

		// - disable ints...
		otg->device_regs.DAINT = OTG_DAINT_OEPINT(ept_indx);
		otg->device_regs.DAINTMSK &= ~OTG_DAINTMSK_OEPM(ept_indx);
	}


	// - cancel transfers...
	usb_drv_end_transfers(epdir, USBD_STATUS_RESET);
    epdir->epd_state = ENDPOINT_STATE_DISABLED;


}

/**
 * Configures an endpoint according to its Endpoint Descriptor.
 * \param pDescriptor Pointer to an Endpoint descriptor.
 */
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor)
{
	const USBEndpointDescriptor* descriptor;

	switch(pDescriptor->bDescriptorType)
	{
	case DEVICE_DESCRIPTOR:
		stm_ept_config(drv_info, 0, pDescriptor);
		stm_ept_config(drv_info, 0x80, pDescriptor);
        break;

	case ENDPOINT_DESCRIPTOR:
		descriptor = (const USBEndpointDescriptor*)pDescriptor;

		stm_ept_config(drv_info, descriptor->bEndpointAddress, pDescriptor);

		break;

	default:
		TRACELN1_USB("Invalid descriptor!");
		return;
	}

}


void usb_hal_config_fifo(USB_DRV_INFO drv_info)
{
	uint32_t adr, size;
	USB_TypeDef* otg = drv_info->hw_base;
	USB_DRIVER_DATA* drv_data = drv_info->drv_data;


	/* EP0 TX*/
	adr = drv_info->cfg->rx_fifo_sz;
	size = drv_data->endpoints[0].epd_in.epd_fifo_sz;
	TRACELN_USB("0:%x -> %x", otg->core_regs.DIEPTXF0_HNPTXFSIZ, adr + OTG_DIEPTXF_INEPTXFD(size));
	otg->core_regs.DIEPTXF0_HNPTXFSIZ = adr + OTG_DIEPTXF_INEPTXFD(size);

	/* EP1 - EP5 TX*/
	for(uint32_t i=0; i<USB_NUMENDPOINTS; i++)
	{
		adr += size;
		size = drv_data->endpoints[i+1].epd_in.epd_fifo_sz;
		TRACELN_USB("%u:%x -> %x", i+1, otg->core_regs.DIEPTXF[i], adr + OTG_DIEPTXF_INEPTXFD(size));
		otg->core_regs.DIEPTXF[i] = adr + OTG_DIEPTXF_INEPTXFD(size);
	}

}


/**
 * Handles out endpoint interrupts, except endpoint 1
 *
 * (DCD_HandleOutEP_ISR)
 * @param drv_info
 */
static void usb_b_ept_rx_handler(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;
	uint32_t status, doepint;
	uint32_t ept_indx = 0;


	/* Read in the device interrupt bits */
	status = otg->device_regs.DAINT;
	status &= otg->device_regs.DAINTMSK;
	status >>= 16;

	while (status)
	{
		if (status & 0x1)
		{
			doepint = otg->out_ept_regs[ept_indx].DOEPINT;
			doepint &= otg->device_regs.DOEPMSK;
			/* Clear the bit in DOEPINTn for this interrupt */
			otg->out_ept_regs[ept_indx].DOEPINT = doepint;

	    	TRACE_USB(" [R%x=", ept_indx);
		    TRACE_USB("%02x]", doepint);

			/* Transfer complete */
			if (doepint & OTG_DOEPINT_XFRC)
			{

				/* RX COMPLETE */
				if(drv_info->drv_data->endpoints[ept_indx].epd_out.epd_pending)
				{
			    	TRACE1_USB(" rcv comp");
				} else
				{
			    	TRACE1_USB(" rcv comp!");

				}
			}
			/* Endpoint disable  */
			if (doepint & OTG_DOEPINT_EPDISD)
			{
			}
			/* Setup Phase Done (control EPs) */
			if (doepint & OTG_DOEPINT_STUP)
			{
				/* inform the upper layer that a setup packet is available */
				/* SETUP COMPLETE */
		    	TRACE1_USB(" stp done");
			}
		}
		ept_indx++;
		status >>= 1;
	}
}

#if USB_STM32_DEDICATED_EP1
/**
 * handles EP1_OUT Handler
 * (USBD_OTG_EP1OUT_ISR_Handler)
 * @param drv_info
 */
void USB_EP1_OUT_ISR(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;
	uint32_t status;

	status = otg->out_ept_regs[1].DOEPINT;
	status &= otg->device_regs.DOUTEP1MSK;
	otg->out_ept_regs[1].DOEPINT = status;
	TRACELN_USB("USB1: [out %02X]", status);

	if(status & OTG_DOEPINT_XFRC)		// Transfer completed interrupt
	{
		// upper ...
		TRACELN1_USB("usb 1o xfrc");
	}

	if (status & OTG_DOEPINT_EPDISD) 	// Endpoint disabled interrupt
	{
		TRACELN1_USB("usb 1 dis");
	}
}
#endif

/**
 * Handles out endpoint interrupts, except endpoint 1
 * DCD_HandleInEP_ISR
 * @param drv_info
 */
static void usb_b_ept_tx_handler(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;

	uint32_t ep_intr, diepint;
	uint32_t ept_indx = 0;

	ep_intr = otg->device_regs.DAINT & OTG_DAINT_IEPINT_Msk;
	ep_intr &= otg->device_regs.DAINTMSK;
	while (ep_intr)
	{
		if (ep_intr & 0x1) /* In ITR */
		{
			/* Get In ITR status */
			diepint = otg->device_regs.DIEPMSK;
			diepint |= ((otg->device_regs.DIEPEMPMSK >> ept_indx) & 1) << 7;
			diepint &= otg->in_ept_regs[ept_indx].DIEPINT;
			otg->in_ept_regs[ept_indx].DIEPINT = diepint;
	    	TRACE_USB(" [T%x=%02x]", ept_indx, diepint);

			if (diepint & OTG_DIEPINT_XFRC)		// Transfer completed
			{
				ep_dir_state_t* epdir;
			    HANDLE hnd;

				/* TX COMPLETE */
				epdir = &(drv_info->drv_data->endpoints[ept_indx].epd_in);
				hnd = epdir->epd_pending;
				if(hnd)
				{
					if(hnd->len == 0)
					{
				    	TRACE1_USB(" Wr!");
						epdir->epd_pending = hnd->next;
						usr_HND_SET_STATUS(hnd, RES_SIG_OK);
						hnd = epdir->epd_pending;
					}
				}
				if(hnd)
				{
					//start next transfer
			    	TRACE1_USB(" Wr");
					stm_start_tx(drv_info, hnd, ept_indx, epdir);
				} else
				{
					epdir->epd_state = ENDPOINT_STATE_IDLE;
				}

			}

			// Transmit FIFO empty
			if (diepint & OTG_DIEPINT_TXFE)
			{
				stm_write_payload(drv_info, ept_indx);
			}
		}
		ept_indx++;
		ep_intr >>= 1;
	}
}

#if USB_STM32_DEDICATED_EP1
/**
 *  USBD_OTG_EP1IN_ISR_Handler
 * @param drv_info
 */
void USB_EP1_IN_ISR(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;
	uint32_t diepint;

	diepint = otg->device_regs.DINEP1MSK;
	diepint |= (otg->device_regs.DIEPEMPMSK & 2) << 6;
	diepint &= otg->in_ept_regs[1].DIEPINT;
	otg->in_ept_regs[1].DIEPINT = diepint;
	TRACELN_USB("USB1: [in %02X]", diepint);

	if(diepint & OTG_DIEPINT_XFRC)	//Transfer completed
	{
		ep_dir_state_t* epdir;
	    HANDLE hnd;

		/* TX COMPLETE */
		epdir = &(drv_info->drv_data->endpoints[1].epd_in);
		hnd = epdir->epd_pending;
		if(hnd)
		{
			if(hnd->len == 0)
			{
		    	TRACE1_USB(" Wr!");
				epdir->epd_pending = hnd->next;
				usr_HND_SET_STATUS(hnd, RES_SIG_OK);
				hnd = epdir->epd_pending;
			}
		}
		if(hnd)
		{
			//start next transfer
	    	TRACE1_USB(" Wr");
			stm_start_tx(drv_info, hnd, 1, epdir);
		} else
		{
			epdir->epd_state = ENDPOINT_STATE_IDLE;
		}

	}


	if(diepint & OTG_DIEPINT_TXFE)		// Transmit FIFO empty
	{
		stm_write_payload(drv_info, 1);
	}
}
#endif

/**
 *  Indicates that the USB_OTG controller has detected a resume or remote Wake-up sequence
 *  (DCD_HandleResume_ISR)
 * @param drv_info
 */
static void usb_b_gint_wkupint(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;

	if(drv_info->cfg->stm32_otg & CFG_STM32_OTG_LOW_POWER)
	{
		/* un-gate USB Core clock */
		otg->PCGCCTL &= ~(OTG_PCGCCTL_GATEHCLK | OTG_PCGCCTL_STPPCLK);
	}

	/* Clear the Remote Wake-up Signaling */
	otg->device_regs.DCTL &= ~OTG_DCTL_RWUSIG;

	/* Inform upper layer by the Resume Event */
	usb_drv_event(drv_info, e_resume);
	TRACELN1_USB("usb resume");

	/* Clear interrupt */
	otg->core_regs.GINTSTS = OTG_GINTSTS_WKUPINT;

}

/**
 * 	Indicates that SUSPEND state has been detected on the USB
 * 	(DCD_HandleUSBSuspend_ISR)
 * @param drv_info
 */
static void usb_b_gint_usbsusp(USB_DRV_INFO drv_info)
{
	uint32_t prev_state;;
	USB_TypeDef* otg = drv_info->hw_base;


	/* Clear interrupt */
	otg->core_regs.GINTSTS = OTG_GINTSTS_USBSUSP | OTG_GINTSTS_ESUSP;

	prev_state = drv_info->drv_data->usb_state;
	if(prev_state != USBST_DEVICE_SUSPENDED)
	{
		if( (drv_info->cfg->stm32_otg & CFG_STM32_OTG_LOW_POWER) &&
			(otg->device_regs.DSTS & OTG_DSTS_SUSPSTS)  &&
			(prev_state == USBST_DEVICE_CONFIGURED))
		{
			/*  switch-off the clocks */
			otg->PCGCCTL |= OTG_PCGCCTL_STPPCLK;
			otg->PCGCCTL |= OTG_PCGCCTL_GATEHCLK;
		}
		usb_drv_event(drv_info, e_susppend);
	}
}

/**
 * Handles the SOF Interrupts
 * (DCD_HandleSof_ISR)
 * @param drv_info
 */
static void usb_b_gint_sof(USB_DRV_INFO drv_info)
{
	/* Inform upper layer ??? */
	TRACE1_USB(" sofint");
	// disable for now...
	drv_info->hw_base->core_regs.GINTMSK &= ~OTG_GINTMSK_SOFM;

  	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_SOF;
}


/** Handle USB bus reset
 *
 * @param drv_info
 */
static void usb_b_gint_usbrst(USB_DRV_INFO drv_info)
{
	uint32_t dev_endpoints;
	const USBDDriverDescriptors * dev_descriptors;
	USB_TypeDef* otg = drv_info->hw_base;

	TRACE1_USB(" rstint");

	/* Clear the Remote Wake-up Signaling */
	otg->device_regs.DCTL &= ~OTG_DCTL_RWUSIG;

	/* Flush the Tx FIFO */
	stm_flush_tx_fifo(otg, 0);

	// reset endpoints
	if(drv_info->cfg->stm32_otg & CFG_STM32_OTG_HS_CORE)
	{
	    dev_endpoints    = 6-1;
	} else
	{
	    dev_endpoints    = 4-1;
	}
	for(uint32_t i= 0; i<dev_endpoints; i++)
	{
		usb_hal_ept_reset(drv_info, i | 0x80);
		usb_hal_ept_reset(drv_info, i);
	}

	dev_descriptors = drv_info->drv_data->device.pDescriptors;

	/*Reset internal state machine */
	usb_drv_event(drv_info, e_reset);

	if(dev_descriptors)
	{
		usb_hal_ept_config(drv_info, &dev_descriptors->pFsDevice->as_generic);
	}

	/* Reset Device Address */
	otg->device_regs.DCFG &= ~ OTG_DCFG_DAD_Msk;


  	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_USBRST;


}


/**
 * Enumeration done interrupt
 * Read the device status register and set the device speed
 * (DCD_HandleEnumDone_ISR)
 * @param drv_info
 */
static void usb_b_gint_enumdne(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;
	uint32_t reg, speed;

	// enables EP0 OUT to receive SETUP packets and configures EP0
	speed = otg->device_regs.DSTS & OTG_DSTS_ENUMSPD_Msk;
	reg = otg->in_ept_regs[0].DIEPCTL & ~OTG_DIEPCTL_MPSIZ_Msk;
	if( speed == OTG_DSTS_ENUMSPD_LS_6)
		reg |= OTG_DIEPCTL_MPSIZ(8);
	else
		reg |= OTG_DIEPCTL_MPSIZ(64);
	otg->in_ept_regs[0].DIEPCTL = reg;

	otg->device_regs.DCTL |= OTG_DCTL_CGINAK;

	/* Set USB turn-around time based on device speed and PHY interface. */
	reg = otg->core_regs.GUSBCFG & ~OTG_GUSBCFG_TRDT_Msk;
	if(speed == OTG_DSTS_ENUMSPD_HS_30_60)
	{
		reg |= OTG_GUSBCFG_TRDT(9);
	} else
	{
		reg |= OTG_GUSBCFG_TRDT(5);
	}
	otg->core_regs.GUSBCFG = reg;

	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_ENUMDNE;

}


/**
 * Handles the Rx Status Queue Level Interrupt
 * (DCD_HandleRxStatusQueueLevel_ISR)
 * @param drv_info
 */
static void usb_b_gint_rxflvl(USB_DRV_INFO drv_info)
{
	uint32_t status;
	USB_TypeDef* otg = drv_info->hw_base;


	/* Get the Status from the top of the FIFO */
	status = otg->core_regs.GRXSTSP;


	switch(status & OTG_GRXSTSP_PKTSTS_Msk)
	{
	case OTG_GRXSTSP_PKTSTS_NAK:
		TRACE1_USB("que: nak");
		break;

	case OTG_GRXSTSP_PKTSTS_SETUP_DATA:
	case OTG_GRXSTSP_PKTSTS_OUT_DATA:
		if(!stm_read_payload(drv_info, status))
		{
			/* Disable the Rx Status Queue Level interrupt */
			otg->core_regs.GINTMSK &= ~OTG_GINTMSK_RXFLVLM;
		}
		break;

	case OTG_GRXSTSP_PKTSTS_OUT_COMP:
		TRACE1_USB(" que: in comp");
		break;

	case OTG_GRXSTSP_PKTSTS_SETUP_COMP:
		TRACE1_USB(" que: stp comp");
		break;

	default:
		TRACELN("que: unk st=%x", status);
		break;
	}

}

/**
 * Indicates that the USB_OTG controller has detected a connection
 * (DCD_SessionRequest_ISR)
 * @param drv_info
 */
static void usb_b_gint_srqint(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;

	/* Clear interrupt */
	otg->core_regs.GINTSTS = OTG_GINTSTS_SRQINT;

	// enable otg interrupts
	otg->core_regs.GINTMSK |= OTG_GINTMSK_OTGM;

	stm_otg_ungate_clock(otg, drv_info->cfg->stm32_otg);
	usb_drv_event(drv_info, e_powered);
}

/**
 * Indicates that the USB_OTG controller has detected an OTG event
 *   used to detect the end of session i.e. disconnection
 *
 * (DCD_OTG_ISR)
 * @param drv_info
 */
static void usb_b_gint_otgint(USB_DRV_INFO drv_info)
{

	uint32_t otg_int;

	otg_int = drv_info->hw_base->core_regs.GOTGINT;

	if (otg_int & OTG_GOTGINT_SEDET)
	{
		/* Inform upper layer by the Disconnect Event */
		TRACELN1_USB("ses end");
		usb_drv_event(drv_info, e_disconnect);

		// disable otg interrupts
		drv_info->hw_base->core_regs.GINTMSK &= ~OTG_GINTMSK_OTGM;

	}

	/* Clear OTG interrupt */
	drv_info->hw_base->core_regs.GOTGINT = otg_int;

	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_OTGINT;
}



void USB_OTG_ISR(USB_DRV_INFO drv_info)
{
	uint32_t status;
	USB_TypeDef* otg = drv_info->hw_base;

	status = otg->core_regs.GINTSTS;
	TRACELN_USB("USB: [%08X]", status);
	/* ensure that we are in device mode */
	if( !(status & OTG_GINTSTS_CMOD) )
	{
		status &= otg->core_regs.GINTMSK;
		if(status) /* avoid spurious interrupt */
		{
			if (status & OTG_GINTSTS_OEPINT) 	// OUT endpoints interrupt
			{
				usb_b_ept_rx_handler(drv_info);
			}

			if (status & OTG_GINTSTS_IEPINT)	// IN endpoints interrupt
			{
				usb_b_ept_tx_handler(drv_info);
			}

			if (status & OTG_GINTSTS_MMIS) 		// Mode Mismatch interrupt
			{
				TRACE1_USB(" mmis!");

				otg->core_regs.GINTSTS = OTG_GINTSTS_MMIS;
			}

			if (status & OTG_GINTSTS_WKUPINT)	// Resume/Remote wakeup detected interrupt
			{
				TRACE1_USB(" wkupint");
				usb_b_gint_wkupint(drv_info);
			}

			if (status & OTG_GINTSTS_USBSUSP)	// USB suspend
			{
				TRACE1_USB(" suspint");
				usb_b_gint_usbsusp(drv_info);
			}

			if (status & OTG_GINTSTS_SOF)		// Start of frame
			{
				usb_b_gint_sof(drv_info);
			}

			if (status & OTG_GINTSTS_RXFLVL)	// RxFIFO non-empty
			{
				usb_b_gint_rxflvl(drv_info);
			}

			if (status & OTG_GINTSTS_USBRST)	// USB reset
			{
				usb_b_gint_usbrst(drv_info);
			}

			if (status & OTG_GINTSTS_ENUMDNE)	// Enumeration done
			{
				TRACE1_USB(" enumint");
				usb_b_gint_enumdne(drv_info);
			}

			if (status & OTG_GINTSTS_IISOIXFR)	// Incomplete isochronous IN transfer
			{
				TRACE1_USB(" isoiint");

				otg->core_regs.GINTSTS = OTG_GINTSTS_IISOIXFR;
			}

			if (status & OTG_GINTSTS_IISOOXFR)	// Incomplete isochronous OUT transfer
			{
				TRACE1_USB(" isooint");

				otg->core_regs.GINTSTS = OTG_GINTSTS_IISOOXFR;
			}

			if (status & OTG_GINTSTS_SRQINT)
			{
				TRACE1_USB(" sesint");
				usb_b_gint_srqint(drv_info);
			}

			if (status & OTG_GINTSTS_OTGINT)
			{
				TRACE1_USB(" otgint");
				usb_b_gint_otgint(drv_info);
			}
		}
	}
}


/**
 * This function handles EXTI15_10_IRQ Handler.
 * @param drv_info
 */
void USB_HS_WKUP_ISR(USB_DRV_INFO drv_info)
{
	uint32_t cfg;

	cfg = drv_info->cfg->stm32_otg;
	if(cfg & CFG_STM32_OTG_LOW_POWER)
	{
		*(uint32_t *)NVIC_SYS_CTRL &= ~(NVIC_SYS_CTRL_SLEEPDEEP | NVIC_SYS_CTRL_SLEEPEXIT);
		stm_otg_ungate_clock(drv_info->hw_base, cfg);
	}
	EXTI->EXTI_PR = EXTI_PR_PR20;

	TRACELN1("usb_wkup!");
}


