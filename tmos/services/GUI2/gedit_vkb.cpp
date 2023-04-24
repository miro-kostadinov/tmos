/*
 * gedit_vkb.cpp
 *
 *  Created on: Mar 22, 2023
 *      Author: bratkov
 */
#include <tmos.h>
#include <gedit_vkb.h>
#include <gedit.h>
#include <gwindow.h>
#include <lcd_multiplex.h>
#include <gedit_vkb.h>


#define VKB_TXT_OFFSET	1	//num of empty pixel rows below TXT zone
#define VKB_CTRL_OFFSET	1	//num of empty pixel rows above CTRL zone
#define VKB_TXT_START	3	//starting pixel for drawing the text in GText_VKTXT

#define pfont ((GEditVKB*)parent)->font
#define vktxt ((GText_VKTXT*)children)


/**
 * @param shift_t: key_mode
 * @return: the string from g_keyboard_menu corresponding to shift_t,
 * or "ABC" by default
 */
const char* get_shift_str(key_mode shift_t)
{
	for(int kbIndex = 0; g_keyboard_menu[kbIndex].item &&
			g_keyboard_menu[kbIndex].item_name != nullptr; ++kbIndex)
		if(g_keyboard_menu[kbIndex].item == shift_t)
		{
			if(strlen(g_keyboard_menu[kbIndex].item_name) >= 6)
				return (g_keyboard_menu[kbIndex].item_name + 3);
			break;
		}

	return "ABC";
}


//---------- GTextVKB ----------//

/**
 * adds a VKB to the screen Z-order and calls its initialize()
 * @return true
 */
bool GEditVKB::CreateVKB()
{
	//1. add to Z-order
	nextObj = Gdesktop->parent->focus->nextObj;
	Gdesktop->parent->focus->nextObj = this;
	//2. Set LCD and send WM_INIT to the new virtual keyboard
	parent = Gdesktop->parent;
#if GUI_DISPLAYS > 1
	GQueue.push(GMessage(WM_INIT, 0, (long long)((LCD_MULT *)(Gdesktop->parent))->lcd, this));
#else
	GQueue.push(GMessage(WM_INIT, 0, (long long)((LCD_MODULE *)(Gdesktop->parent)), this));
#endif
	return true;
}

/**
 * set up container, create and add all children to it
 */
unsigned int GEditVKB::initialize (GMessage& msg)
{
#if GUI_DISPLAYS > 1
	LCD_MODULE* lcd = ((LCD_MODULE **)msg.lparam)[0];
#else
	LCD_MODULE* lcd = (LCD_MODULE *)msg.lparam;
#endif

	client_rect = rect = lcd->rect;  // LCD rectangle, get all display
	//displays = 1; // use main display, already set to 1 from GObject()
	flags = GO_FLG_ENABLED | GO_FLG_SHOW | GO_FLG_SELECTED;
	base_edit->max_len = 30;	//TODO !!! remove after testing

	//text is always the first child
	//H is font H + 2 pixels on each side, -1 because it's inclusive
	addChild(new GText_VKTXT(30, RECT_T(0, 0, rect.x1, font->height + 3),
			base_edit->txt.c_str(), GO_FLG_DEFAULT|GO_FLG_BORDER));

	//use this empty container to draw a border around the CTRL texts, nothing else
	//H is font H + 2 pixels on each side, -1 because it's inclusive
	addChild(new GContainer(31, RECT_T(0, rect.y1 - (font->height + 3), rect.x1, rect.y1),
			GO_FLG_DEFAULT|GO_FLG_BORDER));

	short int abcLen = font->hspacing * 3 + font->hdistance;
	//X: center of the screen, from (len - abcLen)/2 to (len + abcLen)/2
	//Y: start 1 lower than CTRL rectangle start
	addChild(new GText_CTRL(36, RECT_T((rect.x1-abcLen)>>1, rect.y1 - (font->height + 2),
			((rect.x1+abcLen)>>1) - font->hdistance, rect.y1 - 1), get_shift_str(shift), GO_FLG_DEFAULT));

	//X: start from pixel 4 (text starts on 5), end on that + 2*(symbol spacing)
	//Y: start 1 lower than CTRL rectangle start
	addChild(new GText_CTRL(37, RECT_T(4, rect.y1 - (font->height + 2), 4 + 2*font->hspacing, rect.y1 - 1),
			"OK", GO_FLG_DEFAULT));

	//X: end 3 pixels from the end, start (symbol spacing) before that
	//Y: start 1 lower than CTRL rectangle start
	addChild(new GText_CTRL(38, RECT_T(rect.x1 - 3 - font->hspacing, rect.y1 - (font->height + 2),
			rect.x1 - 3, rect.y1 - 1), "X", GO_FLG_DEFAULT));

	//Y: start 1 lower than TXT, end 1 higher than CTRL (offsets used only in draw_this)
	addChild(new GText_KB(39, RECT_T(0, font->height + 4, rect.x1, rect.y1 - (font->height + 4)),
			" ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÜÞß"));



	unsigned int res = GContainer::initialize(msg);
	kb = (GText_KB*)children->nextObj->nextObj->nextObj->nextObj->nextObj;
	send_message(WM_DRAW, 0, 0L, this);
	return res;
}

