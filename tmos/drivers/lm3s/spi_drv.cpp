/*
 * spi_drv.cpp
 *
 *  Created on: 2010-4-21
 *      Author: Miroslav Kostadinov
 *
 *      This driver supports SPI transfer with 4-8 bit size, master mode only!
 *
 */

#include <drivers.h>

#include "spi_drv.h"


//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------

static void SPI_SET_CLOCK(SPI_DRIVER_INFO* drv_info)
{
	//TO DO: calculate the baudrate
	drv_info->hw_base->CPSR  =10;
}

// Enable SPI
static void SPI_ENABLE(SPI_DRIVER_INFO* drv_info)
{
	SSI_Type*  pSPI;

	//		1. Enable the SSI module by setting the SSI bit in the RCGC1 register.
	SysCtlPeripheralEnable(drv_info->info.peripheral_indx);



	//		2. Enable the clock to the appropriate GPIO module via the RCGC2 register (see page 184). To
	//		find out which GPIO port to enable, refer to Table 24-5 on page 1045.
	//
	//		3. Set the GPIO AFSEL bits for the appropriate pins (see page 339). To determine which GPIOs to
	//		configure, see Table 24-4 on page 1038.
	//
	//		4. Configure the PMCn fields in the GPIOPCTL register to assign the SSI signals to the appropriate
	//		pins. See page 357 and Table 24-5 on page 1045.

	PIO_Cfg_List((PIN_DESC*)drv_info->spi_pins);

	pSPI = drv_info->hw_base;
    pSPI->CR1 = SSI_CR1_EOT;
	pSPI->IM = 0;// SSI_RXTO + SSI_RXOR + SSI_TXFF + SSI_RXFF;
	pSPI->CPSR  =10;
	NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);
	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);

	//
	// Configure the SSI0CLK and SSIOTX pins for SSI operation.
	//
//        	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
//        	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,
//        	                 GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
}

// Start a new transaction
static void SPI_START_TRANSACTION(SSI_Type*  pSPI, SPI_DRIVER_MODE* mode)
{

	//		1. Ensure that the SSE bit in the SSICR1 register is clear before making any configuration changes.
	//
	//		2. Select whether the SSI is a master or slave:
	//		a. For master operations, set the SSICR1 register to 0x0000.0000.
	//		b. For slave mode (output enabled), set the SSICR1 register to 0x0000.0004.
	//		c. For slave mode (output disabled), set the SSICR1 register to 0x0000.000C.

    pSPI->CR1 = SSI_CR1_EOT;

    //		4. Write the SSICR0 register with the following configuration:
    //		* Serial clock rate (SCR)
    //		* Desired clock phase/polarity, if using Freescale SPI mode (SPH and SPO)
    //		* The protocol mode: Freescale SPI, TI SSF, MICROWIRE (FRF)
    //		* The data size (DSS)

    pSPI->CR0 = mode->cr0_reg;


    //
    //		6. Enable the SSI by setting the SSE bit in the SSICR1 register.
    pSPI->CR1 = SSI_CR1_SSE + SSI_CR1_EOT;



    // Assert CS
    //drv_info->pPIO->PIO_CODR = mode->cs_pin;
    PIO_ClrOutput(mode->cs_pin);

}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void dcr_SPI_driver(SPI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	switch(reason)
    {
		case DCR_CLOSE:

			//cancel 1st?
			if(--drv_info->drv_data->cnt)
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
        		PIO_SetOutput(mode->cs_pin);
				hnd->res = RES_OK;
        	}
        	break;


        case DCR_CLOCK:
        	SPI_SET_CLOCK(drv_info);
   }
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void dsr_SPI_driver(SPI_DRIVER_INFO* drv_info, HANDLE hnd)
{
	Task* locker;
    SPI_DRIVER_DATA* drv_data = drv_info->drv_data;

//	TRACELN("SHND: %x %x", hnd->cmd, hnd->len);
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
		SPI_START_TRANSACTION(drv_info->hw_base, (SPI_DRIVER_MODE *)hnd->mode.as_voidptr);
		if( hnd->cmd & FLAG_LOCK)
			drv_data->locker = hnd->client.task;


    }


	if(hnd->len)
	{
		hnd->next = NULL;
		hnd->res  = RES_BUSY;
		drv_data->pending = hnd;
	    //enable interrupts (-enable + disable)
		drv_info->hw_base->IM = SSI_TXFF + SSI_RXFF ; //SSI_RXTO + SSI_RXOR - SSI_TXFF - SSI_RXFF;

		//then try to send up to 8 bytes (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
		if(hnd->cmd & FLAG_WRITE)
		{
			while(hnd->len && (drv_data->buffered < 8))
			{
				drv_info->hw_base->DR = *hnd->src.as_byteptr++;
				//			TRACELN("que: %x", temp);
				hnd->len--;
				drv_data->buffered++;
			}
		} else
		{
			while(hnd->len && (drv_data->buffered < 8))
			{
				drv_info->hw_base->DR = 0xFF;
	//			TRACELN("que: %x", temp);
				hnd->len--;
				drv_data->buffered++;
			}
		}

	} else
	{
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}


}


