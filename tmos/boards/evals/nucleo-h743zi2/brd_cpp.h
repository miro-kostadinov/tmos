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

enum DRIVER_INDEX : signed short
{
/******  Cortex-M3 Processor Exceptions Numbers *******************************/
    NonMaskableInt_IRQn     = -14,  /*!< 2 Non Maskable Interrupt             */
    MemoryManagement_IRQn   = -12,  /*!< 4 Cortex-M Memory Management Int     */
    BusFault_IRQn           = -11,  /*!< 5 Cortex-M Bus Fault Interrupt       */
    UsageFault_IRQn         = -10,  /*!< 6 Cortex-M Usage Fault Interrupt     */
    SVCall_IRQn             = -5,   /*!< 11 Cortex-M SV Call Interrupt        */
    DebugMonitor_IRQn       = -4,   /*!< 12 Cortex-M Debug Monitor Interrupt  */
    PendSV_IRQn             = -2,   /*!< 14 Cortex-M Pend SV Interrupt        */
    SysTick_IRQn            = -1,   /*!< 15 Cortex-M System Tick Interrupt    */

    /******  STM32 specific Interrupt Numbers *********************************/
	WWDG_IRQn               = 0,    /*!< Window WatchDog Interrupt ( wwdg1_it, wwdg2_it) */
	PVD_AVD_IRQn            = 1,    /*!< PVD/AVD through EXTI Line detection Interrupt */
	TAMP_STAMP_IRQn         = 2,    /*!< Tamper and TimeStamp interrupts through the EXTI line */
	RTC_WKUP_IRQn           = 3,    /*!< RTC Wakeup interrupt through the EXTI line*/
	FLASH_IRQn              = 4,    /*!< FLASH global Interrupt               */
	RCC_IRQn                = 5,    /*!< RCC global Interrupt                 */
	EXTI0_IRQn              = 6,    /*!< EXTI Line0 Interrupt                 */
	EXTI1_IRQn              = 7,    /*!< EXTI Line1 Interrupt                 */
	EXTI2_IRQn              = 8,    /*!< EXTI Line2 Interrupt                 */
	EXTI3_IRQn              = 9,    /*!< EXTI Line3 Interrupt                 */
	EXTI4_IRQn              = 10,   /*!< EXTI Line4 Interrupt                 */
	DMA1_Stream0_IRQn       = 11,   /*!< DMA1 Stream 0 global Interrupt       */
	DMA1_Stream1_IRQn       = 12,   /*!< DMA1 Stream 1 global Interrupt       */
	DMA1_Stream2_IRQn       = 13,   /*!< DMA1 Stream 2 global Interrupt       */
	DMA1_Stream3_IRQn       = 14,   /*!< DMA1 Stream 3 global Interrupt       */
	DMA1_Stream4_IRQn       = 15,   /*!< DMA1 Stream 4 global Interrupt       */
	DMA1_Stream5_IRQn       = 16,   /*!< DMA1 Stream 5 global Interrupt       */
	DMA1_Stream6_IRQn       = 17,   /*!< DMA1 Stream 6 global Interrupt       */
	ADC_IRQn                = 18,   /*!< ADC1 and  ADC2 global Interrupts     */
	FDCAN1_IT0_IRQn         = 19,   /*!< FDCAN1 Interrupt line 0              */
	FDCAN2_IT0_IRQn         = 20,   /*!< FDCAN2 Interrupt line 0              */
	FDCAN1_IT1_IRQn         = 21,   /*!< FDCAN1 Interrupt line 1              */
	FDCAN2_IT1_IRQn         = 22,   /*!< FDCAN2 Interrupt line 1              */
	EXTI9_5_IRQn            = 23,   /*!< External Line[9:5] Interrupts        */
	TIM1_BRK_IRQn           = 24,   /*!< TIM1 Break Interrupt                 */
	TIM1_UP_IRQn            = 25,   /*!< TIM1 Update Interrupt                */
	TIM1_TRG_COM_IRQn       = 26,   /*!< TIM1 Trigger and Commutation Interrupt */
	TIM1_CC_IRQn            = 27,   /*!< TIM1 Capture Compare Interrupt       */
	TIM2_IRQn               = 28,   /*!< TIM2 global Interrupt                */
	TIM3_IRQn               = 29,   /*!< TIM3 global Interrupt                */
	TIM4_IRQn               = 30,   /*!< TIM4 global Interrupt                */
	I2C1_EV_IRQn            = 31,   /*!< I2C1 Event Interrupt                 */
	I2C1_ER_IRQn            = 32,   /*!< I2C1 Error Interrupt                 */
	I2C2_EV_IRQn            = 33,   /*!< I2C2 Event Interrupt                 */
	I2C2_ER_IRQn            = 34,   /*!< I2C2 Error Interrupt                 */
	SPI1_IRQn               = 35,   /*!< SPI1 global Interrupt                */
	SPI2_IRQn               = 36,   /*!< SPI2 global Interrupt                */
	USART1_IRQn             = 37,   /*!< USART1 global Interrupt              */
	USART2_IRQn             = 38,   /*!< USART2 global Interrupt              */
	USART3_IRQn             = 39,   /*!< USART3 global Interrupt              */
	EXTI15_10_IRQn          = 40,   /*!< External Line[15:10] Interrupts      */
	RTC_Alarm_IRQn          = 41,   /*!< RTC Alarm (A and B) through EXTI Line Interrupt */
	TIM8_BRK_TIM12_IRQn     = 43,   /*!< TIM8 Break Interrupt and TIM12 global interrupt */
	TIM8_UP_TIM13_IRQn      = 44,   /*!< TIM8 Update Interrupt and TIM13 global interrupt */
	TIM8_TRG_COM_TIM14_IRQn = 45,   /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
	TIM8_CC_IRQn            = 46,   /*!< TIM8 Capture Compare Interrupt       */
	DMA1_Stream7_IRQn       = 47,   /*!< DMA1 Stream7 Interrupt               */
	FMC_IRQn                = 48,   /*!< FMC global Interrupt                 */
	SDMMC1_IRQn             = 49,   /*!< SDMMC1 global Interrupt              */
	TIM5_IRQn               = 50,   /*!< TIM5 global Interrupt                */
	SPI3_IRQn               = 51,   /*!< SPI3 global Interrupt                */
	UART4_IRQn              = 52,   /*!< UART4 global Interrupt               */
	UART5_IRQn              = 53,   /*!< UART5 global Interrupt               */
	TIM6_DAC_IRQn           = 54,   /*!< TIM6 global and DAC1&2 underrun error interrupts */
	TIM7_IRQn               = 55,   /*!< TIM7 global interrupt                */
	DMA2_Stream0_IRQn       = 56,   /*!< DMA2 Stream 0 global Interrupt       */
	DMA2_Stream1_IRQn       = 57,   /*!< DMA2 Stream 1 global Interrupt       */
	DMA2_Stream2_IRQn       = 58,   /*!< DMA2 Stream 2 global Interrupt       */
	DMA2_Stream3_IRQn       = 59,   /*!< DMA2 Stream 3 global Interrupt       */
	DMA2_Stream4_IRQn       = 60,   /*!< DMA2 Stream 4 global Interrupt       */
	ETH_IRQn                = 61,   /*!< Ethernet global Interrupt            */
	ETH_WKUP_IRQn           = 62,   /*!< Ethernet Wakeup through EXTI line Interrupt */
	FDCAN_CAL_IRQn          = 63,   /*!< FDCAN Calibration unit Interrupt     */
	DMA2_Stream5_IRQn       = 68,   /*!< DMA2 Stream 5 global interrupt       */
	DMA2_Stream6_IRQn       = 69,   /*!< DMA2 Stream 6 global interrupt       */
	DMA2_Stream7_IRQn       = 70,   /*!< DMA2 Stream 7 global interrupt       */
	USART6_IRQn             = 71,   /*!< USART6 global interrupt              */
	I2C3_EV_IRQn            = 72,   /*!< I2C3 event interrupt                 */
	I2C3_ER_IRQn            = 73,   /*!< I2C3 error interrupt                 */
	OTG_HS_EP1_OUT_IRQn     = 74,   /*!< USB OTG HS End Point 1 Out global interrupt */
	OTG_HS_EP1_IN_IRQn      = 75,   /*!< USB OTG HS End Point 1 In global interrupt */
	OTG_HS_WKUP_IRQn        = 76,   /*!< USB OTG HS Wakeup through EXTI interrupt */
	OTG_HS_IRQn             = 77,   /*!< USB OTG HS global interrupt          */
	DCMI_IRQn               = 78,   /*!< DCMI global interrupt                */
	RNG_IRQn                = 80,   /*!< RNG global interrupt                 */
	FPU_IRQn                = 81,   /*!< FPU global interrupt                 */
	UART7_IRQn              = 82,   /*!< UART7 global interrupt               */
	UART8_IRQn              = 83,   /*!< UART8 global interrupt               */
	SPI4_IRQn               = 84,   /*!< SPI4 global Interrupt                */
	SPI5_IRQn               = 85,   /*!< SPI5 global Interrupt                */
	SPI6_IRQn               = 86,   /*!< SPI6 global Interrupt                */
	SAI1_IRQn               = 87,   /*!< SAI1 global Interrupt                */
	LTDC_IRQn               = 88,   /*!< LTDC global Interrupt                */
	LTDC_ER_IRQn            = 89,   /*!< LTDC Error global Interrupt          */
	DMA2D_IRQn              = 90,   /*!< DMA2D global Interrupt               */
	SAI2_IRQn               = 91,   /*!< SAI2 global Interrupt                */
	QUADSPI_IRQn            = 92,   /*!< Quad SPI global interrupt            */
	LPTIM1_IRQn             = 93,   /*!< LP TIM1 interrupt                    */
	CEC_IRQn                = 94,   /*!< HDMI-CEC global Interrupt            */
	I2C4_EV_IRQn            = 95,   /*!< I2C4 Event Interrupt                 */
	I2C4_ER_IRQn            = 96,   /*!< I2C4 Error Interrupt                 */
	SPDIF_RX_IRQn           = 97,   /*!< SPDIF-RX global Interrupt            */
	OTG_FS_EP1_OUT_IRQn     = 98,   /*!< USB OTG HS2 global interrupt         */
	OTG_FS_EP1_IN_IRQn      = 99,   /*!< USB OTG HS2 End Point 1 Out global interrupt */
	OTG_FS_WKUP_IRQn        = 100,  /*!< USB OTG HS2 End Point 1 In global interrupt */
	OTG_FS_IRQn             = 101,  /*!< USB OTG HS2 Wakeup through EXTI interrupt */
	DMAMUX1_OVR_IRQn        = 102,  /*!< DMAMUX1 Overrun interrupt            */
	HRTIM1_Master_IRQn      = 103,  /*!< HRTIM Master Timer global Interrupts */
	HRTIM1_TIMA_IRQn        = 104,  /*!< HRTIM Timer A global Interrupt       */
	HRTIM1_TIMB_IRQn        = 105,  /*!< HRTIM Timer B global Interrupt       */
	HRTIM1_TIMC_IRQn        = 106,  /*!< HRTIM Timer C global Interrupt       */
	HRTIM1_TIMD_IRQn        = 107,  /*!< HRTIM Timer D global Interrupt       */
	HRTIM1_TIME_IRQn        = 108,  /*!< HRTIM Timer E global Interrupt       */
	HRTIM1_FLT_IRQn         = 109,  /*!< HRTIM Fault global Interrupt         */
	DFSDM1_FLT0_IRQn        = 110,  /*!< DFSDM Filter1 Interrupt              */
	DFSDM1_FLT1_IRQn        = 111,  /*!< DFSDM Filter2 Interrupt              */
	DFSDM1_FLT2_IRQn        = 112,  /*!< DFSDM Filter3 Interrupt              */
	DFSDM1_FLT3_IRQn        = 113,  /*!< DFSDM Filter4 Interrupt              */
	SAI3_IRQn               = 114,  /*!< SAI3 global Interrupt                */
	SWPMI1_IRQn             = 115,  /*!< Serial Wire Interface 1 global interrupt */
	TIM15_IRQn              = 116,  /*!< TIM15 global Interrupt               */
	TIM16_IRQn              = 117,  /*!< TIM16 global Interrupt               */
	TIM17_IRQn              = 118,  /*!< TIM17 global Interrupt               */
	MDIOS_WKUP_IRQn         = 119,  /*!< MDIOS Wakeup  Interrupt              */
	MDIOS_IRQn              = 120,  /*!< MDIOS global Interrupt               */
	JPEG_IRQn               = 121,  /*!< JPEG global Interrupt                */
	MDMA_IRQn               = 122,  /*!< MDMA global Interrupt                */
	SDMMC2_IRQn             = 124,  /*!< SDMMC2 global Interrupt              */
	HSEM1_IRQn              = 125,  /*!< HSEM1 global Interrupt               */
	ADC3_IRQn               = 127,  /*!< ADC3 global Interrupt                */
	DMAMUX2_OVR_IRQn        = 128,  /*!< DMAMUX2 Overrun interrupt            */
	BDMA_Channel0_IRQn      = 129,  /*!< BDMA Channel 0 global Interrupt      */
	BDMA_Channel1_IRQn      = 130,  /*!< BDMA Channel 1 global Interrupt      */
	BDMA_Channel2_IRQn      = 131,  /*!< BDMA Channel 2 global Interrupt      */
	BDMA_Channel3_IRQn      = 132,  /*!< BDMA Channel 3 global Interrupt      */
	BDMA_Channel4_IRQn      = 133,  /*!< BDMA Channel 4 global Interrupt      */
	BDMA_Channel5_IRQn      = 134,  /*!< BDMA Channel 5 global Interrupt      */
	BDMA_Channel6_IRQn      = 135,  /*!< BDMA Channel 6 global Interrupt      */
	BDMA_Channel7_IRQn      = 136,  /*!< BDMA Channel 7 global Interrupt      */
	COMP_IRQn               = 137,  /*!< COMP global Interrupt                */
	LPTIM2_IRQn             = 138,  /*!< LP TIM2 global interrupt             */
	LPTIM3_IRQn             = 139,  /*!< LP TIM3 global interrupt             */
	LPTIM4_IRQn             = 140,  /*!< LP TIM4 global interrupt             */
	LPTIM5_IRQn             = 141,  /*!< LP TIM5 global interrupt             */
	LPUART1_IRQn            = 142,  /*!< LP UART1 interrupt                   */
	CRS_IRQn                = 144,  /*!< Clock Recovery Global Interrupt      */
	ECC_IRQn                = 145,  /*!< ECC diagnostic Global Interrupt      */
	SAI4_IRQn               = 146,  /*!< SAI4 global interrupt                */
	WAKEUP_PIN_IRQn         = 149,  /*!< Interrupt for all 6 wake-up pins     */

    INALID_DRV_INDX	                /*!<  invalid	                          */
};

}

#else

typedef signed char DRIVER_INDEX;

#endif




#endif /*BRD_CPP_H_*/
