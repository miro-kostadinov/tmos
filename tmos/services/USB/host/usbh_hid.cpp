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

RES_CODE hid_parse_report(const unsigned char * report, int rep_size)
{
	bool desktop_foud = false;

	while (rep_size > 0)
	{
		uint8_t item = *report;
		uint32_t data = 0;

		report++;
		rep_size--;

	    switch (item & USBHID_RPTITEM_SIZE_MASK)
		{
		case USBHID_RPTITEM_SIZE_4: /* 4 bytes of little endian data follow */
			data = (uint32_t) (*report++);
			data |= (uint32_t) (*report++) << 8;
			data |= (uint32_t) (*report++) << 16;
			data |= (uint32_t) (*report++) << 24;
			rep_size -= 4;
			break;

		case USBHID_RPTITEM_SIZE_2: /* 2 bytes of little endian data follow */
			data = (uint32_t) (*report++);
			data |= (uint32_t) (*report++) << 8;
			rep_size -= 2;
			break;

		case USBHID_RPTITEM_SIZE_1: /* 1 byte of data follows */
			data = (uint32_t) (*report++);
			rep_size -= 1;
			break;

		case USBHID_RPTITEM_SIZE_0: /* No data follows */
		default:
			break;
		}

	    switch (item & ~USBHID_RPTITEM_SIZE_MASK)
		{

		case USBHID_GLOBAL_USAGEPAGE_PREFIX:
			if((data & 0xFFFF) == USBHID_USAGE_PAGE_GENERIC_DCTRL) // Generic desktop control
				desktop_foud = true;
			else
				desktop_foud = false;
			break;

		case USBHID_LOCAL_USAGE_PREFIX:
			if(desktop_foud)
			{
				if(data == USBHID_DCTRLUSE_KEYBOARD || data == USBHID_DCTRLUSE_KEYPAD)
					return RES_OK;
			}
			break;

/*
		case USBHID_GLOBAL_PUSH_PREFIX:
		case USBHID_GLOBAL_POP_PREFIX:
		case USBHID_GLOBAL_LOGICALMIN_PREFIX:
		case USBHID_GLOBAL_LOGICALMAX_PREFIX:
		case USBHID_GLOBAL_PHYSICALMIN_PREFIX:
		case USBHID_GLOBAL_PHYSMICALAX_PREFIX:
		case USBHID_GLOBAL_UNITEXP_PREFIX:
		case USBHID_GLOBAL_UNIT_PREFIX:
		case USBHID_GLOBAL_REPORTSIZE_PREFIX:
		case USBHID_GLOBAL_REPORTCOUNT_PREFIX:
		case USBHID_GLOBAL_REPORTID_PREFIX:
		case USBHID_LOCAL_USAGEMIN_PREFIX:
		case USBHID_LOCAL_USAGEMAX_PREFIX:
		case USBHID_MAIN_COLLECTION_PREFIX:
		case USBHID_MAIN_ENDCOLLECTION_PREFIX:
		case USBHID_MAIN_INPUT_PREFIX:
		case USBHID_MAIN_OUTPUT_PREFIX:
		case USBHID_MAIN_FEATURE_PREFIX:
*/
		default:
			desktop_foud = false;
			break;
		}

	}
	return RES_EOF;
}

