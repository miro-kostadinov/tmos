#ifndef	EMM_LCD_H
#define	EMM_LCD_H

#include <lcd.h>

#define DISP_XSIZE	102
#define DISP_YSIZE	81
#define DISP_XSTEP	11

typedef struct LCD_BUF_t
{
	unsigned char cmd_buf[5];
	unsigned char video_buf[DISP_XSIZE * DISP_XSTEP];
} LCD_BUF_STRU;

struct EM6125: public LCD_MODULE
{
	unsigned int reset_timeout;
	LCD_BUF_STRU lcd_buf;

	EM6125(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p) :
		LCD_MODULE(x, y, hnd, p), reset_timeout(0)
	{
	}
	;

	//virtual functions
	void lcd_init(GUI_CB splash) override;
	void lcd_reset() override;
	void draw_bitmap(unsigned int x0, unsigned int y0,
			const unsigned char* src, unsigned int width, unsigned int rows) override;
	void draw_hline(unsigned int x0, unsigned int x1, unsigned int y) override;
	void draw_bline(unsigned int x0, unsigned int x1, unsigned int y) override;
	void draw_vline(unsigned int y0, unsigned int y1, unsigned int x) override;
	void invert_vline(unsigned int y0, unsigned int y1, unsigned int x) override;
	void invert_hline(unsigned int x0, unsigned int x1, unsigned int y) override;
	void update_screen() override;
	void clear_screen() override;
	void redraw_screen(WINDOW desktop) override;
};

#endif
