/*
 * gfloating_text.cpp
 *
 *  Created on: 19.09.2013
 *      Author: edguest
 */

#include <gfloating_text.h>

unsigned int GFloating_Text::initialize(GMessage msg)
{
	client_rect = rect;
	addChild (new GText (0, rect, txt, GO_FLG_DEFAULT, font));
	children->client_rect = rect;
	children->client_rect.y0 = client_rect.y0 + ((client_rect.y1 - client_rect.y0) >> 1) - ((((GText*)children)->text_font->vspacing + ((GText*)children)->text_font->vdistance) >> 1);
	children->client_rect.y1 = children->client_rect.y0 + ((GText*)children)->text_font->vdistance + ((GText*)children)->text_font->vspacing;
	children->client_rect.x1 = children->rect.x1 = client_rect.x0 + (((GText*)children)->text_font->hdistance<<2) +(((GText*)children)->txt.length()+1)*((GText*)children)->text_font->hspacing;
	offset = client_rect.x0;
	return 1;
}

unsigned int GFloating_Text::process_idle(GMessage msg)
{
	if (parent->flags & GO_FLG_SELECTED)
	{
		if ((children->client_rect.x1 - children->client_rect.x0) > (client_rect.x1 - client_rect.x0))
			if ((CURRENT_TIME - float_time) > 200)
			{
				invalidate (this, rect);
				if (children->client_rect.x1 <= offset)
				{
					children->client_rect.x0 = children->rect.x0 = client_rect.x1;
					children->client_rect.x1 = children->rect.x1 = client_rect.x1 + (((GText*)children)->text_font->hdistance<<2) +(((GText*)children)->txt.length()+1)*((GText*)children)->text_font->hspacing;
				}
				children->client_rect.x0 -= 3;
				children->client_rect.x1 -= 3;
				children->rect.x0 -= 3;
				children->rect.x1 -= 3;
				float_time = CURRENT_TIME;
			}
	}
	else
	{
		if (children->rect.x0 != client_rect.x0)
		{
			children->rect.x0 = children->client_rect.x0 = client_rect.x0;
			children->client_rect.x1 = children->rect.x1 = client_rect.x0 + (((GText*)children)->text_font->hdistance<<2) +(((GText*)children)->txt.length()+1)*((GText*)children)->text_font->hspacing;
			invalidate (this, rect);
		}
	}
	return 0;
}