/*
int hid_parsereport(const uint8_t *report, int rptlen,
                    hid_rptfilter_t filter, struct hid_rptinfo_s *rptinfo)
{
  struct hid_state_s           state[CONFIG_HID_STATEDEPTH];
  struct hid_state_s          *currstate = &state[0];
  struct hid_collectionpath_s *collectionpath = NULL;
  struct hid_rptsizeinfo_s    *rptidinfo = &rptinfo->rptsize[0];
  uint16_t                     usage[CONFIG_HID_USAGEDEPTH];
  uint8_t                      nusage = 0;
  struct hid_range_s           usage_range = { 0, 0 };
  int                          i;

  DEBUGASSERT(report && filter && rptinfo);

  memset(rptinfo, 0x00, sizeof(struct hid_rptinfo_s));
  memset(currstate, 0x00, sizeof(struct hid_state_s));
  memset(rptidinfo, 0x00, sizeof(struct hid_rptsizeinfo_s));

  rptinfo->nreports = 1;

  while (rptlen > 0)
    {
      uint8_t  item = *report;
      uint32_t data = 0;

      report++;
      rptlen--;

      switch (item & USBHID_RPTITEM_SIZE_MASK)
        {
        case USBHID_RPTITEM_SIZE_4:  4 bytes of little endian data follow
          data    = (uint32_t)(*report++);
          data   |= (uint32_t)(*report++) << 8;
          data   |= (uint32_t)(*report++) << 16;
          data   |= (uint32_t)(*report++) << 24;
          rptlen -= 4;
          break;

        case USBHID_RPTITEM_SIZE_2:  2 bytes of little endian data follow
          data    = (uint32_t)(*report++);
          data   |= (uint32_t)(*report++) << 8;
          rptlen -= 2;
          break;

        case USBHID_RPTITEM_SIZE_1:  1 byte of data follows
          data    = (uint32_t)(*report++);
          rptlen -= 1;
          break;

        case USBHID_RPTITEM_SIZE_0:  No data follows
        default:
          break;
        }

      switch (item & ~USBHID_RPTITEM_SIZE_MASK)
        {
        case USBHID_GLOBAL_PUSH_PREFIX:
          if (currstate == &state[CONFIG_HID_STATEDEPTH - 1])
            {
              return -E2BIG;
            }

          memcpy((currstate + 1),
                 currstate, sizeof(struct hid_rptitem_s));

          currstate++;
          break;

        case USBHID_GLOBAL_POP_PREFIX:
          if (currstate == &state[0])
            {
              return -EINVAL;  Pop without push?
            }

          currstate--;
          break;

        case USBHID_GLOBAL_USAGEPAGE_PREFIX:
          if ((item & USBHID_RPTITEM_SIZE_MASK) == USBHID_RPTITEM_SIZE_4)
            {
              currstate->attrib.usage.page = (data >> 16);
            }

          currstate->attrib.usage.page = data;
          break;

        case USBHID_GLOBAL_LOGICALMIN_PREFIX:
          currstate->attrib.logical.min = data;
          break;

        case USBHID_GLOBAL_LOGICALMAX_PREFIX:
          currstate->attrib.logical.max = data;
          break;

        case USBHID_GLOBAL_PHYSICALMIN_PREFIX:
          currstate->attrib.physical.min = data;
          break;

        case USBHID_GLOBAL_PHYSMICALAX_PREFIX:
          currstate->attrib.physical.max = data;
          break;

        case USBHID_GLOBAL_UNITEXP_PREFIX:
          currstate->attrib.unit.exponent = data;
          break;

        case USBHID_GLOBAL_UNIT_PREFIX:
          currstate->attrib.unit.type = data;
          break;

        case USBHID_GLOBAL_REPORTSIZE_PREFIX:
          currstate->attrib.bitsize = data;
          break;

        case USBHID_GLOBAL_REPORTCOUNT_PREFIX:
          currstate->rptcount = data;
          break;

        case USBHID_GLOBAL_REPORTID_PREFIX:
          currstate->id = data;

          if (rptinfo->haverptid)
            {
              rptidinfo = NULL;

              for (i = 0; i < rptinfo->nreports; i++)
                {
                  if (rptinfo->rptsize[i].id == currstate->id)
                    {
                      rptidinfo = &rptinfo->rptsize[i];
                      break;
                    }
                }

              if (rptidinfo == NULL)
                {
                  if (rptinfo->nreports == CONFIG_HID_MAXIDS)
                    {
                      return -EINVAL;
                    }

                  rptidinfo = &rptinfo->rptsize[rptinfo->nreports++];
                  memset(rptidinfo, 0x00, sizeof(struct hid_rptsizeinfo_s));
                }
            }

          rptinfo->haverptid = true;

          rptidinfo->id = currstate->id;
          break;

        case USBHID_LOCAL_USAGE_PREFIX:
          if (nusage == CONFIG_HID_USAGEDEPTH)
            {
              return -E2BIG;
            }

          usage[nusage++] = data;
          break;

        case USBHID_LOCAL_USAGEMIN_PREFIX:
          usage_range.min = data;
          break;

        case USBHID_LOCAL_USAGEMAX_PREFIX:
          usage_range.max = data;
          break;

        case USBHID_MAIN_COLLECTION_PREFIX:
          if (collectionpath == NULL)
            {
              collectionpath = &rptinfo->collectionpaths[0];
            }
          else
            {
              struct hid_collectionpath_s *ParentCollectionPath = collectionpath;

              collectionpath = &rptinfo->collectionpaths[1];

              while (collectionpath->parent != NULL)
                {
                  if (collectionpath == &rptinfo->collectionpaths[CONFIG_HID_MAXCOLLECTIONS - 1])
                    {
                      return -EINVAL;
                    }

                  collectionpath++;
                }

              collectionpath->parent = ParentCollectionPath;
            }

          collectionpath->type       = data;
          collectionpath->usage.page = currstate->attrib.usage.page;

          if (nusage)
            {
              collectionpath->usage.usage = usage[0];

              for (i = 0; i < nusage; i++)
                usage[i] = usage[i + 1];

              nusage--;
            }
          else if (usage_range.min <= usage_range.max)
            {
              collectionpath->usage.usage = usage_range.min++;
            }

          break;

        case USBHID_MAIN_ENDCOLLECTION_PREFIX:
          if (collectionpath == NULL)
            {
              return -EINVAL;
            }

          collectionpath = collectionpath->parent;
          break;

        case USBHID_MAIN_INPUT_PREFIX:
        case USBHID_MAIN_OUTPUT_PREFIX:
        case USBHID_MAIN_FEATURE_PREFIX:
          {
            int itemno;
            for (itemno = 0; itemno < currstate->rptcount; itemno++)
              {
                struct hid_rptitem_s newitem;
                uint8_t tag;

                memcpy(&newitem.attrib, &currstate->attrib,
                       sizeof(struct hid_rptitem_attributes_s));

                newitem.flags          = data;
                newitem.collectionpath = collectionpath;
                newitem.id             = currstate->id;

                if (nusage)
                  {
                    newitem.attrib.usage.usage = usage[0];

                    for (i = 0; i < nusage; i++)
                      {
                        usage[i] = usage[i + 1];
                      }
                    nusage--;
                  }
                else if (usage_range.min <= usage_range.max)
                  {
                    newitem.attrib.usage.usage = usage_range.min++;
                  }

                tag = (item & ~USBHID_RPTITEM_SIZE_MASK);
                if (tag == USBHID_MAIN_INPUT_PREFIX)
                  {
                    newitem.type = HID_REPORT_ITEM_IN;
                  }
                else if (tag == USBHID_MAIN_OUTPUT_PREFIX)
                  {
                    newitem.type = HID_REPORT_ITEM_OUT;
                  }
                else
                  {
                    newitem.type = HID_REPORT_ITEM_FEATURE;
                  }

                newitem.bitoffset              = rptidinfo->size[newitem.type];
                rptidinfo->size[newitem.type] += currstate->attrib.bitsize;

                 Accumulate the maximum report size

                if (rptinfo->maxrptsize < newitem.bitoffset)
                  {
                    rptinfo->maxrptsize = newitem.bitoffset;
                  }

                if ((data & USBHID_MAIN_CONSTANT) == 0 && filter(&newitem))
                  {
                    if (rptinfo->nitems == CONFIG_HID_MAXITEMS)
                      {
                        return -EINVAL;
                      }

                    memcpy(&rptinfo->items[rptinfo->nitems],
                           &newitem, sizeof(struct hid_rptitem_s));

                    rptinfo->nitems++;
                  }
              }
          }
          break;
        }

      if ((item & USBHID_RPTITEM_TYPE_MASK) == USBHID_RPTITEM_TYPE_MAIN)
        {
          usage_range.min = 0;
          usage_range.max = 0;
          nusage = 0;
        }
    }

  if (!(rptinfo->nitems))
    {
      return -ENOENT;
    }

  return OK;
}
*/

