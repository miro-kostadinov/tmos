/*
 * gedit.cpp
 *
 *  Created on: 17.09.2013
 *      Author: edguest
 */

#include <gedit.h>
#include <lcd.h>

void GEdit::draw_this(LCD_MODULE* lcd)
{
	if(flags & GO_FLG_BORDER)
		draw_border(rect);
	text_offset = lcd->pos_x;
	lcd->set_font(text_font);
	lcd->color = PIX_WHITE;
	lcd->pos_x = client_rect.x0 + lcd->font->hdistance;
	lcd->pos_y = client_rect.y0 + lcd->font->vdistance;
	lcd->allign = ALL_LEFT;
	draw_text(lcd, txt.c_str());
	if (flags & GO_FLG_SELECTED)
	{
		set_cursor_on_char(txt.c_str(), pos);
		if (key_input_time)														//inverts the area where the new character is put
		{
			if ((CURRENT_TIME - key_input_time) < 777)
				for (int i  = cursor.y0; i <= cursor.y1; i++)
					invert_hline (cursor.x0, cursor.x1, i);
		}
		else																	//flashing cursor
			if ((CURRENT_TIME - cursor_blink_time) < 500)
				invert_vline(cursor.y0, cursor.y1, cursor.x0);
	}
	else
		pos = txt.length();
}

unsigned int GEdit::process_idle (GMessage msg)
{
	if ((CURRENT_TIME - cursor_blink_time) > 1000)
		cursor_blink_time = CURRENT_TIME;
	if (key_input_time && ((CURRENT_TIME - key_input_time) > 777))
	{

		pos++;
		key_input_time = 0;
		last_key = 0;
		times_pressed = 0;
	}
	invalidate(this, cursor);
	return 1;
}

const char *key1_vals = " ,.?!";
const char *key2_vals = "àáâãabc2";
const char *key2_shift_vals = "ÀÁÂÃABC2";
const char *key3_vals = "äåæçdef3";
const char *key3_shift_vals = "ÄÅÆÇDEF3";
const char *key4_vals = "èéêëghi4";
const char *key4_shift_vals = "ÈÉÊËGHI4";
const char *key5_vals = "ìíîïjkl5";
const char *key5_shift_vals = "ÌÍÎÏJKL5";
const char *key6_vals = "ðñòmno6";
const char *key6_shift_vals = "ÐÑÒMNO6";
const char *key7_vals = "óôõöpqrs7";
const char *key7_shift_vals = "ÓÔÕÖPQRS7";
const char *key8_vals = "÷øùtuv8";
const char *key8_shift_vals = "×ØÙTUV8";
const char *key9_vals = "úüþÿwxyz9";
const char *key9_shift_vals = "ÚÜÞßWXYZ9";
const char *key0_vals = "0\n-_+=():;/*#@$&'\"";

