/********************************************
 * @ingroup	hardware_stm32f2_iwdg
 * @file	hardware/stm32/stm32f2/iwdg_f2.h
 * @brief  	STM32F2 IWDG
 *
 *  Created on: 10.06.2022 г.
 *      Author: bstanislav
 *********************************************/

#ifndef IWDG_F2_H_
#define IWDG_F2_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Independent watchdog (IWDG)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t IWDG_KR;   		   	//!< (iwdg Offset: 0x00) Key register (IWDG_KR)
  __IO uint32_t IWDG_PR;      		//!< (iwdg Offset: 0x04) Prescaler register (IWDG_PR)
  __IO uint32_t IWDG_RLR;      		//!< (iwdg Offset: 0x08) Reload register (IWDG_RLR)
  __IO uint32_t IWDG_SR;      		//!< (iwdg Offset: 0x0C) Status register (IWDG_SR)
} IWDG_TypeDef;

#define IWDG_KR_START	0xCCCC		//!< starts the watchdog
#define IWDG_KR_RESTART	0xAAAA		//!< These bits must be written ,otherwise the watchdog generates a reset
#define IWDG_KR_ENABLE	0x5555		//!< enable access to the IWDG_PR and IWDG_RLR

#define IWDG_PR_PR		0x0007		//!< PR[2:0]: Prescaler divider (111 divider /256)
#define IWDG_PR_4		0x0000		//!< 000: divider /4
#define IWDG_PR_8		0x0001		//!< 001: divider /8
#define IWDG_PR_16		0x0002		//!< 010: divider /16
#define IWDG_PR_32		0x0003		//!< 011: divider /32
#define IWDG_PR_64		0x0004		//!< 100: divider /64
#define IWDG_PR_128		0x0005		//!< 101: divider /128
#define IWDG_PR_256		0x0006		//!< 110: divider /256

#define IWDG_RLR_LR		0x0FFF		//!< RL[11:0]: Watchdog counter reload value

#define IWDG_SR_RVU		0x0002		//!< Bit 1 RVU: Watchdog counter reload value update
#define IWDG_SR_PVU		0x0001		//!< Bit 0 PVU: Watchdog prescaler value update

#endif /* IWDG_F2_H_ */
