/*
 * fam_cpp.cpp
 *
 *  Created on: Nov 1, 2012
 *      Author: miro
 */


#include <tmos.h>
#include <fam_cpp.h>
#include <cmsis_cpp.h>

unsigned int get_systick_source_clock()
{
//	if(SysTick->CTRL & SysTick_CTRL_CLKSOURCE_Msk)
//		return system_clock_frequency;
	return system_clock_frequency >> 3;
}


