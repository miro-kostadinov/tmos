#ifndef	LCD_H
#define	LCD_H

#include <hardware_cpp.h>
#include <fonts.h>
#include <gcontainer.h>

//#define ALL_LEFT		0
//#define ALL_RIGHT 		1
//#define ALL_CENTER		2

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

#define MSB2LSB(x)			(						  \
								(((x)&0x001)?0x100:0) | \
								(((x)&0x002)?0x080:0) | \
								(((x)&0x004)?0x040:0) | \
								(((x)&0x008)?0x020:0) | \
								(((x)&0x010)?0x010:0) | \
								(((x)&0x020)?0x008:0) | \
								(((x)&0x040)?0x004:0) | \
								(((x)&0x080)?0x002:0) | \
								(((x)&0x100)?0x001:0)   \
							)
typedef void (* GSplash )(LCD_MODULE* lcd);

struct LCD_MODULE: public GContainer
{

	struct
	{
		short pos_x; //!< current draw pos
		short pos_y; //!< current draw pos
		unsigned short size_x;
		unsigned short size_y;
		unsigned short dot_pitch_x; //!< in mm x 1000
		unsigned short dot_pitch_y; //!< in mm x 1000
		unsigned short chars_per_row;
		unsigned short allign;
		const RENDER_MODE* font;
		HANDLE lcd_hnd;
		const PIN_DESC* pins;
		unsigned int color;
	} __attribute__((packed));
	RECT_T	frame;

public:
	LCD_MODULE(	unsigned int x, unsigned int y,
				unsigned int dx, unsigned int dy,
				HANDLE hnd, const PIN_DESC* p) :
		size_x(x), size_y(y), dot_pitch_x(dx), dot_pitch_y(dy), lcd_hnd(hnd), pins(p)
	{
		rect = RECT_T (0, 0, x - 1, y - 1);
		color = PIX_WHITE;
	}
	;
	virtual ~LCD_MODULE(){};

	GUI_GET_OBJECT_TYPE(OBJECT_DISPLAY);

	virtual void lcd_init(GSplash splash);
	virtual void lcd_reset()=0;
	virtual void backlight_signal(void);

	virtual void update_screen()=0;
	virtual void clear_screen()=0;
	void redraw_screen(GObject* object, RECT_T area) override =0 ;
	void invalidate(GObject* object, RECT_T area) override;
	virtual void redraw_rect (GObject* object, RECT_T area){};
	virtual void adjust_for_screen (GObject** object, RECT_T &area){};

	void set_font(const RENDER_MODE* afont);
	void set_xy_all(unsigned int xy, unsigned int all);
	void clear_rect (const RECT_T& area);
	const char* get_next_txt_row(const char *txt) const;
	const char* draw_text(const char *txt);
	const char* draw_text_no_space(const char *txt);
	void draw_icon (unsigned char icon);
	const char* draw_row(const char *txt);
	virtual void direct_write(GSplash draw_cb){};
	void LPtoDP(POINT_T& size, unsigned char lcd_index=0) override;
	void DPtoLP(POINT_T& size, unsigned char lcd_index=0) override;
	void set_color(unsigned int fg_color)
	{
		color = fg_color;
	}
};

struct GClientLcd : GObject
{
	GClientLcd(GObject* ptr): GObject(){client_rect = ptr->client_rect;}
	~GClientLcd(){RelaseLcd();}
	bool CreateLcd(RECT_T& area, LCD_MODULE* lcd);
	bool RelaseLcd( );
private:
	void draw_this(LCD_MODULE* lcd) override
	{
		TRACE_ERROR("GUI Error in draw_this");
	}
};
#endif
