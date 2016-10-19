/**************************************************************************//**
 * @ingroup ports
 * @{
 * @file     tmos_cfg.h
 * @brief    TMOS configuration header
 * @version  V3.00
 * @date     11. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/
#ifndef TMOS_CFG_H_
#define TMOS_CFG_H_

// collect all configuration

#include <brd_cfg.h>

#ifndef USE_TASK_REGISTERS
#define USE_TASK_REGISTERS 1
#endif

#ifndef USE_STATIC_CONSTRUCTORS
#define USE_STATIC_CONSTRUCTORS 1  //enabled by default
#endif

#ifndef USE_CPU_SLEEP_MODE
#define USE_CPU_SLEEP_MODE 0  	  //disabled by default
#endif

/// By default use Fixed code page (cp1251)
#ifndef USE_FIXED_CPAGE
#define USE_FIXED_CPAGE 1251
#endif

#endif /* TMOS_CFG_H_ */
/** @} ingroup ports */
