/*
 * sdio_stm32.h
 *
 *  Created on: Oct 3, 2013
 *      Author: miro
 */

#ifndef SDIO_STM32_H_
#define SDIO_STM32_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Secure digital input/output interface (SDIO)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t SDIO_POWER;     //!< (sdio offset 0x00) SDIO power control register
  __IO uint32_t SDIO_CLKCR;     //!< (sdio offset 0x04) SDIO clock control register
  __IO uint32_t SDIO_ARG;       //!< (sdio offset 0x08) SDIO argument register
  __IO uint32_t SDIO_CMD;       //!< (sdio offset 0x0C) SDIO command register
  __I  uint32_t SDIO_RESPCMD;   //!< (sdio offset 0x10) SDIO command response register
  __I  uint32_t SDIO_RESPx[4];  //!< (sdio offset 0x14) SDIO response 1-4 registers
  __IO uint32_t SDIO_DTIMER;    //!< (sdio offset 0x24) SDIO data timer register
  __IO uint32_t SDIO_DLEN;      //!< (sdio offset 0x28) SDIO data length register
  __IO uint32_t SDIO_DCTRL;     //!< (sdio offset 0x2C) SDIO data control register
  __I  uint32_t SDIO_DCOUNT;    //!< (sdio offset 0x30) SDIO data counter register
  __I  uint32_t SDIO_STA;       //!< (sdio offset 0x34) SDIO status register
  __IO uint32_t SDIO_ICR;       //!< (sdio offset 0x38) SDIO interrupt clear register
  __IO uint32_t SDIO_MASK;      //!< (sdio offset 0x3C) SDIO mask register
       uint32_t reserved0[2];   //!< (sdio offset 0x40)
  __I  uint32_t SDIO_FIFOCNT;   //!< (sdio offset 0x48) SDIO FIFO counter register
       uint32_t reserved1[13];	//!< (sdio offset 0x4C)
  __IO uint32_t SDIO_FIFO;      //!< (sdio offset 0x80) SDIO data FIFO register
} SDIO_TypeDef;

/*******************************************************************************
 * @defgroup SDIO_regs_define
 * @{
 ******************************************************************************/

/** @defgroup SDIO_POWER:   (sdio offset 0x00) SDIO power control register	  */
#define SDIO_POWER_PWRCTRL		0x0003 //!< PWRCTRL[1:0] Power supply control bits
#define SDIO_POWER_PWRCTRL_off	0x0000 //!<  00: Power-off: the clock to card is stopped
#define SDIO_POWER_PWRCTRL_up   0x0002 //!<  10: Reserved power-up
#define SDIO_POWER_PWRCTRL_on   0x0003 //!<  11: Power-on: the card is clocked
/** @} */

/** @defgroup SDIO_CLKCR:   (sdio offset 0x04) SDIO clock control register	  */
#define SDIO_CLKCR_HWFC_EN      0x4000 //!< HW Flow Control enable
#define SDIO_CLKCR_NEGEDGE      0x2000 //!< SDIO_CK dephasing selection bit
#define SDIO_CLKCR_WIDBUS       0x1800 //!< WIDBUS[1:0] Wide bus mode enable bit
#define SDIO_CLKCR_WIDBUS_1b    0x0000 //!<  00: Default bus mode: SDIO_D0 used
#define SDIO_CLKCR_WIDBUS_4b    0x0800 //!<  01: 4-wide bus mode: SDIO_D[3:0] used
#define SDIO_CLKCR_WIDBUS_8b    0x1000 //!<  10: 8-wide bus mode: SDIO_D[7:0] used
#define SDIO_CLKCR_BYPASS       0x0400 //!< Clock divider bypass enable bit
#define SDIO_CLKCR_PWRSAV       0x0200 //!< Power saving configuration bit
#define SDIO_CLKCR_CLKEN        0x0100 //!< Clock enable bit
#define SDIO_CLKCR_CLKDIV       0x00FF //!< Clock divide factor. SDIO_CK frequency = SDIOCLK / [CLKDIV + 2].
#define SDIO_CLKCR_CLKDIV_Get(x) ((x)&0xFF) //!< get macro
#define SDIO_CLKCR_CLKDIV_Set(x) ((x)&0xFF)	//!< set macro
/** @} */

