/****************************************************************
 * @file:		clock_drv.h
 * @purpose:	Clock driver (systic, oscillators, pll etc) for LM3S
 *
 * @version:	1.0
 * @date: 		2010-2-25
 * @author: 	Miroslav Kostadinov
 ****************************************************************/

#ifndef CLOCK_DRV_H_
#define CLOCK_DRV_H_




struct ClockDriver
{
	DRIVER_INFO_Type info; 		//!< doc!
};

void dcr_ClockDriver(ClockDriver* driver, unsigned int reason, void* param);
void dsr_ClockDriver(ClockDriver* driver, HANDLE hnd);
extern "C" void isr_ClockDriver(ClockDriver* driver);


#endif /* CLOCK_DRV_H_ */