RES_CODE usb_remote_hid_t::config_interface_enpoints()
{
	HANDLE interface_ep = epi_hnd[pid->bInterfaceNumber];
	if(interface_ep)
	{
		for(int i=0; i<pid->bNumEndpoints && i<2; i++)
		{
			USBEndpointDescriptor* ped;

			ped = usb_get_enpoint(config_descriptor, i, pid->bInterfaceNumber);
			if(ped && ped->bmAttributes == ENDPOINT_TYPE_INTERRUPT)
			{
				if( ped->bEndpointAddress & 0x80 )
				{
					if(interface_ep->mode.as_bytes[0] == EPT_0)
					{
						interface_ep->mode.as_bytes[0] = ped->bEndpointAddress & 0x7F;
						interface_ep->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
						usb_svc_configendpoint(interface_ep, &ped->as_generic);
					}
				} else
				{
					if(interface_ep->mode.as_bytes[1] == EPT_0)
					{
						interface_ep->mode.as_bytes[1] = ped->bEndpointAddress ;
						interface_ep->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
						usb_svc_configendpoint(interface_ep, &ped->as_generic);
					}
				}
			}
		}
//		set_idle(0, HID_REPORT_ALL);
	}
	return RES_OK;
}

RES_CODE usb_remote_hid_t::is_keypad_report(const USBHIDDescriptor *hid, int i)
{
	RES_CODE res;

	unsigned char *report;
	uint32_t rep_size = hid->hid_descriptors[i].wDescriptorLength;
	report = (unsigned char*)tsk_malloc(rep_size);
	if(report)
	{
		res = get_hid_report_descriptor(report, rep_size);
		if(res == RES_OK)
		{
			res =hid_parse_report(report, rep_size);
		}
		tsk_free(report);
	}
	else
		res = RES_OUT_OF_MEMORY;
	return res;
}

