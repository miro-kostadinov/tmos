/*
 * board_setup.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <drivers.h>
#include <hardware_cpp.h>


/**
 * This WEAK function is called immediately after reset
 */
extern "C" void LowLevelInit( void )
{
	SYSCTL->SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL|SYSCTL_SYSDIV_2_5);
	//SYSCTL->SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC	| SYSCTL_SYSDIV_2);
	system_clock_frequency = SYSCTL->SysCtlClockGet();


	if(exception_record.record_crc != exception_crc((const unsigned int*)&exception_record))
	{
		memclr((void *)&exception_record,sizeof(exception_record));
	}

	exception_record.reset_cause = LowLevelResetCause();
	if(exception_record.reset_cause & SYSCTL_CAUSE_POR)
	{
		memclr((void *)&exception_record,sizeof(exception_record));
	}
	else
	{
		if(!exception_record.restart_cause && exception_record.reset_cause)
			exception_record.restart_cause = 0xff;
	}
}


extern "C" void app_init(void)
{
	TRACELN1("App init");
}