//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void isr_SPI_driver(SPI_DRIVER_INFO* drv_info )
{
    SPI_DRIVER_DATA* drv_data ;
	SSI_Type*  pSPI;
	HANDLE hnd;

	drv_data = drv_info->drv_data;
	pSPI = drv_info->hw_base;
	hnd = drv_data->pending;
//	ASSERT(hnd);

	if(!hnd)
	{
		// disable interrupts
		pSPI->IM = 0;//SSI_RXTO + SSI_RXOR + SSI_TXFF + SSI_RXFF;
		return;
	}
//	TRACELN("SIRQ: %x", pSPI->MIS);
	//two possible interrupts are expected (RX-half full) (End of Transfer)
	//but we always try to clean RX fifo first
	if(pSPI->SR & SSI_SR_RNE)
	{
		if(hnd->cmd & FLAG_READ)
		{
			do
			{
				ASSERT(drv_data->buffered--);

				*hnd->dst.as_byteptr++ = pSPI->DR;
	//			TRACELN("rcv: %x", temp);
			} while(pSPI->SR & SSI_SR_RNE);
		} else
		{
			do
			{
				ASSERT(drv_data->buffered--);

				pSPI->DR;
	//			TRACELN("rcv: %x", temp);
			} while(pSPI->SR & SSI_SR_RNE);
		}
	}

	//then try to send up to 8 bytes (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
	if(hnd->cmd & FLAG_WRITE)
	{
		while(hnd->len && (drv_data->buffered < 8))
		{
			pSPI->DR = *hnd->src.as_byteptr++;
			//		TRACELN("snd: %x", temp);
			hnd->len--;
			drv_data->buffered++;
		}
	} else
	{
		while(hnd->len && (drv_data->buffered < 8))
		{
			pSPI->DR = 0xFF;
			//		TRACELN("snd: %x", temp);
			hnd->len--;
			drv_data->buffered++;
		}
	}

	if(!(pSPI->SR & SSI_SR_BSY))
//	if(pSPI->MIS & SSI_TXFF)
	{
		if(hnd->len)
		{
			//this is strange, but possible
			//We first read then write as much as possible for read. We do not write only if there is too much to read.
			// So it is not possible nothing to be expected if we have something still
			ASSERT((pSPI->SR & SSI_SR_RNE));
			return;
		}
		//noting to send, but we may have something to receive...
		while(drv_data->buffered)
		{
			ASSERT(pSPI->SR & SSI_SR_RNE);
			unsigned int temp = pSPI->DR;
//			TRACELN("rcv1: %x", temp);
			if(hnd->cmd & FLAG_READ)
				*hnd->dst.as_byteptr++ = temp;

			drv_data->buffered--;
		}
		//noting to send, nothing to receive...
		if(!drv_data->locker)
		{
			//SPI_END_TRANSACTION()
        	SPI_DRIVER_MODE * mode;

        	mode = (SPI_DRIVER_MODE *)hnd->mode.as_voidptr;
        	ASSERT(mode);
        	PIO_SetOutput(mode->cs_pin);

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
					SPI_START_TRANSACTION(drv_info->hw_base, (SPI_DRIVER_MODE *)hnd->mode.as_voidptr);
					if( hnd->cmd & FLAG_LOCK)
						drv_data->locker = hnd->client.task;
				}

				//then try to send up to 8 bytes (TX FIFO may have more space, but we do not want to overrun the RX FIFO)
				if(hnd->cmd & FLAG_WRITE)
				{
					while(hnd->len && (drv_data->buffered < 8))
					{
						drv_info->hw_base->DR = *hnd->src.as_byteptr++;
						//			TRACELN("que: %x", temp);
						hnd->len--;
						drv_data->buffered++;
					}
				} else
				{
					while(hnd->len && (drv_data->buffered < 8))
					{
						drv_info->hw_base->DR = 0xFF;
			//			TRACELN("que: %x", temp);
						hnd->len--;
						drv_data->buffered++;
					}
				}

			} else
			{
				// disable interrupts
				pSPI->IM = 0;//SSI_RXTO + SSI_RXOR + SSI_TXFF + SSI_RXFF;
			}
		}
	}
}
