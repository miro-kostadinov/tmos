/*
 * dma_drv.h
 *
 *  Created on: Nov 21, 2012
 *      Author: miro
 */

#ifndef DMA_DRV_H_
#define DMA_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>
#include <mcu_cpp.h>

enum dma_state_t : uint16_t
{
	DMA_ST_POWER_UP	= 0,
	DMA_ST_RESET
};

/** DMA Driver data structure - one instance for each DMA controller **/
struct DMA_DRIVER_DATA
{
	uint16_t dma_state;
	uint16_t cnt;				//!< Number of open handles from this DMA
};

/** DMA Channel data structure **/
struct DMA_CHANNEL_DATA
{
    HANDLE waiting;				//!< waiting to be processed
    HANDLE pending;				//!< currently processed
    DMA_DRIVER_MODE* last_mode;	//!< last DMA mode used by this channel/stream
	uint16_t cnt;				//!< Number of open handles from this channel
};


/** DMA Driver Info structure - one instance for each dma channel**/
struct DMA_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	DMA_TypeDef*		hw_base;		//!< DMA peripheral
	DMA_DRIVER_DATA* 	drv_data;		//!< driver data
	DMA_CHANNEL_DATA* 	ch_data;		//!< channel/stream data
	uint8_t				ch_indx;		//!< DMA channel/stream
};

uint32_t dma_drv_get_ndtr(DRIVER_INDEX drv_index);
uint32_t dma_drv_is_en(DRIVER_INDEX drv_index);

void DMA_DCR(DMA_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void DMA_DSR(DMA_DRIVER_INFO* drv_info, HANDLE hnd);
void DMA_ISR(DMA_DRIVER_INFO* drv_info);



#endif /* DMA_DRV_H_ */
