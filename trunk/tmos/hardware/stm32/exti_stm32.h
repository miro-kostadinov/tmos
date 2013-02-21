/*
 * exti_stm32.h
 *
 *  Created on: Nov 1, 2012
 *      Author: miro
 */

#ifndef EXTI_STM32_H_
#define EXTI_STM32_H_

#include <mcu_inc.h>
#include <tmos_types.h>
#include <gpio_stm32.h>

/*******************************************************************************
 *  External Interrupt/Event Controller (EXTI) registers
 ******************************************************************************/
typedef struct
{
	__IO uint32_t EXTI_IMR;  	//!< (exti Offset: 0x00) EXTI Interrupt mask register
	__IO uint32_t EXTI_EMR;   	//!< (exti Offset: 0x04) EXTI Event mask register
	__IO uint32_t EXTI_RTSR;  	//!< (exti Offset: 0x05) EXTI Rising trigger selection register
	__IO uint32_t EXTI_FTSR;  	//!< (exti Offset: 0x0C) EXTI Falling trigger selection register
	__IO uint32_t EXTI_SWIER; 	//!< (exti Offset: 0x10) EXTI Software interrupt event register
	__IO uint32_t EXTI_PR;    	//!< (exti Offset: 0x14) EXTI Pending register
} EXTI_TypeDef;

/*******************************************************************************
 * @defgroup EXTI_regs_define
 * @{
 ******************************************************************************/


#define EXTI_Line0         		0x00000001 //!< External Interrupt line 0
#define EXTI_Line1              0x00000002 //!< External Interrupt line 1
#define EXTI_Line2              0x00000004 //!< External Interrupt line 2
#define EXTI_Line3              0x00000008 //!< External Interrupt line 3
#define EXTI_Line4              0x00000010 //!< External Interrupt line 4
#define EXTI_Line5              0x00000020 //!< External Interrupt line 5
#define EXTI_Line6              0x00000040 //!< External Interrupt line 6
#define EXTI_Line7              0x00000080 //!< External Interrupt line 7
#define EXTI_Line8              0x00000100 //!< External Interrupt line 8
#define EXTI_Line9              0x00000200 //!< External Interrupt line 9
#define EXTI_Line10             0x00000400 //!< External Interrupt line 10
#define EXTI_Line11             0x00000800 //!< External Interrupt line 11
#define EXTI_Line12             0x00001000 //!< External Interrupt line 12
#define EXTI_Line13             0x00002000 //!< External Interrupt line 13
#define EXTI_Line14             0x00004000 //!< External Interrupt line 14
#define EXTI_Line15             0x00008000 //!< External Interrupt line 15
#define EXTI_Line16             0x00010000 //!< External Interrupt line 16
#define EXTI_Line17             0x00020000 //!< External Interrupt line 17
#define EXTI_Line18             0x00040000 //!< External Interrupt line 18
#define EXTI_Line19             0x00080000 //!< External Interrupt line 19
#define EXTI_Line20             0x00100000 //!< External Interrupt line 20
#define EXTI_Line21             0x00200000 //!< External Interrupt line 21
#define EXTI_Line22             0x00400000 //!< External Interrupt line 22

