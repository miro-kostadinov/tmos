/**************************************************************************//**
 * @ingroup	hardware_stm32_spi
 * @file	hardware/stm32/spi_stm32.h
 * @brief  	STM32 SPI
 * @date    20. November 2012
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32_spi Serial Peripheral Interface (SPI)
 * @ingroup	 hardware_stm32
 * Source and definitions for STM32 SPI Controller
 * @{
 *
 ******************************************************************************/

#ifndef SPI_STM32_H_
#define SPI_STM32_H_

#include <mcu_inc.h>
#include <tmos_types.h>

#ifndef CFG_SERIES
#error "unknown series"
#endif

/// HW version of the SPI, default value
#if (CFG_SERIES == stm32f0) || (CFG_SERIES == stm32f3)
#define STM32_SPI_TYPE 03
#endif

#ifndef STM32_SPI_TYPE
#define STM32_SPI_TYPE 124
#endif

/***************************************************************************//**
 *  Serial Peripheral Interface
 ******************************************************************************/
typedef struct
{
  __IO uint32_t SPI_CR1;        //!< (spi Offset: 0x00) SPI control register 1 (not used in I2S mode)
  __IO uint32_t SPI_CR2;        //!< (spi Offset: 0x04) SPI control register 2
  __IO uint32_t SPI_SR;         //!< (spi Offset: 0x08) SPI status register
  __IO uint32_t SPI_DR;         //!< (spi Offset: 0x0C) SPI data register
  __IO uint32_t SPI_CRCPR;      //!< (spi Offset: 0x10) SPI CRC polynomial register (not used in I2S mode)
  __IO uint32_t SPI_RXCRCR;     //!< (spi Offset: 0x14) SPI RX CRC register (not used in I2S mode)
  __IO uint32_t SPI_TXCRCR;     //!< (spi Offset: 0x18) SPI TX CRC register (not used in I2S mode)
  __IO uint32_t SPI_I2SCFGR;    //!< (spi Offset: 0x1C) SPI_I2S configuration register
  __IO uint32_t SPI_I2SPR;      //!< (spi Offset: 0x20) SPI_I2S prescaler register
} SPI_TypeDef;

/***************************************************************************//**
 * @relates SPI_TypeDef
 * @{
 ******************************************************************************/

/** @name SPI_CR1:      (spi Offset: 0x00) SPI control register 1 (not used in I2S mode) */
/** @{ */
#define  SPI_CR1_CPHA           0x0001 //!< Clock Phase
#define  SPI_CR1_CPOL           0x0002 //!< Clock Polarity
#define  SPI_CR1_MSTR           0x0004 //!< Master Selection

#define  SPI_CR1_BR             0x0038 //!< BR[2:0] bits (Baud Rate Control)
#define  SPI_CR1_BR_DIV2        0x0000 //!<  fPCLK/2
#define  SPI_CR1_BR_DIV4        0x0008 //!<  fPCLK/4
#define  SPI_CR1_BR_DIV8        0x0010 //!<  fPCLK/8
#define  SPI_CR1_BR_DIV16       0x0018 //!<  fPCLK/16
#define  SPI_CR1_BR_DIV32       0x0020 //!<  fPCLK/32
#define  SPI_CR1_BR_DIV64       0x0028 //!<  fPCLK/64
#define  SPI_CR1_BR_DIV128      0x0030 //!<  fPCLK/128
#define  SPI_CR1_BR_DIV256      0x0038 //!<  fPCLK/256

#define  SPI_CR1_SPE            0x0040 //!< SPI Enable
#define  SPI_CR1_LSBFIRST       0x0080 //!< Frame Format
#define  SPI_CR1_MSBFIRST		0x0000 //!< Frame Format
#define  SPI_CR1_SSI            0x0100 //!< Internal slave select
#define  SPI_CR1_SSM            0x0200 //!< Software slave management
#define  SPI_CR1_RXONLY         0x0400 //!< Receive only

