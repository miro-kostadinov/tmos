/*
 * usbh_hid.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: miro
 */

#include <tmos.h>
#include <usbh_hid.h>
#include <usb_descriptors.h>
#include <usb_svc.h>

//------------------------------------------------------------------------------
//	Standard Requests
// 		Note: HID is using Interface recipient and Standard type for GET_DESCRIPTOR
//------------------------------------------------------------------------------
RES_CODE usb_remote_hid_t::get_hid_report_descriptor(void* buf, uint32_t len)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_IN_STANDARD_INTERFACE;
	req.bRequest = USBDeviceRequest_GET_DESCRIPTOR;
	req.wValue = (HIDREPORT_DESCRIPTOR << 8) | 0;		// Descriptor type and index
	req.wIndex = pid->bInterfaceNumber;					// Interface number
	req.wLength = len;

	res = std_request(buf);
	return res;
}

RES_CODE usb_remote_hid_t::get_hid_descriptor(void* buf, uint32_t len)
{
	RES_CODE res;

	// HID is using Interface recipient and Standard type for GET_DESCRIPTOR
	req.bmRequestType = USB_REQ_IN_STANDARD_INTERFACE;
	req.bRequest = USBDeviceRequest_GET_DESCRIPTOR;
	req.wValue = (HID_DESCRIPTOR << 8) | 0;				// Descriptor type and index
	req.wIndex = pid->bInterfaceNumber;					// Interface number
	req.wLength = len;

	res = std_request(buf);
	return res;
}

//------------------------------------------------------------------------------
//	Class Requests
//------------------------------------------------------------------------------
RES_CODE usb_remote_hid_t::get_report(void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id)
{
	RES_CODE res;

	//read status
    TRACELN1("hid: get report");

	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = HIDRequest_GET_REPORT;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = (rep_type << 8) | rep_id;			// HID_REPORTTYPE_INPUT / OUTPUT / FEATURE
	req.wLength = len;
	res = std_request(buf);
	return res;
}

RES_CODE usb_remote_hid_t::set_report(const void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id)
{
	RES_CODE res;

	//read status
    TRACELN1("hid: set report");

	req.bmRequestType = USB_REQ_OUT_CLASS_INTERFACE;
	req.bRequest = HIDRequest_SET_REPORT;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = (rep_type << 8) | rep_id;			// HID_REPORTTYPE_INPUT / OUTPUT / FEATURE
	req.wLength = len;
	res = std_request((void*)buf);
	return res;
}

RES_CODE usb_remote_hid_t::get_idle(uint32_t rep_id)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = HIDRequest_GET_IDLE;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = (0 << 8) | rep_id;					//
	req.wLength = 1;
	res = std_request(&hid_idle);
	return res;
}

RES_CODE usb_remote_hid_t::set_idle(uint32_t duration, uint32_t rep_id)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_OUT_CLASS_INTERFACE;
	req.bRequest = HIDRequest_SET_IDLE;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = (duration << 8) | rep_id;			// duration in 4ms units
	req.wLength = 0;
	res = std_request(NULL);
	if(res == RES_OK)
		hid_idle = duration;
	return res;
}

RES_CODE usb_remote_hid_t::get_protocol(void* buf)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = HIDRequest_GET_PROTOCOL;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = 0;									//
	req.wLength = 1;
	res = std_request(buf);
	return res;
}

RES_CODE usb_remote_hid_t::set_protocol(uint32_t protocol)
{
	RES_CODE res;

	req.bmRequestType = USB_REQ_OUT_CLASS_INTERFACE;
	req.bRequest = HIDRequest_SET_PROTOCOL;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = protocol;							// 0 = Boot Protocol; 1 = Report Protocol
	req.wLength = 0;
	res = std_request(NULL);
	return res;
}

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------