RES_CODE usb_remote_hid_t::scan_hid(uint32_t port_indx, USBSubClassCode subcls, USBProtocolCode proto)
{
	RES_CODE res;

	// select EPT_0 and device address
	ep0_hnd->mode.as_bytes[0] = EPT_0; // RX pipe
	ep0_hnd->mode.as_bytes[1] = EPT_0; // TX pipe
	epi_hnd[0]->mode.as_bytes[0] = EPT_0;
	epi_hnd[0]->mode.as_bytes[1] = EPT_0;
	epi_hnd[1]->mode.as_bytes[0] = EPT_0;
	epi_hnd[1]->mode.as_bytes[1] = EPT_0;
	res = hdc_init(port_indx);
	if(res == RES_OK)
	{
		epi_hnd[0]->mode0 = ep0_hnd->mode0;  // device hub port
		epi_hnd[1]->mode0 = ep0_hnd->mode0;  // device hub port
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
				for(uint32_t iface_indx=0; iface_indx<config_descriptor->bNumInterfaces && iface_indx < 2; iface_indx++)
				{
					pid = usb_get_interface(config_descriptor, iface_indx);
					if(pid  && pid->bDeviceClass == HID_DEVICE_CLASS )
					{
						const USBHIDDescriptor *hid;
						hid = usb_get_hid_descriptor(config_descriptor, iface_indx);
						if(hid)
						{
							config_interface_enpoints();
							for(int i=0; i < hid->bNumDescriptors; i++)
							{
								if(hid->hid_descriptors[i].bDescriptorType == HIDREPORT_DESCRIPTOR)
								{
									if(RES_OK ==is_keypad_report(hid, i) ||
											(hid->bNumDescriptors == 1 && config_descriptor->bNumInterfaces == 1)) // scanner DATALOGIC don't return report descriptor
									{
										// set the configuration
										res = set_configuration(config_descriptor->bConfigurationValue);
										if(res == RES_OK)
										{
											set_idle(0, HID_REPORT_ALL);
											return res;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return RES_ERROR;
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
#define K4F 	255			// RIGHT
#define K50 	255    		// LEFT
#define K51 	255		    // ???
#define K52 	255  		//
#define K53 	255			// Num Lock
#define K54 	'/'			// Keypad /
#define K55 	'*'			// Keypad *
#define K56 	'-'			// Keypad -
#define K57 	'+'			// Keypad +
#define K58 	255			// Keypad ENTER
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

#ifndef KEY_REPEAT_TIME
	#define KEY_REPEAT_TIME 	300
#endif

void usb_remote_kbd_t::kbd_timeout()
{
	uint32_t time, code;
	int32_t	tout;

	time = CURRENT_TIME;

	if(time != key_time)
	{
		tout = time - key_time;
		if(tout < 0)
			tout += 0x80000000;


		for(int i=0; i<old_keys.len; i++)
		{
			key_tout[i] += tout;
			if(key_tout[i] > KEY_REPEAT_TIME)
			{
				key_tout[i] -= KEY_REPEAT_TIME;
				code = old_keys.key_code[i];
				code = usb_scan_to_ascii(code, get_modifier(old_keys.key_modifier), key_lang);
				usb_kbd_post_key(code, KEY_DOWN_CODE);
			}
		}
		key_time = time;
	}

}
