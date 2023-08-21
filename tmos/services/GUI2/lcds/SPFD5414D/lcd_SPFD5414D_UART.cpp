/*
 * lcd_SPFD5414D_UART.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: miro
 */

#include <tmos.h>
#include <lcd_SPFD5414D_UART.h>
#include <fam_cpp.h>

static const unsigned int lut_to_tft_color_rotated[16] =
{
	PIX2LSB_RGB(PIX_RGB_BLACK),
	PIX2LSB_RGB(PIX_RGB_BLUE),
	PIX2LSB_RGB(PIX_RGB_GREEN),
	PIX2LSB_RGB(PIX_RGB_RED),
	PIX2LSB_RGB(PIX_RGB_LIGHTMAGENTA),
	PIX2LSB_RGB(PIX_RGB_BROWN),
	PIX2LSB_RGB(PIX_RGB_LIGHTBLUE),
	PIX2LSB_RGB(PIX_RGB_LIGHTGRAY),
	PIX2LSB_RGB(PIX_RGB_DARKGRAY),
	PIX2LSB_RGB(PIX_RGB_LIGHTGREEN),
	PIX2LSB_RGB(PIX_RGB_LIGHTCYAN),
	PIX2LSB_RGB(PIX_RGB_LIGHTRED),
	PIX2LSB_RGB(PIX_RGB_CYAN),
	PIX2LSB_RGB(PIX_RGB_MAGENTA),
	PIX2LSB_RGB(PIX_RGB_YELLOW),
	PIX2LSB_RGB(PIX_RGB_WHITE)
};

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
	MSB2LSB(SPFD5414D_CASET),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(0)),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(0)),
	MSB2LSB(SPFD5414D_RASET),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(0)),
		MSB2LSB(SPFD5414D_DATA(0)),MSB2LSB(SPFD5414D_DATA(0)),
	MSB2LSB(SPFD5414D_RAMWR)
};

//static inline __attribute__ ((always_inline)) void rotate(uint32_t * const value)
//{
//	asm volatile (
//	"		ldr				r3, [%0]		 \n\t"
//	"		rev				r3, r3			 \n\t"
//    "       rbit            r3, r3			 \n\t"
//    "       rev16           r3, r3			 \n\t"
//    "       lsr           	r3, r3, #7		 \n\t"
//	"		str				r3, [%0]		 \n\t"
//		  :
//		  : "r" (value)
//		  : "r3", "memory"
//	);
//}

static inline __attribute__ ((always_inline)) uint16_t rotate(uint16_t value)
{
	asm volatile (
    "       rbit            %0, %0			 \n\t"
    "       lsr           	%0, %0, #23		 \n\t"
		  : "+r" (value)
	);
	return value;
}

void SPFD5414D_UART::tft_init_address_cmd(unsigned short address_cmd[])
{
	memcpy(address_cmd, spdf5414d_lsb_row_address, sizeof(spdf5414d_lsb_row_address));
}

void SPFD5414D_UART::tft_write_row(unsigned short address_cmd[])
{
/*		0					1					2				3					4
	SPFD5414D_CASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(127),
		5					6					7				8					9
	SPFD5414D_RASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0),
	SPFD5414D_RAMWR
 */
	address_cmd[2] = rotate(SPFD5414D_DATA(frame.x0)); address_cmd[4] = rotate(SPFD5414D_DATA(frame.x1));
	address_cmd[7] = address_cmd[9] = rotate(SPFD5414D_DATA(frame.y0));
	unsigned int * dst = tft_buf;
	for(int x= 0; x < 64; x++)
	{
		*dst++ = lut_to_tft_color_rotated[(disp_buf[frame.y0][x] & 0xF0)>>4];
		*dst++ = lut_to_tft_color_rotated[disp_buf[frame.y0][x] & 0x0F];
	}
	lcd_hnd->tsk_write(address_cmd, sizeof(spdf5414d_lsb_row_address)/2);
	lcd_hnd->tsk_write_locked(tft_buf+frame.x0, (frame.x1 - frame.x0 +1)*2);
}

void SPFD5414D_UART::lcd_reset()
{
	lcd_hnd->tsk_write_locked(spdf5414d_lsb_init, sizeof(spdf5414d_lsb_init)/2);
}





