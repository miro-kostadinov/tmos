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


#ifdef __cplusplus
extern "C"
{
#endif

void PIO_Cfg(PIN_DESC cfg);
void PIO_CfgOutput1(PIN_DESC pins);
void PIO_CfgOutput0(PIN_DESC pins);
void PIO_CfgInput(PIN_DESC pins);
void PIO_Cfg_List(PIN_DESC * list);
void PIO_CfgInput_List(PIN_DESC * list);
void PIO_Free(PIN_DESC cfg);
void PIO_Free_List(PIN_DESC* list);

pio_set PIO_Read(PIN_DESC pins);
void PIO_Write(PIN_DESC pins, unsigned int val);
void PIO_SetOutput(PIN_DESC pins);
void PIO_ClrOutput(PIN_DESC pins);
void PIO_Assert(PIN_DESC pins);
void PIO_Deassert(PIN_DESC pins);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_F2_H_ */
