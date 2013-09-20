/*
 * gradio.h
 *
 *  Created on: 13.09.2013
 *      Author: edguest
 */

#ifndef GRADIO_H_
#define GRADIO_H_

#include <tmos.h>
#include <gcontainer.h>

struct GRadio: GContainer
{
	GObject* checked;
	GRadio (): checked(NULL) {};
	GRadio (GId id_t, RECT_T rect_t,
			GFlags flags_t = GO_FLG_DEFAULT)
	:GContainer (id_t, rect_t, flags_t), checked(NULL) {};

	virtual GObject* addChild (GObject* child);
	unsigned int process_key (GMessage msg);
	unsigned int process_default(GMessage msg);
};


#endif /* GRADIO_H_ */
