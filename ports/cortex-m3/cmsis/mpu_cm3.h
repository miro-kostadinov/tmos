/**************************************************************************//**
 * @ingroup	 cores_cmsis
 * @{
 * @file     cortex-m3/cmsis/mpu_cm3.h
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 * @version  V3.00
 * @date     12. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#ifndef MPU_CM3_H_
#define MPU_CM3_H_

struct MPU_Type
{
  __I  uint32_t TYPE;	/*!< Offset: 0x00  MPU Type Register                              */
  __IO uint32_t CTRL;   /*!< Offset: 0x04  MPU Control Register                           */
  __IO uint32_t RNR;    /*!< Offset: 0x08  MPU Region RNRber Register                     */
  __IO uint32_t RBAR;   /*!< Offset: 0x0C  MPU Region Base Address Register               */
  __IO uint32_t RASR;   /*!< Offset: 0x10  MPU Region Attribute and Size Register         */
  __IO uint32_t RBAR_A1;/*!< Offset: 0x14  MPU Alias 1 Region Base Address Register       */
  __IO uint32_t RASR_A1;/*!< Offset: 0x18  MPU Alias 1 Region Attribute and Size Register */
  __IO uint32_t RBAR_A2;/*!< Offset: 0x1C  MPU Alias 2 Region Base Address Register       */
  __IO uint32_t RASR_A2;/*!< Offset: 0x20  MPU Alias 2 Region Attribute and Size Register */
  __IO uint32_t RBAR_A3;/*!< Offset: 0x24  MPU Alias 3 Region Base Address Register       */
  __IO uint32_t RASR_A3;/*!< Offset: 0x28  MPU Alias 3 Region Attribute and Size Register */

  void Enable(unsigned long ulMPUConfig);
  void Disable(void);
  unsigned long RegionCountGet(void);
  void RegionEnable(unsigned long ulRegion);
  void RegionDisable(unsigned long ulRegion);
  void RegionSet(unsigned long ulRegion, unsigned long ulAddr, unsigned long ulFlags);
  void RegionGet(unsigned long ulRegion, unsigned long *pulAddr, unsigned long *pulFlags);
};

//*****************************************************************************
//
// Flags that can be passed to MPUEnable.
//
//*****************************************************************************
#define MPU_CONFIG_PRIV_DEFAULT     4	/**< doc! */
#define MPU_CONFIG_HARDFLT_NMI      2	/**< doc! */
#define MPU_CONFIG_NONE             0	/**< doc! */

//*****************************************************************************
//
// Flags for the region size to be passed to MPURegionSet.
//
//*****************************************************************************
#define MPU_RGN_SIZE_32B            (4 << 1)	/**< doc! */
#define MPU_RGN_SIZE_64B            (5 << 1)	/**< doc! */
#define MPU_RGN_SIZE_128B           (6 << 1)	/**< doc! */
#define MPU_RGN_SIZE_256B           (7 << 1)	/**< doc! */
#define MPU_RGN_SIZE_512B           (8 << 1)	/**< doc! */

#define MPU_RGN_SIZE_1K             (9 << 1)	/**< doc! */
#define MPU_RGN_SIZE_2K             (10 << 1)	/**< doc! */
#define MPU_RGN_SIZE_4K             (11 << 1)	/**< doc! */
#define MPU_RGN_SIZE_8K             (12 << 1)	/**< doc! */
#define MPU_RGN_SIZE_16K            (13 << 1)	/**< doc! */
#define MPU_RGN_SIZE_32K            (14 << 1)	/**< doc! */
#define MPU_RGN_SIZE_64K            (15 << 1)	/**< doc! */
#define MPU_RGN_SIZE_128K           (16 << 1)	/**< doc! */
#define MPU_RGN_SIZE_256K           (17 << 1)	/**< doc! */
#define MPU_RGN_SIZE_512K           (18 << 1)	/**< doc! */

#define MPU_RGN_SIZE_1M             (19 << 1)	/**< doc! */
#define MPU_RGN_SIZE_2M             (20 << 1)	/**< doc! */
#define MPU_RGN_SIZE_4M             (21 << 1)	/**< doc! */
#define MPU_RGN_SIZE_8M             (22 << 1)	/**< doc! */
#define MPU_RGN_SIZE_16M            (23 << 1)	/**< doc! */
#define MPU_RGN_SIZE_32M            (24 << 1)	/**< doc! */
#define MPU_RGN_SIZE_64M            (25 << 1)	/**< doc! */
#define MPU_RGN_SIZE_128M           (26 << 1)	/**< doc! */
#define MPU_RGN_SIZE_256M           (27 << 1)	/**< doc! */
#define MPU_RGN_SIZE_512M           (28 << 1)	/**< doc! */

#define MPU_RGN_SIZE_1G             (29 << 1)	/**< doc! */
#define MPU_RGN_SIZE_2G             (30 << 1)	/**< doc! */
#define MPU_RGN_SIZE_4G             (31 << 1)	/**< doc! */

//*****************************************************************************
//
// Flags for the permissions to be passed to MPURegionSet.
//
//*****************************************************************************
#define MPU_RGN_PERM_EXEC           0x00000000	/**< doc! */
#define MPU_RGN_PERM_NOEXEC         0x10000000	/**< doc! */
#define MPU_RGN_PERM_PRV_NO_USR_NO  0x00000000	/**< doc! */
#define MPU_RGN_PERM_PRV_RW_USR_NO  0x01000000	/**< doc! */
#define MPU_RGN_PERM_PRV_RW_USR_RO  0x02000000	/**< doc! */
#define MPU_RGN_PERM_PRV_RW_USR_RW  0x03000000	/**< doc! */
#define MPU_RGN_PERM_PRV_RO_USR_NO  0x05000000	/**< doc! */
#define MPU_RGN_PERM_PRV_RO_USR_RO  0x06000000	/**< doc! */

//*****************************************************************************
//
// Flags for the sub-region to be passed to MPURegionSet.
//
//*****************************************************************************
#define MPU_SUB_RGN_DISABLE_0       0x00000100	/**< doc! */
#define MPU_SUB_RGN_DISABLE_1       0x00000200	/**< doc! */
#define MPU_SUB_RGN_DISABLE_2       0x00000400	/**< doc! */
#define MPU_SUB_RGN_DISABLE_3       0x00000800	/**< doc! */
#define MPU_SUB_RGN_DISABLE_4       0x00001000	/**< doc! */
#define MPU_SUB_RGN_DISABLE_5       0x00002000	/**< doc! */
#define MPU_SUB_RGN_DISABLE_6       0x00004000	/**< doc! */
#define MPU_SUB_RGN_DISABLE_7       0x00008000	/**< doc! */

//*****************************************************************************
//
// Flags to enable or disable a region, to be passed to MPURegionSet.
//
//*****************************************************************************
#define MPU_RGN_ENABLE              1	/**< doc! */
#define MPU_RGN_DISABLE             0	/**< doc! */

#define MPU	((MPU_Type *)MPU_BASE)      /*!< MPU configuration struct         */


#endif /* MPU_CM3_H_ */

/** @} ingroup cores_cmsis */
