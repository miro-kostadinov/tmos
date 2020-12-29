/**************************************************************************//**
 * @ingroup	hardware_stm32h7_gpio
 * @file	hardware/stm32/stm32h7/gpio_h7.h
 * @brief  	STM32F2 GPIO
 * @date    7. March 2020
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_gpio  General-purpose I/Os (GPIO)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 General-purpose I/Os (GPIO)
 * @{
 *
 ******************************************************************************/

#ifndef GPIO_H7_H_
#define GPIO_H7_H_

#include <mcu_inc.h>
#include <tmos_types.h>
#include <gpio_stm32.h>

#define PD_LAST_PORT		11				//!< number of physical ports

#define PD_AF0		(PD_AF | PD_MUX_AF0	)	//!< Alternative function map 0
#define PD_AF1		(PD_AF | PD_MUX_AF1	)	//!< Alternative function map 1
#define PD_AF2		(PD_AF | PD_MUX_AF2	)	//!< Alternative function map 2
#define PD_AF3		(PD_AF | PD_MUX_AF3	)	//!< Alternative function map 3
#define PD_AF4		(PD_AF | PD_MUX_AF4	)	//!< Alternative function map 4
#define PD_AF5		(PD_AF | PD_MUX_AF5	)	//!< Alternative function map 5
#define PD_AF6		(PD_AF | PD_MUX_AF6	)	//!< Alternative function map 6
#define PD_AF7		(PD_AF | PD_MUX_AF7	)	//!< Alternative function map 7
#define PD_AF8		(PD_AF | PD_MUX_AF8	)	//!< Alternative function map 8
#define PD_AF9		(PD_AF | PD_MUX_AF9	)	//!< Alternative function map 9
#define PD_AF10		(PD_AF | PD_MUX_AF10)	//!< Alternative function map 10
#define PD_AF11		(PD_AF | PD_MUX_AF11)	//!< Alternative function map 11
#define PD_AF12		(PD_AF | PD_MUX_AF12)	//!< Alternative function map 12
#define PD_AF13		(PD_AF | PD_MUX_AF13)	//!< Alternative function map 13
#define PD_AF14		(PD_AF | PD_MUX_AF14)	//!< Alternative function map 14
#define PD_AF15		(PD_AF | PD_MUX_AF15)	//!< Alternative function map 15