/**
 * handles the MENU zone keys, otherwise calls the respective functions
 */
unsigned int GEditVKB::process_key(GMessage& msg)
{
	if(msg.param & KEY_UP_CODE)
		return 0;

	switch(zone)
	{
	case VKB_ZONE_TXT:
		return process_key_txt(msg);

	case VKB_ZONE_KB:
		return process_key_kb(msg);

	case VKB_ZONE_CTRL_1:
	case VKB_ZONE_CTRL_2:
	case VKB_ZONE_CTRL_3:
		return process_key_ctrl(msg);

	case VKB_ZONE_MENU:
		if(edit_menu)
		{
			switch(msg.param)
			{
			case KEY_OK:
				if(edit_menu->item)
				{
					//get chosen shift from the menu, change the abc text to match
					shift = (key_mode)edit_menu->item->item;
					GText_CTRL* abcObj = (GText_CTRL*) children->nextObj->nextObj;
					abcObj->txt = get_shift_str(shift);

					//change the zone to KB and remove cursor from abc text
					zone = VKB_ZONE_KB;
					abcObj->flags &= ~GO_FLG_SELECTED;

					//set all positions to default and fill the KB
					kb->cursor_pos.x = 0;
					kb->cursor_pos.y = 0;
					kb->page = 0;
					kb->fill_kb();
				}
				break;

			case KEY_ESC:
			case KEY_CANCEL:
				zone = VKB_ZONE_CTRL_2;
				break;

			default:
				if(edit_menu->process_key(msg))
					return 1;
				return 0;
			}
			edit_menu->remove(); //close();
			send_message(WM_DRAW, 0, 0L, this);
			delete edit_menu;
			edit_menu = nullptr;
			return 1;
		}
	}

	return 0;
}

/**
 * some keys are processed here, the others from calling GText_VKTXT::process_key()
 */
unsigned int GEditVKB::process_key_txt(GMessage& msg)
{
	switch(msg.param)
	{
	case KEY_UP:
		return 1;

	case KEY_DOWN:
		zone = VKB_ZONE_KB;
		send_message(WM_DRAW, 0, 0L, this);
		return 1;

	case KEY_LEFT:
	case KEY_RIGHT:
		return vktxt->process_key(msg);

	case KEY_ESC:
	case KEY_CANCEL:
		vktxt->erase_c();
		return 1;

	case KEY_OK:
		exit_ok();
		return 1;
	}

	return 0;
}

/**
 * Key processing is passed to the KB first. In case it wasn't successful,
 * process it here
 */
