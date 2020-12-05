/**************************************************************************//**
 * @file     hardware/stm32/stm32f2/STM32F205ZC/mcu_inc.h
 * @ingroup	 hardware_stm32f205zc
 * @brief    STM32F205ZC definitions
 * @version  V3.00
 * @date     26. October 2012
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h743zi
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H743ZI chip
 *
 *
 ******************************************************************************/

#ifndef MCU_INC_H_
#define MCU_INC_H_

#include <core_map.h>

//#define RAM_SIZE 			(112*1024) 	 //!< 112K RAM size
//#define SRAM1_SIZE 			(96*1024) 	 //!< 96K SRAM1 size
//#define SRAM2_SIZE 			(16*1024) 	 //!< 16K SRAM2 size
#define FLASH_SIZE 				(0x200000U)	 		//!< FLASH size (2MB) see Device Electronic signature
#define FLASH_BANK_SIZE    		(FLASH_SIZE >> 1)   //!< 1 MB
#define FLASH_SECTOR_SIZE  		0x00020000U       	//!< 128 KB
//
//#define BASE_FSMC_R         (BASE_EXT_DEVICE) 	//!< 0xA0000000 FSMC registers base address
#define BASE_D1_ITCMRAM			(BASE_CODE) 			 //!< 0x00000000 64KB RAM reserved for CPU execution/instruction accessible over ITCM
#define BASE_D1_ITCMICP         (BASE_CODE + 0x00100000) //!< 0x00100000 (up to 128KB) embedded Test FLASH memory accessible over ITCM
#define BASE_D1_AXIFLASH        (BASE_CODE + 0x08000000) //!< 0x08000000 (up to 2 MB) embedded FLASH memory accessible over AXI
#define BASE_FLASH				(BASE_CODE + 0x08000000) //!< 0x08000000 FLASH base address
#define BASE_FLASH_BANK1        (BASE_CODE + 0x08000000) //!< 0x08000000 (up to 1 MB) Flash Bank1 accessible over AXI
#define BASE_FLASH_BANK2        (BASE_CODE + 0x08100000) //!< 0x08100000 (up to 1 MB) Flash Bank2 accessible over AXI
#define BASE_D2_AXISRAM         (BASE_CODE + 0x10000000) //!< 0x10000000 (up to 48KB) system data RAM accessible over over AXI
#define BASE_D1_AXIICP          (BASE_CODE + 0x1FF00000) //!< 0x1FF00000 (up to 128KB) embedded Test FLASH memory accessible over AXI

#define BASE_D1_DTCMRAM         (BASE_SRAM) 			 //!< 0x20000000 128KB system data RAM accessible over DTCM
#define BASE_D1_AXISRAM         (BASE_SRAM + 0x04000000) //!< 0x24000000 (up to 384KB) system data RAM accessible over over AXI
#define BASE_D2_AHBSRAM         (BASE_SRAM + 0x10000000) //!< 0x30000000 (up to 48KB) system data RAM accessible over over AXI->AHB Bridge
#define BASE_D3_SRAM            (BASE_SRAM + 0x18000000) //!< 0x38000000 Backup SRAM(64 KB) over AXI->AHB Bridge
#define BASE_D3_BKPSRAM         (BASE_SRAM + 0x18800000) //!< 0x38800000 Backup SRAM(4 KB) over AXI->AHB Bridge

#define BASE_QSPI               (0x90000000) //!< 0x90000000 QSPI memories  accessible over AXI



/********************** Peripheral memory map *********************************/
#define BASE_D2_APB1PERIPH       (BASE_PERIPHERAL)				//!< 0x40000000
#define BASE_D2_APB2PERIPH       (BASE_PERIPHERAL + 0x00010000) //!< 0x40010000
#define BASE_D2_AHB1PERIPH       (BASE_PERIPHERAL + 0x00020000) //!< 0x40020000
#define BASE_D2_AHB2PERIPH       (BASE_PERIPHERAL + 0x08020000) //!< 0x48020000

#define BASE_D1_APB1PERIPH       (BASE_PERIPHERAL + 0x10000000) //!< 0x50000000
#define BASE_D1_AHB1PERIPH       (BASE_PERIPHERAL + 0x12000000) //!< 0x52000000

#define BASE_D3_APB1PERIPH       (BASE_PERIPHERAL + 0x18000000) //!< 0x58000000
#define BASE_D3_AHB1PERIPH       (BASE_PERIPHERAL + 0x18020000) //!< 0x58020000


