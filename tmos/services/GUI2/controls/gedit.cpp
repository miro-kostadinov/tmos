/*
 * gedit.cpp
 *
 *  Created on: 17.09.2013
 *      Author: edguest
 */

#include <stdgui.h>

GEdit::~GEdit()
{
	if(edit_menu)
	{
		delete edit_menu;
		edit_menu = nullptr;
	}
}

//const char *key1_vals = " ,.?!";
//const char *key2_vals = "àáâãabc2";
//const char *key2_shift_vals = "ÀÁÂÃABC2";
//const char *key3_vals = "äåæçdef3";
//const char *key3_shift_vals = "ÄÅÆÇDEF3";
//const char *key4_vals = "èéêëghi4";
//const char *key4_shift_vals = "ÈÉÊËGHI4";
//const char *key5_vals = "ìíîïjkl5";
//const char *key5_shift_vals = "ÌÍÎÏJKL5";
//const char *key6_vals = "ðñòmno6";
//const char *key6_shift_vals = "ÐÑÒMNO6";
//const char *key7_vals = "óôõöpqrs7";
//const char *key7_shift_vals = "ÓÔÕÖPQRS7";
//const char *key8_vals = "÷øùtuv8";
//const char *key8_shift_vals = "×ØÙTUV8";
//const char *key9_vals = "úüþÿwxyz9";
//const char *key9_shift_vals = "ÚÜÞßWXYZ9";
//const char *key0_vals = "0\n-_+=():;/*#@$&'\"";


#if USE_FIXED_CPAGE
#if USE_FIXED_CPAGE == 1250
extern const key_to_char_ref_t WEAK g_key_to_char_ref[] =
{
	{" ,?!:~^", " ,?!:~^"},						// 0
	{"àøšœº", "pqrs"},							// 1
	{"þùúûü", "tuv"},							// 2
	{"ýžŸ¿", "wxyz"},							// 3
	{"íî", "ghi"},								// 4
	{"³¾å", "jkl"},								// 5
	{"ñòóôõö", "mno"},							// 6
	{"-_+=();/%", "-_+=();/%"}, 				// 7
	{"áâãä¹æçè", "abc"},						// 8
	{"ïðéêëì", "def"},							// 9
	{"\n", "\n"},								// 10 key enter
	{".", "."},									// 11 key decimal point
	{"*#@$&'\"", "*#@$&'\""}					// 12 key 00
};
#endif // USE_FIXED_CPAGE == 1250
#if USE_FIXED_CPAGE == 1251
extern const key_to_char_ref_t WEAK g_key_to_char_ref[] =
{
	{" ,?!:~^", " ,?!:~^"},							// 0
	{"ôõö÷", "pqrs"},							// 1
	{"øùúü", "tuv"},							// 2
	{"þÿ", "wxyz"},								// 3
	{"èéêë", "ghi"},							// 4
	{"ìíîï", "jkl"},							// 5
	{"ðñòó", "mno"},							// 6
	{"-_+=();/%", "-_+=();/%"}, 					// 7
	{"àáâã", "abc"},							// 8
	{"äåæç", "def"},							// 9
	{"\n", "\n"},								// 10 key enter
	{".", "."},									// 11 key decimal point
	{"*#@$&'\"", "*#@$&'\""}					// 12 key 00
};
#endif // USE_FIXED_CPAGE == 1251
#endif // USE_FIXED_CPAGE

#define CHAR_TABLE_INDEX_ENTER		10
#define CHAR_TABLE_INDEX_DP			11
#define CHAR_TABLE_INDEX_00			12

