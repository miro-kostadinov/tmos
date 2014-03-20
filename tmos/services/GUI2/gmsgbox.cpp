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
		trect.y1 = trect.y0 + 2*font->vdistance + font->vspacing;
	}
	return trect;
}

static const char ret_codes[] = {GO_IDOK, GO_IDRETRY, GO_IDYES, GO_IDNO, GO_IDCANCEL};

unsigned int GMsgBox::initialize (GMessage& msg)
{
	LCD_MODULE* lcd = ((LCD_MODULE **)msg.lparam)[0];
	id = ID_MB;
	rect = lcd->rect;
	flags = GO_FLG_BORDER|GO_FLG_ENABLED|GO_FLG_SHOW|GO_FLG_SELECTED;

	RECT_T button_rect;
	RECT_T title_rect;
	RECT_T message_rect;

	int bnum =0;
	for(int mask = 1; type && mask < MBF_LAST_BTN; mask <<=1)
		if(type & mask)
			bnum++;

	if(body.empty())
	{
		for(int i=0; i < lcd->chars_per_row/2; i++)
			body += '.';
		type |= MBF_CLR;
	}

	POINT_T bs = get_border_size();
	while(1)
	{
		allocate_border();
		if(bnum)
			button_rect = GetButtonRect();
		title_rect = GetTitleRect();
		message_rect =client_rect;

		message_rect.y0 += title_rect.height();
		message_rect.y1 -= button_rect.height();

		int width=0;
		int dec = message_rect.width();
		if(type & MBF_EDIT)
			dec -= 2*bs.x;
		text_metrics_t new_size;
		text_metrics_t msg_size = get_text_metrics(body.c_str(), dec, font);

		if(msg_size.height + font->vspacing < message_rect.height())
		{
			rect.y1 -= message_rect.height() - (msg_size.height + font->vspacing);
			continue;
		}
		if(dec > msg_size.width + 3*font->hdistance)
			message_rect.x1 -= dec - msg_size.width - 3*font->hdistance;
		width = message_rect.width();
//		if(type & MBF_EDIT)
//			dec -= 2*bs.x;
//		do
//		{
//			if(width < dec)
//				width = dec;
//			dec -= font->hspacing;
//		}while( msg_size.height  == get_text_metrics(body.c_str(), dec, font).height);

		if( title_rect.width() > width)
			width = title_rect.width();
		if(button_rect.width() + bnum * bs.x > width)
			width = button_rect.width() + bnum * bs.x ;
		if(client_rect.width() > width)// + font->hspacing)
		{
			rect.x1 -= client_rect.width() - width;
			continue;
		}
		break;
	}
	if(type & MBF_CLR)
		body.clear();


    int x_offset, y_offset;
    x_offset = (lcd->rect.width() - rect.width())/2;
    y_offset = (lcd->rect.height() - rect.height())/2;

    rect.Offset(x_offset, y_offset);
    client_rect.Offset(x_offset, y_offset);
    button_rect.Offset(x_offset, y_offset);
    if(message_rect.width() < client_rect.width())
    	x_offset += (client_rect.width() - message_rect.width())/2;
    message_rect.Offset(x_offset, y_offset);

    if(type & MBF_EDIT)
    {
    	edit_box = new GEdit(ID_MB-1, message_rect, body, NULL,
    			((bnum)?0:GO_FLG_SELECTED)|
				GO_FLG_BORDER|GO_FLG_VSCROLL|GO_FLG_TRANSPARENT|GO_FLG_DEFAULT,
				 GET_MBF_EDIT_FLAGS(type),
				font);
    	if(edit_box)
    	{
			if(addChild(edit_box))
			{
				edit_box->shift = GET_MBF_INPUT_TYPE(type);
//				edit_box->align = GET_MBF_EDIT_FLAGS(type);
			}
			else
			{
				delete edit_box;
				edit_box = NULL;
			}
    	}
    }
    else
		addChild(new GText(ID_MB-1, message_rect, body, NULL,
    			((bnum)?0:GO_FLG_SELECTED)|
				GO_FLG_VSCROLL|GO_FLG_TRANSPARENT|GO_FLG_DEFAULT,
				SS_DEFAULT,
				font));

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
				button_rect.x0 += bdistance;
				button_rect.x1 = button_rect.x0 + (strlen(MB_IDS[i]) * font->hspacing) + distance;
				addChild(new GButton(ret_codes[i], button_rect, ret_codes[i],
						MB_IDS[i], GO_FLG_DEFAULT|GO_FLG_BORDER | ((bnum)?GO_FLG_SELECTED:0 )));
				bnum = 0;
				button_rect.x0 += button_rect.width();
			}
		}
	}
