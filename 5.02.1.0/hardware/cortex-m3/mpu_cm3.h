/*
 * mpu_cm3.h
 *
 *  Created on: Dec 18, 2013
 *      Author: miro
 */

#ifndef MPU_CM3_H_
#define MPU_CM3_H_

#include <tmos_types.h>

/*******************************************************************************
 *  Memory Protection Unit (MPU)
 ******************************************************************************/
#ifndef __ASSEMBLY__
struct MPU_Type
{
  __I  uint32_t MPU_TYPE;	//!< (MPU Offset: 0x00) MPU Type Register                              */
  __IO uint32_t MPU_CTRL;   //!< (MPU Offset: 0x04) MPU Control Register                           */
  __IO uint32_t MPU_RNR;    //!< (MPU Offset: 0x08) MPU Region Number Register                     */
  __IO uint32_t MPU_RBAR;   //!< (MPU Offset: 0x0C) MPU Region Base Address Register               */
  __IO uint32_t MPU_RASR;   //!< (MPU Offset: 0x10) MPU Region Attribute and Size Register         */
  __IO uint32_t MPU_RBAR_A1;//!< (MPU Offset: 0x14) MPU Alias 1 Region Base Address Register       */
  __IO uint32_t MPU_RASR_A1;//!< (MPU Offset: 0x18) MPU Alias 1 Region Attribute and Size Register */
  __IO uint32_t MPU_RBAR_A2;//!< (MPU Offset: 0x1C) MPU Alias 2 Region Base Address Register       */
  __IO uint32_t MPU_RASR_A2;//!< (MPU Offset: 0x20) MPU Alias 2 Region Attribute and Size Register */
  __IO uint32_t MPU_RBAR_A3;//!< (MPU Offset: 0x24) MPU Alias 3 Region Base Address Register       */
  __IO uint32_t MPU_RASR_A3;//!< (MPU Offset: 0x28) MPU Alias 3 Region Attribute and Size Register */
};
#endif /* __ASSEMBLY__ */

/*******************************************************************************
 * @defgroup MPU_regs_define
 * @{
 ******************************************************************************/

/** @defgroup MPU_TYPE:	   (MPU Offset: 0x00) MPU Type Register                              */
#define MPU_TYPE_IREGION 		   0x00FF0000 //!< Number of supported MPU instruction regions. Always contains 0x00. The MPU memory map is unified and is described by the DREGION field
#define MPU_TYPE_DREGION 		   0x0000FF00 //!< Number of supported MPU data regions: 0x08 = eight MPU regions
#define MPU_TYPE_SEPARATE  		   0x00000001 //!< Indicates support for unified or separate instruction and date memory maps: 0 = unified
/** @} */

/** @defgroup MPU_CTRL:    (MPU Offset: 0x04) MPU Control Register                           */
#define MPU_CTRL_PRIVDEFENA 	   0x00000003 //!< Enables privileged software access to the default memory map
#define MPU_CTRL_HFNMIENA  		   0x00000002 //!< Enables the operation of MPU during hard fault, NMI, and FAULTMASK handlers
#define MPU_CTRL_ENABLE   		   0x00000001 //!< Enables the MPU
/** @} */

/** @defgroup MPU_RNR:     (MPU Offset: 0x08) MPU Region Number Register                     */
#define MPU_RNR_REGION    		   0x000000FF //!< Indicates the MPU region referenced by the MPU_RBAR and MPU_RASR registers
/** @} */

/** @defgroup MPU_RBAR:    (MPU Offset: 0x0C) MPU Region Base Address Register               */
#define MPU_RBAR_ADDR     		   0xFFFFFFE0 //!< Region base address
#define MPU_RBAR_VALID     		   0x00000010 //!< MPU Region Number valid bit
#define MPU_RBAR_REGION    		   0x0000000F //!< MPU region field
/** @} */