#if USE_FIXED_CPAGE
#if USE_FIXED_CPAGE == 1250
extern const MENUTEMPLATE WEAK g_keyboard_menu[] =
{
	{ 0, KT_BG_CAPS, 	GMENU_FLG_SHOW_SELECTED_ITEM, "&1.ÁÂÃ"},
	{ 0, KT_BG, 		GMENU_FLG_SHOW_SELECTED_ITEM, "&2.áâã"},
	{ 0, KT_EN_CAPS, 	GMENU_FLG_SHOW_SELECTED_ITEM, "&3.ABC"},
	{ 0, KT_EN, 		GMENU_FLG_SHOW_SELECTED_ITEM, "&4.abc"},
	{ 0, KT_DIGIT, 		GMENU_FLG_SHOW_SELECTED_ITEM, "&5.123"},
	{ 0, 0, 0, nullptr}
};
#endif // USE_FIXED_CPAGE == 1250
#if USE_FIXED_CPAGE == 1251
extern const MENUTEMPLATE WEAK g_keyboard_menu[] =
{
	{ 0, KT_BG_CAPS, 	GMENU_FLG_SHOW_SELECTED_ITEM, "&1.ÀÁÂ"},
	{ 0, KT_BG, 		GMENU_FLG_SHOW_SELECTED_ITEM, "&2.àáâ"},
	{ 0, KT_EN_CAPS, 	GMENU_FLG_SHOW_SELECTED_ITEM, "&3.ABC"},
	{ 0, KT_EN, 		GMENU_FLG_SHOW_SELECTED_ITEM, "&4.abc"},
	{ 0, KT_DIGIT,	 	GMENU_FLG_SHOW_SELECTED_ITEM, "&5.123"},
	{ 0, 0, 0, nullptr}
};
#endif // USE_FIXED_CPAGE == 1251
#endif // USE_FIXED_CPAGE

unsigned int GEdit::initialize (GMessage& msg)
{
	if((flags & GO_FLG_SELECTED) && is_available() && parent)
		get_focus();

	alloc_scrollbars();
	text_size = SetTextAlign(align);
	unsigned int max_chars = client_rect.width() / text_font->hspacing;
	if(txt.length() > max_len)
		txt.erase(max_len, -1u);
	if(!(align & ES_AUTO_SCROLL))
	{
		if(!max_chars || txt.length() > max_chars -1)
		{
			if(max_chars)
				txt.erase(max_chars -1, -1u);
			else
				txt.clear();
			text_size = SetTextAlign(align);
		}
	}
	if(align & ES_NUMERIC)
		shift = KT_DIGIT;
	pos = txt.length();
	cursor = scroll_rect;
	cursor.y0 --;
	if(pos >= max_chars)
	{
		max_chars = pos - max_chars +1;
		pos -= max_chars;
		set_cursor_on_char();
		pos += max_chars;
	}
	set_cursor_on_char();
	if(msg.param)
		send_message(WM_DRAW, 0, 0L, this);
	return 1;
}

void GEdit::draw_this(LCD_MODULE* lcd)
{
//	if(flags & GO_FLG_BORDER)
//		draw_border(rect);

	lcd->set_font(text_font);
	lcd->set_color(PIX_WHITE);
	lcd->allign = (align & (TA_HORIZONTAL|TA_VERTICAL));
	draw_caption(lcd);

	GClientLcd dc(this);
	if(dc.CreateLcd(scroll_rect, lcd))
	{
		CSTRING password(txt);

		lcd->pos_x = dc.client_rect.x0;
		lcd->pos_y = dc.client_rect.y0;

		if(align & ES_PASSWORD)
		{
			if(!password.empty())
			{
				char ch = password[pos];

				password.reserve(password.length());
				for(unsigned int i=password.length(); i-- ; )
					password[i] = '*';
				if(IsActiveTimer(EDIT_TIMER_INPUT))
					password[pos] = ch;
			}
		}
		if(align & ES_MULTILINE)
			dc.draw_text(lcd, password.c_str());
		else
		{
			if(text_size.width > client_rect.width())
				lcd->allign = (align & ~TA_HORIZONTAL)|TA_LEFT;
			dc.draw_text_line(lcd, password.c_str(), txt.length());
		}
		dc.RelaseLcd();
	}

	if(vscroll)
	{
		client_rect.Inflate(0, 1);
		vscroll->draw_scroll(lcd);
		client_rect.Deflate(0, 1);
	}
	if(hscroll)
		hscroll->draw_scroll(lcd);

	if ( flags & GO_FLG_SELECTED )
	{
		if(!(align & ES_HIDE_CURSOR))
		{
			if(IsActiveTimer(EDIT_TIMER_INPUT))
				draw_rectangle(cursor, true);
			else
			{
				if(cursor_on)
					draw_vline(cursor.y0, cursor.y1, cursor.x0);
			}
		}
	}
	if(flags & GO_FLG_BORDER)
		draw_border(rect);
}

