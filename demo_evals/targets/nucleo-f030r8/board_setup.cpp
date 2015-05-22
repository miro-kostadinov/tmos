/*
 * board_setup.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: miro
 */
#include <tmos.h>
#include <drivers.h>
#include <hardware_cpp.h>
#include <csocket.h>

/*!< Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source)

   IMPORTANT NOTE:
   ==============
   1. After each device reset the HSI is used as System clock source.

   2. Please make sure that the selected System clock doesn't exceed your device's
      maximum frequency.

   3. If none of the define below is enabled, the HSI is used as System clock
    source.

   4. The System clock configuration functions provided within this file assume that:
        - For Low, Medium and High density Value line devices an external 8MHz
          crystal is used to drive the System clock.
        - For Low, Medium and High density devices an external 8MHz crystal is
          used to drive the System clock.
        - For Connectivity line devices an external 25MHz crystal is used to drive
          the System clock.
     If you are using different crystal you have to adapt those functions accordingly.
    */


/**
 * This WEAK function is called immediately after reset
 */
extern "C" void LowLevelInit( void )
{

	//-- Enable HSI
	RCC->RCC_CR |=  RCC_CR_HSION;

	while(!(RCC->RCC_CR & RCC_CR_HSIRDY))
	{
	}

	//--disable the PLL
	RCC->RCC_CFGR &= ~RCC_CFGR_SW;		//select HSI as system clock
	while((RCC->RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
	{
	}
	RCC->RCC_CR &= ~(RCC_CR_HSEON | RCC_CR_PLLON);
	while(RCC->RCC_CR & RCC_CR_PLLRDY)
	{
	}

	//-- Configure the pll
	RCC->RCC_CFGR2 = 0;
	RCC->RCC_CFGR &= ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC); //PLLSRS = HSI/2
	RCC->RCC_CFGR |= RCC_CFGR_PLLMUL_Set(12-2);

	//--enable the PLL
	RCC->RCC_CR |= RCC_CR_PLLON;
	while(!(RCC->RCC_CR & RCC_CR_PLLRDY))
	{
	}

	//-- set flash wait states
	FLASH->FLASH_ACR = (FLASH->FLASH_ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_1WS;

	//-- HCLK = system clock, PCLK = system clock
	RCC->RCC_CFGR = (RCC->RCC_CFGR  & ~(RCC_CFGR_HPRE|RCC_CFGR_PPRE)) | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE_DIV1;

	//-- switch to pll
	RCC->RCC_CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
	{
	}


	system_clock_frequency = 48000000;

	if(exception_record.record_crc != exception_crc((const unsigned int*)&exception_record))
	{
		memclr((void *)&exception_record,sizeof(exception_record));
	}

	exception_record.reset_cause = LowLevelResetCause();
	if(exception_record.reset_cause & RCC_CSR_PORRSTF)
	{
		memclr((void *)&exception_record,sizeof(exception_record));
	}
	else
	{
		if(!exception_record.restart_cause && exception_record.reset_cause)
			exception_record.restart_cause = 0xff;
	}
}



char trace_que[100];
volatile uint8_t trace_queue_in;
volatile uint8_t trace_queue_out;
volatile uint8_t trace_queue_in2=1;
#define TRACE_SIGNAL 1

static void wait_trace_char()
{
	while(trace_queue_in == trace_queue_out)
		tsk_wait_signal(TRACE_SIGNAL, 500);
}

static void get_trace_char()
{
	unsigned out2;

	out2 = trace_queue_out +1;
	if(out2 ==  sizeof(trace_que))
		out2=0;
	trace_queue_out = out2;
}

void trace_uart( void )
{
	CHandle uart;
	uint32_t txt_color = TC_TXT_WHITE;
	uint32_t bg_color = TC_BG_BLACK;
	char buf[8];
	bool def_col = false;

	ALLOCATE_SIGNAL(TRACE_SIGNAL);
	if(uart.tsk_open(USART2_IRQn, &uart_default_mode))
	{
		while(1)
		{
			wait_trace_char();
			while(trace_queue_in != trace_queue_out)
			{
				if(trace_que[trace_queue_out] == '\e')
				{
					uint32_t pos =0;
					uint32_t col, bg;

					for(; pos< sizeof(buf); )
					{
						buf[pos++] = trace_que[trace_queue_out];
						if(trace_que[trace_queue_out] == 'm')
						{
							if(buf[1] == '[')
							{
								if(IS_DIGIT(buf[2]))
								{
									uint32_t i = 3;
									col = buf[2] - '0';

									if(IS_DIGIT(buf[i]))
									{
										col *= 10;
										col += buf[i++] - '0';
									}
									bg = bg_color;
									if(buf[i] == ';')
									{
										if(IS_DIGIT(buf[++i]))
										{
											bg = buf[i++] - '0';

											if(IS_DIGIT(buf[i]))
											{
												bg *= 10;
												bg += buf[i++] - '0';
											}
										}
									}
									if(buf[i] == 'm')
									{
										if(col == txt_color && bg == bg_color)
										{
											pos = 0;
											def_col = false;
										} else
										{
											txt_color = col;
											bg_color = bg;
										}
									}
								} else
									if(buf[2] == 'm')
									{
										def_col = true;
										pos = 0;
									}
							}
							break;
						}
						get_trace_char();
						wait_trace_char();
					}
					if(pos)
					{
						uart.tsk_write(buf, pos);
					}

				} else
				{
					if(def_col)
					{
						uart.tsk_write("\e[m", 3);
						def_col = false;
						txt_color = TC_TXT_WHITE;
						bg_color = TC_BG_BLACK;
					}
					uart.tsk_write(&trace_que[trace_queue_out], 1);
				}
				get_trace_char();
			}

		}
	}
}
TASK_DECLARE_STATIC(TRACE_UART_task, "TRACE", trace_uart, 20, 55);

extern "C" void usr_trace_char(unsigned c)
{
	if ( trace_queue_in2 != trace_queue_out)
	{
		__disable_irq();
		trace_que[trace_queue_in] = c;
		trace_queue_in = trace_queue_in2++;
		if(trace_queue_in2 == sizeof(trace_que))
			trace_queue_in2=0;
		__enable_irq();
		if(__get_CONTROL() & 2)
			tsk_send_signal(&TRACE_UART_task, TRACE_SIGNAL);
	}
}

extern "C" void usr_trace_str(const char *buf)
{
	while(*buf)
	{
		if ( (__get_CONTROL() & 2) && (trace_queue_in2 == trace_queue_out))
		{
			unsigned int time;

			time = CURRENT_TASK->time;
			tsk_sleep(5);
			CURRENT_TASK->time = time;
		}
		usr_trace_char(*buf++);
	}
}

const sock_mode_t g_wifi_mode =
{
	WIFI_DRV_INDX, 			//!< driver to be used for this socket
	1,						//!< driver interface index (e.g. apn index)
	80, 					//!< optional port (e.g. for UDP)
	IP_SOCKET_TCP			//!< IP_SOCKET_TCP or IP_SOCKET_UDP

};

void wifi_test(void)
{
	CSocket sock;

	if(sock.open(&g_wifi_mode) == RES_OK)
	{
		TRACELN1("wifi open");
	}

	TRACELN1("wifi done");
}
TASK_DECLARE_STATIC(wifi_client_task, "FWCT", wifi_test, 2, 55);

extern "C" void app_init(void)
{
    usr_task_init_static(&TRACE_UART_task_desc, true);
    usr_task_init_static(&wifi_client_task_desc, true);
	TRACELN1("App init");
}




