/*
 * gmsgbox.cpp
 *
 *  Created on: 12.11.2013
 *      Author: stanly
 */


#include <gmsgbox.h>
#include <gtext.h>
#include <gedit.h>
#include <gbutton.h>
#include <lcd.h>

const char* MB_IDS[] =
{
	"Ok",
	"Retry",
	"Yes",
	"No",
	"Cancel"
};

WEAK void weak_gui_message_beep(int code)
{

}

WEAK RES_CODE msg_error(CSTRING& msg, int err_code)
{
	MessageBox(msg.c_str());
	return RES_ERROR;
}

WEAK RES_CODE msg_error(const char *msg, int err_code)
{
	MessageBox(msg);
	return RES_ERROR;
}

RECT_T GMsgBox::GetButtonRect(void)
{
	RECT_T brect;
	POINT_T bs = get_border_size();
	int distance = 4*font->hdistance + 2*bs.x;
	for(unsigned char mask =1, i=0; mask < MBF_LAST_BTN; mask <<=1, i++)
	{
		if(type & mask)
			brect.x1 += (strlen(MB_IDS[i]) * font->hspacing) + distance;
	}
	brect.y1 = font->vspacing + 2*font->vdistance + 2*bs.y;
	brect.Deflate(0, 2);
	brect.Offset(client_rect.x0, client_rect.y1 - brect.y1);
	return brect;
}

RECT_T GMsgBox::GetTitleRect(void)
{
	RECT_T trect;
	if(title && *title)
	{
		trect = client_rect;
		trect.x1 = trect.x0 + strlen(title) * font->hspacing +  2*font->hdistance;
		trect.y1 = trect.y0 + /*2* font->vdistance*/ + font->vspacing;
	}
	return trect;
}

static const char ret_codes[] = {GO_IDOK, GO_IDRETRY, GO_IDYES, GO_IDNO, GO_IDCANCEL};