//	else
//		bdistance = 0;

//	int distance = 4*font->hdistance + 2*bs.x;
//	button_rect.Inflate(0,2);
//	button_rect.Offset(0,1);
//	for(unsigned char mask =1, i=0; mask < MBF_LAST_BTN; mask <<=1, i++)
//	{
//		if(type & mask)
//		{
//			button_rect.x0 += bdistance;
//			button_rect.x1 = button_rect.x0 + (strlen(MB_IDS[i]) * font->hspacing) + distance;
//			addChild(new GButton(ID_MB -(i+1), button_rect, ret_codes[i],
//					MB_IDS[i], GO_FLG_DEFAULT|GO_FLG_BORDER | (bnum)?GO_FLG_SELECTED:0 ));
//			bnum = 0;
//			button_rect.x0 += button_rect.width();
//		}
//	}
	return GWindow::initialize(msg);
}

void GMsgBox::draw_this(LCD_MODULE* lcd)
{
	if(flags & GO_FLG_BORDER)
		draw_border(rect);

	if(title && *title)
	{
		RECT_T rc(client_rect);
		client_rect = rect;
		client_rect = GetTitleRect();
		client_rect.x1 = rect.x1;
		draw_border(client_rect);
		lcd->pos_x = client_rect.x0 +font->hdistance;
		lcd->pos_y = client_rect.y0 +font->vdistance;
		lcd->allign = TA_CENTER;
		lcd->font = font;
		draw_text(lcd, title);
		draw_poligon(client_rect, true);
		draw_hline(client_rect.x0, client_rect.x1, client_rect.y1);
		client_rect = rc;
	}
}

unsigned int GMsgBox::process_command (GMessage& msg)
{

	switch(msg.param)
	{
	case ID_MB-1:
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

unsigned int GMsgBox::process_key (GMessage& msg)
{
	if(GDialog::process_key(msg))
		return 1;

	if (msg.param == KEY_OK)
	{
		if (!(type & (MBF_LAST_BTN-1)) || (type & ((MBF_LAST_BTN-1))) == MBF_OK )
		{
			notify_message(WM_CLOSE, GO_IDOK, this);
			return 1;
		}

	}
	if (msg.param == KEY_CANCEL)
	{
		if((type&(MBF_LAST_BTN-1)) == MBF_OK)
		{
			notify_message(WM_CLOSE, GO_IDCANCEL, this);
			return 1;
		}
	}
	return 0;
}
int MessageBox(const char* Text, const char* Caption, unsigned int Style)
{
	GMsgBox box;
	box.displays = 1;
	box.type = Style;
	box.body =  Text;
	box.title = Caption;
	return box.DoModal();
}

int NumEditBox(CSTRING& value, const char* Caption, unsigned int Style)
{
	GMsgBox box;
	box.type = 	MBF_EDIT|MBF_CLR|
				MBF_INPUT_TYPE(KT_DIGIT)|Style;
	box.body =  value;
	box.title = Caption;

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

int EditBox(CSTRING& value, const char* Caption, unsigned int Style)
{
	GMsgBox box;
	GMessage msg;

	box.type = 	Style;
	box.body =  value;
	box.title = Caption;

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
