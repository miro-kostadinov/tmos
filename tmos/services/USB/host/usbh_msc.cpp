/*
 * usbh_msc.cpp
 *
 *  Created on: Aug 8, 2016
 *      Author: miro
 */

#include <tmos.h>
#include <usbh_msc.h>
#include <usb_descriptors.h>
#include <usb_svc.h>
#include <scsi.h>
#include <usb_msc_reqs.h>

//------------------------------------------------------------------------------
//	Class Requests
//------------------------------------------------------------------------------
RES_CODE usb_remote_msc_t::get_max_lun()
{
	RES_CODE res;

	msc_max_lun = 0;
	req.bmRequestType = USB_REQ_IN_CLASS_INTERFACE;
	req.bRequest = MSCRequest_GET_MAX_LUN;
	req.wIndex = pid->bInterfaceNumber;				// Interface number
	req.wValue = 0;									//
	req.wLength = 1;
	res = std_request(&msc_max_lun);
	return res;
}


//------------------------------------------------------------------------------
//	Commands
//------------------------------------------------------------------------------
RES_CODE usb_remote_msc_t::msc_command(usbmsc_cs_t* transaction, void* buf, uint32_t len)
{
	RES_CODE res;

	transaction->cbw.dCBWSignature = USBMSC_CBW_SIGNATURE;
	transaction->cbw.dCBWTag = ++cb_tag;
	transaction->cbw.bCBWLUN = lun;
	transaction->cbw.dCBWDataTransferLength = len;
	res = msc_hnd->tsk_write(&transaction->cbw, USBMSC_CBW_SIZEOF, USBMSC_WRITE_TOUT);
	if(res == RES_OK)
	{
		// data transfer
		if(buf && len)
		{
			if(transaction->cbw.bmCBWFlags & USBMSC_CBW_FLAGS_IN)
			{
				// IN transaction
				if(transaction->cbw.dCBWDataTransferLength)
				{
					res = msc_hnd->tsk_read(buf, len, USBMSC_READ_TOUT);
				}
			} else
			{
				// OUT transaction
				res = msc_hnd->tsk_write(buf, len, USBMSC_WRITE_TOUT);
			}
		}
		if(res == RES_OK)
			res = msc_hnd->tsk_read(&transaction->csw, USBMSC_CSW_SIZEOF, USBMSC_READ_TOUT);
	}

	if(res != RES_OK)
		msc_hnd->tsk_read(&transaction->csw, USBMSC_CSW_SIZEOF, USBMSC_READ_TOUT);
	return res;
}

RES_CODE usb_remote_msc_t::cmd_test_unit_ready(usbmsc_cs_t* transaction)
{
	scsi_cmd_test_unit_ready_t* cdb = (scsi_cmd_test_unit_ready_t*)transaction->cbw.CBWCB;

	transaction->cbw.bmCBWFlags = USBMSC_CBW_FLAGS_OUT;

	transaction->cbw.bCBWCBLength = sizeof(*cdb);
	memclr(transaction->cbw.CBWCB, USBMSC_MAX_CDBLEN);
	cdb->opcode = SCSI_CMD_TEST_UNIT_READY;

	return msc_command(transaction, nullptr, SCSI_TRLEN_TEST_UNIT_READY);
}

RES_CODE usb_remote_msc_t::cmd_read_capcity(usbmsc_cs_t* transaction)
{
	scsi_cmd_read_capacity_t* cdb = (scsi_cmd_read_capacity_t*)transaction->cbw.CBWCB;
	scsi_data_read_capacity_t data;
	RES_CODE res;

	transaction->cbw.bmCBWFlags = USBMSC_CBW_FLAGS_IN;

	transaction->cbw.bCBWCBLength = sizeof(*cdb);
	memclr(transaction->cbw.CBWCB, USBMSC_MAX_CDBLEN);
	cdb->opcode = SCSI_CMD_READ_CAPACITY;

	data.block_len = 0;
	data.lba = 0;
	res = msc_command(transaction, &data, SCSI_TRLEN_READ_CAPACITY);
	blk_size = __REV(data.block_len);
	capacity = __REV(data.lba) + 1;
	return res;
}

RES_CODE usb_remote_msc_t::cmd_inquiry(usbmsc_cs_t* transaction)
{
	scsi_cmd_inquiry_t* cdb = (scsi_cmd_inquiry_t*)(transaction->cbw.CBWCB);
	scsi_data_inquiry_t data;
	RES_CODE res;

	transaction->cbw.bmCBWFlags = USBMSC_CBW_FLAGS_IN;

	transaction->cbw.bCBWCBLength = sizeof(*cdb);
	memclr(transaction->cbw.CBWCB, USBMSC_MAX_CDBLEN);
	cdb->opcode = SCSI_CMD_INQUIRY;
	cdb->allocation_length = __REV16(SCSI_TRLEN_INQUIRY);

	memclr(&data, sizeof(data));
	res = msc_command(transaction, &data, SCSI_TRLEN_INQUIRY);
	if(res == RES_OK)
	{
		TRACELN("MSC Found:\r\ntype=%x", data.qualifier_type);
		TRACELN("rmb=%x", data.rmb);
		TRACELN("version=%x", data.version);
		TRACELN("len=%x\r\nvid=", data.len);
		TRACE_BUF(data.vendorid, 8);
		TRACELN1("pid=");
		TRACE_BUF(data.productid, 16);
		TRACELN1("nrev=");
		TRACE_BUF(data.revision, 4);
		TRACELN1("");

	}
	return res;
}

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------

