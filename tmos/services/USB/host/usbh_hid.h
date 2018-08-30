/*
 * usbh_hid.h
 *
 *  Created on: Oct 14, 2014
 *      Author: miro
 */

#ifndef USBH_HID_H_
#define USBH_HID_H_


#include <tmos.h>
#include <usb_host.h>

#define USBHID_RPTITEM_SIZE_MASK          0x03
#  define USBHID_RPTITEM_SIZE_0           0x00 /* No data follows */
#  define USBHID_RPTITEM_SIZE_1           0x01 /* 1 byte of data follows */
#  define USBHID_RPTITEM_SIZE_2           0x02 /* 2 bytes of data follow */
#  define USBHID_RPTITEM_SIZE_4           0x03 /* 4 bytes of data follow */
#define USBHID_RPTITEM_TYPE_MASK          0x0c
#  define USBHID_RPTITEM_TYPE_MAIN        0x00
#  define USBHID_RPTITEM_TYPE_GLOBAL      0x04
#  define USBHID_RPTITEM_TYPE_LOCAL       0x08
#define USBHID_RPTITEM_TAG_MASK           0xf0

/* Global Items (HID 6.2.2.7) */

#define USBHID_GLOBAL_SIZE(pfx)           ((pfx) & USBHID_RPTITEM_SIZE_MASK)
#define USBHID_GLOBAL_USAGEPAGE_PREFIX    0x04 /* Usage Page */
#define USBHID_GLOBAL_LOGICALMIN_PREFIX   0x14 /* Logical Minimum */
#define USBHID_GLOBAL_LOGICALMAX_PREFIX   0x24 /* Logical Maximum */
#define USBHID_GLOBAL_PHYSICALMIN_PREFIX  0x34 /* Physical Minimum */
#define USBHID_GLOBAL_PHYSMICALAX_PREFIX  0x44 /* Physical Maximum */
#define USBHID_GLOBAL_UNITEXP_PREFIX      0x54 /* Unit Exponent */
#define USBHID_GLOBAL_UNIT_PREFIX         0x64 /* Unit */
#define USBHID_GLOBAL_REPORTSIZE_PREFIX   0x74 /* Report Size */
#define USBHID_GLOBAL_REPORTID_PREFIX     0x84 /* Report ID */
#define USBHID_GLOBAL_REPORTCOUNT_PREFIX  0x94 /* Report Count */
#define USBHID_GLOBAL_PUSH_PREFIX         0xa4 /* Push */
#define USBHID_GLOBAL_POP_PREFIX          0xb4 /* Pop */

/* Local Items (HID 6.2.2.8) */

#define USBHID_LOCAL_SIZE(pfx)            ((pfx) & USBHID_RPTITEM_SIZE_MASK)
#define USBHID_LOCAL_USAGE_PREFIX         0x08 /* Usage */
#define USBHID_LOCAL_USAGEMIN_PREFIX      0x18 /* Usage Minimum */
#define USBHID_LOCAL_USAGEMAX_PREFIX      0x28 /* Usage Maximum */
#define USBHID_LOCAL_DESIGNATORIDX_PREFIX 0x38 /* Designator Index  */
#define USBHID_LOCAL_DESIGNATORMIN_PREFIX 0x48 /* Designator Minimum */
#define USBHID_LOCAL_DESIGNATORMAX_PREFIX 0x58 /* Designator Maximum */
#define USBHID_LOCAL_STRINGIDX_PREFIX     0x78 /* String Index */
#define USBHID_LOCAL_STRINGMIN_PREFIX     0x88 /* String Minimum */
#define USBHID_LOCAL_STRINGMAX_PREFIX     0x98 /* xx */
#define USBHID_LOCAL_DELIMITER_PREFIX     0xa8 /*Delimiter */

/* Usage pages (HuT 3) */

