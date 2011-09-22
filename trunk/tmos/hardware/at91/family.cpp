/**************************************************************************//**
 * @ingroup	 hardware_at91
 * @file	 hardware/at91/family.cpp
 * @brief    AT91 family functions
 * @version  V3.00
 * @date     08. December 2010
 * @author	 Miroslav Kostadinov
 *
 */

#include <tmos.h>

/**
 * Returns divider for desired baudrate
 *
 * @param baudrate
 * @return
 */
extern "C" unsigned int AT91_GetDiv(unsigned int baudrate)
{
	return ((((system_clock_frequency * 10) / (baudrate * 16)) + 5) / 10);
}


