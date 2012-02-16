/*
 * display.c
 *
 *  Created on: 2008-10-19
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <stdgui.h>
#include <lcd.h>


unsigned int current_laguage;

//*----------------------------------------------------------------------------

bool CWindow::tsk_window_init( GUI_CB cback)
{
	return (tsk_open(GUI_DRV_INDX, (const void*) cback));
}

WINDOW tsk_new_window(GUI_CB callback)
{
	WINDOW win;

	win = new CWindow();
	if(win)
	{

		if(!win->tsk_open(GUI_DRV_INDX, (const void *)callback))
		{
			delete win;
			win = NULL;
		}
	}
	return (win);
}

//*----------------------------------------------------------------------------
//*			Menu box
//*----------------------------------------------------------------------------


#if MENU_INDEX == ENABLE_INDEX

static unsigned int menu_index=0;

static void set_index( unsigned int pos )
{
	menu_index -= menu_index%10;
	if(pos < 9)
		menu_index += pos +1;
}

RES_CODE menu_draw(MENU_WINDOW menu_hnd, LCD_MODULE* lcd)
{
    unsigned int first, last;
//    char buf[12];
    set_index(menu_hnd->pos);
//    tmos_sprintf(buf,"code:%d", menu_index );
//    lcd->set_xy_all(77, ALL_LEFT);
//    lcd->draw_text(buf);

    lcd->color = PIX_BLUE;
    lcd->set_xy_all(2, ALL_CENTER);
    lcd->draw_text(menu_hnd->menu->name[current_laguage].c_str());
    lcd->draw_hline(0, lcd->size_x-1, 12);
    lcd->color = PIX_WHITE;

    unsigned int menu_lines = (lcd->size_y - 22)/ lcd->font->vspacing;
    if(!(menu_lines&1))
    	menu_lines--;

    if(menu_hnd->pos > (menu_lines/2))
    {
        first = menu_hnd->pos-(menu_lines/2);
        while(first && ((menu_hnd->menu->count - first)<menu_lines))
            first --;
    }
    else
        first = 0;

    last = first +menu_lines;
    if (last > menu_hnd->menu->count)
        last = menu_hnd->menu->count;

    CSTRING line;
    lcd->set_xy_all(16, ALL_LEFT);
    for(unsigned int i=first; i<last; i++)
    {
    	unsigned int offset = 0;
		line = menu_hnd->menu->items[i].name[current_laguage];
    	if(menu_hnd->pos == i )
    	{
    		if(line.length() >= lcd->chars_per_row)
    		{
    			if(lcd->frame_y0 == 0)
					if(line.length()+ 2 < ++menu_hnd->text_offset)
						menu_hnd->text_offset = 0;
    			offset = menu_hnd->text_offset;
    			CSTRING tmp("  ");
    			tmp += line;
    			line += tmp;
    			line.erase(0, offset);
//    	    	if(line.length() - (lcd->chars_per_row - 2) < offset)
//    	    	{
//    	    		CSTRING str(line);
//    	    		line = str.c_str() + menu_hnd->text_offset;
//    	    		line += "  ";
//    	    		line += str;
//    	    		offset = 0;
//    	    	}
    		}
    	}
		if(line.length() >= lcd->chars_per_row)
		{
			line[lcd->chars_per_row]=0;
			line.storage.ram->len = lcd->chars_per_row;
		}
    	lcd->draw_text(line.c_str());
//    	lcd->draw_text(line.c_str()+ offset);
    }
    first = 14 + (menu_hnd->pos - first)*11;
    last = first+11;
    for(unsigned int i=first; i<last; i++)
    {
    	lcd->invert_hline(0, lcd->size_x-1, i);
    }
	return (0);
}
#else
RES_CODE menu_draw(MENU_HANDLE menu_hnd)
{
    unsigned int first, last;
    lcd->set_xy_all(10, ALL_CENTER);
    lcd->draw_text(menu_hnd->menu->name));
    lcd->draw_hline(0, 101, 20);

    if(menu_hnd->pos > 2)
    {
        first = menu_hnd->pos-2;
        while(first && ((menu_hnd->menu->count - first)<5))
            first --;
    }
    else
        first = 0;

    last = first +5;
    if (last > menu_hnd->menu->count)
        last = menu_hnd->menu->count;

    lcd->set_xy_all(24, ALL_LEFT);
    for(unsigned int i=first; i<last; i++)
    {
    	lcd->draw_text(menu_hnd->menu->items[i].name);
    }
    first = 23 + (menu_hnd->pos - first)*11;
    last = first+11;
    for(unsigned int i=first; i<last; i++)
    {
		lcd->invert_hline(0, lcd->size_x-1, i);
    }
	return (0);
}
#endif

extern const unsigned char key_codes[];

RES_CODE menu_cb(MENU_WINDOW menu_hnd, unsigned int param, unsigned int msg)
{
    if(msg == WM_DRAW)
        return (menu_draw(menu_hnd, (LCD_MODULE*) param));

    if(msg == WM_KEY)
    {
        switch (param & KEY_PRESREP_MASK)
        {
#ifdef KEY_UP
	        case KEY_UP:
	        	menu_hnd->text_offset = 0;
				if(menu_hnd->pos)
					menu_hnd->pos--;
	    	    return (FLG_BUSY);
#endif

#ifdef KEY_DOWN
	        case KEY_DOWN:
	        	menu_hnd->text_offset = 0;
				if(menu_hnd->pos < ( menu_hnd->menu->count-1))
					menu_hnd->pos++;
		        return (FLG_BUSY);
#endif

#ifdef KEY_OK
	        case KEY_OK:
                return RES_SIG_OK;
#endif

#ifdef KEY_C
        	case KEY_C:
                return RES_SIG_IDLE;
#endif

#if MENU_INDEX == ENABLE_INDEX
        	default:
                param = key_codes[param];
                if( (param >= '1') && (param <= '9'))
                {
                	param -='1';
                	if( menu_hnd->menu->count > param )
                	{
                		set_index(param);
                		menu_hnd->pos= param;
                		return RES_SIG_OK;
                	}
                }
                break;
#endif
         }
    }

    return (0);
}


// this function can be called recursively
// MENU_CBF functions will:
//	return 0 to keep the current menu open
//	anything else will close everything

RES_CODE menu_box(const MENU_STRU *menu, MENU_WINDOW menu_hnd)
{
	unsigned int pos=0;
    RES_CODE res =RES_OK;

#if MENU_INDEX == ENABLE_INDEX
    menu_index *=10;
#endif
    while(menu)
	{

	    menu_hnd->pos = pos;
		menu_hnd->menu = menu;
		menu_hnd->text_offset = 0;
		menu_hnd->tsk_window_showmodal();
		if(menu_hnd->res != RES_OK)
		{
			res=0;
			break;
		}
		//menu selected
	    pos = menu_hnd->pos;
        {
            MENU_ITEM item;

            item = (MENU_ITEM)&menu_hnd->menu->items[pos];
            if(item->menu_func)
            {
				res = item->menu_func(item->param, menu_hnd);
            } else
            {
            	//menu with no callback ...
                res = RES_ERROR;
            }
        }
		if(res)
			break;
    }
#if MENU_INDEX == ENABLE_INDEX
	menu_index/=10;
#endif
    return (res);
}


//*----------------------------------------------------------------------------
//*			msg_box
//*----------------------------------------------------------------------------
static void draw_box_frame(CMessageBox* msg_hnd, LCD_MODULE* lcd)
{
	unsigned int posy;
	const char *txt;

	lcd->draw_hline(0, lcd->size_x-1, 0);
	posy = msg_hnd->rect.y0 + 10;
	lcd->clear_rect(1, 1, lcd->size_x-1, posy-1);
	txt = msg_hnd->msg_text.c_str();
	if(txt)
	{
		lcd->set_xy_all(posy, ALL_CENTER);
		while(txt[0] && ((posy+lcd->font->hight) < msg_hnd->rect.y1))
		{
			lcd->clear_rect(1, posy, lcd->size_x-1, posy+10);
			txt = lcd->draw_text(txt);
			posy += 11;
		}
	}
	lcd->draw_hline(0, lcd->size_x-1, posy);
	lcd->draw_vline(0, posy, 0);
	lcd->draw_vline(0, posy, lcd->size_x-1);
}

RES_CODE msgbox_cb(CMessageBox* msg_hnd, unsigned int param, unsigned int msg)
{
    if(msg == WM_DRAW)
    {
        draw_box_frame(msg_hnd, (LCD_MODULE*)param);
        return (0);
    }

    if(msg == WM_KEY)
    {
        switch (param /*& KEY_PRESREP_MASK*/)
        {

#ifdef KEY_OK
	        case KEY_OK:
                return RES_SIG_OK;
#endif

#ifdef KEY_C
        	case KEY_C:
                return RES_SIG_CANCEL;
#endif
        }
    }

    return (0);
}

