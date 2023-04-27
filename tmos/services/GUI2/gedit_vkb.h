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
#include <gbutton.h>


//contains the text at the top of the screen
struct GVKB_edit : GEdit
{
	GVKB_edit (GId id_t, const RECT_T& rect_t, CSTRING txt_t, GFlags flags_t = GO_FLG_DEFAULT)
		:GEdit (id_t, rect_t, txt_t, nullptr, flags_t, ES_LEFT|ES_MIDDLE|ES_AUTO_SCROLL)
	{;}

protected:
	unsigned int process_key(GMessage& msg) override;
};


//for keeping positions on the KB
struct KBPos
{
	union{
		struct{
			short int x;
			short int y;
		};
		int as_int;
	}__attribute__((packed));


	KBPos(short int x_t, short int y_t): x(x_t), y(y_t) {;}
	KBPos(): as_int(0) {;}

	inline bool operator!=(const KBPos& p1){
		return (p1.as_int != as_int);
	}
};

//contains an alphabet (symbols of one "language") and can draw it
struct GVKB_keyboard : GObject
{
	friend GEditVKB;

	CSTRING alphabet;	//contains the currently displayed alphabet
	KBPos cursor_pos;	//represents the cursor position on the KB
	KBPos max_pos;		//the maximum position for the cursor on this page and alphabet
	struct{
	unsigned char rows, cols;		//num of rows and cols of symbols, used when calculating positions
	unsigned char page, max_page;	//used to indicate current page and max page number
	} __attribute__((packed));


	GVKB_keyboard (GId id_t, const RECT_T& rect_t, GFlags flags_t = GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t),
		 rows(0), cols(0), page(0), max_page(0)
	{;}

	void draw_this (LCD_MODULE* lcd) override;

	void fill_kb();
	void reinitialize();
	inline char getc();

protected:
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_key(GMessage& msg) override;
};


//struct for the OK, ABC and X texts
struct GVKB_button : GButton
{
	GVKB_button (GId id_t, const RECT_T& rect_t, unsigned int cmd_id, const char* txt_t,
		GFlags flags_t = GO_FLG_DEFAULT)
		: GButton (id_t, rect_t, cmd_id, txt_t, flags_t)

	{;}

	void draw_border(RECT_T& frame) override
	{/* do nothing */;}
	void draw_this (LCD_MODULE* lcd) override;

};

struct GVKB_Controls : GContainer
{
	const RENDER_MODE* font;
	GVKB_Controls(GId id_t, const RECT_T& rect_t, GFlags flags_t= GO_FLG_DEFAULT)
		:GContainer(id_t, rect_t, flags_t)
		,font(nullptr)
	{;}

	virtual ~GVKB_Controls()
	{;}
protected:
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_command(GMessage& msg)	override;
	unsigned int process_key(GMessage& msg)	override;
	unsigned int process_default(GMessage& msg)	override;
};

struct GEditVKB : GContainer
{
	GEdit *	base_edit;	//pointer to the GEdit whose text we are editing
	key_mode shift;		//current type of letters
	const RENDER_MODE* font;	//font size for all text on the VKB


	GEditVKB(GEdit* base) :
		GContainer(), base_edit(base),
		shift(base->shift), font(base->text_font)
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
	unsigned int process_command(GMessage& msg)	override;
};

#endif /* GEDIT_VKB_H_ */
