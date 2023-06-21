#ifndef	LCD_H
#define	LCD_H

#include <stdgui.h>
#include <hardware_cpp.h>

#define SIG_BACKLIGHT_TASK	1

/*
#define PIX		0x01000100
//pixel format
// 00000001 GGGBBBBB 00000001 RRRRRGGG
#define PIX_R(x)	((x) << 3 )
#define PIX_G(x)	( (((x)&07)<<21 ) | ((x)>>3))
#define PIX_B(x)	((x) << 16)
#define PIX_RGB(r,g,b)	(PIX | PIX_R(r) | PIX_G(g) | PIX_B(b))

#define PIX_RGB_BLACK 			PIX_RGB(0x00, 0x00, 0x00)
#define PIX_RGB_BLUE			PIX_RGB(0x00, 0x00, 0x1f)
#define PIX_RGB_GREEN			PIX_RGB(0x00, 0x3F, 0x00)
#define PIX_RGB_CYAN			PIX_RGB(0x00, 0x3F, 0x1F)
#define PIX_RGB_RED				PIX_RGB(0x1F, 0x00, 0x00)
#define PIX_RGB_MAGENTA			PIX_RGB(0x1F, 0x00, 0x1F)
#define PIX_RGB_BROWN			PIX_RGB(0x08, 0x04, 0x01)
#define PIX_RGB_LIGHTGRAY		PIX_RGB(0x04, 0x04, 0x04)
#define PIX_RGB_DARKGRAY		PIX_RGB(0x02, 0x02, 0x02)
#define PIX_RGB_LIGHTBLUE		PIX_RGB(0x0A, 0x0D, 0x0E)
#define PIX_RGB_LIGHTGREEN		PIX_RGB(0x09, 0x0E, 0x09)
#define PIX_RGB_LIGHTCYAN		PIX_RGB(0x0E, 0x0F, 0x0F)
#define PIX_RGB_LIGHTRED		PIX_RGB(0x0F, 0x08, 0x08)
#define PIX_RGB_LIGHTMAGENTA	PIX_RGB(0x0F, 0x08, 0x0E)
#define PIX_RGB_YELLOW			PIX_RGB(0x1F, 0x3F, 0x00)
#define PIX_RGB_WHITE			PIX_RGB(0x1F, 0x3F, 0x1F)
#define PIX_RGB_ORANGE			PIX_RGB(0x1F, 0x1F, 0x00)

//RGB565 values of colors, compatible with TFT LCDs and 4bit colors
//RRRRRGGG GGGBBBBB
#define PIX_565_BLACK			0
#define PIX_565_BLUE			0x001F
#define PIX_565_GREEN			0x07E0
#define PIX_565_CYAN			0x07FF
#define PIX_565_RED				0xF800
#define PIX_565_MAGENTA			0xF81F
#define PIX_565_BROWN			0x4081
#define PIX_565_LIGHTGRAY		0x2084
#define PIX_565_DARKGRAY		0x1042
#define PIX_565_LIGHTBLUE		0x51AE
#define PIX_565_LIGHTGREEN		0x49C9
#define PIX_565_LIGHTCYAN		0x71EF
#define PIX_565_LIGHTRED		0x7908
#define PIX_565_LIGHTMAGENTA	0x790E
#define PIX_565_YELLOW			0xFFE0
#define PIX_565_WHITE			0xFFFF

#define PIX_4BIT_BLACK			lut_black
#define PIX_4BIT_BLUE			lut_blue
#define PIX_4BIT_GREEN			lut_green
#define PIX_4BIT_CYAN			lut_cyan
#define PIX_4BIT_RED		    lut_red
#define PIX_4BIT_MAGENTA	    lut_magenta
#define PIX_4BIT_BROWN			lut_brown
#define PIX_4BIT_LIGHTGRAY		lut_lightgray
#define PIX_4BIT_DARKGRAY		lut_darkblue
#define PIX_4BIT_LIGHTBLUE		lut_lightblue
#define PIX_4BIT_LIGHTGREEN		lut_lightgreen
#define PIX_4BIT_LIGHTCYAN		lut_lightcyan
#define PIX_4BIT_LIGHTRED		lut_lightred
#define PIX_4BIT_LIGHTMAGENTA	lut_lightmagenta
#define PIX_4BIT_YELLOW			lut_yellow
#define PIX_4BIT_WHITE			lut_white

//can be RGB, 565 or 4BIT. controls which color format is used
#ifndef PIX_FORMAT
#define PIX_FORMAT			565
#endif

#define PIX_CONCAT_MACRO(x, y)	PIX_ ## y ## _ ## x
#define PIX_EXPAND_MACRO(x, y)	PIX_CONCAT_MACRO(x, y)

//the color macros are converted to the desired format based on the PIX_FORMAT
//flag. PIX_FORMAT is always passed so it can be expanded before concatenated
#define PIX_BLACK			PIX_EXPAND_MACRO(BLACK, PIX_FORMAT)
#define PIX_BLUE			PIX_EXPAND_MACRO(BLUE, PIX_FORMAT)
#define PIX_GREEN			PIX_EXPAND_MACRO(GREEN, PIX_FORMAT)
#define PIX_CYAN			PIX_EXPAND_MACRO(CYAN, PIX_FORMAT)
#define PIX_RED				PIX_EXPAND_MACRO(RED, PIX_FORMAT)
#define PIX_MAGENTA			PIX_EXPAND_MACRO(MAGENTA, PIX_FORMAT)
#define PIX_BROWN			PIX_EXPAND_MACRO(BROWN, PIX_FORMAT)
#define PIX_LIGHTGRAY		PIX_EXPAND_MACRO(LIGHTGRAY, PIX_FORMAT)
#define PIX_DARKGRAY		PIX_EXPAND_MACRO(DARKGRAY, PIX_FORMAT)
#define PIX_LIGHTBLUE		PIX_EXPAND_MACRO(LIGHTBLUE, PIX_FORMAT)
#define PIX_LIGHTGREEN		PIX_EXPAND_MACRO(LIGHTGREEN, PIX_FORMAT)
#define PIX_LIGHTCYAN		PIX_EXPAND_MACRO(LIGHTCYAN, PIX_FORMAT)
#define PIX_LIGHTRED		PIX_EXPAND_MACRO(LIGHTRED, PIX_FORMAT)
#define PIX_LIGHTMAGENTA	PIX_EXPAND_MACRO(LIGHTMAGENTA, PIX_FORMAT)
#define PIX_YELLOW			PIX_EXPAND_MACRO(YELLOW, PIX_FORMAT)
#define PIX_WHITE			PIX_EXPAND_MACRO(WHITE, PIX_FORMAT)
*/


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

