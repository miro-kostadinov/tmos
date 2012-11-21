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
static void SPI_START_TRANSACTION(SPI_TypeDef*  pSPI, SPI_DRIVER_MODE* mode)
{

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


    pSPI->SPI_CR2 = (pSPI->SPI_CR2 & (SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN)) | mode->spi_cr2;


    // Assert CS
    PIO_Assert(mode->cs_pin);

}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void SPI_DCR(SPI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	switch(reason)
    {
        case DCR_RESET:
			RCCPeripheralReset(drv_info->info.peripheral_indx);
        	RCCPeripheralDisable(drv_info->info.peripheral_indx);
        	break;

        case DCR_OPEN:

        	SPI_DRIVER_MODE * mode;
        	mode = (SPI_DRIVER_MODE *)hnd->mode.as_voidptr;
        	if(mode)
        	{
        		if(!drv_info->drv_data->cnt++)
        		{
        			SPI_ENABLE(drv_info);
        		}
        		PIO_Cfg(mode->cs_pin);
				hnd->res = RES_OK;
        	}
        	break;

		case DCR_CANCEL:
			break;

		case DCR_CLOSE:
			if(!--drv_info->drv_data->cnt)
				RCCPeripheralDisable(drv_info->info.peripheral_indx);
        	break;


        case DCR_PARAMS:
        	//send handle's client to release the lock
        	if(hnd == (HANDLE)drv_info->drv_data->locker)
        	{
        		//unlock
        		drv_info->drv_data->locker = NULL;

        		//start waiting
        		hnd = drv_info->drv_data->waiting;
        		if(hnd)
        		{
        			drv_info->drv_data->waiting = hnd->next;
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
		SPI_START_TRANSACTION(drv_info->hw_base, (SPI_DRIVER_MODE*)hnd->mode.as_voidptr);
		if( hnd->cmd & FLAG_LOCK)
			drv_data->locker = hnd->client.task;
    }


    /*
	if(hnd->len)
	{
		hnd->next = NULL;
		hnd->res  = RES_BUSY;
		drv_data->pending = hnd;
	    //enable interrupts (-enable + disable)
		drv_info->hw_base->IM = SSI_TXFF + SSI_RXFF ; //SSI_RXTO + SSI_RXOR - SSI_TXFF - SSI_RXFF;

		if(hnd->cmd & FLAG_WRITE)
		{
			if((((SPI_DRIVER_MODE *)hnd->mode.as_voidptr)->cr0_reg & SSI_CR0_DSS_M) < 8 )
			{
				//then try to send up to 8 bytes (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
				while(hnd->len && (drv_data->buffered < 8))
				{
					drv_info->hw_base->DR = *hnd->src.as_byteptr++;
					//			TRACELN("que: %x", temp);
					hnd->len--;
					drv_data->buffered++;
				}
			}
			else
			{
				//then try to send up to 8 short (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
				while(hnd->len && (drv_data->buffered < 8))
				{
					drv_info->hw_base->DR = *hnd->src.as_shortptr++;
					//			TRACELN("que: %x", temp);
					hnd->len--;
					drv_data->buffered++;
				}
			}
		} else
		{
			while(hnd->len && (drv_data->buffered < 8))
			{
				drv_info->hw_base->DR = 0xFFFF;
	//			TRACELN("que: %x", temp);
				hnd->len--;
				drv_data->buffered++;
			}
		}

	} else
	{
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
*/
}

//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void SPI_ISR(SPI_DRIVER_INFO* drv_info)
{

}