#if STM32_SPI_TYPE == 124
#define  SPI_CR1_DFF            0x0800 //!< Data Frame Format
#define  SPI_CR1_DFF_8bit       0x0000 //!<  8-bit data frame format
#define  SPI_CR1_DFF_16bit      0x0800 //!<  8-bit data frame format
#else
#define  SPI_CR1_CRCL           0x0800 //!< CRC Length
#endif

#define  SPI_CR1_CRCNEXT        0x1000 //!< Transmit CRC next
#define  SPI_CR1_CRCEN          0x2000 //!< Hardware CRC calculation enable
#define  SPI_CR1_BIDIOE         0x4000 //!< Output enable in bidirectional mode
#define  SPI_CR1_BIDIMODE       0x8000 //!< Bidirectional data mode enable
/** @} */

/** @name SPI_CR2:      (spi Offset: 0x04) SPI control register 2 				*/
/** @{ */
#define  SPI_CR2_RXDMAEN        0x01   //!< Rx Buffer DMA Enable
#define  SPI_CR2_TXDMAEN        0x02   //!< Tx Buffer DMA Enable
#define  SPI_CR2_SSOE           0x04   //!< SS Output Enable
#define  SPI_CR2_FRF            0x0010 //!< Frame Format Enable
#define  SPI_CR2_FRF_MOTO       0x0000 //!<  SPI Motorola mode
#define  SPI_CR2_FRF_TI         0x0010 //!<  SPI TI mode

#if STM32_SPI_TYPE == 03
#define  SPI_CR2_NSSP           0x0008 //!< NSS pulse management Enable
#endif

#define  SPI_CR2_ERRIE          0x20   //!< Error Interrupt Enable
#define  SPI_CR2_RXNEIE         0x40   //!< RX buffer Not Empty Interrupt Enable
#define  SPI_CR2_TXEIE          0x80   //!< Tx buffer Empty Interrupt Enable

#if STM32_SPI_TYPE == 03
#define  SPI_CR2_DS             0x0F00 //!< DS[3:0] Data Size
#define  SPI_CR2_DS_4bit        0x0300 //!<  4-bit
#define  SPI_CR2_DS_5bit        0x0400 //!<  5-bit
#define  SPI_CR2_DS_6bit        0x0500 //!<  6-bit
#define  SPI_CR2_DS_7bit        0x0600 //!<  7-bit
#define  SPI_CR2_DS_8bit        0x0700 //!<  8-bit
#define  SPI_CR2_DS_9bit        0x0800 //!<  9-bit
#define  SPI_CR2_DS_10bit       0x0900 //!<  10-bit
#define  SPI_CR2_DS_11bit       0x0A00 //!<  11-bit
#define  SPI_CR2_DS_12bit       0x0B00 //!<  12-bit
#define  SPI_CR2_DS_13bit       0x0C00 //!<  13-bit
#define  SPI_CR2_DS_14bit       0x0D00 //!<  14-bit
#define  SPI_CR2_DS_15bit       0x0E00 //!<  15-bit
#define  SPI_CR2_DS_16bit       0x0F00 //!<  16-bit
#define  SPI_CR2_FRXTH          0x1000 //!< FIFO reception Threshold
#define  SPI_CR2_LDMARX         0x2000 //!< Last DMA transfer for reception
#define  SPI_CR2_LDMATX         0x4000 //!< Last DMA transfer for transmission
#endif

/** @} */

/** @name SPI_SR:       (spi Offset: 0x08) SPI status register					*/
/** @{ */
#define  SPI_SR_RXNE            0x01   //!< Receive buffer Not Empty
#define  SPI_SR_TXE             0x02   //!< Transmit buffer Empty
#define  SPI_SR_CHSIDE          0x04   //!< Channel side
#define  SPI_SR_UDR             0x08   //!< Underrun flag
#define  SPI_SR_CRCERR          0x10   //!< CRC Error flag
#define  SPI_SR_MODF            0x20   //!< Mode fault
#define  SPI_SR_OVR             0x40   //!< Overrun flag
#define  SPI_SR_BSY             0x80   //!< Busy flag

