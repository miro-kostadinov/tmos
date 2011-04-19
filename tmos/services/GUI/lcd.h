#ifndef	LCD_H
#define	LCD_H

#include <tmos_drivers.h>
#include <fonts.h>
#include <stdgui.h>

#define ALL_LEFT		0
#define ALL_RIGHT 		1
#define ALL_CENTER		2

#define SIG_BACKLIGHT_TASK	1

#define PIX		0x01000100
//pixel format
// 00000001 GGGBBBBB 00000001 RRRRRGGG
#define PIX_R(x)	(x << 3 )
#define PIX_G(x)	( ((x&07)<<21 ) | (x>>3))
#define PIX_B(x)	(x << 16)
#define PIX_RGB(r,g,b)	(PIX | PIX_R(r) | PIX_G(g) | PIX_B(b))

#define PIX_BLACK 			PIX_RGB(0x00, 0x00, 0x00)
#define PIX_BLUE			PIX_RGB(0x00, 0x00, 0x1f)
#define PIX_GREEN			PIX_RGB(0x00, 0x3F, 0x00)
#define PIX_CYAN			PIX_RGB(0x00, 0x3F, 0x1F)
#define PIX_RED				PIX_RGB(0x1F, 0x00, 0x00)
#define PIX_MAGENTA			PIX_RGB(0x1F, 0x00, 0x1F)
#define PIX_BROWN			PIX_RGB(0x08, 0x04, 0x01)
#define PIX_LIGHTGRAY		PIX_RGB(0x04, 0x04, 0x04)
#define PIX_DARKGRAY		PIX_RGB(0x02, 0x02, 0x02)
#define PIX_LIGHTBLUE		PIX_RGB(0x0A, 0x0D, 0x0E)
#define PIX_LIGHTGREEN		PIX_RGB(0x09, 0x0E, 0x09)
#define PIX_LIGHTCYAN		PIX_RGB(0x0E, 0x0F, 0x0F)
#define PIX_LIGHTRED		PIX_RGB(0x0F, 0x08, 0x08)
#define PIX_LIGHTMAGENTA	PIX_RGB(0x0F, 0x08, 0x0E)
#define PIX_YELLOW			PIX_RGB(0x1F, 0x3F, 0x00)
#define PIX_WHITE			PIX_RGB(0x1F, 0x3F, 0x1F)
#define PIX_ORANGE			PIX_RGB(0x1F, 0x1F, 0x00)

#define BKLT_PIN_INDX	0
#define RST_PIN_INDX	1

struct LCD_MODULE
{
	unsigned short pos_x; //!< current draw pos
	unsigned short pos_y; //!< current draw pos
	unsigned short size_x;
	unsigned short size_y;
	unsigned short frame_y0;
	unsigned short frame_y1;
	unsigned short chars_per_row;
	unsigned short allign;
	const RENDER_MODE* font;
	unsigned int color;
	HANDLE lcd_hnd;
	const PIN_DESC* pins;

	LCD_MODULE(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p) :
		size_x(x), size_y(y), lcd_hnd(hnd), pins(p)
	{
	}
	;

	virtual void lcd_init(GUI_CB splash);
	virtual void lcd_reset()=0;
	virtual void backlight_signal(void);

	virtual void draw_bitmap(unsigned int x0, unsigned int y0,
			const unsigned char* src, unsigned int width, unsigned int rows)=0;
	virtual void draw_hline(unsigned int x0, unsigned int x1, unsigned int y)=0;
	virtual void draw_bline(unsigned int x0, unsigned int x1, unsigned int y)=0;
	virtual void draw_vline(unsigned int y0, unsigned int y1, unsigned int x)=0;
	virtual void
	invert_vline(unsigned int y0, unsigned int y1, unsigned int x)=0;
	virtual void
	invert_hline(unsigned int x0, unsigned int x1, unsigned int y)=0;
	virtual void update_screen()=0;
	virtual void clear_screen()=0;
	virtual void redraw_screen(WINDOW desktop)=0;

	void set_font(const RENDER_MODE* afont);
	void set_xy_all(unsigned int xy, unsigned int all);
	void clear_rect(unsigned int x0, unsigned int y0, unsigned int x1,
			unsigned int y1);
	const char* draw_text(const char *txt);
	void lcd_single_window(GUI_CB callback);
};

#endif