/********************** D2_APB1 peripherals ***********************************/
#define BASE_TIM2             (BASE_D2_APB1PERIPH + 0x0000) //!< 0x40000000
#define BASE_TIM3             (BASE_D2_APB1PERIPH + 0x0400) //!< 0x40000400
#define BASE_TIM4             (BASE_D2_APB1PERIPH + 0x0800) //!< 0x40000800
#define BASE_TIM5             (BASE_D2_APB1PERIPH + 0x0C00) //!< 0x40000C00
#define BASE_TIM6             (BASE_D2_APB1PERIPH + 0x1000) //!< 0x40001000
#define BASE_TIM7             (BASE_D2_APB1PERIPH + 0x1400) //!< 0x40001400
#define BASE_TIM12            (BASE_D2_APB1PERIPH + 0x1800) //!< 0x40001800
#define BASE_TIM13            (BASE_D2_APB1PERIPH + 0x1C00) //!< 0x40001C00
#define BASE_TIM14            (BASE_D2_APB1PERIPH + 0x2000) //!< 0x40002000
#define BASE_LPTIM1           (BASE_D2_APB1PERIPH + 0x2400) //!< 0x40002400
#define BASE_SPI2             (BASE_D2_APB1PERIPH + 0x3800) //!< 0x40003800
#define BASE_SPI3             (BASE_D2_APB1PERIPH + 0x3C00) //!< 0x40003C00
#define BASE_SPDIFRX          (BASE_D2_APB1PERIPH + 0x4000) //!< 0x40004000
#define BASE_USART2           (BASE_D2_APB1PERIPH + 0x4400) //!< 0x40004400
#define BASE_USART3           (BASE_D2_APB1PERIPH + 0x4800) //!< 0x40004800
#define BASE_UART4            (BASE_D2_APB1PERIPH + 0x4C00) //!< 0x40004C00
#define BASE_UART5            (BASE_D2_APB1PERIPH + 0x5000) //!< 0x40005000
#define BASE_I2C1             (BASE_D2_APB1PERIPH + 0x5400) //!< 0x40005400
#define BASE_I2C2             (BASE_D2_APB1PERIPH + 0x5800) //!< 0x40005800
#define BASE_I2C3             (BASE_D2_APB1PERIPH + 0x5C00) //!< 0x40005C00
#define BASE_CEC              (BASE_D2_APB1PERIPH + 0x6C00) //!< 0x40006C00
#define BASE_DAC1             (BASE_D2_APB1PERIPH + 0x7400) //!< 0x40007400
#define BASE_UART7            (BASE_D2_APB1PERIPH + 0x7800) //!< 0x40007800
#define BASE_UART8            (BASE_D2_APB1PERIPH + 0x7C00) //!< 0x40007C00
#define BASE_CRS              (BASE_D2_APB1PERIPH + 0x8400) //!< 0x40008400
#define BASE_SWPMI1           (BASE_D2_APB1PERIPH + 0x8800) //!< 0x40008800
#define BASE_OPAMP            (BASE_D2_APB1PERIPH + 0x9000) //!< 0x40009000
#define BASE_OPAMP1           (BASE_D2_APB1PERIPH + 0x9000) //!< 0x40009000
#define BASE_OPAMP2           (BASE_D2_APB1PERIPH + 0x9010) //!< 0x40009010
#define BASE_MDIOS            (BASE_D2_APB1PERIPH + 0x9400) //!< 0x40009400
#define BASE_FDCAN1           (BASE_D2_APB1PERIPH + 0xA000) //!< 0x4000A000
#define BASE_FDCAN2           (BASE_D2_APB1PERIPH + 0xA400) //!< 0x4000A400
#define BASE_FDCAN_CCU        (BASE_D2_APB1PERIPH + 0xA800) //!< 0x4000A800
#define BASE_SRAMCAN          (BASE_D2_APB1PERIPH + 0xAC00) //!< 0x4000AC00

