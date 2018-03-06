/*
 * lcd_SPFD5414D_UART.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: miro
 */

#include <tmos.h>
#include <lcd_SPFD5414D_UART.h>
#include <fam_cpp.h>

static const unsigned short spdf5414d_lsb_init[] =
{
	MSB2LSB(SPFD5414D_SLPOUT),

	// ColorModeSet 16bpp
	MSB2LSB(SPFD5414D_COLMOD),
		MSB2LSB(SPFD5414D_DATA(SPFD5414D_COLMOD_16BPP)),

	//_MemoryAccess BGR
	MSB2LSB(SPFD5414D_MADCTR),
		MSB2LSB(SPFD5414D_DATA(
				SPFD5414D_MADCTR_MY | SPFD5414D_MADCTR_MX |
				SPFD5414D_MADCTR_ML | SPFD5414D_MADCTR_BGR)),

	// Display on
	MSB2LSB(SPFD5414D_NORON),
	MSB2LSB(SPFD5414D_DISPON),


	// SPFD5414D_ColumnAddressSet
	MSB2LSB(SPFD5414D_CASET),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(0)),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(127)),

	// SPFD5414D_RowAddressSet
	MSB2LSB(SPFD5414D_RASET),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(0)),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(127))
};


static const unsigned short spdf5414d_lsb_row_address[] =
{
	MSB2LSB(SPFD5414D_RASET),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(0)),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(0)),
	MSB2LSB(SPFD5414D_RAMWR)
};

static inline void rotate(uint32_t * const value)
{
	asm volatile (
	"		ldr				r3, [%0]		 \n\t"
	"		rev				r3, r3			 \n\t"
    "       rbit            r3, r3			 \n\t"
    "       rev16           r3, r3			 \n\t"
    "       lsr           	r3, r3, #7		 \n\t"
	"		str				r3, [%0]		 \n\t"
		  :
		  : "r" (value)
		  : "r3", "memory"
	);
}

static inline uint16_t rotate(uint16_t value)
{
	asm volatile (
    "       rbit            %0, %0			 \n\t"
    "       lsr           	%0, %0, #23		 \n\t"
		  : "=r" (value)
	);
	return value;
}

void SPFD5414D_UART::tft_write_row(unsigned short address_cmd[], unsigned short row)
{
	if(row == 0xFFFF) // initialize row_address buffer
	{
		memcpy(address_cmd, spdf5414d_lsb_row_address, sizeof(spdf5414d_lsb_row_address));
	}
	else
	{
		address_cmd[2] = address_cmd[4] = rotate(row|0x100);
		lcd_hnd->tsk_write(address_cmd, sizeof(spdf5414d_lsb_row_address)/sizeof(unsigned short));
		for(int i=0; i< 128; i++)
		{
			rotate(disp_buf +i);
		}

		lcd_hnd->tsk_write(disp_buf, 256);
		if(disp_buf == video_buf)
			disp_buf += 128;
		else
			disp_buf = video_buf;
	}

}

void SPFD5414D_UART::lcd_reset()
{
	lcd_hnd->tsk_write(spdf5414d_lsb_init, sizeof(spdf5414d_lsb_init)/2);
}





