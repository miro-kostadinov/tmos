/**************************************************************************//**
 * @file     usart_drv.cpp
 * @ingroup  DRV_AT91_USART
 * @brief    USART driver implementation
 * @version  V3.00
 * @date     17. December 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <usart_drv.h>
#include <fam_cpp.h>
#include <platform_drv.h>

//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------

/**
 * Turn off the USART
 * @param drv_info
 */
static void USART_OFF(USART_INFO drv_info)
{
	Usart* pUsart= drv_info->hw_base;

    //* Disable all interrupts
   	drv_isr_disable(&drv_info->info);
    drv_pmc_enable(&drv_info->info);
    pUsart->US_WPMR = US_WPMR_WPKEY(0x555341);

    pUsart->US_IDR = 0xFFFFFFFF;

    //* Reset the baud rate divisor register
    pUsart->US_BRGR = 0 ;

    //* Reset the USART mode
    pUsart->US_MR = 0  ;

    //* Abort the Peripheral Data Transfers
	pUsart->US_PTCR =US_PTCR_RXTDIS | US_PTCR_TXTDIS;
	pUsart->US_RNCR = 0;
	pUsart->US_TNCR = 0;
	pUsart->US_RCR = 0;
	pUsart->US_TCR = 0;


    //* Disable receiver and transmitter and stop any activity immediately
    pUsart->US_CR = US_CR_TXDIS | US_CR_RXDIS | US_CR_RSTTX | US_CR_RSTRX | US_CR_RSTSTA;

	//RTS =1 (not ready)
    PIO_CfgInput(&drv_info->pins);
   	drv_pmc_disable(&drv_info->info);
}

/** Turn On the USART
 *
 * @param drv_info
 * @param pMode
 */
void USART_CFG(USART_INFO drv_info, DRV_UART_MODE pMode)
{
	Usart* pUsart =drv_info->hw_base;

    drv_pmc_enable(&drv_info->info);
    pUsart->US_WPMR = US_WPMR_WPKEY(0x555341);

    //* Reset receiver and transmitter
    pUsart->US_CR = US_CR_RSTRX | US_CR_RSTTX | US_CR_RXDIS | US_CR_TXDIS;

  	//* Define the baud rate divisor register
	pUsart->US_BRGR = AT91_GetDiv(pMode->baudrate);

	//* Write the Timeguard Register
    pUsart->US_TTGR = 3 ;
    pUsart->US_RTOR = pMode->rtout;

    //* Define the USART mode
    pUsart->US_MR = pMode->mode;

    //* Clear Transmit and Receive Counters
    pUsart->US_TNCR = 0;
	pUsart->US_TCR = 0;
	pUsart->US_RNCR = 0;
	pUsart->US_RCR = 0;
	pUsart->US_PTCR = US_PTCR_RXTEN | US_PTCR_TXTEN;

   	// Enable Usart
    pUsart->US_CR = US_CR_TXEN | US_CR_RXEN | US_CR_STTTO;

    pUsart->US_IER = US_IER_ENDRX | US_IER_TIMEOUT;
   	drv_enable_isr(&drv_info->info);
    PIO_CfgPeriph(&drv_info->pins);
}

/** STOP the receiver */
#define STOP_RX(pUsart) 	pUsart->US_PTCR = US_PTCR_RXTDIS
/** STOP the transmitter */
#define STOP_TX(pUsart) 	pUsart->US_PTCR = US_PTCR_TXTDIS
/** Resume the receiving */
#define RESUME_RX(pUsart) 	pUsart->US_PTCR = US_PTCR_RXTEN

/** Start receiving to rx_buf
 *
 * @param pUsart
 * @param drv_data
 */
static void START_RX_BUF(Usart*	pUsart, UART_DRIVER_DATA drv_data)
{
	pUsart->US_RPR = (unsigned int)drv_data->rx_buf;
	drv_data->rx_ptr = drv_data->rx_buf;
	pUsart->US_RCR = drv_data->buf_size;
	pUsart->US_PTCR = US_PTCR_RXTEN ;
}

/** Start receiving to hnd
 *
 * @param pUsart
 * @param drv_data
 * @param hnd
 */
static void START_RX_HND(Usart*	pUsart, UART_DRIVER_DATA drv_data, HANDLE hnd)
{
	pUsart->US_RPR = hnd->dst.as_int;
	pUsart->US_RCR = hnd->len;
	pUsart->US_PTCR = US_PTCR_RXTEN ;
    drv_data->rtout = ((DRV_UART_MODE)(hnd->mode.as_voidptr))->rtout;
}