void GEdit::process_alpha_key(char pressed_key, const char* key_val)
{
	if (last_key != pressed_key)												//checks if the key is pressed for the first time
	{
		if (key_input_time)
		{
			key_input_time = 0;													//if a new key is pressed before the timer went out
			invalidate (this, cursor);											//invalidates the current character
			pos++;																//and moves to the next one
		}
		key_input_time = CURRENT_TIME;
		times_pressed = 0;
		last_key = pressed_key;
		txt.insert(key_val[times_pressed], pos);								//inserts the new character from the key array into the string
		send_message (GMessage(WM_DRAW, 0, 0L, this));							//redraws the whole text
	}
	else
	{
		key_input_time = CURRENT_TIME;
		if (++times_pressed >= strlen(key_val))									//sets the array index in the beginning if it overflows
			times_pressed = 0;
		char prev_char = txt[pos];
		txt[pos] = key_val[times_pressed];
		if (strchr(" \n-", txt[pos]) || strchr(" \n-", prev_char))				//if this or the previous character is " ", "\n" or "-" redraws the whole text
			send_message (GMessage(WM_DRAW, 0, 0L, this));
		else																	//else only the current character
			invalidate (this, cursor);
	}

}
unsigned int GEdit::process_key (GMessage msg)
{
	switch (msg.param)
	{
	case KEY_LEFT:
		if (pos && !key_input_time)												//forbids moving if char typing has been started or the position is the first one
		{
			cursor_blink_time = 0;
			invalidate(this, cursor);											//invalidates the previous position of the cursor
			pos--;
			cursor_blink_time = CURRENT_TIME;
			set_cursor_on_char(txt.c_str(), pos);								//moves the cursor and the position in the string
			invalidate(this, cursor);											//invalidates the new position of the cursor
		}
		return 1;
	case KEY_RIGHT:
		if (pos < txt.length() && !key_input_time)								//forbids moving if char typing has been started or the position is the last one
		{
		cursor_blink_time = 0;
		invalidate(this, cursor);												//invalidates the previous position of the cursor
			pos++;
			cursor_blink_time = CURRENT_TIME;
			set_cursor_on_char(txt.c_str(), pos);								//moves the cursor and the position in the string
			invalidate(this, cursor);											//invalidates the new position of the cursor
		}
		return 1;
	case KEY_1:
		process_alpha_key(msg.param, key1_vals);								//puts a character in the string based
		return 1;																//on the key pressed
	case KEY_2:																	//the shift status
		process_alpha_key(msg.param, shift? key2_shift_vals : key2_vals);		//and the count of the key presses
		return 1;
	case KEY_3:
		process_alpha_key(msg.param, shift? key3_shift_vals : key3_vals);
		return 1;
	case KEY_4:
		process_alpha_key(msg.param, shift? key4_shift_vals : key4_vals);
		return 1;
	case KEY_5:
		process_alpha_key(msg.param, shift? key5_shift_vals : key5_vals);
		return 1;
	case KEY_6:
		process_alpha_key(msg.param, shift? key6_shift_vals : key6_vals);
		return 1;
	case KEY_7:
		process_alpha_key(msg.param, shift? key7_shift_vals : key7_vals);
		return 1;
	case KEY_8:
		process_alpha_key(msg.param, shift? key8_shift_vals : key8_vals);
		return 1;
	case KEY_9:
		process_alpha_key(msg.param, shift? key9_shift_vals : key9_vals);
		return 1;
	case KEY_0:
		process_alpha_key(msg.param, key0_vals);
		return 1;
	case KEY_SHIFT:																//changes the shift status
		if (shift)
			shift = false;
		else
			shift = true;
		return 1;
	case KEY_CANCEL:
		if (pos)
		{
			txt.erase(--pos, 1);												//erases the previous symbol
			send_message (GMessage(WM_DRAW, 0, 0L, this));						//redraws the whole text
		}
		return 1;
	}
	return 0;
}

void GEdit::set_cursor_on_char (const char* text, unsigned int index)
{
    unsigned int max_chars, len, tmp_len;
	unsigned int c;

	cursor.x0 = client_rect.x0 + text_font->hdistance;							//sets the start location of the cursor
	cursor.y0 = client_rect.y0 + text_font->vdistance;
	if(!text)
		return;
	max_chars = (client_rect.x1 - text_offset - (text_font->hdistance << 1)) / text_font->hspacing; //calculates the maximum number of characters that would fit on one text line
	len = 0;
    tmp_len = 0;
    while ((c = text[len++]) != '\0')
    {
    	if (len == max_chars)													//checks if the maximum number of characters has been reached
    	{
    		if (tmp_len)
    			len = tmp_len;
    		if ((signed int)(index - len) < 0)									//if the index is higher than this number, else breaks the cycle
    			break;
    		else
   				index -= len;													//subtracts it from the index
    		cursor.y0 += text_font->vspacing;									//moves the cursor to the next text line
    		text += len;														//increases the text pointer by the number of char written in the line
    		len = tmp_len = 0;													//resets the number of chars and the last " " or "-" position
    		continue;
    	}
    	if (c == ' ' || c == '-')												//if " " or "-" is encountered
    	{
    		tmp_len = len;														//updates the position of the last " " or "-" to it
    		continue;
    	}
    	if (c == '\n' || c == '\r')												//checks if new line is encountered
    	{
    		if ((signed int)(index - len) < 0)									//if the index is lesser than the number of character on the current line breaks the cycle
    			break;
    		else
    			index -= len;													//else subtracts this number from the index
    		cursor.y0 += text_font->vspacing;									//moves the cursor to the next text line
    		text += len;														//increases the text pointer by the number of char written in the line
    		len = tmp_len = 0;													//resets the number of chars and the last " " or "-" position
    	}
    }
    cursor.x0 = client_rect.x0 + text_font->hdistance + text_font->hspacing * index;	//moves the cursor to the position of the index char from the current line
	cursor.x1 = cursor.x0 + text_font->hspacing;								//sets the width of the cursor according to the spacing of the font
	cursor.y1 = cursor.y0 + text_font->height;									//sets the height of the cursor according to the font
	cursor.y0--;																//makes the cursor a little bit longer that the font height
}
