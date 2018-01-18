/**************************************************************************//**
 * @ingroup	hardware_stm32f2_dma
 * @file	hardware/stm32/stm32f2/dma_f2.h
 * @brief  	STM32F2 DMA
 * @date    7. November 2012
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f2_dma  DMA Controller
 * @ingroup	 hardware_stm32f2
 * Source and definitions for STM32F2 DMA Controller
 * @{
 *
 ******************************************************************************/

#ifndef DMA_F2_H_
#define DMA_F2_H_

#include <tmos_types.h>
#include <mcu_inc.h>

/***************************************************************************//**
 *  DMA Stream
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DMA_SxCR;     //!< (dmas Offset: 0x00) DMA stream x configuration register
  __IO uint32_t DMA_SxNDTR;   //!< (dmas Offset: 0x04) DMA stream x number of data register
  __IO uint32_t DMA_SxPAR;    //!< (dmas Offset: 0x08) DMA stream x peripheral address register
  __IO uint32_t DMA_SxM0AR;   //!< (dmas Offset: 0x0C) DMA stream x memory 0 address register
  __IO uint32_t DMA_SxM1AR;   //!< (dmas Offset: 0x10) DMA stream x memory 1 address register
  __IO uint32_t DMA_SxFCR;    //!< (dmas Offset: 0x14) DMA stream x FIFO control register
} DMA_Stream_TypeDef;

/***************************************************************************//**
 *  DMA Controller
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DMA_ISR[2];		 //!< (dma Offset: 0x00) DMA low/high DMA_LISR/DMA_LISR interrupt status register
  __IO uint32_t DMA_IFCR[2];	 //!< (dma Offset: 0x08) DMA low/high interrupt flag clear register
  DMA_Stream_TypeDef DMA_Chx[8]; //!< (dma Offset: 0x10) DMA channel x
} DMA_TypeDef;

/***************************************************************************//**
 * @relates DMA_Stream_TypeDef
 * @{
 ******************************************************************************/

