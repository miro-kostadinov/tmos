/*
 * rftest.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: miro
 */


#include <tmos.h>
#include <drivers.h>

#ifdef RF_TEST

#include <tim_drv.h>

const TIMER_CONTROL_MODE g_tim2_ctrl_mode =
{
	0,					//!< timer index (0..X)
	0,					//!< channel index (0=control, 1-4=channel)
	0,					//!< dma/interrupt enable register (TIM_DIER)
	0xFFFF,				//!< auto reload or capture/compare register (TIM_ARR or TIM_CCRx)

	/*TIM_CR1_ARPE |*/ TIM_CR1_URS,			//!< control register 1 (TIM_CR1)
	0,					//!< control register 2 (TIM_CR2)
	0,					//!< slave mode register (TIM_SMCR)
	72,					//!< prescaler (TIM_PSC)
	0					//!< repetition counter	(TIM_RCR)
};

struct time_base_t
{
	unsigned int start_ms;
	unsigned int start_us;
	unsigned int us;
	unsigned int ms;

	void start()
	{
		start_ms = CURRENT_TIME;
		start_us = TIM2->TIM_CNT;
	}

	unsigned int elapsed()
	{

		us = TIM2->TIM_CNT;
		ms = CURRENT_TIME;
//		TRACELN("        %ums %uus", ms, us);

		if(ms < start_ms)
			ms += 0x80000000;
		ms -= start_ms;

		if(us <start_us)
			us += 0x10000;
		us -= start_us;

		if(ms > 50)
			return 0;		//above 5ms
		return us;
	}

	bool in_range(unsigned int min, unsigned int max)
	{
		unsigned int time = elapsed();

		if((time >= min) && (time <= max))
			return true;

		return false;
	}
};

enum rf_state {st_header, st_start, st_bit};

struct rf_pair
{
	uint16_t min;
	uint16_t max;
};

const rf_pair header[]=
{
	{1300, 1700},
	{}
};

