/*
 * usb_hal.cpp
 *
 *  Created on: Dec 3, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <usb_hal.h>
#include <cmsis_cpp.h>

void trace_usb_state(USB_TypeDef* otg)
{
	return;
	TRACELN_USB("\r\nDIEPCTL  =%08X", otg->in_ept_regs[0].DIEPCTL);
	TRACELN_USB("DIEPTSIZ =%08X", otg->in_ept_regs[0].DIEPTSIZ);
	TRACELN_USB("DTXFSTS  =%08X", otg->in_ept_regs[0].DTXFSTS);
	TRACELN_USB("DOEPCTL  =%08X", otg->out_ept_regs[0].DOEPCTL);
	TRACELN_USB("DOEPTSIZ =%08X", otg->out_ept_regs[0].DOEPTSIZ);
	TRACELN_USB("DAINTMSK =%08X", otg->device_regs.DAINTMSK);
	TRACELN_USB("DIEPEMPMS=%08X", otg->device_regs.DIEPEMPMSK);
	TRACELN_USB("DOEPMSK  =%08X %08X", otg->device_regs.DOEPMSK, otg->out_ept_regs[0].DOEPINT & ~otg->device_regs.DOEPMSK);
	TRACELN_USB("DEACHMSK =%08X", otg->device_regs.DEACHMSK);
	TRACELN_USB("GINTMSK  =%08X %08X", otg->core_regs.GINTMSK, otg->core_regs.GINTSTS & ~otg->core_regs.GINTMSK);
	TRACELN_USB("DCFG     =%08X", otg->device_regs.DCFG);
	TRACELN_USB("DCTL     =%08X", otg->device_regs.DCTL);
	TRACELN_USB("DSTS     =%08X", otg->device_regs.DSTS);
	TRACELN_USB("DTHRCTL  =%08X", otg->device_regs.DTHRCTL);
	TRACELN_USB("GAHBCFG  =%08X", otg->core_regs.GAHBCFG);
	TRACELN_USB("GUSBCFG  =%08X", otg->core_regs.GUSBCFG);

}

bool usb_hal_get_ep_status(USB_DRV_INFO drv_info, uint8_t eptnum, uint16_t* data)
{
	uint32_t num = eptnum & 0xF;

	// Check if the endpoint exists
    if (num  > USB_NUMENDPOINTS)
    {
    	return false;
    }
    else
    {
        // Check if the endpoint if currently halted
    	if(eptnum & 0x80)
    	{
    		// IN direction
    		num = drv_info->drv_data->endpoints[num].epd_in.epd_state;
    	} else
    	{
    		num = drv_info->drv_data->endpoints[num].epd_out.epd_state;
    	}
        if (num  == ENDPOINT_STATE_HALTED)
        {
            *data = 1;
        } else
        {
        	*data = 0;
        }

    }
    return true;
}

void usb_end_transfers(ep_dir_state_t* epdir, unsigned int status)
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

void usb_hal_cancel_hnd(USB_DRV_INFO drv_info, HANDLE hnd)
{
    ep_dir_state_t* epdir;

    if(hnd->res & FLG_BUSY)
    {
    	if(hnd->cmd & FLAG_WRITE)
    	{
    		epdir = &(drv_info->drv_data->endpoints[hnd->mode1].epd_in);
    	} else
    	{
    		epdir = &(drv_info->drv_data->endpoints[hnd->mode0].epd_out);
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

void hal_uDelay (uint32_t usec)
{
	usec ++;
	usec *= 16;
	while(usec--)
		__NOP();
}

static void stm32_otg_disable_global_int(USB_CONTROLLER* otg)
{
	otg->core_regs.GAHBCFG &= ~OTG_GAHBCFG_GINTMSK;
}

/**
 * Enables the controller's Global Int in the AHB Config reg
 * @param otg
 */
static void stm32_otg_enable_global_int(USB_TypeDef* otg)
{
	otg->core_regs.GAHBCFG |= OTG_GAHBCFG_GINTMSK;
}

/** Initializes the common interrupts, used in both device and modes
 *
 * @param otg
 * @param cfg
 */
static void stm32_otg_enable_common_int(USB_TypeDef* otg, uint32_t cfg)
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
 * Enables the Device mode interrupts
 * @param otg
 * @param cfg
 */
static void stm32_otg_enable_dev_int(USB_TypeDef* otg, uint32_t cfg)
{
	uint32_t mask;

	/* Disable all interrupts. */
	otg->core_regs.GINTMSK = 0;

	/* Clear any pending interrupts */
	otg->core_regs.GINTSTS = 0xBFFFFFFF;

	/* Enable the common interrupts */
	stm32_otg_enable_common_int(otg, cfg);

	/* Enable interrupts matching to the Device mode ONLY */
	mask = OTG_GINTMSK_USBSUSPM | OTG_GINTMSK_USBRSTM | OTG_GINTMSK_ENUMDNEM |
			OTG_GINTMSK_IEPM | OTG_GINTMSK_OEPM | OTG_GINTMSK_SOFM |
			OTG_GINTMSK_IISOOXFRM | OTG_GINTMSK_IISOIXFRM | OTG_GINTMSK_RXFLVLM;

    if(cfg & CFG_STM32_OTG_VBUS_SENS)
    	mask |= OTG_GINTMSK_SRQM | OTG_GINTMSK_OTGM;

	otg->core_regs.GINTMSK |= mask;
}