/** @name DMA_SxCR:     (dmas Offset: 0x00) DMA stream x configuration register */
/** @{ */
#define DMA_SxCR_EN                 0x00000001 //!< Stream enable / flag stream ready when read low
#define DMA_SxCR_DMEIE              0x00000002 //!< Direct mode error interrupt enable
#define DMA_SxCR_TEIE               0x00000004 //!< Transfer error interrupt enable
#define DMA_SxCR_HTIE               0x00000008 //!< Half transfer interrupt enable
#define DMA_SxCR_TCIE               0x00000010 //!< Transfer complete interrupt enable
#define DMA_SxCR_PFCTRL             0x00000020 //!< Peripheral flow controller
#define DMA_SxCR_DIR                0x000000C0 //!< Data transfer direction
#define DMA_SxCR_DIR_P2M            0x00000000 //!<  Peripheral-to-memory
#define DMA_SxCR_DIR_M2P            0x00000040 //!<  Memory-to-peripheral
#define DMA_SxCR_DIR_M2M            0x00000080 //!<  Memory-to-memory
#define DMA_SxCR_CIRC               0x00000100 //!< Circular mode
#define DMA_SxCR_PINC               0x00000200 //!< Peripheral increment mode
#define DMA_SxCR_MINC               0x00000400 //!< Memory increment mode
#define DMA_SxCR_PSIZE              0x00001800 //!< PSIZE[1:0] bits (Peripheral data size)
#define DMA_SxCR_PSIZE_8bit         0x00000000 //!<  8-bits
#define DMA_SxCR_PSIZE_16bit        0x00000800 //!<  16-bits
#define DMA_SxCR_PSIZE_32bit        0x00001000 //!<  32-bits
#define DMA_SxCR_MSIZE              0x00006000 //!< MSIZE[1:0] bits (Memory data size)
#define DMA_SxCR_MSIZE_8bit         0x00000000 //!<  8-bits
#define DMA_SxCR_MSIZE_16bit        0x00002000 //!<  16-bits
#define DMA_SxCR_MSIZE_32bit        0x00004000 //!<  32-bits
#define DMA_SxCR_PINCOS             0x00008000 //!< Peripheral increment offset size
#define DMA_SxCR_PINCOS_psize       0x00000000 //!<  address calculation is linked to the PSIZE
#define DMA_SxCR_PINCOS_32bit       0x00008000 //!<  address calculation is fixed to 4 (32-bit alignment)
#define DMA_SxCR_PL                 0x00030000 //!< PL[1:0] bits(Channel Priority level)
#define DMA_SxCR_PL_low             0x00000000 //!<  Low
#define DMA_SxCR_PL_medium          0x00010000 //!<  Medium
#define DMA_SxCR_PL_high            0x00020000 //!<  High
#define DMA_SxCR_PL_veryhigh        0x00030000 //!<  Very high
#define DMA_SxCR_DBM                0x00040000 //!< Double buffer mode
#define DMA_SxCR_CT                 0x00080000 //!< Current target (only in double buffer mode)
#define DMA_SxCR_ACK                0x00100000 //!< ?
#define DMA_SxCR_PBURST             0x00600000 //!< Peripheral burst transfer configuration
#define DMA_SxCR_PBURST_burst0      0x00000000 //!<  single transfer
#define DMA_SxCR_PBURST_burst4      0x00200000 //!<  INCR4 (incremental burst of 4 beats)
#define DMA_SxCR_PBURST_burst8      0x00400000 //!<  INCR8 (incremental burst of 8 beats)
#define DMA_SxCR_PBURST_burst16     0x00600000 //!<  INCR16 (incremental burst of 16 beats)
#define DMA_SxCR_MBURST             0x01800000 //!< Memory burst transfer configuration
#define DMA_SxCR_MBURST_burst0      0x00000000 //!<  single transfer
#define DMA_SxCR_MBURST_burst4      0x00800000 //!<  INCR4 (incremental burst of 4 beats)
#define DMA_SxCR_MBURST_burst8      0x01000000 //!<  INCR8 (incremental burst of 8 beats)
#define DMA_SxCR_MBURST_burst16     0x01800000 //!<  INCR16 (incremental burst of 16 beats)
#define DMA_SxCR_CHSEL              0x0E000000 //!< CHSEL[2:0]: Channel selection
#define DMA_SxCR_CHSEL_0            0x00000000 //!<  channel 0 selected
#define DMA_SxCR_CHSEL_1            0x02000000 //!<  channel 1 selected
#define DMA_SxCR_CHSEL_2            0x04000000 //!<  channel 2 selected
#define DMA_SxCR_CHSEL_3            0x06000000 //!<  channel 3 selected
#define DMA_SxCR_CHSEL_4            0x08000000 //!<  channel 4 selected
#define DMA_SxCR_CHSEL_5            0x0A000000 //!<  channel 5 selected
#define DMA_SxCR_CHSEL_6            0x0C000000 //!<  channel 6 selected
#define DMA_SxCR_CHSEL_7            0x0E000000 //!<  channel 7 selected
#define DMA_SxCR_RESERVED_NOINTS    0x80000000 //!< DISABLE interrupts
/** @} */

/** @name DMA_SxNDTR:   (dmas Offset: 0x04) DMA stream x number of data register */
/** @{ */
#define DMA_SxNDTR_NDT              0x0000FFFF //!< Number of data to Transfer
/** @} */

/** @name DMA_SxPAR:    (dmas Offset: 0x08) DMA stream x peripheral address register */
/** @{ */
#define DMA_SxPAR_PAR              	0xFFFFFFFF //!< Peripheral Address
/** @} */

/** @name DMA_SxM0AR:   (dmas Offset: 0x0C) DMA stream x memory 0 address register */
/** @{ */
#define DMA_SxM0AR_M0A              0xFFFFFFFF //!< Memory 0 address
/** @} */