//----- bingo!
//003706 0: 1ms 1509us
//004181 1: 1ms 475us			1
//004392 0: 0ms 211us
//004867 1: 1ms 475us			2
//005115 0: 0ms 248us
//005559 1: 1ms 444us			3
//005832 0: 0ms 273us
//006255 1: 0ms 423us			4
//006548 0: 1ms 293us
//006948 1: 0ms 400us			5
//007255 0: 0ms 307us
//007646 1: 0ms 391us			6
//007968 0: 0ms 322us
//008352 1: 1ms 384us			7
//008677 0: 0ms 325us
//009053 1: 1ms 376us			8
//009386 0: 0ms 333us
//009753 1: 0ms 367us			9
//010099 0: 1ms 346us
//010456 1: 0ms 357us			10
//010808 0: 1ms 352us
//011154 1: 0ms 346us			11
//011516 0: 0ms 362us
//011866 1: 1ms 350us			12
//016116 0: 4ms 4250us
//017023 1: 1ms 907us
//017226 0: 0ms 203us
//017714 1: 1ms 488us
//018346 0: 1ms 632us
//018814 1: 0ms 468us
//019450 0: 1ms 636us
//019915 1: 0ms 465us
//020550 0: 1ms 635us
//021018 1: 0ms 468us
//021655 0: 1ms 637us
//022121 1: 1ms 466us
//022750 0: 0ms 629us
//023217 1: 1ms 467us
//023857 0: 1ms 640us
//024322 1: 0ms 465us
//024952 0: 1ms 630us
//025394 1: 0ms 442us
//026063 0: 1ms 669us
//026860 1: 1ms 797us
//027163 0: 0ms 303us
//027573 1: 1ms 410us
//028275 0: 0ms 702us
//028682 1: 1ms 407us
//029368 0: 1ms 686us
//030142 1: 0ms 774us
//030476 0: 1ms 334us
//031223 1: 1ms 747us
//031586 0: 0ms 363us
//032309 1: 1ms 723us
//032714 0: 0ms 405us
//033407 1: 1ms 693us
//033812 0: 0ms 405us
//034116 1: 1ms 304us
//034921 0: 1ms 805us
//035234 1: 0ms 313us
//035998 0: 1ms 764us
//036354 1: 0ms 356us
//037091 0: 1ms 737us
//037825 1: 1ms 734us
//038195 0: 0ms 370us
//038557 1: 1ms 362us
//039302 0: 0ms 745us
//040022 1: 1ms 720us
//040395 0: 1ms 373us
//040764 1: 0ms 369us
//041500 0: 1ms 736us
//041877 1: 0ms 377us
//042588 0: 1ms 711us
//043327 1: 1ms 739us
//043700 0: 0ms 373us
//044035 1: 1ms 335us
//044792 0: 0ms 757us
//045150 1: 1ms 358us
//045890 0: 1ms 740us
//046633 1: 0ms 743us
//046989 0: 1ms 356us
//047361 1: 0ms 372us
//048101 0: 1ms 740us
//048473 1: 0ms 372us
//049181 0: 0ms 708us
//049586 1: 1ms 405us
//050284 0: 0ms 698us
//050689 1: 0ms 405us
//051369 0: 1ms 680us
//051759 1: 0ms 390us
//052475 0: 1ms 716us
//052875 1: 0ms 400us
//053563 0: 1ms 688us
//053988 1: 1ms 425us
//054664 0: 0ms 676us
//055447 1: 1ms 783us
//055767 0: 0ms 320us
//056172 1: 0ms 405us
//056881 0: 1ms 709us
//057627 1: 0ms 746us
//057974 0: 0ms 347us
//058715 1: 1ms 741us
//059095 0: 0ms 380us
//059811 1: 1ms 716us
//060205 0: 0ms 394us
//060516 1: 1ms 311us
//061311 0: 1ms 795us
//062011 1: 0ms 700us
//062404 0: 1ms 393us
//063094 1: 0ms 690us
//063525 0: 0ms 431us
//064194 1: 1ms 669us
//064623 0: 0ms 429us
//065284 1: 1ms 661us
//065744 0: 0ms 460us
//066022 1: 1ms 278us
//066820 0: 1ms 798us
//067495 1: 0ms 675us
//067926 0: 1ms 431us
//068586 1: 0ms 660us
//069026 0: 0ms 440us
//069276 1: 0ms 250us
//096817 0: 28ms 27541us
//098247 1: 2ms 1430us
//098310 0: 0ms 63us
//098902 1: 1ms 592us
//099044 0: 0ms 142us
//099585 1: 0ms 541us
//099762 0: 1ms 177us
//100267 1: 0ms 505us
//100482 0: 0ms 215us
//100957 1: 1ms 475us
//101202 0: 0ms 245us
//101643 1: 1ms 441us
//101916 0: 0ms 273us
//102336 1: 0ms 420us
//102626 0: 1ms 290us
//103041 1: 0ms 415us
//103344 0: 1ms 303us
//103744 1: 0ms 400us
//104057 0: 0ms 313us
//104444 1: 1ms 387us
//104766 0: 0ms 322us
//105137 1: 1ms 371us
//109412 0: 4ms 4275us
//110318 1: 1ms 906us
//110524 0: 0ms 206us
//111024 1: 1ms 500us
//111658 0: 0ms 634us
//112125 1: 0ms 467us
//112758 0: 1ms 633us
//113232 1: 0ms 474us
//113874 0: 1ms 642us
//114351 1: 0ms 477us
//114974 0: 1ms 623us
//115443 1: 1ms 469us
//116088 0: 0ms 645us
//116554 1: 1ms 466us
//117194 0: 1ms 640us
//117669 1: 0ms 475us
//118297 0: 1ms 628us
//118727 1: 0ms 430us
//119402 0: 1ms 675us
//120210 1: 1ms 808us
//120509 0: 0ms 299us
//120920 1: 1ms 411us
//121622 0: 0ms 702us
//122035 1: 1ms 413us
//122736 0: 1ms 701us
//123508 1: 0ms 772us
//123841 0: 1ms 333us
//124594 1: 1ms 753us
//124965 0: 0ms 371us
//125685 1: 1ms 720us
//126077 0: 0ms 392us
//126781 1: 1ms 704us
//127204 0: 0ms 423us
//127495 1: 0ms 291us
//128296 0: 1ms 801us
//128621 1: 0ms 325us
//129392 0: 1ms 771us
//129747 1: 0ms 355us
//130483 0: 1ms 736us
//131228 1: 1ms 745us
//131602 0: 0ms 374us
//131964 1: 1ms 362us
//132696 0: 0ms 732us
//133426 1: 0ms 730us
//133809 0: 1ms 383us
//134174 1: 0ms 365us
//134907 0: 1ms 733us
//135290 1: 0ms 383us
//136016 0: 1ms 726us
//136748 1: 1ms 732us
//137115 0: 0ms 367us
//137462 1: 1ms 347us
//138233 0: 1ms 771us
//138586 1: 0ms 353us
//139321 0: 1ms 735us
//140066 1: 1ms 745us
//140434 0: 0ms 368us
//140810 1: 0ms 376us
//141532 0: 1ms 722us
//141915 1: 1ms 383us
//142640 0: 0ms 725us
//143044 1: 1ms 404us
//143734 0: 1ms 690us
//144155 1: 0ms 421us
//144838 0: 1ms 683us
//145225 1: 0ms 387us
//145930 0: 1ms 705us
//146341 1: 0ms 411us
//147040 0: 1ms 699us
//147461 1: 1ms 421us
//148141 0: 0ms 680us
//148931 1: 1ms 790us
//149252 0: 1ms 321us
//149648 1: 0ms 396us
//150360 0: 1ms 712us
//151118 1: 1ms 758us
//151477 0: 0ms 359us
//152207 1: 1ms 730us
//152585 0: 0ms 378us
//153299 1: 1ms 714us
//153715 0: 0ms 416us
//154016 1: 0ms 301us
//154811 0: 1ms 795us
//155516 1: 0ms 705us
//155916 0: 1ms 400us
//156614 1: 1ms 698us
//157028 0: 0ms 414us
//157704 1: 1ms 676us
//158151 0: 0ms 447us
//158813 1: 1ms 662us
//159257 0: 1ms 444us
//159548 1: 0ms 291us
//160362 0: 1ms 814us
//161032 1: 0ms 670us
//161457 0: 0ms 425us
//162126 1: 1ms 669us
//162581 0: 0ms 455us
//162823 1: 0ms 242us
//0: 296ms 29104us

