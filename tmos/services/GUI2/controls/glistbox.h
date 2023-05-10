/*
 * glistbox.h
 *
 *  Created on: 22.11.2013
 *      Author: stanly
 */

#ifndef GLISTBOX_H_
#define GLISTBOX_H_

#include <stdgui.h>
#include <gtext.h>
#include <gmenu.h>

struct GListBoxMenu : GMenu
{
	GListBoxMenu (GId id_t, const RECT_T& rect_t, const char* title_t, GFlags flags_t = GO_FLG_DEFAULT)
		:GMenu( id_t, rect_t, title_t, flags_t)
	{}

	GListBoxMenu():GMenu()
	{}

	POINT_T get_border_size(void) const override;
	void draw_border(RECT_T& frame) override;
};

struct GListBox:GText
{
private:
	GListBoxMenu*	list;
public:
	GListBox (GId id_t, const RECT_T& rect_t,
			CSTRING txt_t, const char* caption_t=nullptr,
			GFlags flags_t = GO_FLG_DEFAULT,
			unsigned short align_t = LBS_DEFAULT,
			const RENDER_MODE* font_t = &FNT5x7):
			GText (id_t, rect_t, txt_t, caption_t, flags_t, align_t, font_t),
			list(nullptr)
	{;}
	~GListBox();

	GUI_GET_OBJECT_TYPE(OBJECT_LISTBOX);

	bool addItem(int num, const char* name);
	bool select(int num);

	POINT_T get_border_size(void) const override;
	void draw_border(RECT_T& frame) override;
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_key (GMessage& msg) override;
	void draw_this (LCD_MODULE* lcd) override;

};

#endif /* GLISTBOX_H_ */
