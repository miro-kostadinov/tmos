#ifndef STDGUI_H
#define STDGUI_H

#ifndef GUI_LANGUAGES
#define GUI_LANGUAGES	2
#endif

extern unsigned int current_laguage;

//----- GUI Objects
typedef struct CWindow  *WINDOW;
typedef RES_CODE (* GUI_CB)(WINDOW obj, unsigned int param, unsigned int msg);
typedef union
{
    struct {unsigned char x0, y0, x1, y1;};
    unsigned int as_int;
} RECT_STRU;


struct CWindow : CHandle
{
	//-- common fields with handle
		//	mode0 =  set from the driver/cleared from helper (FLG_SIGNALED to close, FLG_OK to callback)
		//	mode1 =  last callback result.
						// FLG_BUSY - request a redraw
						// FLG_SIGNALED	- setting this flag will close the GUI object
		//	next  =	 next in down direction (Z-order)
		//	mode  =  cb_fun

	// common edns
    RECT_STRU		rect;
//	WINDOW			prev;					//up in Z-order
	RES_CODE	callback(unsigned int param, unsigned int msg)
	{
		return (((GUI_CB)mode.as_voidptr)(this, param, msg));
	}
	bool tsk_window_init( GUI_CB cback);

	RES_CODE tsk_window_showmodal()
	{
		return (tsk_read(NULL, 0));
	}

	void tsk_window_show()
	{
		tsk_start_read(NULL, 0);
	}

};

WINDOW tsk_new_window(GUI_CB callback);

//----- menus

typedef struct MENU_ITEM_STRU* MENU_ITEM;
typedef struct CMenu * MENU_WINDOW;

typedef unsigned int (*MENU_CBF)(void *param, MENU_WINDOW menu_hnd);

struct  MENU_ITEM_STRU
{
	unsigned int	priority;
    CSTRING			name[GUI_LANGUAGES];
    MENU_CBF		menu_func;
    void *			param;
};

struct  STATIC_MENU_ITEM
{
	unsigned int	priority;
    const char*		name[GUI_LANGUAGES];
    MENU_CBF		menu_func;
    void *			param;
};

struct MENU_STRU
{
    unsigned int count;
    CSTRING			name[GUI_LANGUAGES];
    CSTRING			file;
    MENU_ITEM_STRU	items[];
};

struct CMenu: CWindow
{
    unsigned short int		pos;
    unsigned short int		text_offset;
    const MENU_STRU		*menu;
};

RES_CODE menu_cb(MENU_WINDOW menu_hnd, unsigned int param, unsigned int msg);
RES_CODE menu_box(const MENU_STRU *menu, MENU_WINDOW menu_hnd);

#define MENU_DECLARE_STATIC(amenu, acount)	\
    typedef struct { unsigned int count;  const char* name[GUI_LANGUAGES]; const char *file; STATIC_MENU_ITEM	items[acount];} amenu##_type;

//----- message box

struct CMessageBox: CWindow
{
    CSTRING			msg_text;
    unsigned int 	offset;
    CMessageBox(): CWindow(), offset(0) {};

    void update(const char* str)
    {
    	if(this)
    		msg_text = str;
    };
    void update(CSTRING& str)
    {
    	if(this)
    		msg_text = str;
    };
};


#define TXT_FLAGS_CURSOR	0x01
#define TXT_FLAGS_EDIT		0x02
#define TXT_FLAGS_CONST		0x04
#define TXT_FLAGS_RES		0x08

struct CGetBox: CWindow
{
	CSTRING			title;
	CSTRING			data;
	unsigned char	pos;
	unsigned char	flags;
	char			ch;
	unsigned char	x, y;
	unsigned char	sy;
};

//----- status box
void status_box_free(CMessageBox* msg_hnd);
CMessageBox* status_box_show(const char *msg);
CMessageBox* status_box_show(CSTRING& msg);

RES_CODE msg_box(const char *msg);
RES_CODE msg_box(CSTRING& msg);
RES_CODE msg_error(CSTRING& msg, int err_code);
RES_CODE msg_error(const char *msg, int err_code);
RES_CODE get_box(const char * title, CSTRING& data, bool constant = false);
RES_CODE get_box(CSTRING& title, CSTRING& data, bool constant = false);

#endif