unsigned int GEditVKB::process_key_kb(GMessage& msg)
{
	//try to process the key in the KB
	if(kb->process_key(msg))
		return 1;	//return if the it was processed successfully

	//otherwise process it here
	switch(msg.param)
	{
	case KEY_UP:
		zone = VKB_ZONE_TXT;
		send_message(WM_DRAW, 0, 0L, this);
		return 1;

	case KEY_DOWN:
		zone = VKB_ZONE_CTRL_2;
		children->nextObj->nextObj->flags |= GO_FLG_SELECTED; //draw the cursor
		send_message(WM_DRAW, 0, 0L, this);
		return 1;

	case KEY_OK:
		vktxt->insert_c(kb->getc());
		return 1;

	case KEY_ESC:
	case KEY_CANCEL:
		vktxt->erase_c();
		return 1;
	}

	return 0;
}

/**
 * arrow keys change the zone and SELECTED flags ("cursor") as needed, OK selects
 */
unsigned int GEditVKB::process_key_ctrl(GMessage& msg)
{
	switch(msg.param)
	{
	case KEY_UP:
		zone = VKB_ZONE_KB;
		children->nextObj->nextObj->flags &= ~GO_FLG_SELECTED; //remove cursor
		children->nextObj->nextObj->nextObj->flags &= ~GO_FLG_SELECTED;
		children->nextObj->nextObj->nextObj->nextObj->flags &= ~GO_FLG_SELECTED;
		send_message(WM_DRAW, 0, 0L, this);
		return 1;

	case KEY_DOWN:
		return 1;

	case KEY_LEFT:
		switch(zone)
		{
		case VKB_ZONE_CTRL_1:
			return 1;

		case VKB_ZONE_CTRL_2:
			zone = VKB_ZONE_CTRL_1;
			children->nextObj->nextObj->flags &= ~GO_FLG_SELECTED;
			children->nextObj->nextObj->nextObj->flags |= GO_FLG_SELECTED;
			send_message(WM_DRAW, 0, children->nextObj->rect.as_int, this);
			return 1;

		case VKB_ZONE_CTRL_3:
			zone = VKB_ZONE_CTRL_2;
			children->nextObj->nextObj->nextObj->nextObj->flags &= ~GO_FLG_SELECTED;
			children->nextObj->nextObj->flags |= GO_FLG_SELECTED;
			send_message(WM_DRAW, 0, children->nextObj->rect.as_int, this);
			return 1;

		default:
			return 0;
		}

	case KEY_RIGHT:
		switch(zone)
		{
		case VKB_ZONE_CTRL_1:
			zone = VKB_ZONE_CTRL_2;
			children->nextObj->nextObj->nextObj->flags &= ~GO_FLG_SELECTED;
			children->nextObj->nextObj->flags |= GO_FLG_SELECTED;
			send_message(WM_DRAW, 0, children->nextObj->rect.as_int, this);
			return 1;

		case VKB_ZONE_CTRL_2:
			zone = VKB_ZONE_CTRL_3;
			children->nextObj->nextObj->flags &= ~GO_FLG_SELECTED;
			children->nextObj->nextObj->nextObj->nextObj->flags |= GO_FLG_SELECTED;
			send_message(WM_DRAW, 0, children->nextObj->rect.as_int, this);
			return 1;

		case VKB_ZONE_CTRL_3:
			return 1;

		default:
			return 0;
		}

	case KEY_OK:
		switch(zone)
		{
		case VKB_ZONE_CTRL_1:
			exit_ok();
			return 1;

		case VKB_ZONE_CTRL_2:
			if(!edit_menu)
			{
				edit_menu = new GMenu(10, rect, nullptr, GO_FLG_DEFAULT|GO_FLG_BORDER);
				if(edit_menu)
				{
					POINT_T bs = get_border_size();
					int max = (5 * edit_menu->text_font->hspacing + GO_SCROLL_WIDTH+1 + 2*bs.x + 3);
					if(edit_menu->rect.width() > max)
					{
						edit_menu->rect.x0 = rect.x0 +((rect.width() - max)>>1);
						edit_menu->rect.x1 = edit_menu->rect.x0 + max;
					}

					edit_menu->LoadMenu(g_keyboard_menu);
					max = max(edit_menu->size, 1);
					max = max*(edit_menu->text_font->vspacing + edit_menu->text_font->vdistance) + 2*bs.y;
					if(rect.height() > max)
					{
						edit_menu->rect.y0 = rect.y0 +((rect.height() - max)>>1);
						edit_menu->rect.y1 = edit_menu->rect.y0 + max;
					}

					addChild(edit_menu);
					send_message(WM_INIT, 0, 0L, edit_menu);
					if(!(edit_menu->item = edit_menu->FindItem(shift)))
						edit_menu->item = edit_menu->menu;
					send_message(WM_DRAW, 0, edit_menu->rect.as_int, parent);

					zone = VKB_ZONE_MENU;
				}
			}
			return 1;

		case VKB_ZONE_CTRL_3:
			exit_cancel();
			return 1;

		default:
			return 0;
		}

	case KEY_ESC:
	case KEY_CANCEL:
		zone = VKB_ZONE_KB;
		kb->cursor_pos.x = 0;
		kb->cursor_pos.y = 0;
		children->nextObj->nextObj->flags &= ~GO_FLG_SELECTED; //remove cursor
		children->nextObj->nextObj->nextObj->flags &= ~GO_FLG_SELECTED;
		children->nextObj->nextObj->nextObj->nextObj->flags &= ~GO_FLG_SELECTED;
		send_message(WM_DRAW, 0, 0L, this);
		return 0;
	}

	return 0;
}