void GEdit::move(int x, int y)
{
	GText::move(x, y);
	cursor.Offset(x, y);
}

void GEdit::text_change()
{
	RECT_T r(client_rect.as_int);

	text_size = SetTextAlign(align);		//redraws the whole text
	if(r.x0 > client_rect.x0)
		r.x0 = client_rect.x0;
	if(r.y0 > client_rect.y0)
		r.y0 = client_rect.y0;
	if(r.x1 < client_rect.x1)
		r.x1 = client_rect.x1;
	if(r.y1 < client_rect.y1)
		r.y1 = client_rect.y1;
	set_cursor_on_char();
	send_message(WM_DRAW, 0, r.as_int, this);
}

void GEdit::pos_change(int val, bool modified_text)
{
	StopTimer(EDIT_TIMER_INPUT);
	cursor_on = false;
	show_cursor();
	if(val > 0)
	{
		pos += val;
		if(!(align & ES_AUTO_SCROLL))
		{
			if((int)pos > client_rect.width() / text_font->hspacing)
				pos -= val;
		}
		if(pos > max_len)
			pos = max_len;
	}
	if(val < 0 && pos)
	{
		val = -val;
		if((unsigned int)val > pos)
			pos = 0;
		else
			pos -= val;
	}

	if(!(align & ES_HIDE_CURSOR))
	{
		SetTimer(EDIT_TIMER_BLINK, EDIT_BLINK_TIME);
		cursor_on = true;
	}

	if(!set_cursor_on_char())								//moves the cursor and the position in the string
		show_cursor();
	else
		invalidate(this, client_rect);
	if(modified_text && parent)
		send_message(WM_CHANGE, (unsigned int) this, 0L, parent);
}

unsigned int GEdit::process_default (GMessage& msg)
{
	switch(msg.code)
	{
	case WM_TIMER:
		switch(msg.param)
		{
		case EDIT_TIMER_INPUT:
			last_key = 0;
			times_pressed = 0;
			pos_change(+1);
			break;
		case EDIT_TIMER_BLINK:
			if(!(align & ES_HIDE_CURSOR))
			{
				SetTimer(EDIT_TIMER_BLINK, EDIT_BLINK_TIME);
				cursor_on = (cursor_on)?false:true;
			}
			else
			{
				KillTimer(EDIT_TIMER_BLINK);
				cursor_on = false;
			}
			show_cursor();
			break;
		}
		break;

	case WM_SETFOCUS:
		if(!(align & ES_HIDE_CURSOR))
		{
			cursor_on = true;
			SetTimer(EDIT_TIMER_BLINK, EDIT_BLINK_TIME);
		}
		else
			cursor_on = false;
		break;
	case WM_KILLFOCUS:
		cursor_on = false;
		KillTimer(EDIT_TIMER_BLINK);
		break;
	default:
		break;
	}
	return GObject::process_default(msg);
}

unsigned int GEdit::process_sysctrl(GMessage& msg)
{
	unsigned int change ;

	switch(msg.code)
	{
	case WM_SYSCTRL_SET:
		change = align & msg.param;
		align |= msg.param;
		msg.param &= ~change;
		if(msg.param) // flags set
		{
			//TODO:
		}
		break;

	case WM_SYSCTRL_CLR:
		change = align;
		align &= ~msg.param;
		if(align & change) // flags clear
		{
			//TODO:
		}
		break;

	default: //	WM_SYSCTRL
		break;
	}
	return 0;
}