/********************** D2_APB2 peripherals ***********************************/
#define BASE_TIM1             (BASE_D2_APB2PERIPH + 0x0000) //!< 0x40010000
#define BASE_TIM8             (BASE_D2_APB2PERIPH + 0x0400) //!< 0x40010400
#define BASE_USART1           (BASE_D2_APB2PERIPH + 0x1000) //!< 0x40011000
#define BASE_USART6           (BASE_D2_APB2PERIPH + 0x1400) //!< 0x40011400
#define BASE_SPI1             (BASE_D2_APB2PERIPH + 0x3000) //!< 0x40013000
#define BASE_SPI4             (BASE_D2_APB2PERIPH + 0x3400) //!< 0x40013400
#define BASE_TIM15            (BASE_D2_APB2PERIPH + 0x4000) //!< 0x40014000
#define BASE_TIM16            (BASE_D2_APB2PERIPH + 0x4400) //!< 0x40014400
#define BASE_TIM17            (BASE_D2_APB2PERIPH + 0x4800) //!< 0x40014800
#define BASE_SPI5             (BASE_D2_APB2PERIPH + 0x5000) //!< 0x40015000
#define BASE_SAI1             (BASE_D2_APB2PERIPH + 0x5800) //!< 0x40015800
#define BASE_SAI2             (BASE_D2_APB2PERIPH + 0x5C00) //!< 0x40015C00
#define BASE_SAI3             (BASE_D2_APB2PERIPH + 0x6000) //!< 0x40016000
#define BASE_DFSDM1           (BASE_D2_APB2PERIPH + 0x7000) //!< 0x40017000
#define BASE_HRTIM1           (BASE_D2_APB2PERIPH + 0x7400) //!< 0x40017400

/********************** D2_AHB1 peripherals ***********************************/
#define BASE_DMA1             (BASE_D2_AHB1PERIPH + 0x00000) //!< 0x40020000
#define BASE_DMA2             (BASE_D2_AHB1PERIPH + 0x00400) //!< 0x40020400
#define BASE_DMAMUX1          (BASE_D2_AHB1PERIPH + 0x00800) //!< 0x40020800
#define BASE_ADC1             (BASE_D2_AHB1PERIPH + 0x02000) //!< 0x40022000
#define BASE_ADC2             (BASE_D2_AHB1PERIPH + 0x02100) //!< 0x40022100
#define BASE_ADC12_COMMON     (BASE_D2_AHB1PERIPH + 0x02300) //!< 0x40022300
#define BASE_ETH              (BASE_D2_AHB1PERIPH + 0x08000) //!< 0x40028000
#define BASE_ETH_MAC          (BASE_ETH)                     //!< 0x40028000
#define BASE_USB1_OTG_HS      (BASE_D2_AHB1PERIPH + 0x20000) //!< 0x40040000
#define BASE_USB2_OTG_FS      (BASE_D2_AHB1PERIPH + 0x60000) //!< 0x40080000

/********************** D2_AHB2 peripherals ***********************************/
#define BASE_DCMI             (BASE_D2_AHB2PERIPH + 0x0000) //!< 0x48020000
#define BASE_RNG              (BASE_D2_AHB2PERIPH + 0x1800) //!< 0x48021800
#define BASE_SDMMC2           (BASE_D2_AHB2PERIPH + 0x2400) //!< 0x48022400
#define BASE_DLYB_SDMMC2      (BASE_D2_AHB2PERIPH + 0x2800) //!< 0x48022800
#define BASE_RAMECC2          (BASE_D2_AHB2PERIPH + 0x3000) //!< 0x48023000

/********************** D1_APB1 peripherals ***********************************/
#define BASE_WWDG1            (BASE_D1_APB1PERIPH + 0x3000) //!< 0x50003000

/********************** D1_AHB1 peripherals ***********************************/
#define BASE_MDMA             (BASE_D1_AHB1PERIPH + 0x0000) //!< 0x52000000
#define BASE_DMA2D            (BASE_D1_AHB1PERIPH + 0x1000) //!< 0x52001000
#define BASE_FLASH_R          (BASE_D1_AHB1PERIPH + 0x2000) //!< 0x52002000
#define BASE_FMC_R            (BASE_D1_AHB1PERIPH + 0x4000) //!< 0x52004000
#define BASE_QSPI_R           (BASE_D1_AHB1PERIPH + 0x5000) //!< 0x52005000
#define BASE_DLYB_QSPI        (BASE_D1_AHB1PERIPH + 0x6000) //!< 0x52006000
#define BASE_SDMMC1           (BASE_D1_AHB1PERIPH + 0x7000) //!< 0x52007000
#define BASE_DLYB_SDMMC1      (BASE_D1_AHB1PERIPH + 0x8000) //!< 0x52008000
#define BASE_RAMECC1          (BASE_D1_AHB1PERIPH + 0x9000) //!< 0x52009000

