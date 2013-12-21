/*
 * gpio_f2.h
 *
 *  Created on: Oct 29, 2012
 *      Author: miro
 */

#ifndef GPIO_F2_H_
#define GPIO_F2_H_

#include <mcu_inc.h>
#include <tmos_types.h>
#include <gpio_stm32.h>

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

#define PD_AF_SYS 		PD_AF0			//!< Alternate function map for SYS
#define PD_AF_TIM1 		PD_AF1			//!< Alternate function map for Timer 1
#define PD_AF_TIM2 		PD_AF1			//!< Alternate function map for Timer 2
#define PD_AF_TIM3 		PD_AF2			//!< Alternate function map for Timer 3
#define PD_AF_TIM4 		PD_AF2			//!< Alternate function map for Timer 4
#define PD_AF_TIM5 		PD_AF2			//!< Alternate function map for Timer 5
#define PD_AF_TIM8 		PD_AF3			//!< Alternate function map for Timer 8
#define PD_AF_TIM9 		PD_AF3			//!< Alternate function map for Timer 9
#define PD_AF_TIM10		PD_AF3			//!< Alternate function map for Timer 10
#define PD_AF_TIM11		PD_AF3			//!< Alternate function map for Timer 11
#define PD_AF_I2C1 		PD_AF4			//!< Alternate function map for I2C1
#define PD_AF_I2C2 		PD_AF4			//!< Alternate function map for I2C2
#define PD_AF_I2C3 		PD_AF4			//!< Alternate function map for I2C3
#define PD_AF_SPI1 		PD_AF5			//!< Alternate function map for SPI1
#define PD_AF_SPI2 		PD_AF5			//!< Alternate function map for SPI2
#define PD_AF_I2S2 		PD_AF5			//!< Alternate function map for I2S2
#define PD_AF_SPI3 		PD_AF6			//!< Alternate function map for SPI3
#define PD_AF_I2S3 		PD_AF6			//!< Alternate function map for I2S3
#define PD_AF_USART1	PD_AF7			//!< Alternate function map for USART1
#define PD_AF_USART2	PD_AF7			//!< Alternate function map for USART2
#define PD_AF_USART3	PD_AF7			//!< Alternate function map for USART3
#define PD_AF_USART6	PD_AF8			//!< Alternate function map for USART6
#define PD_AF_UART4		PD_AF8			//!< Alternate function map for UART4
#define PD_AF_UART5		PD_AF8			//!< Alternate function map for UART5
#define PD_AF_CAN1		PD_AF9			//!< Alternate function map for CAN1
#define PD_AF_CAN2		PD_AF9			//!< Alternate function map for CAN2
#define PD_AF_TIM12		PD_AF9			//!< Alternate function map for Timer 12
#define PD_AF_TIM13		PD_AF9			//!< Alternate function map for Timer 13
#define PD_AF_TIM14 	PD_AF9			//!< Alternate function map for Timer 14
#define PD_AF_OTG_HS	PD_AF10			//!< Alternate function map for OTG_HS
#define PD_AF_ETH		PD_AF11			//!< Alternate function map for ETH
#define PD_AF_FSMC		PD_AF12			//!< Alternate function map for FSMC
#define PD_AF_SDIO		PD_AF12			//!< Alternate function map for SDIO
#define PD_AF_DCMI		PD_AF13			//!< Alternate function map for DCMI