/**
 * active USB Core clock
 * @param otg
 * @param cfg
 */
void stm32_otg_ungate_clock(USB_TypeDef* otg, uint32_t cfg)
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

/**
* Soft reset of the core
*/
static void stm32_otg_core_reset(USB_TypeDef* otg)
{
	uint32_t count = 0;

	/* Wait for AHB master IDLE state. */
	while (!(otg->core_regs.GRSTCTL & OTG_GRSTCTL_AHBIDL))
	{
		if (++count > 200000)
			break;
		hal_uDelay(3);
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
	hal_uDelay(3);
}

void stm32_otg_core_init(USB_TypeDef* otg, uint32_t cfg)
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
		stm32_otg_core_reset(otg);
	} else
	{
		otg->core_regs.GUSBCFG |= OTG_GUSBCFG_PHYSEL_FSSER; /* FS Interface */

		/* Reset after a PHY select and set Host mode */
		stm32_otg_core_reset(otg);

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
	  /* case the HS core is working in FS mode */
    if(cfg & CFG_STM32_OTG_DMA_EN)
    {
      otg->core_regs.GAHBCFG = OTG_GAHBCFG_DMAEN | OTG_GAHBCFG_HBSTLEN(5); /* 64 x 32-bits*/
    }
	  /* initialize OTG features */
    if( cfg & CFG_STM32_OTG_MODE)
    {
    	otg->core_regs.GUSBCFG |= OTG_GUSBCFG_HNPCAP | OTG_GUSBCFG_SRPCAP;
    	stm32_otg_enable_common_int(otg, cfg);
    }
}

/** USB_OTG_InitDevSpeed :Initializes the DevSpd field of DCFG register
 * 	depending the PHY type and the enumeration speed of the device.
 * @param otg
 * @param speed = OTG_DCFG_DSPD_HS or OTG_DCFG_DSPD_HS_FS or OTG_DCFG_DSPD_FS11
 */
static void stm32_otg_init_dev_speed(USB_TypeDef* otg, uint32_t speed)
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
void stm32_otg_flush_tx_fifo(USB_TypeDef* otg, uint32_t num )
{
	uint32_t count = 0;

	otg->core_regs.GRSTCTL = OTG_GRSTCTL_TXFFLSH | OTG_GRSTCTL_TXFNUM(num);
	while(otg->core_regs.GRSTCTL & OTG_GRSTCTL_TXFFLSH)
	{
		if (++count > 200000)
			break;
	}
	/* Wait for 3 PHY Clocks*/
	hal_uDelay(3);
}

/**
 * Flush a Rx FIFO
 * @param otg
 */
void stm32_otg_flush_rx_fifo( USB_TypeDef* otg )
{
	uint32_t count = 0;

	otg->core_regs.GRSTCTL = OTG_GRSTCTL_RXFFLSH;
	while(otg->core_regs.GRSTCTL & OTG_GRSTCTL_RXFFLSH)
	{
		if (++count > 200000)
			break;
	}

	/* Wait for 3 PHY Clocks*/
	hal_uDelay(3);
}

void stm32_otg_core_init_dev(USB_TypeDef* otg, const usb_config_t* cfg)
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
			stm32_otg_init_dev_speed(otg, OTG_DCFG_DSPD_HS);
		}
		else /* set High speed phy in Full speed mode */
		{
//			stm32_otg_init_dev_speed(otg, OTG_DCFG_DSPD_HS_FS);
			stm32_otg_init_dev_speed(otg, OTG_DCFG_DSPD_FS11);
		}

	    dev_endpoints    = 6-1;
	} else
	{
		// FS core
		/* Set Full speed phy */
		stm32_otg_init_dev_speed(otg, OTG_DCFG_DSPD_FS11);

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
	stm32_otg_flush_tx_fifo(otg, 0x10); /* all Tx FIFOs */
	stm32_otg_flush_rx_fifo(otg);
	/* Clear all pending Device Interrupts */
	otg->device_regs.DIEPMSK 	= 0;
	otg->device_regs.DOEPMSK  	= 0;
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
		otg->in_ept_regs[i].DIEPTSIZ = 0;
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
		otg->out_ept_regs[i].DOEPTSIZ = 0;
		otg->out_ept_regs[i].DOEPINT = 0xFF;
	}
	otg->device_regs.DIEPMSK |= OTG_DIEPMSK_TXFURM;

	if (cfg->stm32_otg & CFG_STM32_OTG_DMA_EN)
	{
		otg->device_regs.DTHRCTL = OTG_DTHRCTL_TXTHRLEN(64) |
				OTG_DTHRCTL_RXTHRLEN(64) |	OTG_DTHRCTL_RXTHREN |
				OTG_DTHRCTL_NONISOTHREN | OTG_DTHRCTL_ISOTHREN;
	}
	stm32_otg_enable_dev_int(otg, cfg->stm32_otg);
}

/**
 *
 * @param otg
 * @param mode - select OTG_GUSBCFG_FDMOD or OTG_GUSBCFG_FHMOD
 */
void stm32_otg_set_current_mode(USB_CONTROLLER* otg , uint32_t mode)
{
	uint32_t reg;

	reg = otg->core_regs.GUSBCFG;
	reg &= ~(OTG_GUSBCFG_FDMOD | OTG_GUSBCFG_FHMOD);
	otg->core_regs.GUSBCFG = reg | mode;

	tsk_sleep(50);
}


