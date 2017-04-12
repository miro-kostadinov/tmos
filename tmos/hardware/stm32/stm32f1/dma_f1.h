/**************************************************************************//**
 * @ingroup	hardware_stm32f1_dma
 * @file	hardware/stm32/stm32f1/dma_f1.h
 * @brief  	STM32F1 DMA
 * @date    7. November 2012
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f1_dma  DMA Controller
 * @ingroup	 hardware_stm32f1
 * Source and definitions for STM32F1 DMA Controller
 * @{
 *
 ******************************************************************************/

#ifndef DMA_F1_H_
#define DMA_F1_H_

#include <tmos_types.h>
#include <mcu_inc.h>

/***************************************************************************//**
 *  DMA Channel
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DMA_CCRx;		//!< (dmach Offset: 0x00) DMA channel x configuration register
  __IO uint32_t DMA_CNDTRx;		//!< (dmach Offset: 0x04) DMA channel x number of data register
  __IO uint32_t DMA_CPARx;		//!< (dmach Offset: 0x08) DMA channel x peripheral address register
  __IO uint32_t DMA_CMARx;		//!< (dmach Offset: 0x0C) DMA channel x memory address register
  __IO uint32_t reserved[836];	//!< (dmach Offset: 0x10) reserved
} DMA_Channel_TypeDef;

/***************************************************************************//**
 *  DMA Controller
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DMA_ISR;		  //!< (dma Offset: 0x00) DMA interrupt status register
  __IO uint32_t DMA_IFCR;		  //!< (dma Offset: 0x04) DMA interrupt flag clear register
  DMA_Channel_TypeDef DMA_Chx[7]; //!< (dma Offset: 0x08) DMA channels (up to 7)
} DMA_TypeDef;

/***************************************************************************//**
 * @relates DMA_Channel_TypeDef
 * @{
 ******************************************************************************/

/** @name DMA_CCRx:		(dmach Offset: 0x00) DMA channel x configuration register */
/** @{ */
#define DMA_CCRx_EN                 0x0001 //!< Channel enable
#define DMA_CCRx_TCIE               0x0002 //!< Transfer complete interrupt enable
#define DMA_CCRx_HTIE               0x0004 //!< Half Transfer interrupt enable
#define DMA_CCRx_TEIE               0x0008 //!< Transfer error interrupt enable
#define DMA_CCRx_DIR                0x0010 //!< Data transfer direction
#define DMA_CCRx_CIRC               0x0020 //!< Circular mode
#define DMA_CCRx_PINC               0x0040 //!< Peripheral increment mode
#define DMA_CCRx_MINC               0x0080 //!< Memory increment mode

#define DMA_CCRx_PSIZE              0x0300 //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CCRx_PSIZE_8bit         0x0000 //!< 8-bits
#define DMA_CCRx_PSIZE_16bit        0x0100 //!< 16-bits
#define DMA_CCRx_PSIZE_32bit        0x0200 //!< 32-bits

#define DMA_CCRx_MSIZE              0x0C00 //!< MSIZE[1:0] bits (Memory size)
#define DMA_CCRx_MSIZE_8bit         0x0000 //!< 8-bits
#define DMA_CCRx_MSIZE_16bit        0x0400 //!< 16-bits
#define DMA_CCRx_MSIZE_32bit        0x0800 //!< 32-bits

#define DMA_CCRx_PL                 0x3000 //!< PL[1:0] bits(Channel Priority level)
#define DMA_CCRx_PL_low             0x0000 //!< Low
#define DMA_CCRx_PL_medium          0x1000 //!< Medium
#define DMA_CCRx_PL_high            0x2000 //!< High
#define DMA_CCRx_PL_veryhigh        0x3000 //!< Very high

#define DMA_CCRx_MEM2MEM            0x4000 //!< Memory to memory mode
/** @} */

/** @name DMA_CNDTRx:	(dmach Offset: 0x04) DMA channel x number of data register */
/** @{ */
#define DMA_CNDTRx_NDT              0xFFFF //!< Number of data to Transfer
/** @} */

/** @name DMA_CPARx:	(dmach Offset: 0x08) DMA channel x peripheral address register */
/** @{ */
#define DMA_CPARx_PA                0xFFFFFFFF //!< Peripheral Address
/** @} */

/** @name DMA_CMARx:	(dmach Offset: 0x0C) DMA channel x memory address register */
/** @{ */
#define DMA_CMAR1_MA                0xFFFFFFFF //!< Memory Address
/** @} */

/** @} */ // @relates DMA_Channel_TypeDef


/***************************************************************************//**
 * @relates DMA_TypeDef
 * @{
 ******************************************************************************/

/** @name DMA_ISR:		(dma Offset: 0x00) DMA interrupt status register	  */
/** @{ */
#define DMA_ISR_GIFx                0x1 //!< Channel x Global interrupt flag
#define DMA_ISR_TCIFx               0x2 //!< Channel x Transfer Complete flag
#define DMA_ISR_HTIFx               0x4 //!< Channel x Half Transfer flag
#define DMA_ISR_TEIFx               0x8 //!< Channel x Transfer Error flag

#define DMA_ISR_Msk(x)  	(0xF<<((x)*4))      //!< mask
#define DMA_ISR_Get(x,y)  (((y)>>((x)*4))& 0xF) //!< get macro
#define DMA_ISR_Set(x,y)      ((y)<<((x)*4)) 	//!< set macro
/** @} */

/** @name DMA_IFCR:		(dma Offset: 0x04) DMA interrupt flag clear register  */
/** @{ */
#define DMA_IFCR_CGIFx              0x1 //!< Channel x Global interrupt clear
#define DMA_IFCR_CTCIFx             0x2 //!< Channel x Transfer Complete clear
#define DMA_IFCR_CHTIFx             0x4 //!< Channel x Half Transfer clear
#define DMA_IFCR_CTEIFx             0x8 //!< Channel x Transfer Error clear

#define DMA_IFCR_Msk(x)  	(0xF<<((x)*4))      //!< mask
#define DMA_IFCR_Get(x,y)  (((y)>>((x)*4))& 0xF) //!< get macro
#define DMA_IFCR_Set(x,y)      ((y)<<((x)*4)) 	//!< set macro
/** @} */

/** @} */ // @relates DMA_TypeDef


#define STM32_DMA_ERRORS (DMA_ISR_TEIFx )	//!< errors for F1

#define STM32_DMA_COMPLETE (DMA_ISR_TCIFx)	//!< complete for F1
#define STM32_DMA_HALF (DMA_IFCR_CHTIFx)	//!< half transfer for F1

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

	indx <<= 2;
	status = dmac->DMA_ISR & (0xf << indx);
	dmac->DMA_IFCR = status;

	return status >> indx;
}

static inline uint32_t stm32_dma_ndtr(DMA_TypeDef* dmac, uint32_t indx)
{
	return dmac->DMA_Chx[indx].DMA_CNDTRx;
}

static inline uint32_t stm32_dma_is_en(DMA_TypeDef* dmac, uint32_t indx)
{
	return dmac->DMA_Chx[indx].DMA_CCRx & DMA_CCRx_EN;
}

void stm32_en_ints(DMA_TypeDef* dmac, uint32_t indx, DMA_DRIVER_MODE* mode);
void stm32_dis_ints(DMA_TypeDef* dmac, uint32_t indx);





#endif /* DMA_F1_H_ */

/** @} */