RES_CODE msg_box(const char *msg)
{
	CMessageBox msg_hnd;

	if(msg_hnd.tsk_window_init((GUI_CB)msgbox_cb))
	{
		msg_hnd.msg_text = msg;
		msg_hnd.tsk_window_showmodal();
		return (msg_hnd.res);
	}
    return RES_ERROR;
}

RES_CODE msg_box(CSTRING& msg)
{
	CMessageBox msg_hnd;

	if(msg_hnd.tsk_window_init((GUI_CB)msgbox_cb))
	{
		msg_hnd.msg_text = msg;
		msg_hnd.tsk_window_showmodal();
		return (msg_hnd.res);
	}
    return RES_ERROR;
}

WEAK RES_CODE msg_error(CSTRING& msg, int err_code)
{
	return (msg_box(msg));
}

WEAK RES_CODE msg_error(const char *msg, int err_code)
{
	return (msg_box(msg));
}

//*----------------------------------------------------------------------------
//*			get_box
//*----------------------------------------------------------------------------

#define TXT_CHAR_LINES 		16
#define TXT_SCREEN_LINES 	4

const char buttons[160] =
{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', //1
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', //2
		'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', //3
		'U', 'W', 'V', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', //4
		'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', //5
		'o', 'p', 'q', 'r', 's', 't', 'u', 'w', 'v', 'x', //6
		'y', 'z', 'À', 'Á', 'Â', 'Ã', 'Ä', 'Å', 'Æ', 'Ç', //7
		'È', 'É', 'Ê', 'Ë', 'Ì', 'Í', 'Î', 'Ï', 'Ð', 'Ñ', //8
		'Ò', 'Ó', 'Ô', 'Õ', 'Ö', '×', 'Ø', 'Ù', 'Ü', 'Ú', //9
		'Þ', 'ß', 'à', 'á', 'â', 'ã', 'ä', 'å', 'æ', 'ç', //10
		'è', 'é', 'ê', 'ë', 'ì', 'í', 'î', 'ï', 'ð', 'ñ', //11
		'ò', 'ó', 'ô', 'õ', 'ö', '÷', 'ø', 'ù', 'ü', 'ú', //12
		'þ', 'ÿ', '!', '@', '#', '%', '^', '&', '*', '(', //13
		')', '_', '-', '+', '=', '/', '|', '{', '}', '[', //14
		']', '<', '>', ',', '.', '?', '\\', '\"', '~', '`',  //15
		' ', ':', '/', ';', ' ', ' ', ' ', ' ', ' ', ' '	// 16
};