void GEdit::process_alpha_key(char pressed_key, const char* key_val)
{
	if (last_key != pressed_key)												//checks if the key is pressed for the first time
	{
		if(IsActiveTimer(EDIT_TIMER_INPUT))
		{                                                                       //if a new key is pressed before the timer went out
			pos_change(+1);														//invalidates the current character
		}                                                                       //and moves to the next one
		if(!(align & ES_AUTO_SCROLL))
			if((int)txt.length() >= client_rect.width() / text_font->hspacing)
				return;
		if(txt.length() >= max_len)
			return;

		if( shift != KT_DIGIT || (align & ES_PASSWORD))
		{
			SetTimer(EDIT_TIMER_INPUT, EDIT_INPUT_TIME);
			times_pressed = 0;
			last_key = pressed_key;
			if(shift == KT_BG_CAPS || shift == KT_EN_CAPS)
			{
#if USE_CSTRING
				txt.insert(toupper(key_val[times_pressed]), pos);								//inserts the new character from the key array into the string
#else
				txt.insert(pos, 1, toupper(key_val[times_pressed]));			//inserts the new character from the key array into the string
#endif				
			}
			else
			{
#if USE_CSTRING
				txt.insert(key_val[times_pressed], pos);								//inserts the new character from the key array into the string
#else
				txt.insert(pos, 1, key_val[times_pressed]);						//inserts the new character from the key array into the string
#endif				
			}	
			text_change();															//redraws the whole text
		}
		else
		{
			StopTimer(EDIT_TIMER_INPUT);
			last_key = 0;
			times_pressed = 0;
#if USE_CSTRING
			txt.insert(key_val[0], pos);								//inserts the new character from the key array into the string
#else
			txt.insert(pos, 1, key_val[0]);										//inserts the new character from the key array into the string
#endif				
			if((align & ES_AUTO_SCROLL))
			{
				if((int)txt.length() >= client_rect.width() / text_font->hspacing)
					text_change();
			}
			pos_change(+1);
		}
	}
	else
	{
		SetTimer(EDIT_TIMER_INPUT, EDIT_INPUT_TIME);
		if (shift != KT_DIGIT && ++times_pressed >= strlen(key_val))									//sets the array index in the beginning if it overflows
			times_pressed = 0;
		char prev_char = txt[pos];
		if(shift == KT_BG_CAPS || shift == KT_EN_CAPS)
			txt[pos] = toupper(key_val[times_pressed]);
		else
			txt[pos] = key_val[times_pressed];
		if (strchr(" \n-", txt[pos]) || strchr(" \n-", prev_char))				//if this or the previous character is " ", "\n" or "-" redraws the whole text
			invalidate(this, client_rect);
		else																	//else only the current character
			invalidate(this, cursor);
	}

}

void GEdit::show_cursor()
{
	invalidate (this, cursor);
}

