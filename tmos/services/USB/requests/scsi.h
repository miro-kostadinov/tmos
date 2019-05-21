/*
 * scsi.h
 *
 *  Created on: Aug 10, 2016
 *      Author: miro
 */

#ifndef SERVICES_USB_REQUESTS_SCSI_H_
#define SERVICES_USB_REQUESTS_SCSI_H_

#include <tmos.h>

//------------------------------------------------------------------------------
//         SCSI Commands
//------------------------------------------------------------------------------


#define SCSI_CMD_GROUP_CODE				0xE0
#define SCSI_CMD_GROUP0_6BYTES			0x00
#define SCSI_CMD_GROUP1_10BYTES			0x20
#define SCSI_CMD_GROUP2_10BYTES			0x40
#define SCSI_CMD_GROUP4_16BYTES			0x80
#define SCSI_CMD_GROUP5_12BYTES			0xA0
#define SCSI_CMD_GROUP6					0xC0
#define SCSI_CMD_GROUP7					0xE0


//==================================================================================================+
//       									 	      D - DIRECT ACCESS DEVICE                      |
//   Device Column Key  					 	      .T - SEQUENTIAL ACCESS DEVICE                 |
//   M = Mandatory      					 	      . L - PRINTER DEVICE                          |
//   O = Optional       					 	      .  P - PROCESSOR DEVICE                       |
//   V = Vendor Specific					 	      .  .W - WRITE ONCE READ MULTIPLE DEVICE       |
//   R = Reserved       					 	      .  . R - READ ONLY (CD-ROM) DEVICE            |
//                           						  .  .  S - SCANNER DEVICE  					|
//       									 	      .  .  .O - OPTICAL MEMORY DEVICE              |
//       									 	      .  .  . M - MEDIA CHANGER DEVICE              |
//       									 	      .  .  .  C - COMMUNICATION DEVICE             |
//---------------------------------------------------+----------+-----------------------------------|

/// Group 0 (6 bytes)
#define SCSI_CMD_TEST_UNIT_READY       	0x00	//!<  MMMMMMMMMM
#define SCSI_CMD_REZERO_UNIT           	0x01    //!<  O V OO OO
#define SCSI_CMD_REQUEST_SENSE         	0x03    //!<  MMMMMMMMMM
#define SCSI_CMD_FORMAT_UNIT           	0x04    //!<  M      O
#define SCSI_CMD_READ_BLOCK_LIMITS     	0x05    //!<  VMVVVV  V
#define SCSI_CMD_REASSIGN_BLOCKS       	0x07    //!<  OVV O  OV
#define SCSI_CMD_READ_6                	0x08    //!<  OMV OO OV
#define SCSI_CMD_WRITE_6               	0x0a    //!<  OM  O  OV
#define SCSI_CMD_SEEK_6                	0x0b    //!<  O   OO OV
#define SCSI_CMD_READ_REVERSE          	0x0f    //!<  VOVVVV  V
#define SCSI_CMD_WRITE_FILEMARKS       	0x10    //!<  VM VVV
#define SCSI_CMD_SPACE                 	0x11    //!<  VMVVVV
#define SCSI_CMD_INQUIRY               	0x12    //!<  MMMMMMMMMM
#define SCSI_CMD_RECOVER_BUFFERED_DATA 	0x14    //!<  VOOVVV
#define SCSI_CMD_MODE_SELECT           	0x15    //!<  OMO OOOOOO
#define SCSI_CMD_RESERVE               	0x16    //!<  M   MM MO
#define SCSI_CMD_RELEASE               	0x17    //!<  M   MM MO
#define SCSI_CMD_COPY                  	0x18    //!<  OOOOOOOO
#define SCSI_CMD_ERASE                 	0x19    //!<  VMVVVV
#define SCSI_CMD_MODE_SENSE            	0x1a    //!<  OMO OOOOOO
#define SCSI_CMD_START_STOP            	0x1b    //!<  O   OO O
#define SCSI_CMD_RECEIVE_DIAGNOSTIC    	0x1c    //!<  OOOOOOOOOO
#define SCSI_CMD_SEND_DIAGNOSTIC       	0x1d    //!<  MMMMMMMMMM
#define SCSI_CMD_ALLOW_MEDIUM_REMOVAL  	0x1e    //!<  OO  OO OO