//pixel LSB format
// 0000000G | GGBBBBB1 | 0000000G | GGRRRRR1
//        0 | 1201234_ |        3 | 4501234_
#define PIX2LSB_RGB(x)	((MSB2LSB(x>>16) << 16) | MSB2LSB(x))

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
		//draw colors are only allocated if the GUI is monochrome, otherwise they
		//are inherited from GObject. same names for compatibility
#if GUI_MONOCHROME
		unsigned int bg_color;
		union{
			unsigned int fg_color;
			unsigned int color;		//alias for fg_color
		};
#endif
	} __attribute__((packed));
	RECT_T	frame;

public:
	LCD_MODULE(	unsigned int x, unsigned int y,
				unsigned int dx, unsigned int dy,
				HANDLE hnd, const PIN_DESC* p) :
		size_x(x), size_y(y), dot_pitch_x(dx), dot_pitch_y(dy), lcd_hnd(hnd), pins(p)
	{
		rect = RECT_T (0, 0, x - 1, y - 1);
#if GUI_MONOCHROME
		color = PIX_WHITE;
		bg_color = PIX_BLACK;
#endif
	}
	;
	virtual ~LCD_MODULE(){};

	GUI_GET_OBJECT_TYPE(OBJECT_DISPLAY);

	virtual void lcd_init(GSplash splash);
	virtual void lcd_reset()=0;
	virtual void backlight_signal(void);

	virtual void update_screen()=0;
	virtual void clear_screen()=0;
	virtual void do_reset()=0;
	virtual void draw_bitmap(int x0, int y0, const char* src, int width, int rows) =0;
	virtual void draw_char(int x0, unsigned int ch) =0;

	void redraw_screen(GObject* object, RECT_T area) override =0;
	void invalidate(GObject* object, RECT_T area) override;
	virtual void redraw_rect (GObject* object, RECT_T area);
	virtual void adjust_for_screen (GObject** object, RECT_T &area)
		{;}

	void set_font(const RENDER_MODE* afont);
	void set_xy_all(unsigned int xy, unsigned int all);
	void clear_rect (const RECT_T& area) override;
	const char* get_next_txt_row(const char *txt) const;
	const char* draw_text(const char *txt);
	const char* draw_text_no_space(const char *txt);
	void draw_icon (guistd_icons_index icon);
	const char* draw_row(const char *txt);
	virtual void direct_write(GSplash draw_cb){};
	void LPtoDP(POINT_T& size, unsigned char lcd_index=0) const override;
	void DPtoLP(POINT_T& size, unsigned char lcd_index=0) const override;
	POINT_T PolarToDP(const int deg, const int r, const unsigned char lcd_index=0) const override;

#if GUI_MONOCHROME
	inline void set_color(unsigned int fg_color_t)__attribute__((optimize("Os"), always_inline)) //override
	{
		color = fg_color_t;
	}
#endif
protected:
	inline virtual unsigned int is_lcd() const override __attribute__((optimize("Os"), always_inline))
	{
		return 1;
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

template <class T, uint32_t min_size>
class stack
{
private:
	uint32_t sp;
	T*		 memory;

	void stack_alloc()
	{
		unsigned int size;

		size = (sp+1)*sizeof(T);

		if( !memory || ( dyn_sizeof((void*)memory) < size) )
		{
			void* mem;
			do{
				mem = tsk_realloc(memory, size + min_size*sizeof(T));
				if(!mem)
				{
					tsk_sleep(10);
					GUI_ASSERT((T*)mem);
				}
			}while(!mem);
			memory = (T*)mem;
		}
	}

public:
	stack() :
		sp(0)
		,memory(nullptr)
	{;}
	~stack()
	{
		if(memory)
		{
			delete[] memory;
			memory = nullptr;
		}
	}

	void push(const T& data)
	{
		stack_alloc();
		memory[sp++] = data;
	}

	T pop()
	{
		T ret;
		if(sp)
			ret = memory[--sp];
		return ret;
	}

	bool empty() const
	{
		return (sp == 0);
	}

	const T& top() const
	{
		if(sp)
			return memory[sp];
		return RECT_T();
	}
};

#endif