RES_CODE usb_remote_hid_t::scan_hid(uint32_t port_indx, USBSubClassCode subcls, USBProtocolCode proto)
{
	RES_CODE res;

	// select EPT_0 and device address
	ep0_hnd->mode.as_bytes[0] = EPT_0; // RX pipe
	ep0_hnd->mode.as_bytes[1] = EPT_0; // TX pipe
	res = hdc_init(port_indx);
	if(res == RES_OK)
	{
		epi_hnd->mode0 = ep0_hnd->mode0;  // device hub port
		// TRACE Device descriptor
		TRACELN("HID: dev found %x:%x", dev_descriptor.idVendor, dev_descriptor.idProduct);
		trace_usb_descriptor(&dev_descriptor.as_generic);
		if(dev_descriptor.bDeviceClass == INTERFACE_DEFINED_CLASS)
		{
			//loop the configurations
			for(uint32_t cfg_indx=0; cfg_indx<dev_descriptor.bNumConfigurations; cfg_indx++)
			{
				res = get_config_descriptor(cfg_indx);
				if(res != RES_OK)
					break;

				// TRACE Configuration descriptor(s)
				trace_usb_descriptor(&config_descriptor->as_generic);

				//loop the interfaces
				for(uint32_t iface_indx=0; iface_indx<config_descriptor->bNumInterfaces; iface_indx++)
				{
					pid = usb_get_interface(config_descriptor, iface_indx);
					if(pid && pid->bDeviceClass == HID_DEVICE_CLASS &&
							pid->bDeviceSubClass == subcls && pid->bDeviceProtocol == proto)
					{
						// set the configuration
						set_configuration(0);
						res = set_configuration(config_descriptor->bConfigurationValue);

						if(res == RES_OK)
						{
							epi_hnd->mode.as_bytes[0] = EPT_0;
							epi_hnd->mode.as_bytes[1] = EPT_0;
							for(int i=0; i<pid->bNumEndpoints && i<2; i++)
							{
								USBEndpointDescriptor* ped;

								ped = usb_get_enpoint(config_descriptor, i);
								if(ped && ped->bmAttributes == ENDPOINT_TYPE_INTERRUPT)
								{
									if( ped->bEndpointAddress & 0x80 )
									{
										if(epi_hnd->mode.as_bytes[0] == EPT_0)
										{
											epi_hnd->mode.as_bytes[0] = ped->bEndpointAddress & 0x7F;
											epi_hnd->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
											usb_svc_configendpoint(epi_hnd, &ped->as_generic);
										}
									} else
									{
										if(epi_hnd->mode.as_bytes[1] == EPT_0)
										{
											epi_hnd->mode.as_bytes[1] = ped->bEndpointAddress ;
											epi_hnd->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
											usb_svc_configendpoint(epi_hnd, &ped->as_generic);
										}
									}
								}
							}
							hid_idle = 255;
							get_idle(HID_REPORT_ALL);
							return RES_OK;

						}
					}
				}
			}
		}
	}

	if(res == RES_OK)
		res = RES_ERROR;
	return res;
}


//------------------------------------------------------------------------------
//	usb_remote_kbd_t
//------------------------------------------------------------------------------

#define K00		255		// Error
#define K01		255		// Error
#define K02		255		// Error
#define K03		255		// Error
#define K2A		127		// DEL
#define K39 	255		// CAPSLOCK
#define K3A 	255		// F1
#define K3B 	255		// F2
#define K3C 	255		// F3
#define K3D 	255		// F4
#define K3E 	255		// F5
#define K3F 	255		// F6
#define K40 	255		// F7
#define K41 	255		// F8
#define K42 	255		// F9
#define K43 	255		// F10
#define K44 	255		// F11
#define K45 	255		// F12
#define K46 	255		// PrintScreen
#define K47 	255		// SCROLLLOCK
#define K48 	255		// PAUSE
#define K49 	255		// INSERT
#define K4A 	255		// HOME
#define K4B 	255		// PAGEUP
#define K4C 	255		// DELFWD
#define K4D 	255		// END
#define K4E 	255			// PAGEDOWN
#define K4F 	KEY_RIGHT	// RIGHT
#define K50 	KEY_LEFT    // LEFT
#define K51 	255		    // ???
#define K52 	KEY_UP  	//
#define K53 	255			// Num Lock
#define K54 	'/'			// Keypad /
#define K55 	'*'			// Keypad *
#define K56 	'-'			// Keypad -
#define K57 	'+'			// Keypad +
#define K58 	KEY_ENTER	// Keypad ENTER
#define K59 	'1'			// Keypad 1
#define K5A 	'2'			// Keypad 2
#define K5B 	'3'			// Keypad 3
#define K5C 	'4'			// Keypad 4
#define K5D 	'5'			// Keypad 5
#define K5E 	'6'			// Keypad 6
#define K5F 	'7'			// Keypad 7
#define K60 	'8'			// Keypad 8
#define K61 	'9'			// Keypad 9
#define K62 	'0'			// Keypad 0
#define K63 	'.'			// Keypad .
#define K64 	'\\' 		// Non-US \ and |
#define K65 	255			// Application
#define K66 	255			// Power
#define K67 	'='			// Keypad =
#define K68 	255			// F13
#define K69 	255			// F14
#define K6A 	255			// F15
#define K6B 	255			// F16
#define K6C 	255			// F17
#define K6D 	255			// F18
#define K6E 	255			// F19
#define K6F 	255			// F20