/// Group 1 (10 bytes)
#define SCSI_CMD_SET_WINDOW            	0x24    //!<  V   VVM
#define SCSI_CMD_READ_CAPACITY         	0x25    //!<  M   M  M
#define SCSI_CMD_READ_10               	0x28    //!<  M   MMMM
#define SCSI_CMD_WRITE_10              	0x2a    //!<  M   M  M
#define SCSI_CMD_SEEK_10               	0x2b    //!<  O   OO O
#define SCSI_CMD_WRITE_VERIFY          	0x2e    //!<  O   O  O
#define SCSI_CMD_VERIFY                	0x2f    //!<  O   OO O
#define SCSI_CMD_SEARCH_HIGH           	0x30    //!<  O   OO O
#define SCSI_CMD_SEARCH_EQUAL          	0x31    //!<  O   OO O
#define SCSI_CMD_SEARCH_LOW            	0x32    //!<  O   OO O
#define SCSI_CMD_SET_LIMITS            	0x33    //!<  O   OO O
#define SCSI_CMD_PRE_FETCH             	0x34    //!<  O   OO O
#define SCSI_CMD_SYNCHRONIZE_CACHE     	0x35    //!<  O   OO O
#define SCSI_CMD_LOCK_UNLOCK_CACHE     	0x36    //!<  O   OO O
#define SCSI_CMD_READ_DEFECT_DATA      	0x37    //!<  O      O
#define SCSI_CMD_MEDIUM_SCAN           	0x38    //!<      O  O
#define SCSI_CMD_COMPARE               	0x39    //!<  OOOOOOOO
#define SCSI_CMD_COPY_VERIFY           	0x3a    //!<  OOOOOOOO
#define SCSI_CMD_WRITE_BUFFER          	0x3b    //!<  OOOOOOOOOO
#define SCSI_CMD_READ_BUFFER           	0x3c    //!<  OOOOOOOOOO
#define SCSI_CMD_UPDATE_BLOCK          	0x3d    //!<      O  O
#define SCSI_CMD_READ_LONG             	0x3e    //!<  O   OO O
#define SCSI_CMD_WRITE_LONG            	0x3f    //!<  O   O  O

/// Group 2 (10 bytes)                          //!<
#define SCSI_CMD_CHANGE_DEFINITION     	0x40    //!<  OOOOOOOOOO
#define SCSI_CMD_WRITE_SAME            	0x41    //!<  O
#define SCSI_CMD_READ_TOC              	0x43    //!<       O
#define SCSI_CMD_LOG_SELECT            	0x4c    //!<  OOOOOOOOOO
#define SCSI_CMD_LOG_SENSE             	0x4d    //!<  OOOOOOOOOO
#define SCSI_CMD_MODE_SELECT_10        	0x55    //!<  OOO OOOOOO
#define SCSI_CMD_RESERVE_10            	0x56    //!<
#define SCSI_CMD_RELEASE_10            	0x57    //!<
#define SCSI_CMD_MODE_SENSE_10         	0x5a    //!<  OOO OOOOOO
#define SCSI_CMD_PERSISTENT_RESERVE_IN 	0x5e    //!<
#define SCSI_CMD_PERSISTENT_RESERVE_OUT	0x5f    //!<

