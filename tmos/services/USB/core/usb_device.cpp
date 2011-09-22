/*
 * usb_device.cpp
 *
 *  Created on: 23.06.2011
 *      Author: Miroslav Kostadinov
 *
 *	NOTE: This file is based on some Atmel sources, so the original copyright
 *	follows
 */

/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

#include <tmos.h>
#include <usb_opt.h>
#include <usb_drv.h>
#include <usb_device.h>
#include <usb_hal.h>
#include <usb_svc.h>
#include <usb_interface.h>

#include <cdc_data_inf.h>
#include <cdc_acm_inf.h>
/**
 *  Factory to create an usb interface(class) object
 */
WEAK_C usb_interface* InterfaceFactory(const USBInterfaceDescriptor* ds, int size)
{
	switch(ds->bDeviceClass)
	{
	case CDC_DEVICE_CLASS:
		switch(ds->bDeviceSubClass)
		{
		case CDC_ACM_SUBCLASS:
			return (new usb_cdc_acm_interface(ds->bAlternateSetting));
		default:;
		break;
		}
		break;

	case CDCDATA_DEVICE_CLASS:
		return (new usb_cdc_data_interface(ds->bAlternateSetting));

	default:;
	break;
	}

	return (NULL);
}

void usb_device::Initialize(const USBDDriverDescriptors *descriptors)
{
    cfgnum = 0;
#if (BOARD_USB_BMATTRIBUTES == USBConfigurationDescriptor_SELFPOWERED_RWAKEUP) \
    || (BOARD_USB_BMATTRIBUTES == USBConfigurationDescriptor_BUSPOWERED_RWAKEUP)
    isRemoteWakeUpEnabled = 1;
#else
    isRemoteWakeUpEnabled = 0;
#endif

    pDescriptors = descriptors;
//    pInterfaces = interfaces;

//    // Initialize interfaces array if not null
//    if (interfaces != 0)
//	{
//
//		memset(interfaces, sizeof(interfaces), 0);
//	}
}

/**
 * Sends the requested USB descriptor to the host if available, or STALLs  the
 * request.
 * \param pDriver  Pointer to a USBDDriver instance.
 * \param type  Type of the requested descriptor
 * \param index  Index of the requested descriptor.
 * \param length  Maximum number of bytes to return.
 */
