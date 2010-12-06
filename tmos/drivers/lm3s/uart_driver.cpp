/*
 * uart_driver.cpp
 *
 *  Created on: 2010-3-19
 *
 */
#include <drivers.h>

void ConfigureUart(UART_DRIVER_INFO * drv_info, UART_DRIVER_DATA * drv_data, UART_DRIVER_MODE * uart_mode)
{
	UART_Type * Uart = drv_info->hw_base;
	Uart->UARTFIFOEnable();
	Uart->UARTFIFOLevelSet(UART_FIFO_TX4_8, UART_FIFO_RX4_8);
//	Uart->UARTTxIntModeSet(UART_TXINT_MODE_FIFO);//UART_TXINT_MODE_EOT);
	Uart->UARTTxIntModeSet(UART_TXINT_MODE_EOT);
	if(drv_data->mode.hw_flow && drv_info->info.drv_index == UART1_IRQn)
		Uart->UARTFlowControlSet(UART_FLOWCONTROL_TX |UART_FLOWCONTROL_RX);
	Uart->UARTConfigSetExpClk(SYSCTL->SysCtlClockGet(), uart_mode->baudrate, uart_mode->mode); // enable uart
	Uart->UARTEnable();

	NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority   );
	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);

	SYSCTL->SysCtlPeripheralDeepSleepEnable(drv_info->info.peripheral_indx);
    SYSCTL->SysCtlPeripheralSleepEnable(drv_info->info.peripheral_indx);

	memcpy(&drv_data->mode, uart_mode, sizeof(UART_DRIVER_MODE));
}

static inline void STOP_TX(UART_Type * Uart)
{
	Uart->UARTIntDisable(UART_INT_TX);
}

static inline void STOP_RX(UART_Type * Uart)
{
	Uart->UARTIntDisable(UART_INT_RX|UART_INT_RT);
}

static inline void RESUME_RX(UART_Type * Uart)
{
	Uart->UARTIntEnable(UART_INT_RX|UART_INT_RT);
}

static inline void START_RX_BUF(UART_Type * Uart, UART_DRIVER_INFO * drv_info, UART_DRIVER_DATA * drv_data)
{
#ifdef HW_VER_10
	if(drv_info->info.drv_index != UART1_IRQn && drv_info->drv_data->mode.hw_flow)
		PIO_ClrOutput(drv_info->uart_pins[RTS_PIN]);
#endif
	drv_data->rx_ptr = drv_data->rx_buf;
	drv_data->rx_wrptr = drv_data->rx_buf;
	drv_data->rx_remaining = RX_BUF_SIZE;
	Uart->UARTIntEnable(UART_INT_RX|UART_INT_RT);
}

static inline void START_RX_HND(UART_Type * Uart, UART_DRIVER_INFO * drv_info, HANDLE hnd)
{
#ifdef HW_VER_10
	if(drv_info->info.drv_index != UART1_IRQn && drv_info->drv_data->mode.hw_flow)
		PIO_ClrOutput(drv_info->uart_pins[RTS_PIN]);
#endif
	drv_info->drv_data->rx_remaining = hnd->len;
	drv_info->drv_data->rx_wrptr = hnd->dst.as_byteptr;
	Uart->UARTIntEnable(UART_INT_RX|UART_INT_RT);
}

static inline void STOP_RX_HND(UART_Type *Uart, UART_DRIVER_INFO * drv_info, HANDLE hnd)
{
	STOP_RX(Uart);
	drv_info->drv_data->hnd_rcv = hnd->next;
	hnd->len = drv_info->drv_data->rx_remaining;
	usr_HND_SET_STATUS(hnd, RES_SIG_OK);
  	if( (hnd=drv_info->drv_data->hnd_rcv) )
      	START_RX_HND(Uart, drv_info, hnd);
  	else
		START_RX_BUF(Uart, drv_info, drv_info->drv_data);
}
/*
static inline void START_TX_HND(UART_Type * Uart, HANDLE hnd)
{
	Uart->UARTIntDisable(UART_INT_TX);
	while(Uart->UARTSpaceAvail() && hnd->len)
	{
		Uart->DR = *hnd->src.as_charptr++;
		if(!--hnd->len)
			break;
	}
	Uart->UARTIntEnable(UART_INT_TX);
}
*/
static inline void START_TX_HND(UART_Type * Uart, HANDLE hnd)
{
	Uart->UARTIntDisable(UART_INT_TX);
	Uart->UARTTxIntModeSet(UART_TXINT_MODE_EOT);
	if(Uart->UARTSpaceAvail() && hnd->len)
	{
		Uart->DR = *hnd->src.as_charptr++;
		--hnd->len;
	}
	Uart->UARTIntEnable(UART_INT_TX);
}

void TX_CTS(UART_DRIVER_DATA *drv_data, UART_Type * Uart, unsigned int cts)
{
	if(cts)
		Uart->CTL &= ~UART_CTL_TXE; // disable transmit
	else
		Uart->CTL |= UART_CTL_TXE;  // enable transmit
}