#define USBHID_USAGE_PAGE_UNDEFINED       0x00 /* Undefined */
#define USBHID_USAGE_PAGE_GENERIC_DCTRL   0x01 /* Generic Desktop Controls */
#define USBHID_USAGE_PAGE_SIMCTRL         0x02 /* Simulation Controls */
#define USBHID_USAGE_PAGE_VRCTRL          0x03 /* VR Controls */
#define USBHID_USAGE_PAGE_SPORTCTRL       0x04 /* Sport Controls */
#define USBHID_USAGE_PAGE_GAMECTRL        0x05 /* Game Controls */
#define USBHID_USAGE_PAGE_GENERIC_DEVCTRL 0x06 /* Generic Device Controls */
#define USBHID_USAGE_PAGE_KBD             0x07 /* Keyboard/Keypad */
#define USBHID_USAGE_PAGE_LEDS            0x08 /* LEDs */
#define USBHID_USAGE_PAGE_BUTTON          0x09 /* Button */
#define USBHID_USAGE_PAGE_ORDINAL         0x0a /* Ordinal */
#define USBHID_USAGE_PAGE_TELEPHONY       0x0b /* Telephony */
#define USBHID_USAGE_PAGE_CONSUMER        0x0c /* Consumer */
#define USBHID_USAGE_PAGE_DIGITIZER       0x0d /* Digitizer */
                                               /* 0x0e Reserved */
#define USBHID_USAGE_PAGE_PIDPAGE         0x0f /* PID Page USB Physical Interface Device */
#define USBHID_USAGE_PAGE_UNICODE         0x10 /* Unicode */
                                               /* 0x11-13 Reserved */
#define USBHID_USAGE_PAGE_ALPHA_DISPLAY   0x14 /* Alphanumeric Display */
                                               /* 0x15-3f Reserved */
#define USBHID_USAGE_PAGE_MEDICAL         0x40 /* Medical Instruments */
                                               /* 0x41-7f Reserved */
                                               /* 0x80-83 Monitor Devices */
                                               /* 0x84-87 Power Devices */
                                               /* 0x88-8b Reserved */
#define USBHID_USAGE_PAGE_BARCODE_SCANNER 0x8c /* Bar Code Scanner page */
#define USBHID_USAGE_PAGE_SCALE           0x8d /* Scale page */
#define USBHID_USAGE_PAGE_MSR             0x8e /* Magnetic Stripe Reading (MSR) Devices */
#define USBHID_USAGE_PAGE_POS             0x8f /* Point of Sale devices */
#define USBHID_USAGE_PAGE_CAMERA_CTRL     0x90 /* Camera Control Page */

/* Generic Desktop Page Usage IDs (HuT 4) */

#define USBHID_DCTRLUSE_UNDEFINED         0x00 /* Undefined */
#define USBHID_DCTRLUSE_POINTER           0x01 /* Pointer */
#define USBHID_DCTRLUSE_MOUSE             0x02 /* Mouse */
                                               /* 0x03 Reserved */
#define USBHID_DCTRLUSE_JOYSTICK          0x04 /* Joystick */
#define USBHID_DCTRLUSE_GAMEPAD           0x05 /* Game Pad */
#define USBHID_DCTRLUSE_KEYBOARD          0x06 /* Keyboard */
#define USBHID_DCTRLUSE_KEYPAD            0x07 /* Keypad */
#define USBHID_DCTRLUSE_MULTIAXIS         0x08 /* Multi-axis Controller */
#define USBHID_DCTRLUSE_TABLET            0x09 /* Tablet PC System Controls */
                                               /* 0x0a-2f Reserved */