/** @defgroup SDIO_ARG:     (sdio offset 0x08) SDIO argument register		  */
#define SDIO_ARG_CMDARG         0xFFFFFFFF //!< Command argument
/** @} */

/** @defgroup SDIO_CMD:     (sdio offset 0x0C) SDIO command register		  */
#define SDIO_CMD_CEATACMD       0x4000 //!< CE-ATA command
#define SDIO_CMD_NIEN           0x2000 //!< Not Interrupt Enable
#define SDIO_CMD_ENCMDCOMPL     0x1000 //!< Enable CMD completion
#define SDIO_CMD_SDIOSUSPEND    0x0800 //!< SD I/O suspend command
#define SDIO_CMD_CPSMEN         0x0400 //!< Command path state machine (CPSM) Enable bit
#define SDIO_CMD_WAITPEND       0x0200 //!< CPSM Waits for ends of data transfer (CmdPend internal signal)
#define SDIO_CMD_WAITINT        0x0100 //!< CPSM Waits for Interrupt Request
#define SDIO_CMD_WAITRESP       0x00C0 //!< WAITRESP[1:0] bits (Wait for response bits)
#define SDIO_CMD_WAITRESP_no0   0x0000 //!<  00: No response, expect CMDSENT flag
#define SDIO_CMD_WAITRESP_short 0x0040 //!<  01: Short response, expect CMDREND or CCRCFAIL flag
#define SDIO_CMD_WAITRESP_no1   0x0080 //!<  10: No response, expect CMDSENT flag
#define SDIO_CMD_WAITRESP_long  0x00C0 //!<  11: Long response, expect CMDREND or CCRCFAIL flag
#define SDIO_CMD_CMDINDEX       0x003F //!< Command index

//SD Memory Card bus commands (basic commands class 0)
#define SD_CMD00_GO_IDLE_STATE 		 0 //!< Resets all cards to idle state
#define SD_CMD02_ALL_SEND_CID   	 2 //!< Asks any card to send the CID numbers on the CMD line
#define SD_CMD03_SEND_RELATIVE_ADDR	 3 //!< Ask the card to publish a new relative address (RCA)
#define SD_CMD04_SET_DSR             4 //!< Programs the DSR of all cards
#define SD_CMD07_SEL_DESEL_CARD      7 //!< Command toggles a card between the stand-by and transfer states or between the programming and disconnect states.
#define SD_CMD08_SEND_IF_COND	     8 //!< Sends SD Memory Card interface condition
#define SD_CMD09_SEND_CSD            9 //!< Addressed card sends its card-specific data (CSD) on the CMD line.
#define SD_CMD10_SEND_CID           10 //!< Addressed card sends its card identification (CID) on CMD the line.
#define SD_CMD11_VOLTAGE_SWITCH     11 //!< Switch to 1.8V bus signaling level
#define SD_CMD12_STOP_TRANSMISSION  12 //!< Forces the card to stop transmission
#define SD_CMD13_SEND_STATUS        13 //!< Addressed card sends its status register
#define SD_CMD15_GO_INACTIVE_STATE  15 //!< Sends an addressed card into the Inactive State

//SD Memory Card bus commands (Block-Oriented Read Commands class 2)
#define SD_CMD16_SET_BLOCKLEN       16 //!< sets the block length (in bytes) for all following block commands
#define SD_CMD17_READ_SINGLE_BLOCK  17 //!< reads a block of the size selected by the SET_BLOCKLEN
#define SD_CMD18_READ_MULT_BLOCK    18 //!< Continuously transfers data blocks from card to host until interrupted by a STOP_TRANSMISSION command
#define SD_CMD19_SEND_TUNING_BLOCK  19 //!< 64 bytes tuning pattern is sent for SDR50 and SDR104
#define SD_CMD20_SPEED_CLASS_CONTROL 20//!< Speed Class control command
#define SD_CMD23_SET_BLOCK_COUNT    23 //!< Specify block count for CMD18 and CMD25