/********************** D3_APB1 peripherals ***********************************/
#define BASE_EXTI             (BASE_D3_APB1PERIPH + 0x0000) //!< 0x58000000
#define BASE_SYSCFG           (BASE_D3_APB1PERIPH + 0x0400) //!< 0x58000400
#define BASE_LPUART1          (BASE_D3_APB1PERIPH + 0x0C00) //!< 0x58000C00
#define BASE_SPI6             (BASE_D3_APB1PERIPH + 0x1400) //!< 0x58001400
#define BASE_I2C4             (BASE_D3_APB1PERIPH + 0x1C00) //!< 0x58001C00
#define BASE_LPTIM2           (BASE_D3_APB1PERIPH + 0x2400) //!< 0x58002400
#define BASE_LPTIM3           (BASE_D3_APB1PERIPH + 0x2800) //!< 0x58002800
#define BASE_LPTIM4           (BASE_D3_APB1PERIPH + 0x2C00) //!< 0x58002C00
#define BASE_LPTIM5           (BASE_D3_APB1PERIPH + 0x3000) //!< 0x58003000
#define BASE_COMP12           (BASE_D3_APB1PERIPH + 0x3800) //!< 0x58003800
#define BASE_VREFBUF          (BASE_D3_APB1PERIPH + 0x3C00) //!< 0x58003C00
#define BASE_RTC              (BASE_D3_APB1PERIPH + 0x4000) //!< 0x58004000
#define BASE_IWDG1            (BASE_D3_APB1PERIPH + 0x4800) //!< 0x58004800
#define BASE_SAI4             (BASE_D3_APB1PERIPH + 0x5400) //!< 0x58005400

/********************** D3_AHB1 peripherals ***********************************/
#define BASE_GPIOA            (BASE_D3_AHB1PERIPH + 0x0000) //!< 0x58020000
#define BASE_GPIOB            (BASE_D3_AHB1PERIPH + 0x0400) //!< 0x58020400
#define BASE_GPIOC            (BASE_D3_AHB1PERIPH + 0x0800) //!< 0x58020800
#define BASE_GPIOD            (BASE_D3_AHB1PERIPH + 0x0C00) //!< 0x58020C00
#define BASE_GPIOE            (BASE_D3_AHB1PERIPH + 0x1000) //!< 0x58021000
#define BASE_GPIOF            (BASE_D3_AHB1PERIPH + 0x1400) //!< 0x58021400
#define BASE_GPIOG            (BASE_D3_AHB1PERIPH + 0x1800) //!< 0x58021800
#define BASE_GPIOH            (BASE_D3_AHB1PERIPH + 0x1C00) //!< 0x58021C00
#define BASE_GPIOI            (BASE_D3_AHB1PERIPH + 0x2000) //!< 0x58022000
#define BASE_GPIOJ            (BASE_D3_AHB1PERIPH + 0x2400) //!< 0x58022400
#define BASE_GPIOK            (BASE_D3_AHB1PERIPH + 0x2800) //!< 0x58022800
#define BASE_RCC              (BASE_D3_AHB1PERIPH + 0x4400) //!< 0x58024400
#define BASE_PWR              (BASE_D3_AHB1PERIPH + 0x4800) //!< 0x58024800
#define BASE_CRC              (BASE_D3_AHB1PERIPH + 0x4C00) //!< 0x58024C00
#define BASE_BDMA             (BASE_D3_AHB1PERIPH + 0x5400) //!< 0x58025400
#define BASE_DMAMUX2          (BASE_D3_AHB1PERIPH + 0x5800) //!< 0x58025800
#define BASE_ADC3             (BASE_D3_AHB1PERIPH + 0x6000) //!< 0x58026000
#define BASE_ADC3_COMMON      (BASE_D3_AHB1PERIPH + 0x6300) //!< 0x58026300
#define BASE_HSEM             (BASE_D3_AHB1PERIPH + 0x6400) //!< 0x58026400
#define BASE_RAMECC3          (BASE_D3_AHB1PERIPH + 0x7000) //!< 0x58027000


/**********************  FMC Banks registers base address ********************/
#define BASE_FSMC_Bank1_R	(BASE_FMC_R + 0x0000)			//!< 0x52004000
#define BASE_FSMC_Bank1E_R	(BASE_FMC_R + 0x0104)			//!< 0x52004104
#define BASE_FSMC_Bank2_R	(BASE_FMC_R + 0x0060)			//!< 0x52004060
#define BASE_FSMC_Bank3_R	(BASE_FMC_R + 0x0080)			//!< 0x52004080
#define BASE_FSMC_Bank5_6_R	(BASE_FMC_R + 0x0140)			//!< 0x52004140

/* Debug MCU registers base address */
#define BASE_DBGMCU         (0x5C001000)					//!< 0x5C001000


#endif /* MCU_INC_H_ */
