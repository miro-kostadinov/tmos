/*
 *
 *  This file should provide all core and mcu specific defines and types.
 *
 *  C usage:
 *  	From application level is preferred to include <drivers.h>
 *  	<drivers.h> will include <mcu.h>
 *
 */
#ifndef BRD_CPP_H_
#define BRD_CPP_H_

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 defines first
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef __cplusplus
 extern "C" {

 enum DRIVER_INDEX : signed char
{
/******  Cortex-M3 Processor Exceptions Numbers ******************************/
    NonMaskableInt_IRQn     = -14,  /*!< 2 Non Maskable Interrupt            */
    MemoryManagement_IRQn   = -12,  /*!< 4 Cortex-M3 Memory Management Int   */
    BusFault_IRQn           = -11,  /*!< 5 Cortex-M3 Bus Fault Interrupt     */
    UsageFault_IRQn         = -10,  /*!< 6 Cortex-M3 Usage Fault Interrupt   */
    SVCall_IRQn             = -5,   /*!< 11 Cortex-M3 SV Call Interrupt      */
    DebugMonitor_IRQn       = -4,   /*!< 12 Cortex-M3 Debug Monitor Interrupt*/
    PendSV_IRQn             = -2,   /*!< 14 Cortex-M3 Pend SV Interrupt      */
    SysTick_IRQn            = -1,   /*!< 15 Cortex-M3 System Tick Interrupt  */

    /******  STM32 specific Interrupt Numbers *********************************/
    WWDG_IRQn               = 0,    /*!< Window WatchDog Interrupt            */
    PVD_IRQn                = 1,    /*!< PVD through EXTI Line detection Interrupt */
    TAMPER_IRQn             = 2,    /*!< Tamper Interrupt                     */
    RTC_IRQn                = 3,    /*!< RTC global Interrupt                 */
    FLASH_IRQn              = 4,    /*!< FLASH global Interrupt               */
    RCC_IRQn                = 5,    /*!< RCC global Interrupt                 */
    EXTI0_IRQn              = 6,    /*!< EXTI Line0 Interrupt                 */
    EXTI1_IRQn              = 7,    /*!< EXTI Line1 Interrupt                 */
    EXTI2_IRQn              = 8,    /*!< EXTI Line2 Interrupt                 */
    EXTI3_IRQn              = 9,    /*!< EXTI Line3 Interrupt                 */
    EXTI4_IRQn              = 10,   /*!< EXTI Line4 Interrupt                 */
    DMA1_Channel1_IRQn      = 11,   /*!< DMA1 Channel 1 global Interrupt      */
    DMA1_Channel2_IRQn      = 12,   /*!< DMA1 Channel 2 global Interrupt      */
    DMA1_Channel3_IRQn      = 13,   /*!< DMA1 Channel 3 global Interrupt      */
    DMA1_Channel4_IRQn      = 14,   /*!< DMA1 Channel 4 global Interrupt      */
    DMA1_Channel5_IRQn      = 15,   /*!< DMA1 Channel 5 global Interrupt      */
    DMA1_Channel6_IRQn      = 16,   /*!< DMA1 Channel 6 global Interrupt      */
    DMA1_Channel7_IRQn      = 17,   /*!< DMA1 Channel 7 global Interrupt      */


    INALID_DRV_INDX	                /*!<  invalid	                         */
};

}

#else

typedef signed char DRIVER_INDEX;

#endif




#endif /*BRD_CPP_H_*/
