/*
 * usbh_vid-cdc.cpp
 *
 *  Created on: Aug 22, 2018
 *      Author: miro
 */

#include <tmos.h>
#include <usbh_vid-cdc.h>
#include <usb_descriptors.h>
#include <usb_svc.h>



RES_CODE usb_vid_cdc_t::scan_vid(uint32_t port_indx, uint32_t vid, uint32_t pid)
{
	RES_CODE res;

	// select EPT_0 and device address
	ep0_hnd->mode.as_bytes[0] = EPT_0; // RX pipe
	ep0_hnd->mode.as_bytes[1] = EPT_0; // TX pipe
	res = hdc_init(port_indx);
	if(res == RES_OK)
	{
		cdc_hnd->mode0 = ep0_hnd->mode0;  // device hub port
		// TRACE Device descriptor
		TRACELN("CDC: dev found %x:%x", dev_descriptor.idVendor, dev_descriptor.idProduct);
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
					USBInterfaceDescriptor* id;
					id = usb_get_interface(config_descriptor, iface_indx);
					if(id && id->bDeviceClass == VENDOR_SPECIFIC_CLASS &&
							dev_descriptor.idVendor == vid && dev_descriptor.idProduct == pid)
					{
						// set the configuration
//						set_configuration(0);
						res = set_configuration(config_descriptor->bConfigurationValue);

						if(res == RES_OK)
						{
							cdc_hnd->mode.as_bytes[0] = EPT_0;
							cdc_hnd->mode.as_bytes[1] = EPT_0;
							for(int i=0; i<id->bNumEndpoints && i<2; i++)
							{
								USBEndpointDescriptor* ped;

								ped = usb_get_enpoint(config_descriptor, i, id->bInterfaceNumber);
								if(ped && ped->bmAttributes == ENDPOINT_TYPE_BULK)
								{
									if( ped->bEndpointAddress & 0x80 )
									{
										if(cdc_hnd->mode.as_bytes[0] == EPT_0)
										{
											cdc_hnd->mode.as_bytes[0] = ped->bEndpointAddress & 0x7F;
											cdc_hnd->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
											usb_svc_configendpoint(cdc_hnd, &ped->as_generic);
										}
									} else
									{
										if(cdc_hnd->mode.as_bytes[1] == EPT_0)
										{
											cdc_hnd->mode.as_bytes[1] = ped->bEndpointAddress ;
											cdc_hnd->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
											usb_svc_configendpoint(cdc_hnd, &ped->as_generic);
										}
									}
								}
							}
							if(cdc_hnd->mode.as_bytes[0] != EPT_0 && cdc_hnd->mode.as_bytes[1] != EPT_0)
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