//SD Memory Card bus commands (Block-Oriented Write Commands class 4)
#define SD_CMD24_WRITE_BLOCK 		24 //!< Write block. block length is set by the SET_BLOCKLEN for SDSC and fixed 512 bytes for SDHC
#define SD_CMD25_WRITE_MULT_BLOCK   25 //!< Continuously writes blocks of data until a STOP_TRANSMISSION follows
#define SD_CMD27_PROGRAM_CSD        27 //!< Programming of the programmable bits of the CSD

//SD Memory Card bus commands (Block Oriented Write Protection Commands class 6) Not supported SDHC and SDXC Cards
#define SD_CMD28_SET_WRITE_PROT     28 //!< If the card has write protection features, this command sets the write protection bit of the addressed group
#define SD_CMD29_CLR_WRITE_PROT     29 //!< clears the write protection bit of the addressed group
#define SD_CMD30_SEND_WRITE_PROT    30 //!< send the status of the write protection bits

//SD Memory Card bus commands (Erase Commands class 5)
#define SD_CMD32_ERASE_WR_BLK_START 32 //!< Sets the address of the first write block to be erased
#define SD_CMD33_ERASE_WR_BLK_END   33 //!< Sets the address of the last write block of the continuous range to be erased
#define SD_CMD38_ERASE_WR_BLK_START 38 //!< Erases all previously selected write blocks

//SD Memory Card bus commands (Lock Card (class 7)
#define SD_CMD42_LOCK_UNLOCK		42 //!< Used to set/reset the password or lock/unlock the card. The size of the data block is set by the SET_BLOCK_LEN command

//SD Memory Card bus commands (Application-Specific Commands class 8)
#define SD_CMD55_APP_CMD            55 //!< Indicates to the card that the next command is an application specific command
#define SD_CMD56_GEN_CMD            56 //!< Used either to transfer a data block to the card or to get a data block from the card for general purpose/application specific commands

// Application Specific Commands used/reserved by SD Memory Card
// All the following ACMDs shall be preceded with APP_CMD command (CMD55)
#define SD_ACMD06_SET_BUS_WIDTH 	 6 //!< Defines the data bus width
#define SD_ACMD13_SD_STATUS         13 //!< Send the SD Status
#define SD_ACMD22_SEND_NUM_WR_BLOCKS 		22//!< Send the number of the written (without errors) write blocks.
#define SD_ACMD23_SET_WR_BLK_ERASE_COUNT    23 //!< Set the number of write blocks to be pre-erased before writing
#define SD_ACMD41_SD_SEND_OP_COND   		41 //!< Sends host capacity support information (HCS) and asks the accessed card to send its operating condition register (OCR) content
#define SD_ACMD42_SET_CLR_CARD_DETECT    	42 //!< Connect[1]/Disconnect[0] the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card
#define SD_ACMD51_SEND_SCR			    	51 //!< Reads the SD Configuration Register (SCR)

// Switch Function Commands (class 10)
#define SD_CMD06_SWITCH_FUNC 		 6 //!< Checks switchable function (mode 0) and switch card function (mode 1).

/** @} */

/** @defgroup SDIO_RESPCMD: (sdio offset 0x10) SDIO command response register */
#define SDIO_RESPCMD_RESPCMD    0x003F //!< Response command index
/** @} */

/** @defgroup SDIO_RESPx:   (sdio offset 0x14) SDIO response 1-4 registers	  */
#define SDIO_RESPx_CARDSTATUSx  0xFFFFFFFF //!< Card Status