/**
 * destroy the VKB container and its children, redraw the screen. if there was
 * a difference from the original GEdit text, change it to the new one
 */
void GEditVKB::exit_ok()
{
	//TODO add OK exit
	if(base_edit->txt != vktxt->txt)
	{
		unsigned int tlen = base_edit->txt.length();
		base_edit->txt = vktxt->txt;
		base_edit->pos_change(vktxt->txt.length() - tlen, true);
		base_edit->text_change();
		base_edit->shift = shift;
	}
	send_message(WN_DESTROY, 0, 0L, this);
	send_message(WM_DRAW, 0, rect.as_int, parent);
}

/**
 * destroy the VKB container and its children, redraw the screen
 */
void GEditVKB::exit_cancel()
{
	//TODO add cancel exit
	send_message(WN_DESTROY, 0, 0L, this);
	send_message(WM_DRAW, 0, rect.as_int, parent);
}


//---------- GText_VKTXT ----------//

/**
 * set cursor after the last char and calculate max_shown_c
 */
unsigned int GText_VKTXT::initialize (GMessage& msg)
{
	client_rect = rect;

	edit_pos = txt.length();	//start editing after last char

	//(full rect width - padding on both sides - hdistance for cursor on both sides) /
	//hspacing. hdistance is included just once because hspacing already contains the other
	max_shown_c = (unsigned char) ((rect.width() + 1 - 2*VKB_TXT_START - pfont->hdistance) /
			pfont->hspacing);

	return 0;
}

/**
 * left and right key are processed here. called from process_key_kb() as it
 * shouldn't have focus
 */
unsigned int GText_VKTXT::process_key(GMessage& msg)
{
	switch(msg.param)
	{
	case KEY_LEFT:
		if(edit_pos)
		{
			edit_pos--;
			send_message(WM_DRAW, 0, rect.as_int, this);
		}
		return 1;

	case KEY_RIGHT:
		if(edit_pos < txt.length())
		{
			edit_pos++;
			send_message(WM_DRAW, 0, rect.as_int, this);
		}
		return 1;
	}

	return 0;
}

/**
 * inserts the passed char into the text (where the cursor is), moves the cursor
 * forward and redraws
 */
void GText_VKTXT::insert_c(char c)
{
	if(txt.length() < ((GEditVKB*)parent)->base_edit->max_len)
	{
		txt.insert(c, edit_pos);
		edit_pos++;
		send_message(WM_DRAW, 0, rect.as_int, this);
	}
}

