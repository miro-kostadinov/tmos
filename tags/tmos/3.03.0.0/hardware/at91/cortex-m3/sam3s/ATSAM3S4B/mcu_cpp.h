/**************************************************************************//**
 * @file     hardware/at91/cortex-m3/sam3s/ATSAM3S4B/mcu_cpp.h
 * @ingroup	 ATSAM3S4B
 * @brief    ATSAM3S4B header
 * @version  V3.00
 * @date     25. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#ifndef MCU_CPP_H_
#define MCU_CPP_H_

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR ATSAM3S4B */
/* ************************************************************************** */
/**
 * @ingroup	 ATSAM3S4B
 * \addtogroup SAM3S_base Peripheral Base Address Definitions
 * @{
 */
#define HSMCI      CAST(Hsmci     , 0x40000000U) //!< (HSMCI     ) Base Address
#define PDC_HSMCI  CAST(Pdc       , 0x40000100U) //!< (PDC_HSMCI ) Base Address
#define SSC        CAST(Ssc       , 0x40004000U) //!< (SSC       ) Base Address
#define PDC_SSC    CAST(Pdc       , 0x40004100U) //!< (PDC_SSC   ) Base Address
#define SPI        CAST(Spi       , 0x40008000U) //!< (SPI       ) Base Address
#define PDC_SPI    CAST(Pdc       , 0x40008100U) //!< (PDC_SPI   ) Base Address
#define TC0        CAST(Tc        , 0x40010000U) //!< (TC0       ) Base Address
#define TC1        CAST(Tc        , 0x40014000U) //!< (TC1       ) Base Address
#define TWI0       CAST(Twi       , 0x40018000U) //!< (TWI0      ) Base Address
#define PDC_TWI0   CAST(Pdc       , 0x40018100U) //!< (PDC_TWI0  ) Base Address
#define TWI1       CAST(Twi       , 0x4001C000U) //!< (TWI1      ) Base Address
#define PDC_TWI1   CAST(Pdc       , 0x4001C100U) //!< (PDC_TWI1  ) Base Address
#define PWM        CAST(Pwm       , 0x40020000U) //!< (PWM       ) Base Address
#define PDC_PWM    CAST(Pdc       , 0x40020100U) //!< (PDC_PWM   ) Base Address
#define USART0     CAST(Usart     , 0x40024000U) //!< (USART0    ) Base Address
#define PDC_USART0 CAST(Pdc       , 0x40024100U) //!< (PDC_USART0) Base Address
#define USART1     CAST(Usart     , 0x40028000U) //!< (USART1    ) Base Address
#define PDC_USART1 CAST(Pdc       , 0x40028100U) //!< (PDC_USART1) Base Address
#define UDP        CAST(Udp       , 0x40034000U) //!< (UDP       ) Base Address
#define ADC        CAST(Adc       , 0x40038000U) //!< (ADC       ) Base Address
#define PDC_ADC    CAST(Pdc       , 0x40038100U) //!< (PDC_ADC   ) Base Address
#define DACC       CAST(Dacc      , 0x4003C000U) //!< (DACC      ) Base Address
#define PDC_DACC   CAST(Pdc       , 0x4003C100U) //!< (PDC_DACC  ) Base Address
#define ACC        CAST(Acc       , 0x40040000U) //!< (ACC       ) Base Address
#define CRCCU      CAST(Crccu     , 0x40044000U) //!< (CRCCU     ) Base Address
#define SMC        CAST(Smc       , 0x400E0000U) //!< (SMC       ) Base Address
#define MATRIX     CAST(Matrix    , 0x400E0200U) //!< (MATRIX    ) Base Address
#define PMC        CAST(Pmc       , 0x400E0400U) //!< (PMC       ) Base Address
#define UART0      CAST(Uart      , 0x400E0600U) //!< (UART0     ) Base Address
#define PDC_UART0  CAST(Pdc       , 0x400E0700U) //!< (PDC_UART0 ) Base Address
#define CHIPID     CAST(Chipid    , 0x400E0740U) //!< (CHIPID    ) Base Address
#define UART1      CAST(Uart      , 0x400E0800U) //!< (UART1     ) Base Address
#define PDC_UART1  CAST(Pdc       , 0x400E0900U) //!< (PDC_UART1 ) Base Address
#define EFC        CAST(Efc       , 0x400E0A00U) //!< (EFC       ) Base Address
#define PIOA       CAST(Pio       , 0x400E0E00U) //!< (PIOA      ) Base Address
#define PDC_PIOA   CAST(Pdc       , 0x400E0F00U) //!< (PDC_PIOA  ) Base Address
#define PIOB       CAST(Pio       , 0x400E1000U) //!< (PIOB      ) Base Address
#define PIOC       CAST(Pio       , 0x400E1200U) //!< (PIOC      ) Base Address
#define RSTC       CAST(Rstc      , 0x400E1400U) //!< (RSTC      ) Base Address
#define SUPC       CAST(Supc      , 0x400E1410U) //!< (SUPC      ) Base Address
#define RTT        CAST(Rtt       , 0x400E1430U) //!< (RTT       ) Base Address
#define WDT        CAST(Wdt       , 0x400E1450U) //!< (WDT       ) Base Address
#define RTC        CAST(Rtc       , 0x400E1460U) //!< (RTC       ) Base Address
#define GPBR       CAST(Gpbr      , 0x400E1490U) //!< (GPBR      ) Base Address
/** @} */

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR SAM3S */
/* ************************************************************************** */
/**
 * @ingroup	 ATSAM3S4B
 * \addtogroup SAM3S_id Peripheral Ids Definitions
 * @{
 */
