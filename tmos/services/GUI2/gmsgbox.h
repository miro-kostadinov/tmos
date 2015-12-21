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

#define ID_MB_DLG		0xF0
#define ID_MB_TITLE		0xF1
#define ID_MB_TEXT_BOX	0xF2

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
#define MBF_INPUT_TYPE(type) 	(((type)&0xF)<<8)
#define MBF_EDIT_FLAGS(flags) 	((flags)<<16)
#define MBF_BEEP_TYPE(num)		(((num)&0xF)<<12)

#define GET_MBF_INPUT_TYPE(mb_type)	((key_mode)(((mb_type)>>8)&0xF))
#define GET_MBF_EDIT_FLAGS(mb_type)	(((mb_type)>>16)&0xFFFF)
#define GET_MBF_BEEP_TYPE(mb_type)	(((mb_type)>>12)&0xF)

#define MB_OK			MBF_OK
#define MB_OKCANCEL 	(MBF_OK|MBF_CANCEL)
#define	MB_RETRYCANCEL  (MBF_RETRY|MBF_CANCEL)
#define MB_YESNO		(MBF_YES|MBF_NO)
#define MB_YESNOCANCEL  (MBF_YES|MBF_NO|MBF_CANCEL)

#define MB_BEEP(code)	MBF_BEEP_TYPE(code)
/*
 * Message(Edit)Box style format
 * ------------------------------------------------------------------------------
 *  31   30   29   28   27   26   25   24   23   22   21   20   19   18   17   16
 *
 *  15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
 * |   beep type      | BG/EN/bg/en/123   | ctrl flags   |   Buttons             |
 *  nc   nc   nc   nc   nc   nc   nc   nc   nc   CLR  EDI  CANC  NO  YES  RETRY OK
 */

// simple edit dialog
#define EB_
extern const char* MB_IDS[];


struct GMsgBox : GDialog
{
	const char* title;
	CSTRING body;
	const RENDER_MODE* font;
	unsigned int type;
	unsigned int default_button;
	text_metrics_t init_size;
	GEdit* edit_box;
	GTitle* title_box;
	GText*	text_box;
	GMsgBox() : title(nullptr), body(), font(&FNT5x7), type(MB_OK), default_button(0),
			edit_box(nullptr), title_box(nullptr), text_box(nullptr)
	 {};

protected:
	virtual void draw_this(LCD_MODULE* lcd);
	virtual unsigned int initialize (GMessage& msg);
	virtual unsigned int process_command(GMessage& msg);
	virtual unsigned int process_key(GMessage& msg);
	virtual bool is_available()
	{
		return true;
	}
private:
	RECT_T GetButtonRect(void);
	RECT_T GetTitleRect(void);
	bool   SelectDefaultButton(GObject* button);

};

int MessageBox(const char* Text, const char* Caption = nullptr, unsigned int Style=MB_OK, unsigned int def_button=0);
void StatusMessageBox(const char* Text, const char* Caption, unsigned int Style=0, unsigned int time=5000);

#define NEB_PASSWORD (MBF_EDIT_FLAGS(ES_PASSWORD|ES_CENTER|ES_MIDDLE|ES_HIDE_CURSOR))
#define NEB_DIGIT 	 (MBF_EDIT_FLAGS(ES_CENTER|ES_MIDDLE|ES_HIDE_CURSOR))
#define EB_STYLE_TEXT (MBF_EDIT | MBF_INPUT_TYPE(KT_EN)| MBF_EDIT_FLAGS(ES_LEFT|ES_MIDDLE|ES_MULTILINE|ES_AUTO_SCROLL) )
#define EB_STYLE_DIGIT (MBF_EDIT | MBF_INPUT_TYPE(KT_DIGIT)| MBF_EDIT_FLAGS(ES_LEFT|ES_MIDDLE|ES_MULTILINE|ES_AUTO_SCROLL) )

int NumEditBox(CSTRING& value, const char* Caption, unsigned int Style=NEB_PASSWORD, text_metrics_t size = 0);

int EditBox(CSTRING& value, const char* Caption, unsigned int Style = EB_STYLE_TEXT, text_metrics_t size = 0);

#endif /* GMSGBOX_H_ */
