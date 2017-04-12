/**************************************************************************//**
 * @ingroup	hardware_stm32f0_dma
 * @file	hardware/stm32/stm32f0/dma_f0.h
 * @brief  	STM32F0 DMA
 * @date    28. October 2014
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f0_dma  DMA Controller
 * @ingroup	 hardware_stm32f0
 * Source and definitions for STM32F0 DMA Controller
 * @{
 *
 ******************************************************************************/

#ifndef DMA_F0_H_
#define DMA_F0_H_

#include <tmos_types.h>
#include <mcu_inc.h>

/***************************************************************************//**
 *  DMA Channel
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DMA_CCRx;     //!< (dma Offset: 0x00) DMA channel x configuration register
  __IO uint32_t DMA_CNDTRx;   //!< (dma Offset: 0x04) DMA channel x number of data register
  __IO uint32_t DMA_CPARx;    //!< (dma Offset: 0x08) DMA channel x peripheral address register
  __IO uint32_t DMA_CMARx;    //!< (dma Offset: 0x0C) DMA channel x memory address register
  __IO uint32_t reserved[4];  //!< (dma Offset: 0x10)
} DMA_Channel_TypeDef;

/***************************************************************************//**
 *  DMA Controller
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DMA_ISR;	  		//!< (dma Offset: 0x00) DMA interrupt status register
  __IO uint32_t DMA_IFCR;	  		//!< (dma Offset: 0x04) DMA interrupt flag clear register
  DMA_Channel_TypeDef DMA_Chx[5]; 	//!< (dma Offset: 0x08) DMA channel x
} DMA_TypeDef;

/*******************************************************************************
 * @relates DMA_Channel_TypeDef
 * @{
 ******************************************************************************/

/** @name DMA_CCRx:		(dma Offset: 0x00) DMA channel x configuration register */
/** @{ */
#define DMA_CCRx_MEM2MEM            0x00004000 //!< Memory to memory mode
#define DMA_CCRx_PL                 0x00003000 //!< PL[1:0] bits(Channel Priority level)
#define DMA_CCRx_PL_low             0x00000000 //!<  Low
#define DMA_CCRx_PL_medium          0x00001000 //!<  Medium
#define DMA_CCRx_PL_high            0x00002000 //!<  High
#define DMA_CCRx_PL_veryhigh        0x00003000 //!<  Very high
#define DMA_CCRx_MSIZE              0x00000C00 //!< MSIZE[1:0] bits (Memory data size)
#define DMA_CCRx_MSIZE_8bit         0x00000000 //!<  8-bits
#define DMA_CCRx_MSIZE_16bit        0x00000400 //!<  16-bits
#define DMA_CCRx_MSIZE_32bit        0x00000800 //!<  32-bits
#define DMA_CCRx_PSIZE              0x00000300 //!< PSIZE[1:0] bits (Peripheral data size)
#define DMA_CCRx_PSIZE_8bit         0x00000000 //!<  8-bits
#define DMA_CCRx_PSIZE_16bit        0x00000100 //!<  16-bits
#define DMA_CCRx_PSIZE_32bit        0x00000200 //!<  32-bits
#define DMA_CCRx_MINC               0x00000080 //!< Memory increment mode
#define DMA_CCRx_PINC               0x00000040 //!< Peripheral increment mode
#define DMA_CCRx_CIRC               0x00000020 //!< Circular mode
#define DMA_CCRx_DIR                0x00000010 //!< Data transfer direction
#define DMA_CCRx_DIR_RP             0x00000000 //!<  Read from peripheral
#define DMA_CCRx_DIR_RM             0x00000010 //!<  Read from memory
#define DMA_CCRx_TEIE               0x00000008 //!< Transfer error interrupt enable
#define DMA_CCRx_HTIE               0x00000004 //!< Half transfer interrupt enable
#define DMA_CCRx_TCIE               0x00000002 //!< Transfer complete interrupt enable
#define DMA_CCRx_EN                 0x00000001 //!< Channel enable
/** @} */

