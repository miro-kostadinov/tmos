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

struct usb_remote_hid_t: public usb_remote_dev_t
{
	HANDLE 	epi_hnd;
	USBInterfaceDescriptor* pid;
	uint8_t	hid_idle;

	usb_remote_hid_t(HANDLE h1, HANDLE h2): usb_remote_dev_t(h1),
			epi_hnd(h2), pid(NULL) , hid_idle(255) {};

	RES_CODE get_hid_report_descriptor(void* buf, uint32_t len);
	RES_CODE get_hid_descriptor(void* buf, uint32_t len);

	RES_CODE get_report(void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id);
	RES_CODE set_report(const void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id);
	RES_CODE get_idle(uint32_t rep_id);
	RES_CODE set_idle(uint32_t duration, uint32_t rep_id);
	RES_CODE get_protocol(void* buf);
	RES_CODE set_protocol(uint32_t protocol);

	RES_CODE scan_hid(uint32_t port_indx, USBSubClassCode subcls, USBProtocolCode proto);
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

	uint32_t get_modifier(uint32_t modifier);
	void key_press(uint32_t code);
	void key_release(uint32_t code);
	usb_remote_kbd_t(HANDLE h1, HANDLE h2): usb_remote_hid_t(h1, h2),
		key_leds(0), key_leds2(0), key_lang(0), def_lang(1), key_time(0)
	{
	};

	void kbd_process();
	void kbd_timeout();
};

#endif /* USBH_HID_H_ */
