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
	GRadio (): checked(nullptr) {};
	GRadio (GId id_t, const RECT_T& rect_t,
			GFlags flags_t = GO_FLG_DEFAULT)
	:GContainer (id_t, rect_t, flags_t), checked(nullptr) {};

	GUI_GET_OBJECT_TYPE(OBJECT_RADIO);

	GObject* addChild (GObject* child) override;
	unsigned int process_key (GMessage& msg) override;
	unsigned int process_default(GMessage& msg) override;
};


#endif /* GRADIO_H_ */