void DrawButtonsRow(unsigned int n, const char* btns, signed char sx, LCD_MODULE* lcd)
{
	unsigned char y = 34 + n * 11;
	unsigned char x;

	for(x=2; x < 100; x += 10)
	{
		lcd->draw_bitmap(x, y, lcd->font->font_data + ((*btns++) - 0x20)
				* lcd->font->char_bytes, lcd->font->width, lcd->font->hight);
	}

	if (sx >= 0)
	{
		x = (sx * 10) + 1;
		y = n * 11 + 32;
		for (n = 0; n < 12; n++)
			lcd->invert_hline(x, x + 8, y + n);
	}
}

void DrawButtons(CGetBox* box, LCD_MODULE* lcd)
{
	unsigned int i;
	unsigned int start;

	i=box->sy;
	start = box->y;
	while(i)
	{
		start = (start + TXT_CHAR_LINES - 1)%TXT_CHAR_LINES;
		i--;
		DrawButtonsRow(i, &buttons[start * 10], -1, lcd);
	}
	i=box->sy;
	start = box->y;
	DrawButtonsRow(i, &buttons[start * 10], box->x, lcd);
	while( i < TXT_SCREEN_LINES-1)
	{
		start = (start + 1)%TXT_CHAR_LINES;
		i++;
		DrawButtonsRow(i, &buttons[start * 10], -1, lcd);

	}
}

