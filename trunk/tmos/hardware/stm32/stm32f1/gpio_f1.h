/**
 * @ingroup	 hardware_stm32f1
 * @defgroup  STM32F1_GPIO General-purpose I/Os (GPIO)
 * Interface for GPIO
 * @{
 *
 * @file     hardware/stm32/cortex-m3/stm32f1/gpio.h
 * @ingroup	 STM32F1_GPIO
 * @brief    GPIO header
 *
 * @}
*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include <tmos_types.h>
#include <gpio_stm32.h>

/*------------------------------------------------------------------------------
 *         Definitions
 *----------------------------------------------------------------------------*/


#ifdef __cplusplus
 extern "C" {
#endif



#ifndef __ASSEMBLY__

/*******************************************************************************
 *  GPIO hardware registers
 ******************************************************************************/
typedef struct {
	__IO uint32_t GPIO_CRL;    //!< (gpio Offset: 0x0000) Port configuration register low
	__IO uint32_t GPIO_CRH;    //!< (gpio Offset: 0x0004) Port configuration register high
	__IO uint32_t GPIO_IDR;    //!< (gpio Offset: 0x0008) Port input data register
	__IO uint32_t GPIO_ODR;    //!< (gpio Offset: 0x000C) Port output data register
	__IO uint32_t GPIO_BSRR;   //!< (gpio Offset: 0x0010) Port bit set/reset register
	__IO uint32_t GPIO_BRR;    //!< (gpio Offset: 0x0014) Port bit reset register
	__IO uint32_t GPIO_LCKR;   //!< (gpio Offset: 0x0018) ort configuration lock register

} GPIO_TypeDef;
#endif /* __ASSEMBLY__ */

/*******************************************************************************
 * @defgroup GPIO_pins_define
 * @{
 ******************************************************************************/


/** @defgroup GPIO_CRL: (gpio Offset: 0x0000) Port configuration register low */
#define GPIO_CRL_MODEx_Msk(x)    (3<<((x)*4))       //!< Port x mode bits mask
#define GPIO_CRL_MODEx_Set(x,y)  ((y)<<((x)*4))     //!< Port x mode bits Set
#define GPIO_CRL_MODEx_Get(x,y)  (((y)>>((x)*4))&3) //!< Port x mode bits Get

#define GPIO_CRL_CNFx_Msk(x)     (0xC<<((x)*4))     //!< Port x configuration bits
#define GPIO_CRL_CNFx_Set(x,y)   ((y)<<((x+2)*4))   //!< Port x configuration bits Set
#define GPIO_CRL_CNFx_Get(x,y)   (((y)>>((x+2)*4))&3) //!< Port x configuration bits Get
/** @} */

/** @defgroup GPIO_CRH: (gpio Offset: 0x0004) Port configuration register high*/
#define GPIO_CRH_MODEx_Msk(x)    (3<<((x-8)*4))       //!< Port x mode bits mask
#define GPIO_CRH_MODEx_Set(x,y)  ((y)<<((x-8)*4))     //!< Port x mode bits Set
#define GPIO_CRH_MODEx_Get(x,y)  (((y)>>((x-8)*4))&3) //!< Port x mode bits Get

#define GPIO_CRH_CNFx_Msk(x)     (0xC<<((x-8)*4))     //!< Port x configuration bits
#define GPIO_CRH_CNFx_Set(x,y)   ((y)<<((x-6)*4))     //!< Port x configuration bits Set
#define GPIO_CRH_CNFx_Get(x,y)   (((y)>>((x-6)*4))&3) //!< Port x configuration bits Get
/** @} */

#define GPIO_CRx_MODE_INPUT		 0			//!< MODEy - Input mode (reset state)
#define GPIO_CRx_MODE_OUT_10MHz  1			//!< MODEy - Output mode, max speed 10 MHz
#define GPIO_CRx_MODE_OUT_2MHz   2			//!< MODEy - Output mode, max speed 2 MHz
#define GPIO_CRx_MODE_OUT_50MHz  3			//!< MODEy - Output mode, max speed 50 MHz

#define GPIO_CRx_CNF_IN_AN 		 0			//!< CNFx (IN mode) - Analog mode
#define GPIO_CRx_CNF_IN_GP 		 4			//!< CNFx (IN mode) - Floating input (reset state)
#define GPIO_CRx_CNF_IN_PUPD	 8			//!< CNFx (IN mode) - Input with pull-up / pull-down

#define GPIO_CRx_CNF_OUT_GP_PP	 0			//!< CNFx (OUT mode)- General purpose output push-pull
#define GPIO_CRx_CNF_OUT_GP_OD	 4			//!< CNFx (OUT mode)- General purpose output Open-drain
#define GPIO_CRx_CNF_OUT_AF_PP	 8			//!< CNFx (OUT mode)- Alternate function output Push-pull
#define GPIO_CRx_CNF_OUT_AF_OD	 12			//!< CNFx (OUT mode)- Alternate function output Open-drain

/** @defgroup GPIO_IDR: (gpio Offset: 0x0008) Port input data register        */
#define GPIO_IDR_IDR0      0x0001            //!< Port input data, bit 0
#define GPIO_IDR_IDR1      0x0002            //!< Port input data, bit 1
#define GPIO_IDR_IDR2      0x0004            //!< Port input data, bit 2
#define GPIO_IDR_IDR3      0x0008            //!< Port input data, bit 3
#define GPIO_IDR_IDR4      0x0010            //!< Port input data, bit 4
#define GPIO_IDR_IDR5      0x0020            //!< Port input data, bit 5
#define GPIO_IDR_IDR6      0x0040            //!< Port input data, bit 6
#define GPIO_IDR_IDR7      0x0080            //!< Port input data, bit 7
#define GPIO_IDR_IDR8      0x0100            //!< Port input data, bit 8
#define GPIO_IDR_IDR9      0x0200            //!< Port input data, bit 9
#define GPIO_IDR_IDR10     0x0400            //!< Port input data, bit 10
#define GPIO_IDR_IDR11     0x0800            //!< Port input data, bit 11
#define GPIO_IDR_IDR12     0x1000            //!< Port input data, bit 12
#define GPIO_IDR_IDR13     0x2000            //!< Port input data, bit 13
#define GPIO_IDR_IDR14     0x4000            //!< Port input data, bit 14
#define GPIO_IDR_IDR15     0x8000            //!< Port input data, bit 15
/** @} */

/** @defgroup GPIO_ODR: (gpio Offset: 0x000C) Port output data register       */
#define GPIO_ODR_ODR0      0x0001            //!< Port output data, bit 0
#define GPIO_ODR_ODR1      0x0002            //!< Port output data, bit 1
#define GPIO_ODR_ODR2      0x0004            //!< Port output data, bit 2
#define GPIO_ODR_ODR3      0x0008            //!< Port output data, bit 3
#define GPIO_ODR_ODR4      0x0010            //!< Port output data, bit 4
#define GPIO_ODR_ODR5      0x0020            //!< Port output data, bit 5
#define GPIO_ODR_ODR6      0x0040            //!< Port output data, bit 6
#define GPIO_ODR_ODR7      0x0080            //!< Port output data, bit 7
#define GPIO_ODR_ODR8      0x0100            //!< Port output data, bit 8
#define GPIO_ODR_ODR9      0x0200            //!< Port output data, bit 9
#define GPIO_ODR_ODR10     0x0400            //!< Port output data, bit 10
#define GPIO_ODR_ODR11     0x0800            //!< Port output data, bit 11
#define GPIO_ODR_ODR12     0x1000            //!< Port output data, bit 12
#define GPIO_ODR_ODR13     0x2000            //!< Port output data, bit 13
#define GPIO_ODR_ODR14     0x4000            //!< Port output data, bit 14
#define GPIO_ODR_ODR15     0x8000            //!< Port output data, bit 15
/** @} */

/** @defgroup GPIO_BSRR:(gpio Offset: 0x0010) Port bit set/reset register     */
#define GPIO_BSRR_BS0      0x00000001        //!< Port x Set bit 0
#define GPIO_BSRR_BS1      0x00000002        //!< Port x Set bit 1
#define GPIO_BSRR_BS2      0x00000004        //!< Port x Set bit 2
#define GPIO_BSRR_BS3      0x00000008        //!< Port x Set bit 3
#define GPIO_BSRR_BS4      0x00000010        //!< Port x Set bit 4
#define GPIO_BSRR_BS5      0x00000020        //!< Port x Set bit 5
#define GPIO_BSRR_BS6      0x00000040        //!< Port x Set bit 6
#define GPIO_BSRR_BS7      0x00000080        //!< Port x Set bit 7
#define GPIO_BSRR_BS8      0x00000100        //!< Port x Set bit 8
#define GPIO_BSRR_BS9      0x00000200        //!< Port x Set bit 9
#define GPIO_BSRR_BS10     0x00000400        //!< Port x Set bit 10
#define GPIO_BSRR_BS11     0x00000800        //!< Port x Set bit 11
#define GPIO_BSRR_BS12     0x00001000        //!< Port x Set bit 12
#define GPIO_BSRR_BS13     0x00002000        //!< Port x Set bit 13
#define GPIO_BSRR_BS14     0x00004000        //!< Port x Set bit 14
#define GPIO_BSRR_BS15     0x00008000        //!< Port x Set bit 15

#define GPIO_BSRR_BR0      0x00010000        //!< Port x Reset bit 0
#define GPIO_BSRR_BR1      0x00020000        //!< Port x Reset bit 1
#define GPIO_BSRR_BR2      0x00040000        //!< Port x Reset bit 2
#define GPIO_BSRR_BR3      0x00080000        //!< Port x Reset bit 3
#define GPIO_BSRR_BR4      0x00100000        //!< Port x Reset bit 4
#define GPIO_BSRR_BR5      0x00200000        //!< Port x Reset bit 5
#define GPIO_BSRR_BR6      0x00400000        //!< Port x Reset bit 6
#define GPIO_BSRR_BR7      0x00800000        //!< Port x Reset bit 7
#define GPIO_BSRR_BR8      0x01000000        //!< Port x Reset bit 8
#define GPIO_BSRR_BR9      0x02000000        //!< Port x Reset bit 9
#define GPIO_BSRR_BR10     0x04000000        //!< Port x Reset bit 10
#define GPIO_BSRR_BR11     0x08000000        //!< Port x Reset bit 11
#define GPIO_BSRR_BR12     0x10000000        //!< Port x Reset bit 12
#define GPIO_BSRR_BR13     0x20000000        //!< Port x Reset bit 13
#define GPIO_BSRR_BR14     0x40000000        //!< Port x Reset bit 14
#define GPIO_BSRR_BR15     0x80000000        //!< Port x Reset bit 15
/** @} */

/** @defgroup GPIO_BRR: (gpio Offset: 0x0014) Port bit reset register         */
#define GPIO_BRR_BR0       0x0001            //!< Port x Reset bit 0
#define GPIO_BRR_BR1       0x0002            //!< Port x Reset bit 1
#define GPIO_BRR_BR2       0x0004            //!< Port x Reset bit 2
#define GPIO_BRR_BR3       0x0008            //!< Port x Reset bit 3
#define GPIO_BRR_BR4       0x0010            //!< Port x Reset bit 4
#define GPIO_BRR_BR5       0x0020            //!< Port x Reset bit 5
#define GPIO_BRR_BR6       0x0040            //!< Port x Reset bit 6
#define GPIO_BRR_BR7       0x0080            //!< Port x Reset bit 7
#define GPIO_BRR_BR8       0x0100            //!< Port x Reset bit 8
#define GPIO_BRR_BR9       0x0200            //!< Port x Reset bit 9
#define GPIO_BRR_BR10      0x0400            //!< Port x Reset bit 10
#define GPIO_BRR_BR11      0x0800            //!< Port x Reset bit 11
#define GPIO_BRR_BR12      0x1000            //!< Port x Reset bit 12
#define GPIO_BRR_BR13      0x2000            //!< Port x Reset bit 13
#define GPIO_BRR_BR14      0x4000            //!< Port x Reset bit 14
#define GPIO_BRR_BR15      0x8000            //!< Port x Reset bit 15
/** @} */

/** @defgroup GPIO_LCKR:(gpio Offset: 0x0018) ort configuration lock register */
#define GPIO_LCKR_LCK0     0x00000001        //!< Port x Lock bit 0
#define GPIO_LCKR_LCK1     0x00000002        //!< Port x Lock bit 1
#define GPIO_LCKR_LCK2     0x00000004        //!< Port x Lock bit 2
#define GPIO_LCKR_LCK3     0x00000008        //!< Port x Lock bit 3
#define GPIO_LCKR_LCK4     0x00000010        //!< Port x Lock bit 4
#define GPIO_LCKR_LCK5     0x00000020        //!< Port x Lock bit 5
#define GPIO_LCKR_LCK6     0x00000040        //!< Port x Lock bit 6
#define GPIO_LCKR_LCK7     0x00000080        //!< Port x Lock bit 7
#define GPIO_LCKR_LCK8     0x00000100        //!< Port x Lock bit 8
#define GPIO_LCKR_LCK9     0x00000200        //!< Port x Lock bit 9
#define GPIO_LCKR_LCK10    0x00000400        //!< Port x Lock bit 10
#define GPIO_LCKR_LCK11    0x00000800        //!< Port x Lock bit 11
#define GPIO_LCKR_LCK12    0x00001000        //!< Port x Lock bit 12
#define GPIO_LCKR_LCK13    0x00002000        //!< Port x Lock bit 13
#define GPIO_LCKR_LCK14    0x00004000        //!< Port x Lock bit 14
#define GPIO_LCKR_LCK15    0x00008000        //!< Port x Lock bit 15
#define GPIO_LCKR_LCKK     0x00010000        //!< Lock key */
/** @} */

/** @} */ // @defgroup GPIO_pins_define

/// Calculate port address for a given pin definition
#define PORT_ADDRESS(num) ((GPIO_TypeDef*)(GPIOA_BASE + ((num)*0x400)))


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

#endif /* #ifndef _GPIO_H_ */