/** @defgroup EXTI_IMR:  	(exti Offset: 0x00) EXTI Interrupt mask register	*/
#define EXTI_IMR_MR0         	0x00000001 //!< Interrupt Mask on line 0
#define EXTI_IMR_MR1            0x00000002 //!< Interrupt Mask on line 1
#define EXTI_IMR_MR2            0x00000004 //!< Interrupt Mask on line 2
#define EXTI_IMR_MR3            0x00000008 //!< Interrupt Mask on line 3
#define EXTI_IMR_MR4            0x00000010 //!< Interrupt Mask on line 4
#define EXTI_IMR_MR5            0x00000020 //!< Interrupt Mask on line 5
#define EXTI_IMR_MR6            0x00000040 //!< Interrupt Mask on line 6
#define EXTI_IMR_MR7            0x00000080 //!< Interrupt Mask on line 7
#define EXTI_IMR_MR8            0x00000100 //!< Interrupt Mask on line 8
#define EXTI_IMR_MR9            0x00000200 //!< Interrupt Mask on line 9
#define EXTI_IMR_MR10           0x00000400 //!< Interrupt Mask on line 10
#define EXTI_IMR_MR11           0x00000800 //!< Interrupt Mask on line 11
#define EXTI_IMR_MR12           0x00001000 //!< Interrupt Mask on line 12
#define EXTI_IMR_MR13           0x00002000 //!< Interrupt Mask on line 13
#define EXTI_IMR_MR14           0x00004000 //!< Interrupt Mask on line 14
#define EXTI_IMR_MR15           0x00008000 //!< Interrupt Mask on line 15
#define EXTI_IMR_MR16           0x00010000 //!< Interrupt Mask on line 16
#define EXTI_IMR_MR17           0x00020000 //!< Interrupt Mask on line 17
#define EXTI_IMR_MR18           0x00040000 //!< Interrupt Mask on line 18
#define EXTI_IMR_MR19           0x00080000 //!< Interrupt Mask on line 19
#define EXTI_IMR_MR20           0x00100000 //!< Interrupt Mask on line 20
#define EXTI_IMR_MR21           0x00200000 //!< Interrupt Mask on line 21
#define EXTI_IMR_MR22           0x00400000 //!< Interrupt Mask on line 22
/** @} */

/** @defgroup EXTI_EMR:   	(exti Offset: 0x04) EXTI Event mask register		*/
#define EXTI_EMR_MR0            0x00000001 //!< Event Mask on line 0
#define EXTI_EMR_MR1            0x00000002 //!< Event Mask on line 1
#define EXTI_EMR_MR2            0x00000004 //!< Event Mask on line 2
#define EXTI_EMR_MR3            0x00000008 //!< Event Mask on line 3
#define EXTI_EMR_MR4            0x00000010 //!< Event Mask on line 4
#define EXTI_EMR_MR5            0x00000020 //!< Event Mask on line 5
#define EXTI_EMR_MR6            0x00000040 //!< Event Mask on line 6
#define EXTI_EMR_MR7            0x00000080 //!< Event Mask on line 7
#define EXTI_EMR_MR8            0x00000100 //!< Event Mask on line 8
#define EXTI_EMR_MR9            0x00000200 //!< Event Mask on line 9
#define EXTI_EMR_MR10           0x00000400 //!< Event Mask on line 10
#define EXTI_EMR_MR11           0x00000800 //!< Event Mask on line 11
#define EXTI_EMR_MR12           0x00001000 //!< Event Mask on line 12
#define EXTI_EMR_MR13           0x00002000 //!< Event Mask on line 13
#define EXTI_EMR_MR14           0x00004000 //!< Event Mask on line 14
#define EXTI_EMR_MR15           0x00008000 //!< Event Mask on line 15
#define EXTI_EMR_MR16           0x00010000 //!< Event Mask on line 16
#define EXTI_EMR_MR17           0x00020000 //!< Event Mask on line 17
#define EXTI_EMR_MR18           0x00040000 //!< Event Mask on line 18
#define EXTI_EMR_MR19           0x00080000 //!< Event Mask on line 19
#define EXTI_EMR_MR20           0x00100000 //!< Event Mask on line 20
#define EXTI_EMR_MR21           0x00200000 //!< Event Mask on line 21
#define EXTI_EMR_MR22           0x00400000 //!< Event Mask on line 22
/** @} */

