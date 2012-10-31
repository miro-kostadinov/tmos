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

/**\file
 *  Implementation of the CDCDSerialPort class methods.
 */

/** \addtogroup usbd_cdc
 *@{
 */

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/
#include <tmos.h>
#include <cdc_serial_port.h>

/*------------------------------------------------------------------------------
 *         Exported functions
 *------------------------------------------------------------------------------*/

/**
 * Initializes the USB Device CDC serial port function.
 * \param pCdcd Pointer to CDCDSerialPort instance.
 * \param pUsbd Pointer to USBDDriver instance.
 * \param fEventHandler Pointer to event handler function.
 * \param pArg
 * \param firstInterface First interface index for the function
 *                       (0xFF to parse from descriptors).
 * \param numInterface   Number of interfaces for the function.
 */
void CDCDSerialPort_Initialize(CDCDSerialPort * pCdcd,
                               USBDDriver * pUsbd,
                               CDCDSerialPortEventHandler fEventHandler,
                               void * pArg,
                               uint8_t firstInterface,uint8_t numInterface)
{
    TRACE_INFO("CDCDSerialPort_Initialize\n\r");

    /* Initialize event handler */
    pCdcd->fEventHandler = fEventHandler;
    pCdcd->pArg = pArg;

    /* Initialize USB Device Driver interface */
    pCdcd->pUsbd = pUsbd;
    pCdcd->bInterfaceNdx = firstInterface;
    pCdcd->bNumInterface = numInterface;
    pCdcd->bIntInPIPE   = 0;
    pCdcd->bBulkInPIPE  = 0;
    pCdcd->bBulkOutPIPE = 0;

    /* Initialize Abstract Control Model attributes */
    pCdcd->bControlLineState = 0;
    pCdcd->wSerialState      = 0;



    CDCLineCoding *lineCoding = &pCdcd->lineCoding;
    lineCoding->dwDTERate = 115200;
    lineCoding->bCharFormat = CDCLineCoding_ONESTOPBIT;
    lineCoding->bParityType = CDCLineCoding_NOPARITY;
    lineCoding->bDataBits = 8;

}

