/**************************************************************************//**
 * @file     uart_drv.cpp
 * @ingroup  DRV_AT91_UART
 * @brief    UART driver implementation
 * @version  V3.00
 * @date     02. December 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <uart_drv.h>

//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------


void UART_OFF(Uart* pUart)
{
    //* Disable all interrupts
    pUart->UART_IDR = UART_IDR_RXRDY | UART_IDR_TXRDY | UART_IDR_ENDRX
    		| UART_IDR_ENDTX | UART_IDR_OVRE | UART_IDR_FRAME | UART_IDR_PARE
    		| UART_IDR_TXEMPTY | UART_IDR_TXBUFE | UART_IDR_RXBUFF;

    //* Reset the baud rate divisor register
    pUart->UART_BRGR = 0 ;

    //* Reset the USART mode
    pUart->UART_MR = 0  ;

    //* Abort the Peripheral Data Transfers
	pUart->UART_PTCR =UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	pUart->UART_RNCR = 0;
	pUart->UART_TNCR = 0;
	pUart->UART_RCR = 0;
	pUart->UART_TCR = 0;


    //* Disable receiver and transmitter and stop any activity immediately
    pUart->UART_CR = UART_CR_TXDIS | UART_CR_RXDIS | UART_CR_RSTTX | UART_CR_RSTRX;

	//RTS =1 (not ready)
    PIO_CfgPeriph(DRXD_PIN | DTXD_PIN);

}

void UART_CFG(Uart* pUart, DRV_UART_MODE pMode)
{
    pUart->UART_CR = UART_CR_TXEN | UART_CR_RXEN;

  	//* Define the baud rate divisor register
	pUart->UART_BRGR = SYSDRV_GetDiv(pMode->baudrate);

    //* Enable PDC
	pUart->UART_PTCR = UART_PTCR_RXTEN ;
	pUart->UART_PTCR = UART_PTCR_TXTEN;

    //* Define the USART mode
    pUart->UART_MR = pMode->mode;

    pUart->UART_IER = UART_IER_ENDRX ;

}

#define STOP_RX(pUart) 		pUart->UART_PTCR = UART_PTCR_RXTDIS
#define STOP_TX(pUart) 		pUart->UART_PTCR = UART_PTCR_TXTDIS
#define RESUME_RX(pUart) 	pUart->UART_PTCR = UART_PTCR_RXTEN

static void START_RX_BUF(Uart*	pUart, UART_DRIVER_DATA drv_data)
{
	pUart->UART_RPR = (unsigned int)drv_data->rx_buf;
	drv_data->rx_ptr = drv_data->rx_buf;
	pUart->UART_RCR = UART_RX_BUF_SIZE;
	pUart->UART_PTCR = UART_PTCR_RXTEN ;
}

static void START_RX_HND(Uart*	pUart, UART_DRIVER_DATA drv_data, HANDLE hnd)
{
	pUart->UART_RPR = hnd->dst.as_int;
	pUart->UART_RCR = hnd->len;
	pUart->UART_PTCR = UART_PTCR_RXTEN ;
    drv_data->rtout = ((DRV_UART_MODE)(hnd->mode.as_voidptr))->rtout;
}

static void STOP_RX_HND(Uart*	pUart, UART_DRIVER_DATA drv_data, HANDLE hnd)
{
	STOP_RX(pUart);
	drv_data->hnd_rcv = hnd->next;
	hnd->len =pUart->UART_RCR;
	hnd->dst.as_int = pUart->UART_RPR;
	usr_HND_SET_STATUS(hnd, RES_SIG_OK);
  	if( (hnd=drv_data->hnd_rcv) )
      	START_RX_HND(pUart, drv_data, hnd);
  	else
		START_RX_BUF(pUart, drv_data);
}

static void START_TX_HND(Uart*	pUart, HANDLE hnd)
{
	pUart->UART_TPR = hnd->src.as_int;
	pUart->UART_TCR = hnd->len;
	pUart->UART_PTCR = UART_PTCR_RXTEN ;
    pUart->UART_IER = UART_IER_TXBUFE;
}

//*----------------------------------------------------------------------------




//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void UART_DCR(UART_INFO drv_info, unsigned int reason, HANDLE param)
{
    UART_DRIVER_DATA drv_data = drv_info->drv_data;
    unsigned int temp;


	switch(reason)
    {

        case DCR_RESET:
          	UART_OFF((Uart*)param);
            break;

        case DCR_OPEN:
			{
				DRV_UART_MODE pMode = (DRV_UART_MODE)(param->mode.as_voidptr);

				if(pMode)
				{
					if(drv_data->cnt)
					{
						if( (pMode->mode != drv_data->mode) ||
								(pMode->baudrate != drv_data->baudrate))
							return;

					} else
					{
						drv_data->mode = pMode->mode;
						drv_data->baudrate =pMode->baudrate;
						UART_CFG(drv_info->hw_base, pMode);
						START_RX_BUF(drv_info->hw_base, drv_data);
					}
					drv_data->cnt++;
					param->res = RES_OK;
				}
				break;
			}

    	case DCR_CLOSE:
        	if(drv_data->cnt)
				if(!--drv_data->cnt)
					UART_OFF(drv_info->hw_base);
    		break;

    	case DCR_CANCEL:
    	{
    		if (param->cmd & FLAG_READ)
    	    {
    			if(param == drv_data->hnd_rcv)
    			{
    				STOP_RX(drv_info->hw_base);
    				temp = drv_info->hw_base->UART_RCR;
    	      		if(param->len > temp)
    	      		{
    	      			param->len = temp;
    	      			param->dst.as_int = drv_info->hw_base->UART_RPR;
    	      			temp = RES_OK;
    	      		} else
    	      			temp = RES_SIG_IDLE;

    	      		drv_data->hnd_rcv = param->next;
    		      	if( drv_data->hnd_rcv )
    		          	START_RX_HND(drv_info->hw_base, drv_data, drv_data->hnd_rcv);
    		      	else
    					START_RX_BUF(drv_info->hw_base, drv_data);

    	      		svc_HND_SET_STATUS(param, temp);
    			} else
    				param->svc_list_cancel(drv_data->hnd_rcv);
    		} else
    		{
    			if(param == drv_data->hnd_snd)
    			{
    				STOP_TX(drv_info->hw_base);
    				temp = drv_info->hw_base->UART_TCR;
    	      		if(param->len > temp)
    	      		{
    	      			param->len = temp;
    	      			param->src.as_int = drv_info->hw_base->UART_TPR;
    	      			temp = RES_OK;
    	      		} else
    	      			temp = RES_SIG_IDLE;

    	      		drv_data->hnd_snd = param->next;
    		      	if( drv_data->hnd_snd )
    		          	START_TX_HND(drv_info->hw_base, drv_data->hnd_snd);

    	      		svc_HND_SET_STATUS(param, temp);
    			} else
    				param->svc_list_cancel(drv_data->hnd_snd);

    		}
    		break;
    	}

    	case DCR_CLOCK:
        	if(drv_data->cnt)
        		drv_info->hw_base->UART_BRGR = SYSDRV_GetDiv(drv_data->baudrate);
    		break;
    }
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void UART_DSR(UART_INFO drv_info, HANDLE hnd)
{
    UART_DRIVER_DATA drv_data = drv_info->drv_data;
	unsigned char *ptr;
	signed int size;

	if(hnd->len)
	{
		hnd->next = NULL;
		if (hnd->cmd & FLAG_READ)
	    {
	      	if(drv_data->hnd_rcv)
	      	{
	      		hnd->list_add(drv_data->hnd_rcv);
				return;
	      	}

			STOP_RX(drv_info->hw_base);

			//try to read from buffer
	      	ptr = (unsigned char*) (drv_info->hw_base->UART_RPR);
	      	if (ptr != drv_data->rx_ptr)
	      	{
	      		if (ptr < drv_data->rx_ptr)
	      		{
	      			size = min(hnd->len, drv_info->buf_size -
	      					(drv_data->rx_ptr - drv_data->rx_buf));
	      			memcpy(hnd->dst.as_byteptr, drv_data->rx_ptr, size);
	      			hnd->len -= size;
	      			hnd->dst.as_int += size;
	      			drv_data->rx_ptr += size;
	      			if(drv_data->rx_ptr == &drv_data->rx_buf[drv_info->buf_size])
	      				drv_data->rx_ptr = drv_data->rx_buf;
	      		}

	          	if (hnd->len && (ptr > drv_data->rx_ptr))
	      		{
	      			size =min(hnd->len, ptr - drv_data->rx_ptr);
	      			memcpy(hnd->dst.as_byteptr, drv_data->rx_ptr, size);
	      			hnd->len -= size;
	      			hnd->dst.as_int += size;
	      			drv_data->rx_ptr += size;
	      		}
	      	}
	      	if (hnd->len )
	      	{
		      	//receive directly
		      	drv_data->hnd_rcv = hnd;
		      	START_RX_HND(drv_info->hw_base, drv_data, hnd);
	      	} else
	      	{
	      		RESUME_RX(drv_info->hw_base);
	      		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	      	}

	    } else
	    {
	    	if (hnd->cmd & FLAG_WRITE)
	        {
	          	if(drv_data->hnd_snd)
	          	{
	          		hnd->list_add(drv_data->hnd_snd);
	          	} else
	          	{
		          	drv_data->hnd_snd = hnd;
		          	START_TX_HND(drv_info->hw_base, hnd);
	          	}
	        } else
	        {
	    		svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
	        }
	    }
	} else
	{
		svc_HND_SET_STATUS(hnd, RES_SIG_IDLE);
	}

}



//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void UART_ISR(UART_INFO drv_info )
{
    HANDLE hnd;
    unsigned int status;
    Uart* pUart;
	UART_DRIVER_DATA drv_data;

	drv_data = drv_info->drv_data;
	pUart = drv_info->hw_base;

	status = pUart->UART_SR;
	status &=  pUart->UART_IMR;

	// check the transmitter
	if(status & UART_SR_TXBUFE)
	{
		pUart->UART_IDR = UART_IDR_TXBUFE;
      	if( (hnd=drv_data->hnd_snd) )
      	{
      		drv_data->hnd_snd = hnd->next;
      		hnd->len = 0;
  			hnd->src.as_int = pUart->UART_TPR;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
	      	if( drv_data->hnd_snd )
	          	START_TX_HND(pUart, drv_data->hnd_snd);

      	}
	}

	//check the receiver
	if(status & UART_SR_ENDRX)
	{
      	if( (hnd=drv_data->hnd_rcv) )
      	{
			STOP_RX_HND(pUart, drv_data, hnd);

      	} else
			START_RX_BUF(pUart, drv_data);
	} else
	{
		// process tout
      	if( (hnd=drv_data->hnd_rcv) )
      		if(hnd->dst.as_int != pUart->UART_RPR)
      			if(drv_data->rtout)
          			if(!--drv_data->rtout)
          			{
        				STOP_RX_HND(pUart, drv_data, hnd);
          			}
	}


}


