/**************************************************************************//**
 * @file     hardware/stm32/stm32f1/STM32F103RB/mcu_inc.h
 * @ingroup	 hardware_stm32f103rb
 * @brief    STM32F103RB definitions
 * @version  V3.00
 * @date     26. October 2012
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f103rb
 * @ingroup	 hardware_stm32f1
 * Source and definitions for STM32F103RB chip
 *
 *
 ******************************************************************************/

#ifndef MCU_INC_H_
#define MCU_INC_H_

#define STM32F10X_MD			//!< STM32F103RB is medium density line


#define RAM_SIZE 			(20*1024) 	 //!< 20K RAM size

#define FLASH_BASE			(0x08000000) //!< FLASH base address in the alias region
#define SRAM_BASE           (0x20000000) //!< SRAM base address in the alias region
#define PERIPH_BASE         (0x40000000) //!< Peripheral base address in the alias region

#define SRAM_BB_BASE        (0x22000000) //!< SRAM base address in the bit-band region
#define PERIPH_BB_BASE      (0x42000000) //!< Peripheral base address in the bit-band region

#define FSMC_R_BASE         (0xA0000000) //!< FSMC registers base address

/*!< Peripheral memory map */
#define APB1PERIPH_BASE     PERIPH_BASE
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE      (PERIPH_BASE + 0x20000)

#endif /* MCU_INC_H_ */
