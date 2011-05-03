#include <tmos.h>
#include <usart_drv.h>
#include <adc_drv.h>
#include <series_cpp.h>
#include <dacc_drv.h>
#include <usbd_drv.h>
#include "fpga.h"
#include "cpu_pins.h"

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

//extern volatile unsigned int backlight_time;

extern "C" void app_init(void)
{
//	backlight_time = 100*1024;
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

const GPIO_STRU ADC_APINS = {
		PIO_PA18 | PIO_PA19 | PIO_PA20,
		0,
		PORT_A
};

const GPIO_STRU ADC_BPINS = {
		PIO_PB0 | PIO_PB2 | PIO_PB3,
		0,
		PORT_B
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
    PIO_CfgInput(&ADC_APINS);
    PIO_CfgInput(&ADC_BPINS);
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

volatile unsigned int g_fcco_pwm;

void dacc_thread( void )
{
	CHandle handle;
	unsigned int pw=0;

	handle.tsk_safe_open(DACC_IRQn, &DACC_MODE);

	tsk_sleep(10);

	while(1)
	{
//		for(int i=0; i<4095; i+=10)
//		{
//			handle.tsk_write(&i, 1);
//			tsk_sleep_rel(1);
//		}
		if (pw >= g_fcco_pwm)
			pw = g_fcco_pwm;
		else
		{
			TRACELN("DAC %d", pw);
			pw ++;
		}

		handle.tsk_write(&pw, 1);
		tsk_sleep(10);

	}
}
TASK_DECLARE_STATIC(dacc_task, "DACT", dacc_thread, 0, 150+TRACE_SIZE);

GPIO_STRU usb_vbus_pin = {
	PIO_PA31,
	PIOMODE_IER | PIOMODE_IFER,
	PORT_A
};

void usbc_thread( void )
{
	CHandle vbus_hnd;
	unsigned int pin_level;

	vbus_hnd.tsk_safe_open(PIOA_IRQn, &usb_vbus_pin);
	while(1)
	{
		tsk_sleep(50);
		vbus_hnd.tsk_read(&pin_level, 4);
		if(pin_level)
		{
	        TRACELN1( "USB connect" ) ;
			usr_drv_icontrol(UDP_IRQn, DCR_PARAMS, (void *) USBD_STATE_POWERED);
	    }
	    else
	    {
	        TRACE_INFO( "USB disconnect" ) ;

			usr_drv_icontrol(UDP_IRQn, DCR_PARAMS, (void *) USBD_STATE_SUSPENDED);
	    }

		vbus_hnd.tsk_read_locked(&pin_level, 4);
	}

}
TASK_DECLARE_STATIC(usbc_task, "USBC", usbc_thread, 0, 10+TRACE_SIZE);




#define BUTTON_SW1	1
#define BUTTON_SW2	2
#define BUTTON_SW3	0

#define CDCDSerialDriverDescriptors_DATAOUT	1
#define CDCDSerialDriverDescriptors_DATAIN	2


extern "C" uint8_t fpga_image_start;
extern "C" uint8_t fpga_image_end;

#define USC_CDC_HND_MODE  (void*)((CDCDSerialDriverDescriptors_DATAIN <<8) | CDCDSerialDriverDescriptors_DATAOUT)

void keyb_thread(void)
{
	CHandle key_hnd;
	CHandle usb_hnd;
	unsigned char key_code;
	unsigned int signal;
	uint8_t usb_buff[32];

	// switch ON power
	//================
	power_fpga_core_on();

	key_hnd.tsk_open(KEY_DRV_INDX, NULL);

	for(;;)
	{
		key_hnd.tsk_read(&key_code, 1);
		if(key_code == BUTTON_SW1)
		{
			g_fcco_pwm++;
		}

		if(key_code == BUTTON_SW2)
		{
			CHandle fpga_pio_hnd;

			TRACE( "FPGA start\r\n" );
			pio_open( &fpga_pio_hnd, &FPGA_DATANCLK );

			if ( fpga_open( &fpga_pio_hnd ) )
				usb_hnd.tsk_write( "FPGA init timeout\r\n", 19 );

			fpga_write(&fpga_pio_hnd, &fpga_image_start, &fpga_image_end - &fpga_image_start);
			fpga_close( &fpga_pio_hnd );
			TRACE( "FPGA end\r\n" );
		}

		if(key_code == BUTTON_SW3)
		{
			CHandle fpga_pio_hnd;

			pio_open( &fpga_pio_hnd, &FPGA_DATANCLK );

			if ( fpga_open( &fpga_pio_hnd ) )
				usb_hnd.tsk_write( "FPGA init timeout\r\n", 19 );

			if(usb_hnd.tsk_open(  UDP_IRQn , USC_CDC_HND_MODE))
			{
				usb_hnd.tsk_write( "FPGA start\r\n", 13);
				TRACE( "FPGA start\r\n" );

				key_hnd.tsk_start_read(&key_code, 1);
				usb_hnd.tsk_start_read(&usb_buff, sizeof(usb_buff));
				while (1)
				{
					signal = tsk_get_signal(SIGNAL_ANY);

					if(signal & key_hnd.signal)
					{
						key_hnd.res &= ~FLG_SIGNALED;
						if(key_code == BUTTON_SW3)
						{
							usb_hnd.tsk_cancel();
							usb_hnd.tsk_write( "FPGA end\r\n", 11);
							TRACE( "FPGA end\r\n" );
							break;
						}
						else
							key_hnd.tsk_start_read(&key_code, 1);
					}

					if(signal & usb_hnd.signal)
					{
						//we have received something...
						usb_hnd.res &= ~FLG_SIGNALED;
						if(usb_hnd.res & FLG_ERROR)
							tsk_sleep(10);	// opps something went wrong...
						else
							//we have received => sizeof(usb_buff) - usb_hnd.len
							fpga_write(&fpga_pio_hnd, usb_buff, sizeof(usb_buff) - usb_hnd.len);

						usb_hnd.tsk_start_read(&usb_buff, sizeof(usb_buff));
					}
				}
				usb_hnd.close();
			}

			fpga_close( &fpga_pio_hnd );
		}
	}
}
TASK_DECLARE_STATIC(keyb_task, "KEYB", keyb_thread, 0, 256+TRACE_SIZE);

int main(void)
{
	unsigned int clock_freq;
//    usr_task_init_static(&blink_task_desc, true);
//    usr_task_init_static(&uart1_task_desc, true);
//    usr_task_init_static(&adc_task_desc, true);
    usr_task_init_static(&dacc_task_desc, true);
    usr_task_init_static(&usbc_task_desc, true);
    usr_task_init_static(&keyb_task_desc, true);
//    keyb_task.sp->r0.as_int = 0;
//    usr_task_schedule(&keyb_task);

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