/// Group 5 (12 bytes)
#define SCSI_CMD_MOVE_MEDIUM           	0xa5    //!<
#define SCSI_CMD_READ_12               	0xa8    //!<      OO O
#define SCSI_CMD_WRITE_12              	0xaa    //!<      O  O
#define SCSI_CMD_WRITE_VERIFY_12       	0xae    //!<      O  O
#define SCSI_CMD_SEARCH_HIGH_12        	0xb0    //!<      OO O
#define SCSI_CMD_SEARCH_EQUAL_12       	0xb1    //!<      OO O
#define SCSI_CMD_SEARCH_LOW_12         	0xb2    //!<      OO O
#define SCSI_CMD_SEND_VOLUME_TAG       	0xb6    //!<          O
#define SCSI_CMD_READ_ELEMENT_STATUS   	0xb8    //!<          O

/// Group 7 (vendor specific)
#define SCSI_CMD_WRITE_LONG_2          	0xea    //!<


/// SCSI STATUS CODE
#define SCSI_STATUS_GOOD						0x00
#define SCSI_STATUS_CHECK_CONDITION	 			0x02
#define SCSI_STATUS_CONDITION_MET	    		0x04
#define SCSI_STATUS_BUSY						0x08
#define SCSI_STATUS_INTERMEDIATE				0x10  //(obsolete)
#define SCSI_STATUS_INTERMEDIATE_CONDITION_MET 	0x14  //(obsolete)
#define SCSI_STATUS_RESERVATION_CONFLICT		0x18
#define SCSI_STATUS_COMMAND_TERMINATED 			0x22  //(obsolete)
#define SCSI_STATUS_TASK_SET_FULL				0x28
#define SCSI_STATUS_ACA_ACTIVE					0x30
#define SCSI_STATUS_TASK_ABORTED				0x40

/// SCSI_CMD_TEST_UNIT_READY		0x00
struct scsi_cmd_test_unit_ready_t
{
	uint8_t opcode;
	uint8_t reserved[4];
	uint8_t control;
} __attribute__ ((packed)); // GCC

#define SCSI_TRLEN_TEST_UNIT_READY		0

/// SCSI_CMD_REQUEST_SENSE			0x03
struct scsi_cmd_request_sense_t
{
	uint8_t opcode;
	uint8_t lun;            ///< Lun (High 3 bits)
	uint8_t reserved0[2];
	uint8_t alloc_len;       ///< Allocation length
	uint8_t reserved1;
	uint8_t pad[6];
} __attribute__ ((packed)); // GCC

struct scsi_data_request_sense_t
{
	uint8_t error_code;
	uint8_t reserved0;
	uint8_t sense_key;       ///< Sense key (low 4 bits)
	uint8_t information[4];
	uint8_t add_len;         ///< Additional Sense length, 10
	uint8_t reserved1[4];
	uint8_t asc;            ///< Additional Sense Code
	uint8_t ascq;           ///< Additional Sense Code Qualifier
	uint8_t reserverd2[4];
} __attribute__ ((packed)); // GCC

// Convert a LUN number to that in the command
#define USB_BOOT_LUN(Lun) ((Lun) << 5)

//
// The Sense Key part of the sense data. Sense data has three levels:
// Sense key, Additional Sense Code and Additional Sense Code Qualifier
#define USB_BOOT_SENSE_NO_SENSE         0x00 ///< No sense key
#define USB_BOOT_SENSE_RECOVERED        0x01 ///< Last command succeed with recovery actions
#define USB_BOOT_SENSE_NOT_READY        0x02 ///< Device not ready
#define USB_BOOT_SNESE_MEDIUM_ERROR     0X03 ///< Failed probably because flaw in the media
#define USB_BOOT_SENSE_HARDWARE_ERROR   0X04 ///< Non-recoverable hardware failure
#define USB_BOOT_SENSE_ILLEGAL_REQUEST  0X05 ///< Illegal parameters in the request
#define USB_BOOT_SENSE_UNIT_ATTENTION   0X06 ///< Removable medium may have been changed
#define USB_BOOT_SENSE_DATA_PROTECT     0X07 ///< Write protected
#define USB_BOOT_SENSE_BLANK_CHECK      0X08 ///< Blank/non-blank medium while reading/writing
#define USB_BOOT_SENSE_VENDOR           0X09 ///< Vendor specific sense key
#define USB_BOOT_SENSE_ABORTED          0X0B ///< Command aborted by the device
#define USB_BOOT_SENSE_VOLUME_OVERFLOW  0x0D ///< Partition overflow
#define USB_BOOT_SENSE_MISCOMPARE       0x0E ///< Source data mis-match while verfying.