unsigned int GEdit::process_key (GMessage& msg)
{
	if(msg.param & KEY_UP_CODE)
		return 0;

	if(edit_menu)
	{
		switch(msg.param)
		{
		case KEY_1:
		case KEY_2:
		case KEY_3:
		case KEY_4:
		case KEY_5:
		case KEY_6:
		case KEY_7:
		case KEY_8:
		case KEY_9:
		{
			menu_template_t* tmp;
			char  ch = TranslateKey(msg.param);
			char* hot_pos;
			if(ch && !(msg.param & KEY_ASCII_CODE))
			{
				tmp = edit_menu->menu;
				while(tmp)
				{
					hot_pos = strchr(tmp->item_name.c_str(), '&');
					if(hot_pos && hot_pos[1] == ch)
					{
						shift = (key_mode)tmp->item;
						break;
					}
					tmp = edit_menu->GetMenu(edit_menu->menu->parent, tmp +1);
				}
				if(!tmp)
					return 0;
			}
			else
				return 0;
		}
		break;

		case KEY_ENTER:
			if(edit_menu->item)
				shift = (key_mode)edit_menu->item->item;
			break;

		case KEY_ESC:
		case KEY_SHIFT:
		case KEY_CANCEL:
			break;

		default:
			if(edit_menu->process_key(msg))
				return 1;
			return 0;
		}
		edit_menu->remove(); //close();
		send_message(WM_DRAW, 0, edit_menu->rect.as_int, parent);
		delete edit_menu;
		edit_menu = nullptr;
		return 1;
	}

	switch (msg.param)
	{
	case KEY_LEFT|KEY_REPEAT_CODE:
	case KEY_LEFT:
		if (pos && !IsActiveTimer(EDIT_TIMER_INPUT))							//forbids moving if char typing has been started or the position is the first one
		{
			pos_change(-1, false);
		}
		return 1;
	case KEY_RIGHT|KEY_REPEAT_CODE:
	case KEY_RIGHT:
		if (pos < txt.length() && !IsActiveTimer(EDIT_TIMER_INPUT))				//forbids moving if char typing has been started or the position is the last one
		{
			pos_change(+1, false);
		}
		return 1;
	case KEY_SHIFT:
		//changes the shift status
		if(!edit_menu && !(align & ES_NUMERIC))
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
//				else
//				{
//					edit_menu->rect.Deflate(0, max - rect.height());
//				}
				parent->addChild(edit_menu);
				edit_menu->initialize(msg);
				if(!(edit_menu->item = edit_menu->FindItem(shift)))
					edit_menu->item = edit_menu->menu;
				send_message(WM_DRAW, 0, edit_menu->rect.as_int, parent);
			}
		}
		return 1;
	case KEY_CANCEL:
		if (pos && !IsActiveTimer(EDIT_TIMER_INPUT))
		{
			pos_change(-1);
		}
		StopTimer(EDIT_TIMER_INPUT);
		last_key = 0;
		times_pressed = 0;
		txt.erase(pos, 1);												//erases the previous symbol
		text_change();
		return 1;

	case KEY_ENTER:
		if(align & ES_USE_VIRTUAL_KEYBOARD)
			return GEditVKB::Create(this);
	case KEY_ENTER | KEY_ASCII_CODE:
	case KEY_USER_DEFINED: // bar code
		if(align & ES_WANTRETURN)
			 process_alpha_key(msg.param, g_key_to_char_ref[CHAR_TABLE_INDEX_ENTER].bg_vals);
		else
			send_message (WM_COMMAND, id, 0L, parent);
		return 1;

	case KEY_POINT:
		process_alpha_key(msg.param, g_key_to_char_ref[CHAR_TABLE_INDEX_DP].bg_vals);
		return 1;

	case KEY_X:
		if(shift != KT_DIGIT)
			process_alpha_key(msg.param, g_key_to_char_ref[CHAR_TABLE_INDEX_00].bg_vals);
		else
			process_alpha_key(msg.param, "-");
		return 1;

	default:
		{
			 unsigned char  ch = TranslateKey(msg.param) -'0';
			 if(ch <= 9 && !(msg.param & KEY_ASCII_CODE))
			 {
				 switch(shift)
				 {
				 case KT_BG_CAPS:
				 case KT_BG:
					 process_alpha_key(msg.param, g_key_to_char_ref[ch].bg_vals);
					 return 1;
				 case KT_EN_CAPS:
				 case KT_EN:
					 process_alpha_key(msg.param, g_key_to_char_ref[ch].en_vals);
					 return 1;
				 case KT_DIGIT:
					 ch += '0';
					 last_key =0;
					 process_alpha_key(msg.param, (const char *)&ch);
					 return 1;
				 }
			 }
			 else
			 {
				 return process_char(msg.param);
			 }

		}
		break;
	}
	return 0;
}