/** @name DMA_SxM1AR:   (dmas Offset: 0x10) DMA stream x memory 1 address register */
/** @{ */
#define DMA_SxM1AR_M1A              0xFFFFFFFF //!< Memory 1 address (used in case of Double buffer mode)
/** @} */

/** @name DMA_SxFCR:    (dmas Offset: 0x14) DMA stream x FIFO control register */
/** @{ */
#define DMA_SxFCR_FEIE              0x00000080 //!< FIFO error interrupt enable
#define DMA_SxFCR_FS                0x00000038 //!< FS[2:0]: FIFO status
#define DMA_SxFCR_FS_00_14          0x00000000 //!<  0 < fifo_level < 1/4
#define DMA_SxFCR_FS_14_12          0x00000008 //!<  1/4 <= fifo_level < 1/2
#define DMA_SxFCR_FS_12_34          0x00000010 //!<  1/2 <= fifo_level < 3/4
#define DMA_SxFCR_FS_34_44          0x00000018 //!<  3/4 <= fifo_level < full
#define DMA_SxFCR_FS_empty          0x00000020 //!<  FIFO is empty
#define DMA_SxFCR_FS_full           0x00000028 //!<  FIFO is full
#define DMA_SxFCR_DMDIS             0x00000004 //!< Direct mode disable
#define DMA_SxFCR_FTH               0x00000003 //!< FIFO threshold selection
#define DMA_SxFCR_FTH_14            0x00000000 //!<  1/4 full FIFO
#define DMA_SxFCR_FTH_12            0x00000001 //!<  1/2 full FIFO
#define DMA_SxFCR_FTH_34            0x00000002 //!<  3/4 full FIFO
#define DMA_SxFCR_FTH_full          0x00000003 //!   full FIFO
#define DMA_SxFCR_DMODE             0x00000000 //!< Direct mode
/** @} */

/** @} */ // @relates DMA_Stream_TypeDef

/***************************************************************************//**
 * @relates DMA_TypeDef
 * @{
 ******************************************************************************/

/** @name DMA_ISR[2]:	(dma Offset: 0x00) DMA low/high DMA_LISR/DMA_LISR interrupt status register */
/** @{ */
#define DMA_ISR_TCIFx               0x20       //!< Stream x transfer complete interrupt flag
#define DMA_ISR_HTIFx               0x10       //!< Stream x half transfer interrupt flag
#define DMA_ISR_TEIFx               0x08       //!< Stream x transfer error interrupt flag
#define DMA_ISR_DMEIFx              0x04       //!< Stream x direct mode error interrupt flag
#define DMA_ISR_FEIFx               0x01       //!< Stream x FIFO error interrupt flag
/** @} */

/** @name DMA_IFCR[2]:	(dma Offset: 0x08) DMA low/high interrupt flag clear register */
/** @{ */
#define DMA_IFCR_CTCIFx             0x20       //!< Stream x clear transfer complete interrupt flag
#define DMA_IFCR_CHTIFx             0x10       //!< Stream x clear half transfer interrupt flag
#define DMA_IFCR_CTEIFx             0x08       //!< Stream x clear transfer error interrupt flag
#define DMA_IFCR_CDMEIFx            0x04       //!< Stream x clear direct mode error interrupt flag
#define DMA_IFCR_CFEIFx             0x01       //!< Stream x clear FIFO error interrupt flag
/** @} */

/** @} */ // @relates DMA_TypeDef

#define STM32_DMA_ERRORS (DMA_ISR_TEIFx | DMA_ISR_DMEIFx )	//!< errors for F2 (DMA_ISR_FEIFx is fake???)

#define STM32_DMA_COMPLETE (DMA_ISR_TCIFx)	//!< complete for F2
#define STM32_DMA_HALF (DMA_IFCR_CHTIFx)	//!< half transfer for F2

