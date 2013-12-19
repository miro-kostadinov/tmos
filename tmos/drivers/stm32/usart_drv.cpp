/*
 * usart_drv.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <usart_drv.h>

void ConfigureUsart(USART_DRIVER_INFO * drv_info, USART_DRIVER_DATA * drv_data,
		USART_DRIVER_MODE * mode)
{
	USART_TypeDef* USARTx = drv_info->hw_base;
	uint32_t integerdivider , fractionaldivider;

	USARTx->USART_CR2 = mode->mode_cr2;
	USARTx->USART_CR1 = mode->mode_cr1;
	USARTx->USART_CR3 = mode->mode_cr3;

	/* Configure the USART Baud Rate -------------------------------------------*/
	integerdivider = ((25 * get_usart_source_clk(drv_info->info.peripheral_indx))
			/ (2 * mode->baudrate));
	if( !(mode->mode_cr1 & USART_CR1_OVER8) )
		integerdivider /= 2;

	fractionaldivider = integerdivider;
	integerdivider /= 100;
	fractionaldivider -=  100 * integerdivider;
	if (mode->mode_cr1 & USART_CR1_OVER8)
	{
		fractionaldivider = (((fractionaldivider * 8) + 50) / 100) &  0x07;
	}
	else
	{
		fractionaldivider = (((fractionaldivider * 16) + 50) / 100) &  0x0F;
	}

	USARTx->USART_BRR = USART_BRR_DIV_Mantissa_Set(integerdivider) | fractionaldivider;

	drv_enable_isr(&drv_info->info);

	memcpy(&drv_data->mode, mode, sizeof(USART_DRIVER_MODE));
}

static inline void START_RX_BUF(USART_TypeDef* uart, USART_DRIVER_DATA* drv_data)
{
	drv_data->rx_ptr = drv_data->rx_buf;
	drv_data->rx_wrptr = drv_data->rx_buf;
	drv_data->rx_remaining = USART_DRV_RX_BUF_SIZE;
	enable_usart_drv_ints(uart, USART_STATUS_RXNEIE | USART_STATUS_IDLEIE);
}

static inline void START_RX_HND(USART_TypeDef* uart, USART_DRIVER_DATA* drv_data, HANDLE hnd)
{
	drv_data->rx_remaining = hnd->len;
	drv_data->rx_wrptr = hnd->dst.as_byteptr;
	enable_usart_drv_ints(uart, USART_STATUS_RXNEIE | USART_STATUS_IDLEIE);
}

static inline void STOP_TX(USART_TypeDef* uart)
{
	uart->USART_CR1 &= ~USART_CR1_TXEIE;
}

static inline void START_TX_HND(USART_TypeDef* uart, HANDLE hnd)
{
	if( get_usart_sr(uart) & USART_STATUS_TXE )
	{
//		TRACE_BUF(hnd->src.as_charptr, 1, TC_BG_MAGENTA);
		get_usart_tdr(uart) = *hnd->src.as_charptr++;
		hnd->len--;
	}
	uart->USART_CR1 |= USART_CR1_TXEIE;
}

static inline void STOP_RX_HND(USART_TypeDef* uart, USART_DRIVER_DATA* drv_data, HANDLE hnd, RES_CODE res)
{
	// STOP_RX(Uart);
	drv_data->hnd_rcv = hnd->next;
	hnd->dst.as_int += hnd->len - drv_data->rx_remaining;
	hnd->len = drv_data->rx_remaining;
	usr_HND_SET_STATUS(hnd, res);
  	if( (hnd=drv_data->hnd_rcv) )
      	START_RX_HND(uart, drv_data, hnd);
  	else
		START_RX_BUF(uart, drv_data);
}

static inline void RESUME_RX(USART_TypeDef* uart, USART_DRIVER_DATA* drv_data)
{
	unsigned int free_space;

	//	update wr ptr
	if (drv_data->rx_wrptr == &drv_data->rx_buf[USART_DRV_RX_BUF_SIZE])
		drv_data->rx_wrptr = drv_data->rx_buf;

	// get free space
	if (drv_data->rx_wrptr < drv_data->rx_ptr)
	{
		free_space = drv_data->rx_ptr - drv_data->rx_wrptr;
		drv_data->rx_remaining = free_space;
	}
	else
	{
		free_space = &drv_data->rx_buf[USART_DRV_RX_BUF_SIZE] - drv_data->rx_wrptr;
		drv_data->rx_remaining = free_space;
		free_space += drv_data->rx_ptr - drv_data->rx_buf;
	}

	// enable or disable the receiver...
	if(free_space <= 1)
	{
		disable_usart_drv_ints(uart, USART_STATUS_RXNEIE | USART_STATUS_IDLEIE);
	}
	else
	{
		enable_usart_drv_ints(uart, USART_STATUS_RXNEIE | USART_STATUS_IDLEIE);
	}

}