void dcr_SerialDriver(UART_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	UART_Type * Uart = drv_info->hw_base;
	UART_DRIVER_DATA *drv_data = drv_info->drv_data;
	switch(reason)
	{
		case DCR_ISR:
			TX_CTS(drv_data, Uart, (unsigned int)hnd);
			return;
		case DCR_RESET:
			SysCtlPeripheralReset(drv_info->info.peripheral_indx);
			SysCtlPeripheralDisable(drv_info->info.peripheral_indx); // ??? turn off
			return;

		case DCR_OPEN:
		{
			UART_DRIVER_MODE *uart_mode = (UART_DRIVER_MODE *)(hnd->mode.as_voidptr);
			if(uart_mode)
			{
				//unsigned long mode, baudrate;
				if(drv_data->cnt)
				{
					if( uart_mode->mode != drv_data->mode.mode ||
					    uart_mode->baudrate != drv_data->mode.baudrate ||
					    uart_mode->hw_flow != drv_data->mode.hw_flow )
					{
						return;
					}
				}
				else
				{
					// Enable AND Reset the UART peripheral
					SysCtlPeripheralReset(drv_info->info.peripheral_indx);
					if(drv_data->mode.hw_flow)
					{
						PIO_Cfg_List((PIN_DESC *)&drv_info->uart_pins[UART_LIST_ALL_PINS]);
						ConfigureUart(drv_info, drv_data, uart_mode);
#ifdef HW_VER_10
						if(drv_info->info.drv_index != UART1_IRQn)
							TX_CTS(drv_data, Uart, PIO_Read(drv_info->uart_pins[CTS_PIN]));
#endif
					}
					else
					{
						PIO_Cfg_List((PIN_DESC *)&drv_info->uart_pins[UART_LIST_RX_TX_PINS]);
						ConfigureUart(drv_info, drv_data, uart_mode);
					}
					START_RX_BUF(Uart, drv_info, drv_data);
				}
				drv_data->cnt++;
				hnd->res = RES_OK;
			}
			return;
		}

		case DCR_CLOSE:
			if(drv_data->cnt)
				drv_data->cnt--;

		case DCR_CANCEL:
    	{
    		if( !(hnd->res & FLG_BUSY))
    			return;

    		if (hnd->cmd & FLAG_READ)
    	    {
    			if(hnd == drv_data->hnd_rcv)
    			{
    				STOP_RX(Uart);
    	      		if(hnd->len > drv_data->rx_remaining || hnd->mode0 )
    	      		{
    	      			hnd->len = drv_data->rx_remaining;
    	      			hnd->res = RES_SIG_OK;
    	      		}
    	      		else
    	      			hnd->res = RES_SIG_IDLE;

    	      		drv_data->hnd_rcv = hnd->next;
    	      		svc_HND_SET_STATUS(hnd, hnd->res);

    		      	if( (hnd=drv_data->hnd_rcv) )
    		          	START_RX_HND(Uart, drv_info, hnd);
    		      	else
    					START_RX_BUF(Uart, drv_info, drv_data);
    			}
    			else
    			{
    				// try cnacel
    				hnd->svc_list_cancel(drv_data->hnd_rcv);
    			}
    		}
    		else
    		{
    			if(hnd == drv_data->hnd_snd)
    			{
    				STOP_TX(Uart);
    				drv_data->hnd_snd = hnd->next;
    	      		svc_HND_SET_STATUS(hnd, RES_SIG_IDLE);
					if( (hnd=drv_data->hnd_snd) )
						START_TX_HND(Uart, hnd);
    			}
    			else
    			{
    				hnd->svc_list_cancel(drv_data->hnd_snd);
    			}

    		}
        	if(!drv_data->cnt)
			{
					NVIC->NVIC_DisableIRQ(drv_info->info.drv_index);
					Uart->UARTDisable();
					STOP_RX(Uart);
					STOP_TX(Uart);
					SysCtlPeripheralDisable(drv_info->info.peripheral_indx);
					SYSCTL->SysCtlPeripheralDeepSleepDisable(drv_info->info.peripheral_indx);
				    SYSCTL->SysCtlPeripheralSleepDisable(drv_info->info.peripheral_indx);

					if(drv_data->mode.hw_flow)
						PIO_Free_List((PIN_DESC *)&drv_info->uart_pins[UART_LIST_ALL_PINS]);
					else
						PIO_Free_List((PIN_DESC *)&drv_info->uart_pins[UART_LIST_RX_TX_PINS]);
			}
        	return;
    	}
        case DCR_CLOCK:
        	return;
	}
}