#define USBHID_DCTRLUSE_X                 0x30 /* X */
#define USBHID_DCTRLUSE_Y                 0x31 /* Y */
#define USBHID_DCTRLUSE_Z                 0x32 /* Z */
#define USBHID_DCTRLUSE_RX                0x33 /* Rx */
#define USBHID_DCTRLUSE_RY                0x34 /* Ry */
#define USBHID_DCTRLUSE_RZ                0x35 /* Rz */
#define USBHID_DCTRLUSE_SLIDER            0x36 /* Slider */
#define USBHID_DCTRLUSE_DIAL              0x37 /* Dial */
#define USBHID_DCTRLUSE_WHEEL             0x38 /* Wheel */
#define USBHID_DCTRLUSE_HATSWITCH         0x39 /* Hat switch */
#define USBHID_DCTRLUSE_COUNTED           0x3a /* Counted Buffer */
#define USBHID_DCTRLUSE_BYTECOUNT         0x3b /* Byte Count */
#define USBHID_DCTRLUSE_MOTION            0x3c /* Motion Wakeup */
#define USBHID_DCTRLUSE_START             0x3d /* Start */
#define USBHID_DCTRLUSE_SELECT            0x3e /* Select */
                                               /* 0x3f Reserved */
#define USBHID_DCTRLUSE_VX                0x40 /* Vx */
#define USBHID_DCTRLUSE_VY                0x41 /* Vy */
#define USBHID_DCTRLUSE_VZ                0x42 /* Vz */
#define USBHID_DCTRLUSE_VBRX              0x43 /* Vbrx */
#define USBHID_DCTRLUSE_VBRY              0x44 /* Vbry */
#define USBHID_DCTRLUSE_VBRZ              0x45 /* Vbrz */
#define USBHID_DCTRLUSE_VNO               0x46 /* Vno */
#define USBHID_DCTRLUSE_FEATURE           0x47 /* Feature Notification */
#define USBHID_DCTRLUSE_RESOLUTION        0x48 /* Resolution Multiplier */
                                               /* 0x49-7f Reserved */
#define USBHID_DCTRLUSE_CONTROL           0x80 /* System Control */
#define USBHID_DCTRLUSE_POWERDOWN         0x81 /* System Power Down */
#define USBHID_DCTRLUSE_SLEEP             0x82 /* System Sleep */
#define USBHID_DCTRLUSE_WAKEUP            0x83 /* System Wake Up */
#define USBHID_DCTRLUSE_CONTEXT_MENU      0x84 /* System Context Menu */
#define USBHID_DCTRLUSE_MAIN_MENU         0x85 /* System Main Menu */
#define USBHID_DCTRLUSE_APP_MENU          0x86 /* System App Menu */
#define USBHID_DCTRLUSE_MENU_HELP         0x87 /* System Menu Help */
#define USBHID_DCTRLUSE_MENU_EXIT         0x88 /* System Menu Exit */
#define USBHID_DCTRLUSE_MENU_SELECT       0x89 /* System Menu Select */
#define USBHID_DCTRLUSE_MENU_RIGHT        0x8a /* System Menu Right */
#define USBHID_DCTRLUSE_MENU_LEFT         0x8b /* System Menu Left */
#define USBHID_DCTRLUSE_MENU_UP           0x8c /* System Menu Up */
#define USBHID_DCTRLUSE_MENU_DOWN         0x8d /* System Menu Down */
#define USBHID_DCTRLUSE_COLD_RESTART      0x8e /* System Cold Restart */
#define USBHID_DCTRLUSE_WARM_RESTART      0x8f /* System Warm Restart */
#define USBHID_DCTRLUSE_DPAD_UP           0x90 /* D-pad Up */
#define USBHID_DCTRLUSE_DPAD_DOWN         0x91 /* D-pad Down */
#define USBHID_DCTRLUSE_DPAD_RIGHT        0x92 /* D-pad Right */
#define USBHID_DCTRLUSE_DPAD_LEFT         0x93 /* D-pad Left */
                                               /* 0x94-9f Reserved */