const unsigned char usb_hid_keycode[] =
{
	// 0    1    2    3   4    5    6    7    8    9    a    b    c    d    e    f
	 K00, K01, K02, K03, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', //00
	 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', //10
	 '3', '4', '5', '6', '7', '8', '9', '0','\r','\e', K2A,'\t', ' ', '-', '=', '[', //20
	 ']','\\', '`', ';', '\'', '`', ',', '.', '/', K39, K3A, K3B, K3C, K3D, K3E, K3F, //30
	 K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B, K4C, K4D, K4E, K4F, //40
	 K50, K51, K52, K53, K54, K55, K56, K57, K58, K59, K5A, K5B, K5C, K5D, K5E, K5F, //50
	 K60, K61, K62, K63, K64, K65, K66, K67, K68, K69, K6A, K6B, K6C, K6D, K6E, K6F  //60
};

const unsigned char usb_hid_keycode_s[] =
{
	// 0    1    2    3   4    5    6    7    8    9    a    b    c    d    e    f
	 K00, K01, K02, K03, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', //00
	 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@', //10
	 '#', '$', '%', '^', '&', '*', '(', ')','\r','\e', K2A,'\t', ' ', '_', '+', '{', //20
	 '}', '|', '~', ':', '"', '~', '<', '>', '?', K39, K3A, K3B, K3C, K3D, K3E, K3F, //30
	 K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B, K4C, K4D, K4E, K4F, //40
	 K50, K51, K52, K53, K54, K55, K56, K57, K58, K59, K5A, K5B, K5C, K5D, K5E, K5F, //50
	 K60, K61, K62, K63, K64, K65, K66, K67, K68, K69, K6A, K6B, K6C, K6D, K6E, K6F  //60
};

WEAK_C void usb_kbd_post_key(unsigned int key, unsigned int flags)
{
	if(key <255)
	{
		if(flags == KEY_DOWN_CODE)
		{
			TRACE_COLOR(TC_TXT_BLUE);
		} else
			TRACE_COLOR(TC_TXT_CYAN);
		if(key <32 )
			TRACE("{%02x}", key);
		else
			TRACE_CHAR(key);
		TRACE_COLOR_END();
	}
}

WEAK_C unsigned int usb_scan_to_ascii(uint32_t code, uint32_t modifier, uint32_t lang)
{
	TRACELN("key %x:%x ", modifier, code);

	if(modifier & HID_KBD_MODIFIER_RIGHT_SHIFT)
	{
		// process CAPSLOCK
		if(code >= 0x04 && code <= 0x1D)
		{
			modifier ^= HID_KBD_MODIFIER_LEFT_SHIFT;
		}
		modifier ^= HID_KBD_MODIFIER_RIGHT_SHIFT;
	}
	//process SHIFTs only
	switch(modifier)
	{
	case HID_KBD_MODIFIER_NONE:
		if(code < sizeof(usb_hid_keycode))
		{
			code = usb_hid_keycode[code];
		} else
			code = 255;
		break;

	case HID_KBD_MODIFIER_LEFT_SHIFT:
		if(code < sizeof(usb_hid_keycode_s))
		{
			code = usb_hid_keycode_s[code];
		} else
			code = 255;
		break;

	default:
		TRACELN("unk key %x:%x", modifier, code);
		code = 255;
		break;
	}
	return code;
}