unsigned int GMsgBox::initialize (GMessage& msg)
{
	LCD_MODULE* lcd = ((LCD_MODULE **)msg.lparam)[0];
	id = ID_MB_DLG;
	rect = lcd->rect;
	flags = GO_FLG_BORDER|GO_FLG_ENABLED|GO_FLG_SHOW|GO_FLG_SELECTED;

	RECT_T button_rect;
	RECT_T title_rect;
	RECT_T message_rect;

	int bnum =0;
	for(int mask = 1; type && mask < MBF_LAST_BTN; mask <<=1)
		if(type & mask)
			bnum++;

	if(body.empty() && !init_size.as_int )
	{
		for(int i=0; i < lcd->chars_per_row/2; i++)
			body += '.';
		type |= MBF_CLR;
	}

	POINT_T bs = get_border_size();
	text_metrics_t msg_size;
    int x_offset, y_offset;
	while(1)
	{
		allocate_border();
		if(bnum)
			button_rect = GetButtonRect();
		title_rect = GetTitleRect();
		message_rect =client_rect;

		message_rect.y0 += title_rect.height()+1; 								// title to message spacing 1 dot
		message_rect.y1 -= (button_rect.height())?(button_rect.height()):1;		// message to buttons spacing 1 dot

		int width=0;
		int dec = message_rect.width();
		if(type & MBF_EDIT)
			dec -= 2*bs.x;

		if(init_size.as_int)
			msg_size.as_int = init_size.as_int;
		else
			msg_size = get_text_metrics(body.c_str(), dec, font);

		if(msg_size.height + font->vspacing < message_rect.height())
		{
			rect.y1 -= message_rect.height() - (msg_size.height + font->vspacing);
			continue;
		}
		else
		{
			if(message_rect.height() < msg_size.height)
			{
				flags |= GO_FLG_VSCROLL;
				x_offset = message_rect.height();
				if(client_rect.height() > msg_size.height)
					message_rect.y1 = message_rect.y0 + msg_size.height;
				else
					message_rect.y1 = message_rect.y0 + client_rect.height();// +client_rect.y1;
				x_offset = message_rect.height() - x_offset;
				if(button_rect)
				{
					button_rect.Offset(0, x_offset);
				}
			}
		}
		if(dec > msg_size.width + 3*font->hdistance)
		{
			message_rect.x1 -= dec - msg_size.width - 3*font->hdistance;
			text_metrics_t new_msg_size;
			new_msg_size = get_text_metrics(body.c_str(), message_rect.width(), font);
			while(new_msg_size.height != msg_size.height )
			{
				message_rect.x1 += 3*font->hdistance;
				new_msg_size = get_text_metrics(body.c_str(), message_rect.width(), font);
			}
		}
		width = message_rect.width();
		if( title_rect.width() > width)
			width = title_rect.width();
		if(button_rect.width() + bnum * bs.x > width)
			width = button_rect.width() + bnum * bs.x ;
		if(client_rect.width() > width)
		{
			rect.x1 -= client_rect.width() - width;
			continue;
		}
		break;
	}
	if(type & MBF_CLR)
		body.clear();

    x_offset = (lcd->rect.width() - rect.width())/2;
    y_offset = (lcd->rect.height() - rect.height())/2;

    rect.Offset(x_offset, y_offset);
    client_rect.Offset(x_offset, y_offset);
    button_rect.Offset(x_offset, y_offset);
    if(title_rect)
    {
    	title_rect.x0 = rect.x0;
    	title_rect.x1 = rect.x1;
    	title_rect.Offset(0, y_offset);
    	GFlags flg = GO_FLG_SHOW|GO_FLG_TRANSPARENT;
    	if(message_rect.height() >= client_rect.height())
    		flg |= GO_FLG_ENABLED;
    	title_box = new GTitle(	ID_MB_TITLE, title_rect, title, nullptr,
    							flg, SS_CENTER|SS_TOP);
    	addChild(title_box);
    }
    if(message_rect.width() < client_rect.width())
    	x_offset += (client_rect.width() - message_rect.width())/2;
    message_rect.Offset(x_offset, y_offset);

    if(type & MBF_EDIT)
    {
    	edit_box = new GEdit(ID_MB_TEXT_BOX, message_rect, body, nullptr,
    			((bnum)?0:GO_FLG_SELECTED)|
				 GO_FLG_BORDER|GO_FLG_VSCROLL|GO_FLG_TRANSPARENT|GO_FLG_DEFAULT,
				 GET_MBF_EDIT_FLAGS(type),
				font);
		if(addChild(edit_box))
		{
			edit_box->shift = GET_MBF_INPUT_TYPE(type);
			text_box = edit_box;
		}
    }
    else
    {
    	text_box = new GText(ID_MB_TEXT_BOX, message_rect, body, nullptr,
 				GO_FLG_VSCROLL|GO_FLG_TRANSPARENT|GO_FLG_SHOW,SS_DEFAULT,font);
		if(addChild( text_box ))
		{
			if(message_rect.height() < msg_size.height)
				text_box->flags |= GO_FLG_ENABLED;
		}
    }
	int bdistance = client_rect.width();

	if ( bnum )
	{
		if(bdistance > button_rect.width())
		{
			bdistance -= button_rect.width();
			bdistance /= bnum+1;
		}
		else
			bdistance = 0;
		int distance = 4*font->hdistance + 2*bs.x;
		button_rect.Inflate(0,2);
		button_rect.Offset(0,1);
		for(unsigned char mask =1, i=0; mask < MBF_LAST_BTN; mask <<=1, i++)
		{
			if(type & mask)
			{
				if(!default_button)
					default_button = mask;

				button_rect.x0 += bdistance;
				button_rect.x1 = button_rect.x0 + (strlen(MB_IDS[i]) * font->hspacing) + distance;
				addChild(new GButton(mask/*ret_codes[i]*/, button_rect, ret_codes[i], MB_IDS[i],
						GO_FLG_DEFAULT|GO_FLG_BORDER|GO_FLG_TRANSPARENT|
						((default_button&mask)?GO_FLG_SELECTED:0 )));
				button_rect.x0 += button_rect.width();
			}
		}
	}
	weak_gui_message_beep(GET_MBF_BEEP_TYPE(type));
	return GDialog::initialize(msg);
}

void GMsgBox::draw_this(LCD_MODULE* lcd)
{
	GWindow::draw_this(lcd);
}

unsigned int GMsgBox::process_command (GMessage& msg)
{

	switch(msg.param)
	{
	case ID_MB_TEXT_BOX:
		if(type &(MBF_LAST_BTN-1))
			break;
		notify_message(WM_CLOSE, GO_IDOK, this);								//closes the window
		return 1;

	case GO_IDYES:
		notify_message(WM_CLOSE, GO_IDYES, this);								//closes the window
		return 1;
	case GO_IDNO:
		notify_message(WM_CLOSE, GO_IDNO, this);							//closes the window
		return 1;
	case GO_IDRETRY:
		notify_message(WM_CLOSE, GO_IDRETRY, this);							//closes the window
		return 1;
	default:
		return GDialog::process_command(msg);
	}
	return 0;
}

