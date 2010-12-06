/**************************************************************************//**
 * @file     hardware/at91/cortex-m3/sam3s/series_inc.h
 * @ingroup	 hardware_sam3s
 * @brief    SAM3S series CPP header
 * @version  V3.00
 * @date     11. November 2010
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_sam3s SAM3S series
 * @ingroup	 at91_cortex_m3
 * Source and definitions for SAM3S series mcu's
 *
 *
 ******************************************************************************/

#ifndef SERIES_INC_H_
#define SERIES_INC_H_

#include <mcu_inc.h>

/** CFG_SAM3S flag will be used for conditional compilation */
#define CFG_SAM3S 	1

/**
 * @ingroup hardware_sam3s
 * @addtogroup CMSIS_CM3_core_definitions CM3 Core Definitions
 * This file defines all structures and symbols for CMSIS core:
 *  - CMSIS version number
 *  - Cortex-M core registers and bitfields
 *  - Cortex-M core peripheral base address
 * @{
 */

#define __CM3_CMSIS_VERSION_MAIN  (0x01)  //!< [31:16] CMSIS HAL main version
#define __CM3_CMSIS_VERSION_SUB   (0x30)  //!< [15:0]  CMSIS HAL sub version
#define __CORTEX_M                (0x03)  //!< Cortex core

#define __MPU_PRESENT  		1 //!< SAM3S provides a MPU
#define __NVIC_PRIO_BITS    4 //!< SAM3S uses 4 Bits for the Priority Levels
/**	 @} end of addtogroup CMSIS_CM3_core_definitions	*/

/** The IAP function entry address for SAM3S*/
#define CHIP_FLASH_IAP_ADDRESS  (0x00800008)

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR SAM3S */
/* ************************************************************************** */

#define IFLASH_ADDR  (0x00400000u) /**< Internal Flash base address */
#define IROM_ADDR    (0x00800000u) /**< Internal ROM base address */
#define IRAM_ADDR    (0x20000000u) /**< Internal RAM base address */
#define EBI_CS0_ADDR (0x60000000u) /**< EBI Chip Select 0 base address */
#define EBI_CS1_ADDR (0x61000000u) /**< EBI Chip Select 1 base address */
#define EBI_CS2_ADDR (0x62000000u) /**< EBI Chip Select 2 base address */
#define EBI_CS3_ADDR (0x63000000u) /**< EBI Chip Select 3 base address */

#if defined sam3s1
#define IFLASH_SIZE 0x10000
#define IFLASH_PAGE_SIZE              (256) /* Internal FLASH 0 Page Size: 256 bytes */
#define IFLASH_LOCK_REGION_SIZE     (16384) /* Internal FLASH 0 Lock Region Size: 16 Kbytes */
#define IFLASH_NB_OF_PAGES            (256) /* Internal FLASH 0 Number of Pages: 256 */
#define IFLASH_NB_OF_LOCK_BITS          (4) /* Internal FLASH 0 Number of Lock Bits: 4 */
#define IRAM_SIZE 0x4000
#elif defined sam3s2
#define IFLASH_SIZE 0x20000
#define IFLASH_PAGE_SIZE              (256) /* Internal FLASH 0 Page Size: 256 bytes */
#define IFLASH_LOCK_REGION_SIZE     (16384) /* Internal FLASH 0 Lock Region Size: 16 Kbytes */
#define IFLASH_NB_OF_PAGES            (512) /* Internal FLASH 0 Number of Pages: 512 */
#define IFLASH_NB_OF_LOCK_BITS          (8) /* Internal FLASH 0 Number of Lock Bits: 8 */
#define IRAM_SIZE 0x8000
#elif defined sam3s4
#define IFLASH_SIZE 0x40000
#define IFLASH_PAGE_SIZE              (256) /* Internal FLASH 0 Page Size: 256 bytes */
#define IFLASH_LOCK_REGION_SIZE     (16384) /* Internal FLASH 0 Lock Region Size: 16 Kbytes */
#define IFLASH_NB_OF_PAGES           (1024) /* Internal FLASH 0 Number of Pages: 1024 */
#define IFLASH_NB_OF_LOCK_BITS         (16) /* Internal FLASH 0 Number of Lock Bits: 16 */
#define IRAM_SIZE 0xC000
#else
  #error Library does not support the specified device.
#endif


#endif /* SERIES_INC_H_ */
