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

/** \file
 * \addtogroup usbd_interface
 *@{
 */

/*------------------------------------------------------------------------------
 *      Headers
 *------------------------------------------------------------------------------*/
#include <tmos.h>
#include <usbd_drv.h>
#include <USBD_.h>
#include <USBDescriptors.h>
#include <drivers.h>



/**
 * Indicates that the current configuration of the device has changed.
 * \param cfgnum  New device configuration index.
 */
WEAK void USBDDriverCallbacks_ConfigurationChanged(uint8_t cfgnum)
{
	TRACE_USB("cfgChanged%d ", cfgnum);
}

/**
 * Notifies of a change in the currently active setting of an interface.
 * \param interface  Number of the interface whose setting has changed.
 * \param setting  New interface setting.
 */
WEAK void USBDDriverCallbacks_InterfaceSettingChanged(
    uint8_t interface,
    uint8_t setting)
{
	TRACE_USB("ifSettingChanged%d.%d ", interface, setting);
}



/**
 * Configures the device by setting it into the Configured state and
 * initializing all endpoints.
 * \param pDriver  Pointer to a USBDDriver instance.
 * \param cfgnum  Configuration number to set.
 */
static void SetConfiguration(USBDDriver *pDriver, uint8_t cfgnum, HANDLE hnd)
{
    USBEndpointDescriptor *pEndpoints[17];
    const USBConfigurationDescriptor *pConfiguration;

    /* Use different descriptor depending on device speed */

    if (USBD_IsHighSpeed()) {

        pConfiguration = pDriver->pDescriptors->pHsConfiguration;
    }
    else {

        pConfiguration = pDriver->pDescriptors->pFsConfiguration;
    }

    /* Set & save the desired configuration */
//    USBD_SetConfiguration(cfgnum);
    // Enter the Configured state
	hnd->dst.as_int = DCR_USB_SET_CONFIGURATION;
    hnd->src.as_int = cfgnum;
    hnd->hcontrol(DCR_HANDLE);

    pDriver->cfgnum = cfgnum;
    pDriver->isRemoteWakeUpEnabled =
        ((pConfiguration->bmAttributes & 0x20) > 0);

    /* If the configuration is not 0, configure endpoints */
    if (cfgnum != 0)
    {
        /* Parse configuration to get endpoint descriptors */
        USBConfigurationDescriptor_Parse(pConfiguration, 0, pEndpoints, 0);

        /* Configure endpoints */
        int i = 0;
        while (pEndpoints[i] != 0)
        {
    		hnd->dst.as_int = DCR_USB_CONFIGUREENDPOINT;
    	    hnd->src.as_voidptr = pEndpoints[i];
    	    hnd->hcontrol(DCR_HANDLE);
            i++;
        }
    }
    /* Should be done before send the ZLP */
    USBDDriverCallbacks_ConfigurationChanged(cfgnum);

    /* Acknowledge the request */
    hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
}


/**
 * Sends the requested USB descriptor to the host if available, or STALLs  the
 * request.
 * \param pDriver  Pointer to a USBDDriver instance.
 * \param type  Type of the requested descriptor
 * \param index  Index of the requested descriptor.
 * \param length  Maximum number of bytes to return.
 */
