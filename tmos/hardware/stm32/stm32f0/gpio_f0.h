/**************************************************************************//**
 * @ingroup	hardware_stm32f0_gpio
 * @file	hardware/stm32/stm32f0/gpio_f0.h
 * @brief  	STM32F0 GPIO
 * @date    27. October 2014
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f0_gpio General-purpose I/Os (GPIO)
 * @ingroup	 hardware_stm32f0
 * Source and definitions for STM32F0 General-purpose I/Os (GPIO)
 * @{
 *
 ******************************************************************************/

#ifndef GPIO_F0_H_
#define GPIO_F0_H_

#include <mcu_inc.h>
#include <tmos_types.h>
#include <gpio_stm32.h>

#define PD_LAST_PORT		5				//!< number of physical ports

#define PD_AF0		(PD_AF | PD_MUX_AF0	)	//!< Alternative function map 0
#define PD_AF1		(PD_AF | PD_MUX_AF1	)	//!< Alternative function map 1
#define PD_AF2		(PD_AF | PD_MUX_AF2	)	//!< Alternative function map 2
#define PD_AF3		(PD_AF | PD_MUX_AF3	)	//!< Alternative function map 3
#define PD_AF4		(PD_AF | PD_MUX_AF4	)	//!< Alternative function map 4
#define PD_AF5		(PD_AF | PD_MUX_AF5	)	//!< Alternative function map 5
#define PD_AF6		(PD_AF | PD_MUX_AF6	)	//!< Alternative function map 6
#define PD_AF7		(PD_AF | PD_MUX_AF7	)	//!< Alternative function map 7

#define PD_AF_SPI1 		PD_AF0			//!< Alternate function map for SPI1
#define PD_AF_SPI2 		PD_AF0			//!< Alternate function map for SPI2
#define PD_AF_SDIO		PD_AF0			//!< Alternate function map for SDIO
#define PD_AF_USART2	PD_AF1			//!< Alternate function map for USART2

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
	__IO uint32_t GPIO_BRR;		 //!< (gpio Offset: 0x28) Port bit reset register
} GPIO_TypeDef;

/*******************************************************************************
 * @relates GPIO_TypeDef
 * @{
 ******************************************************************************/


/** @name GPIO_MODER:	(gpio Offset: 0x00) GPIO port mode register		  	  */
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
#define GPIO_OSPEEDER_OSPEEDRy_10MHz   	1		//!< 10 MHz Medium speed
#define GPIO_OSPEEDER_OSPEEDRy_50MHz   	3		//!< 50 MHz High speed
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
/** @} */


/** @} */ // @relates GPIO_TypeDef

/***************************************************************************//**
 *  System configuration controller
 ******************************************************************************/
typedef struct
{
  __IO uint32_t SYSCFG_CFGR1; 	    //!< (syscfg Offset: 0x00) SYSCFG configuration register 1
  __IO uint32_t reserved; 		    //!< (syscfg Offset: 0x04)
  __IO uint32_t SYSCFG_EXTICR[4];   //!< (syscfg Offset: 0x08) SYSCFG external interrupt configuration registers
  __IO uint32_t SYSCFG_CFGR2; 	    //!< (syscfg Offset: 0x18) SYSCFG configuration register 2
} SYSCFG_TypeDef;

/*******************************************************************************
 * @relates SYSCFG_TypeDef
 * @{
 ******************************************************************************/

/** @name SYSCFG_CFGR1:    (syscfg Offset: 0x00) SYSCFG configuration register 1 */
/** @{ */
#define SYSCFG_CFGR1_MEM_MODE     	 	0x00000003 //!< SYSCFG_Memory Remap Config
#define SYSCFG_CFGR1_MEM_MODE_FLASH     0x00000000 //!<  Main Flash memory mapped
#define SYSCFG_CFGR1_MEM_MODE_ROM    	0x00000001 //!<  System Flash memory mapped
#define SYSCFG_CFGR1_MEM_MODE_FSMC    	0x00000002 //!<  FSMC Bank1 (NOR/PSRAM 1 and 2)
#define SYSCFG_CFGR1_MEM_MODE_SRAM    	0x00000003 //!<  Embedded SRAM (112kB) mapped
/** @} */


/** @name SYSCFG_EXTICR[4]: (syscfg Offset: 0x08) SYSCFG external interrupt configuration registers */
/** @{ */
#define SYSCFG_EXTICRy_EXTIx_Msk(x)     (0xF << ((x)*4))            //!< EXTI x configuration
#define SYSCFG_EXTICRy_EXTIx_Get(x,y)   (((y) >> ((x)*4)) & 0xF)    //!< EXTI x configuration get
#define SYSCFG_EXTICRy_EXTIx_Set(x,y)   ((y) << ((x)*4))            //!< EXTI x configuration set
/** @} */

/** @name SYSCFG_CFGR2:     (syscfg Offset: 0x18) SYSCFG configuration register 2 */
/** @{ */
/** @} */


/** @} */ // @relates SYSCFG_TypeDef

/// Calculate port address for a given pin definition
#define PORT_ADDRESS(num) ((GPIO_TypeDef*)(BASE_GPIOA + ((num)*0x400)))

/// Return IRqn for exti line
#define stm_get_drv_indx(line) (((line)<4)?(EXTI0_1_IRQn + ((line)>>1)):EXTI4_15_IRQn)

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


#ifdef __cplusplus
}
#endif




#endif /* GPIO_F0_H_ */

/** @} */
