/*
 * menus.h
 *
 *  Created on: 2008-10-19
 *      Author: Miroslav Kostadinov
 */

#ifndef MENUS_H_
#define MENUS_H_

#define MAIN_MENU_SIG 1
#define UART_MENU_SIG 2

extern TASK_STRU menu_task;

RES_CODE keytest_cb(WINDOW obj, unsigned int param, unsigned int msg);

#endif /* MENUS_H_ */
