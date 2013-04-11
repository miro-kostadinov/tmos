/*
 * menus.c
 *
 *  Created on: 2008-10-19
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <lcd.h>
#include <drivers.h>
#include <menus.h>

//extern void DEVICE_SHUTDOWN(void);

//*----------------------------------------------------------------------------
//*			Menu functions
//*----------------------------------------------------------------------------

MENU_DECLARE_STATIC(menu1, 1)
MENU_DECLARE_STATIC(menu2, 2)
MENU_DECLARE_STATIC(menu3, 3)
MENU_DECLARE_STATIC(menu4, 4)
MENU_DECLARE_STATIC(menu5, 5)
MENU_DECLARE_STATIC(menu6, 6)
MENU_DECLARE_STATIC(menu7, 7)
MENU_DECLARE_STATIC(menu8, 8)
MENU_DECLARE_STATIC(menu9, 9)


//*----------------------------------------------------------------------------
//*			Menu Tools->keys test
//*----------------------------------------------------------------------------

#define ROW1	9
#define ROW2	19
#define ROW3	29
#define ROW4	39

const unsigned char key_positions [] =
{
		ROW2, 01, 01+8,		// <
		ROW1, 17, 17+8,		// U
		ROW3, 17, 17+8,		// D
		ROW2, 33, 33+8,		// >
		ROW4, 24, 24+18,	// NO
		ROW4, 01, 01+16,	// OK
};

extern unsigned int volatile btns_pressed;

static void DrawKeyD(LCD_MODULE* lcd)
{
	unsigned int i, mask, y, start, end;

	mask = 1;
	for(i=0; i<6; i++)
	{
		if(mask & btns_pressed)
		{
			start = key_positions[1 + (3*i)];
			end = key_positions[2 + (3*i)];

			for(y=key_positions[3*i]; y < (unsigned)(key_positions[3*i]+10); y++)
				lcd->invert_hline(start, end, y);
		}
		mask <<=1;
	}
}
RES_CODE keytest_cb(WINDOW obj, unsigned int param, unsigned int msg)
{
	unsigned int pressed;

	pressed = ~*obj->dst.as_intptr;
    if(msg == WM_DRAW)
    {
    	LCD_MODULE* lcd = (LCD_MODULE*)param;

		lcd->clear_screen();

		if( pressed & (1 << KEY_UP))
		{
			lcd->set_xy_all(10 + (16<<16), ALL_LEFT);
			lcd->draw_text("U");
		}
		if( pressed & (1 << KEY_DOWN))
		{
			lcd->set_xy_all(30 + (16<<16), ALL_LEFT);
			lcd->draw_text("D");
		}
		if( pressed & (1 << KEY_OK))
		{
			lcd->set_xy_all(40 , ALL_LEFT);
			lcd->draw_text("OK");
		}
		if( pressed & (1 << KEY_C))
		{
			lcd->set_xy_all(40 + (24<<16), ALL_LEFT);
			lcd->draw_text("NO");
		}

		lcd->set_xy_all(65, ALL_CENTER);
		lcd->draw_text("press all");
		lcd->draw_text("keys");
		DrawKeyD(lcd);
	    return 0;
    }

    if(msg == WM_KEY)
    {

        if((param & 0xE0) == 0)
		{
        	if(!*obj->dst.as_intptr)
        		return RES_SIG_OK;
        	*obj->dst.as_intptr &= ~(1<<param);
		}
		return FLG_BUSY;
    }

    return 0;
}


unsigned int keyboard_test(void)
{
	CWindow key_dialg;
	unsigned int res;

	res = 0x3f;

	if(key_dialg.tsk_window_init(keytest_cb))
	{
		key_dialg.tsk_read(&res, 0);
	}
    return 0;
}



//*----------------------------------------------------------------------------
//*			Menu Settings->back light time
//*----------------------------------------------------------------------------

extern volatile unsigned int backlight_time;
unsigned int menu_set_backlight(void *p, MENU_WINDOW menu_hnd)
{
	CSTRING str;

	str.format("%u", backlight_time);
	if( get_box("backlight", str) == RES_OK)
		backlight_time = str.atoi();
	return 0;
}



//RES_CODE msgbox_cb(CMessageBox* msg_hnd, unsigned int param, unsigned int msg);

//*----------------------------------------------------------------------------
//*			Menu structures
//*----------------------------------------------------------------------------


const menu2_type settings_menu =
{
	2,
	{"Settings", "Настройки"},
	NULL,
	{
		{0, {"Backlight", "Подсветка"}, (MENU_CBF)menu_set_backlight, NULL },
		{0, {"keys test", "Тест бутони"}, (MENU_CBF)keyboard_test, NULL}
	}
};


const menu1_type main_menu =
{
    1,
    {"Main menu", "Главно меню"},
    "main.menu",
    {
		{0, {"Settings", "Настройки"}, (MENU_CBF)menu_box, (void *)&settings_menu}
    }
};


//*----------------------------------------------------------------------------
//*			Menu task
//*----------------------------------------------------------------------------

void menu_thread(void)
{
	CMenu menu_hnd;
	unsigned int  i;

    ALLOCATE_SIGNAL(MAIN_MENU_SIG | UART_MENU_SIG);
	CSTRING password, input;

    current_laguage = 0;

    menu_hnd.tsk_window_init((GUI_CB)menu_cb);

	menu_hnd.displays = 2;
//    if(gui_driver.lcd[1])
//    	menu_hnd.displays = 2;

    for(;;)
    {

        i = tsk_get_signal(MAIN_MENU_SIG | UART_MENU_SIG);
        if (i & MAIN_MENU_SIG)
        {
       		menu_box((const MENU_STRU*)&main_menu, &menu_hnd);
        }
    }
}

TASK_DECLARE_STATIC(menu_task, "MENT", menu_thread, 2, 400+TRACE_SIZE);

extern "C" void init_main_menu(void)
{
	usr_task_init_static(&menu_task_desc, true);
}