void usb_device::GetDescriptor(const USBGenericRequest *pRequest, HANDLE hnd)
{
    uint32_t length;

    const USBDeviceDescriptor *pDevice;
    const USBConfigurationDescriptor *pConfiguration;
    const USBDeviceQualifierDescriptor *pQualifier;
    const USBConfigurationDescriptor *pOtherSpeed;
    const USBGenericDescriptor **pStrings =
        (const USBGenericDescriptor **) pDescriptors->pStrings;
    const USBGenericDescriptor *pString;
    uint8_t numStrings = pDescriptors->numStrings;
    uint8_t terminateWithNull = 0;

    /* Use different set of descriptors depending on device speed */

    if (usb_hal_IsHighSpeed())
    {
        TRACE1_USB(" HS");
        pDevice = pDescriptors->pHsDevice;
        pConfiguration = pDescriptors->pHsConfiguration;
        pQualifier = pDescriptors->pHsQualifier;
        pOtherSpeed = pDescriptors->pHsOtherSpeed;
    }
    else {

        TRACE1_USB(" FS");
        pDevice = pDescriptors->pFsDevice;
        pConfiguration = pDescriptors->pFsConfiguration;
        pQualifier = pDescriptors->pFsQualifier;
        pOtherSpeed = pDescriptors->pFsOtherSpeed;
    }

    /* Check the descriptor type */
    length = pRequest->GetLength();
    switch (pRequest->GetDescriptorType())
    {
        case DEVICE_DESCRIPTOR:
            TRACE1_USB(" Dev");

            /* Adjust length and send descriptor */

            if (length > pDevice->as_generic.GetLength() )
            {
                length = pDevice->as_generic.GetLength() ;
            }
            hnd->tsk_write(pDevice, length, USB_SETUP_WRITE_TOUT);
            break;

        case CONFIGURATION_DESCRIPTOR:
            TRACE1_USB(" Cfg");

            /* Adjust length and send descriptor */

            if (length > pConfiguration->wTotalLength)
            {
                length = pConfiguration->wTotalLength;
                terminateWithNull = ((length % pDevice->bMaxPacketSize0) == 0);
            }
            hnd->tsk_write(pConfiguration, length, USB_SETUP_WRITE_TOUT);
            if(terminateWithNull)
            	hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
            break;

        case DEVICEQUALIFIER_DESCRIPTOR:
            TRACE1_USB(" Qua");

            /* Check if descriptor exists */
            if (!pQualifier)
			{
				usb_svc_stall(hnd);
			}
			else
			{

				/* Adjust length and send descriptor */

				if (length > pQualifier->as_generic.GetLength())
				{
					length = pQualifier->as_generic.GetLength();
				}
				hnd->tsk_write(pQualifier, length, USB_SETUP_WRITE_TOUT);
			}
            break;

        case OTHERSPEEDCONFIGURATION_DESCRIPTOR:
            TRACE1_USB(" OSC");

            /* Check if descriptor exists */
            if (!pOtherSpeed)
            {
                usb_svc_stall(hnd);
            }
            else
			{
				/* Adjust length and send descriptor */

				if (length > pOtherSpeed->wTotalLength)
				{
					length = pOtherSpeed->wTotalLength;
					terminateWithNull = ((length % pDevice->bMaxPacketSize0) == 0);
				}
				hnd->tsk_write(pOtherSpeed, length, USB_SETUP_WRITE_TOUT);
				if (terminateWithNull)
					hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
			}
            break;

        case STRING_DESCRIPTOR:
            uint8_t indexRDesc;
            indexRDesc = pRequest->GetDescriptorIndex();
            TRACE_USB(" Str%d", indexRDesc);

            /* Check if descriptor exists */

            if (indexRDesc >= numStrings)
			{
				usb_svc_stall(hnd);
			}
			else
			{

				pString = pStrings[indexRDesc];

				/* Adjust length and send descriptor */

				if (length > pString->bLength)
				{
					length = pString->bLength;
					terminateWithNull = ((length % pDevice->bMaxPacketSize0) == 0);
				}
				hnd->tsk_write(pString, length, USB_SETUP_WRITE_TOUT);
				if (terminateWithNull)
					hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
			}
            break;

        default:
            TRACE_USB(" Unknown descriptor type (%d)", pRequest->GetDescriptorType());
            usb_svc_stall(hnd);
            break;
    }
}

/** Returns the configuration descriptor
 *  Multiple configurations are not supported here
 * @param cfg
 * @return
 */
const USBConfigurationDescriptor* usb_device::GetConfiguration(uint8_t cfg)
{
	const USBConfigurationDescriptor *pConfiguration;

	if (usb_hal_IsHighSpeed())
	{
		pConfiguration = pDescriptors->pHsConfiguration;
	}
	else
	{
		pConfiguration = pDescriptors->pFsConfiguration;
	}
	return (pConfiguration);
}

/**
 * Configures the device by setting it into the Configured state and
 * initializing all endpoints.
 * \param cfg  Configuration number to set.
 */
void usb_device::SetConfiguration(const void* drv, uint8_t cfg, HANDLE hnd)
{
	const USBConfigurationDescriptor *pConfiguration;
	const USBGenericDescriptor *descriptor;
	const USBInterfaceDescriptor* interface;
    USB_DRIVER_DATA* drv_data = ((USB_DRV_INFO )drv)->drv_data;
	int size;

	/* Use different descriptor depending on device speed */

	ShutdownInterfaces();
	for(size=1; size<USB_NUMENDPOINTS; size++)
	{
		drv_data->endpoints[size].rxfifo_size =0;
		drv_data->endpoints[size].txfifo_size =0;
	}
	pConfiguration = GetConfiguration(cfg);

	/* Set & save the desired configuration */
	usb_svc_setconfiguration(hnd, cfg);

	isRemoteWakeUpEnabled = ((pConfiguration->bmAttributes & 0x20) > 0);

	/* If the configuration is not 0, configure interfaces */
	if (cfg != 0)
	{
		// Start parsing descriptors
		size = pConfiguration->GetTotalLength();
		descriptor = &pConfiguration->as_generic;
		while ((descriptor = descriptor->FindNext(INTERFACE_DESCRIPTOR,
				CONFIGURATION_DESCRIPTOR, size)))
		{
			interface = (const USBInterfaceDescriptor*)descriptor;
			if(interface->bAlternateSetting == 0)
			{
				ConfigInterface(interface, size, hnd);
			}
		}
		usb_hal_config_fifo((USB_DRV_INFO)drv);
	}

	/* Acknowledge the request */
	hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
}

