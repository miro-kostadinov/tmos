/*
 * base_colors.h
 *
 * a support file containing macros and a struct for colored displays
 * control macros are marked with <- -> comments
 * the rest are mostly used for support (as infrastructure for controls)
 *
 *  Created on: Jun 16, 2023
 *      Author: viktor
 */

#ifndef SERVICES_GUI2_LCDS_BASE_COLORS_H_
#define SERVICES_GUI2_LCDS_BASE_COLORS_H_

#include <tmos.h>

#ifndef GUI_MONOCHROME
#define GUI_MONOCHROME	1	//<- ->
#endif

/**************************COLOR DEFINITIONS***********************************/
//ALL color macros use the same colors, predefined here in RGB565 format. max
//value for R and B is 0x1F (5 bits), max value for G is 0x3F (6 bits)
										//R		G	  B
#define TEMPLATE_COLOR_BLACK			0x00, 0x00, 0x00	//<-
#define TEMPLATE_COLOR_BLUE				0x00, 0x1F, 0x1F //0x00, 0x00, 0x1f
#define TEMPLATE_COLOR_GREEN			0x00, 0x3F, 0x00
#define TEMPLATE_COLOR_RED				0x1F, 0x00, 0x00
#define TEMPLATE_COLOR_LIGHTMAGENTA		0x0F, 0x08, 0x0E
#define TEMPLATE_COLOR_BROWN			0x08, 0x04, 0x01
#define TEMPLATE_COLOR_LIGHTBLUE		0x14, 0x34, 0x1F //original 0x0A, 0x0D, 0x0E
#define TEMPLATE_COLOR_LIGHTGRAY		0x14, 0x2A, 0x14 //66%, originally all 0x02
#define TEMPLATE_COLOR_DARKGRAY			0x0A, 0x15, 0x0A //33%, originally all 0x04
#define TEMPLATE_COLOR_LIGHTGREEN		0x09, 0x0E, 0x09
#define TEMPLATE_COLOR_LIGHTCYAN		0x0E, 0x0F, 0x0F
#define TEMPLATE_COLOR_LIGHTRED			0x0F, 0x08, 0x08
#define TEMPLATE_COLOR_CYAN				0x00, 0x3F, 0x1F
#define TEMPLATE_COLOR_MAGENTA			0x1F, 0x00, 0x1F
#define TEMPLATE_COLOR_YELLOW			0x1F, 0x3F, 0x00
#define TEMPLATE_COLOR_WHITE			0x1F, 0x3F, 0x1F	//->

//pixel format. these are used in the LCD controller itself
//00000001 GGGBBBBB 00000001 RRRRRGGG
#define PIX				0x01000100
#define PIX_R(x)		((x) << 3 )
#define PIX_G(x)		((((x)&07)<<21 ) | ((x)>>3))
#define PIX_B(x)		((x) << 16)
#define PIX_RGB(r,g,b)	(PIX | PIX_R(r) | PIX_G(g) | PIX_B(b))

//values stored in the RGB565 format
//RRRRRGGG GGGBBBBB
#define PIX_565(r,g,b)	(((r)<<11) | ((g)<<5) | (b))

//expansion macros
#define PIX_RGB_COLOR(x) PIX_RGB(x)
#define PIX_565_COLOR(x) PIX_565(x)


//pixel format generators
#define PIX_RGB_BLACK 			PIX_RGB_COLOR(TEMPLATE_COLOR_BLACK)
#define PIX_RGB_BLUE			PIX_RGB_COLOR(TEMPLATE_COLOR_BLUE)
#define PIX_RGB_GREEN			PIX_RGB_COLOR(TEMPLATE_COLOR_GREEN)
#define PIX_RGB_RED				PIX_RGB_COLOR(TEMPLATE_COLOR_RED)
#define PIX_RGB_LIGHTMAGENTA	PIX_RGB_COLOR(TEMPLATE_COLOR_LIGHTMAGENTA)
#define PIX_RGB_BROWN			PIX_RGB_COLOR(TEMPLATE_COLOR_BROWN)
#define PIX_RGB_LIGHTBLUE		PIX_RGB_COLOR(TEMPLATE_COLOR_LIGHTBLUE)
#define PIX_RGB_LIGHTGRAY		PIX_RGB_COLOR(TEMPLATE_COLOR_LIGHTGRAY)
#define PIX_RGB_DARKGRAY		PIX_RGB_COLOR(TEMPLATE_COLOR_DARKGRAY)
#define PIX_RGB_LIGHTGREEN		PIX_RGB_COLOR(TEMPLATE_COLOR_LIGHTGREEN)
#define PIX_RGB_LIGHTCYAN		PIX_RGB_COLOR(TEMPLATE_COLOR_LIGHTCYAN)
#define PIX_RGB_LIGHTRED		PIX_RGB_COLOR(TEMPLATE_COLOR_LIGHTRED)
#define PIX_RGB_CYAN			PIX_RGB_COLOR(TEMPLATE_COLOR_CYAN)
#define PIX_RGB_MAGENTA			PIX_RGB_COLOR(TEMPLATE_COLOR_MAGENTA)
#define PIX_RGB_YELLOW			PIX_RGB_COLOR(TEMPLATE_COLOR_YELLOW)
#define PIX_RGB_WHITE			PIX_RGB_COLOR(TEMPLATE_COLOR_WHITE)
#define PIX_RGB_ORANGE			PIX_RGB(0x1F, 0x1F, 0x00)	//unused