#define SDIO_RESP1_OUT_OF_RANGE 		0x80000000 //!< The command's argument was out of the allowed range for this card
#define SDIO_RESP1_ADDRESS_ERROR 		0x40000000 //!< A misaligned address which did not match the block length was used in the command
#define SDIO_RESP1_BLOCK_LEN_ERROR 		0x20000000 //!< The transferred block length is not allowed for this card, or the number of transferred bytes does not match the block length
#define SDIO_RESP1_ERASE_SEQ_ERROR 		0x10000000 //!< An error in the sequence of erase commands occurred
#define SDIO_RESP1_ERASE_PARAM 			0x08000000 //!< An invalid selection of write-blocks for erase occurred
#define SDIO_RESP1_WP_VIOLATION 		0x04000000 //!< Set when the host attempts to write to a protected block or to the temporary or permanent write protected card
#define SDIO_RESP1_CARD_IS_LOCKED 		0x02000000 //!< When set, signals that the card is locked by the host
#define SDIO_RESP1_LOCK_UNLOCK_FAILED 	0x01000000 //!< Set when a sequence or password error has been detected in lock/unlock card command
#define SDIO_RESP1_COM_CRC_ERROR 		0x00800000 //!< The CRC check of the previous command failed
#define SDIO_RESP1_ILLEGAL_COMMAND 		0x00400000 //!< Command not legal for the card state
#define SDIO_RESP1_CARD_ECC_FAILED 		0x00200000 //!< Card internal ECC was applied but failed to correct the data
#define SDIO_RESP1_CC_ERROR 			0x00100000 //!< Internal card controller error
#define SDIO_RESP1_ERROR 				0x00080000 //!< A general or an unknown error occurred during the operation.
#define SDIO_RESP1_CSD_OVERWRITE 		0x00010000 //!< Can be either one of the following errors:
												   //!< - The read only section of the CSD does not match the card content.
												   //!< - An attempt to reverse the copy (set as original) or permanent WP (unprotected) bits was made
#define SDIO_RESP1_WP_ERASE_SKIP 		0x00008000 //!< "Set when only partial address space was erased due to existing write protected blocks or the temporary or permanent write protected card was erased
#define SDIO_RESP1_CARD_ECC_DISABLE 	0x00004000 //!< The command has been executed without using the internal ECC
#define SDIO_RESP1_ERASE_RESET 			0x00002000 //!< An erase sequence was cleared
#define SDIO_RESP1_CURRENT_STATE 		0x00001E00 //!< The state of the card when receiving the command. If the command execution causes a state change, it will be visible to the host in the response to the next command.
#define SDIO_RESP1_STATE_IDLE	 		0x00000000 //!< The card state is idle
#define SDIO_RESP1_STATE_READY          0x00000200 //!< The card state is ready
#define SDIO_RESP1_STATE_IDENT          0x00000300 //!< The card state is ident
#define SDIO_RESP1_STATE_STBY           0x00000400 //!< The card state is stby
#define SDIO_RESP1_STATE_TRAN           0x00000500 //!< The card state is tran
#define SDIO_RESP1_STATE_DATA           0x00000600 //!< The card state is data
#define SDIO_RESP1_STATE_RCV            0x00000700 //!< The card state is rcv
#define SDIO_RESP1_STATE_PRG            0x00000800 //!< The card state is prg
#define SDIO_RESP1_STATE_DIS            0x00001000 //!< The card state is dis

#define SDIO_RESP1_READY_FOR_DATA 		0x00000100 //!< Corresponds to buffer empty on the bus
#define SDIO_RESP1_APP_CMD 				0x00000020 //!< The card will expect ACMD, or an indication that the command has been interpreted as ACMD
#define SDIO_RESP1_SDIO					0x00000010 //!< reserved for SD I/O Card
#define SDIO_RESP1_AKE_SEQ_ERROR 		0x00000008 //!< Error in the sequence of the authentication process

#define SDIO_RESP1_ERRORS (SDIO_RESP1_OUT_OF_RANGE | SDIO_RESP1_ADDRESS_ERROR \
		| SDIO_RESP1_BLOCK_LEN_ERROR | SDIO_RESP1_ERASE_SEQ_ERROR \
		| SDIO_RESP1_ERASE_PARAM | SDIO_RESP1_WP_VIOLATION \
		| SDIO_RESP1_LOCK_UNLOCK_FAILED | SDIO_RESP1_COM_CRC_ERROR \
		| SDIO_RESP1_ILLEGAL_COMMAND | SDIO_RESP1_CARD_ECC_FAILED \
		| SDIO_RESP1_CC_ERROR | SDIO_RESP1_ERROR | SDIO_RESP1_CSD_OVERWRITE \
		| SDIO_RESP1_WP_ERASE_SKIP | SDIO_RESP1_CARD_ECC_DISABLE \
		| SDIO_RESP1_ERASE_RESET | SDIO_RESP1_AKE_SEQ_ERROR)