/** Stop receiving hnd
 *
 * @param pUsart
 * @param drv_data
 * @param hnd
 */
static void STOP_RX_HND(Usart*	pUsart, UART_DRIVER_DATA drv_data, HANDLE hnd)
{
	STOP_RX(pUsart);
	drv_data->hnd_rcv = hnd->next;
	hnd->len =pUsart->US_RCR;
	hnd->dst.as_int = pUsart->US_RPR;
	usr_HND_SET_STATUS(hnd, RES_SIG_OK);
  	if( (hnd=drv_data->hnd_rcv) )
      	START_RX_HND(pUsart, drv_data, hnd);
  	else
		START_RX_BUF(pUsart, drv_data);
}


/** Start transmit hnd
 *
 * @param pUsart
 * @param hnd
 */
static void START_TX_HND(Usart*	pUsart, HANDLE hnd)
{
	pUsart->US_TPR = hnd->src.as_int;
	pUsart->US_TCR = hnd->len;
	pUsart->US_PTCR = US_PTCR_RXTEN ;
    pUsart->US_IER = US_IER_TXBUFE;
}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
/** USART DCR
 *
 * @param drv_info
 * @param reason
 * @param param
 */
void USART_DCR(USART_INFO drv_info, unsigned int reason, HANDLE param)
{
    UART_DRIVER_DATA drv_data = drv_info->drv_data;
    unsigned int temp;

	switch(reason)
    {

        case DCR_RESET:
        	drv_data->buf_size = drv_info->buf_size;
          	USART_OFF(drv_info);
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
						USART_CFG(drv_info, pMode);
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
					USART_OFF(drv_info);
    		break;

    	case DCR_CANCEL:
    	{
    		if (param->cmd & FLAG_READ)
    	    {
    			if(param == drv_data->hnd_rcv)
    			{
    				STOP_RX(drv_info->hw_base);
    				temp = drv_info->hw_base->US_RCR;
    	      		if(param->len > temp)
    	      		{
    	      			param->len = temp;
    	      			param->dst.as_int = drv_info->hw_base->US_RPR;
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
    				temp = drv_info->hw_base->US_TCR;
    	      		if(param->len > temp)
    	      		{
    	      			param->len = temp;
    	      			param->src.as_int = drv_info->hw_base->US_TPR;
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
        		drv_info->hw_base->US_BRGR = AT91_GetDiv(drv_data->baudrate);
    		break;
    }
}

//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
/**
 * USART DSR
 * @param drv_info
 * @param hnd
 */
void USART_DSR(USART_INFO drv_info, HANDLE hnd)
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
	      	ptr = (unsigned char*) (drv_info->hw_base->US_RPR);
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
/**
 * USART ISR
 *
 * @param drv_info
 */
void USART_ISR(USART_INFO drv_info )
{
    HANDLE hnd;
    unsigned int status;
    Usart* pUsart;
	UART_DRIVER_DATA drv_data;

	drv_data = drv_info->drv_data;
	pUsart = drv_info->hw_base;

	status = pUsart->US_CSR;
	status &=  pUsart->US_IMR;

	// check the transmitter
	if(status & US_CSR_TXBUFE)
	{
		pUsart->US_IDR = US_IDR_TXBUFE;
      	if( (hnd=drv_data->hnd_snd) )
      	{
      		drv_data->hnd_snd = hnd->next;
      		hnd->len = 0;
  			hnd->src.as_int = pUsart->US_TPR;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
	      	if( drv_data->hnd_snd )
	          	START_TX_HND(pUsart, drv_data->hnd_snd);

      	}
	}

	//check for timeout
	if(status & US_CSR_TIMEOUT)
	{
		pUsart->US_CR = US_CR_STTTO;
      	if( (hnd=drv_data->hnd_rcv) )
      	{
			STOP_RX_HND(pUsart, drv_data, hnd);
      	}
	}

	//check the receiver
	if(status & US_CSR_ENDRX)
	{
      	if( (hnd=drv_data->hnd_rcv) )
      	{
    		pUsart->US_CR = US_CR_STTTO;
			STOP_RX_HND(pUsart, drv_data, hnd);

      	} else
			START_RX_BUF(pUsart, drv_data);
	} else
	{
		// process tout
      	if( (hnd=drv_data->hnd_rcv) )
      		if(hnd->dst.as_int != pUsart->US_RPR)
      			if(drv_data->rtout)
          			if(!--drv_data->rtout)
          			{
        				STOP_RX_HND(pUsart, drv_data, hnd);
          			}
	}
}