void dsr_SerialDriver(UART_DRIVER_INFO* drv_info, HANDLE hnd)
{
	unsigned char *ptr;
	signed int size;
	UART_DRIVER_DATA *drv_data = drv_info->drv_data;
	UART_Type * Uart = drv_info->hw_base;

	hnd->next = NULL;
	if (hnd->cmd & FLAG_READ)
    {
		hnd->mode0 = 0; // няма нищо прието в хендъла
      	if(drv_data->hnd_rcv)
      	{
			hnd->list_add(drv_data->hnd_rcv);
			return;
      	}

		STOP_RX(Uart);
		//try to read from buffer
      	ptr = drv_data->rx_wrptr;
      	if( hnd->len && (ptr!= drv_data->rx_ptr) )
      	{
      		if(ptr < drv_data->rx_ptr)
      		{
      			size =min(hnd->len, (unsigned int)(&drv_data->rx_buf[RX_BUF_SIZE]) - (unsigned int)drv_data->rx_ptr);
      			memcpy(hnd->dst.as_byteptr, drv_data->rx_ptr, size);
      			hnd->mode0 = 1; // в хендъла е писано
      			hnd->len -= size;
      			hnd->dst.as_int += size;
      			drv_data->rx_ptr += size;
      			if(drv_data->rx_ptr == &drv_data->rx_buf[RX_BUF_SIZE])
      				drv_data->rx_ptr = drv_data->rx_buf;
      		}

          	if( hnd->len && (ptr!= drv_data->rx_ptr) )
      		{
      			size =min(hnd->len, ptr - drv_data->rx_ptr);
      			hnd->mode0 = 1; // в хендъла е писано
      			memcpy(hnd->dst.as_byteptr, drv_data->rx_ptr, size);
      			hnd->len -= size;
      			hnd->dst.as_int += size;
      			drv_data->rx_ptr += size;
      		}
      	}
      	if (hnd->len == 0)
      	{
      		RESUME_RX(Uart);
      		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
      		return;
      	}

      	//receive directly
      	hnd->res = RES_BUSY;
      	drv_data->hnd_rcv = hnd;
      	START_RX_HND(Uart, drv_info, hnd);
		return;
    }

	if (hnd->cmd & FLAG_WRITE)
    {
		if(hnd->len)
		{
			if(drv_data->hnd_snd)
			{
				hnd->list_add(drv_data->hnd_snd);
				return;
			}
			hnd->res = RES_BUSY;
			drv_data->hnd_snd = hnd;
			START_TX_HND(Uart, hnd);
		}
		else
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		return;
    }

	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);

}

void isr_SerilaDriver(UART_DRIVER_INFO* drv_info )
{
	UART_Type * Uart = drv_info->hw_base;
	UART_DRIVER_DATA * drv_data = drv_info->drv_data;
	HANDLE hnd;

	unsigned int Status = Uart->UARTIntStatus(true);
	Uart->UARTIntClear(Status);

	if( Status & UART_INT_TX)
	{
		if((hnd=drv_data->hnd_snd))
		{
			while(Uart->UARTSpaceAvail())
			{
				if(!hnd->len)
				{
					drv_data->hnd_snd = hnd->next;
					usr_HND_SET_STATUS(hnd, RES_SIG_OK);
					if(!(hnd=drv_data->hnd_snd))
					{
						STOP_TX(Uart);
						break;
					}
				}
				hnd->len--;
				Uart->DR = *hnd->src.as_charptr++;
			}
			if(hnd)
			{
				Uart->UARTTxIntModeSet(UART_TXINT_MODE_FIFO);
			}
		}
		else
		{
			STOP_TX(Uart);
		}
	}

	//check the receiver
    if(Status&(UART_INT_RT|UART_INT_RX))
    {
    	while(Uart->UARTCharsAvail())
    	{
    		drv_data->rx_remaining--;
    		*drv_data->rx_wrptr++ = Uart->DR;
    		if(drv_data->hnd_rcv && !drv_data->hnd_rcv->mode0)
    			drv_data->hnd_rcv->mode0 = 1;
    		if(!drv_data->rx_remaining )
    		{
    	    	if( (hnd=drv_data->hnd_rcv) )
    				STOP_RX_HND(Uart, drv_info, hnd);
    	      	else
    				START_RX_BUF(Uart, drv_info, drv_data);
    		}
#ifdef HW_VER_10
    		else
    		{
    			if(drv_data->mode.hw_flow && drv_info->info.drv_index != UART1_IRQn)
					if((!drv_data->hnd_rcv) && (drv_data->rx_remaining < (RX_BUF_SIZE/4)))
					{
						if(drv_info->uart_pins[RTS_PIN])
							PIO_SetOutput(drv_info->uart_pins[RTS_PIN]);
					}
    		}
#endif
    	}
    	if( (Status&UART_INT_RT) && drv_data->mode.rx_tout )
    	{
	    	if( (hnd=drv_data->hnd_rcv) )
	    	{
	    		if(drv_data->rx_remaining != hnd->len)
	    			STOP_RX_HND(Uart, drv_info, hnd);
	    	}
    	}
	}
}