#ifdef __cplusplus
 extern "C" {
#endif

/***************************************************************************//**
 *  GPIO hardware registers
 ******************************************************************************/
typedef struct
{
	__IO uint32_t GPIO_MODER;    //!< (gpio Offset: 0x00) GPIO port mode register
	__IO uint32_t GPIO_OTYPER;   //!< (gpio Offset: 0x04) GPIO port output type register
	__IO uint32_t GPIO_OSPEEDR;  //!< (gpio Offset: 0x08) GPIO port output speed register
	__IO uint32_t GPIO_PUPDR;    //!< (gpio Offset: 0x0C) GPIO port pull-up/pull-down register
	__IO uint32_t GPIO_IDR;      //!< (gpio Offset: 0x10) GPIO port input data register
	__IO uint32_t GPIO_ODR;      //!< (gpio Offset: 0x14) GPIO port output data register
	__IO uint32_t GPIO_BSRR;     //!< (gpio Offset: 0x18) GPIO port bit set/reset register
	__IO uint32_t GPIO_LCKR;     //!< (gpio Offset: 0x1C) GPIO port configuration lock register
	__IO uint32_t GPIO_AFR[2];   //!< (gpio Offset: 0x20) GPIO alternate function registers
} GPIO_TypeDef;

/***************************************************************************//**
 * @relates GPIO_TypeDef
 * @{
 ******************************************************************************/


/** @name GPIO_MODER:	(gpio Offset: 0x00) GPIO port mode register			  */
/** @{ */
#define GPIO_MODER_MODERy_Msk(pin)		(3<<((pin)*2))       //!< Port x configuration bits (y = 0..15)
#define GPIO_MODER_MODERy_Get(pin,x)  	(((x)>>((pin)*2))&3)
#define GPIO_MODER_MODERy_Set(pin,x)    ((x)<<((pin)*2))

#define GPIO_MODER_MODERy_INPUT			0		//!< input (reset state)
#define GPIO_MODER_MODERy_OUTPUT		1		//!< general purpose output mode
#define GPIO_MODER_MODERy_AF			2		//!< alternative function mode
#define GPIO_MODER_MODERy_ANALOG		3		//!< analog mode
/** @} */

/** @name GPIO_OTYPER:	(gpio Offset: 0x04) GPIO port output type register	  */
/** @{ */
#define GPIO_OTYPER_OTy				0x0000FFFF	//!< Port x configuration bits (y = 0..15)
#define GPIO_OTYPER_OTy_PP			0			//!< Output push-pull (reset state)
#define GPIO_OTYPER_OTy_OD			1			//!< Output open-drain
/** @} */

/** @name GPIO_OSPEEDR:	(gpio Offset: 0x08) GPIO port output speed register   */
/** @{ */
#define GPIO_OSPEEDER_OSPEEDRy_Msk(pin)		(3<<((pin)*2))       //!< OSPEEDRy[1:0]: Port x configuration bits (y = 0..15)
#define GPIO_OSPEEDER_OSPEEDRy_Get(pin,x)  	(((x)>>((pin)*2))&3)
#define GPIO_OSPEEDER_OSPEEDRy_Set(pin,x)   ((x)<<((pin)*2))

#define GPIO_OSPEEDER_OSPEEDRy_2MHz   	0		//!< 2 MHz Low speed
#define GPIO_OSPEEDER_OSPEEDRy_25MHz   	1		//!< 25 MHz Medium speed
#define GPIO_OSPEEDER_OSPEEDRy_50MHz   	2		//!< 50 MHz Fast speed
#define GPIO_OSPEEDER_OSPEEDRy_100MHz  	3		//!< 100 MHz High speed on 30 pF (80 MHz Output max speed on 15 pF)
/** @} */

/** @name GPIO_PUPDR:	(gpio Offset: 0x0C) GPIO port pull-up/pull-down register */
/** @{ */
#define GPIO_PUPDR_PUPDRy_Msk(pin)		(3<<((pin)*2))    //!< PUPDRy[1:0]: Port x configuration bits (y = 0..15)
#define GPIO_PUPDR_PUPDRy_Get(pin,x)  	(((x)>>((pin)*2))&3)
#define GPIO_PUPDR_PUPDRy_Set(pin,x)   ((x)<<((pin)*2))

#define GPIO_PUPDR_PUPDRy_NONE   	0		//!< No pull-up, pull-down
#define GPIO_PUPDR_PUPDRy_PU   		1		//!< Pull-up
#define GPIO_PUPDR_PUPDRy_PD   		2		//!< Pull-down
/** @} */


/** @name GPIO_BSRR:	(gpio Offset: 0x18) GPIO port bit set/reset register  */
/** @{ */
#define GPIO_BSRR_BS(x)				x			//!< set macro
#define GPIO_BSRR_BR(x)				((x)<<16)	//!< reset macro
/** @} */

/** @name GPIO_LCKR:	(gpio Offset: 0x1C) GPIO port configuration lock register */
/** @{ */
#define GPIO_LCKR_LCKy				0x0000FFFF	//!< Port x configuration bits (y = 0..15)
#define GPIO_LCKR_LCKK				0x00010000	//!< Lock key
/** @} */

/** @name GPIO_AFR[2]:	(gpio Offset: 0x20) GPIO alternate function registers */
/** @{ */
#define GPIO_AFR_AFRy_Msk(pin)		(15<<(((pin) & 7)*4))       //!< AFRy: Alternate function selection for port x bit y (y = 0..7)
#define GPIO_AFR_AFRy_Get(pin,x)  	(((x)>>(((pin) & 7)*4))&15)
#define GPIO_AFR_AFRy_Set(pin,x)    ((x)<<(((pin) & 7)*4))

#define GPIO_AFR_AFRy_AF0   		0			//!< AF0
#define GPIO_AFR_AFRy_AF1   		1 			//!< AF1
#define GPIO_AFR_AFRy_AF2   		2 			//!< AF2
#define GPIO_AFR_AFRy_AF3   		3 			//!< AF3
#define GPIO_AFR_AFRy_AF4   		4 			//!< AF4
#define GPIO_AFR_AFRy_AF5   		5 			//!< AF5
#define GPIO_AFR_AFRy_AF6   		6 			//!< AF6
#define GPIO_AFR_AFRy_AF7   		7 			//!< AF7
#define GPIO_AFR_AFRy_AF8   		8 			//!< AF8
#define GPIO_AFR_AFRy_AF9   		9 			//!< AF9
#define GPIO_AFR_AFRy_AF10  		10			//!< AF10
#define GPIO_AFR_AFRy_AF11  		11			//!< AF11
#define GPIO_AFR_AFRy_AF12  		12			//!< AF12
#define GPIO_AFR_AFRy_AF13  		13			//!< AF13
#define GPIO_AFR_AFRy_AF14  		14			//!< AF14
#define GPIO_AFR_AFRy_AF15  		15			//!< AF15
/** @} */


/** @} */ // @@relates GPIO_TypeDef

/***************************************************************************//**
 *  System configuration controller
 ******************************************************************************/
typedef struct
{
  __IO uint32_t reserved1; 		    //!< (syscfg Offset: 0x000) reserved
  __IO uint32_t SYSCFG_PMCR;        //!< (syscfg Offset: 0x004) SYSCFG peripheral mode configuration register
  __IO uint32_t SYSCFG_EXTICR[4];   //!< (syscfg Offset: 0x008) SYSCFG external interrupt configuration registers
  __IO uint32_t SYSCFG_CFGR;        //!< (syscfg Offset: 0x018) SYSCFG configuration registers
  __IO uint32_t reserved2;          //!< (syscfg Offset: 0x01C) reserved
  __IO uint32_t SYSCFG_CCCSR;       //!< (syscfg Offset: 0x020) SYSCFG compensation cell control/status register
  __IO uint32_t SYSCFG_CCVR;        //!< (syscfg Offset: 0x024) SYSCFG compensation cell value register
  __IO uint32_t SYSCFG_CCCR;        //!< (syscfg Offset: 0x028) SYSCFG compensation cell code register
  __IO uint32_t SYSCFG_PWRCR;       //!< (syscfg Offset: 0x02C) PWR control register
  __IO uint32_t reserved3[61];  	//!< (syscfg Offset: 0x030) reserved
  __IO uint32_t SYSCFG_PKGR;        //!< (syscfg Offset: 0x124) SYSCFG package register
  __IO uint32_t reserved4[118]; 	//!< (syscfg Offset: 0x128) Reserved
  __IO uint32_t SYSCFG_UR0;         //!< (syscfg Offset: 0x300) SYSCFG user register 0
  __IO uint32_t SYSCFG_UR1;         //!< (syscfg Offset: 0x304) SYSCFG user register 1
  __IO uint32_t SYSCFG_UR2;         //!< (syscfg Offset: 0x308) SYSCFG user register 2
  __IO uint32_t SYSCFG_UR3;         //!< (syscfg Offset: 0x30C) SYSCFG user register 3
  __IO uint32_t SYSCFG_UR4;         //!< (syscfg Offset: 0x310) SYSCFG user register 4
  __IO uint32_t SYSCFG_UR5;         //!< (syscfg Offset: 0x314) SYSCFG user register 5
  __IO uint32_t SYSCFG_UR6;         //!< (syscfg Offset: 0x318) SYSCFG user register 6
  __IO uint32_t SYSCFG_UR7;         //!< (syscfg Offset: 0x31C) SYSCFG user register 7
  __IO uint32_t SYSCFG_UR8;         //!< (syscfg Offset: 0x320) SYSCFG user register 8
  __IO uint32_t SYSCFG_UR9;         //!< (syscfg Offset: 0x324) SYSCFG user register 9
  __IO uint32_t SYSCFG_UR10;        //!< (syscfg Offset: 0x328) SYSCFG user register 10
  __IO uint32_t SYSCFG_UR11;        //!< (syscfg Offset: 0x32C) SYSCFG user register 11
  __IO uint32_t SYSCFG_UR12;        //!< (syscfg Offset: 0x330) SYSCFG user register 12
  __IO uint32_t SYSCFG_UR13;        //!< (syscfg Offset: 0x334) SYSCFG user register 13
  __IO uint32_t SYSCFG_UR14;        //!< (syscfg Offset: 0x338) SYSCFG user register 14
  __IO uint32_t SYSCFG_UR15;        //!< (syscfg Offset: 0x33C) SYSCFG user register 15
  __IO uint32_t SYSCFG_UR16;        //!< (syscfg Offset: 0x340) SYSCFG user register 16
  __IO uint32_t SYSCFG_UR17;        //!< (syscfg Offset: 0x344) SYSCFG user register 17
} SYSCFG_TypeDef;

/***************************************************************************//**
 * @relates SYSCFG_TypeDef
 * @{
 ******************************************************************************/

/** @name SYSCFG_PMCR:      (syscfg Offset: 0x004) SYSCFG peripheral mode configuration register */
/** @{ */
#define SYSCFG_PMCR_PC3SO		      	0x08000000 //!< PC3 Switch Open
#define SYSCFG_PMCR_PC2SO		      	0x04000000 //!< PC2 Switch Open
#define SYSCFG_PMCR_PA1SO		      	0x02000000 //!< PA1 Switch Open
#define SYSCFG_PMCR_PA0SO		      	0x01000000 //!< PA0 Switch Open
#define SYSCFG_PMCR_EPIS		      	0x00E00000 //!< Ethernet PHY Interface Selection
#define SYSCFG_PMCR_EPIS_MII	      	0x00000000 //!<  MII
#define SYSCFG_PMCR_EPIS_RMII	      	0x00800000 //!<  RMII
#define SYSCFG_PMCR_BOOSTVDDSEL	      	0x00000200 //!< Analog switch supply voltage selection (V DD /V DDA /booster)
#define SYSCFG_PMCR_BOOSTE		      	0x00000100 //!< Booster Enable
#define SYSCFG_PMCR_PB9FMP		      	0x00000080 //!< enables I2C Fm+ on PB(9)
#define SYSCFG_PMCR_PB8FMP		      	0x00000040 //!< enables I2C Fm+ on PB(8)
#define SYSCFG_PMCR_PB7FMP		      	0x00000020 //!< enables I2C Fm+ on PB(7)
#define SYSCFG_PMCR_PB6FMP		      	0x00000010 //!< enables I2C Fm+ on PB(6)
#define SYSCFG_PMCR_I2C4FMP		      	0x00000008 //!< enables Fm+ on I2C4
#define SYSCFG_PMCR_I2C3FMP		      	0x00000004 //!< enables Fm+ on I2C3
#define SYSCFG_PMCR_I2C2FMP		      	0x00000002 //!< enables Fm+ on I2C2
#define SYSCFG_PMCR_I2C1FMP		      	0x00000001 //!< enables Fm+ on I2C1
/** @} */

/** @name SYSCFG_EXTICR[4]: (syscfg Offset: 0x008) SYSCFG external interrupt configuration registers */
/** @{ */
#define SYSCFG_EXTICRy_EXTIx_Msk(x)     (0xF << ((x)*4))            //!< EXTI x configuration
#define SYSCFG_EXTICRy_EXTIx_Get(x,y)   (((y) >> ((x)*4)) & 0xF)    //!< EXTI x configuration get
#define SYSCFG_EXTICRy_EXTIx_Set(x,y)   ((y) << ((x)*4))            //!< EXTI x configuration set
/** @} */

/** @name SYSCFG_CFGR:      (syscfg Offset: 0x018) SYSCFG configuration registers */
/** @{ */
#define SYSCFG_CFGR_AXISRAML	      	0x00008000 //!< D1 AXI-SRAM double ECC error lock bit
#define SYSCFG_CFGR_ITCML		      	0x00004000 //!< D1 ITCM double ECC error lock bit
#define SYSCFG_CFGR_DTCML		      	0x00002000 //!< D1 DTCM double ECC error lock bit
#define SYSCFG_CFGR_SRAM1L		      	0x00001000 //!< D2 SRAM1 double ECC error lock bit
#define SYSCFG_CFGR_SRAM2L		      	0x00000800 //!< D2 SRAM2 double ECC error lock bit
#define SYSCFG_CFGR_SRAM3L		      	0x00000400 //!< D2 SRAM3 double ECC error lock bit
#define SYSCFG_CFGR_SRAM4L		      	0x00000200 //!< D3 SRAM4 double ECC error lock bit
#define SYSCFG_CFGR_BKRAML		      	0x00000080 //!< Backup SRAM double ECC error lock bit
#define SYSCFG_CFGR_CM7L		      	0x00000040 //!< Arm Cortex-M7 LOCKUP (HardFault) output enable bit
#define SYSCFG_CFGR_FLASHL		      	0x00000008 //!< FLASH double ECC error lock bit
#define SYSCFG_CFGR_PVDL		      	0x00000004 //!< PVD lock enable bit
/** @} */

/** @name SYSCFG_CCCSR:     (syscfg Offset: 0x020) SYSCFG compensation cell control/status register */
/** @{ */
#define SYSCFG_CCCSR_HSLV		      	0x00010000 //!< High-speed at low-voltage
#define SYSCFG_CCCSR_READY		      	0x00000100 //!< Compensation cell ready flag
#define SYSCFG_CCCSR_CS			      	0x00000002 //!< Code selection
#define SYSCFG_CCCSR_EN			      	0x00000001 //!< Enable I/O compensation cell
/** @} */

/** @name SYSCFG_CCVR:      (syscfg Offset: 0x024) SYSCFG compensation cell value register */
/** @{ */
#define SYSCFG_CCVR_PCV			      	0x000000F0 //!< PMOS compensation value
#define SYSCFG_CCVR_NCV			      	0x0000000F //!< NMOS compensation value
/** @} */

/** @name SYSCFG_CCCR:      (syscfg Offset: 0x028) SYSCFG compensation cell code register */
/** @{ */
#define SYSCFG_CCCR_PCC			      	0x000000F0 //!< PMOS compensation code
#define SYSCFG_CCCR_NCC			      	0x0000000F //!< NMOS compensation code
/** @} */

/** @name SYSCFG_PWRCR:     (syscfg Offset: 0x02C) PWR control register 	   */
/** @{ */
#define SYSCFG_PWRCR_ODEN		      	0x00000001//!< Overdrive enable, this bit allows to activate the LDO regulator overdrive mode
/** @} */

/** @name SYSCFG_PKGR:      (syscfg Offset: 0x124) SYSCFG package register	   */
/** @{ */
#define SYSCFG_PKGR_PKG			      	0x0000000F//!< Package
#define SYSCFG_PKGR_PKG_100			   	0x00000000//!<  LQFP100
#define SYSCFG_PKGR_PKG_144		      	0x00000002//!<  TQFP144
#define SYSCFG_PKGR_PKG_176		      	0x00000005//!<  TQFP176/UFBGA176
#define SYSCFG_PKGR_PKG_200		      	0x00000008//!<  LQFP208/TFBGA240
/** @} */

/** @name SYSCFG_UR0:       (syscfg Offset: 0x300) SYSCFG user register 0	   */
/** @{ */
#define SYSCFG_UR0_RDP			      	0x00FF0000//!< Readout protection
#define SYSCFG_UR0_BKS			      	0x00000001//!< Bank Swap
/** @} */

/** @name SYSCFG_UR1:       (syscfg Offset: 0x304) SYSCFG user register 1	   */
/** @{ */
/** @} */

/** @name SYSCFG_UR2:       (syscfg Offset: 0x308) SYSCFG user register 2	   */
/** @{ */
#define SYSCFG_UR2_BOOT_ADD0	      	0xFFFF0000//!< Boot Address 0
#define SYSCFG_UR2_BORH			      	0x00000003//!< BOR_LVL Brownout Reset Threshold Level
#define SYSCFG_UR2_BORH_off		      	0x00000000//!<  BOR OFF (Level 0)
#define SYSCFG_UR2_BORH_1		      	0x00000001//!<  BOR Level 1
#define SYSCFG_UR2_BORH_2		      	0x00000002//!<  BOR Level 2
#define SYSCFG_UR2_BORH_3		      	0x00000003//!<  BOR Level 3
/** @} */

/** @name SYSCFG_UR3:       (syscfg Offset: 0x30C) SYSCFG user register 3	   */
/** @{ */
#define SYSCFG_UR3_BOOT_ADD1	      	0x0000FFFF//!< Boot Address 1
/** @} */

/** @name SYSCFG_UR4:       (syscfg Offset: 0x310) SYSCFG user register 4	   */
/** @{ */
#define SYSCFG_UR4_MEPAD_1		      	0x00010000//!< Mass Erase Protected Area Disabled for bank 1
/** @} */

/** @name SYSCFG_UR5:       (syscfg Offset: 0x314) SYSCFG user register 5	   */
/** @{ */
#define SYSCFG_UR5_WRPN_1		      	0x00FF0000//!< Write protection for flash bank 1
#define SYSCFG_UR5_MESAD_1		      	0x00000001//!< Mass erase secured area disabled for bank 1
/** @} */

/** @name SYSCFG_UR6:       (syscfg Offset: 0x318) SYSCFG user register 6	   */
/** @{ */
#define SYSCFG_UR6_PA_END_1		      	0x0FFF0000//!< Protected area end address for bank 1
#define SYSCFG_UR6_PA_BEG_1		      	0x00000FFF//!< Protected area start address for bank 1
/** @} */

/** @name SYSCFG_UR7:       (syscfg Offset: 0x31C) SYSCFG user register 7	   */
/** @{ */
#define SYSCFG_UR7_SA_END_1		      	0x0FFF0000//!< Secured area end address for bank 1
#define SYSCFG_UR7_SA_BEG_1		      	0x00000FFF//!< Secured area start address for bank 1
/** @} */

/** @name SYSCFG_UR8:       (syscfg Offset: 0x320) SYSCFG user register 8	   */
/** @{ */
#define SYSCFG_UR8_MESAD_2		      	0x00010000//!< Mass erase secured area disabled for bank 2
#define SYSCFG_UR8_MEPAD_2		      	0x00000001//!< Mass erase protected area disabled for bank 2
/** @} */

/** @name SYSCFG_UR9:       (syscfg Offset: 0x324) SYSCFG user register 9	   */
/** @{ */
#define SYSCFG_UR9_PA_BEG_2		      	0x0FFF0000//!< Protected area start address for bank 2
#define SYSCFG_UR9_WRPN_2		      	0x000000FF//!< Write protection for flash bank 2
/** @} */

/** @name SYSCFG_UR10:      (syscfg Offset: 0x328) SYSCFG user register 10	   */
/** @{ */
#define SYSCFG_UR10_SA_BEG_2	      	0x0FFF0000//!< Secured area start address for bank 2
#define SYSCFG_UR10_PA_END_2	      	0x00000FFF//!< Protected area end address for bank 2
/** @} */

/** @name SYSCFG_UR11:      (syscfg Offset: 0x32C) SYSCFG user register 11	   */
/** @{ */
#define SYSCFG_UR11_IWDG1M		      	0x00010000//!< Independent Watchdog 1 mode
#define SYSCFG_UR11_SA_END_2	      	0x00000FFF//!< Secured area end address for bank 2
/** @} */

/** @name SYSCFG_UR12:      (syscfg Offset: 0x330) SYSCFG user register 12	   */
/** @{ */
#define SYSCFG_UR12_SECURE		      	0x00010000//!< Secure mode
/** @} */

/** @name SYSCFG_UR13:      (syscfg Offset: 0x334) SYSCFG user register 13	   */
/** @{ */
#define SYSCFG_UR13_D1SBRST		      	0x00010000//!< D1 Standby reset
#define SYSCFG_UR13_SDRS		      	0x00000003//!< Secured DTCM RAM Size
#define SYSCFG_UR13_SDRS_2k		      	0x00000000//!<  2 Kbytes
#define SYSCFG_UR13_SDRS_4k		      	0x00000001//!<  4 Kbytes
#define SYSCFG_UR13_SDRS_8k		      	0x00000002//!<  8 Kbytes
#define SYSCFG_UR13_SDRS_16k	      	0x00000003//!<  16 Kbytes
/** @} */

/** @name SYSCFG_UR14:      (syscfg Offset: 0x338) SYSCFG user register 14	   */
/** @{ */
#define SYSCFG_UR14_D1STPRST	      	0x00000001//!< D1 Stop Reset
/** @} */

/** @name SYSCFG_UR15:      (syscfg Offset: 0x33C) SYSCFG user register 15	   */
/** @{ */
#define SYSCFG_UR15_FZIWDGSTB	      	0x00010000//!< Freeze independent watchdog in Standby mode
/** @} */

/** @name SYSCFG_UR16:      (syscfg Offset: 0x340) SYSCFG user register 16	   */
/** @{ */
#define SYSCFG_UR16_PKP			      	0x00010000//!< Private key programmed
#define SYSCFG_UR16_FZIWDGSTP	      	0x00000001//!< Freeze independent watchdog in Stop mode
/** @} */

/** @name SYSCFG_UR17:      (syscfg Offset: 0x344) SYSCFG user register 17	   */
/** @{ */
#define SYSCFG_UR17_IO_HSLV		      	0x00000001//!< I/O high speed / low voltage
/** @} */


/** @} */ // @relates SYSCFG_TypeDef

/// Return IRqn for exti line
static inline uint32_t stm_get_drv_indx(uint32_t line)
{
	if(line < 5)
		line = EXTI0_IRQn + line;
	else
	{
		if(line <10)
			line = EXTI9_5_IRQn;
		else
			line = EXTI15_10_IRQn;
	}
	return line;
}

void PIO_Cfg(PIN_DESC cfg);
void PIO_CfgOutput1(PIN_DESC pins);
void PIO_CfgOutput0(PIN_DESC pins);
void PIO_CfgInput(PIN_DESC pins);
void PIO_Cfg_List(const PIN_DESC* list);
void PIO_CfgInput_List(PIN_DESC * list);
void PIO_Free(PIN_DESC cfg);
void PIO_Free_List(const PIN_DESC* list);

pio_set PIO_Read(PIN_DESC pins);
void PIO_Write(PIN_DESC pins, unsigned int val);
void PIO_SetOutput(PIN_DESC pins);
void PIO_ClrOutput(PIN_DESC pins);
void PIO_Assert(PIN_DESC pins);
void PIO_Deassert(PIN_DESC pins);

void exti_set_line_source(unsigned int line, unsigned int port);

// Virtual PIO functions which must be implemented from the application
/**
 * Write to virtual port
 * @note must be implemented from the application
 * @param pio
 * @param val
 */
void VPIO_Write(pio_def pio, unsigned int val );
/**
 * Read from virtual port
 * @note must be implemented from the application
 * @param pio
 * @return
 */
pio_set VPIO_Read(pio_def pio);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H7_H_ */

 /** @} */