//static void usb_write_payload(USB_DRV_INFO drv_info, HANDLE hnd)
//{
//	__IO uint32_t *fifo;
//	uint32_t len;
//
//	if ( !(drv_info->cfg->stm32_otg & CFG_STM32_OTG_DMA_EN))
//	{
//
//		len = (hnd->len + 3) / 4;
//		hnd->len = 0;
//		fifo = drv_info->hw_base->DFIFO[hnd->mode1].DFIFO;
//		while (len--)
//		{
//			fifo[0] = *hnd->src.as_intptr++;
//		}
//	}
//}


unsigned int usb_read_payload(volatile void* src, HANDLE hnd, unsigned int size)
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

    return (size);
}

/** Reset on DCR_RESET
 *
 * @param drv_info
 */
void usb_hal_reset(USB_DRV_INFO drv_info)
{
    TRACELN1_USB("USBD_Init");
	NVIC->NVIC_SetPriority(drv_info->info.drv_index,
			drv_info->info.isr_priority);
}

void usb_hal_stall(USB_TypeDef* hw_base, unsigned int eptnum)
{
	uint32_t ep_num = eptnum & 0xF;

	if (eptnum & 0x80)
	{
		uint32_t value;
		__IO uint32_t* reg;

		reg = &hw_base->in_ept_regs[ep_num].DIEPCTL;

		value = *reg;
		if(value & OTG_DIEPCTL_EPENA)
			value |= OTG_DIEPCTL_EPDIS;
		value |= OTG_DIEPCTL_STALL;

		*reg = value;
	} else
	{
		hw_base->out_ept_regs[ep_num].DOEPCTL |= OTG_DOEPCTL_STALL;
	}

}

void usb_hal_stall_clear(USB_TypeDef* hw_base, unsigned int eptnum)
{
	uint32_t value;
	__IO uint32_t* reg;
	uint32_t ep_num = eptnum & 0xF;

	if (eptnum & 0x80)
	{
		reg = &hw_base->in_ept_regs[ep_num].DIEPCTL;
	} else
	{
		reg = &hw_base->out_ept_regs[ep_num].DOEPCTL;

	}
	value = *reg;
	value &= ~OTG_DIEPCTL_STALL;

	if(eptnum >0) // this should be for interrupt/bulk
		value |= OTG_DIEPCTL_SD0PID; // set endpoint data PID to DATA0

	*reg = value;
}

/** PKTRDY must be set to enable transmission of the packet that was loaded
 */
void usb_hal_txpktrdy(USB_TypeDef* hw_base, unsigned int eptnum, int len)
{
/*
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
*/

}

/** Configure as device
 * called from the usbdrv thread
 * @param drv_info
 */
void usb_hal_configure(USB_DRV_INFO drv_info)
{
	USB_CONTROLLER* otg = drv_info->hw_base;

    // Enable Clocking to the USB controller.
	RCCPeripheralEnable(drv_info->info.peripheral_indx);
	RCCPeripheralReset(drv_info->info.peripheral_indx);


	// Enable USB related pins
	PIO_Cfg_List(drv_info->cfg->usb_pins);

	// disable global interrupts
	stm32_otg_disable_global_int(otg);

	// init core
	stm32_otg_core_init(otg, drv_info->cfg->stm32_otg);

	// force device mode
	stm32_otg_set_current_mode(otg, OTG_GUSBCFG_FDMOD);

	// init device
	stm32_otg_core_init_dev(otg, drv_info->cfg);

	// enable global ints
	stm32_otg_enable_global_int(otg);

	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);
}



void usb_ept_reset(USB_DRV_INFO drv_info, unsigned int eptnum)
{
	USB_TypeDef* otg = drv_info->hw_base;
	uint32_t ep_num = eptnum & 0xF;
	ep_dir_state_t* epdir;

	// - clear status and flush
	if(eptnum & 0x80)
	{
        epdir = &drv_info->drv_data->endpoints[ep_num].epd_in;
		otg->in_ept_regs[ep_num].DIEPINT = 0xFF;
		stm32_otg_flush_tx_fifo(otg, ep_num);

		otg->device_regs.DAINT = OTG_DAINT_IEPINT(ep_num);
		otg->device_regs.DAINTMSK &= ~OTG_DAINTMSK_IEPM(ep_num);
	} else
	{
        epdir = &drv_info->drv_data->endpoints[ep_num].epd_out;
		otg->out_ept_regs[ep_num].DOEPINT = 0xFF;

		otg->device_regs.DAINT = OTG_DAINT_OEPINT(ep_num);
		otg->device_regs.DAINTMSK &= ~OTG_DAINTMSK_OEPM(ep_num);
	}

	// - disable ints...

	// - cancel transfers...
	usb_end_transfers(epdir, USBD_STATUS_RESET);
    epdir->epd_state = ENDPOINT_STATE_DISABLED;


}