//565 format generators
#define PIX_565_BLACK			PIX_565_COLOR(TEMPLATE_COLOR_BLACK)
#define PIX_565_BLUE			PIX_565_COLOR(TEMPLATE_COLOR_BLUE)
#define PIX_565_GREEN			PIX_565_COLOR(TEMPLATE_COLOR_GREEN)
#define PIX_565_RED				PIX_565_COLOR(TEMPLATE_COLOR_RED)
#define PIX_565_LIGHTMAGENTA	PIX_565_COLOR(TEMPLATE_COLOR_LIGHTMAGENTA)
#define PIX_565_BROWN			PIX_565_COLOR(TEMPLATE_COLOR_BROWN)
#define PIX_565_LIGHTBLUE		PIX_565_COLOR(TEMPLATE_COLOR_LIGHTBLUE)
#define PIX_565_LIGHTGRAY		PIX_565_COLOR(TEMPLATE_COLOR_LIGHTGRAY)
#define PIX_565_DARKGRAY		PIX_565_COLOR(TEMPLATE_COLOR_DARKGRAY)
#define PIX_565_LIGHTGREEN		PIX_565_COLOR(TEMPLATE_COLOR_LIGHTGREEN)
#define PIX_565_LIGHTCYAN		PIX_565_COLOR(TEMPLATE_COLOR_LIGHTCYAN)
#define PIX_565_LIGHTRED		PIX_565_COLOR(TEMPLATE_COLOR_LIGHTRED)
#define PIX_565_CYAN			PIX_565_COLOR(TEMPLATE_COLOR_CYAN)
#define PIX_565_MAGENTA			PIX_565_COLOR(TEMPLATE_COLOR_MAGENTA)
#define PIX_565_YELLOW			PIX_565_COLOR(TEMPLATE_COLOR_YELLOW)
#define PIX_565_WHITE			PIX_565_COLOR(TEMPLATE_COLOR_WHITE)

enum tft_color_lut:unsigned char
{
	lut_black =0,
	lut_blue,
	lut_green,
	lut_red,
	lut_lightmagenta,
	lut_brown,
	lut_lightblue,
	lut_lightgray,
	lut_darkgray,
	lut_lightgreen,
	lut_lightcyan,
	lut_lightred,
	lut_cyan,
	lut_magenta,
	lut_yellow,
	lut_white
};

//these are used in the tft_buf to save memory
#define PIX_4BIT_BLACK			lut_black
#define PIX_4BIT_BLUE			lut_blue
#define PIX_4BIT_GREEN			lut_green
#define PIX_4BIT_RED		    lut_red
#define PIX_4BIT_LIGHTMAGENTA	lut_lightmagenta
#define PIX_4BIT_BROWN			lut_brown
#define PIX_4BIT_LIGHTBLUE		lut_lightblue
#define PIX_4BIT_LIGHTGRAY		lut_lightgray
#define PIX_4BIT_DARKGRAY		lut_darkgray
#define PIX_4BIT_LIGHTGREEN		lut_lightgreen
#define PIX_4BIT_LIGHTCYAN		lut_lightcyan
#define PIX_4BIT_LIGHTRED		lut_lightred
#define PIX_4BIT_CYAN			lut_cyan
#define PIX_4BIT_MAGENTA	    lut_magenta
#define PIX_4BIT_YELLOW			lut_yellow
#define PIX_4BIT_WHITE			lut_white


/**************************FORMAT DEFINITIONS**********************************/
#define PIX_4BIT_FORMAT			1
#define PIX_565_FORMAT			2
#define PIX_RGB_FORMAT			3

//can be PIX_4BIT_FORMAT, PIX_565_FORMAT or PIX_RGB_FORMAT. controls which color
//format is used
#ifndef PIX_FORMAT
#define PIX_FORMAT			PIX_4BIT_FORMAT		//<- ->
#endif

//pattern is used only for macro generation. do not use elsewhere
#if PIX_FORMAT == PIX_4BIT_FORMAT
#define PIX_FORMAT_PATTERN 4BIT
#else
#if PIX_FORMAT == PIX_565_FORMAT
#define PIX_FORMAT_PATTERN 565
#else
#if PIX_FORMAT == PIX_RGB_FORMAT
#define PIX_FORMAT_PATTERN RGB
#else
#error "PIX_FORMAT must be one of these: PIX_4BIT_FORMAT, PIX_565_FORMAT or PIX_RGB_FORMAT"
#endif // PIX_RGB_FORMAT
#endif // PIX_565_FORMAT
#endif // PIX_4BIT_FORMAT