/** @} */


/** @defgroup SDIO_DTIMER:  (sdio offset 0x24) SDIO data timer register       */
#define SDIO_DTIMER_DATATIME    0xFFFFFFFF //!< Data timeout period.
/** @} */

/** @defgroup SDIO_DLEN:    (sdio offset 0x28) SDIO data length register      */
#define SDIO_DLEN_DATALENGTH    0x01FFFFFF //!< Data length value
/** @} */

/** @defgroup SDIO_DCTRL:   (sdio offset 0x2C) SDIO data control register     */
#define SDIO_DCTRL_SDIOEN       0x0800 //!< SD I/O enable functions
#define SDIO_DCTRL_RWMOD        0x0400 //!< Read wait mode
#define SDIO_DCTRL_RWSTOP       0x0200 //!< Read wait stop
#define SDIO_DCTRL_RWSTART      0x0100 //!< Read wait start
#define SDIO_DCTRL_DBLOCKSIZE   0x00F0 //!< DBLOCKSIZE[3:0] bits (Data block size)
#define SDIO_DCTRL_DBLOCKSIZE_1b	0x0000 //!< 1 byte
#define SDIO_DCTRL_DBLOCKSIZE_2b    0x0010 //!< 2 bytes
#define SDIO_DCTRL_DBLOCKSIZE_4b    0x0020 //!< 4 bytes
#define SDIO_DCTRL_DBLOCKSIZE_8b    0x0030 //!< 8 bytes
#define SDIO_DCTRL_DBLOCKSIZE_16b   0x0040 //!< 16 bytes
#define SDIO_DCTRL_DBLOCKSIZE_32b   0x0050 //!< 32 bytes
#define SDIO_DCTRL_DBLOCKSIZE_64b   0x0060 //!< 64 bytes
#define SDIO_DCTRL_DBLOCKSIZE_128b  0x0070 //!< 128 bytes
#define SDIO_DCTRL_DBLOCKSIZE_256b  0x0080 //!< 256 bytes
#define SDIO_DCTRL_DBLOCKSIZE_512b  0x0090 //!< 512 bytes
#define SDIO_DCTRL_DBLOCKSIZE_1kb 	0x00A0 //!< 1024 bytes
#define SDIO_DCTRL_DBLOCKSIZE_2kb 	0x00B0 //!< 2048 bytes
#define SDIO_DCTRL_DBLOCKSIZE_4kb	0x00C0 //!< 4096 bytes
#define SDIO_DCTRL_DBLOCKSIZE_8kb 	0x00D0 //!< 8192 bytes
#define SDIO_DCTRL_DBLOCKSIZE_16kb  0x00E0 //!< 16384 bytes

#define SDIO_DCTRL_DMAEN        0x0008 //!< DMA enabled bit
#define SDIO_DCTRL_DTMODE       0x0004 //!< Data transfer mode selection
#define SDIO_DCTRL_DTDIR        0x0002 //!< Data transfer direction selection
#define SDIO_DCTRL_DTEN         0x0001 //!< Data transfer enabled bit
/** @} */

/** @defgroup SDIO_DCOUNT:  (sdio offset 0x30) SDIO data counter register     */
#define SDIO_DCOUNT_DATACOUNT   0x01FFFFFF //!<Data count value
/** @} */

