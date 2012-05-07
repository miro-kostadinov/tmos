/**************************************************************************//**
 * @ingroup	 hardware_lm3s
 * @defgroup lm3s_types types
 * @{
 * @file     lm3s/types_lm3s.h
 * @brief    Helper Macros for determining silicon revisions, etc.
 *
 * These macros will be used by Driverlib at "run-time" to create necessary
 * conditional code blocks that will allow a single version of the Driverlib
 * "binary" code to support multiple(all) Stellaris silicon revisions.
 *
 * It is expected that these macros will be used inside of a standard 'C'
 * conditional block of code, e.g.
 * @code
 *     if(CLASS_IS_SANDSTORM)
 *     {
 *         do some Sandstorm-class specific code here.
 *     }
 * @endcode
 * By default, these macros will be defined as run-time checks of the
 * appropriate register(s) to allow creation of run-time conditional code
 * blocks for a common DriverLib across the entire Stellaris family.
 *
 * However, if code-space optimization is required, these macros can be "hard-
 * coded" for a specific version of Stellaris silicon.  Many compilers will
 * then detect the "hard-coded" conditionals, and appropriately optimize the
 * code blocks, eliminating any "unreachable" code.  This would result in
 * a smaller Driverlib, thus producing a smaller final application size, but
 * at the cost of limiting the Driverlib binary to a specific Stellaris
 * silicon revision.
 *
 ******************************************************************************/
#ifndef __HW_TYPES_H__
#define __HW_TYPES_H__

#ifndef CLASS_IS_SANDSTORM
/** mcu class */
#define CLASS_IS_SANDSTORM                                                    \
        (((SYSCTL->DID0 & SYSCTL_DID0_VER_M) == SYSCTL_DID0_VER_0) ||   \
         ((SYSCTL->DID0 & (SYSCTL_DID0_VER_M | SYSCTL_DID0_CLASS_M)) == \
          (SYSCTL_DID0_VER_1 | SYSCTL_DID0_CLASS_SANDSTORM)))
#endif

#ifndef CLASS_IS_FURY
/** mcu class */
#define CLASS_IS_FURY                                                        \
        ((SYSCTL->DID0 & (SYSCTL_DID0_VER_M | SYSCTL_DID0_CLASS_M)) == \
         (SYSCTL_DID0_VER_1 | SYSCTL_DID0_CLASS_FURY))
#endif

#ifndef CLASS_IS_DUSTDEVIL
/** mcu class */
#define CLASS_IS_DUSTDEVIL                                                   \
        ((SYSCTL->DID0 & (SYSCTL_DID0_VER_M | SYSCTL_DID0_CLASS_M)) == \
         (SYSCTL_DID0_VER_1 | SYSCTL_DID0_CLASS_DUSTDEVIL))
#endif

#ifndef CLASS_IS_TEMPEST
/** mcu class */
#define CLASS_IS_TEMPEST                                                     \
        ((SYSCTL->DID0 & (SYSCTL_DID0_VER_M | SYSCTL_DID0_CLASS_M)) == \
         (SYSCTL_DID0_VER_1 | SYSCTL_DID0_CLASS_TEMPEST))
#endif

#ifndef REVISION_IS_A0
/** mcu revision */
#define REVISION_IS_A0                                                     \
        ((SYSCTL->DID0 & (SYSCTL_DID0_MAJ_M | SYSCTL_DID0_MIN_M)) == \
         (SYSCTL_DID0_MAJ_REVA | SYSCTL_DID0_MIN_0))
#endif

#ifndef REVISION_IS_A1
/** mcu revision */
#define REVISION_IS_A1                                                     \
        ((SYSCTL->DID0 & (SYSCTL_DID0_MAJ_M | SYSCTL_DID0_MIN_M)) == \
         (SYSCTL_DID0_MAJ_REVA | SYSCTL_DID0_MIN_0))
#endif

#ifndef REVISION_IS_A2
/** mcu revision */
#define REVISION_IS_A2                                                     \
        ((SYSCTL->DID0 & (SYSCTL_DID0_MAJ_M | SYSCTL_DID0_MIN_M)) == \
         (SYSCTL_DID0_MAJ_REVA | SYSCTL_DID0_MIN_2))
#endif

#ifndef REVISION_IS_B0
/** mcu revision */
#define REVISION_IS_B0                                                     \
        ((SYSCTL->DID0 & (SYSCTL_DID0_MAJ_M | SYSCTL_DID0_MIN_M)) == \
         (SYSCTL_DID0_MAJ_REVB | SYSCTL_DID0_MIN_0))
#endif

#ifndef REVISION_IS_B1
/** mcu revision */
#define REVISION_IS_B1                                                     \
        ((SYSCTL->DID0 & (SYSCTL_DID0_MAJ_M | SYSCTL_DID0_MIN_M)) == \
         (SYSCTL_DID0_MAJ_REVB | SYSCTL_DID0_MIN_1))
#endif

#ifndef REVISION_IS_C1
/** mcu revision */
#define REVISION_IS_C1                                                     \
        ((SYSCTL->DID0 & (SYSCTL_DID0_MAJ_M | SYSCTL_DID0_MIN_M)) == \
         (SYSCTL_DID0_MAJ_REVC | SYSCTL_DID0_MIN_1))
#endif

#ifndef REVISION_IS_C2
/** mcu revision */
#define REVISION_IS_C2                                                     \
        ((SYSCTL->DID0 & (SYSCTL_DID0_MAJ_M | SYSCTL_DID0_MIN_M)) == \
         (SYSCTL_DID0_MAJ_REVC | SYSCTL_DID0_MIN_2))
#endif

#endif // __HW_TYPES_H__
/** @} defgroup lm3s_types */