static void GetDescriptor(
    const USBDDriver *pDriver,
    const USBGenericRequest *pRequest,
    HANDLE hnd )
{
    uint8_t type;
    uint8_t indexRDesc;
    uint32_t length;

    const USBDeviceDescriptor *pDevice;
    const USBConfigurationDescriptor *pConfiguration;
    const USBDeviceQualifierDescriptor *pQualifier;
    const USBConfigurationDescriptor *pOtherSpeed;
    const USBGenericDescriptor **pStrings =
        (const USBGenericDescriptor **) pDriver->pDescriptors->pStrings;
    const USBGenericDescriptor *pString;
    uint8_t numStrings = pDriver->pDescriptors->numStrings;
    uint8_t terminateWithNull = 0;

    /* Use different set of descriptors depending on device speed */

    if (USBD_IsHighSpeed())
    {
        TRACE_DEBUG("HS ");
        pDevice = pDriver->pDescriptors->pHsDevice;
        pConfiguration = pDriver->pDescriptors->pHsConfiguration;
        pQualifier = pDriver->pDescriptors->pHsQualifier;
        pOtherSpeed = pDriver->pDescriptors->pHsOtherSpeed;
    }
    else {

        TRACE_DEBUG("FS ");
        pDevice = pDriver->pDescriptors->pFsDevice;
        pConfiguration = pDriver->pDescriptors->pFsConfiguration;
        pQualifier = pDriver->pDescriptors->pFsQualifier;
        pOtherSpeed = pDriver->pDescriptors->pFsOtherSpeed;
    }

    /* Check the descriptor type */
    type = USBGetDescriptorRequest_GetDescriptorType(pRequest);
    indexRDesc = USBGetDescriptorRequest_GetDescriptorIndex(pRequest);
    length = USBGenericRequest_GetLength(pRequest);
    switch (type)
    {
        case USBGenericDescriptor_DEVICE:
            TRACE1_USB_DEBUG("Dev ");

            /* Adjust length and send descriptor */

            if (length > pDevice->bLength )
            {
                length = pDevice->bLength ;
            }
            hnd->tsk_write(pDevice, length, USB_SETUP_WRITE_TOUT);
            break;

        case USBGenericDescriptor_CONFIGURATION:
            TRACE1_USB_DEBUG("Cfg ");

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

        case USBGenericDescriptor_DEVICEQUALIFIER:
            TRACE1_USB_DEBUG("Qua ");

            /* Check if descriptor exists */

            if (!pQualifier) {

                USBD_Stall(hnd);
            }
            else {

                /* Adjust length and send descriptor */

                if (length > pQualifier->bLength)
                {
                    length = pQualifier->bLength;
                }
                hnd->tsk_write(pQualifier, length, USB_SETUP_WRITE_TOUT);
            }
            break;

        case USBGenericDescriptor_OTHERSPEEDCONFIGURATION:
            TRACE1_USB_DEBUG("OSC ");

            /* Check if descriptor exists */

            if (!pOtherSpeed)
            {
                USBD_Stall(hnd);
            }
            else {

                /* Adjust length and send descriptor */

                if (length > pOtherSpeed->wTotalLength)
                {
                    length = pOtherSpeed->wTotalLength;
                    terminateWithNull = ((length % pDevice->bMaxPacketSize0) == 0);
                }
                hnd->tsk_write(pOtherSpeed, length, USB_SETUP_WRITE_TOUT);
                if(terminateWithNull)
                	hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
            }
            break;

        case USBGenericDescriptor_STRING:
            TRACE_USB_DEBUG("Str%d ", indexRDesc);

            /* Check if descriptor exists */

            if (indexRDesc >= numStrings) {

                USBD_Stall(hnd);
            }
            else {

                pString = pStrings[indexRDesc];

                /* Adjust length and send descriptor */

                if (length > pString->bLength)
                {
                    length = pString->bLength;
                    terminateWithNull = ((length % pDevice->bMaxPacketSize0) == 0);
                }
                hnd->tsk_write(pString, length, USB_SETUP_WRITE_TOUT);
                if(terminateWithNull)
                	hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
            }
            break;

        default:
            TRACE_WARNING(
                      "USBDDriver_GetDescriptor: Unknown descriptor type (%d)\n\r",
                      type);
            USBD_Stall(hnd);
    }
}

/**
 * Sends the current status of the device to the host.
 * \param pDriver  Pointer to a USBDDriver instance.
 */