static void dcr_cancel_handle(USART_DRIVER_INFO* drv_info,
		USART_DRIVER_DATA *drv_data, HANDLE hnd)
{
	USART_TypeDef * uart = drv_info->hw_base;

	if( hnd->res & FLG_BUSY )
	{
		if (hnd->cmd & FLAG_READ)
	    {
			if(hnd == drv_data->hnd_rcv)
			{
				RES_CODE res;

				//STOP_RX(Uart);
	      		drv_data->hnd_rcv = hnd->next;

	      		res  = hnd->res ^ (FLG_SIGNALED | FLG_BUSY);
	      		if(hnd->len > drv_data->rx_remaining  )
	      		{
	      			hnd->len = drv_data->rx_remaining;
	      			res = RES_SIG_OK;
	      		}

	      		svc_HND_SET_STATUS(hnd, res);

		      	if( (hnd=drv_data->hnd_rcv) )
		          	START_RX_HND(uart, drv_data, hnd);
		      	else
					START_RX_BUF(uart, drv_data);
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
				STOP_TX(uart);
				drv_data->hnd_snd = hnd->next;
	      		svc_HND_SET_STATUS(hnd, RES_SIG_IDLE);
				if( (hnd=drv_data->hnd_snd) )
					START_TX_HND(uart, hnd);
			}
			else
			{
				hnd->svc_list_cancel(drv_data->hnd_snd);
			}

		}
	}

}

void USART_DCR(USART_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
//	UART_Type * Uart = drv_info->hw_base;
	USART_DRIVER_DATA *drv_data = drv_info->drv_data;

	switch(reason)
	{
		case DCR_RESET:
			RCCPeripheralReset(drv_info->info.peripheral_indx);
			RCCPeripheralDisable(drv_info->info.peripheral_indx); // ??? turn off
			break;

		case DCR_OPEN:
		{
			USART_DRIVER_MODE *usart_mode = (USART_DRIVER_MODE *)(hnd->mode.as_voidptr);
			if(usart_mode)
			{
				//unsigned long mode, baudrate;
				if(drv_data->cnt)
				{
					if( usart_mode->mode_cr1 != drv_data->mode.mode_cr1 ||
						usart_mode->mode_cr2 != drv_data->mode.mode_cr2 ||
						usart_mode->mode_cr3 != drv_data->mode.mode_cr3 ||
						usart_mode->baudrate != drv_data->mode.baudrate)
					{
						break;
					}
				}
				else
				{
					// Enable AND Reset the UART peripheral
					RCCPeripheralEnable(drv_info->info.peripheral_indx);
					RCCPeripheralReset(drv_info->info.peripheral_indx);
					PIO_Cfg_List(drv_info->uart_pins);
					ConfigureUsart(drv_info, drv_data, usart_mode);
					START_RX_BUF(drv_info->hw_base, drv_data);
				}
				drv_data->cnt++;
				hnd->res = RES_OK;
			}
			break;
		}

		case DCR_CLOSE:
			if(drv_data->cnt)
				drv_data->cnt--;
			if(!drv_data->cnt)
			{
				//Disable ?
				NVIC_DisableIRQ(drv_info->info.drv_index);
				//	Uart->UARTDisable();
				//	STOP_RX(Uart);
				STOP_TX(drv_info->hw_base);
				RCCPeripheralDisable(drv_info->info.peripheral_indx);

				PIO_Free_List(drv_info->uart_pins);
			}
			break;

		case DCR_CANCEL:
			dcr_cancel_handle(drv_info, drv_data, hnd);
			break;

        case DCR_CLOCK:
        	break;
	}
}

void USART_DSR(USART_DRIVER_INFO* drv_info, HANDLE hnd)
{
	unsigned char *ptr;
	signed int size;
	USART_DRIVER_DATA* drv_data = drv_info->drv_data;
	USART_TypeDef* uart = drv_info->hw_base;

	hnd->next = NULL;
	if (hnd->cmd & FLAG_READ)
    {
      	hnd->res = RES_BUSY; // no data transfered
      	if(drv_data->hnd_rcv)
      	{
			hnd->list_add(drv_data->hnd_rcv);
			return;
      	}

      	if(drv_data->usart_err)
      	{
      		hnd->error = drv_data->usart_err;
      		drv_data->usart_err = 0;
      		svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
      		return;
      	}

      	if (hnd->len == 0)
      	{
      		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
      		return;
      	}

		// STOP_RX(uart);
		//try to read from buffer
      	ptr = drv_data->rx_wrptr;
      	if( ptr != drv_data->rx_ptr )
      	{
      		if(ptr < drv_data->rx_ptr)
      		{
      			size =min(hnd->len, (&drv_data->rx_buf[USART_DRV_RX_BUF_SIZE]) - drv_data->rx_ptr);
      			memcpy(hnd->dst.as_byteptr, drv_data->rx_ptr, size);
      			hnd->len -= size;
      			hnd->dst.as_int += size;
      			drv_data->rx_ptr += size;
      			if(drv_data->rx_ptr == &drv_data->rx_buf[USART_DRV_RX_BUF_SIZE])
      				drv_data->rx_ptr = drv_data->rx_buf;
      		}

          	if( hnd->len && (ptr!= drv_data->rx_ptr) )
      		{
      			size =min(hnd->len, ptr - drv_data->rx_ptr);
      			memcpy(hnd->dst.as_byteptr, drv_data->rx_ptr, size);
      			hnd->len -= size;
      			hnd->dst.as_int += size;
      			drv_data->rx_ptr += size;
      		}

      		RESUME_RX(uart, drv_data);
      		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
      		return;
      	}

      	//receive directly
      	drv_data->hnd_rcv = hnd;
      	START_RX_HND(uart, drv_data, hnd);
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
			START_TX_HND(uart, hnd);
		}
		else
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		return;
    }

	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}

