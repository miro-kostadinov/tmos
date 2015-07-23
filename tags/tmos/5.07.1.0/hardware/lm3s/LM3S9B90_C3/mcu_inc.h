/*
 * mcu_inc.h for LM2S9B90 rev C3
 *
 *  Created on: 11.11.2010
 *      Author: Miroslav Kostadinov
 */

#ifndef MCU_INC_H_
#define MCU_INC_H_


#include <core_map.h>

#define TARGET_IS_TEMPEST_RC3	1		// Tempest rev C3

#define RAM_SIZE 			0x18000
#define FLASH_PAGE_SIZE		0x400

#define MCU_RESET_CAUSE_SW  0x00000010	//SYSCTL_CAUSE_SW

#endif /* MCU_INC_H_ */