/** DMA Driver mode structure **/
struct DMA_DRIVER_MODE
{
	DRIVER_INDEX dma_index;	//!< DMA driver index (DMAx_Streamx_IRQn or DMAx_Channelx_IRQn)

	uint32_t dma_ch_cr;		//!< DMA channel/stream config register value
	uint32_t dma_ch_fr;		//!< DMA channel/stream fifo register (not usend in F1..)
};

void stm32_dma_start(DMA_TypeDef* dmac, uint32_t indx, HANDLE hnd);
void stm32_dma_stop(DMA_TypeDef* dmac, uint32_t indx);
void stm32_dma_ch_cfg(DMA_TypeDef* dmac, uint32_t indx, DMA_DRIVER_MODE* mode);

static inline uint32_t stm32_get_ints(DMA_TypeDef* dmac, uint32_t indx)
{
	uint32_t status;

	switch(indx)
	{
	case 0:
		status = dmac->DMA_ISR[0] & 0x3f;
		dmac->DMA_IFCR[0] = status;
		break;

	case 1:
		status = dmac->DMA_ISR[0] & (0x3f << 6);
		dmac->DMA_IFCR[0] = status;
		status >>=6;
		break;

	case 2:
		status = dmac->DMA_ISR[0] & (0x3f << 16);
		dmac->DMA_IFCR[0] = status;
		status >>=16;
		break;

	case 3:
		status = dmac->DMA_ISR[0] & (0x3f << 22);
		dmac->DMA_IFCR[0] = status;
		status >>=22;
		break;

	case 4:
		status = dmac->DMA_ISR[1] & 0x3f;
		dmac->DMA_IFCR[1] = status;
		break;

	case 5:
		status = dmac->DMA_ISR[1] & (0x3f << 6);
		dmac->DMA_IFCR[1] = status;
		status >>=6;
		break;

	case 6:
		status = dmac->DMA_ISR[1] & (0x3f << 16);
		dmac->DMA_IFCR[1] = status;
		status >>=16;
		break;

	case 7:
		status = dmac->DMA_ISR[1] & (0x3f << 22);
		dmac->DMA_IFCR[1] = status;
		status >>=22;
		break;

	default:
		status = DMA_ISR_TEIFx;
		break;

	}

	return status;
}

static inline uint32_t stm32_dma_ndtr(DMA_TypeDef* dmac, uint32_t indx)
{
	return dmac->DMA_Chx[indx].DMA_SxNDTR;
}

static inline uint32_t stm32_dma_is_en(DMA_TypeDef* dmac, uint32_t indx)
{
	return dmac->DMA_Chx[indx].DMA_SxCR & DMA_SxCR_EN;
}

static inline uint32_t stm32_dma_msize(uint32_t SxCR, uint32_t size)
{
	switch(size)
	{
	case 8:
		size = DMA_SxCR_MSIZE_8bit;
		break;
	case 16:
		size = DMA_SxCR_MSIZE_16bit;
		break;
	case 32:
		size = DMA_SxCR_MSIZE_32bit;
		break;
	default:
		size = 0;
	}
	return ((SxCR & ~(DMA_SxCR_MSIZE))|size);
}

static inline uint32_t stm32_dma_psize(uint32_t SxCR, uint32_t size)
{
	switch(size)
	{
	case 8:
		size = DMA_SxCR_PSIZE_8bit;
		break;
	case 16:
		size = DMA_SxCR_PSIZE_16bit;
		break;
	case 32:
		size = DMA_SxCR_PSIZE_32bit;
		break;
	default:
		size = 0;
	}
	return ((SxCR & ~(DMA_SxCR_PSIZE))|size);
}

void stm32_en_ints(DMA_TypeDef* dmac, uint32_t indx, DMA_DRIVER_MODE* mode);
void stm32_dis_ints(DMA_TypeDef* dmac, uint32_t indx);

#endif /* DMA_F2_H_ */

/** @} */
