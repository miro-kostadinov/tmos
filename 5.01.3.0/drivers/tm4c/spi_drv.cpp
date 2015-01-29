/*
 * spi_drv.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: miro
 */

#include <tmos.h>
#include <spi_drv.h>

//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------

static void SPI_SET_CLOCK(const SPI_DRIVER_INFO* drv_info)
{
	//TO DO: calculate the baudrate
	drv_info->hw_base->SSICPSR  =2;
}

// Enable SPI
static void SPI_ENABLE(const SPI_DRIVER_INFO* drv_info)
{
	QSSI_TypeDef*  pSPI;

	//		1. Enable the QSSI module using the RCGCSSI register
	SysCtlPeripheralEnable(drv_info->info.peripheral_indx);



	//		2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 382)..
	//		3. Set the GPIO AFSEL bits for the appropriate pins (see page 770)
	//		4. Configure the PMCn fields in the GPIOPCTL register to assign the QSSI signals to the appropriate
	//		pins.
	// 		5. Program the GPIODEN register to enable the pin's digital function. In addition, the drive strength,
	//		  drain select and pull-up/pull-down functions must be configured.
	PIO_Cfg_List(drv_info->spi_pins);

	pSPI = drv_info->hw_base;
    pSPI->SSICR1 = 0;
	pSPI->SSIIM = 0;// SSI_RXTO + SSI_RXOR + SSI_TXFF + SSI_RXFF;
	pSPI->SSICPSR  = 3;
	drv_enable_isr(&drv_info->info);

}

// Start a new transaction
static void SPI_START_TRANSACTION(QSSI_TypeDef*  pSPI, SPI_DRIVER_MODE* mode)
{

	//		1. If initializing out of reset, ensure that the SSE bit in the SSICR1 register is clear before making
	//		any configuration changes. Otherwise, configuration changes for Advanced SSI can be made
	//		while the SSE bit is set
	//
	//		2. Select whether the QSSI is a master or slave:
	//		a. a. For master operations, set the SSICR1 register to 0x0000.0000.
	//		b. For slave mode (output enabled), set the SSICR1 register to 0x0000.0004.
	//		c. For slave mode (output disabled), set the SSICR1 register to 0x0000.000C.
    pSPI->SSICR1 = 0;

    // 		3. Configure the QSSI clock source by writing to the SSICC register.
    //		4. Configure the clock prescale divisor by writing the SSICPSR register.
    //		5. Write the SSICR0 register with the following configuration:
    //			- Serial clock rate (SCR)
    //			- Desired clock phase/polarity, if using Freescale SPI mode (SPH and SPO)
    //			- The protocol mode: Freescale SPI or TI SSF
    //			- The data size (DSS)
    pSPI->SSICR0 = mode->cr0_reg;

    //		6. Optionally, configure the SSI module for uDMA use with the following steps:
    //			a. Configure a uDMA for SSI use. See “Micro Direct Memory Access (uDMA)” on page 678 for
    //			more information.
    //			b. Enable the SSI Module's TX FIFO or RX FIFO by setting the TXDMAE or RXDMAE bit in the
    //			SSIDMACTL register.
    //			c. Optionally, enable the uDMA completion interrupt by setting the DMATXIM or DMARXIM bit
    //			in the SSIIM register.

    //		7. If this is the first initialization out of reset, enable the QSSI by setting the SSE bit in the SSICR1
    //		register
    pSPI->SSICR1 |= QSSI_SSICR1_SSE;



    // Assert CS
    PIO_Assert(mode->cs_pin);
}

static void SPI_HND_TRY_SEND(HANDLE hnd, SPI_DRIVER_DATA* drv_data, QSSI_TypeDef* pSPI)
{
	if(hnd->cmd & FLAG_WRITE)
	{
		if((((SPI_DRIVER_MODE *)hnd->mode.as_voidptr)->cr0_reg & QSSI_SSICR0_DSS) < 8 )
		{
			//then try to send up to 8 bytes (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
			while(hnd->len && (drv_data->buffered < 8))
			{
				pSPI->SSIDR = *hnd->src.as_byteptr++;
//				TRACELN("que: %x", temp);
				hnd->len--;
				drv_data->buffered++;
			}
		}
		else
		{
			//then try to send up to 8 short (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
			while(hnd->len && (drv_data->buffered < 8))
			{
				pSPI->SSIDR = *hnd->src.as_shortptr++;
//				TRACELN("que: %x", temp);
				hnd->len--;
				drv_data->buffered++;
			}
		}
	} else
	{
		while(hnd->len && (drv_data->buffered < 8))
		{
			pSPI->SSIDR = 0xFFFF;
//			TRACELN("que: %x", temp);
			hnd->len--;
			drv_data->buffered++;
		}
	}
}


