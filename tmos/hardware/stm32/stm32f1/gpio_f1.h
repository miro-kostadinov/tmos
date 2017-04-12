/**************************************************************************//**
 * @ingroup	hardware_stm32f1_gpio
 * @file	hardware/stm32/stm32f1/gpio_f1.h
 * @brief  	STM32F1 GPIO
 * @date    7. November 2012
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f1_gpio  GPIO General-purpose I/Os (GPIO)
 * @ingroup	 hardware_stm32f1
 * Source and definitions for STM32F1 _GPIO General-purpose I/Os (GPIO)
 * @{
 *
 ******************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_

#include <tmos_types.h>
#include <gpio_stm32.h>

#define PD_LAST_PORT		6				//!< number of physical ports

/*------------------------------------------------------------------------------
 *         Definitions
 *----------------------------------------------------------------------------*/


#ifdef __cplusplus
 extern "C" {
#endif



/***************************************************************************//**
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

/***************************************************************************//**
 * @relates GPIO_TypeDef
 * @{
 ******************************************************************************/


/** @name GPIO_CRL: (gpio Offset: 0x0000) Port configuration register low 	  */
/** @{ */
#define GPIO_CRL_MODEx_Msk(x)    (3<<((x)*4))       //!< Port x mode bits mask
#define GPIO_CRL_MODEx_Set(x,y)  ((y)<<((x)*4))     //!< Port x mode bits Set
#define GPIO_CRL_MODEx_Get(x,y)  (((y)>>((x)*4))&3) //!< Port x mode bits Get

#define GPIO_CRL_CNFx_Msk(x)     (0xC<<((x)*4))     //!< Port x configuration bits
#define GPIO_CRL_CNFx_Set(x,y)   ((y)<<((x+2)*4))   //!< Port x configuration bits Set
#define GPIO_CRL_CNFx_Get(x,y)   (((y)>>((x+2)*4))&3) //!< Port x configuration bits Get
/** @} */

/** @name GPIO_CRH: (gpio Offset: 0x0004) Port configuration register high	  */
/** @{ */
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

/** @name GPIO_IDR: (gpio Offset: 0x0008) Port input data register 			  */
/** @{ */
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

/** @name GPIO_ODR: (gpio Offset: 0x000C) Port output data register     	  */
/** @{ */
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

/** @name GPIO_BSRR:(gpio Offset: 0x0010) Port bit set/reset register 	      */
/** @{ */
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

/** @name GPIO_BRR: (gpio Offset: 0x0014) Port bit reset register    	      */
/** @{ */
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

/** @name GPIO_LCKR:(gpio Offset: 0x0018) ort configuration lock register	  */
/** @{ */
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

/** @} */ // @relates GPIO_TypeDef

/***************************************************************************//**
 *  Alternate Function I/O (AFIO) registers
 ******************************************************************************/
typedef struct
{
	__IO uint32_t AFIO_EVCR;		//!< (afio Offset: 0x00) Event control register
	__IO uint32_t AFIO_MAPR;		//!< (afio Offset: 0x04) AF remap and debug I/O configuration register
	__IO uint32_t AFIO_EXTICR[4];	//!< (afio Offset: 0x08) External interrupt configuration registers
	__IO uint32_t AFIO_RESERVED0;	//!< (afio Offset: 0x18) reserved
	__IO uint32_t AFIO_MAPR2;  		//!< (afio Offset: 0x1C) AF remap and debug I/O configuration register2
} AFIO_TypeDef;

/***************************************************************************//**
 * @relates AFIO_TypeDef
 * @{
 ******************************************************************************/

/** @name AFIO_EVCR:		(afio Offset: 0x00) Event control register 		  */
/** @{ */
/*!< PIN configuration */
#define AFIO_EVCR_PIN               0x0000000F //!< PIN[3:0] bits (Pin selection)
#define AFIO_EVCR_PIN_Msk           0x0000000F //!< Pin selection mask
#define AFIO_EVCR_PIN_Set(x)        (x)        //!< Pin selection set
#define AFIO_EVCR_PIN_Get(x)        ((x)& 0xF) //!< Pin selection get

/*!< PORT configuration */
#define AFIO_EVCR_PORT              0x00000070 //!< PORT[2:0] bits (Port selection) */
#define AFIO_EVCR_PORT_Msk          0x00000070 //!< Port selection mask
#define AFIO_EVCR_PORT_Set(x)       ((x) << 4) //!< Port selection set
#define AFIO_EVCR_PORT_Get(x)       (((x)>>4)&7) //!< Port selection get
#define AFIO_EVCR_PORT_PA           0x00000000 //!< Port A selected */
#define AFIO_EVCR_PORT_PB           0x00000010 //!< Port B selected */
#define AFIO_EVCR_PORT_PC           0x00000020 //!< Port C selected */
#define AFIO_EVCR_PORT_PD           0x00000030 //!< Port D selected */
#define AFIO_EVCR_PORT_PE           0x00000040 //!< Port E selected */

#define AFIO_EVCR_EVOE              0x00000080 //!< Event Output Enable */
/** @} */

/** @name AFIO_MAPR:		(afio Offset: 0x04) AF remap and debug I/O configuration register */
/** @{ */
#define AFIO_MAPR_SPI1_REMAP                 0x00000001 //!< SPI1 remapping
#define AFIO_MAPR_I2C1_REMAP                 0x00000002 //!< I2C1 remapping
#define AFIO_MAPR_USART1_REMAP               0x00000004 //!< USART1 remapping
#define AFIO_MAPR_USART2_REMAP               0x00000008 //!< USART2 remapping

#define AFIO_MAPR_USART3_REMAP               0x00000030 //!< USART3_REMAP[1:0] bits (USART3 remapping)
#define AFIO_MAPR_USART3_REMAP_0             0x00000010 //!< Bit 0
#define AFIO_MAPR_USART3_REMAP_1             0x00000020 //!< Bit 1

/* USART3_REMAP configuration */
#define AFIO_MAPR_USART3_REMAP_NOREMAP       0x00000000 //!< No remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14)
#define AFIO_MAPR_USART3_REMAP_PARTIALREMAP  0x00000010 //!< Partial remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)
#define AFIO_MAPR_USART3_REMAP_FULLREMAP     0x00000030 //!< Full remap (TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12)

#define AFIO_MAPR_TIM1_REMAP                 0x000000C0 //!< TIM1_REMAP[1:0] bits (TIM1 remapping)
#define AFIO_MAPR_TIM1_REMAP_0               0x00000040 //!< Bit 0
#define AFIO_MAPR_TIM1_REMAP_1               0x00000080 //!< Bit 1

/*!< TIM1_REMAP configuration */
#define AFIO_MAPR_TIM1_REMAP_NOREMAP         0x00000000 //!< No remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15)
#define AFIO_MAPR_TIM1_REMAP_PARTIALREMAP    0x00000040 //!< Partial remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PA6, CH1N/PA7, CH2N/PB0, CH3N/PB1)
#define AFIO_MAPR_TIM1_REMAP_FULLREMAP       0x000000C0 //!< Full remap (ETR/PE7, CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8, CH2N/PE10, CH3N/PE12)

#define AFIO_MAPR_TIM2_REMAP                 0x00000300 //!< TIM2_REMAP[1:0] bits (TIM2 remapping)
#define AFIO_MAPR_TIM2_REMAP_0               0x00000100 //!< Bit 0
#define AFIO_MAPR_TIM2_REMAP_1               0x00000200 //!< Bit 1

/*!< TIM2_REMAP configuration */
#define AFIO_MAPR_TIM2_REMAP_NOREMAP         0x00000000 //!< No remap (CH1/ETR/PA0, CH2/PA1, CH3/PA2, CH4/PA3)
#define AFIO_MAPR_TIM2_REMAP_PARTIALREMAP1   0x00000100 //!< Partial remap (CH1/ETR/PA15, CH2/PB3, CH3/PA2, CH4/PA3)
#define AFIO_MAPR_TIM2_REMAP_PARTIALREMAP2   0x00000200 //!< Partial remap (CH1/ETR/PA0, CH2/PA1, CH3/PB10, CH4/PB11)
#define AFIO_MAPR_TIM2_REMAP_FULLREMAP       0x00000300 //!< Full remap (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)

#define AFIO_MAPR_TIM3_REMAP                 0x00000C00 //!< TIM3_REMAP[1:0] bits (TIM3 remapping)
#define AFIO_MAPR_TIM3_REMAP_0               0x00000400 //!< Bit 0 */
#define AFIO_MAPR_TIM3_REMAP_1               0x00000800 //!< Bit 1 */

/*!< TIM3_REMAP configuration */
#define AFIO_MAPR_TIM3_REMAP_NOREMAP         0x00000000 //!< No remap (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)
#define AFIO_MAPR_TIM3_REMAP_PARTIALREMAP    0x00000800 //!< Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)
#define AFIO_MAPR_TIM3_REMAP_FULLREMAP       0x00000C00 //!< Full remap (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)

#define AFIO_MAPR_TIM4_REMAP                 0x00001000 //!< TIM4_REMAP bit (TIM4 remapping)

#define AFIO_MAPR_CAN_REMAP                  0x00006000 //!< CAN_REMAP[1:0] bits (CAN Alternate function remapping)
#define AFIO_MAPR_CAN_REMAP_0                0x00002000 //!< Bit 0
#define AFIO_MAPR_CAN_REMAP_1                0x00004000 //!< Bit 1

/*!< CAN_REMAP configuration */
#define AFIO_MAPR_CAN_REMAP_REMAP1           0x00000000 //!< CANRX mapped to PA11, CANTX mapped to PA12
#define AFIO_MAPR_CAN_REMAP_REMAP2           0x00004000 //!< CANRX mapped to PB8, CANTX mapped to PB9
#define AFIO_MAPR_CAN_REMAP_REMAP3           0x00006000 //!< CANRX mapped to PD0, CANTX mapped to PD1

#define AFIO_MAPR_PD01_REMAP                 0x00008000 //!< Port D0/Port D1 mapping on OSC_IN/OSC_OUT
#define AFIO_MAPR_TIM5CH4_IREMAP             0x00010000 //!< TIM5 Channel4 Internal Remap
#define AFIO_MAPR_ADC1_ETRGINJ_REMAP         0x00020000 //!< ADC 1 External Trigger Injected Conversion remapping
#define AFIO_MAPR_ADC1_ETRGREG_REMAP         0x00040000 //!< ADC 1 External Trigger Regular Conversion remapping
#define AFIO_MAPR_ADC2_ETRGINJ_REMAP         0x00080000 //!< ADC 2 External Trigger Injected Conversion remapping
#define AFIO_MAPR_ADC2_ETRGREG_REMAP         0x00100000 //!< ADC 2 External Trigger Regular Conversion remapping

/*!< SWJ_CFG configuration */
#define AFIO_MAPR_SWJ_CFG                    0x07000000 //!< SWJ_CFG[2:0] bits (Serial Wire JTAG configuration)
#define AFIO_MAPR_SWJ_CFG_0                  0x01000000 //!< Bit 0
#define AFIO_MAPR_SWJ_CFG_1                  0x02000000 //!< Bit 1
#define AFIO_MAPR_SWJ_CFG_2                  0x04000000 //!< Bit 2

#define AFIO_MAPR_SWJ_CFG_RESET              0x00000000 //!< Full SWJ (JTAG-DP + SW-DP) : Reset State
#define AFIO_MAPR_SWJ_CFG_NOJNTRST           0x01000000 //!< Full SWJ (JTAG-DP + SW-DP) but without JNTRST
#define AFIO_MAPR_SWJ_CFG_JTAGDISABLE        0x02000000 //!< JTAG-DP Disabled and SW-DP Enabled
#define AFIO_MAPR_SWJ_CFG_DISABLE            0x04000000 //!< JTAG-DP Disabled and SW-DP Disabled

#ifdef STM32F10X_CL
/*!< ETH_REMAP configuration */
 #define AFIO_MAPR_ETH_REMAP                 0x00200000 //!< SPI3_REMAP bit (Ethernet MAC I/O remapping)

/*!< CAN2_REMAP configuration */
 #define AFIO_MAPR_CAN2_REMAP                0x00400000 //!< CAN2_REMAP bit (CAN2 I/O remapping)

/*!< MII_RMII_SEL configuration */
 #define AFIO_MAPR_MII_RMII_SEL              0x00800000 //!< MII_RMII_SEL bit (Ethernet MII or RMII selection)

/*!< SPI3_REMAP configuration */
 #define AFIO_MAPR_SPI3_REMAP                0x10000000 //!< SPI3_REMAP bit (SPI3 remapping)

/*!< TIM2ITR1_IREMAP configuration */
 #define AFIO_MAPR_TIM2ITR1_IREMAP           0x20000000 //!< TIM2ITR1_IREMAP bit (TIM2 internal trigger 1 remapping)

/*!< PTP_PPS_REMAP configuration */
 #define AFIO_MAPR_PTP_PPS_REMAP             0x40000000 //!< PTP_PPS_REMAP bit (Ethernet PTP PPS remapping)
#endif
/** @} */

/** @name AFIO_EXTICR[4]:	(afio Offset: 0x08) External interrupt configuration registers */
/** @{ */
#define AFIO_EXTICRy_EXTIx_Msk(x)            (0xF << ((x)*4))            //!< EXTI x configuration
#define AFIO_EXTICRy_EXTIx_Get(x,y)          (((y) >> ((x)*4)) & 0xF)    //!< EXTI x configuration get
#define AFIO_EXTICRy_EXTIx_Set(x,y)          ((y) << ((x)*4))            //!< EXTI x configuration set
/** @} */

/** @name AFIO_MAPR2:  		(afio Offset: 0x1C) AF remap and debug I/O configuration register2 */
/** @{ */
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
#define AFIO_MAPR2_TIM15_REMAP               0x00000001 //!< TIM15 remapping
#define AFIO_MAPR2_TIM16_REMAP               0x00000002 //!< TIM16 remapping
#define AFIO_MAPR2_TIM17_REMAP               0x00000004 //!< TIM17 remapping
#define AFIO_MAPR2_CEC_REMAP                 0x00000008 //!< CEC remapping
#define AFIO_MAPR2_TIM1_DMA_REMAP            0x00000010 //!< TIM1_DMA remapping
#endif

#ifdef STM32F10X_HD_VL
#define AFIO_MAPR2_TIM13_REMAP               0x00000100 //!< TIM13 remapping
#define AFIO_MAPR2_TIM14_REMAP               0x00000200 //!< TIM14 remapping
#define AFIO_MAPR2_FSMC_NADV_REMAP           0x00000400 //!< FSMC NADV remapping
#define AFIO_MAPR2_TIM67_DAC_DMA_REMAP       0x00000800 //!< TIM6/TIM7 and DAC DMA remapping
#define AFIO_MAPR2_TIM12_REMAP               0x00001000 //!< TIM12 remapping
#define AFIO_MAPR2_MISC_REMAP                0x00002000 //!< Miscellaneous remapping
#endif

#ifdef STM32F10X_XL
#define AFIO_MAPR2_TIM9_REMAP                0x00000020 //!< TIM9 remapping
#define AFIO_MAPR2_TIM10_REMAP               0x00000040 //!< TIM10 remapping
#define AFIO_MAPR2_TIM11_REMAP               0x00000080 //!< TIM11 remapping
#define AFIO_MAPR2_TIM13_REMAP               0x00000100 //!< TIM13 remapping
#define AFIO_MAPR2_TIM14_REMAP               0x00000200 //!< TIM14 remapping
#define AFIO_MAPR2_FSMC_NADV_REMAP           0x00000400 //!< FSMC NADV remapping
#endif
/** @} */



/** @} */ // @relates AFIO_TypeDef

/// Calculate port address for a given pin definition
#define PORT_ADDRESS(num) ((GPIO_TypeDef*)(GPIOA_BASE + ((num)*0x400)))


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


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _GPIO_H_ */

 /** @} */