void usb_ept_config(USB_DRV_INFO drv_info, uint32_t eptnum, const USBGenericDescriptor* pDescriptor)
{
	ep_dir_state_t* epdir;
	uint32_t ep_num = eptnum & 0xF;
	USB_TypeDef* otg = drv_info->hw_base;

    if(ep_num >= USB_NUMENDPOINTS)
    {
		TRACELN1_USB("Invalid endpoint descriptor!");

    } else
    {
        TRACE_USB(" CfgEp(%x)", eptnum);

        /* Abort the current transfer is the endpoint was configured and in
    	 Write or Read state */
    	usb_ept_reset(drv_info, eptnum);

        if(eptnum & 0x80)
        {
        	eptnum &= 0xF;
            epdir = &drv_info->drv_data->endpoints[ep_num].epd_in;
        	otg->device_regs.DAINTMSK |= OTG_DAINTMSK_IEPM(ep_num);
        } else
        {
            epdir = &drv_info->drv_data->endpoints[ep_num].epd_out;
        	otg->device_regs.DAINTMSK |= OTG_DAINTMSK_OEPM(ep_num);
        	if(ep_num == EPT_0)
        	{
        		otg->out_ept_regs[0].DOEPTSIZ = OTG_DOEPTSIZ_STUPCNT(3) |
        			OTG_DOEPTSIZ_PKTCNT(1) | OTG_DOEPTSIZ_XFRSIZ(8 * 3);
        	}
        }

        // set fifo size
    	switch(pDescriptor->bDescriptorType)
    	{
    	case DEVICE_DESCRIPTOR:
        	epdir->epd_fifo_sz = ((USBDeviceDescriptor *)pDescriptor)->bMaxPacketSize0;
        	break;

    	case ENDPOINT_DESCRIPTOR:
    		epdir->epd_fifo_sz = ((const USBEndpointDescriptor*)pDescriptor)->wMaxPacketSize;
    		break;

    	default:
    		epdir->epd_fifo_sz = 0;
        }

    	epdir->epd_state = ENDPOINT_STATE_IDLE;
//        // Set the maximum packet size.
//        hw_base->DEVICE_EP[eptnum].USBTXMAXP = pEndpoint->txfifo_size;
//
//
//        // Allow auto setting of TxPktRdy when max packet size has been loaded
//        // into the FIFO.
//
//        // Enable isochronous mode if requested.
//        if(ept_type == ENDPOINT_TYPE_ISOCHRONOUS)
//        {
//            reg |= USB_USBTXCSRH_ISO;
//        }
//
//        // Write the transmit control value.
//        hw_base->DEVICE_EP[eptnum].USBTXCSRH = reg;
//
//        // Reset the Data toggle to zero.
//        hw_base->DEVICE_EP[eptnum].USBTXCSRL = USB_USBTXCSRL_CLRDT;
//
//		hw_base->USBTXIE |= 1 << eptnum;

    }
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
		usb_ept_config(drv_info, 0, pDescriptor);
		usb_ept_config(drv_info, 0x80, pDescriptor);
        break;

	case ENDPOINT_DESCRIPTOR:
		descriptor = (const USBEndpointDescriptor*)pDescriptor;

		usb_ept_config(drv_info, descriptor->bEndpointAddress, pDescriptor);

		break;

	default:
		TRACELN1_USB("Invalid descriptor!");
		return;
	}

}


void usb_hal_config_fifo(USB_DRV_INFO drv_info)
{
/*
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
*/

}

void usb_ack_packet(USB_TypeDef* hw_base, Endpoint* endpoint, unsigned int eptnum)
{
/*
    if(eptnum == 0)
    {
        // Clear RxPktRdy, and optionally DataEnd, on endpoint zero.
    	hw_base->DEVICE_EP[0].USBTXCSRL = USB_USBTXCSRL0_RXRDYC
				| (endpoint->rxfifo_cnt ? 0 : USB_USBTXCSRL0_DATAEND);

    	//enable interrupt
    	hw_base->USBTXIE |= USB_USBTXIE_EP0;
    }
    else
    {
        // Clear RxPktRdy on all other endpoints.
        hw_base->DEVICE_EP[eptnum].USBRXCSRL &= ~(USB_USBRXCSRL_RXRDY);
    }
*/
}


void usb_b_ept0_handler(USB_DRV_INFO drv_info)
{
/*
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
//	    		hw_base->USBTXIE &= ~USB_USBTXIE_EP0;
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
*/
}

void usb_b_ept_rx_handler(USB_DRV_INFO drv_info, unsigned int eptnum)
{
/*
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
*/
}

void usb_b_ept_tx_handler(USB_DRV_INFO drv_info, unsigned int eptnum)
{
/*
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
*/
}