#define ID_SUPC   ( 0) //!< Supply Controller (SUPC) */
#define ID_RSTC   ( 1) //!< Reset Controller (RSTC) */
#define ID_RTC    ( 2) //!< Real Time Clock (RTC) */
#define ID_RTT    ( 3) //!< Real Time Timer (RTT) */
#define ID_WDT    ( 4) //!< Watchdog Timer (WDT) */
#define ID_PMC    ( 5) //!< Power Management Controller (PMC) */
#define ID_EFC    ( 6) //!< Enhanced Embedded Flash Controller (EFC) */
#define ID_UART0  ( 8) //!< UART 0 (UART0) */
#define ID_UART1  ( 9) //!< UART 1 (UART1) */
#define ID_SMC    (10) //!< Static Memory Controller (SMC) */
#define ID_PIOA   (11) //!< Parallel I/O Controller A (PIOA) */
#define ID_PIOB   (12) //!< Parallel I/O Controller B (PIOB) */
#define ID_PIOC   (13) //!< Parallel I/O Controller C (PIOC) */
#define ID_USART0 (14) //!< USART 0 (USART0) */
#define ID_USART1 (15) //!< USART 1 (USART1) */
#define ID_HSMCI  (18) //!< Multimedia Card Interface (HSMCI) */
#define ID_TWI0   (19) //!< Two Wire Interface 0 (TWI0) */
#define ID_TWI1   (20) //!< Two Wire Interface 1 (TWI1) */
#define ID_SPI    (21) //!< Serial Peripheral Interface (SPI) */
#define ID_SSC    (22) //!< Synchronous Serial Controler (SSC) */
#define ID_TC0    (23) //!< Timer/Counter 0 (TC0) */
#define ID_TC1    (24) //!< Timer/Counter 1 (TC1) */
#define ID_TC2    (25) //!< Timer/Counter 2 (TC2) */
#define ID_TC3    (26) //!< Timer/Counter 3 (TC3) */
#define ID_TC4    (27) //!< Timer/Counter 4 (TC4) */
#define ID_TC5    (28) //!< Timer/Counter 5 (TC5) */
#define ID_ADC    (29) //!< Analog To Digital Converter (ADC) */
#define ID_DACC   (30) //!< Digital To Analog Converter (DACC) */
#define ID_PWM    (31) //!< Pulse Width Modulation (PWM) */
#define ID_CRCCU  (32) //!< CRC Calculation Unit (CRCCU) */
#define ID_ACC    (33) //!< Analog Comparator (ACC) */
#define ID_UDP    (34) //!< USB Device Port (UDP) */

#define ID_NO_PERIPHERAL (255) //!< invalid index for peripheral
/** @}*/

#endif /* MCU_CPP_H_ */