/** @defgroup EXTI_RTSR:  	(exti Offset: 0x05) EXTI Rising trigger selection register */
#define EXTI_RTSR_TR0           0x00000001 //!< Rising trigger event configuration bit of line 0
#define EXTI_RTSR_TR1           0x00000002 //!< Rising trigger event configuration bit of line 1
#define EXTI_RTSR_TR2           0x00000004 //!< Rising trigger event configuration bit of line 2
#define EXTI_RTSR_TR3           0x00000008 //!< Rising trigger event configuration bit of line 3
#define EXTI_RTSR_TR4           0x00000010 //!< Rising trigger event configuration bit of line 4
#define EXTI_RTSR_TR5           0x00000020 //!< Rising trigger event configuration bit of line 5
#define EXTI_RTSR_TR6           0x00000040 //!< Rising trigger event configuration bit of line 6
#define EXTI_RTSR_TR7           0x00000080 //!< Rising trigger event configuration bit of line 7
#define EXTI_RTSR_TR8           0x00000100 //!< Rising trigger event configuration bit of line 8
#define EXTI_RTSR_TR9           0x00000200 //!< Rising trigger event configuration bit of line 9
#define EXTI_RTSR_TR10          0x00000400 //!< Rising trigger event configuration bit of line 10
#define EXTI_RTSR_TR11          0x00000800 //!< Rising trigger event configuration bit of line 11
#define EXTI_RTSR_TR12          0x00001000 //!< Rising trigger event configuration bit of line 12
#define EXTI_RTSR_TR13          0x00002000 //!< Rising trigger event configuration bit of line 13
#define EXTI_RTSR_TR14          0x00004000 //!< Rising trigger event configuration bit of line 14
#define EXTI_RTSR_TR15          0x00008000 //!< Rising trigger event configuration bit of line 15
#define EXTI_RTSR_TR16          0x00010000 //!< Rising trigger event configuration bit of line 16
#define EXTI_RTSR_TR17          0x00020000 //!< Rising trigger event configuration bit of line 17
#define EXTI_RTSR_TR18          0x00040000 //!< Rising trigger event configuration bit of line 18
#define EXTI_RTSR_TR19          0x00080000 //!< Rising trigger event configuration bit of line 19
/** @} */

/** @defgroup EXTI_FTSR:  	(exti Offset: 0x0C) EXTI Falling trigger selection register */
#define EXTI_FTSR_TR0           0x00000001 //!< Falling trigger event configuration bit of line 0
#define EXTI_FTSR_TR1           0x00000002 //!< Falling trigger event configuration bit of line 1
#define EXTI_FTSR_TR2           0x00000004 //!< Falling trigger event configuration bit of line 2
#define EXTI_FTSR_TR3           0x00000008 //!< Falling trigger event configuration bit of line 3
#define EXTI_FTSR_TR4           0x00000010 //!< Falling trigger event configuration bit of line 4
#define EXTI_FTSR_TR5           0x00000020 //!< Falling trigger event configuration bit of line 5
#define EXTI_FTSR_TR6           0x00000040 //!< Falling trigger event configuration bit of line 6
#define EXTI_FTSR_TR7           0x00000080 //!< Falling trigger event configuration bit of line 7
#define EXTI_FTSR_TR8           0x00000100 //!< Falling trigger event configuration bit of line 8
#define EXTI_FTSR_TR9           0x00000200 //!< Falling trigger event configuration bit of line 9
#define EXTI_FTSR_TR10          0x00000400 //!< Falling trigger event configuration bit of line 10
#define EXTI_FTSR_TR11          0x00000800 //!< Falling trigger event configuration bit of line 11
#define EXTI_FTSR_TR12          0x00001000 //!< Falling trigger event configuration bit of line 12
#define EXTI_FTSR_TR13          0x00002000 //!< Falling trigger event configuration bit of line 13
#define EXTI_FTSR_TR14          0x00004000 //!< Falling trigger event configuration bit of line 14
#define EXTI_FTSR_TR15          0x00008000 //!< Falling trigger event configuration bit of line 15
#define EXTI_FTSR_TR16          0x00010000 //!< Falling trigger event configuration bit of line 16
#define EXTI_FTSR_TR17          0x00020000 //!< Falling trigger event configuration bit of line 17
#define EXTI_FTSR_TR18          0x00040000 //!< Falling trigger event configuration bit of line 18
#define EXTI_FTSR_TR19          0x00080000 //!< Falling trigger event configuration bit of line 19
/** @} */