/** @defgroup MPU_RASR:    (MPU Offset: 0x10) MPU Region Attribute and Size Register         */
#define MPU_RASR_XN 	   		   0x10000000 //!< Instruction access disable bit
#define MPU_RASR_AP 	   		   0x07000000 //!< Access permission field
#define MPU_RASR_AP_PRV_NO_USR_NO  0x00000000 //!<  All accesses generate a permission fault
#define MPU_RASR_AP_PRV_RW_USR_NO  0x01000000 //!<  Access from privileged software only
#define MPU_RASR_AP_PRV_RW_USR_RO  0x02000000 //!<  Writes by unprivileged software generate a permission fault
#define MPU_RASR_AP_PRV_RW_USR_RW  0x03000000 //!<  Full access
#define MPU_RASR_AP_PRV_RO_USR_NO  0x05000000 //!<  Reads by privileged software only
#define MPU_RASR_AP_PRV_RO_USR_RO  0x06000000 //!<  Read only, by privileged or unprivileged software
#define MPU_RASR_TEX 	   		   0x00380000 //!< TEXT Memory access attributes
#define MPU_RASR_S	 	   		   0x00040000 //!< Shareable bit
#define MPU_RASR_C	 	   		   0x00020000 //!< C Memory access attributes
#define MPU_RASR_B	 	   		   0x00010000 //!< B Memory access attributes
#define MPU_RASR_TEXCB    		   0x003B0000 //!< TEX,C,B Memory access attributes
#define MPU_RASR_SRD    		   0x0000FF00 //!< Subregion disable bits
#define MPU_RASR_SIZE     		   0x0000003E //!< Specifies the size of the MPU protection region. Region size in bytes = 2^(SIZE+1)
#define MPU_RASR_SIZE_32B          0x00000008 //!< 	32B
#define MPU_RASR_SIZE_64B          0x0000000A //!<  64B
#define MPU_RASR_SIZE_128B         0x0000000C //!<  128B
#define MPU_RASR_SIZE_256B         0x0000000E //!<  256B
#define MPU_RASR_SIZE_512B         0x00000010 //!<  512B
#define MPU_RASR_SIZE_1K           0x00000012 //!<  1K
#define MPU_RASR_SIZE_2K           0x00000014 //!<  2K
#define MPU_RASR_SIZE_4K           0x00000016 //!<  4K
#define MPU_RASR_SIZE_8K           0x00000018 //!<  8K
#define MPU_RASR_SIZE_16K          0x0000001A //!<  16K
#define MPU_RASR_SIZE_32K          0x0000001C //!<  32K
#define MPU_RASR_SIZE_64K          0x0000001E //!<  64K
#define MPU_RASR_SIZE_128K         0x00000020 //!<  128K
#define MPU_RASR_SIZE_256K         0x00000022 //!<  256K
#define MPU_RASR_SIZE_512K         0x00000024 //!<  512K
#define MPU_RASR_SIZE_1M           0x00000026 //!<  1M
#define MPU_RASR_SIZE_2M           0x00000028 //!<  2M
#define MPU_RASR_SIZE_4M           0x0000002A //!<  4M
#define MPU_RASR_SIZE_8M           0x0000002C //!<  8M
#define MPU_RASR_SIZE_16M          0x0000002E //!<  16M
#define MPU_RASR_SIZE_32M          0x00000030 //!<  32M
#define MPU_RASR_SIZE_64M          0x00000032 //!<  64M
#define MPU_RASR_SIZE_128M         0x00000034 //!<  128M
#define MPU_RASR_SIZE_256M         0x00000036 //!<  256M
#define MPU_RASR_SIZE_512M         0x00000038 //!<  512M
#define MPU_RASR_SIZE_1G           0x0000003A //!<  1G
#define MPU_RASR_SIZE_2G           0x0000003C //!<  2G
#define MPU_RASR_SIZE_4G           0x0000003E //!<  4G
#define MPU_RASR_ENABLE    		   0x00000001 //!< Region enable bit

/** @} */

/** @defgroup MPU_RBAR_A1: (MPU Offset: 0x14) MPU Alias 1 Region Base Address Register       */
// see MPU_RBAR register
/** @} */

/** @defgroup MPU_RASR_A1: (MPU Offset: 0x18) MPU Alias 1 Region Attribute and Size Register */
// see MPU_RASR register
/** @} */

/** @defgroup MPU_RBAR_A2: (MPU Offset: 0x1C) MPU Alias 2 Region Base Address Register       */
// see MPU_RBAR register
/** @} */

/** @defgroup MPU_RASR_A2: (MPU Offset: 0x20) MPU Alias 2 Region Attribute and Size Register */
// see MPU_RASR register
/** @} */

/** @defgroup MPU_RBAR_A3: (MPU Offset: 0x24) MPU Alias 3 Region Base Address Register       */
// see MPU_RBAR register
/** @} */

/** @defgroup MPU_RASR_A3: (MPU Offset: 0x28) MPU Alias 3 Region Attribute and Size Register */
// see MPU_RASR register
/** @} */

/** @} */ // @defgroup MPU_regs_define



#endif /* MPU_CM3_H_ */