uint32_t usb_remote_kbd_t::get_modifier(uint32_t modifier)
{
	// ignore left/right
	modifier = (modifier&0xF) | (modifier>>4);

	//apply capslock
	if(key_leds & HID_KBD_LED_CAPS_LOCK)
		modifier |= HID_KBD_MODIFIER_RIGHT_SHIFT;
	return modifier;
}

void usb_remote_kbd_t::key_press(uint32_t code)
{

	if(code == HID_KBD_SCAN_CAPSLOCK && new_keys.key_modifier==0)
	{
		key_leds2 = key_leds;
		if(!(key_leds& HID_KBD_LED_CAPS_LOCK))
		{
			key_leds |= HID_KBD_LED_CAPS_LOCK;
			set_report(&key_leds, 1, HID_REPORTTYPE_OUTPUT, 0);
		}
	}

	old_keys.key_code[old_keys.len] = code;
	key_tout[old_keys.len++] = 0;
	code = usb_scan_to_ascii(code, get_modifier(new_keys.key_modifier), key_lang);
	usb_kbd_post_key(code, KEY_DOWN_CODE);

}

void usb_remote_kbd_t::key_release(uint32_t code)
{

	if(code == HID_KBD_SCAN_CAPSLOCK && old_keys.key_modifier==0)
	{
		if(key_leds2 & HID_KBD_LED_CAPS_LOCK)
		{
			key_leds &= ~HID_KBD_LED_CAPS_LOCK;
			set_report(&key_leds, 1, HID_REPORTTYPE_OUTPUT, 0);
		}
	}

	code = usb_scan_to_ascii(code, get_modifier(old_keys.key_modifier), key_lang);
	usb_kbd_post_key(code, KEY_UP_CODE);
}

void usb_remote_kbd_t::kbd_process()
{
	int i, j, k, code;

	// find new len
	for(i=5; i>=0; i--)
		if(new_keys.key_code[i]> HID_KBD_SCAN_ERRUNDEF)
			break;
	new_keys.len = i+1;

	if(!new_keys.len || old_keys.key_modifier != new_keys.key_modifier)
	{
		// release all old
		while(old_keys.len)
		{
			key_release(old_keys.key_code[--old_keys.len]);
		}
		old_keys.key_modifier = new_keys.key_modifier;
	}

	if(new_keys.len)
	{
		// release old
		j=0;
		for(i=0; i<old_keys.len; i++)
		{
			code = old_keys.key_code[i];
			for(k=0; k<new_keys.len; k++)
			{
				if(code == new_keys.key_code[k])
					break;
			}
			if(k<new_keys.len)
			{
				//keep
				old_keys.key_code[j] = code;
				key_tout[j++] = key_tout[i];
				new_keys.key_code[k] = HID_KBD_SCAN_NONE;
			} else
			{
				//release old
				key_release(code);
			}
		}
		old_keys.len = j;

		//push new
		for(i=0; i<new_keys.len; i++)
		{
			code = new_keys.key_code[i];
			if(code > HID_KBD_SCAN_ERRUNDEF)
			{
				key_press(code);
			}
		}
	} else
	{
		// process ALT-SHIFT
		switch(new_keys.key_modifier)
		{
		case HID_KBD_MODIFIER_LEFT_SHIFT|HID_KBD_MODIFIER_RIGHT_SHIFT|HID_KBD_MODIFIER_LEFT_CTRL:
			if(def_lang == 1)
			{
				def_lang = 2;
				if(key_lang)
					key_lang = 2;
			}
			else
			{
				def_lang = 1;
				if(key_lang)
					key_lang = 1;
			}
			break;
		case HID_KBD_MODIFIER_LEFT_SHIFT|HID_KBD_MODIFIER_LEFT_ALT:
		case HID_KBD_MODIFIER_RIGHT_SHIFT|HID_KBD_MODIFIER_RIGHT_ALT:
			if(key_lang)
				key_lang = 0;
			else
				key_lang = def_lang;
			break;
		}

	}
}
