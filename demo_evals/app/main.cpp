#include <tmos.h>
#include <drivers.h>
#include <hardware_cpp.h>

#ifndef TEST_TASK_SWITCH_SPEED
#define TEST_TASK_SWITCH_SPEED 0
#endif

#if TEST_TASK_SWITCH_SPEED == 1
void high_thread(void)
{
	PIN_DESC led_pin = PIN_BOX;

	PIO_Cfg(led_pin);
	while(1)
	{
		PIO_Assert(led_pin);
		tsk_get_signal(1);
		PIO_Deassert(led_pin);
		tsk_get_signal(1);
	}
}
TASK_DECLARE_STATIC(high_task, "HIGHT", high_thread, 60, 20); //priority 60

void low_thread(void)
{
	while(1)
	{
		tsk_send_signal(&high_task, 1);
	}
}
TASK_DECLARE_STATIC(low_task, "HIGHT", low_thread, 30, 20); //priority 30
#endif

#if TEST_TASK_SWITCH_SPEED == 2
// optimized version for fpp100
void high_thread(void)
{
	PIO_Cfg(PD_PD6  | PD_OUT);
	while(1)
	{
		GPIOD->GPIO_BSRR = PD_PIN_6;		// PIO_Assert();
		tsk_get_signal(1);
		GPIOD->GPIO_BSRR = PD_PIN_6 << 16;	// PIO_Deassert();
		tsk_get_signal(1);
	}
}
TASK_DECLARE_STATIC(high_task, "HIGHT", high_thread, 60, 20); //priority 60

void low_thread(void)
{
	while(1)
	{
		tsk_send_signal(&high_task, 1);
	}
}
TASK_DECLARE_STATIC(low_task, "HIGHT", low_thread, 30, 20); //priority 30
#endif

#ifndef TEST_MEM2MEM_DMA
#define TEST_MEM2MEM_DMA 0
#endif

#if TEST_MEM2MEM_DMA
// only for STM targets
const DMA_DRIVER_MODE test_dma_mode =
{
	// dma_index
	DMA2_Stream0_IRQn,
	// dma_ch_cr
	DMA_SxCR_CHSEL_0 |
	DMA_SxCR_MBURST_burst0 |
	DMA_SxCR_PBURST_burst0 |
	DMA_SxCR_PL_low	|
	DMA_SxCR_MINC |
	DMA_SxCR_MSIZE_8bit |
	DMA_SxCR_PINC |
	DMA_SxCR_PSIZE_8bit |
	DMA_SxCR_DIR_M2M,

	// dma_ch_fr
	DMA_SxFCR_FTH_full
};
void mem_dma_thread(void)
{
	CHandle hnd;
	char dst[10];
	RES_CODE res;

	memclr(dst, sizeof(dst));
	if(hnd.tsk_open(test_dma_mode.dma_index, &test_dma_mode))
	{
		// normal operation (memcopy)
		res = hnd.tsk_read_write(dst, "test", 4);
		TRACELN("DMA TEST res=%u", res);
	}
}
TASK_DECLARE_STATIC(mem_dma_task, "MEMD", mem_dma_thread, 1, 20+TRACE_SIZE);
#endif

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
TASK_DECLARE_STATIC(led_task, "LEDT", led_thread, 0, 20);


#ifndef TEST_BUTTON
#define TEST_BUTTON 0
#endif
#define DEBOUNCE_TIME 40		//40 ms debounce

#if TEST_BUTTON
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
#endif

#ifndef TEST_UART
#define TEST_UART 0
#endif

#if TEST_UART
void uart_thread(void)
{
	CHandle uart_hnd;
	char buf[8];
	RES_CODE res;
	unsigned int received, remain, dwRead;


	if(uart_hnd.tsk_open(UART_TEST_DRIVER, &uart_default_mode))
	{
		//loopback test
		while(1)
		{
			tsk_sleep(2);
			//transmit something..
			res = uart_hnd.tsk_write("hello\r\n");
			TRACELN("uart tsk send %x", res);
			remain = 7;

			//receive...
			received = 0;
			memclr(buf, sizeof(buf));
			while(remain)
			{
				res = uart_hnd.tsk_read(buf + received, remain, 1000);
				TRACELN("uart tsk rcv %x", res);
				if(res != RES_OK)
				{
					TRACELN1("failed !!!!");
					break;
				}

				dwRead = remain - uart_hnd.len;
				received += dwRead;
				remain -= dwRead;

				TRACE(" %u bytes", dwRead);
			}
			if(!remain && strcmp("hello\r\n", buf))
			{
				TRACELN1("strcmp !!!!");
			}
		}
	}
}
TASK_DECLARE_STATIC(uart_task, "UART", uart_thread, 5, 100+TRACE_SIZE);
#endif

