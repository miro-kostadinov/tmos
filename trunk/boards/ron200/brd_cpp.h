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
    SUPC_IRQn   			=  0,   /*!<  Supply Controller 				 */
    RSTC_IRQn   	   		=  1,   /*!<  Reset Controller 					 */
    RTC_IRQn           		=  2,   /*!<  Real Time Clock 					 */
    RTT_IRQn            	=  3,   /*!<  Real Time Timer 					 */
    WDT_IRQn            	=  4,   /*!<  Watchdog Timer 					 */
    PMC_IRQn            	=  5,   /*!<  Power Management Controller 		 */
    EFC_IRQn            	=  6,   /*!<  Enhanced Embedded Flash Controller */
    UART0_IRQn         		=  7,   /*!<  UART 0 		 					 */
    UART1_IRQn          	=  8,   /*!<  UART 1 							 */
    SMC_IRQn            	=  9,   /*!<  Static Memory Controller 			 */
    PIOA_IRQn           	= 10,   /*!<  Parallel I/O Controller A 		 */
    PIOB_IRQn           	= 11,   /*!<  Parallel I/O Controller B 		 */
    PIOC_IRQn           	= 12,   /*!<  Parallel I/O Controller C 		 */
    USART0_IRQn         	= 13,   /*!<  USART 0 							 */
    USART1_IRQn         	= 14,   /*!<  USART 1 							 */
    HSMCI_IRQn          	= 15,   /*!<  Multimedia Card Interface 		 */
    TWI0_IRQn           	= 16,   /*!<  Two Wire Interface 0 				 */
    TWI1_IRQn           	= 17,   /*!<  Two Wire Interface 1 				 */
    SPI_IRQn            	= 18,   /*!<  Serial Peripheral Interface 		 */
    SSC_IRQn            	= 19,   /*!<  Synchronous Serial Controler 		 */
    TC0_IRQn            	= 20,   /*!<  Timer/Counter 0 					 */
    TC1_IRQn            	= 21,   /*!<  Timer/Counter 1 					 */
    TC2_IRQn            	= 22,   /*!<  Timer/Counter 2 					 */
    TC3_IRQn            	= 23,   /*!<  Timer/Counter 3 					 */
    TC4_IRQn            	= 24,   /*!<  Timer/Counter 4 					 */
    TC5_IRQn            	= 25,   /*!<  Timer/Counter 5 					 */
    ADC_IRQn            	= 26,   /*!<  Analog To Digital Converter 		 */
    DACC_IRQn           	= 27,   /*!<  Digital To Analog Converter 		 */
    PWM_IRQn            	= 28,   /*!<  Pulse Width Modulation 			 */
    CRCCU_IRQn          	= 29,   /*!<  CRC Calculation Unit 				 */
    ACC_IRQn            	= 30,   /*!<  Analog Comparator 				 */
    UDP_IRQn            	= 31,   /*!<  USB Device Port 					 */
    INALID_DRV_INDX	                /*!<  invalid	                         */
};







#endif /*BRD_CPP_H_*/
