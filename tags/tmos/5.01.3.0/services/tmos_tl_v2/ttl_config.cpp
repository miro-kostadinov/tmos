/*
 * ttl_config.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: miro
 */

#include <tmos.h>
#include <ttl_config.h>

WEAK_C void __ttl_trow(unsigned int what, const char* file, unsigned int line, const char* func)
{
	TRACELN("--------- EXCEPTION =%x ", what);
	TRACE1(file);
	TRACE("(%u) in ", line);
	TRACE1(func);
	TRACE1("() -------\r\n");
}


