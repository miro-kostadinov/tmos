/**************************************************************************//**
 * @ingroup	 lm3s_can
 * @{
 * @file     lm3s/can_lm3s.h
 * @brief    Macros used when accessing the CAN hardware.
 *
 * Copyright (c) 2005-2009 Luminary Micro, Inc.  All rights reserved.
 * Software License Agreement
 *
 * Luminary Micro, Inc. (LMI) is supplying this software for use solely and
 * exclusively on LMI's microcontroller products.
 *
 * The software is owned by LMI and/or its suppliers, and is protected under
 * applicable copyright laws.  All rights are reserved.  You may not combine
 * this software with "viral" open-source software in order to form a larger
 * program.  Any use in violation of the foregoing restrictions may subject
 * the user to criminal sanctions under applicable laws, as well as to civil
 * liability for the breach of the terms and conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * This is part of revision 5228 of the Stellaris Firmware Development Package.
 *
 *
 ******************************************************************************/

#ifndef CAN_LM3S_H_
#define CAN_LM3S_H_

#include <tmos_types.h>

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif


/*----------       Controller Area Network (CAN)                  -----------*/
///  Controller Area Network (CAN)
typedef struct
{
    __IO uint32_t CTL;              //!< Control register
    __IO uint32_t STS;              //!< Status register
    __I  uint32_t ERR;              //!< Error register
    __IO uint32_t BIT;              //!< Bit Timing register
    __I  uint32_t INT;              //!< Interrupt register
    __IO uint32_t TST;              //!< Test register
    __IO uint32_t BRPE;             //!< Baud Rate Prescaler register
         uint8_t  RESERVED0[4];		//!< reserved
    __IO uint32_t IF1CRQ;           //!< Interface 1 Command Request reg.
    __IO uint32_t IF1CMSK;          //!< Interface 1 Command Mask reg.
    __IO uint32_t IF1MSK1;          //!< Interface 1 Mask 1 register
    __IO uint32_t IF1MSK2;          //!< Interface 1 Mask 2 register
    __IO uint32_t IF1ARB1;          //!< Interface 1 Arbitration 1 reg.
    __IO uint32_t IF1ARB2;          //!< Interface 1 Arbitration 2 reg.
    __IO uint32_t IF1MCTL;          //!< Interface 1 Message Control reg.
    __IO uint32_t IF1DA1;           //!< Interface 1 DataA 1 register
    __IO uint32_t IF1DA2;           //!< Interface 1 DataA 2 register
    __IO uint32_t IF1DB1;           //!< Interface 1 DataB 1 register
    __IO uint32_t IF1DB2;           //!< Interface 1 DataB 2 register
         uint8_t  RESERVED1[52];	//!< reserved
    __IO uint32_t IF2CRQ;           //!< Interface 2 Command Request reg.
    __IO uint32_t IF2CMSK;          //!< Interface 2 Command Mask reg.
    __IO uint32_t IF2MSK1;          //!< Interface 2 Mask 1 register
    __IO uint32_t IF2MSK2;          //!< Interface 2 Mask 2 register
    __IO uint32_t IF2ARB1;          //!< Interface 2 Arbitration 1 reg.
    __IO uint32_t IF2ARB2;          //!< Interface 2 Arbitration 2 reg.
    __IO uint32_t IF2MCTL;          //!< Interface 2 Message Control reg.
    __IO uint32_t IF2DA1;           //!< Interface 2 DataA 1 register
    __IO uint32_t IF2DA2;           //!< Interface 2 DataA 2 register
    __IO uint32_t IF2DB1;           //!< Interface 2 DataB 1 register
    __IO uint32_t IF2DB2;           //!< Interface 2 DataB 2 register
         uint8_t  RESERVED2[84];	//!< reserved
    __I  uint32_t TXRQ1;            //!< Transmission Request 1 register
    __I  uint32_t TXRQ2;            //!< Transmission Request 2 register
         uint8_t  RESERVED3[24];	//!< reserved
    __I  uint32_t NWDA1;            //!< New Data 1 register
    __I  uint32_t NWDA2;            //!< New Data 2 register
         uint8_t  RESERVED4[24];	//!< reserved
    __I  uint32_t MSG1INT;          //!< CAN Message 1 Interrupt Pending
    __I  uint32_t MSG2INT;          //!< CAN Message 2 Interrupt Pending
         uint8_t  RESERVED5[24];	//!< reserved
    __I  uint32_t MSG1VAL;          //!< CAN Message 1 Valid
    __I  uint32_t MSG2VAL;          //!< CAN Message 2 Valid
} CAN_Type;


#define CAN0                ((CAN_Type *)CAN0_BASE)
#define CAN1                ((CAN_Type *)CAN1_BASE)
#define CAN2                ((CAN_Type *)CAN2_BASE)


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif


#endif /* CAN_LM3S_H_ */