bool GEdit::process_char(unsigned int ch)
{
	if(txt.length() < max_len && ch > KEY_ASCII_CODE)
	{
		ch &= 0xFF;
		if(IS_ALPHANUM(ch) || IS_PUNC(ch) || ch == ' ')
		{
			StopTimer(EDIT_TIMER_INPUT);
			last_key = 0;
			times_pressed = 0;
#if USE_CSTRING
			txt.insert(ch, pos);								//inserts the new character from the key array into the string
#else
			txt.insert(pos, 1, ch);								//inserts the new character from the key array into the string
#endif				
			if((align & ES_AUTO_SCROLL))
			{
				if((int)txt.length() >= client_rect.width() / text_font->hspacing)
					text_change();
			}
			pos_change(+1);
			return true;
		}
	}
	return false;
}

bool GEdit::set_cursor_on_char(void)
{
	if(!txt.empty())
	{
		if(align & ES_MULTILINE)
			return set_cursor_y_char();
		return set_cursor_x_char();
	}
	scroll_rect.y0 = client_rect.y0; cursor.y0 = client_rect.y0 -1;
	cursor.y1 = cursor.y0 + text_font->vspacing;
	scroll_rect.x0 = client_rect.x0; cursor.x0 = client_rect.x0;
	return false;
}

bool GEdit::set_cursor_y_char(void)
{
	const char* text = txt.c_str();
	unsigned int index = pos;

	unsigned int max_chars, len, tmp_len;
	unsigned int c;

	bool res = false;
    int y=0;

	int offset=client_rect.y0 - rect.y0;
	cursor.y0 -= offset;
	scroll_rect.y0  -= offset;

	max_chars = (client_rect.width() - (text_font->hdistance << 1)) / text_font->hspacing; //calculates the maximum number of characters that would fit on one text line
	len = 0;
	tmp_len = 0;

	while ((c = text[len++]) != '\0')
	{
		if (c == ' ' || c == '-')												//if " " or "-" is encountered
			tmp_len = len;														//updates the position of the last " " or "-" to it

		if (len == max_chars)													//checks if the maximum number of characters has been reached
		{
			if (tmp_len && text[len] && !strchr(" -\n\r", text[len]))
				len = tmp_len;
			if ((signed int) (index - len) < 0 || text[len] == 0) //if the index is higher than this number, else breaks the cycle
				break;
			else
				index -= len;													//subtracts it from the index
			y += text_font->vspacing;									//moves the cursor to the next text line
			text += len;														//increases the text pointer by the number of char written in the line
			len = tmp_len = 0;													//resets the number of chars and the last " " or "-" position
			continue;
		}
		if (c == '\n' || c == '\r')												//checks if new line is encountered
		{
			if ((signed int)(index - len) < 0)									//if the index is lesser than the number of character on the current line breaks the cycle
				break;
			else
				index -= len;													//else subtracts this number from the index
			y += text_font->vspacing;									//moves the cursor to the next text line
			text += len;														//increases the text pointer by the number of char written in the line
			len = tmp_len = 0;													//resets the number of chars and the last " " or "-" position
		}
	}

	offset = -scroll_rect.y0;
	cursor.y0++;

	if(y != offset + cursor.y0)
	{
		res = true;
		if(y > offset + cursor.y0)
		{
			// move down
			if( y - offset  <= rect.y0 + client_rect.height() - text_font->vspacing)
				cursor.y0 = y - offset;
			else
			{
				scroll_rect.y0 = cursor.y0 -y;
			}
		}
		else
		{
			// move up
			if( y >= offset && (y-offset) >= rect.y0)
			{
				cursor.y0 = y - offset;
			}
			else
			{
				if( !y )
					scroll_rect.y0 = cursor.y0 = rect.y0;
				else
					scroll_rect.y0 += offset + cursor.y0 -y;
			}
		}
	}

	offset  = client_rect.y0 - rect.y0;
	scroll_rect.x0 = client_rect.x0;
	scroll_rect.x1 = client_rect.x1;
	scroll_rect.y0  += offset;
	scroll_rect.y1  = scroll_rect.y0 + text_size.height;

	cursor.y0 += offset;
	cursor.y1 = cursor.y0 + text_font->height;			//sets the height of the cursor according to the font

	switch (align & TA_HORIZONTAL)
	{
	case TA_LEFT:
		cursor.x0 = client_rect.x0 +
			text_font->hspacing * index;
		break;
	case TA_RIGHT:
		cursor.x0 = (client_rect.x1  - ((len-1) * text_font->hspacing))
			+ text_font->hspacing * index;
		break;
	case TA_CENTER:
		if(len && (strchr(" \r\n",text[len-1]) || text[len-1] == '\0') )
			len--;
		cursor.x0 = (((client_rect.x1 + client_rect.x0) - (len * text_font->hspacing)) >> 1)
			+ text_font->hspacing * index;
		break;
	}


	if(vscroll && res)
	{
		client_rect.Inflate(0, 1);
		vscroll->SetScrollPos(GO_FLG_VSCROLL, (cursor.y0 - scroll_rect.y0)/text_font->vspacing/*y/text_font->vspacing*/ , true);
		client_rect.Deflate(0, 1);
	}

	cursor.x1 = cursor.x0 + text_font->hspacing;								//sets the width of the cursor according to the spacing of the font
	cursor.y0--;																//makes the cursor a little bit longer that the font height
	return res;
}


