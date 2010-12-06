/**
 * @file: mpu_cm3.cpp
 *
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 *
 * license: check README.TXT
 * @date 2010-4-13
 *
 */

#include <tmos.h>
#include "cmsis_cpp.h"

//*****************************************************************************
//
//! Enables and configures the MPU for use.
//!
//! \param ulMPUConfig is the logical OR of the possible configurations.
//!
//! This function enables the Cortex-M3 memory protection unit.  It also
//! configures the default behavior when in privileged mode and while
//! handling a hard fault or NMI.  Prior to enabling the MPU, at least one
//! region must be set by calling MPURegionSet() or else by enabling the
//! default region for privileged mode by passing the
//! \b MPU_CONFIG_PRIV_DEFAULT flag to MPUEnable().
//! Once the MPU is enabled, a memory management fault will be generated
//! for any memory access violations.
//!
//! The \e ulMPUConfig parameter should be the logical OR of any of the
//! following:
//!
//! - \b MPU_CONFIG_PRIV_DEFAULT enables the default memory map when in
//! privileged mode and when no other regions are defined.  If this option
//! is not enabled, then there must be at least one valid region already
//! defined when the MPU is enabled.
//! - \b MPU_CONFIG_HARDFLT_NMI enables the MPU while in a hard fault or NMI
//! exception handler.  If this option is not enabled, then the MPU is
//! disabled while in one of these exception handlers and the default
//! memory map is applied.
//! - \b MPU_CONFIG_NONE chooses none of the above options.  In this case,
//! no default memory map is provided in privileged mode, and the MPU will
//! not be enabled in the fault handlers.
//!
//! \return None.
//
//*****************************************************************************
void MPU_Type::Enable(unsigned long ulMPUConfig)
{
    ASSERT(!(ulMPUConfig & ~(MPU_CONFIG_PRIV_DEFAULT | MPU_CONFIG_HARDFLT_NMI)));
    // Set the MPU control bits according to the flags passed by the user, and also set the enable bit.
    this->CTRL = ulMPUConfig | NVIC_MPU_CTRL_ENABLE;
}

//*****************************************************************************
//
//! Disables the MPU for use.
//!
//! This function disables the Cortex-M3 memory protection unit.  When the
//! MPU is disabled, the default memory map is used and memory management
//! faults are not generated.
//!
//! \return None.
//
//*****************************************************************************
void MPU_Type::Disable(void)
{
    this->CTRL &= ~NVIC_MPU_CTRL_ENABLE;
}

//*****************************************************************************
//
//! Gets the count of regions supported by the MPU.
//!
//! This function is used to get the number of regions that are supported by
//! the MPU.  This is the total number that are supported, including regions
//! that are already programmed.
//!
//! \return The number of memory protection regions that are available
//! for programming using MPURegionSet().
//
//*****************************************************************************
unsigned long MPU_Type::RegionCountGet(void)
{
    // Read the DREGION field of the MPU type register, and mask off
    // the bits of interest to get the count of regions.
    return((this->TYPE & NVIC_MPU_TYPE_DREGION_M) >> NVIC_MPU_TYPE_DREGION_S);
}

//*****************************************************************************
//
//! Enables a specific region.
//!
//! \param ulRegion is the region number to enable.
//!
//! This function is used to enable a memory protection region.  The region
//! should already be set up with the MPURegionSet() function.  Once enabled,
//! the memory protection rules of the region will be applied and access
//! violations will cause a memory management fault.
//!
//! \return None.
//
//*****************************************************************************
void MPU_Type::RegionEnable(unsigned long ulRegion)
{
    // Check the arguments.
    ASSERT(ulRegion < 8);
    // Select the region to modify.
    this->RNR = ulRegion;

    // Modify the enable bit in the region attributes.
    this->RASR |= NVIC_MPU_ATTR_ENABLE;
}

