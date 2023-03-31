/*
 * gedit_vkb.h
 *
 *  Created on: Mar 22, 2023
 *      Author: bratkov
 */

#ifndef GEDIT_VKB_H_
#define GEDIT_VKB_H_

#include <tmos.h>
#include <gui_drv.h>
#include <gcontainer.h>

struct GEdit;
struct GEditVKB : GContainer
{
	GEdit *	base_edit;
	GEditVKB(GEdit* base) :
		GContainer(), base_edit(base)
	{;}

	virtual ~GEditVKB()
	{;}

	bool CreateVKB();

protected:
	// GUI driver methods
	friend void gui_thread(GUI_DRIVER_INFO* drv_info);
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_key(GMessage& msg) override;
};

#endif /* GEDIT_VKB_H_ */
