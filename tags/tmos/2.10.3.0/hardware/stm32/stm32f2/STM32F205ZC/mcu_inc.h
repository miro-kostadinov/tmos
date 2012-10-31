/**************************************************************************//**
 * @file     hardware/stm32/stm32f2/STM32F205ZC/mcu_inc.h
 * @ingroup	 hardware_stm32f205zc
 * @brief    STM32F205ZC definitions
 * @version  V3.00
 * @date     26. October 2012
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f205zc
 * @ingroup	 hardware_stm32f2
 * Source and definitions for STM32F205ZC chip
 *
 *
 ******************************************************************************/

#ifndef MCU_INC_H_
#define MCU_INC_H_

#define RAM_SIZE 			(96*1024) 	 //!< 96K RAM size
#define SRAM1_SIZE 			(80*1024) 	 //!< 80K SRAM1 size
#define SRAM2_SIZE 			(16*1024) 	 //!< 16K SRAM2 size
#define FLASH_SIZE 			(256*1024) 	 //!< 256K FLASH size

#define FLASH_BASE			(0x08000000) //!< FLASH base address in the alias region
#define SRAM_BASE           (0x20000000) //!< SRAM base address in the alias region
#define PERIPH_BASE         (0x40000000) //!< Peripheral base address in the alias region

#define SRAM_BB_BASE        (0x22000000) //!< SRAM base address in the bit-band region
#define PERIPH_BB_BASE      (0x42000000) //!< Peripheral base address in the bit-band region

#define FSMC_R_BASE         (0xA0000000) //!< FSMC registers base address

/*!< Peripheral memory map */
#define APB1PERIPH_BASE     PERIPH_BASE
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000)
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE     (PERIPH_BASE + 0x10000000)

#endif /* MCU_INC_H_ */
