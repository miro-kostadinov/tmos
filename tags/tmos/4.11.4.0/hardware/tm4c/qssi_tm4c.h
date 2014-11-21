/*
 * qssi_tm4c.h
 *
 *  Created on: Sep 3, 2014
 *      Author: miro
 */

#ifndef QSSI_TM4C_H_
#define QSSI_TM4C_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Quad Synchronous Serial Interface (QSSI)
 ******************************************************************************/
struct QSSI_TypeDef
{
	__IO uint32_t SSICR0;  			//!< (qssi: 0x000) QSSI Control 0
	__IO uint32_t SSICR1;  			//!< (qssi: 0x004) QSSI Control 1
	__IO uint32_t SSIDR;   			//!< (qssi: 0x008) QSSI Data
	__I  uint32_t SSISR;   			//!< (qssi: 0x00C) QSSI Status
	__IO uint32_t SSICPSR;  		//!< (qssi: 0x010) QSSI Clock Prescale
	__IO uint32_t SSIIM;   			//!< (qssi: 0x014) QSSI Interrupt Mask
	__I  uint32_t SSIRIS;   		//!< (qssi: 0x018) QSSI Raw Interrupt Status
	__I  uint32_t SSIMIS;   		//!< (qssi: 0x01C) QSSI Masked Interrupt Status
	__IO uint32_t SSIICR;   		//!< (qssi: 0x020) QSSI Interrupt Clear
	__IO uint32_t SSIDMACTL; 		//!< (qssi: 0x024) QSSI DMA Control
		 uint32_t reserved[1000];	//!< (qssi: 0x024)
	__I  uint32_t SSIPP;   			//!< (qssi: 0xFC0) QSSI Peripheral Properties
	__IO uint32_t SSICC;   			//!< (qssi: 0xFC8) QSSI Clock Configuration
};

/*******************************************************************************
 * @defgroup QSPI_regs_define
 * @{
 ******************************************************************************/

/** @defgroup SSICR0:  		(qssi: 0x000) QSSI Control 0                      */
#define QSSI_SSICR0_SCR				0x0000FF00 		//!< QSSI Serial Clock Rate
#define QSSI_SSICR0_SPH				0x00000080 		//!< QSSI Serial Clock Phase
#define QSSI_SSICR0_SPO				0x00000040 		//!< QSSI Serial Clock Polarity
#define QSSI_SSICR0_FRF				0x00000030 		//!< QSSI Frame Format Select
#define QSSI_SSICR0_DSS				0x0000000F 		//!< QSSI Data Size Select
/** @} */

/** @defgroup SSICR1:  		(qssi: 0x004) QSSI Control 1                      */
#define QSSI_SSICR1_EOM				0x00000800 		//!< Stop Frame (End of Message)
#define QSSI_SSICR1_FSSHLDFRM		0x00000400 		//!< FSS Hold Frame
#define QSSI_SSICR1_HSCLKEN			0x00000200 		//!< High Speed Clock Enable
#define QSSI_SSICR1_DIR				0x00000100 		//!< QSSI Direction of Operation
#define QSSI_SSICR1_DIR_TX			0x00000000 		//!<  TX (Transmit Mode) write direction
#define QSSI_SSICR1_DIR_RX			0x00000100 		//!<  RX (Receive Mode) read direction
#define QSSI_SSICR1_MODE			0x000000C0 		//!< QSSI Mode
#define QSSI_SSICR1_MODE_SSI		0x00000000 		//!<  Legacy SSI mode
#define QSSI_SSICR1_MODE_BSSI		0x00000040 		//!<  Bi-SSI mode
#define QSSI_SSICR1_MODE_QSSI		0x00000080 		//!<  Quad-SSI Mode
#define QSSI_SSICR1_MODE_ASSI		0x000000C0 		//!<  Advanced SSI Mode with 8-bit packet size
#define QSSI_SSICR1_MS				0x00000004 		//!< QSSI Master/Slave Select
#define QSSI_SSICR1_MS_MASTER		0x00000000 		//!<  The QSSI is configured as a master
#define QSSI_SSICR1_MS_SLAVE		0x00000004 		//!<  The QSSI is configured as a slave
#define QSSI_SSICR1_SSE				0x00000002 		//!< QSSI Synchronous Serial Port Enable
#define QSSI_SSICR1_LBM				0x00000001 		//!< QSSI Loopback Mode
/** @} */

/** @defgroup SSIDR:   		(qssi: 0x008) QSSI Data                           */
#define QSSI_SSIDR_DATA				0x0000FFFF 		//!< QSSI Receive/Transmit Data
/** @} */

/** @defgroup SSISR:   		(qssi: 0x00C) QSSI Status                         */
#define QSSI_SSISR_BSY				0x00000010 		//!< QSSI Busy Bit
#define QSSI_SSISR_RFF				0x00000008 		//!< QSSI Receive FIFO Full
#define QSSI_SSISR_RNE				0x00000004 		//!< QSSI Receive FIFO Not Empty
#define QSSI_SSISR_TNF				0x00000002 		//!< QSSI Transmit FIFO Not Full
#define QSSI_SSISR_TFE				0x00000001 		//!< QSSI Transmit FIFO Empty
/** @} */

/** @defgroup SSICPSR:  	(qssi: 0x010) QSSI Clock Prescale                 */
#define QSSI_SSICPSR_CPSDVSR		0x000000FF 		//!< QSSI Clock Prescale Divisor
/** @} */