/**
 * Sends the current status of the device to the host.
 */
void usb_device::GetDeviceStatus(HANDLE hnd)
{
    static unsigned short data;
    const USBConfigurationDescriptor *pConfiguration;

    data = 0;
    /* Use different configuration depending on device speed */

    if (usb_hal_IsHighSpeed())
    {
        pConfiguration = pDescriptors->pHsConfiguration;
    }
    else
    {
        pConfiguration = pDescriptors->pFsConfiguration;
    }

    /* Check current configuration for power mode (if device is configured) */

    if (cfgnum != 0)
    {
        if ( pConfiguration->IsSelfPowered() )
        {
            data |= 1;
        }
    }

    /* Check if remote wake-up is enabled */

    if (isRemoteWakeUpEnabled)
    {
        data |= 2;
    }

    // Send the device status
    hnd->tsk_write(&data, 2, USB_SETUP_WRITE_TOUT);
}

/**
 * Sends the current status of an endpoints to the USB host.
 * \param bEndpoint  Endpoint number.
 */
void usb_device::GetEndpointStatus(const void* drv, uint8_t eptnum, HANDLE hnd)
{
	USB_DRV_INFO drv_info = (USB_DRV_INFO )drv;
    // Check if the endpoint exists
    if (eptnum > USB_NUMENDPOINTS)
    {
    	usb_svc_stall(hnd);
    }
    else
    {
        unsigned short data;

        // Check if the endpoint if currently halted
        if (drv_info->drv_data->endpoints[eptnum].state  == ENDPOINT_STATE_HALTED)
        {
            data = 1;
        } else
        {
        	data = 0;
        }

        // Send the endpoint status
        hnd->tsk_write(&data, 2, USB_SETUP_WRITE_TOUT);
    }
}


/**
 * Sets the active setting of the given interface if the configuration supports
 * it; otherwise, the control pipe is STALLed. If the setting of an interface
 * changes.
 * \parma infnum  Interface number.
 * \parma setting  New active setting for the interface.
 */
void usb_device::SetInterface(uint8_t infnum, uint8_t setting, HANDLE hnd)
{
	/* Search for the interface */
	if ( infnum < USB_MAX_INTERACES )
	{
		const USBConfigurationDescriptor *pConfiguration;
		const USBGenericDescriptor *descriptor;
		const USBInterfaceDescriptor* interface;
		int size;

		/* Use different descriptor depending on device speed */
		pConfiguration = GetConfiguration(cfgnum);


		// Start parsing descriptors
		size = pConfiguration->GetTotalLength();
		descriptor = &pConfiguration->as_generic;
		while ((descriptor = descriptor->FindNext(INTERFACE_DESCRIPTOR,
				CONFIGURATION_DESCRIPTOR, size)))
		{
			interface = (const USBInterfaceDescriptor*)descriptor;
			if ((interface->bInterfaceNumber == infnum)
					&& (interface->bAlternateSetting == setting))
			{
				if (!interfaces[infnum]
						|| (interfaces[infnum]->bAlternativeSetting != setting))
				{
					if (ConfigInterface(interface, size, hnd))
					{
						/* Acknowledge the request */
						hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
						return;
					}

				}
			}
		}
	}
	usb_svc_stall(hnd);
}

/**
 * Sends the currently active setting of the given interface to the USB
 * host. If alternate settings are not supported, this function STALLs the
 * control pipe.
 * \param infnum  Interface number.
 */