#define USB_BOOT_ASC_NO_ADDITIONAL_SENSE_INFORMATION  0x00
#define USB_BOOT_ASC_NOT_READY                        0x04
#define USB_BOOT_ASC_NO_MEDIA                         0x3A
#define USB_BOOT_ASC_MEDIA_CHANGE                     0x28

#define SCSI_TRLEN_REQUEST_SENSE		18

/// SCSI_CMD_INQUIRY				0x12
struct scsi_cmd_inquiry_t
{
	uint8_t opcode;
	uint8_t evpd;
	uint8_t page_code;
	uint16_t allocation_length;		//!<  (MSB)
	uint8_t control;
} __attribute__ ((packed));

struct scsi_data_inquiry_t
{
	uint8_t qualifier_type;     //!< 0: Bits 5-7: Peripheral qualifier; Bits 0-4: Peripheral device type
	uint8_t rmb;        		//!< 1: Removable Media bit
	uint8_t version;       		//!< 2: Version
	uint8_t flags3;     		//!< 3: Bit 5 NORMACA, Bit 4 HISP, Bits 0-3 Response data format
	uint8_t len;           		//!< 4: Additional length
	uint8_t flags5;      		//!< 5: 7: SCCS; 6: ACC; 4-5 TPGS; 3: 3PC; 0: PROTECT
	uint8_t flags6;  			//!< 6: 6: ENCSERV; 5: VS; 4: MULTIPl 3: MCHNGR; 0: ADDR16
	uint8_t flags7;       		//!< 7
	uint8_t vendorid[8];   		//!< 8-15: T10 Vendor Identification
	uint8_t productid[16]; 		//!< 16-31: Product Identification
	uint8_t revision[4];   		//!< 32-35: Product Revision Level
} __attribute__ ((packed));

#define SCSI_TRLEN_INQUIRY				36

/// SCSI_CMD_READ_CAPACITY			0x25
struct scsi_cmd_read_capacity_t
{
	uint8_t opcode;
	uint8_t reserved;
	uint32_t lba;					//!< Logical Block Address (MSB)  (shall be set to zero if the PMI bit is set to zero)
	uint8_t reserved2[2];
	uint8_t pmi;
	uint8_t control;
} __attribute__ ((packed));

struct scsi_data_read_capacity_t
{
	uint32_t lba;        			//!< Returned Logical Block Address (MSB)
	uint32_t block_len;   			//!< BLOCK LENGTH IN BYTES (MSB)
} __attribute__ ((packed));

#define SCSI_TRLEN_READ_CAPACITY		8

/// SCSI_CMD_READ_10				0x28
struct scsi_cmd_read_10_t
{
	uint8_t opcode;
	uint8_t flags1;
	uint32_t lba;        		//!< Logical Block Address (MSB)
	uint8_t group;
	uint16_t blocks;  			//!<  Transfer length (in contiguous logical blocks) (MSB)
	uint8_t control;
} __attribute__ ((packed));

/// SCSI_CMD_WRITE_10				0x2A
struct scsi_cmd_write_10_t
{
	uint8_t opcode;
	uint8_t flags1;
	uint32_t lba;       		//!< Logical Block Address (MSB)
	uint8_t group;
	uint16_t blocks;
	uint8_t control;
} __attribute__ ((packed));


#endif /* SERVICES_USB_REQUESTS_SCSI_H_ */
