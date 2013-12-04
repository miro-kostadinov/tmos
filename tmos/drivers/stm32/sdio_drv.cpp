/*
 * sdio_drv.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: miro
 */

#include <tmos.h>
#include <sdio_drv.h>

static void ConfigureSDIO(SDIO_INFO drv_info)
{
	SDIO_TypeDef* hw_base = drv_info->hw_base;

	RCCPeripheralEnable(drv_info->info.peripheral_indx);
	RCCPeripheralReset(drv_info->info.peripheral_indx);

	PIO_Cfg_List(drv_info->sdio_pins);

	if(drv_info->sdio_pwr_en)
		PIO_Assert(drv_info->sdio_pwr_en);

	hw_base->SDIO_CLKCR  = SDIO_CLKCR_WIDBUS_1b | SDIO_CLKCR_CLKEN |
			SDIO_CLKCR_CLKDIV_Set(48000/400 -2);
	hw_base->SDIO_POWER = SDIO_POWER_PWRCTRL_on;

	hw_base->SDIO_MASK = SDIO_STA_DONE_FLAGS | SDIO_STA_ERROR_FLAGS
			| SDIO_STA_CCRCFAIL ;
	hw_base->SDIO_DTIMER = 0xFFFFFFFF;

	NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);
	NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);
}

#define IS_SD_HIGH_CAPACITY(x) 1// should return 1 for SDHC and SDXC cards

static RES_CODE SDIO_START_HND(SDIO_INFO drv_info, HANDLE hnd, SDIO_DRIVER_DATA *drv_data)
{
	SDIO_TypeDef* hw_base = drv_info->hw_base;
	RES_CODE res = RES_IDLE;

	if( (hnd->cmd & FLAG_READ) && hnd->len)
	{
		TRACE("[r %u]", hnd->len);
		//read block or multiple block command
		hw_base->SDIO_DLEN = hnd->len;
		hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_512b | SDIO_DCTRL_DTDIR
				| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;

		uint32_t adr = hnd->src.as_int;
		if(!(IS_SD_HIGH_CAPACITY(drv_data->card_type )))
				adr *= 256;
		hw_base->SDIO_ARG = adr;
		hw_base->SDIO_CMD = SD_CMD17_READ_SINGLE_BLOCK | SDIO_CMD_WAITRESP_short | SDIO_CMD_CPSMEN;
		//wait for cmdend (R1) or error
		drv_data->sdio_op = SDIO_OP_READ | SDIO_OP_R1;
	} else
	{
		if( (hnd->cmd & FLAG_WRITE) && hnd->len)
		{
			TRACE("[w %u]", hnd->len);
			//write block or multiple block command
			uint32_t adr = hnd->dst.as_int;
			if(!(IS_SD_HIGH_CAPACITY(drv_data->card_type )))
					adr *= 256;
			hw_base->SDIO_ARG = adr;
			hw_base->SDIO_CMD = SD_CMD24_WRITE_BLOCK | SDIO_CMD_WAITRESP_short | SDIO_CMD_CPSMEN;

			//wait for cmdend (R1) or error
			drv_data->sdio_op = SDIO_OP_WRITE | SDIO_OP_R1;
		} else
		{
			if( hnd->cmd & FLAG_COMMAND)
			{
				unsigned int cmd, cmd_indx;

				cmd = hnd->src.as_intptr[0];
				cmd_indx = cmd & 0x3F;
				TRACE("[c %u]", cmd_indx);
				if( (hnd->len > 4) && !(cmd & SDIO_CMD_WAITRESP_no1) )
				{
					// Commands like ACMD51 (read SCR), CMD6 (SWITCH func) ->R1 + read
					hw_base->SDIO_DLEN = hnd->len;
					switch(hnd->len)
					{
					case 8:
						hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_8b | SDIO_DCTRL_DTDIR
								| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;
						break;
					case 16:
						hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_16b | SDIO_DCTRL_DTDIR
								| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;
						break;
					case 32:
						hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_32b | SDIO_DCTRL_DTDIR
								| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;
						break;
					case 64:
						hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_64b | SDIO_DCTRL_DTDIR
								| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;
						break;
					case 128:
						hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_128b | SDIO_DCTRL_DTDIR
								| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;
						break;
					case 256:
						hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_256b | SDIO_DCTRL_DTDIR
								| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;
						break;
					default: //512
						hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_512b | SDIO_DCTRL_DTDIR
								| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;
						break;
					}
					drv_data->sdio_op = SDIO_OP_READ | SDIO_OP_R1;
				} else
				{
					// R3 response for ACMD41 and CMD58 (CMD51 and ACMD58 are reserved)
					if( (cmd_indx==41) || (cmd_indx==58))
						drv_data->sdio_op = SDIO_OP_CMD | SDIO_OP_R3;
					else
						drv_data->sdio_op = SDIO_OP_CMD;
				}
				hw_base->SDIO_ARG = hnd->src.as_intptr[1];
				hw_base->SDIO_CMD = cmd;
				//wait for something..
			} else
			{
				res = RES_SIG_ERROR;
				drv_data->sdio_op = SDIO_OP_IDLE;
			}

		}
	}
	return res;
}