static void GetDeviceStatus(const USBDDriver *pDriver, HANDLE hnd)
{
    static unsigned short data;
    const USBConfigurationDescriptor *pConfiguration;

    data = 0;
    /* Use different configuration depending on device speed */

    if (USBD_IsHighSpeed())
    {
        pConfiguration = pDriver->pDescriptors->pHsConfiguration;
    }
    else
    {
        pConfiguration = pDriver->pDescriptors->pFsConfiguration;
    }

    /* Check current configuration for power mode (if device is configured) */

    if (pDriver->cfgnum != 0)
    {
        if (USBConfigurationDescriptor_IsSelfPowered(pConfiguration))
        {
            data |= 1;
        }
    }

    /* Check if remote wake-up is enabled */

    if (pDriver->isRemoteWakeUpEnabled)
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
static void GetEndpointStatus(uint8_t bEndpoint, HANDLE hnd)
{
    // Check if the endpoint exists
    if (bEndpoint > CHIP_USB_NUMENDPOINTS)
    {
        USBD_Stall(hnd);
    }
    else
    {
        unsigned short data = 0;
        USBD_INFO drv_info;
        const char* ptr = DRV_TABLE[hnd->drv_index];

        ptr--;
        drv_info = (USBD_INFO)(void*)ptr;

        // Check if the endpoint if currently halted
        if (drv_info->drv_data->endpoints[bEndpoint].state  == UDP_ENDPOINT_HALTED)
        {
            data = 1;
        }

        // Send the endpoint status
        hnd->tsk_write(&data, 2, USB_SETUP_WRITE_TOUT);
    }
}

/**
 * Sets the active setting of the given interface if the configuration supports
 * it; otherwise, the control pipe is STALLed. If the setting of an interface
 * changes.
 * \parma pDriver  Pointer to a USBDDriver instance.
 * \parma infnum  Interface number.
 * \parma setting  New active setting for the interface.
 */
static void SetInterface(
    USBDDriver *pDriver,
    uint8_t infnum,
    uint8_t setting,
    HANDLE hnd)
{
    /* Make sure alternate settings are supported */
    if (!pDriver->pInterfaces)
    {
        USBD_Stall(hnd);
    }
    else
    {
        /* Change the current setting of the interface and trigger the callback */
        /* if necessary */
        if (pDriver->pInterfaces[infnum] != setting)
        {
            pDriver->pInterfaces[infnum] = setting;
            USBDDriverCallbacks_InterfaceSettingChanged(infnum, setting);
        }

        /* Acknowledge the request */

        hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
    }
}

/**
 * Sends the currently active setting of the given interface to the USB
 * host. If alternate settings are not supported, this function STALLs the
 * control pipe.
 * \param pDriver  Pointer to a USBDDriver instance.
 * \param infnum  Interface number.
 */
static void GetInterface(
    const USBDDriver *pDriver,
    uint8_t infnum,
    HANDLE hnd)
{
    /* Make sure alternate settings are supported, or STALL the control pipe */

    if (!pDriver->pInterfaces)
    {
        USBD_Stall(hnd);
    }
    else
    {
        /* Sends the current interface setting to the host */

    	hnd->tsk_write(&(pDriver->pInterfaces[infnum]), 1, USB_SETUP_WRITE_TOUT);
    }
}

/**
 * Handles the given request if it is standard, otherwise STALLs it.
 * \param pDriver  Pointer to a USBDDriver instance.
 * \param pRequest  Pointer to a USBGenericRequest instance.
 */
void USBDDriver_RequestHandler(
    USBDDriver *pDriver,
    const USBGenericRequest *pRequest,
    HANDLE hnd)
{
    uint8_t cfgnum;
    uint8_t infnum;
    uint8_t eptnum;
    uint8_t setting;

    TRACE1_USB_DEBUG("Std ");

    /* Check request code */
    switch (USBGenericRequest_GetRequest(pRequest))
    {
        case USBGenericRequest_GETDESCRIPTOR:
        	TRACE1_USB_DEBUG("gDesc ");

            /* Send the requested descriptor */
            GetDescriptor(pDriver, pRequest, hnd);
            break;

        case USBGenericRequest_SETADDRESS:
        	TRACE1_USB_DEBUG("sAddr ");

            /* Sends a zero-length packet and then set the device address */
            if(hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT)==RES_OK)
            {
            	hnd->dst.as_int = DCR_USB_SET_ADDRESS;
	            hnd->src.as_int = pRequest->wValue & 0x7F;
	            hnd->hcontrol(DCR_HANDLE);
            }
            break;

        case USBGenericRequest_SETCONFIGURATION:
        	TRACE1_USB_DEBUG("sCfg ");

            /* Set the requested configuration */
            cfgnum = USBSetConfigurationRequest_GetConfiguration(pRequest);
            SetConfiguration(pDriver, cfgnum, hnd);
            break;

        case USBGenericRequest_GETCONFIGURATION:
        	TRACE1_USB_DEBUG("gCfg ");

            /* Send the current configuration number */

            //Function: GetConfiguration(pDriver);
            hnd->tsk_write( &pDriver->cfgnum, 1, USB_SETUP_WRITE_TOUT);
            break;

        case USBGenericRequest_GETSTATUS:
        	TRACE1_USB_DEBUG("gSta ");

            /* Check who is the recipient */
            switch (USBGenericRequest_GetRecipient(pRequest)) {

                case USBGenericRequest_DEVICE:
                    TRACE1_USB_DEBUG("Dev ");

                    /* Send the device status */
                    GetDeviceStatus(pDriver, hnd);
                    break;

                case USBGenericRequest_ENDPOINT:
                    TRACE1_USB_DEBUG("Ept ");

                    /* Send the endpoint status */
                    eptnum = USBGenericRequest_GetEndpointNumber(pRequest);
                    GetEndpointStatus(eptnum, hnd);
                    break;

                default:
                    TRACE_WARNING(
                              "USBDDriver_RequestHandler: Unknown recipient (%d)\n\r",
                              USBGenericRequest_GetRecipient(pRequest));
                    USBD_Stall(hnd);
            }
            break;

        case USBGenericRequest_CLEARFEATURE:
            TRACE1_USB_DEBUG("cFeat ");

            /* Check which is the requested feature */
            switch (USBFeatureRequest_GetFeatureSelector(pRequest)) {

                case USBFeatureRequest_ENDPOINTHALT:
                    TRACE1_USB_DEBUG("Hlt ");

                    /* Unhalt endpoint and send a zero-length packet */
					hnd->dst.as_int = DCR_USB_UNHALT;
				    hnd->src.as_int = USBGenericRequest_GetEndpointNumber(pRequest);
				    hnd->hcontrol(DCR_HANDLE);
                    hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                    break;

                case USBFeatureRequest_DEVICEREMOTEWAKEUP:
                    TRACE1_USB_DEBUG("RmWU ");

                    /* Disable remote wake-up and send a zero-length packet */
                    pDriver->isRemoteWakeUpEnabled = 0;
                    hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                    break;

                default:
                    TRACE_WARNING(
                              "USBDDriver_RequestHandler: Unknown feature selector (%d)\n\r",
                              USBFeatureRequest_GetFeatureSelector(pRequest));
                    USBD_Stall(hnd);
            }
            break;

    case USBGenericRequest_SETFEATURE:
        TRACE1_USB_DEBUG("sFeat ");

        /* Check which is the selected feature */
        switch (USBFeatureRequest_GetFeatureSelector(pRequest)) {

            case USBFeatureRequest_DEVICEREMOTEWAKEUP:
                TRACE1_USB_DEBUG("RmWU ");

                /* Enable remote wake-up and send a ZLP */
                pDriver->isRemoteWakeUpEnabled = 1;
                hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                break;

            case USBFeatureRequest_ENDPOINTHALT:
                TRACE1_USB_DEBUG("Halt ");
                /* Halt endpoint */
				hnd->dst.as_int = DCR_USB_HALT;
			    hnd->src.as_int = USBGenericRequest_GetEndpointNumber(pRequest);
			    hnd->hcontrol(DCR_HANDLE);
                hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                break;

            case USBFeatureRequest_OTG_B_HNP_ENABLE:
                    TRACE1_USB_DEBUG("OTG_B_HNP_ENABLE ");
                    pDriver->otg_features_supported |=
                        1<<USBFeatureRequest_OTG_B_HNP_ENABLE;
                    hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                break;

            case USBFeatureRequest_OTG_A_HNP_SUPPORT:
                    TRACE1_USB_DEBUG("OTG_A_HNP_SUPPORT ");
                    pDriver->otg_features_supported |=
                        1<<USBFeatureRequest_OTG_A_HNP_SUPPORT;
                    hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                break;

            case USBFeatureRequest_OTG_A_ALT_HNP_SUPPORT:
                    TRACE1_USB_DEBUG("OTG_A_ALT_HNP_SUPPORT ");
                    pDriver->otg_features_supported |=
                        1<<USBFeatureRequest_OTG_A_ALT_HNP_SUPPORT;
                    hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT);
                break;

            default:
                TRACE_WARNING(
                          "USBDDriver_RequestHandler: Unknown feature selector (%d)\n\r",
                          USBFeatureRequest_GetFeatureSelector(pRequest));
                USBD_Stall(hnd);
        }
        break;

    case USBGenericRequest_SETINTERFACE:
        TRACE1_USB_DEBUG("sInterface ");

        infnum = USBInterfaceRequest_GetInterface(pRequest);
        setting = USBInterfaceRequest_GetAlternateSetting(pRequest);
        SetInterface(pDriver, infnum, setting, hnd);
        break;

    case USBGenericRequest_GETINTERFACE:
        TRACE1_USB_DEBUG("gInterface ");

        infnum = USBInterfaceRequest_GetInterface(pRequest);
        GetInterface(pDriver, infnum, hnd);
        break;

    default:
        TRACE_WARNING(
                  "USBDDriver_RequestHandler: Unknown request code (%d)\n\r",
                  USBGenericRequest_GetRequest(pRequest));
        USBD_Stall(hnd);
    }
}

/**
 * Initializes a USBDDriver instance with a list of descriptors. If
 * interfaces can have multiple alternate settings, an array to store the
 * current setting for each interface must be provided.
 * \param pDriver  Pointer to a USBDDriver instance.
 * \param pDescriptors  Pointer to a USBDDriverDescriptors instance.
 * \param pInterfaces  Pointer to an array for storing the current alternate
 *                     setting of each interface (optional).
 */
void USBDDriver_Initialize(
    USBDDriver *pDriver,
    const USBDDriverDescriptors *pDescriptors,
    uint8_t *pInterfaces)
{

    pDriver->cfgnum = 0;
	pDriver->isRemoteWakeUpEnabled = 0;

	pDriver->pDescriptors = pDescriptors;
	pDriver->pInterfaces = pInterfaces;

	/* Initialize interfaces array if not null */

	if (pInterfaces != 0)
	{
		memclr(pInterfaces, sizeof(pInterfaces));
	}
}