/** @defgroup SDIO_STA:     (sdio offset 0x34) SDIO status register           */
#define SDIO_STA_CEATAEND       0x00800000 //!< CE-ATA command completion signal received for CMD61
#define SDIO_STA_SDIOIT         0x00400000 //!< SDIO interrupt received
#define SDIO_STA_RXDAVL         0x00200000 //!< Data available in receive FIFO
#define SDIO_STA_TXDAVL         0x00100000 //!< Data available in transmit FIFO
#define SDIO_STA_RXFIFOE        0x00080000 //!< Receive FIFO empty
#define SDIO_STA_TXFIFOE        0x00040000 //!< Transmit FIFO empty
#define SDIO_STA_RXFIFOF        0x00020000 //!< Receive FIFO full
#define SDIO_STA_TXFIFOF        0x00010000 //!< Transmit FIFO full
#define SDIO_STA_RXFIFOHF       0x00008000 //!< Receive FIFO Half Full: there are at least 8 words in the FIFO
#define SDIO_STA_TXFIFOHE       0x00004000 //!< Transmit FIFO Half Empty: at least 8 words can be written into the FIFO
#define SDIO_STA_RXACT          0x00002000 //!< Data receive in progress
#define SDIO_STA_TXACT          0x00001000 //!< Data transmit in progress
#define SDIO_STA_CMDACT         0x00000800 //!< Command transfer in progress
#define SDIO_STA_DBCKEND        0x00000400 //!< Data block sent/received (CRC check passed)
#define SDIO_STA_STBITERR       0x00000200 //!< Start bit not detected on all data signals in wide bus mode
#define SDIO_STA_DATAEND        0x00000100 //!< Data end (data counter, SDIDCOUNT, is zero)
#define SDIO_STA_CMDSENT        0x00000080 //!< Command sent (no response required)
#define SDIO_STA_CMDREND        0x00000040 //!< Command response received (CRC check passed)
#define SDIO_STA_RXOVERR        0x00000020 //!< Received FIFO overrun error
#define SDIO_STA_TXUNDERR       0x00000010 //!< Transmit FIFO underrun error
#define SDIO_STA_DTIMEOUT       0x00000008 //!< Data timeout
#define SDIO_STA_CTIMEOUT       0x00000004 //!< Command response timeout
#define SDIO_STA_DCRCFAIL       0x00000002 //!< Data block sent/received (CRC check failed)
#define SDIO_STA_CCRCFAIL       0x00000001 //!< Command response received (CRC check failed)

#define SDIO_STA_DONE_TR	(SDIO_STA_CEATAEND | SDIO_STA_SDIOIT | \
							  SDIO_STA_DBCKEND | SDIO_STA_DATAEND )

#define SDIO_STA_DONE_CMD	(SDIO_STA_CMDSENT  | SDIO_STA_CMDREND )

#define SDIO_STA_DONE_FLAGS	(SDIO_STA_DONE_TR | SDIO_STA_DONE_CMD)

#define SDIO_STA_ERROR_FLAGS (SDIO_STA_STBITERR | SDIO_STA_RXOVERR  | SDIO_STA_TXUNDERR \
		| SDIO_STA_DTIMEOUT | SDIO_STA_CTIMEOUT | SDIO_STA_DCRCFAIL | SDIO_STA_CCRCFAIL)

#define SDIO_STA_RX_FLAGS 	(SDIO_STA_RXDAVL | SDIO_STA_RXFIFOF | SDIO_STA_RXFIFOHF)

#define SDIO_STA_TX_FLAGS 	( SDIO_STA_TXFIFOE | SDIO_STA_TXFIFOHE )

#define SDIO_STA__XXX 	(SDIO_STA_RX_FLAGS | SDIO_STA_TX_FLAGS | SDIO_STA_TXDAVL \
		| SDIO_STA_RXFIFOE | SDIO_STA_TXFIFOF | SDIO_STA_RXACT 			  \
		| SDIO_STA_TXACT | SDIO_STA_CMDACT)


/** @} */

/** @defgroup SDIO_ICR:     (sdio offset 0x38) SDIO interrupt clear register  */
#define SDIO_ICR_CCRCFAILC      0x00000001 //!< CCRCFAIL flag clear bit
#define SDIO_ICR_DCRCFAILC      0x00000002 //!< DCRCFAIL flag clear bit
#define SDIO_ICR_CTIMEOUTC      0x00000004 //!< CTIMEOUT flag clear bit
#define SDIO_ICR_DTIMEOUTC      0x00000008 //!< DTIMEOUT flag clear bit
#define SDIO_ICR_TXUNDERRC      0x00000010 //!< TXUNDERR flag clear bit
#define SDIO_ICR_RXOVERRC       0x00000020 //!< RXOVERR flag clear bit
#define SDIO_ICR_CMDRENDC       0x00000040 //!< CMDREND flag clear bit
#define SDIO_ICR_CMDSENTC       0x00000080 //!< CMDSENT flag clear bit
#define SDIO_ICR_DATAENDC       0x00000100 //!< DATAEND flag clear bit
#define SDIO_ICR_STBITERRC      0x00000200 //!< STBITERR flag clear bit
#define SDIO_ICR_DBCKENDC       0x00000400 //!< DBCKEND flag clear bit
#define SDIO_ICR_SDIOITC        0x00400000 //!< SDIOIT flag clear bit
#define SDIO_ICR_CEATAENDC      0x00800000 //!< CEATAEND flag clear bit
/** @} */