/**
* @brief  DCD_HandleOutEP_ISR
*         Indicates that an OUT EP has a pending Interrupt
* @param  pdev: device instance
* @retval status
*/
static void DCD_HandleOutEP_ISR(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;
	uint32_t status, doepint;//, deptsiz;
	uint32_t epnum = 0;


	/* Read in the device interrupt bits */
	status = otg->device_regs.DAINT;
	status &= otg->device_regs.DAINTMSK;
	status >>= 16;

	while (status)
	{
		if (status & 0x1)
		{
			doepint = otg->out_ept_regs[epnum].DOEPINT;
			doepint &= otg->device_regs.DOEPMSK;
			/* Clear the bit in DOEPINTn for this interrupt */
			otg->out_ept_regs[epnum].DOEPINT = doepint;

	    	TRACELN_USB("USB:[R%x=", epnum);
		    TRACE_USB("%02x]", doepint);

			/* Transfer complete */
			if (doepint & OTG_DOEPINT_XFRC)
			{

				/* RX COMPLETE */
				if(drv_info->drv_data->endpoints[epnum].epd_out.epd_pending)
				{
			    	TRACE1_USB(" rcv comp");
				} else
				{
			    	TRACE1_USB(" rcv comp!");

				}

//				if (pdev->cfg.dma_enable == 1)
//				{
//					deptsiz = otg->out_ept_regs[epnum].DOEPTSIZ;
//					deptsiz.d32 = USB_OTG_READ_REG32(
//							&(pdev->regs.OUTEP_REGS[epnum]->DOEPTSIZ));
//					/*ToDo : handle more than one single MPS size packet */
//					pdev->dev.out_ep[epnum].xfer_count =
//							pdev->dev.out_ep[epnum].maxpacket
//									- deptsiz.b.xfersize;
//				}
				/* Inform upper layer: data ready */
				/* RX COMPLETE */
//				USBD_DCD_INT_fops->DataOutStage(pdev, epnum);
//
//				if (pdev->cfg.dma_enable == 1)
//				{
//					if ((epnum == 0)
//							&& (pdev->dev.device_state == USB_OTG_EP0_STATUS_OUT))
//					{
//						/* prepare to rx more setup packets */
//						USB_OTG_EP0_OutStart(pdev);
//					}
//				}
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
//				USBD_DCD_INT_fops->SetupStage(pdev);
		    	TRACE1_USB(" stp done");
			}
		}
		epnum++;
		status >>= 1;
	}
}

/**
* @brief  DCD_WriteEmptyTxFifo
*         check FIFO for the next packet to be loaded
* @param  pdev: device instance
* @retval status
*/
static void DCD_WriteEmptyTxFifo(USB_DRV_INFO drv_info, uint32_t ep_num)
{
    ep_dir_state_t* epdir;
    HANDLE hnd;
	uint32_t len, len32b, txsize;
    __IO uint32_t *fifo;

	if (! (drv_info->cfg->stm32_otg & CFG_STM32_OTG_DMA_EN) )
	{
		epdir = &(drv_info->drv_data->endpoints[ep_num].epd_in);
		fifo = drv_info->hw_base->DFIFO[ep_num].DFIFO;

	    hnd = epdir->epd_pending;
	    if(hnd)
	    {
			txsize = drv_info->hw_base->in_ept_regs[ep_num].DTXFSTS & DTXFSTS_INEPTFSAV_Msk;

		    TRACE_USB(" Wr%d(%d) ", ep_num , hnd->len);
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
    				drv_info->hw_base->device_regs.DIEPEMPMSK |= OTG_DIEPEMPMSK_INEPTXFEM(ep_num);
	    		}

	    	} else
	    	{
	    		drv_info->hw_base->device_regs.DIEPEMPMSK &= ~OTG_DIEPEMPMSK_INEPTXFEM(ep_num);
	    	}
	    } else
	    {
	    	TRACELN1_USB("usb tx emp !h");
	    }
	}
}

static void usb_tx_pending(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t eptnum, ep_dir_state_t* epdir)
{
	uint32_t reg;
	USB_TypeDef* otg = drv_info->hw_base;

	if(hnd->len == 0)
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

		if(epdir->epd_type == ENDPOINT_TYPE_ISOCHRONOUS)
		{
			reg |= OTG_DIEPTSIZ_MCNT(1);
		}

	}
	otg->in_ept_regs[eptnum].DIEPTSIZ = reg;

	if(drv_info->cfg->stm32_otg & CFG_STM32_OTG_DMA_EN)
	{
		otg->in_ept_regs[eptnum].DIEPDMA = hnd->src.as_int;
	}

	reg = otg->in_ept_regs[eptnum].DIEPCTL;
	if(epdir->epd_type == ENDPOINT_TYPE_ISOCHRONOUS)
	{
		if(otg->device_regs.DSTS & OTG_DSTS_FNSOF_ODD)
		{
			reg |= OTG_DIEPCTL_SD0PID;
		} else
		{
			reg |= OTG_DIEPCTL_SD1PID;
		}
	}
	otg->in_ept_regs[eptnum].DIEPCTL = reg | OTG_DIEPCTL_CNAK | OTG_DIEPCTL_EPENA;

//	if(endpoint->usb_type == ENDPOINT_TYPE_ISOCHRONOUS)
//	{
//		if(endpoint->pending)
//			usb_write_payload(drv_info, endpoint->pending);
//	}
	DCD_WriteEmptyTxFifo(drv_info, eptnum);
}

void usb_start_tx(USB_DRV_INFO drv_info, HANDLE hnd)
{
	ep_dir_state_t* epdir;
	uint32_t eptnum;
	HANDLE prev;

	eptnum = hnd->mode1;
    TRACE_USB(" Write%d(%d) ", eptnum , hnd->len);
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

		    usb_tx_pending(drv_info, hnd, eptnum, epdir);

		    // Enable interrupt on endpoint
		    ENTPOINT_ENABLE_INT(drv_info->hw_base, eptnum);
	    }
    }
}

static void usb_start_rx_ept(USB_DRV_INFO drv_info, uint32_t eptnum, Endpoint *endpoint)
{
	HANDLE hnd;

	hnd = endpoint->epd_out.epd_pending;
	if(hnd && drv_info->drv_data->usb_state >= USB_STATE_ADDRESS /*USB_STATE_DEFAULT*/)
	{
    	OTG_OUT_EPT_REGS* otg_regs;
    	uint32_t depctl, deptsiz, maxpacket;

    	//start xfer
    	otg_regs = &drv_info->hw_base->out_ept_regs[eptnum];
    	depctl = otg_regs->DOEPCTL;
    	if( !( depctl & OTG_DOEPCTL_EPENA) )
    	{
    		deptsiz = otg_regs->DOEPTSIZ;
    		TRACELN_USB("usb sr%u sz=%08x", eptnum, deptsiz);
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
//    		TRACE_USB(" ns=%08x", deptsiz);
    		trace_usb_state(drv_info->hw_base);

    	} else
    	{
    		TRACELN_USB("usb rr%u sz=%08x", eptnum, otg_regs->DOEPTSIZ);
    	}
	}
}