#define USBHID_DCTRLUSE_DOCK              0xa0 /* System Dock */
#define USBHID_DCTRLUSE_UNDOCK            0xa1 /* System Undock */
#define USBHID_DCTRLUSE_SETUP             0xa2 /* System Setup */
#define USBHID_DCTRLUSE_BREAK             0xa3 /* System Break */
#define USBHID_DCTRLUSE_DEBUG_BREAK       0xa4 /* System Debugger Break */
#define USBHID_DCTRLUSE_APP_BREAK         0xa5 /* Application Break */
#define USBHID_DCTRLUSE_APP_DEBUG_BREAK   0xa6 /* Application Debugger Break */
#define USBHID_DCTRLUSE_MUTE              0xa7 /* System Speaker Mute */
#define USBHID_DCTRLUSE_HIBERNATE         0xa8 /* System Hibernate */
                                               /* 0xa9-af Reserved */
#define USBHID_DCTRLUSE_DISPLAY_INVERT    0xb0 /* System Display Invert */
#define USBHID_DCTRLUSE_DISPALY_INTERNAL  0xb1 /* System Display Internal */
#define USBHID_DCTRLUSE_DISPLAY_EXTERNAL  0xb2 /* System Display External */
#define USBHID_DCTRLUSE_DISPLAY_BOTH      0xb3 /* System Display Both */
#define USBHID_DCTRLUSE_DISPLAY_DUAL      0xb4 /* System Display Dual */
#define USBHID_DCTRLUSE_DISPLAY_TOGGLE    0xb5 /* System Display Toggle Int/Ext */
#define USBHID_DCTRLUSE_DISPLAY_SWAP      0xb6 /* System Display Swap */
#define USBHID_DCTRLUSE_                  0xb7 /* System Display LCD Autoscale */
                                               /* 0xb8-ffff Reserved */


struct usb_remote_hid_t: public usb_remote_dev_t
{
	HANDLE 	epi_hnd[2];
	USBInterfaceDescriptor* pid;
	uint8_t	hid_idle;

	usb_remote_hid_t(HANDLE h1, HANDLE hi1, HANDLE hi2): usb_remote_dev_t(h1),
			epi_hnd{hi1, hi2}, pid(NULL) , hid_idle(255) {};

	RES_CODE get_hid_report_descriptor(void* buf, uint32_t len);
	RES_CODE get_hid_descriptor(void* buf, uint32_t len);

	RES_CODE get_report(void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id);
	RES_CODE set_report(const void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id);
	RES_CODE get_idle(uint32_t rep_id);
	RES_CODE set_idle(uint32_t duration, uint32_t rep_id);
	RES_CODE get_protocol(void* buf);
	RES_CODE set_protocol(uint32_t protocol);

	RES_CODE scan_hid(uint32_t port_indx, USBSubClassCode subcls, USBProtocolCode proto);
	RES_CODE config_interface_enpoints();
	RES_CODE is_keypad_report(const USBHIDDescriptor *hid, int i);
};


/// HID Keyboard BOOT input report
struct hid_key_buf_t
{
  uint8_t key_modifier;  	//!< HID_KBD_MODIFIER_xxx Modifiers
  uint8_t len;
  uint8_t key_code[6];    	//!< key codes

  hid_key_buf_t()
  {
	  memclr(this, sizeof(hid_key_buf_t));
  }

}__attribute__ ((packed));

struct usb_remote_kbd_t: public usb_remote_hid_t
{
	hid_key_buf_t 	old_keys;
	hid_key_buf_t	new_keys;
	uint32_t		key_tout[6];
	uint8_t 		key_leds;
	uint8_t 		key_leds2;
	uint8_t 		key_lang;
	uint8_t 		def_lang;
	uint32_t		key_time;

	uint32_t get_modifier(uint32_t modifier) const;
	void key_press(uint32_t code);
	void key_release(uint32_t code);
	usb_remote_kbd_t(HANDLE h1, HANDLE hi1, HANDLE hi2): usb_remote_hid_t(h1, hi1, hi2),
		key_leds(0), key_leds2(0), key_lang(0), def_lang(1), key_time(0)
	{
	};

	void kbd_process();
	void kbd_timeout();
};

#endif /* USBH_HID_H_ */
