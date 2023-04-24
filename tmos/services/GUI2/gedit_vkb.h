/*
 * gedit_vkb.h
 *
 * On some devices with only 6 buttons, we use GEditVKB to enter text.
 * It is created when the user presses OK on a GEdit field if that GEdit has
 * the ES_USE_VIRTUAL_KB flag. It is a virtual keyboard with on-screen buttons,
 * where the arrow keys are used to traverse them and OK to add them to the text
 *
 *  Created on: Mar 22, 2023
 *      Author: bratkov
 */

#ifndef GEDIT_VKB_H_
#define GEDIT_VKB_H_

#include <tmos.h>
#include <gui_drv.h>
#include <gcontainer.h>
#include <gedit.h>


//contains the text at the top of the screen
struct GText_VKTXT : GObject
{
	friend GEditVKB;

	CSTRING txt;
	unsigned int edit_pos;		//position of the cursor on the text
	unsigned char max_shown_c;	//how many maximum chars On Screen

	GText_VKTXT (GId id_t, const RECT_T& rect_t, const char* txt_t, GFlags flags_t = GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t),
		 txt(txt_t), edit_pos(0), max_shown_c(0)
	{;}

	void insert_c(char c);
	void erase_c();

	void draw_this (LCD_MODULE* lcd) override;

protected:
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_key(GMessage& msg) override;
};


//for keeping positions on the KB
struct KBPos
{
	unsigned char x;
	unsigned char y;

	KBPos(unsigned char x_t, unsigned char y_t): x(x_t), y(y_t) {;}
	KBPos(): x(0), y(0) {;}

	inline bool operator!=(const KBPos& p1){
		return ((p1.x!=x || p1.y!= y)?true:false);
	}
};

//contains an alphabet (symbols of one "language") and can draw it
struct GText_KB : GObject
{
	friend GEditVKB;

	CSTRING alphabet;	//contains the currently displayed alphabet
	unsigned char rows, cols;	//num of rows and cols of symbols, used when calculating positions
	KBPos cursor_pos;	//represents the cursor position on the KB
	KBPos max_pos;		//the maximum position for the cursor on this page and alphabet
	unsigned char page, max_page;	//used to indicate current page and max page number


	GText_KB (GId id_t, const RECT_T& rect_t, const char* alphabet_t, GFlags flags_t = GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t),
		 alphabet(alphabet_t), rows(0), cols(0), cursor_pos(), max_pos(), page(0), max_page(0)
	{;}

	void draw_this (LCD_MODULE* lcd) override;

	void fill_kb();
	inline char getc();

protected:
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_key(GMessage& msg) override;
};


//struct for the OK, ABC and X texts
struct GText_CTRL : GObject
{
	CSTRING txt;


	GText_CTRL (GId id_t, const RECT_T& rect_t, const char* txt_t,
		GFlags flags_t = GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t), txt(txt_t)
	{;}

	void draw_this (LCD_MODULE* lcd) override;

protected:
	unsigned int initialize (GMessage& msg) override;
};


enum zone_type:unsigned char {VKB_ZONE_TXT=1, VKB_ZONE_KB, VKB_ZONE_CTRL_1,
	VKB_ZONE_CTRL_2, VKB_ZONE_CTRL_3, VKB_ZONE_MENU};

struct GEditVKB : GContainer
{
	GEdit *	base_edit;	//pointer to the GEdit whose text we are editing
	GText_KB * kb;		//pointer to the keyboard object (child)
	zone_type zone;		//represents the "zone" of the screen we're in
	key_mode shift;		//current type of letters
	GMenu* edit_menu;	//pointer for language menu, only used when changing it
	const RENDER_MODE* font;	//font size for all text on the VKB


	GEditVKB(GEdit* base) :
		GContainer(), base_edit(base), kb(nullptr), zone(VKB_ZONE_KB),
		shift(base->shift), edit_menu(nullptr), font(base->text_font)
	{;}

	virtual ~GEditVKB()
	{;}

	bool CreateVKB();
	void exit_ok();
	void exit_cancel();

	GUI_GET_OBJECT_TYPE(OBJECT_VKB);

protected:
	// GUI driver methods
	friend void gui_thread(GUI_DRIVER_INFO* drv_info);
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_key(GMessage& msg) override;

	unsigned int process_key_txt(GMessage& msg);
	unsigned int process_key_kb(GMessage& msg);
	unsigned int process_key_ctrl(GMessage& msg);
};

#endif /* GEDIT_VKB_H_ */