#if STM32_SPI_TYPE == 03
#define  SPI_SR_FRE             0x0100 //!< TI frame format error
#define  SPI_SR_FRLVL           0x0600 //!< FIFO Reception Level
#define  SPI_SR_FRLVL_0         0x0200 //!< Bit 0
#define  SPI_SR_FRLVL_1         0x0400 //!< Bit 1
#define  SPI_SR_FTLVL           0x1800 //!< FIFO Transmission Level
#define  SPI_SR_FTLVL_0         0x0800 //!< Bit 0
#define  SPI_SR_FTLVL_1         0x1000 //!< Bit 1
#endif

/** @} */

/** @name SPI_DR:       (spi Offset: 0x0C) SPI data register					*/
/** @{ */
#define  SPI_DR_DR              0xFFFF //!< Data Register
/** @} */

/** @name SPI_CRCPR:    (spi Offset: 0x10) SPI CRC polynomial register (not used in I2S mode) */
/** @{ */
#define  SPI_CRCPR_CRCPOLY      0xFFFF //!< CRC polynomial register
/** @} */

/** @name SPI_RXCRCR:   (spi Offset: 0x14) SPI RX CRC register (not used in I2S mode) */
/** @{ */
#define  SPI_RXCRCR_RXCRC       0xFFFF //!< Rx CRC Register
/** @} */

/** @name SPI_TXCRCR:   (spi Offset: 0x18) SPI TX CRC register (not used in I2S mode) */
/** @{ */
#define  SPI_TXCRCR_TXCRC       0xFFFF //!< Tx CRC Register
/** @} */

/** @name SPI_I2SCFGR:  (spi Offset: 0x1C) SPI_I2S configuration register	 	*/
/** @{ */
#define  SPI_I2SCFGR_CHLEN      0x0001 //!< Channel length (number of bits per audio channel)

#define  SPI_I2SCFGR_DATLEN     0x0006 //!< DATLEN[1:0] bits (Data length to be transferred)
#define  SPI_I2SCFGR_DATLEN_0   0x0002 //!< Bit 0
#define  SPI_I2SCFGR_DATLEN_1   0x0004 //!< Bit 1

#define  SPI_I2SCFGR_CKPOL      0x0008 //!< steady state clock polarity */

#define  SPI_I2SCFGR_I2SSTD     0x0030 //!< I2SSTD[1:0] bits (I2S standard selection) */
#define  SPI_I2SCFGR_I2SSTD_0   0x0010 //!< Bit 0
#define  SPI_I2SCFGR_I2SSTD_1   0x0020 //!< Bit 1

#define  SPI_I2SCFGR_PCMSYNC    0x0080 //!< PCM frame synchronization

#define  SPI_I2SCFGR_I2SCFG     0x0300 //!< I2SCFG[1:0] bits (I2S configuration mode)
#define  SPI_I2SCFGR_I2SCFG_0   0x0100 //!< Bit 0
#define  SPI_I2SCFGR_I2SCFG_1   0x0200 //!< Bit 1

#define  SPI_I2SCFGR_I2SE       0x0400 //!< I2S Enable
#define  SPI_I2SCFGR_I2SMOD     0x0800 //!< I2S mode selection
/** @} */

/** @name SPI_I2SPR:    (spi Offset: 0x20) SPI_I2S prescaler register			*/
/** @{ */
#define  SPI_I2SPR_I2SDIV       0x00FF //!< I2S Linear prescaler
#define  SPI_I2SPR_ODD          0x0100 //!< Odd factor for the prescaler
#define  SPI_I2SPR_MCKOE        0x0200 //!< Master Clock Output Enable
/** @} */


/** @} */ // @relates SPI_TypeDef



#endif /* SPI_STM32_H_ */

/** @} */
