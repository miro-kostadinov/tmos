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
 *
 *  \section Purpose
 *
 *  Implementation of USB device functions on a UDP controller.
 *
 *  See \ref usbd_api "USBD API Methods".
 */


#include <tmos.h>


///**
// *  Handle the USB resume event, should be invoked whenever
// *  HW reports a resume signal.
// */
//void USBD_ResumeHandler(void)
//{
//    /* Don't do anything if the device was not suspended */
//    if (deviceState == USBD_STATE_SUSPENDED) {
//        /* Active the device */
//        USBD_HAL_Activate();
//        deviceState = previousDeviceState;
//        if (deviceState >= USBD_STATE_DEFAULT) {
//            /* Invoke the Resume callback */
//            if (USBDCallbacks_Resumed)
//                USBDCallbacks_Resumed();
//        }
//    }
//}

/**
 * Indicates if the device is running in high or full-speed. Always returns 0
 * since UDP does not support high-speed mode.
 */
uint8_t USBD_IsHighSpeed(void)
{
  //  return USBD_HAL_IsHighSpeed();
	return (0);
}

