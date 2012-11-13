#include <tmos.h>
#include <drivers.h>
#include <hardware_cpp.h>


/**
 * example how to drive pins directly...
 *
 * Note:
 * 	There are target specific things like the PIN_DESC type and it's value.
 * 	So these things are defined either in the OS or in the target directory of
 * 	the project.
 * 	The code here is supposed to be universal. We just want to flash a led,
 * 	we don't care if the actual board is using Atmel, ST or Luminary CPUs.
 * 	We don't care if the pin is PA0 or PB1. We don't care if the pin needs to be
 * 	asserted low or high. These are target specific things.
 * 	All we need here is to have a pin that can drive a led. We assume the pin
 * 	definition is called PIN_LED and we ask assertion or deassertion of this pin.
 * 	If you want to change the pin, just go in drivers.h in the target's directory
 * 	and change the definition.
 *
 */
void led_thread(void)
{
	PIN_DESC led_pin = PIN_LED;

	PIO_Cfg(led_pin);
	while(1)
	{
		PIO_Assert(led_pin);
		tsk_sleep(100);
		PIO_Deassert(led_pin);
		tsk_sleep(500);
	}
}
TASK_DECLARE_STATIC(led_task, "LEDT", led_thread, 0, 20+TRACE_SIZE);

#define DEBOUNCE_TIME 40		//40 ms debounce

void button_thread(void)
{
	/* We always need a handle to work with drivers	 */
	CHandle hnd;
	/* As a mode structure we must provide a zero/null terminated array of pin
	 * descriptions (many pins can be read/write at once).*/
	PIN_DESC pin_definitions[] = {PIN_BUTTON, 0};
	pio_set read_values[1];	// here we will read the pin values.

	/* If we want to open a handle for gpio operations we can use GPIO_IRQn as
	 * the driver index.
	 * The tsk_open() will also configure the pins according to the descriptions */
	if(hnd.tsk_open(GPIO_IRQn, &pin_definitions))
	{
		while(1)
		{
			/* Reading from the handle will return immediately with the pin state(s).
			 * The same can be done with PIO_Read() without handles and drivers.
			 *
			 * Locked read will return when at least one pin from the set triggers
			 * an interrupt. In our case we have one pin, so tsk_read_locked() will
			 * return as soon as active edge is detected on this pin.
			 * Make sure the definition includes rasing/falling edge flag. Otherwise
			 * the function will block forever.
			 */
			if(hnd.tsk_read_locked(read_values, sizeof(read_values)) == RES_OK)
			{
				// The pin changed once...

				//to debounce keep reading while it is changing too fast
				int oscilations =0;
				while(hnd.tsk_read_locked(read_values, sizeof(read_values), DEBOUNCE_TIME) == RES_OK)
				{
					// we have a change within less the DEBOUNCE time - ignore it!
					oscilations++;
				}

				// tsk_read_locked() timed out... the pin has settled at last.
				TRACELN("PIN change: %04X (%u)", read_values[0], oscilations);
			}
		}
	}
}
TASK_DECLARE_STATIC(button_task, "LEDT", button_thread, 5, 50+TRACE_SIZE);

void uart_thread(void)
{
	CHandle uart_hnd;
	char buf[8];
	RES_CODE res;

	RCC_ClocksTypeDef  clocks;

	RCC_GetClocksFreq (&clocks);

	if(uart_hnd.tsk_open(UART_TEST_DRIVER, &uart_default_mode))
	{
		//transmit something..
		res = uart_hnd.tsk_write("hello\r\n");
		TRACELN("uart tsk send %x", res);

		//loopback
		while(1)
		{
			res = uart_hnd.tsk_read(buf, sizeof(buf));
			TRACELN("uart tsk rcv %x", res);
			if(res <= RES_OK)
			{
				unsigned int dwRead;

				dwRead = sizeof(buf) - uart_hnd.len;
				TRACE(" %u bytes", dwRead);
				if(dwRead)
				{
					res = uart_hnd.tsk_write(buf, dwRead);
					TRACELN("uart tsk send %x", res);
				}
			}
		}
	}
}
TASK_DECLARE_STATIC(uart_task, "UART", uart_thread, 5, 100+TRACE_SIZE);


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
	  : "r1", "r2", "r3", "r4", "memory"
	  );
	return res;
}

int main(void)
{
	unsigned int clock_freq;


	//start other tasks
    usr_task_init_static(&led_task_desc, true);
    usr_task_init_static(&button_task_desc, true);
    usr_task_init_static(&uart_task_desc, true);

    //clocks in 250mS
    clock_freq = system_clock_frequency >> 2;

    for(;;)
	{
		unsigned int clocks;
		clocks = (125 * get_clocks_per200ms()) / clock_freq;
		if(clocks >= 100)
			clocks =1;
		else
			clocks = 100 - clocks;

		if(cpu_usage > clocks)
			cpu_usage -= (cpu_usage - clocks) >> 3 ;
		else
			cpu_usage += (clocks -cpu_usage) >> 3 ;

	}

    //codan!
    return 0;
}
TASK_DECLARE_STATIC(main_task, "MAIN", (void (*)(void))main, 0, 20+TRACE_SIZE);
