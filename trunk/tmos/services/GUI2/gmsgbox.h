/*
 * gmsgbox.h
 *
 *  Created on: 12.11.2013
 *      Author: stanly
 */

#ifndef GMSGBOX_H_
#define GMSGBOX_H_

#include <gdialog.h>
#include <gedit.h>

#define ID_MB		0xFF

// buttons text index
#define IDS_OK			0
#define IDS_RETRY		1
#define IDS_YES			2
#define IDS_NO			3
#define IDS_CANCEL		4

// buttons flags
#define MBF_OK			(1<<0)
#define MBF_RETRY		(1<<1)
#define MBF_YES			(1<<2)
#define MBF_NO			(1<<3)
#define MBF_CANCEL		(1<<4)
#define MBF_LAST_BTN	(1<<5)
// control flags
#define MBF_EDIT		(1<<5)
#define MBF_CLR			(1<<6)

// control fields
#define MBF_INPUT_TYPE(type) 	((type)<<8)
#define MBF_EDIT_FLAGS(flags) 	((flags)<<16)

#define GET_MBF_INPUT_TYPE(mb_type)	((key_mode)(((mb_type)>>8)&0xFF))
#define GET_MBF_EDIT_FLAGS(mb_type)	(((mb_type)>>16)&0xFFFF)

#define MB_OK			MBF_OK
#define MB_OKCANCEL 	(MBF_OK|MBF_CANCEL)
#define	MB_RETRYCANCEL  (MBF_RETRY|MBF_CANCEL)
#define MB_YESNO		(MBF_YES|MBF_NO)
#define MB_YESNOCANCEL  (MBF_YES|MBF_NO|MBF_CANCEL)

// simple edit dialog
#define EB_
extern const char* MB_IDS[];


struct GMsgBox : GDialog
{
	const char* title;
	CSTRING body;
	const RENDER_MODE* font;
	unsigned int type;
	GEdit* edit_box;
	GMsgBox() : title(NULL), body(), font(&FNT5x7), type(MB_OK), edit_box(NULL)
	 {};

	virtual void draw_this(LCD_MODULE* lcd);
	virtual unsigned int initialize (GMessage& msg);
	virtual unsigned int process_command(GMessage& msg);
	virtual unsigned int process_key(GMessage& msg);
private:
	RECT_T GetButtonRect(void);
	RECT_T GetTitleRect(void);

};

int MessageBox(const char* Text, const char* Caption = NULL, unsigned int Style=MB_OK);
int NumEditBox(CSTRING& value, const char* Caption);

#endif /* GMSGBOX_H_ */