bool GMsgBox::SelectDefaultButton(GObject* button)
{
	while(button)
	{
		if(button->is_available() && (button->id & default_button))
			return button->get_focus();
		button = button->nextObj;
	}
	return false;
}

unsigned int GMsgBox::process_key (GMessage& msg)
{
	unsigned int param;
	unsigned int res=0;
	GObject* tmp = focus;

	if(GWindow::process_key(msg))
		return 1;

	if( !text_box )
		return 0;

	switch (msg.param)
	{
	case KEY_RIGHT:
		if(focus)
		{
			if(focus->rect.y1 <= rect.y1)
			{
				if(text_box == focus || title_box == focus)
					break;	// It's a message or title_box, do nothing
				if(focus != last_available()) // It's not a last button
					res = focus_on_next();
				if(res)
					default_button = focus->id;
			}
			move(0,0);
		}
		break;

	case KEY_DOWN:
		if(focus)
		{
			if(focus->rect.y1 <= rect.y1)
			{
				if(focus == title_box)
				{
					// try to move on the text
					res = text_box->get_focus();
					if(!res)
					{
						// all text is visible, try buttons
						res = SelectDefaultButton(text_box->nextObj);
						if(!res)
						{
							// without buttons, do nothing
							break;
						}
					}
				}
				else
				{
					if(text_box == focus)
					{	// move on the buttons
						res = SelectDefaultButton(text_box->nextObj);
						if(!res)
						{
							// without buttons, try title
							if(title_box)
							{
								res = title_box->get_focus();
								if(!res)
								{
									// all text is visible and no buttons, scroll to title
									focus = title_box;
									focus->flags |= GO_FLG_ENABLED;
									move(0,0);
									focus->flags &= ~GO_FLG_ENABLED;
									focus = tmp;
									break;
								}
							}
						}
					}
					else
					{
						// at buttons
						if(title_box && title_box->rect.y0 <= rect.y0) // title not full visible
						{
							// try title
							res = title_box->get_focus();
							if(!res)
							{
								// all text visible and has a title
								focus = title_box;
								focus->flags |= GO_FLG_ENABLED;
								move(0,0);
								focus->flags &= ~GO_FLG_ENABLED;
								focus = tmp;
								break;
							}
						}
						else
						{
							res = text_box->get_focus();
							if(!res)
							{
								// all text visible and no title
								focus = text_box;
								focus->flags |= GO_FLG_ENABLED;
								move(0,0);
								focus->flags &= ~GO_FLG_ENABLED;
								focus = tmp;
								break;
							}
						}
					}
				}
			}
			move(0,0);
		}
		break;

	case KEY_LEFT:
		if(focus)
		{
			if(focus->rect.y1 <= rect.y1)
			{
				if(text_box == focus || title_box == focus) // It's a message, do nothing
					break;
				if(text_box->nextObj != focus) // It's not a first button
					res =  focus_on_previous();
				if(res)
					default_button = focus->id;
			}
			move(0,0);
		}
		break;

	case KEY_UP:
		if(focus)
		{
			if(focus->rect.y1 <= rect.y1)
			{
				if(focus == title_box)
				{
					// at the title
					// try to go on the buttons
					res = SelectDefaultButton(text_box->nextObj);
					if(!res)
					{
						// box without buttons.
						res = text_box->get_focus();
						if(!res)
						{
							// the whole message is visible on the screen
							// do nothing
							break;
						}
					}
				}
				else
				{
					if(focus == text_box)
					{
						// at the text
						// text can be scrolled, try to select title
						if(title_box)
						{
							res = title_box->get_focus();
							if(!res)
							{
								// try to go on buttons
								res = SelectDefaultButton(text_box->nextObj);
								if(!res)
								{
									// box with text only
								}
							}
						}
						else
						{
							//box without title, we are at text. try to select buttons
							res = SelectDefaultButton(text_box->nextObj);
							if(!res)
							{
								// box with text only and all text is visible
								break;
							}
						}
					}
					else
					{
						// at the button
						res = text_box->get_focus();
						if(!res)
						{
							// all text is visible
							if(title_box)
							{
								// has a title, move to it
								res = title_box->get_focus();
								if(!res)
								{
									focus = title_box;
									focus->flags |= GO_FLG_ENABLED;
									move(0,0);
									focus->flags &= ~GO_FLG_ENABLED;
									focus = tmp;
									break;
								}
							}
							else
								break;
						}
					}
				}
			}
			move(0,0);
		}
		break;

	case KEY_OK:
		if (!(type & (MBF_LAST_BTN-1)) || (type & ((MBF_LAST_BTN-1))) == MBF_OK )
		{
			notify_message(WM_CLOSE, GO_IDOK, this);
			return 1;
		}
		break;

	case KEY_CANCEL:
		switch((type&(MBF_LAST_BTN-1)))
		{
		case 0: // no buttons
//			param = GO_EXIT;
		case MB_OKCANCEL:
		case MB_RETRYCANCEL:
		case MB_YESNOCANCEL:
			param = GO_IDCANCEL;
			break;
		case MB_YESNO:
			param = GO_IDNO;
			break;
		default:
			return 0;
		}
		notify_message(WM_CLOSE, param, this);
		return 1;
		break;

	default:
		return 0;
	}

	return res;
}