void usb_start_rx(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t eptnum, Endpoint *endpoint)
{
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
		    	usb_start_rx_ept(drv_info, eptnum, endpoint);
		    }
		    return;
		}
	}
	svc_HND_SET_STATUS(hnd, RES_SIG_OK);

}

void usb_process_rx(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t eptnum, Endpoint *endpoint)
{

}

/**
 * Reads a packet from the Rx FIFO
 * @param drv_info
 * @param endpoint
 * @param len
 */
bool USB_OTG_ReadPacket(USB_DRV_INFO drv_info,	uint32_t status)
{
    Endpoint* endpoint;
    uint32_t bcnt, top_rx_cnt;
    HANDLE hnd;
    __IO uint32_t* fifo = drv_info->hw_base->DFIFO[0].DFIFO;

	endpoint = &drv_info->drv_data->endpoints[status & OTG_GRXSTSP_EPNUM_Msk];
	bcnt = OTG_GRXSTSP_BCNT_Get(status);
	top_rx_cnt = 0;
	TRACE_USB(" que:[R%x, %ub]", status & OTG_GRXSTSP_EPNUM_Msk, bcnt);

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
 * Indicates that an IN EP has a pending Interrupt
 * @param drv_info
 */
static void DCD_HandleInEP_ISR(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;

	uint32_t ep_intr, diepint;
	uint32_t epnum = 0;

	ep_intr = otg->device_regs.DAINT & OTG_DAINT_IEPINT_Msk;
	ep_intr &= otg->device_regs.DAINTMSK;
	ep_intr &= 0xFFFF;
	while (ep_intr)
	{
		if (ep_intr & 0x1) /* In ITR */
		{
	    	TRACELN_USB("USB:[T%x=", epnum);
			/* Get In ITR status */
			diepint = otg->device_regs.DIEPMSK;
			diepint |= ((otg->device_regs.DIEPEMPMSK >> epnum) & 1) << 7;
			diepint &= otg->in_ept_regs[epnum].DIEPINT;
		    TRACE_USB("%02x]", diepint);
			if (diepint & OTG_DIEPINT_XFRC)		// Transfer completed
			{
				ep_dir_state_t* epdir;
			    HANDLE hnd;

				otg->in_ept_regs[epnum].DIEPINT = OTG_DIEPINT_XFRC;

				/* TX COMPLETE */
				epdir = &(drv_info->drv_data->endpoints[epnum].epd_in);
				hnd = epdir->epd_pending;
				if(hnd)
				{
					if(hnd->len == 0)
					{
				    	TRACE1_USB(" Wr!");
//				    	trace_usb_state(otg);
						epdir->epd_pending = hnd->next;
						usr_HND_SET_STATUS(hnd, RES_SIG_OK);
						hnd = epdir->epd_pending;
					}
				}
				if(hnd)
				{
					//start next transfer
			    	TRACE1_USB(" Wr");
					usb_tx_pending(drv_info, hnd, epnum, epdir);
				} else
				{
					epdir->epd_state = ENDPOINT_STATE_IDLE;
				}

//				if (pdev->cfg.dma_enable == 1)
//				{
//					if ((epnum == 0)
//							&& (pdev->dev.device_state == USB_OTG_EP0_STATUS_IN))
//					{
//						/* prepare to rx more setup packets */
//						USB_OTG_EP0_OutStart (pdev);
//					}
//				}
			}

			if (diepint & OTG_DIEPINT_TOC) 		// Timeout condition
			{
				otg->in_ept_regs[epnum].DIEPINT = OTG_DIEPINT_TOC;
			}

			if (diepint & OTG_DIEPINT_ITTXFE)	// IN Token received when TxFIFO is empty
			{
				otg->in_ept_regs[epnum].DIEPINT = OTG_DIEPINT_ITTXFE;
			}

			if (diepint & OTG_DIEPINT_INEPNE)	// IN endpoint NAK effective
			{
				otg->in_ept_regs[epnum].DIEPINT = OTG_DIEPINT_INEPNE;
			}

			if (diepint & OTG_DIEPINT_EPDISD)	// Endpoint disabled interrupt
			{
				otg->in_ept_regs[epnum].DIEPINT = OTG_DIEPINT_EPDISD;
			}

			if (diepint & OTG_DIEPINT_TXFE)		// Transmit FIFO empty
			{

				DCD_WriteEmptyTxFifo(drv_info, epnum);

				otg->in_ept_regs[epnum].DIEPINT = OTG_DIEPINT_TXFE;
			}
		}
		epnum++;
		ep_intr >>= 1;
	}
}

/**
 *   Indicates that the USB_OTG controller has detected a resume or remote Wake-up sequence
 * @param drv_info
 */
static void DCD_HandleResume_ISR(USB_DRV_INFO drv_info)
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
	drv_info->drv_data->usb_event(e_resume);
	TRACELN1_USB("usb resume");

	/* Clear interrupt */
	otg->core_regs.GINTSTS = OTG_GINTSTS_WKUPINT;

}

