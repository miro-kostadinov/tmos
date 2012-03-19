/*
 * board.h
 *
 *  Created on: 2012-17-01
 *      Author: Miroslav kostadinov
 *
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

/******  LM3S Specific Interrupt Numbers *************************************/
    GPIOPortA_IRQn          =  0,   /*!<  GPIO Port A                        */
    GPIOPortB_IRQn          =  1,   /*!<  GPIO Port B                        */
    GPIOPortC_IRQn          =  2,   /*!<  GPIO Port C                        */
    GPIOPortD_IRQn          =  3,   /*!<  GPIO Port D                        */
    GPIOPortE_IRQn          =  4,   /*!<  GPIO Port E                        */
    UART0_IRQn              =  5,   /*!<  UART0                              */
    UART1_IRQn              =  6,   /*!<  UART1                              */
    SSI0_IRQn               =  7,   /*!<  SSI0                               */
    I2C0_IRQn               =  8,   /*!<  I2C0                               */
    PWMFault_IRQn           =  9,   /*!<  PWM Fault                          */
    PWMGen0_IRQn            = 10,   /*!<  PWM Generator 0                    */
    PWMGen1_IRQn            = 11,   /*!<  PWM Generator 1                    */
    PWMGen2_IRQn            = 12,   /*!<  PWM Generator 2                    */
    QEI0_IRQn               = 13,   /*!<  Quadrature Encoder 0               */
    ADCSeq0_IRQn            = 14,   /*!<  ADC Sequence 0                     */
    ADCSeq1_IRQn            = 15,   /*!<  ADC Sequence 1                     */
    ADCSeq2_IRQn            = 16,   /*!<  ADC Sequence 2                     */
    ADCSeq3_IRQn            = 17,   /*!<  ADC Sequence 3                     */
    Watchdog_IRQn           = 18,   /*!<  Watchdog                           */
    Timer0A_IRQn            = 19,   /*!<  Timer 0A                           */
    Timer0B_IRQn            = 20,   /*!<  Timer 0B                           */
    Timer1A_IRQn            = 21,   /*!<  Timer 1A                           */
    Timer1B_IRQn            = 22,   /*!<  Timer 1B                           */
    Timer2A_IRQn            = 23,   /*!<  Timer 2A                           */
    Timer2B_IRQn            = 24,   /*!<  Timer 2B                           */
    Comp0_IRQn              = 25,   /*!<  Comp 0                             */
    Comp1_IRQn              = 26,   /*!<  Comp 1                             */
    Comp2_IRQn              = 27,   /*!<  Comp 2                             */
    SysCtrl_IRQn            = 28,   /*!<  System Control                     */
    FlashCtrl_IRQn          = 29,   /*!<  Flash Control                      */
    GPIOPortF_IRQn          = 30,   /*!<  GPIO Port F                        */
    GPIOPortG_IRQn          = 31,   /*!<  GPIO Port G                        */
    GPIOPortH_IRQn          = 32,   /*!<  GPIO Port H                        */
    UART2_IRQn              = 33,   /*!<  UART2 Rx and Tx                    */
    SSI1_IRQn               = 34,   /*!<  SSI1 Rx and Tx                     */
    Timer3A_IRQn            = 35,   /*!<  Timer 3 subtimer A                 */
    Timer3B_IRQn            = 36,   /*!<  Timer 3 subtimer B                 */
    I2C1_IRQn               = 37,   /*!<  I2C1 Master and Slave              */
    QEI1_IRQn               = 38,   /*!<  Quadrature Encoder 1               */
    CAN0_IRQn               = 39,   /*!<  CAN0                               */
    CAN1_IRQn               = 40,   /*!<  CAN1                               */
    CAN2_IRQn               = 41,   /*!<  CAN2                               */
    Ethernet_IRQn           = 42,   /*!<  Ethernet                           */
    Hibernate_IRQn          = 43,   /*!<  Hibernate                          */
    USB0_IRQn               = 44,   /*!<  USB0                               */
    PWMGen3_IRQn            = 45,   /*!<  PWM Generator 3                    */
    uDMA_IRQn               = 46,   /*!<  uDMA Software Transfer             */
    uDMAErr_IRQn            = 47,    /*!<  uDMA Error                        */
    ADC1Seq0_IRQn           = 48,   /*!<  ADC 1 Sequence 0                   */
    ADC1Seq1_IRQn           = 49,   /*!<  ADC 1 Sequence 1                   */
    ADC1Seq2_IRQn           = 50,   /*!<  ADC 1 Sequence 2                   */
    ADC1Seq3_IRQn           = 51,   /*!<  ADC 1 Sequence 3                   */
    I2SO_IRQn				= 52,	/*!<  */
    EPI_IRQn				= 53,	/*!<  */
    GPIOPortJ_IRQn			= 54,	/*!<  */
    KEY_DRV_INDX,					//55
    GUI_DRV_INDX,					//56
    FILE_DRV_INDX,					//57
    FD_DRV_INDX,					//58
    SDCARD_DRV_INDX,				//59
    GSM_DRV_INDX,					//60
    INALID_DRV_INDX	                /*!<  invalid	                         */
};

}

#else

typedef signed char DRIVER_INDEX;

#endif




#endif /*BRD_CPP_H_*/
