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

#define GMENU_FLG_CHECK_ITEM			0x0100
#define GMENU_FLG_DISABLE_ITEM			0x0200
#define GMENU_FLG_SHOW_SELECTED_ITEM	0x0400 //!< always shows the selected item, independent of menu flags

struct MENUTEMPLATE
{
	int 		parent;
	short unsigned int	item;
	short unsigned int	flags;
	const char*	item_name;
};

struct menu_template_t
{
	int 		parent;
	short unsigned int 	item;
	short unsigned int	flags;
	CSTRING		item_name;
};


struct GEdit;
struct GEditVKB;
struct GListBox;

struct GMenu: GObject
{
	friend GEdit;
	friend GEditVKB;
	friend GListBox;

	menu_template_t*	base;
	menu_template_t*	menu;
	menu_template_t*	item;
	const char*		title;
	int 			size;
	GScroll* 		scroll;
	const RENDER_MODE*	text_font;

	GMenu (GId id_t, const RECT_T& rect_t, const char* title_t, GFlags flags_t = GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t),
		 base(nullptr), menu(nullptr), item(nullptr), title(title_t),
		 size(0), scroll(nullptr), text_font(&FNT5x7)
	{}

	GMenu(): base(nullptr), menu(nullptr), item(nullptr), title(nullptr), size(0),
			scroll(nullptr), text_font(&FNT5x7)
	{}

	~GMenu()
	{
		if(scroll)
		{
			delete scroll;
			scroll = nullptr;
		}
		if(base)
		{
			size =0;
			while( !IsEmpty(base + size) )
			{
				base[size++].item_name.free();
			}
			delete base;
			base = nullptr;
		}
	}

	void free()
	{
		if(scroll)
		{
			delete scroll;
			scroll = nullptr;
		}
		if(base)
		{
			size =0;
			while(	!IsEmpty(base + size) )
			{
				base[size++].item_name.free();
			}
			delete base;
			base = nullptr;
		}
		item = menu = nullptr;
		size=0;
	}

	GUI_GET_OBJECT_TYPE(OBJECT_MENU);

	virtual bool AppendMenu(int parent_id, int item_id, const CSTRING& item_name, short unsigned int flg =0);
	bool LoadMenu(const MENUTEMPLATE* pat);

	menu_template_t* GetItem(int parent_id, int item_id);
	menu_template_t* FindItem(int item_id);
	menu_template_t* GetMenu(int parent_id, menu_template_t* start = nullptr);
	int GetMenuSize(int menu_id) const;
	bool SetReplaceItem(int item_id, const CSTRING& item_name, short unsigned int flg =0);
	bool Select(int item_id, bool redraw = false);
	virtual bool RemoveItem(int item_id);
	bool InsertItem(int item_id, int new_item_id, const CSTRING& new_item_name, short unsigned int new_flg =0);

protected:
	void draw_this (LCD_MODULE* lcd) override;
	unsigned int process_key (GMessage& msg) override;
	unsigned int initialize (GMessage& msg) override;

	static bool IsEmpty(menu_template_t* ptr)
	{
		return (!ptr->item && !ptr->parent && ptr->item_name.empty());
	}
	bool process_selected();
	int get_item_pos(menu_template_t* ptr);
//private:
	virtual bool set_scroll(void);
	bool add_item(int parent_id, int item_id, const CSTRING& name, short unsigned int flg =0);
	void adjust_item_names();
	int get_base_size() const;
};

unsigned int remove_amp(CSTRING& str);
#endif /* GMENU_H_ */