/** @name DMA_CNDTRx:	(dma Offset: 0x04) DMA channel x number of data register */
/** @{ */
#define DMA_CNDTRx_NDT              0x0000FFFF //!< Number of data to Transfer
/** @} */

/** @name DMA_CPARx:	(dma Offset: 0x08) DMA channel x peripheral address register */
/** @{ */
#define DMA_CPARx_PA               	0xFFFFFFFF //!< Peripheral Address
/** @} */

/** @name DMA_CMARx:	(dmas Offset: 0x0C) DMA channel x memory address register */
/** @{ */
#define DMA_CMARx_MA                0xFFFFFFFF //!< Memory address
/** @} */

/** @} */ // @relates DMA_Channel_TypeDef


/*******************************************************************************
 * @relates DMA_TypeDef
 * @{
 ******************************************************************************/
/** @name DMA_ISR:		(dma Offset: 0x00) DMA interrupt status register  	  */
/** @{ */
#define DMA_ISR_TEIFx               0x08       //!< Channel x transfer error flag
#define DMA_ISR_HTIFx               0x04       //!< Channel x half transfer flag
#define DMA_ISR_TCIFx               0x02       //!< Channel x transfer complete flag
#define DMA_ISR_GIFx                0x01       //!< Channel x global interrupt flag
/** @} */

/** @name DMA_IFCR:		(dma Offset: 0x04) DMA interrupt flag clear register  */
/** @{ */
#define DMA_IFCR_CTEIFx             0x08       //!< Channel x transfer error clear
#define DMA_IFCR_CHTIFx             0x04       //!< Channel x half transfer clear
#define DMA_IFCR_CTCIFx             0x02       //!< Channel x transfer complete clear
#define DMA_IFCR_CGIFx	            0x01       //!< Channel x global interrupt clear
/** @} */

/** @} */ // @relates DMA_TypeDef

#define STM32_DMA_ERRORS (DMA_ISR_TEIFx)	//!< errors for F0

#define STM32_DMA_COMPLETE (DMA_ISR_TCIFx)	//!< complete for F0
#define STM32_DMA_HALF (DMA_ISR_HTIFx)		//!< half transfer for F0

/** DMA Driver mode structure **/
struct DMA_DRIVER_MODE
{
	DRIVER_INDEX dma_index;	//!< DMA driver index (DMAx_Streamx_IRQn or DMAx_Channelx_IRQn)

	uint32_t dma_ch_cr;		//!< DMA channel/stream config register value
};

void stm32_dma_start(DMA_TypeDef* dmac, uint32_t indx, HANDLE hnd);
void stm32_dma_stop(DMA_TypeDef* dmac, uint32_t indx);
void stm32_dma_ch_cfg(DMA_TypeDef* dmac, uint32_t indx, DMA_DRIVER_MODE* mode);

static inline uint32_t stm32_get_ints(DMA_TypeDef* dmac, uint32_t indx)
{
	uint32_t status;

	status = (dmac->DMA_ISR >> (indx << 2)) & 0xf;
	dmac->DMA_IFCR = status;
	return status;
}

static inline uint32_t stm32_dma_ndtr(DMA_TypeDef* dmac, uint32_t indx)
{
	return dmac->DMA_Chx[indx].DMA_CNDTRx;
}

static inline uint32_t stm32_dma_is_en(DMA_TypeDef* dmac, uint32_t indx)
{
	return dmac->DMA_Chx[indx].DMA_CCRx & DMA_CCRx_EN;
}

void stm32_dis_ints(DMA_TypeDef* dmac, uint32_t indx);
void stm32_en_ints(DMA_TypeDef* dmac, uint32_t indx, DMA_DRIVER_MODE* mode);

#endif /* DMA_F0_H_ */

/** @} */