void rftest_thread()
{
	CHandle tim_hnd;
	CHandle pin_hnd;
	PIN_DESC pin_definitions[] = {PIN_PD2, 0};
	pio_set pin_value;
	time_base_t timer;
	rf_state state;
	unsigned int pos;

	/* If we want to open a handle for gpio operations we can use GPIO_IRQn as
	 * the driver index.
	 * The tsk_open() will also configure the pins according to the descriptions */
	pin_hnd.tsk_open(GPIO_IRQn, &pin_definitions);

	//start Timer 2
	tim_hnd.tsk_open(TIM2_IRQn, &g_tim2_ctrl_mode);
	tim_hnd.tsk_command(NULL, NULL);
	TRACELN1("rftest");
	PIO_Cfg(PIN_PC8);
	while(1)
	{
		//-- wait for low level
		while(PIO_Read(PIN_PD2))
		{
			pin_hnd.tsk_read_locked(&pin_value, sizeof(pin_value));
		}

		PIO_Deassert(PIN_PC8);

		//-- wait for rising edge
		pin_hnd.tsk_read_locked(&pin_value, sizeof(pin_value));

		PIO_Assert(PIN_PC8);
		timer.start();
		//-- wait for falling edge
		pin_hnd.tsk_read_locked(&pin_value, sizeof(pin_value));
		//-- get 2ms beat?
		if(timer.in_range(2000, 2300))
		{
			time_base_t timer2;
			unsigned int level = 0;
			unsigned int t = timer.us;
			unsigned int data[3];
			unsigned int low_bit=0;
			unsigned int byte_indx;

			TRACELN1("----- bingo!");
			state = st_header;
			pos = 0;
			while(1)
			{
				// wait for edge
				timer2.start();
				pin_hnd.tsk_read_locked(&pin_value, sizeof(pin_value));
				if(!timer2.in_range(50, 30000))
				{
					TRACELN("%u: %ums %uus", level, timer2.ms, timer2.us);
					break;
				}
				t += timer2.us;
				TRACELN("%u %06u %u: %ums %uus", pos, t, level, timer2.ms, timer2.us);
				switch(state)
				{
				case st_header:
					pos += level;
					if(pos ==12)
					{
						data[0] = 0;
						data[1] = 0;
						data[2] = 0;
						pos = 0;
						state = st_start;
						byte_indx = 0;
					}
					break;

				case st_start:
					state = st_bit;
					break;

				case st_bit:
					if(level)
					{
						low_bit = timer2.us;
					} else
					{
						if(low_bit > timer2.us)
						{
							data[byte_indx] |= 1 << pos;
						}
						pos ++;
						if(pos == 16)
						{
							pos = 0;
							byte_indx++;
							if(byte_indx == 3)
							{
								byte_indx = 0;
								TRACE_COLOR(TC_BG_RED);
								TRACELN("** %04x %04x %04x", data[0], data[1], data[2]);
								TRACE_COLOR_END();
								data[0] = 0;
								data[1] = 0;
								data[2] = 0;
							}
						}
					}
					break;
				}
				level ^= 1;
			}
		}
	}
}
TASK_DECLARE_STATIC(rftest_task, "RFTT", rftest_thread, 50, 500+TRACE_SIZE);

void start_rftest()
{
    usr_task_init_static(&rftest_task_desc, true);
}

#endif