/**
 * erases the symbol before the cursor, moves the cursor back and redraws.
 * if the cursor is at the start of the text, erase the first symbol instead
 */
void GText_VKTXT::erase_c()
{
	if(txt.length())
	{
		if(edit_pos)
			txt.erase(--edit_pos, 1);
		else
			txt.erase(edit_pos, 1);

		send_message(WM_DRAW, 0, rect.as_int, this);
	}
}

/**
 * draws the symbols and the cursor. if there are more symbols than can be
 * displayed, only draws as many as possible and close to the cursor
 */
void GText_VKTXT::draw_this (LCD_MODULE* lcd)
{
	if(flags & GO_FLG_BORDER)
		draw_border(rect);

	if(client_rect.height() > 0 && client_rect.width() > 0)
	{
		lcd->set_color(PIX_WHITE);

		lcd->set_font(pfont);
		lcd->allign = (SS_LEFT|SS_TOP);
		lcd->pos_x = client_rect.x0;
		lcd->pos_y = 2;

		unsigned int tempPos = VKB_TXT_START + lcd->font->hdistance;
		if(txt.length() <= max_shown_c || edit_pos <= max_shown_c)
		{
			//text shorter than screen or cursor in first part of text, print
			//the cursor on its pos
			for(unsigned int i = 0; i < txt.length() && i < max_shown_c; ++i)
			{
				lcd->draw_char(tempPos, txt[i]);
				tempPos += lcd->font->hspacing;
			}

			lcd->draw_rectangle(RECT_T(POINT_T(VKB_TXT_START + edit_pos*lcd->font->hspacing, 1),
					lcd->font->hdistance - 1, lcd->font->height + 2), true);
		}
		else if(edit_pos == txt.length())
		{
			//otherwise, if the cursor is at the end of the text, print the last
			//symbols and cursor at the end
			for(unsigned int i = txt.length()-max_shown_c; i < txt.length(); ++i)
			{
				lcd->draw_char(tempPos, txt[i]);
				tempPos += lcd->font->hspacing;
			}

			lcd->draw_rectangle(RECT_T(POINT_T(VKB_TXT_START + max_shown_c*lcd->font->hspacing, 1),
					lcd->font->hdistance - 1, lcd->font->height + 2), true);
		}
		else
		{
			//otherwise, print the cursor before the last showing symbol and
			//print the other symbols accordingly
			for(unsigned int i = edit_pos - (max_shown_c - 1); i <= edit_pos; ++i)
			{
				lcd->draw_char(tempPos, txt[i]);
				tempPos += lcd->font->hspacing;
			}

			lcd->draw_rectangle(RECT_T(POINT_T(VKB_TXT_START + (max_shown_c-1)*lcd->font->hspacing, 1),
					lcd->font->hdistance - 1, lcd->font->height + 2), true);
		}
	}
}


//---------- GText_KB ----------//

/**
 * set up variables and call fill_kb()
 */
unsigned int GText_KB::initialize (GMessage& msg)
{
	client_rect = rect;

	//calculate number of rows and cols on the keyboard
	//cols = (num pixels -1 starting padding) / character hspacing
	cols = (unsigned char) ((rect.x1 - rect.x0) / pfont->hspacing);
	//rows = (num pixels - TXT offset - CTRL offset) / character vspacing
	rows = (unsigned char) (((rect.y1 - rect.y0 + 1) + 1 - VKB_TXT_OFFSET - VKB_CTRL_OFFSET) / pfont->vspacing);

	if(rows < 1 || cols < 1)
	{ //a VKB cannot be created, close it
		((GEditVKB*)parent)->exit_cancel();
		return 1;
	}

	fill_kb();

	return 0;
}

/**
 * usually called from GEditVKB::process_key(), as this doesn't have focus.
 * redraws the KB in case of page change
 */