#ifdef __cplusplus
 extern "C" {
#endif

/*******************************************************************************
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

/*******************************************************************************
 * @defgroup GPIO_regs_define
 * @{
 ******************************************************************************/


/** @defgroup GPIO_MODER:	(gpio Offset: 0x00) GPIO port mode register			*/
#define GPIO_MODER_MODERy_Msk(pin)		(3<<((pin)*2))       //!< Port x configuration bits (y = 0..15)
#define GPIO_MODER_MODERy_Get(pin,x)  	(((x)>>((pin)*2))&3)
#define GPIO_MODER_MODERy_Set(pin,x)    ((x)<<((pin)*2))

#define GPIO_MODER_MODERy_INPUT			0		//!< input (reset state)
#define GPIO_MODER_MODERy_OUTPUT		1		//!< general purpose output mode
#define GPIO_MODER_MODERy_AF			2		//!< alternative function mode
#define GPIO_MODER_MODERy_ANALOG		3		//!< analog mode


/** @} */

/** @defgroup GPIO_OTYPER:	(gpio Offset: 0x04) GPIO port output type register	*/
#define GPIO_OTYPER_OTy				0x0000FFFF	//!< Port x configuration bits (y = 0..15)
#define GPIO_OTYPER_OTy_PP			0			//!< Output push-pull (reset state)
#define GPIO_OTYPER_OTy_OD			1			//!< Output open-drain
/** @} */

/** @defgroup GPIO_OSPEEDR:	(gpio Offset: 0x08) GPIO port output speed register */
#define GPIO_OSPEEDER_OSPEEDRy_Msk(pin)		(3<<((pin)*2))       //!< OSPEEDRy[1:0]: Port x configuration bits (y = 0..15)
#define GPIO_OSPEEDER_OSPEEDRy_Get(pin,x)  	(((x)>>((pin)*2))&3)
#define GPIO_OSPEEDER_OSPEEDRy_Set(pin,x)   ((x)<<((pin)*2))

#define GPIO_OSPEEDER_OSPEEDRy_2MHz   	0		//!< 2 MHz Low speed
#define GPIO_OSPEEDER_OSPEEDRy_25MHz   	1		//!< 25 MHz Medium speed
#define GPIO_OSPEEDER_OSPEEDRy_50MHz   	2		//!< 50 MHz Fast speed
#define GPIO_OSPEEDER_OSPEEDRy_100MHz  	3		//!< 100 MHz High speed on 30 pF (80 MHz Output max speed on 15 pF)
/** @} */

/** @defgroup GPIO_PUPDR:	(gpio Offset: 0x0C) GPIO port pull-up/pull-down register */
#define GPIO_PUPDR_PUPDRy_Msk(pin)		(3<<((pin)*2))    //!< PUPDRy[1:0]: Port x configuration bits (y = 0..15)
#define GPIO_PUPDR_PUPDRy_Get(pin,x)  	(((x)>>((pin)*2))&3)
#define GPIO_PUPDR_PUPDRy_Set(pin,x)   ((x)<<((pin)*2))

#define GPIO_PUPDR_PUPDRy_NONE   	0		//!< No pull-up, pull-down
#define GPIO_PUPDR_PUPDRy_PU   		1		//!< Pull-up
#define GPIO_PUPDR_PUPDRy_PD   		2		//!< Pull-down

/** @} */


/** @defgroup GPIO_BSRR:	(gpio Offset: 0x18) GPIO port bit set/reset register */
#define GPIO_BSRR_BS(x)				x			//!< set macro
#define GPIO_BSRR_BR(x)				((x)<<16)	//!< reset macro
/** @} */

/** @defgroup GPIO_LCKR:	(gpio Offset: 0x1C) GPIO port configuration lock register */
#define GPIO_LCKR_LCKy				0x0000FFFF	//!< Port x configuration bits (y = 0..15)
#define GPIO_LCKR_LCKK				0x00010000	//!< Lock key
/** @} */

/** @defgroup GPIO_AFR[2]:	(gpio Offset: 0x20) GPIO alternate function registers */
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


/** @} */ // @defgroup GPIO_regs_define

/*******************************************************************************
 *  System configuration controller
 ******************************************************************************/
typedef struct
{
  __IO uint32_t SYSCFG_MEMRMP;      //!< (syscfg Offset: 0x00) SYSCFG memory remap register
  __IO uint32_t SYSCFG_PMC;         //!< (syscfg Offset: 0x04) SYSCFG peripheral mode configuration register
  __IO uint32_t SYSCFG_EXTICR[4];   //!< (syscfg Offset: 0x08) SYSCFG external interrupt configuration registers
  __IO uint32_t SYSCFG_RESERVED[2]; //!< (syscfg Offset: 0x18) Reserved
  __IO uint32_t SYSCFG_CMPCR;       //!< (syscfg Offset: 0x20) SYSCFG Compensation cell control register
} SYSCFG_TypeDef;

/*******************************************************************************
 * @defgroup SYSCFG_pins_define
 * @{
 ******************************************************************************/

/** @defgroup SYSCFG_MEMRMP:    (syscfg Offset: 0x00) SYSCFG memory remap register */
#define SYSCFG_MEMRMP_MEM_MODE      	0x00000003 //!< <SYSCFG_Memory Remap Config
#define SYSCFG_MEMRMP_MEM_MODE_FLASH    0x00000000 //!<  Main Flash memory mapped
#define SYSCFG_MEMRMP_MEM_MODE_ROM    	0x00000001 //!<  System Flash memory mapped
#define SYSCFG_MEMRMP_MEM_MODE_FSMC    	0x00000002 //!<  FSMC Bank1 (NOR/PSRAM 1 and 2)
#define SYSCFG_MEMRMP_MEM_MODE_SRAM    	0x00000003 //!<  Embedded SRAM (112kB) mapped
/** @} */

/** @defgroup SYSCFG_PMC:       (syscfg Offset: 0x04) SYSCFG peripheral mode configuration register */
#define SYSCFG_PMC_MII_RMII         	0x0080 //!< Ethernet PHY interface selection
/** @} */

/** @defgroup SYSCFG_EXTICR[4]: (syscfg Offset: 0x08) SYSCFG external interrupt configuration registers */
#define SYSCFG_EXTICRy_EXTIx_Msk(x)     (0xF << ((x)*4))            //!< EXTI x configuration
#define SYSCFG_EXTICRy_EXTIx_Get(x,y)   (((y) >> ((x)*4)) & 0xF)    //!< EXTI x configuration get
#define SYSCFG_EXTICRy_EXTIx_Set(x,y)   ((y) << ((x)*4))            //!< EXTI x configuration set
/** @} */

/** @defgroup SYSCFG_CMPCR:     (syscfg Offset: 0x20) SYSCFG Compensation cell control register */
#define SYSCFG_CMPCR_CMP_PD         0x00000001 //!< Compensation cell ready flag
#define SYSCFG_CMPCR_READY          0x00000100 //!< Compensation cell power-down
/** @} */


/** @} */ // @defgroup SYSCFG_pins_define

/// Calculate port address for a given pin definition
#define PORT_ADDRESS(num) ((GPIO_TypeDef*)(BASE_GPIOA + ((num)*0x400)))


void PIO_Cfg(PIN_DESC cfg);
void PIO_CfgOutput1(PIN_DESC pins);
void PIO_CfgOutput0(PIN_DESC pins);
void PIO_CfgInput(PIN_DESC pins);
void PIO_Cfg_List(const PIN_DESC* list);
void PIO_CfgInput_List(PIN_DESC * list);
void PIO_Free(PIN_DESC cfg);
void PIO_Free_List(PIN_DESC* list);

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

#endif /* GPIO_F2_H_ */
