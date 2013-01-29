/*
 * spi_drv.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <cmsis_cpp.h>
#include <spi_drv.h>


// Enable SPI
static void SPI_ENABLE(SPI_DRIVER_INFO* drv_info)
{
	SPI_TypeDef*  pSPI;

	RCCPeripheralEnable(drv_info->info.peripheral_indx);

	PIO_Cfg_List(drv_info->spi_pins);

	pSPI = drv_info->hw_base;
    pSPI->SPI_CR2 = 0; //Reset value (interrupts disabled)
	NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);
	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);

}

// Start a new transaction
static void SPI_START_TRANSACTION(SPI_DRIVER_INFO* drv_info, SPI_DRIVER_MODE* mode)
{
	SPI_TypeDef* pSPI = drv_info->hw_base;

	//	1. Select the BR[2:0] bits to define the serial clock baud rate (see
	//		SPI_CR1 register).
	//	2. Select the CPOL and CPHA bits to define one of the four relationships
	//		between the	data transfer and the serial clock (see Figure 249).
	//		This step is not required when the TI mode is selected.
	//	3. Set the DFF bit to define 8- or 16-bit data frame format
	//	4. Configure the LSBFIRST bit in the SPI_CR1 register to define the frame
	//		format. This step is not required when the TI mode is selected.
	//	5. If the NSS pin is required in input mode, in hardware mode, connect
	//		the NSS pin to a high-level signal during the complete byte transmit
	//		sequence. In NSS software mode, set the SSM and SSI bits in the
	//		SPI_CR1 register. If the NSS pin is required in output	mode, the
	//		SSOE bit only should be set. This step is not required when the TI
	//		mode is	selected.
	pSPI->SPI_CR1 = mode->spi_cr1;

	//	6. Set the FRF bit in SPI_CR2 to select the TI protocol for serial communications.
	//	7. The MSTR and SPE bits must be set (they remain set only if the NSS pin is connected
	//	  to a high-level signal).

#if USE_SPI_DMA_DRIVER
	if(drv_info->rx_dma_mode.dma_index < INALID_DRV_INDX)
	{
		pSPI->SPI_CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN ;
	}
	else
#endif
	{
		pSPI->SPI_CR2 = (pSPI->SPI_CR2 & (SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN)) | mode->spi_cr2 | SPI_CR2_RXNEIE;
	}
    //enable
    pSPI->SPI_CR1 = mode->spi_cr1 | SPI_CR1_SPE;

    // Assert CS
    PIO_Assert(mode->cs_pin);
}

static void SPI_END_TRANSACTION(SPI_DRIVER_MODE* mode)
{
	PIO_Deassert(mode->cs_pin);
}

static void START_HND(SPI_DRIVER_INFO* drv_info, SPI_DRIVER_DATA* drv_data, HANDLE hnd)
{
#if USE_SPI_DMA_DRIVER
	if(drv_info->rx_dma_mode.dma_index < INALID_DRV_INDX)
	{
		void *ptr;

		if(hnd->cmd & FLAG_READ)
		{
			ptr = hnd->dst.as_voidptr;
		} else
		{
			ptr = NULL;
		}
		drv_data->rx_dma_hnd.drv_read_write(ptr, (void *)&drv_info->hw_base->SPI_DR, hnd->len);

		if(hnd->cmd & FLAG_WRITE)
		{
			ptr = hnd->src.as_voidptr;
		} else
		{
			ptr = (void*) 0xff; //fill with ff...
		}
		drv_data->tx_dma_hnd.drv_read_write((void *)&drv_info->hw_base->SPI_DR, ptr, hnd->len);

	} else
#endif
	{
		//no DMA
		if(hnd->cmd & FLAG_WRITE)
		{
			drv_info->hw_base->SPI_DR = *hnd->src.as_byteptr++;
		} else
		{
			drv_info->hw_base->SPI_DR = 0xFF;
		}
	}
}

static void SPI_RESUME(SPI_DRIVER_INFO* drv_info, SPI_DRIVER_DATA* drv_data)
{
	HANDLE hnd;

	hnd = drv_data->pending;
	if( !hnd )
	{
		// no pending try with waiting...
		hnd = drv_data->waiting;

		if(drv_data->locker)
		{
			//search for the same client...
			while(hnd)
			{
				if(hnd->client.task == drv_data->locker)
					break;
				hnd = hnd->next;
			}
		}

		if( hnd )
		{
			// make it pending
			hnd->list_remove(drv_data->waiting);
			drv_data->pending = hnd;
			hnd->next = NULL;
		}
	}
	if(hnd)
	{
		if(!drv_data->locker)
		{
			SPI_START_TRANSACTION(drv_info, (SPI_DRIVER_MODE *)hnd->mode.as_voidptr);
			if( hnd->cmd & FLAG_LOCK)
				drv_data->locker = hnd->client.task;
		}
		START_HND(drv_info, drv_data, hnd);
	}
}
//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void SPI_DCR(SPI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
    SPI_DRIVER_DATA* drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
			RCCPeripheralReset(drv_info->info.peripheral_indx);
        	RCCPeripheralDisable(drv_info->info.peripheral_indx);
#if USE_SPI_DMA_DRIVER
        	drv_data->rx_dma_hnd.client.drv_index = drv_info->info.drv_index;
        	drv_data->tx_dma_hnd.client.drv_index = drv_info->info.drv_index;
#endif
        	break;

        case DCR_OPEN:

        	SPI_DRIVER_MODE * mode;
        	mode = (SPI_DRIVER_MODE *)hnd->mode.as_voidptr;
        	if(mode)
        	{
        		if(!drv_data->cnt)
        		{
#if USE_SPI_DMA_DRIVER
        			if(!drv_data->rx_dma_hnd.drv_open(
        					drv_info->rx_dma_mode.dma_index,
        					&drv_info->rx_dma_mode))
        				break;
        			if(!drv_data->tx_dma_hnd.drv_open(
        					drv_info->tx_dma_mode.dma_index,
        					&drv_info->tx_dma_mode))
        				break;
#endif
        			SPI_ENABLE(drv_info);
        		}
    			drv_data->cnt++;
        		PIO_Cfg(mode->cs_pin);
				hnd->res = RES_OK;
        	}
        	break;

		case DCR_CANCEL:
			break;

		case DCR_CLOSE:
			if(!--drv_data->cnt)
			{
				RCCPeripheralDisable(drv_info->info.peripheral_indx);
#if USE_SPI_DMA_DRIVER
				drv_data->rx_dma_hnd.close();
				drv_data->tx_dma_hnd.close();
#endif
			}
        	break;

		case DCR_SIGNAL:
			//signal rx/dma complete
			if(hnd == &drv_data->rx_dma_hnd)
			{
				//process rx dma signal only (data shifted out)

				hnd = drv_data->pending;
				if(hnd)
				{
					//pending is done
					if(!drv_data->locker)
					{
						SPI_END_TRANSACTION((SPI_DRIVER_MODE *)hnd->mode.as_voidptr);
					}
					drv_data->pending = hnd->next;
					svc_HND_SET_STATUS(hnd, RES_SIG_OK);

					//resume waiting
					SPI_RESUME(drv_info, drv_data);
				}
			}
			break;


        case DCR_PARAMS:
        	//send handle's client to release the lock
        	if(hnd == (HANDLE)drv_data->locker)
        	{
        		//unlock
        		drv_data->locker = NULL;

        		//start waiting
        		hnd = drv_data->waiting;
        		if(hnd)
        		{
        			drv_data->waiting = hnd->next;
        			SPI_DSR(drv_info, hnd);
        		}
        	}
        	break;

   }
}

//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void SPI_DSR(SPI_DRIVER_INFO* drv_info, HANDLE hnd)
{
	Task* locker;
    SPI_DRIVER_DATA* drv_data = drv_info->drv_data;

    if((locker = drv_data->locker))
	{
        //the SPI is locked
        if(locker != hnd->client.task)
        {
        	//the SPI is locked from other client
        	hnd->list_add(drv_data->waiting);
        	return;
        }

		if( !(hnd->cmd & FLAG_LOCK) )
			drv_data->locker = 0;

		if(	drv_data->pending)
		{
			hnd->list_add(drv_data->pending);
			return;
		}

		//start hnd
	} else
    {
		//the SPI is not locked
		if(	drv_data->pending)
		{
			//but is busy
			hnd->list_add(drv_data->waiting);
			return;
		}

		//the SPI is idle so start a new transaction
		SPI_START_TRANSACTION(drv_info, (SPI_DRIVER_MODE*)hnd->mode.as_voidptr);
		if( hnd->cmd & FLAG_LOCK)
			drv_data->locker = hnd->client.task;
    }



	if(hnd->len)
	{
		hnd->next = NULL;
		hnd->res  = RES_BUSY;
		drv_data->pending = hnd;

		START_HND(drv_info, drv_data, hnd);

	} else
	{
		if(!drv_data->locker)
			SPI_END_TRANSACTION((SPI_DRIVER_MODE *)hnd->mode.as_voidptr);

		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}

//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void SPI_ISR(SPI_DRIVER_INFO* drv_info)
{
	SPI_TypeDef* pSPI = drv_info->hw_base;
    SPI_DRIVER_DATA* drv_data ;
	unsigned int status, cr1;

	cr1 = pSPI->SPI_CR1;
	status = pSPI->SPI_SR;
	drv_data = drv_info->drv_data;

	if(status & SPI_SR_OVR)
	{
		//this should never happen but just in case...
		// Clearing the OVR bit is done by a read operation on the SPI_DR register
		// followed by a read access to the SPI_SR register
		pSPI->SPI_DR;
		status |= pSPI->SPI_SR;
	}

	if(status & SPI_SR_MODF)
	{
//		Use the following software sequence to clear the MODF bit:
//		1. Make a read or write access to the SPI_SR register while the MODF bit is set.
//		2. Then write to the SPI_CR1 register.
		pSPI->SPI_CR1 = cr1;
	}


	if(status & SPI_SR_RXNE)
	{
		HANDLE hnd;

		status = pSPI->SPI_DR;
		hnd = drv_data->pending;
		if(hnd)
		{
			if(hnd->cmd & FLAG_READ)
			{
				*hnd->dst.as_charptr++ = status;
			}

			if(--hnd->len)
			{
				// send next byte..
				if(hnd->cmd & FLAG_WRITE)
				{
					pSPI->SPI_DR = *hnd->src.as_byteptr++;
				} else
				{
					pSPI->SPI_DR = 0xFF;
				}
			} else
			{
				//done
				if(!drv_data->locker)
				{
					SPI_END_TRANSACTION((SPI_DRIVER_MODE *)hnd->mode.as_voidptr);
				}

				drv_data->pending = hnd->next;
				usr_HND_SET_STATUS(hnd, RES_SIG_OK);

				SPI_RESUME(drv_info, drv_data);
			}
		}
	}
}