extern TASK_STRU gui_task;
RES_CODE getbox_cb(CGetBox* box, unsigned int param, unsigned int msg)
{
	const char * buf;
	unsigned char pos;
    if(msg == WM_DRAW)
    {
    	LCD_MODULE* lcd = (LCD_MODULE*)param;

    	lcd->clear_screen();
    	box->flags ^= TXT_FLAGS_CURSOR;
        lcd->set_xy_all(2, ALL_CENTER);
        lcd->draw_text(box->title.c_str());
    	if(box->flags & TXT_FLAGS_CONST)
    	{
    		if(box->data.length() > 10)
    		{
    			if(!lcd->frame_y0)
    			{
    				box->pos ++;
    			//	tsk_sleep(250);
    				usr_send_signal(&gui_task, 0x100);
    			}
    		}
    		if(box->pos > box->data.length() +10 )
    		{
    			box->pos = box->data.length();
    			if(box->pos > 10)
    				box->pos = 0;
    		}
    	}
    	if(box->flags & TXT_FLAGS_EDIT)
    	{
			lcd->draw_hline(0, lcd->size_x-1, 1+12);
			if(!(box->flags & TXT_FLAGS_CONST))
			{
				lcd->draw_vline(1+12, 1+12+15, 0);
				lcd->draw_vline(1+12, 1+12+15, lcd->size_x-1);
				lcd->draw_hline(0, lcd->size_x-1, 1+12+15);
	    		lcd->set_xy_all(lcd->size_y -23, ALL_RIGHT);
#ifdef KEY_C
	    		if(box->flags & TXT_FLAGS_RES)
	    			lcd->draw_text("CANCEL");
	    		else
	    			lcd->draw_text("DEL");
#endif
			}
#ifdef KEY_OK
    		lcd->set_xy_all(lcd->size_y -23, ALL_LEFT);
    		lcd->draw_text("OK");
#endif
    	}
    	else
    	{
    		DrawButtons(box, lcd);
    	}

		buf = box->data.c_str();
		pos = box->pos;
    	if(!buf)
			buf = "";
        lcd->set_xy_all(17, ALL_LEFT);
		if( pos > 10 )
		{
	        buf += pos -10;
	        pos = 10;
		}
		lcd->draw_text(buf);
		if(box->flags & (TXT_FLAGS_CURSOR|TXT_FLAGS_CONST))
		{
			pos = 2 + pos * lcd->font->spacing;
		   lcd->draw_hline(pos, pos + lcd->font->spacing, 1+12+15-2);
		   lcd->draw_hline(pos, pos + lcd->font->spacing, 1+12+15-1);
		}
        return (0);
    }

    if(msg == WM_KEY)
    {
        if( !(param & KEY_UP_CODE))
        {
            param &= KEY_PRESREP_MASK;
			switch (param )
			{

#ifdef KEY_OK
				case KEY_OK:
				{
					if(box->flags & TXT_FLAGS_CONST)
						return RES_SIG_OK;
					if(box->flags & TXT_FLAGS_EDIT)
						return RES_SIG_OK;
					//box->flags |= TXT_FLAGS_EDIT;
					CSTRING s1;
					CSTRING s2;
					s1 = box->data.substr(0,box->pos);
					s2 = box->data.substr(box->pos, box->data.length());
					box->data  = s1;
					box->data += buttons[box->x + box->y * 10];
					box->data += s2;
					box->pos++;
					return (FLG_BUSY);
				}
#endif

#ifdef KEY_C
				case KEY_C:
					if(box->flags & (TXT_FLAGS_CONST|TXT_FLAGS_RES))
						return RES_SIG_CANCEL;
					if(box->flags & TXT_FLAGS_EDIT)
					{
						if(box->pos < box->data.length())
							box->data.erase(box->pos,1);
					}
					else
						box->flags ^= TXT_FLAGS_EDIT;
					box->flags &=~TXT_FLAGS_CURSOR;
					return (FLG_BUSY);
#endif

#ifdef KEY_RIGHT
				case KEY_RIGHT:
					box->flags &=~TXT_FLAGS_RES;
					if(box->flags & TXT_FLAGS_EDIT)
					{
						if(++box->pos > box->data.length())
							box->pos = box->data.length();
						box->flags &=~TXT_FLAGS_CURSOR;
					}
					else
					{
						if(++box->x > 9)
							box->x=0;
					}
					return (FLG_BUSY);
#endif

#ifdef KEY_LEFT
				case KEY_LEFT:
					box->flags &=~TXT_FLAGS_RES;
					if(box->flags & TXT_FLAGS_EDIT)
					{
						if(box->pos-- == 0)
							box->pos = 0;
						box->flags &=~TXT_FLAGS_CURSOR;
					}
					else
					{
						if(box->x-- == 0)
							box->x=9;
					}
					return (FLG_BUSY);
#endif

#ifdef KEY_UP
				case KEY_UP:
					box->flags &=~TXT_FLAGS_RES;
					if(box->flags & TXT_FLAGS_CONST)
						return (0);
					if(box->flags & TXT_FLAGS_EDIT)
						box->flags ^= TXT_FLAGS_EDIT;
					else
					{
						if(box->y-- == 0 )
							box->y = TXT_CHAR_LINES-1;
						if(box->sy-- == 0 )
							box->sy = 0;
					}
					return (FLG_BUSY);
#endif

#ifdef KEY_DOWN
				case KEY_DOWN:
					box->flags &=~TXT_FLAGS_RES;
					if(box->flags & TXT_FLAGS_CONST)
						return (0);
					if(box->flags & TXT_FLAGS_EDIT)
						box->flags ^= TXT_FLAGS_EDIT;
					else
					{
						if(++box->y >= TXT_CHAR_LINES)
							box->y = 0;
						if(++box->sy >= TXT_SCREEN_LINES)
							box->sy = TXT_SCREEN_LINES-1;
					}
					return (FLG_BUSY);
#endif
			}
            return (FLAG_WRITE);
        }

    }

    return (0);
}