RES_CODE usb_remote_msc_t::init_msd()
{
	usbmsc_cs_t transaction;
	RES_CODE res;

	get_max_lun();

	// test init
	for(int i=0; i <100; i++)
	{
		tsk_sleep(50);

		res = cmd_test_unit_ready( &transaction);
		if(res == RES_OK && transaction.csw.bCSWStatus == USBMSC_CSW_STATUS_PASS)
		{
				break;
		}
	}

	// get capacity of volume
	if(res != RES_OK)
		res = RES_BUSY_WAITING;
	else
	{
		res = cmd_read_capcity(&transaction);
	}

	// Inquiry - information regarding the logical unit
	if(res == RES_OK)
	{
		res = cmd_inquiry(&transaction);
	}

	return res;
}

RES_CODE usb_remote_msc_t::msc_read(void* buf, uint32_t sector, uint32_t count)
{
	usbmsc_cs_t transaction;
	scsi_cmd_read_10_t* cdb = (scsi_cmd_read_10_t*)transaction.cbw.CBWCB;
	RES_CODE res;

	transaction.cbw.bmCBWFlags = USBMSC_CBW_FLAGS_IN;

	transaction.cbw.bCBWCBLength = sizeof(*cdb);
	memclr(transaction.cbw.CBWCB, USBMSC_MAX_CDBLEN);
	cdb->opcode = SCSI_CMD_READ_10;
	cdb->blocks = __REV16(count);
	cdb->lba = __REV(sector);

	res = msc_command(&transaction, buf, blk_size * count);
	return res;
}

RES_CODE usb_remote_msc_t::msc_write(const void* buf, uint32_t sector, uint32_t count)
{
	usbmsc_cs_t transaction;
	scsi_cmd_write_10_t* cdb = (scsi_cmd_write_10_t*)transaction.cbw.CBWCB;
	RES_CODE res;

	transaction.cbw.bmCBWFlags = USBMSC_CBW_FLAGS_OUT;

	transaction.cbw.bCBWCBLength = sizeof(*cdb);
	memclr(transaction.cbw.CBWCB, USBMSC_MAX_CDBLEN);
	cdb->opcode = SCSI_CMD_WRITE_10;
	cdb->blocks = __REV16(count);
	cdb->lba = __REV(sector);

	res = msc_command(&transaction, (void*)buf, blk_size * count);
	return res;

}

RES_CODE usb_remote_msc_t::scan_msc(uint32_t port_indx, USBSubClassCode subcls, USBProtocolCode proto)
{
	RES_CODE res;

	// select EPT_0 and device address
	ep0_hnd->mode.as_bytes[0] = EPT_0; // RX pipe
	ep0_hnd->mode.as_bytes[1] = EPT_0; // TX pipe
	res = hdc_init(port_indx);
	if(res == RES_OK)
	{
		msc_hnd->mode0 = ep0_hnd->mode0;  // device hub port
		// TRACE Device descriptor
		TRACELN("MSC: dev found %x:%x", dev_descriptor.idVendor, dev_descriptor.idProduct);
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
					if(pid && pid->bDeviceClass == MASSSTORAGE_DEVICE_CLASS &&
							pid->bDeviceSubClass == subcls && pid->bDeviceProtocol == proto)
					{
						// set the configuration
//						set_configuration(0);
						res = set_configuration(config_descriptor->bConfigurationValue);

						if(res == RES_OK)
						{
							msc_hnd->mode.as_bytes[0] = EPT_0;
							msc_hnd->mode.as_bytes[1] = EPT_0;
							for(int i=0; i<pid->bNumEndpoints && i<2; i++)
							{
								USBEndpointDescriptor* ped;

								ped = usb_get_enpoint(config_descriptor, i, pid->bInterfaceNumber);
								if(ped && ped->bmAttributes == ENDPOINT_TYPE_BULK)
								{
									if( ped->bEndpointAddress & 0x80 )
									{
										if(msc_hnd->mode.as_bytes[0] == EPT_0)
										{
											msc_hnd->mode.as_bytes[0] = ped->bEndpointAddress & 0x7F;
											msc_hnd->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
											usb_svc_configendpoint(msc_hnd, &ped->as_generic);
										}
									} else
									{
										if(msc_hnd->mode.as_bytes[1] == EPT_0)
										{
											msc_hnd->mode.as_bytes[1] = ped->bEndpointAddress ;
											msc_hnd->mode.as_ushort[1] = ep0_hnd->mode.as_ushort[1]; //drv_state_cnt
											usb_svc_configendpoint(msc_hnd, &ped->as_generic);
										}
									}
								}
							}
							return init_msd();
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