//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void SPI_DCR(const SPI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	switch(reason)
    {
		case DCR_CLOSE:

			//cancel 1st?
			if(--drv_info->drv_data->cnt)
				break;
        	SysCtlPeripheralDisable(drv_info->info.peripheral_indx);
        	break;

        case DCR_RESET:
        	SysCtlPeripheralDisable(drv_info->info.peripheral_indx);
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

        case DCR_PARAMS:
        	//send handle's client to release the lock
        	if(hnd == (HANDLE)drv_info->drv_data->locker)
        	{
        		drv_info->drv_data->locker = NULL;
        		hnd = drv_info->drv_data->waiting;
        		if(hnd)
        		{
        			drv_info->drv_data->waiting = hnd->next;
        			SPI_DSR(drv_info, hnd);
        		}
        	}
        	break;

        case DCR_CLOCK:
        	SPI_SET_CLOCK(drv_info);
        	break;
   }
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void SPI_DSR(const SPI_DRIVER_INFO* drv_info, HANDLE hnd)
{
	Task* locker;
    SPI_DRIVER_DATA* drv_data = drv_info->drv_data;
	QSSI_TypeDef*  pSPI = drv_info->hw_base;


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
			return ;
		}

		//the SPI is idle so start a new transaction
		SPI_START_TRANSACTION(pSPI, (SPI_DRIVER_MODE *)hnd->mode.as_voidptr);
		if( hnd->cmd & FLAG_LOCK)
			drv_data->locker = hnd->client.task;
    }


	if(hnd->len)
	{
		hnd->next = NULL;
		hnd->res  = RES_BUSY;
		drv_data->pending = hnd;
	    //enable interrupts
		pSPI->SSIIM = QSSI_SSIIM_TXIM + QSSI_SSIIM_RXIM;

		SPI_HND_TRY_SEND(hnd, drv_data, pSPI);

	} else
	{
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}


}


//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void SPI_ISR(const SPI_DRIVER_INFO* drv_info )
{
    SPI_DRIVER_DATA* drv_data ;
	QSSI_TypeDef*  pSPI;
	HANDLE hnd;

	drv_data = drv_info->drv_data;
	pSPI = drv_info->hw_base;
	hnd = drv_data->pending;

	if(!hnd)
	{
		// disable interrupts
		pSPI->SSIIM = 0;//SSI_RXTO + SSI_RXOR + SSI_TXFF + SSI_RXFF;
		return;
	}
//	TRACELN("SIRQ: %x", pSPI->MIS);

	//two possible interrupts are expected (RX-half full) (End of Transfer)
	//but we always try to clean RX fifo first
	if(pSPI->SSISR & QSSI_SSISR_RNE)
	{
		if(hnd->cmd & FLAG_READ)
		{
			if((((SPI_DRIVER_MODE *)hnd->mode.as_voidptr)->cr0_reg & QSSI_SSICR0_DSS) < 8 )
			{
				do
				{
					ASSERT(drv_data->buffered--);

					*hnd->dst.as_byteptr++ = pSPI->SSIDR;
		//			TRACELN("rcv: %x", temp);
				} while(pSPI->SSISR & QSSI_SSISR_RNE);
			}
			else
			{
				do
				{
					ASSERT(drv_data->buffered--);

					*hnd->dst.as_shortptr++ = pSPI->SSIDR;
		//			TRACELN("rcv: %x", temp);
				} while(pSPI->SSISR & QSSI_SSISR_RNE);
			}
		} else
		{
			do
			{
				ASSERT(drv_data->buffered--);

				pSPI->SSIDR;
	//			TRACELN("rcv: %x", temp);
			} while(pSPI->SSISR & QSSI_SSISR_RNE);
		}
	}

	//then try to send up to 8 bytes (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
	SPI_HND_TRY_SEND(hnd, drv_data, pSPI);

	if(!(pSPI->SSISR & QSSI_SSISR_BSY) && !hnd->len)
	{
		//noting to send, but we may have something to receive...
		while(drv_data->buffered)
		{
			ASSERT(pSPI->SSISR & QSSI_SSISR_RNE);
			unsigned int temp = pSPI->SSIDR;
//			TRACELN("rcv1: %x", temp);
			if(hnd->cmd & FLAG_READ)
			{
				if((((SPI_DRIVER_MODE *)hnd->mode.as_voidptr)->cr0_reg & QSSI_SSICR0_DSS) < 8 )
					*hnd->dst.as_byteptr++ = temp;
				else
					*hnd->dst.as_shortptr++ = temp;
			}
			drv_data->buffered--;
		}

		//noting to send, nothing to receive...
		if(!drv_data->locker)
		{
			//SPI_END_TRANSACTION()
        	SPI_DRIVER_MODE * mode;

        	mode = (SPI_DRIVER_MODE *)hnd->mode.as_voidptr;
        	ASSERT(mode);
        	PIO_Deassert(mode->cs_pin);

		}

		drv_data->pending = hnd->next;
		usr_HND_SET_STATUS(hnd, RES_SIG_OK);

		if( !drv_data->pending )
		{
			// current is done, try to start new transactions...
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
				hnd->list_remove(drv_data->waiting);
				drv_data->pending = hnd;
				hnd->next = NULL;

				if(!drv_data->locker)
				{
					SPI_START_TRANSACTION(pSPI, (SPI_DRIVER_MODE *)hnd->mode.as_voidptr);
					if( hnd->cmd & FLAG_LOCK)
						drv_data->locker = hnd->client.task;
				}

				//then try to send up to 8 bytes (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
				SPI_HND_TRY_SEND(hnd, drv_data, pSPI);

			} else
			{
				// disable interrupts
				pSPI->SSIIM = 0;//SSI_RXTO + SSI_RXOR + SSI_TXFF + SSI_RXFF;
			}
		}
	}
}