/*
RES_CODE get_box(CSTRING& title, CSTRING& data)
{
	return get_box(title.c_str(), data);
}
*/
RES_CODE get_box(const char * title, CSTRING& data, bool constant)
{
	RES_CODE res = RES_ERROR;
	CGetBox	box_hnd;
	box_hnd.title = title;
	box_hnd.pos = data.length();
	box_hnd.data += data;
	box_hnd.flags = TXT_FLAGS_EDIT|TXT_FLAGS_RES;
	if(constant)
	{
		box_hnd.pos = 0;
		box_hnd.flags |= TXT_FLAGS_CONST;
	}
	box_hnd.x = 0; box_hnd.y = 0; box_hnd.sy = 0;
	if(box_hnd.tsk_window_init((GUI_CB)getbox_cb))
	{
		res = box_hnd.tsk_window_showmodal();
		if((constant == true) || (box_hnd.flags & TXT_FLAGS_RES) )
			return (res);
		if(data != box_hnd.data)
		{
			res = msg_box("Save changes ?");
			if(res == RES_OK)
				data = box_hnd.data;
		}
		else
			res = RES_IDLE;
	}
	return (res);
}

RES_CODE get_box(CSTRING& title, CSTRING& data, bool constant)
{
	return (get_box(title.c_str(), data, constant));
}



//*----------------------------------------------------------------------------
//*			status_box
//*----------------------------------------------------------------------------
RES_CODE statusbox_cb(CMessageBox* msg_hnd, unsigned int param, unsigned int msg)
{
    if(msg == WM_DRAW)
    {
        draw_box_frame(msg_hnd, (LCD_MODULE*)param);
    }

    return (0);
}

void status_box_free(CMessageBox* msg_hnd)
{
	if(msg_hnd)
	{
		delete msg_hnd;
	}
}

CMessageBox* status_box_show(const char *msg)
{
	CMessageBox* msg_hnd;

	msg_hnd = new CMessageBox();
	if(msg_hnd)
	{
		msg_hnd->msg_text = msg;
		if(msg_hnd->tsk_window_init((GUI_CB)statusbox_cb))
		{
			msg_hnd->rect.y0 += 10;
			msg_hnd->tsk_window_show();
			return (msg_hnd);
		}
		delete msg_hnd;
	}
	return (NULL);
}

CMessageBox* status_box_show(CSTRING& msg)
{
	CMessageBox* msg_hnd;

	msg_hnd = new CMessageBox();
	if(msg_hnd)
	{
		msg_hnd->msg_text = msg;
		if(msg_hnd->tsk_window_init((GUI_CB)statusbox_cb))
		{
			msg_hnd->rect.y0 += 10;
			msg_hnd->tsk_window_show();
			return (msg_hnd);
		}
		delete msg_hnd;
	}
	return (NULL);
}



