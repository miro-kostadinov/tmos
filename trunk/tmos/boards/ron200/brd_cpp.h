/*
 * board.h
 *
 *  Created on: 2010-3-11
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
enum DRIVER_INDEX : signed char
#else
typedef signed char DRIVER_INDEX;
enum DRIVER_INDEX_t
#endif
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

/******  SAM3S Specific Interrupt Numbers *************************************/
    SUPC_IRQn            =  0, /*!<  0 Supply Controller (SUPC) 			  */
    RSTC_IRQn            =  1, /*!<  1 Reset Controller (RSTC)          	  */
    RTC_IRQn             =  2, /*!<  2 Real Time Clock (RTC)            	  */
    RTT_IRQn             =  3, /*!<  3 Real Time Timer (RTT)            	  */
    WDT_IRQn             =  4, /*!<  4 Watchdog Timer (WDT)             	  */
    PMC_IRQn             =  5, /*!<  5 Power Management Controller (PMC)	  */
    EFC_IRQn             =  6, /*!<  6 Enhanced Embedded Flash Controller 	  */
    UART0_IRQn           =  8, /*!<  8 UART 0 (UART0)                   	  */
    UART1_IRQn           =  9, /*!<  9 UART 1 (UART1) 				  		  */
    SMC_IRQn             = 10, /*!< 10 Static Memory Controller (SMC)   	  */
    PIOA_IRQn            = 11, /*!< 11 Parallel I/O Controller A (PIOA) 	  */
    PIOB_IRQn            = 12, /*!< 12 Parallel I/O Controller B (PIOB)   	  */
    PIOC_IRQn            = 13, /*!< 13 Parallel I/O Controller C (PIOC) 	  */
    USART0_IRQn          = 14, /*!< 14 USART 0 (USART0)     			  	  */
    USART1_IRQn          = 15, /*!< 15 USART 1 (USART1) 				  	  */
    HSMCI_IRQn           = 18, /*!< 18 Multimedia Card Interface (HSMCI)	  */
    TWI0_IRQn            = 19, /*!< 19 Two Wire Interface 0 (TWI0) 	  		  */
    TWI1_IRQn            = 20, /*!< 20 Two Wire Interface 1 (TWI1) 	  		  */
    SPI_IRQn             = 21, /*!< 21 Serial Peripheral Interface (SPI)	  */
    SSC_IRQn             = 22, /*!< 22 Synchronous Serial Controller(SSC)	  */
    TC0_IRQn             = 23, /*!< 23 Timer/Counter 0 (TC0) 			  	  */
    TC1_IRQn             = 24, /*!< 24 Timer/Counter 1 (TC1) 			  	  */
    TC2_IRQn             = 25, /*!< 25 Timer/Counter 2 (TC2) 				  */
    TC3_IRQn             = 26, /*!< 26 Timer/Counter 3 (TC3) 				  */
    TC4_IRQn             = 27, /*!< 27 Timer/Counter 4 (TC4) 				  */
    TC5_IRQn             = 28, /*!< 28 Timer/Counter 5 (TC5) 				  */
    ADC_IRQn             = 29, /*!< 29 Analog To Digital Converter (ADC) 	  */
    DACC_IRQn            = 30, /*!< 30 Digital To Analog Converter (DACC) 	  */
    PWM_IRQn             = 31, /*!< 31 Pulse Width Modulation (PWM)    		  */
    CRCCU_IRQn           = 32, /*!< 32 CRC Calculation Unit (CRCCU) 		  */
    ACC_IRQn             = 33, /*!< 33 Analog Comparator (ACC) 				  */
    UDP_IRQn             = 34, /*!< 34 USB Device Port (UDP) 				  */
    KEY_DRV_INDX         = 35, /*!< 35 Key Driver			 				  */
//    GUI_DRV_INDX         = 36, /*!< 36 GUI Driver			 				  */
    INALID_DRV_INDX	           /*!<  invalid	                              */
};







#endif /*BRD_CPP_H_*/