void USART_ISR(USART_DRIVER_INFO* drv_info)
{
	USART_TypeDef* uart = drv_info->hw_base;
	USART_DRIVER_DATA* drv_data = drv_info->drv_data;
	HANDLE hnd;
	unsigned int status;



	status = get_usart_sr(uart);
	status &= get_usart_imr(uart);

	if( status & USART_STATUS_TC )
	{
		if((hnd=drv_data->hnd_snd))
		{
			drv_data->hnd_snd = hnd->next;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
		}
		uart->USART_CR1 = (uart->USART_CR1 &~USART_CR1_TCIE) | USART_CR1_TXEIE;
	}
	if( status & USART_STATUS_TXE )
	{
		if((hnd=drv_data->hnd_snd))
		{
			if(hnd->len)
			{
				hnd->len--;
//				TRACE_BUF(hnd->src.as_charptr, 1, TC_TXT_MAGENTA);
				get_usart_tdr(uart) = *hnd->src.as_charptr++;
			} else
			{
				if(hnd->cmd & FLAG_LOCK)
				{
					uart->USART_CR1 = (uart->USART_CR1 &~USART_CR1_TXEIE) | USART_CR1_TCIE;
				} else
				{
					drv_data->hnd_snd = hnd->next;
					usr_HND_SET_STATUS(hnd, RES_SIG_OK);
				}
			}

		}
		if(!hnd)
		{
			STOP_TX(uart);
		}
	}

	//check the receiver
	if (status & USART_STATUS_ORE)
	{
		TRACELN("over");
		get_usart_rdr(uart); //clear overrun flag
    	if( (hnd=drv_data->hnd_rcv) )
    	{
			STOP_RX_HND(uart, drv_data, hnd, RES_SIG_ERROR);
    	} else
    	{
    		//disable ovr interrupt
    		drv_data->usart_err = status;
    	}
	}
	if (status & USART_STATUS_RXNE)
    {
		*drv_data->rx_wrptr = get_usart_rdr(uart);
//		TRACE_BUF(drv_data->rx_wrptr, 1, TC_TXT_CYAN);
		drv_data->rx_wrptr++;
		if(!--drv_data->rx_remaining )
		{
	    	if( (hnd=drv_data->hnd_rcv) )
				STOP_RX_HND(uart, drv_data, hnd, RES_SIG_OK);
	      	else
	      		RESUME_RX(uart,drv_data);

		}
	} else
	{
		if (status & USART_STATUS_IDLE)
		{
			get_usart_rdr(uart); //clear idle flag
	    	if( (hnd=drv_data->hnd_rcv) )
	    	{
	    		if(drv_data->rx_remaining != hnd->len)
	    		{
	    			TRACELN("idle");
	    			STOP_RX_HND(uart, drv_data, hnd, RES_SIG_OK);
	    		}
	    	}
		}
	}

}

/*
 * 	DMA Goals:
 * 		1) autonomous mode (no need to serve fast interrupts)
 * 		2) less cpu usage
 *
 * 	Memcopy variant:
 * 		goal 1 -> OK
 * 		goal 2  ok?
 *
 * 		problems
 * 		- how to signal the client??? (e.g. client waits for 3 bytes, DMA started for 256)
 *
 * 	DMA switch
 * 		goal 1 -> no way...
 * 		goal 2 -> OK
 *
 *
 *
 *
 *	1) copy as much as avail...
 *
 *	2) if remain < 8
 *		enable rx int (continue with memcpy(..1) ISR for each byte)
 *
 *	3) if remain >= 8
 *		stop DMA
 *		DMA1 = hnd.dst
 *		DMA2 = buf
 *		dma_len >= 8...
 *			usr_Send_Signal..
 *			update dma_len...
 *
 *
 *		8888888888888
 *		8888888888888
 *
 *		CCCCCCCCCCCCC
 *		CCCCCCCCCCCCC
 *		CCCCCCCCCCCCC
 *		CCCCCCCCCCCCC
 *
 *
 *
 *
 *
 *
 *
 *
 */