///**
// * Handles CDC-specific SETUP requests. Should be called from a
// * re-implementation of USBDCallbacks_RequestReceived() method.
// * \param pCdcd Pointer to CDCDSerialPort instance.
// * \param request Pointer to a USBGenericRequest instance.
// * \return USBRC_SUCCESS if request handled, otherwise error.
// */
//uint32_t CDCDSerialPort_RequestHandler(
//    CDCDSerialPort *pCdcd,
//    const USBGenericRequest *request)
//{
//    if (USBGenericRequest_GetType(request) != USBGenericRequest_CLASS)
//        return USBRC_PARAM_ERR;
//
//    TRACE_INFO_WP("Cdcs ");
//
//    /* Validate interface */
//    if (request->wIndex >= pCdcd->bInterfaceNdx &&
//        request->wIndex < pCdcd->bInterfaceNdx + pCdcd->bNumInterface) {
//    }
//    else {
//        return USBRC_PARAM_ERR;
//    }
//
//    /* Handle the request */
//    switch (USBGenericRequest_GetRequest(request)) {
//
//        case CDCGenericRequest_SETLINECODING:
//
//            _SetLineCoding(pCdcd);
//            break;
//
//        case CDCGenericRequest_GETLINECODING:
//
//            _GetLineCoding(pCdcd);
//            break;
//
//        case CDCGenericRequest_SETCONTROLLINESTATE:
//
//            _SetControlLineState(pCdcd, request);
//            break;
//
//        default:
//
//            return USBRC_PARAM_ERR;
//    }
//
//    return USBRC_SUCCESS;
//}
//
///**
// * Receives data from the host through the virtual COM port created by
// * the CDC device serial driver. This function behaves like USBD_Read.
// * \param pCdcd  Pointer to CDCDSerialPort instance.
// * \param pData  Pointer to the data buffer to put received data.
// * \param dwSize Size of the data buffer in bytes.
// * \param fCallback Optional callback function to invoke when the transfer
// *                  finishes.
// * \param pArg      Optional argument to the callback function.
// * \return USBD_STATUS_SUCCESS if the read operation has been started normally;
// *         otherwise, the corresponding error code.
// */
//uint32_t CDCDSerialPort_Read(const CDCDSerialPort * pCdcd,
//                          void * pData,uint32_t dwSize,
//                          TransferCallback fCallback,void * pArg)
//{
//    if (pCdcd->bBulkOutPIPE == 0)
//        return USBRC_PARAM_ERR;
//
//    return USBD_Read(pCdcd->bBulkOutPIPE,
//                     pData, dwSize,
//                     fCallback, pArg);
//}
//
///**
// * Sends a data buffer through the virtual COM port created by the CDC
// * device serial driver. This function behaves exactly like USBD_Write.
// * \param pCdcd  Pointer to CDCDSerialPort instance.
// * \param pData  Pointer to the data buffer to send.
// * \param dwSize Size of the data buffer in bytes.
// * \param fCallback Optional callback function to invoke when the transfer
// *                  finishes.
// * \param pArg      Optional argument to the callback function.
// * \return USBD_STATUS_SUCCESS if the read operation has been started normally;
// *         otherwise, the corresponding error code.
// */
//uint32_t CDCDSerialPort_Write(const CDCDSerialPort * pCdcd,
//                                   void * pData, uint32_t dwSize,
//                                   TransferCallback fCallback, void * pArg)
//{
//    if (pCdcd->bBulkInPIPE == 0)
//        return USBRC_PARAM_ERR;
//
//    return USBD_Write(pCdcd->bBulkInPIPE,
//                      pData, dwSize,
//                      fCallback, pArg);
//}
//
///**
// * Returns the current control line state of the RS-232 line.
// * \param pCdcd  Pointer to CDCDSerialPort instance.
// */
//uint8_t CDCDSerialPort_GetControlLineState(const CDCDSerialPort * pCdcd)
//{
//    return pCdcd->bControlLineState;
//}
//
///**
// * Copy current line coding settings to pointered space.
// * \param pCdcd  Pointer to CDCDSerialPort instance.
// * \param pLineCoding Pointer to CDCLineCoding instance.
// */
//void CDCDSerialPort_GetLineCoding(const CDCDSerialPort * pCdcd,
//                                  CDCLineCoding* pLineCoding)
//{
//    if (pLineCoding) {
//        pLineCoding->dwDTERate   = pCdcd->lineCoding.dwDTERate;
//        pLineCoding->bCharFormat = pCdcd->lineCoding.bCharFormat;
//        pLineCoding->bParityType = pCdcd->lineCoding.bParityType;
//        pLineCoding->bDataBits   = pCdcd->lineCoding.bDataBits;
//    }
//}
//
///**
// * Returns the current status of the RS-232 line.
// * \param pCdcd  Pointer to CDCDSerialPort instance.
// */
//uint16_t CDCDSerialPort_GetSerialState(const CDCDSerialPort * pCdcd)
//{
//    return pCdcd->wSerialState;
//}
//
///**
// * Sets the current serial state of the device to the given value.
// * \param pCdcd  Pointer to CDCDSerialPort instance.
// * \param wSerialState  New device state.
// */
//void CDCDSerialPort_SetSerialState(CDCDSerialPort * pCdcd,
//                                   uint16_t wSerialState)
//{
//    if (pCdcd->bIntInPIPE == 0)
//        return;
//
//    /* If new state is different from previous one, send a notification to the
//       host */
//    if (pCdcd->wSerialState != wSerialState) {
//
//        pCdcd->wSerialState = wSerialState;
//        USBD_Write(pCdcd->bIntInPIPE,
//                   &(pCdcd->wSerialState),
//                   2,
//                   0,
//                   0);
//
//        /* Reset one-time flags */
//        pCdcd->wSerialState &= ~(CDCSerialState_OVERRUN
//                              | CDCSerialState_PARITY
//                              | CDCSerialState_FRAMING
//                              | CDCSerialState_RINGSIGNAL
//                              | CDCSerialState_BREAK);
//    }
//}

/**@}*/

