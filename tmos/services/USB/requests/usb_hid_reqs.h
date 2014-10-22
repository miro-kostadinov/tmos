/*
 * usb_hid_reqs.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_HID_REQS_H_
#define USB_HID_REQS_H_

#include <tmos.h>
#include <usb_opt.h>


/// HID class specific request codes
enum HIDGenericRequests : unsigned char
{
	HIDRequest_GET_REPORT			= 0x01,
	HIDRequest_GET_IDLE				= 0x02,
	HIDRequest_GET_PROTOCOL			= 0x03,
	HIDRequest_SET_REPORT			= 0x09,
	HIDRequest_SET_IDLE				= 0x0A,
	HIDRequest_SET_PROTOCOL			= 0x0B
};


/// Report types for get/set report request (top byte of wValue)
#define HID_REPORTTYPE_INPUT           0x01
#define HID_REPORTTYPE_OUTPUT          0x02
#define HID_REPORTTYPE_FEATURE         0x03


//------------------------------------------------------------------------------
//     USB HID Keyboard
//------------------------------------------------------------------------------

#define HID_KBD_MODIFIER_NONE			0x00
#define HID_KBD_MODIFIER_LEFT_CTRL		0x01
#define HID_KBD_MODIFIER_LEFT_SHIFT		0x02
#define HID_KBD_MODIFIER_LEFT_ALT		0x04
#define HID_KBD_MODIFIER_LEFT_GUI		0x08
#define HID_KBD_MODIFIER_RIGHT_CTRL		0x10
#define HID_KBD_MODIFIER_RIGHT_SHIFT	0x20
#define HID_KBD_MODIFIER_RIGHT_ALT		0x40
#define HID_KBD_MODIFIER_RIGHT_GUI		0x80

/// HID Keyboard BOOT input report
struct hid_kbd_bin_report_t
{
  uint8_t key_modifier;  	//!< HID_KBD_MODIFIER_xxx Modifiers
  uint8_t reserved;
  uint8_t key_code[6];    	//!< key codes
}__attribute__ ((packed));

#define HID_KBD_LED_NUM_LOCK			0x01
#define HID_KBD_LED_CAPS_LOCK			0x02
#define HID_KBD_LED_SCROLL_LOCK			0x04
#define HID_KBD_LED_COMPOSE				0x08
#define HID_KBD_LED_KANA				0x10

/// HID Keyboard BOOT input report
struct hid_kbd_bout_report_t
{
  uint8_t key_leds;  	//!< HID_KBD_LED_xxx
};

#define HID_KBD_SCAN_NONE                0x00 //!< Reserved (no event indicated)
#define HID_KBD_SCAN_ERRORROLLOVER       0x01 //!< Keyboard ErrorRollOver
#define HID_KBD_SCAN_POSTFAIL            0x02 //!< Keyboard POSTFail
#define HID_KBD_SCAN_ERRUNDEF            0x03 //!< Keyboard ErrorUndefined
#define HID_KBD_SCAN_CAPSLOCK            0x39 //!< CAPSLOCK


#endif /* USB_HID_REQS_H_ */