unsigned int GText_KB::process_key(GMessage& msg)
{
	if(msg.param & KEY_UP_CODE)
		return 0;

	switch(msg.param)
	{
	case KEY_OK:
	case KEY_ESC:
	case KEY_CANCEL:
		return 0;

	case KEY_UP:
		if(cursor_pos.y)
		{
			cursor_pos.y--;
			send_message(WM_DRAW, 0, rect.as_int, this);
			return 1;
		}
		return 0;

	case KEY_DOWN:
		if(cursor_pos.y < max_pos.y - 1 ||
				(cursor_pos.y < max_pos.y && cursor_pos.x <= max_pos.x))
		{
			cursor_pos.y++;
			send_message(WM_DRAW, 0, rect.as_int, this);
			return 1;
		}
		return 0;

	case KEY_LEFT:
		if(cursor_pos.x)
		{
			cursor_pos.x--;
			send_message(WM_DRAW, 0, rect.as_int, this);
			return 1;
		}
		if(page)
		{
			//page change
			page--;
			cursor_pos.x = 0;
			cursor_pos.y = 0;
			fill_kb(); //draws the new KB page
			return 1;
		}
		return 1;

	case KEY_RIGHT:
		if((cursor_pos.y != max_pos.y && cursor_pos.x + 1 < cols) ||
				(cursor_pos.y == max_pos.y && cursor_pos.x < max_pos.x))
		{
			cursor_pos.x++;
			send_message(WM_DRAW, 0, rect.as_int, this);
			return 1;
		}
		if(page < max_page)
		{
			//page change
			page++;
			cursor_pos.x = 0;
			cursor_pos.y = 0;
			fill_kb(); //draws the new KB page
			return 1;
		}
		return 1;
	}

	return 0;
}

/**
 * draws the symbols on the keyboard (from the current page)
 * if the VKB zone is on the KB, also draw the cursor
 */
void GText_KB::draw_this (LCD_MODULE* lcd)
{
	if(client_rect.height() > 0 && client_rect.width() > 0)
	{
		lcd->set_color(PIX_WHITE);

		lcd->set_font(pfont);
		lcd->allign = (SS_LEFT|SS_TOP);
		lcd->pos_x = client_rect.x0;
		//rectangle takes more space but we want pixels between TXT and KB
		lcd->pos_y = client_rect.y0 + VKB_TXT_OFFSET; //so we use offset

		//draw alphabet on screen
		unsigned int alphaIndex = 0;
		int pos_x = 1;	//start from 2nd pixel
		KBPos tempPos;
		do
		{
			if(tempPos.x == cols)	//end of row reached
			{
				tempPos.x = 0;
				pos_x = 1;
				tempPos.y++;
				lcd->pos_y += lcd->font->vspacing;
			}

			lcd->draw_char(pos_x, alphabet[alphaIndex]);
			pos_x += lcd->font->hspacing;
		}
		while(tempPos != max_pos && ++alphaIndex < alphabet.length() && ++(tempPos.x) >= 0);

		//draw cursor if this is the current zone
		if(((GEditVKB*)parent)->zone == VKB_ZONE_KB)
			for(uint8_t i = 0; i <= lcd->font->height + 1; ++i)
				lcd->invert_hline(cursor_pos.x*lcd->font->hspacing, cursor_pos.x*lcd->font->hspacing +
						lcd->font->width + 1, rect.y0 + cursor_pos.y*lcd->font->vspacing + i);
	}
}

/**
 * gets the whole alphabet from the current shift, places the part
 * that needs to be displayed in alphabet var, and draws it. calculates the
 * values of max_page and max_pos. variable "page" is controlled in process_key()
 */
