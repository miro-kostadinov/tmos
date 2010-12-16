#include <tmos.h>
#include <uart_drv.h>

volatile unsigned int cpu_usage;

static unsigned int get_clocks_per200ms(void)
{
	unsigned int res;

	asm volatile (
		"1: \n\t"
		"	movs			%0, #0				\n\t"
		"	mov				r1, r10				\n\t"	//r1 = REG_MTASK
		"	movs			r2, #200			\n\t"	//r2 = mS
		"	ldr				r3, [r1, %1]		\n\t"	//r3 = CURRENT_TIME
		"2:										\n\t"
		"	adds			%0, #6				\n\t"	// r0 += [6]		[1]
		"	ldr				r4, [r1, %1]		\n\t"	//					[2]
		"	cmp				r3, r4				\n\t"	//					[1]
		"	beq				2b 					\n\t"	// do while (r2==r4)[2]
		"	bhi				1b 					\n\t"

		"	adds			r3, #1				\n\t"
		"	subs			r2, #1				\n\t"
		"	bne				2b 					\n\t"

	  : "=r"(res)
	  : "I" (tsk_time)
	  : "r1", "r2", "r3", "r4"
	  );
	return res;
}


int main(void);
TASK_DECLARE_STATIC(main_task, "MAIN", (void (*)(void))main, 0, 20+TRACE_SIZE);

const DRV_UART_MODE_STRU UART1_MODE = {
	9600, 				// baudrate for the mode
	UART_MR_PAR_NO,		// UART_MR register value
	100,				// receive timeout
	0					// transmit timeout (not supported?)
};

void blink_thread(void)
{
	while(1)
	{
		TRACELN1(".");
		tsk_sleep(1000);
	}
}
TASK_DECLARE_STATIC(blink_task, "BLNK", blink_thread, 0, 10+TRACE_SIZE);


void uart1_thread( void )
{
	unsigned char symbol;
	CHandle handle;

	// remote connection
	handle.tsk_safe_open(UART1_IRQn, &UART1_MODE);


	while(1)
	{
		// Get the start symbol
		handle.tsk_read(&symbol, 1);
		handle.tsk_write(&symbol, 1);
		TRACE("%c", symbol);
	}
}
TASK_DECLARE_STATIC(uart1_task, "URT1", uart1_thread, 0, 150+TRACE_SIZE);



int main(void)
{
	unsigned int clock_freq;
    usr_task_init_static(&blink_task_desc, true);
    usr_task_init_static(&uart1_task_desc, true);


    //clocks in 250mS
    clock_freq = system_clock_frequency >> 2;

	for(;;)
	{
		unsigned int clocks;
		clocks = (125 * get_clocks_per200ms()) / clock_freq;
		if(clocks >= 100)
			cpu_usage =1;
		else
			cpu_usage = 100-clocks;

	}
}