/** @defgroup SDIO_MASK:    (sdio offset 0x3C) SDIO mask register             */
#define SDIO_MASK_CEATAENDIE    0x00800000 //!<CE-ATA command completion signal received Interrupt Enable
#define SDIO_MASK_SDIOITIE      0x00400000 //!<SDIO Mode Interrupt Received interrupt Enable
#define SDIO_MASK_RXDAVLIE      0x00200000 //!<Data available in Rx FIFO interrupt Enable
#define SDIO_MASK_TXDAVLIE      0x00100000 //!<Data available in Tx FIFO interrupt Enable
#define SDIO_MASK_RXFIFOEIE     0x00080000 //!<Rx FIFO Empty interrupt Enable
#define SDIO_MASK_TXFIFOEIE     0x00040000 //!<Tx FIFO Empty interrupt Enable
#define SDIO_MASK_RXFIFOFIE     0x00020000 //!<Rx FIFO Full interrupt Enable
#define SDIO_MASK_TXFIFOFIE     0x00010000 //!<Tx FIFO Full interrupt Enable
#define SDIO_MASK_RXFIFOHFIE    0x00008000 //!<Rx FIFO Half Full interrupt Enable
#define SDIO_MASK_TXFIFOHEIE    0x00004000 //!<Tx FIFO Half Empty interrupt Enable
#define SDIO_MASK_RXACTIE       0x00002000 //!<Data receive acting interrupt enabled
#define SDIO_MASK_TXACTIE       0x00001000 //!<Data Transmit Acting Interrupt Enable
#define SDIO_MASK_CMDACTIE      0x00000800 //!<CCommand Acting Interrupt Enable
#define SDIO_MASK_DBCKENDIE     0x00000400 //!<Data Block End Interrupt Enable
#define SDIO_MASK_STBITERRIE    0x00000200 //!<Start Bit Error Interrupt Enable
#define SDIO_MASK_DATAENDIE     0x00000100 //!<Data End Interrupt Enable
#define SDIO_MASK_CMDSENTIE     0x00000080 //!<Command Sent Interrupt Enable
#define SDIO_MASK_CMDRENDIE     0x00000040 //!<Command Response Received Interrupt Enable
#define SDIO_MASK_RXOVERRIE     0x00000020 //!<Rx FIFO OverRun Error Interrupt Enable
#define SDIO_MASK_TXUNDERRIE    0x00000010 //!<Tx FIFO UnderRun Error Interrupt Enable
#define SDIO_MASK_DTIMEOUTIE    0x00000008 //!<Data TimeOut Interrupt Enable
#define SDIO_MASK_CTIMEOUTIE    0x00000004 //!<Command TimeOut Interrupt Enable
#define SDIO_MASK_DCRCFAILIE    0x00000002 //!<Data CRC Fail Interrupt Enable
#define SDIO_MASK_CCRCFAILIE    0x00000001 //!<Command CRC Fail Interrupt Enable
/** @} */

/** @defgroup SDIO_FIFOCNT: (sdio offset 0x48) SDIO FIFO counter register     */
#define SDIO_FIFOCNT_FIFOCOUNT  0x00FFFFFF //!< Remaining number of words to be written to or read from the FIFO
/** @} */

/** @defgroup SDIO_FIFO:    (sdio offset 0x80) SDIO data FIFO register        */
#define SDIO_FIFO_FIFODATA      0xFFFFFFFF //!< Receive and transmit FIFO data
/** @} */


/** @} */ // @defgroup SDIO_regs_define


#endif /* SDIO_STM32_H_ */