bool GEdit::set_cursor_x_char(void)
{
	const char* text = txt.c_str();
	unsigned int index = pos;

	bool res = false;
    int  x=0;

	scroll_rect.y0 = client_rect.y0;
	scroll_rect.y1 = client_rect.y1;

	if(text_size.width <= client_rect.width())
	{
		x = text_font->hspacing * index;
		switch (align & TA_HORIZONTAL)
		{
		case TA_LEFT:
			cursor.x0 = client_rect.x0 + x;
			break;
		case TA_RIGHT:
			cursor.x0 = (client_rect.x1  - text_size.width) + x;
			break;
		case TA_CENTER:
			index = strlen(text);
			if(index && (strchr(" \r\n",text[index-1]) || text[index-1] == '\0') )
				x -= text_font->hspacing;
			cursor.x0 = (((client_rect.x1 + client_rect.x0) - text_size.width) >> 1)
				+ x;
			break;
		}
		if(hscroll)
			hscroll->SetScrollRange(GO_FLG_HSCROLL, 0);
	}
	else
	{
		int offset;

		unsigned int max_chars = client_rect.width() / text_font->hspacing;

		offset=client_rect.x0 - rect.x0;
		scroll_rect.x0  -= offset;
		cursor.x0 -= offset;
		while(max_chars && index > max_chars)
		{
			x += max_chars;
			index -= max_chars;
		}
		x = (index + x)* text_font->hspacing;

		offset = -scroll_rect.x0;

		if(x != offset + cursor.x0)
		{
			res = true;
			if(x > offset + cursor.x0)
			{
				// move down
				if( x - offset  <= rect.x0 + client_rect.width() - text_font->hspacing)
					cursor.x0 = x - offset;
				else
				{
					scroll_rect.x0 = cursor.x0 -x;
				}
			}
			else
			{
				// move up
				if( x >= offset && (x-offset) >= rect.x0)
				{
					cursor.x0 = x - offset;
				}
				else
				{
					if( !x )
						scroll_rect.x0 = cursor.x0 = rect.x0;
					else
						scroll_rect.x0 += offset + cursor.x0 -x;
				}
			}
		}
		if(hscroll)
			hscroll->SetScrollPos(GO_FLG_HSCROLL, x/text_font->hspacing, true);

		offset  = client_rect.x0 - rect.x0;
		scroll_rect.x0  += offset;
		cursor.x0 += offset;

		scroll_rect.x1  = scroll_rect.x0 + text_size.width;
	}
	cursor.x1 = cursor.x0 + text_font->hspacing;								//sets the width of the cursor according to the spacing of the font
	cursor.y1 = cursor.y0 + text_font->vspacing;			//sets the height of the cursor according to the font

	return res;
}