//*****************************************************************************
//
//! Disables a specific region.
//!
//! \param ulRegion is the region number to disable.
//!
//! This function is used to disable a previously enabled memory protection
//! region.  The region will remain configured if it is not overwritten with
//! another call to MPURegionSet(), and can be enabled again by calling
//! MPURegionEnable().
//!
//! \return None.
//
//*****************************************************************************
void MPU_Type::RegionDisable(unsigned long ulRegion)
{
    // Check the arguments.
    ASSERT(ulRegion < 8);
    // Select the region to modify.
    this->RNR = ulRegion;
    // Modify the enable bit in the region attributes.
    this->RASR &= ~NVIC_MPU_ATTR_ENABLE;
}

//*****************************************************************************
//
//! Sets up the access rules for a specific region.
//!
//! \param ulRegion is the region number to set up.
//! \param ulAddr is the base address of the region.  It must be aligned
//! according to the size of the region specified in ulFlags.
//! \param ulFlags is a set of flags to define the attributes of the region.
//!
//! This function sets up the protection rules for a region.  The region has
//! a base address and a set of attributes including the size, which must
//! be a power of 2.  The base address parameter, \e ulAddr, must be aligned
//! according to the size.
//!
//! The \e ulFlags parameter is the logical OR of all of the attributes
//! of the region.  It is a combination of choices for region size,
//! execute permission, read/write permissions, disabled sub-regions,
//! and a flag to determine if the region is enabled.
//!
//! The size flag determines the size of a region, and must be one of the
//! following:
//!
//! - \b MPU_RGN_SIZE_32B
//! - \b MPU_RGN_SIZE_64B
//! - \b MPU_RGN_SIZE_128B
//! - \b MPU_RGN_SIZE_256B
//! - \b MPU_RGN_SIZE_512B
//! - \b MPU_RGN_SIZE_1K
//! - \b MPU_RGN_SIZE_2K
//! - \b MPU_RGN_SIZE_4K
//! - \b MPU_RGN_SIZE_8K
//! - \b MPU_RGN_SIZE_16K
//! - \b MPU_RGN_SIZE_32K
//! - \b MPU_RGN_SIZE_64K
//! - \b MPU_RGN_SIZE_128K
//! - \b MPU_RGN_SIZE_256K
//! - \b MPU_RGN_SIZE_512K
//! - \b MPU_RGN_SIZE_1M
//! - \b MPU_RGN_SIZE_2M
//! - \b MPU_RGN_SIZE_4M
//! - \b MPU_RGN_SIZE_8M
//! - \b MPU_RGN_SIZE_16M
//! - \b MPU_RGN_SIZE_32M
//! - \b MPU_RGN_SIZE_64M
//! - \b MPU_RGN_SIZE_128M
//! - \b MPU_RGN_SIZE_256M
//! - \b MPU_RGN_SIZE_512M
//! - \b MPU_RGN_SIZE_1G
//! - \b MPU_RGN_SIZE_2G
//! - \b MPU_RGN_SIZE_4G
//!
//! The execute permission flag must be one of the following:
//!
//! - \b MPU_RGN_PERM_EXEC enables the region for execution of code
//! - \b MPU_RGN_PERM_NOEXEC disables the region for execution of code
//!
//! The read/write access permissions are applied separately for the
//! privileged and user modes.  The read/write access flags must be one
//! of the following:
//!
//! - \b MPU_RGN_PERM_PRV_NO_USR_NO - no access in privileged or user mode
//! - \b MPU_RGN_PERM_PRV_RW_USR_NO - privileged read/write, user no access
//! - \b MPU_RGN_PERM_PRV_RW_USR_RO - privileged read/write, user read-only
//! - \b MPU_RGN_PERM_PRV_RW_USR_RW - privileged read/write, user read/write
//! - \b MPU_RGN_PERM_PRV_RO_USR_NO - privileged read-only, user no access
//! - \b MPU_RGN_PERM_PRV_RO_USR_RO - privileged read-only, user read-only
//!
//! The region is automatically divided into 8 equally-sized sub-regions by
//! the MPU.  Sub-regions can only be used in regions of size 256 bytes
//! or larger.  Any of these 8 sub-regions can be disabled.  This allows
//! for creation of ``holes'' in a region which can be left open, or overlaid
//! by another region with different attributes.  Any of the 8 sub-regions
//! can be disabled with a logical OR of any of the following flags:
//!
//! - \b MPU_SUB_RGN_DISABLE_0
//! - \b MPU_SUB_RGN_DISABLE_1
//! - \b MPU_SUB_RGN_DISABLE_2
//! - \b MPU_SUB_RGN_DISABLE_3
//! - \b MPU_SUB_RGN_DISABLE_4
//! - \b MPU_SUB_RGN_DISABLE_5
//! - \b MPU_SUB_RGN_DISABLE_6
//! - \b MPU_SUB_RGN_DISABLE_7
//!
//! Finally, the region can be initially enabled or disabled with one of
//! the following flags:
//!
//! - \b MPU_RGN_ENABLE
//! - \b MPU_RGN_DISABLE
//!
//! As an example, to set a region with the following attributes: size of
//! 32 KB, execution enabled, read-only for both privileged and user, one
//! sub-region disabled, and initially enabled; the \e ulFlags parameter would
//! have the following value:
//!
//! <code>
//! (MPU_RG_SIZE_32K | MPU_RGN_PERM_EXEC | MPU_RGN_PERM_PRV_RO_USR_RO |
//!  MPU_SUB_RGN_DISABLE_2 | MPU_RGN_ENABLE)
//! </code>
//!
//! \note This function will write to multiple registers and is not protected
//! from interrupts.  It is possible that an interrupt which accesses a
//! region may occur while that region is in the process of being changed.
//! The safest way to handle this is to disable a region before changing it.
//! Refer to the discussion of this in the API Detailed Description section.
//!
//! \return None.
//
//*****************************************************************************
void MPU_Type::RegionSet(unsigned long ulRegion, unsigned long ulAddr, unsigned long ulFlags)
{
    // Check the arguments.
    ASSERT(ulRegion < 8);
    ASSERT((ulAddr & ~0 << (((ulFlags & NVIC_MPU_ATTR_SIZE_M) >> 1) + 1)) == ulAddr);
    // Program the base address, use the region field to select the region at the same time.
    this->RBAR = ulAddr | ulRegion | NVIC_MPU_BASE_VALID;
    // Program the region attributes.  Set the TEX field and the S, C,
    // and B bits to fixed values that are suitable for all Stellaris memory.
    this->RASR = (ulFlags & ~(NVIC_MPU_ATTR_TEX_M |NVIC_MPU_ATTR_CACHEABLE)) |
                              NVIC_MPU_ATTR_SHAREABLE |
                              NVIC_MPU_ATTR_BUFFRABLE;
}

//*****************************************************************************
//
//! Gets the current settings for a specific region.
//!
//! \param ulRegion is the region number to get.
//! \param pulAddr points to storage for the base address of the region.
//! \param pulFlags points to the attribute flags for the region.
//!
//! This function retrieves the configuration of a specific region.  The
//! meanings and format of the parameters is the same as that of the
//! MPURegionSet() function.
//!
//! This function can be used to save the configuration of a region for
//! later use with the MPURegionSet() function.  The region's enable state
//! will be preserved in the attributes that are saved.
//!
//! \return None.
//
//*****************************************************************************
void MPU_Type::RegionGet(unsigned long ulRegion, unsigned long *pulAddr, unsigned long *pulFlags)
{
    // Check the arguments.
    ASSERT(ulRegion < 8); ASSERT(pulAddr); ASSERT(pulFlags);
    // Select the region to get.
    this->RNR = ulRegion;
    // Read and store the base address for the region.
    *pulAddr = this->RBAR;
    // Read and store the region attributes.
    *pulFlags = this->RASR;
}

