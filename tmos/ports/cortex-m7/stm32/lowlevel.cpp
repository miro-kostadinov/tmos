/**************************************************************************//**
 * @ingroup	 ports_cortex
 * @file
 * @brief    LowLevel init for STM32
 *
 */


#include <tmos.h>
#include <fam_cpp.h>

/**
 *  Low Level init for STM32
 */
WEAK_C void LowLevelInit( void )
{

}

/**
 * Low level peripheral reset
 * Can be replaced by app
 */
WEAK_C void LowLevelReset( void)
{
}

/**
 * Reboots the device
 * Can be replaced by app
 */
WEAK_C void LowLevelReboot( void)
{
}

/**
 * Reset cause
 * @return device specific value for the cause
 */
WEAK_C unsigned int LowLevelResetCause(void)
{
	unsigned int res;

	res = RCC->RCC_RSR;
	RCC->RCC_RSR = RCC_RSR_RMVF;
	RCC->RCC_RSR = 0;
	return (res);
}