/**
 * 	Indicates that SUSPEND state has been detected on the USB
 * @param drv_info
 */
static void DCD_HandleUSBSuspend_ISR(USB_DRV_INFO drv_info)
{
	USB_DRIVER_STATE prev_state;
	USB_TypeDef* otg = drv_info->hw_base;

	/* Inform upper layer by the Suspend Event */
	prev_state = drv_info->drv_data->usb_state;
	drv_info->drv_data->usb_event(e_susppend);

	/* Clear interrupt */
	otg->core_regs.GINTSTS = OTG_GINTSTS_USBSUSP | OTG_GINTSTS_ESUSP;

	if( (drv_info->cfg->stm32_otg & CFG_STM32_OTG_LOW_POWER) &&
			(otg->device_regs.DSTS & OTG_DSTS_SUSPSTS) &&
			(prev_state == USB_STATE_CONFIGURED))
	{
		/*  switch-off the clocks */
		otg->PCGCCTL |= OTG_PCGCCTL_STPPCLK;
		otg->PCGCCTL |= OTG_PCGCCTL_GATEHCLK;
	}
}

/**
 * Handles the SOF Interrupts
 * @param drv_info
 */
static void DCD_HandleSof_ISR(USB_DRV_INFO drv_info)
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
void usb_handle_bus_reset(USB_DRV_INFO drv_info)
{
	uint32_t dev_endpoints;
	const USBDDriverDescriptors * dev_descriptors;
	USB_TypeDef* otg = drv_info->hw_base;

	/* Clear the Remote Wake-up Signaling */
	otg->device_regs.DCTL &= ~OTG_DCTL_RWUSIG;

	/* Flush the Tx FIFO */
	stm32_otg_flush_tx_fifo(otg, 0);

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
		usb_ept_reset(drv_info, i | 0x80);
		usb_ept_reset(drv_info, i);
	}

	dev_descriptors = drv_info->drv_data->device.pDescriptors;

	/*Reset internal state machine */
	drv_info->drv_data->usb_event(e_reset);

	if(dev_descriptors)
	{
		usb_hal_ept_config(drv_info, &dev_descriptors->pFsDevice->as_generic);
	}




	otg->device_regs.DOEPMSK = OTG_DOEPMSK_STUPM | OTG_DOEPMSK_XFRCM | OTG_DOEPMSK_EPDM;
	otg->device_regs.DIEPMSK = OTG_DIEPMSK_XFRCM | OTG_DIEPMSK_TOCM | OTG_DIEPMSK_EPDM;

	if(drv_info->cfg->stm32_otg & CFG_STM32_OTG_DEDICATED_EP1)
	{
		otg->device_regs.DOUTEP1MSK = OTG_DOEPMSK_STUPM | OTG_DOEPMSK_XFRCM | OTG_DOEPMSK_EPDM;
		otg->device_regs.DINEP1MSK = OTG_DIEPMSK_XFRCM | OTG_DIEPMSK_TOCM | OTG_DIEPMSK_EPDM;
	}

	/* Reset Device Address */
	otg->device_regs.DCFG &= ~ OTG_DCFG_DAD_Msk;


	/* setup EP0 to receive SETUP packets */
//@@	USB_OTG_EP0_OutStart (pdev);

  	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_USBRST;


}


/**
 * Read the device status register and set the device speed
 * @param drv_info
 */
static void DCD_HandleEnumDone_ISR(USB_DRV_INFO drv_info)
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
 * handle the ISO IN incomplete interrupt
 * @param drv_info
 */
static void DCD_IsoINIncomplete_ISR(USB_DRV_INFO drv_info)
{
	/* Inform upper layer ??? */
	TRACELN1_USB("usb iiinc");

	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_IISOIXFR;
}

/**
 * handle the ISO OUT incomplete interrupt
 * @param drv_info
 */
static void DCD_IsoOUTIncomplete_ISR(USB_DRV_INFO drv_info)
{
	/* Inform upper layer ??? */
	TRACELN1_USB("usb ioinc");

	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_IISOOXFR;
}

/**
* @brief  DCD_HandleRxStatusQueueLevel_ISR
*         Handles the Rx Status Queue Level Interrupt
* @param  pdev: device instance
* @retval status
*/
static void DCD_HandleRxStatusQueueLevel_ISR(USB_DRV_INFO drv_info)
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
		if(!USB_OTG_ReadPacket(drv_info, status))
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
 * @param drv_info
 */
static void DCD_SessionRequest_ISR(USB_DRV_INFO drv_info)
{
	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_SRQINT;

	drv_info->drv_data->usb_event(e_powered);

	// enable otg interrupts
	drv_info->hw_base->core_regs.GINTMSK |= OTG_GINTMSK_OTGM;
}

/**
* @brief  DCD_OTG_ISR
*         Indicates that the USB_OTG controller has detected an OTG event:
*                 used to detect the end of session i.e. disconnection
* @param  pdev: device instance
* @retval status
*/
static void DCD_OTG_ISR(USB_DRV_INFO drv_info)
{

	uint32_t otg_int;

	otg_int = drv_info->hw_base->core_regs.GOTGINT;

	if (otg_int & OTG_GOTGINT_SEDET)
	{
		/* Inform upper layer by the Disconnect Event */
		TRACELN1_USB("ses end");
		drv_info->drv_data->usb_event(e_disconnect);

		// disable otg interrupts
		drv_info->hw_base->core_regs.GINTMSK &= ~OTG_GINTMSK_OTGM;

	}

	/* Clear OTG interrupt */
	drv_info->hw_base->core_regs.GOTGINT = otg_int;

	/* Clear interrupt */
	drv_info->hw_base->core_regs.GINTSTS = OTG_GINTSTS_OTGINT;
}