void usb_device::GetInterface(uint8_t infnum, HANDLE hnd)
{
	/* Make sure alternate settings are supported, or STALL the control pipe */

	if ( (infnum >= USB_MAX_INTERACES) || !interfaces[infnum])
	{
		usb_svc_stall(hnd);
	}
	else
	{
		/* Sends the current interface setting to the host */
		hnd->tsk_write(&interfaces[infnum]->bAlternativeSetting, 1, USB_SETUP_WRITE_TOUT);
	}
}

void usb_device::RequestHandler(const void* drv,
		const USBGenericRequest *pRequest, HANDLE hnd)
{
    uint8_t infnum;
    uint8_t eptnum;
    uint8_t setting;

    switch (pRequest->bmRequestType.recipient)
    {
    case USB_REQUEST_RECIPIENT_DEVICE:
        TRACE1_USB(" Std");

        /* Check request code */
        switch (pRequest->bRequest)
        {
            case USBGenericRequest_GETDESCRIPTOR:
            	TRACE1_USB(" gDesc");

                /* Send the requested descriptor */
                GetDescriptor(pRequest, hnd);
                break;

            case USBGenericRequest_SETADDRESS:
            {
                USB_DRIVER_DATA* drv_data = ((USB_DRV_INFO )drv)->drv_data;
    //			drv_data->dev_adr = pRequest->wValue & 0x7F;
    	        drv_data->usb_state = USB_STATE_DEFAULT;
            	TRACE_USB(" sAddr(%d)", pRequest->wValue & 0x7F);

                /* Sends a zero-length packet and then set the device address */
                if(hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT)==RES_OK)
                {
                	usb_svc_setaddress(hnd, pRequest->wValue & 0x7F);
                }
                break;
            }

            case USBGenericRequest_SETCONFIGURATION:
            	TRACE1_USB(" sCfg");

                /* Set the requested configuration */
                SetConfiguration(drv, pRequest->GetConfiguration(), hnd);
                break;

            case USBGenericRequest_GETCONFIGURATION:
            	TRACE1_USB(" gCfg");

                /* Send the current configuration number */

                //Function: GetConfiguration(pDriver);
                hnd->tsk_write( &cfgnum, 1, USB_SETUP_WRITE_TOUT);
                break;

            case USBGenericRequest_GETSTATUS:
            	TRACE1_USB(" gSta");

                /* Check who is the recipient */
                switch (pRequest->GetRecipient())
                {
                    case USB_REQUEST_RECIPIENT_DEVICE:
                        TRACE1_USB(" Dev");

                        /* Send the device status */
                        GetDeviceStatus(hnd);
                        break;

                    case USB_REQUEST_RECIPIENT_ENDPOINT:
                        TRACE1_USB(" Ept");

                        /* Send the endpoint status */
                        eptnum = pRequest->GetEndpointNumber();
                        GetEndpointStatus(drv, eptnum, hnd);
                        break;

                    default:
                        TRACE_USB(" Unknown recipient(%d)",
                        		pRequest->GetRecipient());
                        usb_svc_stall(hnd);
                        break;
                }
                break;

            case USBGenericRequest_CLEARFEATURE:
                TRACE1_USB(" cFeat");

                /* Check which is the requested feature */
                switch (pRequest->GetFeatureSelector())
                {
                    case USBFeatureRequest_ENDPOINTHALT:
                    	usb_svc_unhalt(hnd, pRequest->GetEndpointNumber(), pRequest->GetDirection());
                        hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                        break;

                    case USBFeatureRequest_DEVICEREMOTEWAKEUP:
                        TRACE1_USB(" RmWU");

                        /* Disable remote wake-up and send a zero-length packet */
                        isRemoteWakeUpEnabled = 0;
                        hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                        break;

                    default:
                    	TRACELN_USB(" Unknown feature selector(%d)",
                    			pRequest->GetFeatureSelector());
                        usb_svc_stall(hnd);
                        break;
                }
                break;

        case USBGenericRequest_SETFEATURE:
            TRACE1_USB(" sFeat");

            /* Check which is the selected feature */
            switch (pRequest->GetFeatureSelector())
            {
                case USBFeatureRequest_DEVICEREMOTEWAKEUP:
                    TRACE1_USB(" RmWU");

                    /* Enable remote wake-up and send a ZLP */
                    isRemoteWakeUpEnabled = 1;
                    hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                    break;

                case USBFeatureRequest_ENDPOINTHALT:
                	usb_svc_halt(hnd, pRequest->GetEndpointNumber(), pRequest->GetDirection());
                    hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                    break;

                case USBFeatureRequest_OTG_B_HNP_ENABLE:
                        TRACE1_USB(" HNP_ENABLE");
                        otg_features_supported |=
                            1<<USBFeatureRequest_OTG_B_HNP_ENABLE;
                        hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                    break;

                case USBFeatureRequest_OTG_A_HNP_SUPPORT:
                        TRACE1_USB(" HNP_SUPPORT");
                        otg_features_supported |=
                            1<<USBFeatureRequest_OTG_A_HNP_SUPPORT;
                        hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                    break;

                case USBFeatureRequest_OTG_A_ALT_HNP_SUPPORT:
                        TRACE1_USB(" ALT_HNP_SUPPORT");
                        otg_features_supported |=
                            1<<USBFeatureRequest_OTG_A_ALT_HNP_SUPPORT;
                        hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                    break;

                default:
                	TRACELN_USB(" Unknown feature selector(%d)",
                			pRequest->GetFeatureSelector());
                    usb_svc_stall(hnd);
                    break;
            }
            break;

        case USBGenericRequest_SETINTERFACE:
            TRACE1_USB(" sInterface");

            infnum = pRequest->GetInterface();
            setting = pRequest->GetAlternateSetting();
            SetInterface(infnum, setting, hnd);
            break;

        case USBGenericRequest_GETINTERFACE:
            TRACE1_USB(" gInterface");

            infnum = pRequest->GetInterface();
            GetInterface(infnum, hnd);
            break;

        default:
        	TRACE_USB(" Unknown request(%d)",
                      pRequest->GetRequest());
            usb_svc_stall(hnd);
            break;
        }
    	break;

    case USB_REQUEST_RECIPIENT_INTERFACE:
        infnum = pRequest->GetInterface();
    	if ( (infnum < USB_MAX_INTERACES) && interfaces[infnum])
    	{
    		// dispatch the request to the interface
    		interfaces[infnum]->RequestHandler(drv, pRequest, hnd);
    	}
    	else
    	{
    		usb_svc_stall(hnd);
        	TRACE_USB(" Invalid infnum(%d)", infnum);
    	}
    	break;

    default:
    	TRACE_USB(" Unknown recipient(%d)",
    			pRequest->bmRequestType.recipient);
        usb_svc_stall(hnd);
        break;

    }

}

void usb_device::ShutdownInterfaces()
{
	for(int infnum=0; infnum < USB_MAX_INTERACES; infnum++)
	{
		if(interfaces[infnum])
		{
			delete interfaces[infnum];
			interfaces[infnum] = NULL;
		}
	}
}

bool usb_device::ConfigInterface(const USBInterfaceDescriptor* ds, int size, HANDLE hnd)
{
	int infnum;
	const USBGenericDescriptor* endpoint;

	infnum = ds->bInterfaceNumber;
	TRACE_USB(" CfgInf(%d)", infnum);
	if(interfaces[infnum])
	{
		if(interfaces[infnum]->bAlternativeSetting == ds->bAlternateSetting)
			return (true);
		//shutdown here!?
		delete interfaces[infnum];
	}
	interfaces[infnum] = InterfaceFactory(ds, size);
	if(interfaces[infnum])
	{
		interfaces[infnum]->Initialize(ds, size);

		//Parse and configure endpoints
		endpoint = &ds->as_generic;
		while ((endpoint = endpoint->FindNext(ENDPOINT_DESCRIPTOR,
				INTERFACE_DESCRIPTOR, size)))
		{
			usb_svc_configendpoint(hnd, endpoint);
		}
		return (true);
	}
	return (false);
}