/** @defgroup SSIIM:   		(qssi: 0x014) QSSI Interrupt Mask                 */
#define QSSI_SSIIM_EOTIM			0x00000040 		//!< End of Transmit Interrupt Mask
#define QSSI_SSIIM_DMATXIM			0x00000020 		//!< QSSI Transmit DMA Interrupt Mask
#define QSSI_SSIIM_DMARXIM			0x00000010 		//!< QSSI Receive DMA Interrupt Mask
#define QSSI_SSIIM_TXIM				0x00000008 		//!< QSSI Transmit FIFO Interrupt Mask
#define QSSI_SSIIM_RXIM				0x00000004 		//!< QSSI Receive FIFO Interrupt Mask
#define QSSI_SSIIM_RTIM				0x00000002 		//!< QSSI Receive Time-Out Interrupt Mask
#define QSSI_SSIIM_RORIM			0x00000001 		//!< QSSI Receive Overrun Interrupt Mask
/** @} */

/** @defgroup SSIRIS:   	(qssi: 0x018) QSSI Raw Interrupt Status           */
#define QSSI_SSIRIS_EOTRIS			0x00000040 		//!< End of Transmit Raw Interrupt Status
#define QSSI_SSIRIS_DMATXRIS		0x00000020 		//!< QSSI Transmit DMA Raw Interrupt Status
#define QSSI_SSIRIS_DMARXRIS		0x00000010 		//!< QSSI Receive DMA Raw Interrupt Status
#define QSSI_SSIRIS_TXRIS			0x00000008 		//!< QSSI Transmit FIFO Raw Interrupt Status
#define QSSI_SSIRIS_RXRIS			0x00000004 		//!< QSSI Receive FIFO Raw Interrupt Status
#define QSSI_SSIRIS_RTRIS			0x00000002 		//!< QSSI Receive Time-Out Raw Interrupt Status
#define QSSI_SSIRIS_RORRIS			0x00000001 		//!< QSSI Receive Overrun Raw Interrupt Status
/** @} */

/** @defgroup SSIMIS:   	(qssi: 0x01C) QSSI Masked Interrupt Status        */
#define QSSI_SSIMIS_EOTMIS			0x00000040 		//!< End of Transmit Masked Interrupt Status
#define QSSI_SSIMIS_DMATXMIS		0x00000020 		//!< QSSI Transmit DMA Masked Interrupt Status
#define QSSI_SSIMIS_DMARXMIS		0x00000010 		//!< QSSI Receive DMA Masked Interrupt Status
#define QSSI_SSIMIS_TXMIS			0x00000008 		//!< QSSI Transmit FIFO Masked Interrupt Status
#define QSSI_SSIMIS_RXMIS			0x00000004 		//!< QSSI Receive FIFO Masked Interrupt Status
#define QSSI_SSIMIS_RTMIS			0x00000002 		//!< QSSI Receive Time-Out Masked Interrupt Status
#define QSSI_SSIMIS_RORMIS			0x00000001 		//!< QSSI Receive Overrun Masked Interrupt Status
/** @} */

/** @defgroup SSIICR:   	(qssi: 0x020) QSSI Interrupt Clear                */
#define QSSI_SSIICR_EOTIC			0x00000040 		//!< End of Transmit Interrupt Clear
#define QSSI_SSIICR_DMATXIC			0x00000020 		//!< QSSI Transmit DMA Interrupt Clear
#define QSSI_SSIICR_DMARXIC			0x00000010 		//!< QSSI Receive DMA Interrupt Clear
#define QSSI_SSIICR_RTIC			0x00000002 		//!< QSSI Receive Time-Out Interrupt Clear
#define QSSI_SSIICR_RORIC			0x00000001 		//!< QSSI Receive Overrun Interrupt Clear
/** @} */

/** @defgroup SSIDMACTL: 	(qssi: 0x024) QSSI DMA Control                    */
#define QSSI_SSIDMACTL_TXDMAE		0x00000002 		//!< Transmit DMA Enable
#define QSSI_SSIDMACTL_RXDMAE		0x00000001 		//!< Receive DMA Enable
/** @} */

/** @defgroup SSIPP:   		(qssi: 0xFC0) QSSI Peripheral Properties          */
#define QSSI_SSIPP_FSSHLDFRM		0x00000008 		//!< SSInFss Hold Frame Capability
#define QSSI_SSIPP_MODE				0x00000006 		//!< Mode of Operation
#define QSSI_SSIPP_MODE_SSI			0x00000000 		//!<  Legacy SSI mode
#define QSSI_SSIPP_MODE_ABSSI		0x00000002 		//!<  Legacy mode, Advanced SSI mode and Bi-SSI mode enabled
#define QSSI_SSIPP_MODE_ABQSSI		0x00000004 		//!<  Legacy mode, Advanced mode, Bi-SSI and Quad-SSI mode enabled
#define QSSI_SSIPP_HSCLK			0x00000001 		//!< High Speed Capability
/** @} */

/** @defgroup SSICC:   		(qssi: 0xFC8) QSSI Clock Configuration 			  */
#define QSSI_SSICC_CS				0x0000000F 		//!< QSSI Baud Clock Source
#define QSSI_SSICC_CS_SYSCLK		0x00000000 		//!<  System clock (based on clock source and divisor factor programmed in RSCLKCFG register in the System Control Module)
#define QSSI_SSICC_CS_ALTCLK		0x00000005 		//!<  Alternate clock source as defined by ALTCLKCFG register in System Control Module.
/** @} */


/** @} */ // @defgroup QSPI_regs_define



#endif /* QSSI_TM4C_H_ */
