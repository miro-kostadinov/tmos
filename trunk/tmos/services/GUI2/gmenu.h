/*
 * gmenu.h
 *
 *  Created on: 03.10.2013
 *      Author: stanly
 */

#ifndef GMENU_H_
#define GMENU_H_

#include <gobject.h>
#include <gscroll.h>


typedef struct
{
	int 		parent;
	int 		item;
	const char*	item_name;
} MENUTEMPLATE;


struct GEdit;
struct GListBox;

struct GMenu: GObject
{
	friend GEdit;
	friend GListBox;

	MENUTEMPLATE*	base;
	MENUTEMPLATE*	menu;
	MENUTEMPLATE*	item;
	const char*		title;
	int 			size;
	GScroll* 		scroll;
	const RENDER_MODE*	text_font;

	GMenu (GId id_t, RECT_T rect_t, const char* title_t, GFlags flags_t = GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t),
		 base(NULL), menu(NULL), item(NULL), title(title_t),
		 size(0), scroll(NULL), text_font(&FNT5x7)
	{}

	GMenu(): base(NULL), menu(NULL), item(NULL), title(NULL), size(0),
			scroll(NULL), text_font(&FNT5x7)
	{}

	~GMenu()
	{
		if(scroll)
		{
			delete scroll;
			scroll = NULL;
		}
		if(base)
		{
			delete base;
			base = NULL;
		}
	}
	bool AppendMenu(int parent_id, int menu_id, const char* menu_name);
	bool LoadMenu(const MENUTEMPLATE* pat);

	MENUTEMPLATE* GetItem(int parent_id, int menu_id);
	MENUTEMPLATE* FindItem(int item_id);
	MENUTEMPLATE* GetMenu(int parent_id, MENUTEMPLATE* start = NULL);
	int GetMenuSize(int menu_id);
	bool SetReplaceItem(int item_id, const char* item_name);
	bool Select(int item_id);

protected:
	virtual void draw_this (LCD_MODULE* lcd);
	virtual unsigned int process_key (GMessage& msg);
	virtual unsigned int initialize (GMessage& msg);
private:
	bool process_selected();
	bool set_scroll(void);
	bool add_item(int parent_id, int item_id, const char* name);
	int get_item_pos(MENUTEMPLATE* ptr);

};

unsigned int remove_amp(CSTRING& str);
#endif /* GMENU_H_ */