#ifndef TEST_USB
#define TEST_USB 0
#endif

#if TEST_USB
#include <usb_api.h>
#include <usb_setup.h>
void usb_thread()
{
	char buf[8];
	CHandle usb_hnd;
	RES_CODE res=RES_FATAL;

	usb_hnd.tsk_open(USB_TEST_DRIVER, USB_DRV_MODE(USB_CDC_IN_EPT, USB_CDC_OUT_EPT));

	while(1)
	{
		while(res == RES_FATAL)
		{
			tsk_sleep(1000);
			TRACELN1("main conect");
			res = usb_hnd.tsk_command(USB_CMD_DEVICE_CONFIG, NULL);
//			res = usb_hnd.tsk_command(USB_CMD_OTG_CONFIG, NULL);
		}

		res = usb_hnd.tsk_read(buf, (unsigned int)sizeof(buf));
		TRACELN("usb tsk rcv %x", res);
		if(res <= RES_OK)
		{
			unsigned int dwRead;

			dwRead = sizeof(buf) - usb_hnd.len;
			TRACE(" %u bytes", dwRead);
			if(dwRead)
			{
				res = usb_hnd.tsk_write(buf, dwRead);
				TRACELN("usb tsk send %x", res);
			}
		}
	}
}
TASK_DECLARE_STATIC(usb_task, "USBT", usb_thread, 5, 100+TRACE_SIZE);
#endif

#if TEST_ADC
void adc_thread()
{
	uint16_t buf;
	CHandle adc_hnd;
	RES_CODE res=RES_FATAL;

	adc_hnd.tsk_open(ADC_TEST_DRV, &adc_test_mode);

	while(1)
	{
		res = adc_hnd.tsk_read(&buf, 2);
		TRACELN("adc tsk rcv %x %u", res, buf);
		tsk_sleep(100);
	}
}
TASK_DECLARE_STATIC(adc_task, "ADCT", adc_thread, 5, 100+TRACE_SIZE);
#endif

volatile unsigned int cpu_usage;

static unsigned int get_clocks_per200ms(void)
{
	unsigned int res;

	asm volatile (
#ifdef USE_ARMv6_SET 
		".syntax unified  				\n\t"
#endif			
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
#ifdef USE_ARMv6_SET 
		".syntax divided						\n\t"
#endif			

	  : "=r"(res)
	  : "I" (tsk_time)
	  : "r1", "r2", "r3", "r4", "memory"
	  );
	return res;
}

void start_rftest();
#ifndef USE_RFM73
#define USE_RFM73 0
#endif

#if USE_RFM73
void start_rfm73();
#endif

int main(void)
{
	unsigned int clock_freq;


	//start other tasks
    usr_task_init_static(&led_task_desc, true);
#if TEST_BUTTON
    usr_task_init_static(&button_task_desc, true);
#endif
#if TEST_UART
    usr_task_init_static(&uart_task_desc, true);
#endif
#if TEST_USB
    usr_task_init_static(&usb_task_desc, true);
#endif
#if TEST_TASK_SWITCH_SPEED
    usr_task_init_static(&high_task_desc, true);
    usr_task_init_static(&low_task_desc, true);
#endif
#if TEST_MEM2MEM_DMA
    usr_task_init_static(&mem_dma_task_desc, true);
#endif
#ifdef RF_TEST
    start_rftest();
#endif
#if USE_RFM73
    start_rfm73();
#endif
#if TEST_ADC
    usr_task_init_static(&adc_task_desc, true);
#endif

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
TASK_DECLARE_STATIC(main_task, "MAIN", (void (*)(void))main, 0, 25);
