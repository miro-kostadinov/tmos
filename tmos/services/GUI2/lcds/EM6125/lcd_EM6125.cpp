//////////////////////////////////////////////////////////////////////////
//
//			LCD Library
//
//	for: EMM6125 based
//
//		LCD functions are called from the GUI task context
//
//////////////////////////////////////////////////////////////////////////

#include <tmos.h>
#include "lcd_EM6125.h"
#include "render.h"
#include <fam_cpp.h>




#define EMM_CB_INIT0	0x80
#define EMM_DB_INIT0	0xA0
#define EMM_CB_INIT1	0x81
#define EMM_CB_INIT2	0x82
#define EMM_CB_INIT3	0x83
#define EMM_DB_INIT3	0x8E
#define EMM_CB_INIT4	0x84
#define EMM_DB_INIT4	0xC0
#define EMM_CB_TEST0	0xA0
#define EMM_CB_TEST1	0xA8
#define EMM_CB_TEST2	0xB0
#define EMM_CB_TEST3	0xB8
#define EMM_CB_DRAM		0x40


const unsigned char emm_init1[]=
{
    EMM_CB_INIT0, 0xA0,	// set functions					= Mux mode TC=1
    EMM_CB_INIT1, 0x01, // Set column adress for DDRAM		= X=0, V=1
    EMM_CB_INIT2, 0x10, // Set bank adress for DDRAM 		= Y=1
    EMM_CB_INIT3, 0x6E, // Programming the internally generated LCD voltage supply VLCD = 0x6E
    EMM_CB_INIT4, 0xC0, // Number of voltage multiplier stages = Mult=3, First Row=0
    EMM_CB_TEST0, 0x02, // Byte test 0, all bits must be set to 0 ??
    EMM_CB_TEST1, 0x00, // Byte test 1, all bits must be set to 0
    EMM_CB_TEST2, 0x00, // Byte test 2, all bits must be set to 0
    EMM_CB_TEST3, 0x00  // Byte test 3, all bits must be set to 0
};

const unsigned char emm_init2[]=
{
    EMM_CB_INIT0, 0xB8, // set functions					= Mux mode, TC=1, Inv=1, MX
    EMM_CB_INIT1, 0x01, // Set column adress for DDRAM		= X=0, V=1
    EMM_CB_INIT2, 0x10, // Set bank adress for DDRAM 		= Y=1
    EMM_CB_INIT3, 0x7E, // Programming the internally generated LCD voltage supply VLCD = 0x7E
    EMM_CB_INIT4, 0xC0, // Number of voltage multiplier stages = Mult=3, First Row=0
    EMM_CB_TEST0, 0x00,
    EMM_CB_TEST1, 0x00,
    EMM_CB_TEST2, 0x00,
    EMM_CB_TEST3, 0x00
};

const unsigned char res_emm_init1[]=
{
    EMM_CB_INIT0, 0xB8,
    EMM_CB_INIT1, 0x01,
    EMM_CB_INIT2, 0x10,
    EMM_CB_INIT3, 0xA8,
    EMM_CB_INIT4, 0xC0,
    EMM_CB_TEST0, 0x02,
    EMM_CB_TEST1, 0x00,
    EMM_CB_TEST2, 0x00,
    EMM_CB_TEST3, 0x00
};

const unsigned char res_emm_init2[]=
{
    EMM_CB_INIT0, 0xB8,
    EMM_CB_INIT1, 0x01,
    EMM_CB_INIT2, 0x10,
    EMM_CB_INIT3, 0xA8,
    EMM_CB_INIT4, 0xC0,
    EMM_CB_TEST0, 0x00,
    EMM_CB_TEST1, 0x00,
    EMM_CB_TEST2, 0x00,
    EMM_CB_TEST3, 0x00
};

const unsigned char emm_init4[]=
{
    EMM_CB_INIT1, 1,
    EMM_CB_INIT2, 1,
    EMM_CB_DRAM
};




// InitLcd is called to initialize the hardware
void EM6125::lcd_init(GSplash splash)
{

	LCD_MODULE::lcd_init(splash);

    memcpy(lcd_buf.cmd_buf, emm_init4, sizeof(lcd_buf.cmd_buf));

    //LCD Handle initialization (LCD attached to SPI)
//    lcd_hnd->tsk_write(emm_init1, sizeof(emm_init1));
//    tsk_sleep(160);


    //Splash screen..
    if(splash)
    {
    	splash(this);

        //dimish the splash
        memcpy(lcd_buf.video_buf, emm_init2, sizeof(emm_init2));
        do
    	{
    		lcd_buf.video_buf[7]++;
    		lcd_hnd->tsk_write(lcd_buf.video_buf, sizeof(emm_init2));
    		tsk_sleep(50);
    	} while (lcd_buf.video_buf[7] < 0xA8);
    }

}

void EM6125::lcd_reset()
{
	lcd_hnd->tsk_write(res_emm_init2, sizeof(res_emm_init2));
}

void EM6125::do_reset()
{
	if( ms_since(reset_timeout) > 500 )
	{
		reset_timeout = CURRENT_TIME;
		lcd_reset();
	}

}
void EM6125::draw_bitmap(unsigned int x0, unsigned int y0,
		const unsigned char* src, unsigned int width, unsigned int rows)
{
	y0 += 8;
	RenderGDIBitmap(lcd_buf.video_buf + (y0>>3) + (x0 * 11), 1<<(y0&7), src, width, rows);
}


void EM6125::draw_hline(unsigned int x0, unsigned int x1, unsigned int y)
{
	DrawHLine(x0, x1, y+8, lcd_buf.video_buf);
}

void EM6125::draw_bline(unsigned int x0, unsigned int x1, unsigned int y)
{
	y+= 8;
    x0 = x0*DISP_XSTEP + (y>>3);
    x1 = x1*DISP_XSTEP + (y>>3);
    y = ~(1<<(y&7));
	while(x0<=x1)
    {
       	lcd_buf.video_buf[x0] &= y;
        x0 += DISP_XSTEP;
    }
}

void EM6125::draw_vline(unsigned int y0, unsigned int y1, unsigned int x)
{
	DrawVLine(y0+8, y1+8, x, lcd_buf.video_buf);
}

void EM6125::invert_vline(unsigned int y0, unsigned int y1, unsigned int x)
{
	InvertVLine(y0+8, y1+8, x, lcd_buf.video_buf);
}

void EM6125::invert_hline(unsigned int x0, unsigned int x1, unsigned int y)
{
	InvertHLine(x0, x1, y+8, lcd_buf.video_buf);
}

void EM6125::update_screen()
{
	lcd_hnd->tsk_write(&lcd_buf, sizeof(lcd_buf));
}

void EM6125::clear_screen()
{
	memclr(lcd_buf.video_buf, sizeof(lcd_buf.video_buf));
}

//extern unsigned int cpu_usage;
#if USE_MEMORY_TRACKING
extern char end;
#endif

