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

bool GEditVKB::CreateVKB()
{
	//1. add to Z-order
	nextObj = Gdesktop->parent->focus->nextObj;
	Gdesktop->parent->focus->nextObj = this;
	//2. Set LCD
	parent = Gdesktop->parent;
	GQueue.push(GMessage(WM_INIT, 0 , this));	//send WM_INIT to the new virtual keyboard
	return true;
}

unsigned int GEditVKB::initialize (GMessage& msg)
{
	flags = GO_FLG_ENABLED | GO_FLG_SHOW;
	rect = parent->rect; // LCD rectangle, get all display
	//TODO: add children,
	return GContainer::initialize(msg);
}

unsigned int GEditVKB::process_key(GMessage& msg)
{
	return 0;
}