static void SDIO_CMD_HND(SDIO_INFO drv_info, HANDLE hnd, SDIO_DRIVER_DATA *drv_data)
{
	SDIO_TypeDef* hw_base = drv_info->hw_base;

	if(drv_data->sdio_op & SDIO_OP_READ)
	{
#if USE_SDIO_DMA_DRIVER
		if(drv_info->rx_dma_mode.dma_index < INALID_DRV_INDX)
		{
			drv_data->rx_dma_hnd.drv_read_write(hnd->dst.as_voidptr, (void*)&hw_base->SDIO_FIFO, hnd->len/4);
		} else
#endif
		{
			//read without DMA
			while((hw_base->SDIO_STA & SDIO_STA_RXFIFOHF) && hnd->len > 31)
			{
				for(int i=0; i<8; i++)
					*hnd->dst.as_intptr++ = hw_base->SDIO_FIFO;
				hnd->len -= 32;
			}
			while(!(hw_base->SDIO_STA & SDIO_STA_RXFIFOE) && hnd->len > 3)
			{
				*hnd->dst.as_intptr++ = hw_base->SDIO_FIFO;
				hnd->len -= 4;
			}
			if(hnd->len <= 3)
			{
				usr_HND_SET_STATUS(hnd, RES_SIG_OK);
				drv_data->pending = NULL;
			}
		}
	} else
	{
		if(drv_data->sdio_op & SDIO_OP_WRITE)
		{
#if USE_SDIO_DMA_DRIVER
			if(drv_info->tx_dma_mode.dma_index < INALID_DRV_INDX)
			{
				drv_data->tx_dma_hnd.drv_read_write((void *)&drv_info->hw_base->SDIO_FIFO, hnd->src.as_voidptr, hnd->len/4);
			} else
#endif
			{
				//write without DMA
				while((hw_base->SDIO_STA & SDIO_STA_TXFIFOHE) && hnd->len > 31)
				{
					for(int i=0; i<8; i++)
						hw_base->SDIO_FIFO = *hnd->src.as_intptr++ ;
					hnd->len -= 32;
				}
				while(!(hw_base->SDIO_STA & SDIO_STA_TXFIFOF) && hnd->len > 3)
				{
					hw_base->SDIO_FIFO = *hnd->src.as_intptr++;
					hnd->len -= 4;
				}
				if(hnd->len <= 3)
				{
					usr_HND_SET_STATUS(hnd, RES_SIG_OK);
					drv_data->pending = NULL;
				}
			}
		} else
		{
			unsigned int len = hnd->len /4;

			if (len > 4)
				len = 4;
			while(len--)
			{
				hnd->dst.as_intptr[len] = hw_base->SDIO_RESPx[len];
			}
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
			drv_data->pending = NULL;
		}
	}
}
//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void SDIO_DCR(SDIO_INFO drv_info, unsigned int reason, HANDLE hnd)
{
	SDIO_DRIVER_DATA *drv_data;

	drv_data = drv_info->drv_data;
	switch(reason)
	{
		case DCR_RESET:
			RCCPeripheralReset(drv_info->info.peripheral_indx);
			RCCPeripheralDisable(drv_info->info.peripheral_indx); // ??? turn off
			if(drv_info->sdio_pwr_en)
				PIO_Cfg(drv_info->sdio_pwr_en);
			break;

		case DCR_OPEN:
		{
			SDIO_DRIVER_MODE *mode = (SDIO_DRIVER_MODE *)(hnd->mode.as_voidptr);
			if(mode)
			{
				if(drv_data->cnt)
				{
				}
				else
				{
					ConfigureSDIO(drv_info);
#if USE_SDIO_DMA_DRIVER
        			if(!drv_data->rx_dma_hnd.drv_open(
        					drv_info->rx_dma_mode.dma_index,
        					&drv_info->rx_dma_mode))
        				break;
        			if(!drv_data->tx_dma_hnd.drv_open(
        					drv_info->tx_dma_mode.dma_index,
        					&drv_info->tx_dma_mode))
        				break;
#endif
				}
				drv_data->cnt++;
				hnd->res = RES_OK;
			}
			break;
		}

		case DCR_CLOSE:
		{
			if(drv_data->cnt)
				drv_data->cnt--;
			if(!drv_data->cnt)
			{
#if USE_SDIO_DMA_DRIVER
				drv_data->rx_dma_hnd.close();
				drv_data->tx_dma_hnd.close();
#endif
				NVIC->NVIC_DisableIRQ(drv_info->info.drv_index);
				RCCPeripheralReset(drv_info->info.peripheral_indx);
				RCCPeripheralDisable(drv_info->info.peripheral_indx);
				if(drv_info->sdio_pwr_en)
					PIO_Deassert(drv_info->sdio_pwr_en);
			}
			break;
		}

		case DCR_CLOCK:
			//patch for low speed cards
			drv_info->hw_base->SDIO_CLKCR  = (drv_info->hw_base->SDIO_CLKCR &
					~SDIO_CLKCR_CLKDIV) | SDIO_CLKCR_CLKDIV_Set(48000/16000 -2);

			break;

		case DCR_SIGNAL:
			//signal rx/dma complete
			// do nothing, use data end irq instead
			break;

		case DCR_CANCEL:
		{
			hnd->svc_list_cancel(drv_data->waiting);
			break;
		}

	}
}

