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

    /******  tm4c specific Interrupt Numbers *********************************/
    GPIOPortA_IRQn          =  0,   /*!< GPIO Port A                          */
    GPIOPortB_IRQn          =  1,   /*!< GPIO Port B                          */
    GPIOPortC_IRQn          =  2,   /*!< GPIO Port C                          */
    GPIOPortD_IRQn          =  3,   /*!< GPIO Port D                          */
    GPIOPortE_IRQn          =  4,   /*!< GPIO Port E                          */
    UART0_IRQn              =  5,   /*!< UART0                                */
    UART1_IRQn              =  6,   /*!< UART1                                */
    SSI0_IRQn               =  7,   /*!< SSI0                                 */
    I2C0_IRQn               =  8,   /*!< I 2 C0                               */
    PWMFault_IRQn           =  9,   /*!< PWM Fault                            */
    PWMGen0_IRQn            = 10,   /*!< PWM Generator 0                      */
    PWMGen1_IRQn            = 11,   /*!< PWM Generator 1                      */
    PWMGen2_IRQn            = 12,   /*!< PWM Generator 2                      */
    QEI0_IRQn               = 13,   /*!< QEI0                                 */
    ADCSeq0_IRQn            = 14,   /*!< ADC0 Sequence 0                      */
    ADCSeq1_IRQn            = 15,   /*!< ADC0 Sequence 1                      */
    ADCSeq2_IRQn            = 16,   /*!< ADC0 Sequence 2                      */
    ADCSeq3_IRQn            = 17,   /*!< ADC0 Sequence 3                      */
    Watchdog_IRQn           = 18,   /*!< Watchdog Timers 0 and 1              */
    Timer0A_IRQn            = 19,   /*!< 16/32-Bit Timer 0A                   */
    Timer0B_IRQn            = 20,   /*!< 16/32-Bit Timer 0B                   */
    Timer1A_IRQn            = 21,   /*!< 16/32-Bit Timer 1A                   */
    Timer1B_IRQn            = 22,   /*!< 16/32-Bit Timer 1B                   */
    Timer2A_IRQn            = 23,   /*!< 16/32-Bit Timer 2A                   */
    Timer2B_IRQn            = 24,   /*!< 16/32-Bit Timer 2B                   */
    Comp0_IRQn              = 25,   /*!< Analog Comparator 0                  */
    Comp1_IRQn              = 26,   /*!< Analog Comparator 1                  */
    Comp2_IRQn              = 27,   /*!< Analog Comparator 2                  */
    SysCtrl_IRQn            = 28,   /*!< System Control                       */
    FlashCtrl_IRQn          = 29,   /*!< Flash Memory Control                 */
    GPIOPortF_IRQn          = 30,   /*!< GPIO Port F                          */
    GPIOPortG_IRQn          = 31,   /*!< GPIO Port G                          */
    GPIOPortH_IRQn          = 32,   /*!< GPIO Port H                          */
    UART2_IRQn              = 33,   /*!< UART2                                */
    SSI1_IRQn               = 34,   /*!< SSI1                                 */
    Timer3A_IRQn            = 35,   /*!< 16/32-Bit Timer 3A                   */
    Timer3B_IRQn            = 36,   /*!< 16/32-Bit Timer 3B                   */
    I2C1_IRQn               = 37,   /*!< I 2 C1                               */
    CAN0_IRQn               = 38,   /*!< CAN 0                                */
    CAN1_IRQn               = 39,   /*!< CAN1                                 */
    EMAC_IRQn               = 40,   /*!< Ethernet MAC                         */
    Hibernate_IRQn          = 41,   /*!< HIB                                  */
    USB0_IRQn	            = 42,   /*!< USB MAC                              */
    PWMGen3_IRQn	        = 43,   /*!< PWM Generator 3                      */
    uDMA_IRQn               = 44,   /*!< uDMA 0 Software                      */
    uDMAErr_IRQn            = 45,   /*!< uDMA 0 Error                         */
    ADC1Seq0_IRQn           = 46,   /*!< ADC1 Sequence 0                      */
    ADC1Seq1_IRQn           = 47,   /*!< ADC1 Sequence 1            		  */
    ADC1Seq2_IRQn           = 48,   /*!< ADC1 Sequence 2                      */
    ADC1Seq3_IRQn           = 49,   /*!< ADC1 Sequence 3                      */
    EPI0_IRQn	            = 50,   /*!< EPI 0                                */
    GPIOPortJ_IRQn          = 51,   /*!< GPIO Port J                          */
    GPIOPortK_IRQn			= 52,	/*!< GPIO Port K                          */
    GPIOPortL_IRQn			= 53,	/*!< GPIO Port L                          */
    SSI2_IRQn				= 54,	/*!< SSI 2                                */
    SSI3_IRQn               = 55,   /*!< SSI 3                                */
    UART3_IRQn              = 56,   /*!< UART 3                               */
    UART4_IRQn              = 57,   /*!< UART 4                               */
    UART5_IRQn              = 58,   /*!< UART 5                               */
    UART6_IRQn              = 59,   /*!< UART 6                               */
    UART7_IRQn              = 60,   /*!< UART 7                               */
    I2C2_IRQn               = 61,   /*!< I 2 C 2                              */
    I2C3_IRQn               = 62,   /*!< I 2 C 3                              */
    Timer4A_IRQn            = 63,   /*!< Timer 4A                             */
    Timer4B_IRQn            = 64,   /*!< Timer 4B                             */
    Timer5A_IRQn            = 65,   /*!< Timer 5A                             */
    Timer5B_IRQn            = 66,   /*!< Timer 5B                             */
    FP_EXC_IRQn             = 67,   /*!< Floating-Point Exception (imprecise) */
    Reserved68_IRQn         = 68,   /*!< Reserved                             */
    Reserved69_IRQn         = 69,   /*!< Reserved                             */
    I2C4_IRQn               = 70,   /*!< I 2 C 4                              */
    I2C5_IRQn               = 71,   /*!< I 2 C 5                              */
    GPIOPortM_IRQn          = 72,   /*!< GPIO Port M                          */
    GPIOPortN_IRQn          = 73,   /*!< GPIO Port N                          */
    Reserved74_IRQn         = 74,   /*!< Reserved                             */
    Tamper_IRQn             = 75,   /*!< Tamper                               */
    GPIOPortP0_IRQn         = 76,   /*!< GPIO Port P (Summary or P0)          */
    GPIOPortP1_IRQn         = 77,   /*!< GPIO Port P1                         */
    GPIOPortP2_IRQn         = 78,   /*!< GPIO Port P2                         */
    GPIOPortP3_IRQn         = 79,   /*!< GPIO Port P3                         */
    GPIOPortP4_IRQn         = 80,   /*!< GPIO Port P4                         */
    GPIOPortP5_IRQn         = 81,   /*!< GPIO Port P5                         */
    GPIOPortP6_IRQn         = 82,   /*!< GPIO Port P6                         */
    GPIOPortP7_IRQn         = 83,   /*!< GPIO Port P7                         */
    GPIOPortQ0_IRQn         = 84,   /*!< GPIO Port Q (Summary or Q0)          */
    GPIOPortQ1_IRQn         = 85,   /*!< GPIO Port Q1                         */
    GPIOPortQ2_IRQn         = 86,   /*!< GPIO Port Q2                         */
    GPIOPortQ3_IRQn         = 87,   /*!< GPIO Port Q3                         */
    GPIOPortQ4_IRQn         = 88,   /*!< GPIO Port Q4                         */
    GPIOPortQ5_IRQn         = 89,   /*!< GPIO Port Q5                         */
    GPIOPortQ6_IRQn         = 90,   /*!< GPIO Port Q6                         */
    GPIOPortQ7_IRQn         = 91,   /*!< GPIO Port Q7                         */
    Reserved92_IRQn         = 92,   /*!< Reserved                             */
    Reserved93_IRQn         = 93,   /*!< Reserved                             */
    Reserved94_IRQn         = 94,   /*!< Reserved                             */
    Reserved95_IRQn         = 95,   /*!< Reserved                             */
    Reserved96_IRQn         = 96,   /*!< Reserved                             */
    Reserved97_IRQn         = 97,   /*!< Reserved                             */
    Timer6A_IRQn            = 98,   /*!< 16/32-Bit Timer 6A                   */
    Timer6B_IRQn            = 99,   /*!< 16/32-Bit Timer 6B                   */
    Timer7A_IRQn            = 100,  /*!< 16/32-Bit Timer 7A                   */
    Timer7B_IRQn            = 101,  /*!< 16/32-Bit Timer 7B                   */
    I2C6_IRQn               = 102,  /*!< I 2 C 6                              */
    I2C7_IRQn               = 103,  /*!< I 2 C 7                              */
    Reserved104_IRQn        = 104,  /*!< Reserved                             */
    Reserved105_IRQn        = 105,  /*!< Reserved                             */
    Reserved106_IRQn        = 106,  /*!< Reserved                             */
    Reserved107_IRQn        = 107,  /*!< Reserved                             */
    Reserved108_IRQn        = 108,  /*!< Reserved                             */
    I2C8_IRQn               = 109,  /*!< I 2 C 8                              */
    I2C9_IRQn               = 110,  /*!< I 2 C 9                              */
    INALID_DRV_INDX	                /*!<  invalid	                         */

 };

}

#else

typedef signed char DRIVER_INDEX;

#endif




#endif /*BRD_CPP_H_*/
