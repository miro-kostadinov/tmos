/*
 * gmsgbox.h
 *
 *  Created on: 12.11.2013
 *      Author: stanly
 */

#ifndef GMSGBOX_H_
#define GMSGBOX_H_

#include <stdgui.h>
#include <gdialog.h>
#include <gedit.h>


// simple edit dialog


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

	GUI_GET_OBJECT_TYPE(OBJECT_MESSAGEBOX);

protected:
	void draw_this(LCD_MODULE* lcd) override;
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_command(GMessage& msg) override;
	unsigned int process_key(GMessage& msg) override;
	bool is_available() override
	{
		return true;
	}
private:
	RECT_T GetButtonRect(void);
	RECT_T GetTitleRect(void);
	bool   SelectDefaultButton(GObject* button) const;

};

int MessageBox(const char* Text, const char* Caption = nullptr, unsigned int Style=MB_OK, unsigned int def_button=0);
void StatusMessageBox(const char* Text, const char* Caption, unsigned int Style=0, unsigned int time=5000);

#define NEB_PASSWORD (MBF_EDIT_FLAGS(ES_PASSWORD|ES_CENTER|ES_MIDDLE|ES_HIDE_CURSOR))
#define NEB_DIGIT 	 (MBF_EDIT_FLAGS(ES_CENTER|ES_MIDDLE|ES_HIDE_CURSOR))
#define EB_STYLE_TEXT (MBF_EDIT | MBF_INPUT_TYPE(KT_EN)| MBF_EDIT_FLAGS(ES_LEFT|ES_MIDDLE|ES_MULTILINE|ES_AUTO_SCROLL) )
#define EB_STYLE_DIGIT (MBF_EDIT | MBF_INPUT_TYPE(KT_DIGIT)| MBF_EDIT_FLAGS(ES_LEFT|ES_MIDDLE|ES_MULTILINE|ES_AUTO_SCROLL) )

int NumEditBox(CSTRING& value, const char* Caption, unsigned int Style=NEB_PASSWORD, text_metrics_t size = 0);

int EditBox(CSTRING& value, const char* Caption, unsigned int Style = EB_STYLE_TEXT, text_metrics_t size = 0);

RES_CODE msg_error(CSTRING& msg, int err_code);
RES_CODE msg_error(const char *msg, int err_code);

#endif /* GMSGBOX_H_ */