int _MessageBox(const char* Text, const char* Caption, unsigned int Style, unsigned int def_button)
{
	GMsgBox box;
	box.displays = 1;
	box.type = Style;
	box.body =  Text;
	box.title = Caption;
	box.default_button = def_button;
	return box.DoModal();
}

int MessageBox(const char* Text, const char* Caption, unsigned int Style, unsigned int def_button)
{
	GMsgBox* box = new GMsgBox;
	unsigned int res = 0;
	if(box)
	{
		box->displays = 1;
		box->type = Style;
		box->body =  Text;
		box->title = Caption;
		box->default_button = def_button;
		res = box->DoModal();
		delete box;
	}
	else
		res = _MessageBox(Text, Caption, Style, def_button);
	return res;
}

int NumEditBox(CSTRING& value, const char* Caption, unsigned int Style, text_metrics_t size)
{
	GMsgBox box;
	box.displays = 1;
	box.type = 	MBF_EDIT|
				MBF_EDIT_FLAGS(ES_NUMERIC) |
				MBF_INPUT_TYPE(KT_DIGIT)|Style;
	box.body =  value;
	box.title = Caption;
	box.init_size = size;

	GMessage msg;

	if(box.Create())
	{
		while(1)
		{
			if(box.GetMessage(msg))
			{
				if(msg.code == WM_CLOSE && (msg.param == GO_IDOK || msg.param == GO_IDYES))
				{
					if(box.edit_box)
						value = box.edit_box->txt;
				}
				if(box.DefWinProc(msg))
					return msg.param;
			}
		}
	}
	return 0;

}

int EditBox(CSTRING& value, const char* Caption, unsigned int Style, text_metrics_t size)
{
	GMsgBox box;
	GMessage msg;

	box.displays = 1;
	box.type = 	Style;
	box.body =  value;
	box.title = Caption;
	box.init_size = size;

	if(box.Create())
	{
		while(1)
		{
			if(box.GetMessage(msg))
			{
				if(msg.code == WM_CLOSE && (msg.param == GO_IDOK || msg.param == GO_IDYES))
				{
					if(box.edit_box)
						value = box.edit_box->txt;
				}
				if(box.DefWinProc(msg))
					return msg.param;
			}
		}
	}

	return 0;
}

static void static_StatusMessageBox(const char* Text, const char* Caption, unsigned int Style, unsigned int time)
{
	GMsgBox box;
	GMessage msg;

	box.displays = 1;
	box.type = 	Style;
	box.body =  Text;
	box.title = Caption;

	if(box.Create())
	{
		while(1)
		{
			if(box.GetMessage(msg, 100))
			{
				if(box.DefWinProc(msg))
					break;
			}
			else
			{
				if(time)
				{
					if(time > 100)
						time -= 100;
					else
						break;
				}
			}
		}
	}
}

void StatusMessageBox(const char* Text, const char* Caption, unsigned int Style, unsigned int time)
{
	GMsgBox* box = new GMsgBox;
	if(box)
	{
		box->displays = 1;
		box->type = 	Style;
		box->body =  Text;
		box->title = Caption;
		if(box->Create())
		{
			GMessage msg;
			unsigned int start = CURRENT_TIME;
			while(1)
			{
				if(box->GetMessage(msg, 100))
				{
					if(box->DefWinProc(msg))
						break;
				}
				else
				{
					if(ms_since(start) > time)
						break;
				}
			}
		}
		delete box;
	}
	else
		static_StatusMessageBox(Text, Caption, Style, time);
}