//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void SDIO_DSR(SDIO_INFO drv_info, HANDLE hnd)
{
	SDIO_DRIVER_DATA *drv_data = drv_info->drv_data;

	if(drv_data->pending || (drv_data->locker && hnd != drv_data->locker))
	{
    	//the driver is busy || locked from other client
    	hnd->list_add(drv_data->waiting);
	} else
	{
		if( hnd->cmd & FLAG_LOCK)
			drv_data->locker = hnd;
		else
			drv_data->locker = NULL;

		RES_CODE res;

		res = SDIO_START_HND(drv_info, hnd, drv_data);
		if(res & FLG_SIGNALED)
		{
			svc_HND_SET_STATUS(hnd, res);
		} else
		{
			drv_data->pending = hnd;
			hnd->next = NULL;
			hnd->res  = RES_BUSY;
		}
	}
}

//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void SDIO_ISR(SDIO_INFO drv_info)
{
	SDIO_DRIVER_DATA *drv_data = drv_info->drv_data;
	SDIO_TypeDef* hw_base = drv_info->hw_base;
	HANDLE hnd;
	unsigned int status;

	status = hw_base->SDIO_STA;
	status &= hw_base->SDIO_MASK;
	hw_base->SDIO_ICR = status;
	TRACE("{i %x}", status);
	if((hnd = drv_data->pending))
	{
		if( (status & SDIO_STA_CCRCFAIL) && (drv_data->sdio_op & SDIO_OP_R3))
		{
			// Response R3 -> ignore CRC
			status &= ~SDIO_STA_CCRCFAIL;
			status |= SDIO_STA_CMDREND;
		}
		if(status & SDIO_STA_ERROR_FLAGS)
		{
			// error -> done!
			hnd->error = status;
			usr_HND_SET_STATUS(hnd, RES_SIG_ERROR);
			drv_data->pending = NULL;
		} else
		{
			if(status & SDIO_STA_DONE_TR)
			{
				// done...
				usr_HND_SET_STATUS(hnd, RES_SIG_OK);
				drv_data->pending = NULL;
			} else
			{
				if(status & SDIO_STA_DONE_CMD)
				{
					//command end
					if((drv_data->sdio_op & SDIO_OP_R1) && (hw_base->SDIO_RESPx[0] & SDIO_RESP1_ERRORS))
					{
						// R1 response is bad...
						hnd->error = hw_base->SDIO_RESPx[0];
						usr_HND_SET_STATUS(hnd, FLG_SIGNALED | RES_INVALID_DATA);
						drv_data->pending = NULL;
					} else
					{
						if(drv_data->sdio_op & SDIO_OP_WRITE)
						{
							hw_base->SDIO_DLEN = hnd->len;
							hw_base->SDIO_DCTRL = SDIO_DCTRL_DBLOCKSIZE_512b
									| SDIO_DCTRL_DTEN | SDIO_DCTRL_DMAEN;
						}

						if(drv_data->sdio_op & SDIO_OP_CMD)
						{
							switch(hnd->src.as_intptr[0] & 0x3f)
							{
							case SD_CMD00_GO_IDLE_STATE:
								//reset bus width
								break;

							case SD_ACMD06_SET_BUS_WIDTH: // CMD6 has different length
								if(!(hw_base->SDIO_RESPx[0] & SDIO_RESP1_ERRORS))
								{
									unsigned int reg;

									//change bus width
									reg = hw_base->SDIO_CLKCR & ~SDIO_CLKCR_WIDBUS;
									if(hnd->src.as_intptr[1] == 2)
										reg |= SDIO_CLKCR_WIDBUS_4b;
									hw_base->SDIO_CLKCR = reg;
								}
								break;


							}
						}
						SDIO_CMD_HND(drv_info, hnd, drv_data);
					}

				} else
				{
					if( ((drv_data->sdio_op & SDIO_OP_READ) && (status & SDIO_STA_RX_FLAGS))
						|| ((drv_data->sdio_op & SDIO_OP_WRITE) && (status & SDIO_STA_TX_FLAGS)) )
					{
						SDIO_CMD_HND(drv_info, hnd, drv_data);
					}

				}
			}
		}
		if(!drv_data->pending)
		{
			// start waiting handle
			while(!drv_data->locker && (hnd=drv_data->waiting))
			{
				drv_data->waiting = hnd->next;
				if( hnd->cmd & FLAG_LOCK)
					drv_data->locker = hnd;

				RES_CODE res;

				res = SDIO_START_HND(drv_info, hnd, drv_data);
				if(res & FLG_SIGNALED)
				{
					usr_HND_SET_STATUS(hnd, res);
				} else
				{
					drv_data->pending = hnd;
					hnd->next = NULL;
					break;
				}
			}
		}
	}
}