/** @defgroup EXTI_SWIER: 	(exti Offset: 0x10) EXTI Software interrupt event register */
#define EXTI_SWIER_SWIER0       0x00000001 //!< Software Interrupt on line 0
#define EXTI_SWIER_SWIER1       0x00000002 //!< Software Interrupt on line 1
#define EXTI_SWIER_SWIER2       0x00000004 //!< Software Interrupt on line 2
#define EXTI_SWIER_SWIER3       0x00000008 //!< Software Interrupt on line 3
#define EXTI_SWIER_SWIER4       0x00000010 //!< Software Interrupt on line 4
#define EXTI_SWIER_SWIER5       0x00000020 //!< Software Interrupt on line 5
#define EXTI_SWIER_SWIER6       0x00000040 //!< Software Interrupt on line 6
#define EXTI_SWIER_SWIER7       0x00000080 //!< Software Interrupt on line 7
#define EXTI_SWIER_SWIER8       0x00000100 //!< Software Interrupt on line 8
#define EXTI_SWIER_SWIER9       0x00000200 //!< Software Interrupt on line 9
#define EXTI_SWIER_SWIER10      0x00000400 //!< Software Interrupt on line 10
#define EXTI_SWIER_SWIER11      0x00000800 //!< Software Interrupt on line 11
#define EXTI_SWIER_SWIER12 	    0x00001000 //!< Software Interrupt on line 12
#define EXTI_SWIER_SWIER13      0x00002000 //!< Software Interrupt on line 13
#define EXTI_SWIER_SWIER14      0x00004000 //!< Software Interrupt on line 14
#define EXTI_SWIER_SWIER15      0x00008000 //!< Software Interrupt on line 15
#define EXTI_SWIER_SWIER16		0x00010000 //!< Software Interrupt on line 16
#define EXTI_SWIER_SWIER17      0x00020000 //!< Software Interrupt on line 17
#define EXTI_SWIER_SWIER18      0x00040000 //!< Software Interrupt on line 18
#define EXTI_SWIER_SWIER19      0x00080000 //!< Software Interrupt on line 19
/** @} */

/** @defgroup EXTI_PR:    	(exti Offset: 0x14) EXTI Pending register			*/
#define EXTI_PR_PR0             0x00000001 //!< Pending bit for line 0
#define EXTI_PR_PR1             0x00000002 //!< Pending bit for line 1
#define EXTI_PR_PR2             0x00000004 //!< Pending bit for line 2
#define EXTI_PR_PR3             0x00000008 //!< Pending bit for line 3
#define EXTI_PR_PR4             0x00000010 //!< Pending bit for line 4
#define EXTI_PR_PR5             0x00000020 //!< Pending bit for line 5
#define EXTI_PR_PR6             0x00000040 //!< Pending bit for line 6
#define EXTI_PR_PR7             0x00000080 //!< Pending bit for line 7
#define EXTI_PR_PR8             0x00000100 //!< Pending bit for line 8
#define EXTI_PR_PR9             0x00000200 //!< Pending bit for line 9
#define EXTI_PR_PR10            0x00000400 //!< Pending bit for line 10
#define EXTI_PR_PR11            0x00000800 //!< Pending bit for line 11
#define EXTI_PR_PR12            0x00001000 //!< Pending bit for line 12
#define EXTI_PR_PR13            0x00002000 //!< Pending bit for line 13
#define EXTI_PR_PR14            0x00004000 //!< Pending bit for line 14
#define EXTI_PR_PR15            0x00008000 //!< Pending bit for line 15
#define EXTI_PR_PR16            0x00010000 //!< Pending bit for line 16
#define EXTI_PR_PR17            0x00020000 //!< Pending bit for line 17
#define EXTI_PR_PR18            0x00040000 //!< Pending bit for line 18
#define EXTI_PR_PR19            0x00080000 //!< Pending bit for line 19
#define EXTI_PR_PR20            0x00100000 //!< Pending bit for line 20
#define EXTI_PR_PR21            0x00200000 //!< Pending bit for line 21
#define EXTI_PR_PR22            0x00400000 //!< Pending bit for line 22
/** @} */


/** @} */ // @defgroup EXTI_regs_define

void exti_enable_irq(EXTI_TypeDef* exti, uint32_t lines, pio_def type);
void exti_enable_event(EXTI_TypeDef* exti, uint32_t lines, pio_def type);
void exti_disable_irq(EXTI_TypeDef* exti, uint32_t lines);
void exti_disable_event(EXTI_TypeDef* exti, uint32_t lines);



#endif /* EXTI_STM32_H_ */
