#ifndef	LCD_H
#define	LCD_H

#include <stdgui.h>
#include <hardware_cpp.h>

#define SIG_BACKLIGHT_TASK	1

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
	} __attribute__((packed));
	RECT_T	frame;
#if GUI_MONOCHROME
protected:
	struct{
		color_t fg_color;
		color_t bg_color;
	}__attribute__((packed));
#endif

public:
	LCD_MODULE(	unsigned int x, unsigned int y,
				unsigned int dx, unsigned int dy,
				HANDLE hnd, const PIN_DESC* p) :
		size_x(x), size_y(y), dot_pitch_x(dx), dot_pitch_y(dy), lcd_hnd(hnd), pins(p)
	{
		rect = RECT_T (0, 0, x - 1, y - 1);
#if GUI_MONOCHROME
		fg_color = PIX_WHITE;
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
	__attribute__((optimize("Os"), always_inline))
	void set_color(color_t fg_color_t) //shadows u_colors
	{
		if(fg_color_t != PIX_BLACK)
			fg_color = PIX_WHITE;
		else
			fg_color = PIX_BLACK;
	}
	__attribute__((optimize("Os"), always_inline))
	void set_background(color_t  background) //shadows u_colors
	{
		if(background != PIX_BLACK)
			bg_color = PIX_WHITE;
		else
			bg_color = PIX_BLACK;
	}
	__attribute__((optimize("Os"), always_inline))
	void set_colors(color_t fg_color_t, color_t bg_color_t)
	{
		set_color(fg_color_t);
		set_background(bg_color_t);
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
