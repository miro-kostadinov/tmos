#include <tmos.h>
#include <usart_drv.h>
#include <adc_drv.h>
#include <series_cpp.h>
#include <dacc_drv.h>

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
	US_MR_PAR_NO | US_MR_CHRL_8_BIT,		// UART_MR register value
	100,				// receive timeout
	100					// transmit timeout (not supported?)
};

void blink_thread(void)
{
	CHandle handle;

	// remote connection
	handle.tsk_safe_open(USART1_IRQn, &UART1_MODE);
	while(1)
	{
//		TRACELN1(".");
		tsk_sleep(1000);
//		handle.tsk_write("1");
	}
}
TASK_DECLARE_STATIC(blink_task, "BLNK", blink_thread, 0, 10+TRACE_SIZE);


void uart1_thread( void )
{
	unsigned char symbol;
	CHandle handle;

	// remote connection
	handle.tsk_safe_open(USART1_IRQn, &UART1_MODE);


	while(1)
	{
		// Get the start symbol
		handle.tsk_read(&symbol, 1);
		handle.tsk_write(&symbol, 1);
		TRACELN(">%02x %c", symbol, symbol);
	}
}
TASK_DECLARE_STATIC(uart1_task, "URT1", uart1_thread, 0, 150+TRACE_SIZE);

unsigned int get_mV(RES_CODE& res, CHandle& handle, unsigned int channel)
{
	handle.mode.as_int = channel;
	res = handle.tsk_read(NULL, 8, 1000);
	return handle.dst.as_int*3300/8/4095;
}

const GPIO_STRU PWR_DISABLE = {
		PIO_PA14,
		PIOA
};

const GPIO_STRU ADC_APINS = {
		PIO_PA18 | PIO_PA19 | PIO_PA20,
		PIOA
};

const GPIO_STRU ADC_BPINS = {
		PIO_PB0 | PIO_PB2 | PIO_PB3,
		PIOB
};

void adc_thread( void )
{
	RES_CODE res;
	CHandle handle;
	unsigned int mV;
	signed int temp;

	// remote connection
	handle.tsk_safe_open(ADC_IRQn, NULL);

    /* Enable the temperature sensor */
    ADC_EnableTS( ADC, ADC_ACR_TSON ) ;

    ADC_check( ADC, system_clock_frequency );
    GPIO_CfgOutput1(&PWR_DISABLE);
    GPIO_CfgInput(&ADC_APINS);
    GPIO_CfgInput(&ADC_BPINS);
	while(1)
	{
		tsk_sleep(1000);

		// Read AD15
		mV = get_mV(res, handle, ADC_CHER_CH15);
		temp = mV - 800; //Vt @ 27C
        /* using multiply instead of dividing /2.65 */
        temp *= 37736;		// 0.37736
        temp /= 100;		// mC
		temp += 27000;
		TRACELN("Temp %d.%03dC  %dmV (%x)", temp/1000, temp%100, mV, res);


		// Read AD1
		mV = get_mV(res, handle, ADC_CHER_CH1);
		TRACELN("JVCC = %dmV (%x)", mV, res);

		// Read AD2
		mV = get_mV(res, handle, ADC_CHER_CH2);
		TRACELN("VREF = %dmV (%x)", mV, res);

		// Read AD3
		mV = get_mV(res, handle, ADC_CHER_CH3);
		TRACELN("VCO = %dmV (%x)", mV, res);

		// Read AD4
		mV = get_mV(res, handle, ADC_CHER_CH4);
		TRACELN("V3 = %dmV (%x)", mV, res);

		// Read AD5
		mV = get_mV(res, handle, ADC_CHER_CH6);
		TRACELN("V25 = %dmV (%x)", mV, res);

		// Read AD7
		mV = get_mV(res, handle, ADC_CHER_CH7);
		TRACELN("V12 = %dmV (%x)", mV, res);

	    GPIO_CfgOutput0(&PWR_DISABLE);
		TRACELN("----");
	}
}
TASK_DECLARE_STATIC(adc_task, "ADCT", adc_thread, 0, 150+TRACE_SIZE);


const DRV_DACC_MODE_STRU DACC_MODE =
{
	//DACC_MR
	DACC_MR_TRGEN_DIS | DACC_MR_REFRESH(1) | DACC_MR_STARTUP_960 |
	DACC_MR_USER_SEL_CHANNEL1,

	//DACC_CHER
	DACC_CHER_CH1
};

void dacc_thread( void )
{
	CHandle handle;

	handle.tsk_safe_open(DACC_IRQn, &DACC_MODE);

	tsk_sleep(10);
	while(1)
	{
		for(int i=0; i<4095; i+=10)
		{
			handle.tsk_write(&i, 1);
			tsk_sleep_rel(1);
		}
	}
}
TASK_DECLARE_STATIC(dacc_task, "DACT", dacc_thread, 0, 150+TRACE_SIZE);

int main(void)
{
	unsigned int clock_freq;
    usr_task_init_static(&blink_task_desc, true);
    usr_task_init_static(&uart1_task_desc, true);
    usr_task_init_static(&adc_task_desc, true);
    usr_task_init_static(&dacc_task_desc, true);


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