void GText_KB::fill_kb()
{
	CSTRING full_alphabet(" ");	//alphabets start with a space
	if(full_alphabet != nullptr)
	{
		//get alphabet
		key_mode shift = ((GEditVKB*)parent)->shift;
		switch(shift)
		{
		case KT_BG:
		case KT_BG_CAPS:
			full_alphabet += g_key_to_char_ref[8].bg_vals;
			full_alphabet += g_key_to_char_ref[9].bg_vals;
			for(unsigned char i = 4; i < 7; ++i)
				full_alphabet += g_key_to_char_ref[i].bg_vals;
			for(unsigned char i = 1; i < 4; ++i)
				full_alphabet += g_key_to_char_ref[i].bg_vals;
			break;
		case KT_EN:
		case KT_EN_CAPS:
			full_alphabet += g_key_to_char_ref[8].en_vals;
			full_alphabet += g_key_to_char_ref[9].en_vals;
			for(unsigned char i = 4; i < 7; ++i)
				full_alphabet += g_key_to_char_ref[i].en_vals;
			for(unsigned char i = 1; i < 4; ++i)
				full_alphabet += g_key_to_char_ref[i].en_vals;
			break;
		case KT_DIGIT:
			full_alphabet += "0123456789.";
			full_alphabet += g_key_to_char_ref[0].bg_vals;
			full_alphabet += g_key_to_char_ref[7].bg_vals;
			full_alphabet += g_key_to_char_ref[12].bg_vals;
			break;
		}

		full_alphabet += full_alphabet;
		full_alphabet += full_alphabet;	//TODO !!! remove after testing

		if(full_alphabet != nullptr && full_alphabet.length() > 1)
		{
			if(shift == KT_BG_CAPS || shift == KT_EN_CAPS)	//upper letters if needed
				for(unsigned int i = 1; i < full_alphabet.length(); ++i)
					full_alphabet[i] = toupper(full_alphabet[i]);

			//get the desired part of the alphabet and display it
			if(alphabet != nullptr)
			{
				unsigned int startPos = page * rows*cols;
				alphabet = full_alphabet.substr(startPos, min((unsigned) (rows*cols), full_alphabet.length() - startPos));
				//calculate max positions, div by 0 should be impossible here
				max_page = (unsigned char) ((full_alphabet.length() - 1) / (rows*cols));
				max_pos.x = (unsigned char) ((alphabet.length() - 1) % cols);
				max_pos.y = (unsigned char) ((alphabet.length() - 1) / cols);
				send_message(WM_DRAW, 0, client_rect.as_int, this);
				return;
			}
		}
	}
	//in case of error
	alphabet = "ERR";
	cursor_pos.x = 0;
	cursor_pos.y = 0;
	max_pos.x = 2;
	max_pos.y = 0;
	page = 0;
	max_page = 0;
}

/**
 * @return the selected char (the one under the "cursor"), using cursor_pos
 */
inline char GText_KB::getc()
{
	return alphabet[cursor_pos.y*cols + cursor_pos.x];
}


//---------- GText_CTRL ----------//

unsigned int GText_CTRL::initialize (GMessage& msg)
{
	client_rect = rect;

	return 0;
}

/**
 * draws the CTRL texts as it's easier to fit them (there is no align here).
 * also inverts the "selected" text if it has the GO_FLG_SELECTED flag
 */
void GText_CTRL::draw_this(LCD_MODULE* lcd)
{
	if(client_rect.height() > 0 && client_rect.width() > 0)
	{
		lcd->set_color(PIX_WHITE);

		lcd->set_font(pfont);
		lcd->allign = (SS_LEFT|SS_TOP);

		int tempPos;

		lcd->pos_x = client_rect.x0;
		lcd->pos_y = client_rect.y0 + 1;
		tempPos = client_rect.x0 + 1; //start from (1, 1)

		for(unsigned int cIndex = 0; cIndex < txt.length() && tempPos <= client_rect.x1; ++cIndex)
		{
			lcd->draw_char(tempPos, txt[cIndex]);
			tempPos += lcd->font->hspacing;
		}

		//draw cursor only if this has GO_FLG_SELECTED
		if(flags & GO_FLG_SELECTED)
			for(tempPos = client_rect.y0; tempPos <= client_rect.y1; ++tempPos)
				lcd->invert_hline(client_rect.x0, client_rect.x1, tempPos);
	}
}