void USB_B_ISR(USB_DRV_INFO drv_info)
{
	TRACELN("usbisr");
/*
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
*/


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
//		SystemInit();
		stm32_otg_ungate_clock(drv_info->hw_base, cfg);
	}
	EXTI->EXTI_PR = EXTI_PR_PR20;

	TRACELN1("usb_wkup!");
}

void USB_OTG_ISR(USB_DRV_INFO drv_info)
{
	uint32_t status;
	USB_TypeDef* otg = drv_info->hw_base;

	status = otg->core_regs.GINTSTS;
	TRACELN_USB("USB: %05u %u[otg=%08X]", CURRENT_TIME, drv_info->drv_data->usb_state, status);
	/* ensure that we are in device mode */
	if( !(status & OTG_GINTSTS_CMOD) )
	{
		status &= otg->core_regs.GINTMSK;
		if(status) /* avoid spurious interrupt */
		{
			if (status & OTG_GINTSTS_OEPINT) 	// OUT endpoints interrupt
			{
//				TRACELN1_USB("usb oepint");
				DCD_HandleOutEP_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_IEPINT)	// IN endpoints interrupt
			{
//				TRACELN1_USB("usb iepint");
				DCD_HandleInEP_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_MMIS) 		// Mode Mismatch interrupt
			{
				TRACE1_USB(" mmis!");

				otg->core_regs.GINTSTS = OTG_GINTSTS_MMIS;
			}

			if (status & OTG_GINTSTS_WKUPINT)	// Resume/Remote wakeup detected interrupt
			{
				TRACE1_USB(" wkupint");
				DCD_HandleResume_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_USBSUSP)	// USB suspend
			{
				TRACE1_USB(" suspint");
				DCD_HandleUSBSuspend_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_SOF)		// Start of frame
			{
//				TRACELN1_USB("usb sofint");
				DCD_HandleSof_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_RXFLVL)	// RxFIFO non-empty
			{
//				TRACELN1_USB("usb rxflvl");
				DCD_HandleRxStatusQueueLevel_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_USBRST)	// USB reset
			{
				TRACE1_USB(" rstint");
				usb_handle_bus_reset(drv_info);
			}

			if (status & OTG_GINTSTS_ENUMDNE)	// Enumeration done
			{
				TRACE1_USB(" enumint");
				DCD_HandleEnumDone_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_IISOIXFR)	// Incomplete isochronous IN transfer
			{
				TRACE1_USB(" isoiint");
				DCD_IsoINIncomplete_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_IISOOXFR)	// Incomplete isochronous OUT transfer
			{
				TRACE1_USB(" isooint");
				DCD_IsoOUTIncomplete_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_SRQINT)
			{
				TRACE1_USB(" sesint");
				DCD_SessionRequest_ISR(drv_info);
			}

			if (status & OTG_GINTSTS_OTGINT)
			{
				TRACE1_USB(" otgint");
				DCD_OTG_ISR(drv_info);
			}
		}
	}
}

/**
 *  USBD_OTG_EP1IN_ISR_Handler
 * @param drv_info
 */
void USB_EP1_IN_ISR(USB_DRV_INFO drv_info)
{
	USB_TypeDef* otg = drv_info->hw_base;
	uint32_t status;

	status = otg->device_regs.DINEP1MSK;
	status |= (otg->device_regs.DIEPEMPMSK & 2) << 6;
	status &= otg->in_ept_regs[1].DIEPINT;

	if(status & OTG_DIEPINT_XFRC)	//Transfer completed
	{
		// disable tx empty interrupt
		otg->device_regs.DIEPEMPMSK &= ~2;
		// clear transfer completed interrupt
		otg->in_ept_regs[1].DIEPINT = OTG_DIEPINT_XFRC;
		// signal...
		TRACELN1_USB("usb xfrc 1");
	}

	if(status & OTG_DIEPINT_EPDISD)	// Endpoint disabled
	{
		otg->in_ept_regs[1].DIEPINT = OTG_DIEPINT_EPDISD;
	}

	if(status & OTG_DIEPINT_TOC) 	// Timeout condition
	{
		otg->in_ept_regs[1].DIEPINT = OTG_DIEPINT_TOC;
	}

	if(status & OTG_DIEPINT_ITTXFE)	// IN Token received when TxFIFO is empty
	{
		otg->in_ept_regs[1].DIEPINT = OTG_DIEPINT_ITTXFE;
	}

	if(status & OTG_DIEPINT_INEPNE)	// IN endpoint NAK effective
	{
		otg->in_ept_regs[1].DIEPINT = OTG_DIEPINT_INEPNE;
	}

	if(status & OTG_DIEPINT_TXFE)		// Transmit FIFO empty
	{

		DCD_WriteEmptyTxFifo(drv_info, 1);

		otg->in_ept_regs[1].DIEPINT = OTG_DIEPINT_TXFE;
	}
}


/** handles EP1_OUT Handler
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