#define PIX_CONCAT_MACRO(x, y)	PIX_ ## y ## _ ## x
#define PIX_EXPAND_MACRO(x, y)	PIX_CONCAT_MACRO(x, y)

//the color macros are converted to the desired format based on the PIX_FORMAT
//flag. PIX_FORMAT_PATTERN is always passed so it can be expanded before concatenated
#define PIX_BLACK			PIX_EXPAND_MACRO(BLACK, 		PIX_FORMAT_PATTERN)
#define PIX_BLUE			PIX_EXPAND_MACRO(BLUE, 			PIX_FORMAT_PATTERN)
#define PIX_GREEN			PIX_EXPAND_MACRO(GREEN, 		PIX_FORMAT_PATTERN)
#define PIX_RED				PIX_EXPAND_MACRO(RED, 			PIX_FORMAT_PATTERN)
#define PIX_LIGHTMAGENTA	PIX_EXPAND_MACRO(LIGHTMAGENTA, 	PIX_FORMAT_PATTERN)
#define PIX_BROWN			PIX_EXPAND_MACRO(BROWN, 		PIX_FORMAT_PATTERN)
#define PIX_LIGHTBLUE		PIX_EXPAND_MACRO(LIGHTBLUE, 	PIX_FORMAT_PATTERN)
#define PIX_LIGHTGRAY		PIX_EXPAND_MACRO(LIGHTGRAY, 	PIX_FORMAT_PATTERN)
#define PIX_DARKGRAY		PIX_EXPAND_MACRO(DARKGRAY, 		PIX_FORMAT_PATTERN)
#define PIX_LIGHTGREEN		PIX_EXPAND_MACRO(LIGHTGREEN, 	PIX_FORMAT_PATTERN)
#define PIX_LIGHTCYAN		PIX_EXPAND_MACRO(LIGHTCYAN, 	PIX_FORMAT_PATTERN)
#define PIX_LIGHTRED		PIX_EXPAND_MACRO(LIGHTRED, 		PIX_FORMAT_PATTERN)
#define PIX_CYAN			PIX_EXPAND_MACRO(CYAN, 			PIX_FORMAT_PATTERN)
#define PIX_MAGENTA			PIX_EXPAND_MACRO(MAGENTA, 		PIX_FORMAT_PATTERN)
#define PIX_YELLOW			PIX_EXPAND_MACRO(YELLOW, 		PIX_FORMAT_PATTERN)
#define PIX_WHITE			PIX_EXPAND_MACRO(WHITE, 		PIX_FORMAT_PATTERN)

template<typename scolor_t, bool type_t>
struct s_colors;

template <typename scolor_t>
struct s_colors<scolor_t, false>
{

protected:
		//if the LCD supports color we store two colors in each GObject
	struct{
		scolor_t bg_color;
		scolor_t fg_color;
	}__attribute__((packed));
public:
	s_colors(scolor_t fg, scolor_t bg)
		:bg_color(bg)
		,fg_color(fg)
	{;}

	inline __attribute__((optimize("Os"), always_inline))
	void set_color(scolor_t fg_color_t)
	{
		fg_color = fg_color_t;
	}
	inline __attribute__((optimize("Os"), always_inline))
	void set_background(scolor_t background)
	{
		bg_color = background;
	}
	inline __attribute__((optimize("Os"), always_inline))
	void set_colors(scolor_t fg_color_t, scolor_t bg_color_t)
	{
		fg_color = fg_color_t;
		bg_color = bg_color_t;
	}
	__attribute__((optimize("Os"), always_inline))
	scolor_t get_fg_color()
	{
		return fg_color;
	}
	__attribute__((optimize("Os"), always_inline))
	scolor_t get_bg_color()
	{
		return bg_color;
	}
};


template <typename scolor_t>
struct s_colors<scolor_t, true>
{
	s_colors()
	{;}
	s_colors(scolor_t bg, scolor_t fg)
	{;}

	inline __attribute__((optimize("Os"), always_inline))
	void set_color(scolor_t fg_color_t)
	{
		;
	}
	inline __attribute__((optimize("Os"), always_inline))
	void set_background(scolor_t background)
	{
		;
	}
	inline __attribute__((optimize("Os"), always_inline))
	void set_colors(scolor_t fg_color_t, scolor_t bg_color_t)
	{
		;
	}
	__attribute__((optimize("Os"), always_inline))
	scolor_t get_fg_color()
	{
		return PIX_WHITE;
	}
	__attribute__((optimize("Os"), always_inline))
	scolor_t get_bg_color()
	{
		return PIX_BLACK;
	}
};


//colors struct changes according to the monochrome and format flags. most funcs
//outside of here that deal with colors use unsigned int to pass them as arguments

typedef
#if PIX_FORMAT == PIX_RGB_FORMAT
unsigned int
#elif PIX_FORMAT == PIX_565_FORMAT
unsigned short
#elif PIX_FORMAT == PIX_4BIT_FORMAT
unsigned char
#endif
color_t;

typedef struct s_colors<color_t, GUI_MONOCHROME>  u_colors_t;

#endif /* SERVICES_GUI2_LCDS_BASE_COLORS_H_ */
