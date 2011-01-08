/**************************************************************************//**
 * @ingroup	 hardware_sam3s
 * @{
 * @file     hardware/at91/cortex-m3/sam3s/series_cpp.h
 * @brief    SAM3S series CPP header
 * @version  V3.00
 * @date     11. November 2010
 * @author	 Miroslav Kostadinov
 *
 *
 *
 ******************************************************************************/
#ifndef SERIES_CPP_H_
#define SERIES_CPP_H_

#include "series_inc.h"
#include <tmos_types.h>
#include <at91_types.h>

#include "mcu_cpp.h"

#include "acc.h"
#include "adc.h"
#include "dacc.h"

#include "series_map.h"

/* ************************************************************************** */
/*   PIO DEFINITIONS FOR SAM3S */
/* ************************************************************************** */
#define PIO_PA0                (1u << 0) //!< Pin Controlled by PA0 */
#define PIO_PA1                (1u << 1) //!< Pin Controlled by PA1 */
#define PIO_PA2                (1u << 2) //!< Pin Controlled by PA2 */
#define PIO_PA3                (1u << 3) //!< Pin Controlled by PA3 */
#define PIO_PA4                (1u << 4) //!< Pin Controlled by PA4 */
#define PIO_PA5                (1u << 5) //!< Pin Controlled by PA5 */
#define PIO_PA6                (1u << 6) //!< Pin Controlled by PA6 */
#define PIO_PA7                (1u << 7) //!< Pin Controlled by PA7 */
#define PIO_PA8                (1u << 8) //!< Pin Controlled by PA8 */
#define PIO_PA9                (1u << 9) //!< Pin Controlled by PA9 */
#define PIO_PA10               (1u << 10) //!< Pin Controlled by PA10 */
#define PIO_PA11               (1u << 11) //!< Pin Controlled by PA11 */
#define PIO_PA12               (1u << 12) //!< Pin Controlled by PA12 */
#define PIO_PA13               (1u << 13) //!< Pin Controlled by PA13 */
#define PIO_PA14               (1u << 14) //!< Pin Controlled by PA14 */
#define PIO_PA15               (1u << 15) //!< Pin Controlled by PA15 */
#define PIO_PA16               (1u << 16) //!< Pin Controlled by PA16 */
#define PIO_PA17               (1u << 17) //!< Pin Controlled by PA17 */
#define PIO_PA18               (1u << 18) //!< Pin Controlled by PA18 */
#define PIO_PA19               (1u << 19) //!< Pin Controlled by PA19 */
#define PIO_PA20               (1u << 20) //!< Pin Controlled by PA20 */
#define PIO_PA21               (1u << 21) //!< Pin Controlled by PA21 */
#define PIO_PA22               (1u << 22) //!< Pin Controlled by PA22 */
#define PIO_PA23               (1u << 23) //!< Pin Controlled by PA23 */
#define PIO_PA24               (1u << 24) //!< Pin Controlled by PA24 */
#define PIO_PA25               (1u << 25) //!< Pin Controlled by PA25 */
#define PIO_PA26               (1u << 26) //!< Pin Controlled by PA26 */
#define PIO_PA27               (1u << 27) //!< Pin Controlled by PA27 */
#define PIO_PA28               (1u << 28) //!< Pin Controlled by PA28 */
#define PIO_PA29               (1u << 29) //!< Pin Controlled by PA29 */
#define PIO_PA30               (1u << 30) //!< Pin Controlled by PA30 */
#define PIO_PA31               (1u << 31) //!< Pin Controlled by PA31 */
#define PIO_PB0                (1u << 0) //!< Pin Controlled by PB0 */
#define PIO_PB1                (1u << 1) //!< Pin Controlled by PB1 */
#define PIO_PB2                (1u << 2) //!< Pin Controlled by PB2 */
#define PIO_PB3                (1u << 3) //!< Pin Controlled by PB3 */
#define PIO_PB4                (1u << 4) //!< Pin Controlled by PB4 */
#define PIO_PB5                (1u << 5) //!< Pin Controlled by PB5 */
#define PIO_PB6                (1u << 6) //!< Pin Controlled by PB6 */
#define PIO_PB7                (1u << 7) //!< Pin Controlled by PB7 */
#define PIO_PB8                (1u << 8) //!< Pin Controlled by PB8 */
#define PIO_PB9                (1u << 9) //!< Pin Controlled by PB9 */
#define PIO_PB10               (1u << 10) //!< Pin Controlled by PB10 */
#define PIO_PB11               (1u << 11) //!< Pin Controlled by PB11 */
#define PIO_PB12               (1u << 12) //!< Pin Controlled by PB12 */
#define PIO_PB13               (1u << 13) //!< Pin Controlled by PB13 */
#define PIO_PB14               (1u << 14) //!< Pin Controlled by PB14 */
#define PIO_PC0                (1u << 0) //!< Pin Controlled by PC0 */
#define PIO_PC1                (1u << 1) //!< Pin Controlled by PC1 */
#define PIO_PC2                (1u << 2) //!< Pin Controlled by PC2 */
#define PIO_PC3                (1u << 3) //!< Pin Controlled by PC3 */
#define PIO_PC4                (1u << 4) //!< Pin Controlled by PC4 */
#define PIO_PC5                (1u << 5) //!< Pin Controlled by PC5 */
#define PIO_PC6                (1u << 6) //!< Pin Controlled by PC6 */
#define PIO_PC7                (1u << 7) //!< Pin Controlled by PC7 */
#define PIO_PC8                (1u << 8) //!< Pin Controlled by PC8 */
#define PIO_PC9                (1u << 9) //!< Pin Controlled by PC9 */
#define PIO_PC10               (1u << 10) //!< Pin Controlled by PC10 */
#define PIO_PC11               (1u << 11) //!< Pin Controlled by PC11 */
#define PIO_PC12               (1u << 12) //!< Pin Controlled by PC12 */
#define PIO_PC13               (1u << 13) //!< Pin Controlled by PC13 */
#define PIO_PC14               (1u << 14) //!< Pin Controlled by PC14 */
#define PIO_PC15               (1u << 15) //!< Pin Controlled by PC15 */
#define PIO_PC16               (1u << 16) //!< Pin Controlled by PC16 */
#define PIO_PC17               (1u << 17) //!< Pin Controlled by PC17 */
#define PIO_PC18               (1u << 18) //!< Pin Controlled by PC18 */
#define PIO_PC19               (1u << 19) //!< Pin Controlled by PC19 */
#define PIO_PC20               (1u << 20) //!< Pin Controlled by PC20 */
#define PIO_PC21               (1u << 21) //!< Pin Controlled by PC21 */
#define PIO_PC22               (1u << 22) //!< Pin Controlled by PC22 */
#define PIO_PC23               (1u << 23) //!< Pin Controlled by PC23 */
#define PIO_PC24               (1u << 24) //!< Pin Controlled by PC24 */
#define PIO_PC25               (1u << 25) //!< Pin Controlled by PC25 */
#define PIO_PC26               (1u << 26) //!< Pin Controlled by PC26 */
#define PIO_PC27               (1u << 27) //!< Pin Controlled by PC27 */
#define PIO_PC28               (1u << 28) //!< Pin Controlled by PC28 */
#define PIO_PC29               (1u << 29) //!< Pin Controlled by PC29 */
#define PIO_PC30               (1u << 30) //!< Pin Controlled by PC30 */
#define PIO_PC31               (1u << 31) //!< Pin Controlled by PC31 */
/* ========== Pio definition for HSMCI peripheral ========== */
#define PIO_PA28C_MCCDA      (1u << 28)  //!< Hsmci signal: MCCDA */
#define PIO_PA29C_MCCK       (1u << 29)  //!< Hsmci signal: MCCK */
#define PIO_PA30C_MCDA0      (1u << 30)  //!< Hsmci signal: MCDA0 */
#define PIO_PA31C_MCDA1      (1u << 31)  //!< Hsmci signal: MCDA1 */
#define PIO_PA26C_MCDA2      (1u << 26)  //!< Hsmci signal: MCDA2 */
#define PIO_PA27C_MCDA3      (1u << 27)  //!< Hsmci signal: MCDA3 */
/* ========== Pio definition for SSC peripheral ========== */
#define PIO_PA18A_RD         (1u << 18)  //!< Ssc signal: RD */
#define PIO_PA20A_RF         (1u << 20)  //!< Ssc signal: RF */
#define PIO_PA19A_RK         (1u << 19)  //!< Ssc signal: RK */
#define PIO_PA17A_TD         (1u << 17)  //!< Ssc signal: TD */
#define PIO_PA15A_TF         (1u << 15)  //!< Ssc signal: TF */
#define PIO_PA16A_TK         (1u << 16)  //!< Ssc signal: TK */
/* ========== Pio definition for SPI peripheral ========== */
#define PIO_PA12A_MISO       (1u << 12)  //!< Spi signal: MISO */
#define PIO_PA13A_MOSI       (1u << 13)  //!< Spi signal: MOSI */
#define PIO_PA11A_NPCS0      (1u << 11)  //!< Spi signal: NPCS0 */
#define PIO_PA9B_NPCS1       (1u << 9)   //!< Spi signal: NPCS1 */
#define PIO_PA31A_NPCS1      (1u << 31)  //!< Spi signal: NPCS1 */
#define PIO_PB14A_NPCS1      (1u << 14)  //!< Spi signal: NPCS1 */
#define PIO_PC4B_NPCS1       (1u << 4)   //!< Spi signal: NPCS1 */
#define PIO_PA10B_NPCS2      (1u << 10)  //!< Spi signal: NPCS2 */
#define PIO_PA30B_NPCS2      (1u << 30)  //!< Spi signal: NPCS2 */
#define PIO_PB2B_NPCS2       (1u << 2)   //!< Spi signal: NPCS2 */
#define PIO_PA3B_NPCS3       (1u << 3)   //!< Spi signal: NPCS3 */
#define PIO_PA5B_NPCS3       (1u << 5)   //!< Spi signal: NPCS3 */
#define PIO_PA22B_NPCS3      (1u << 22)  //!< Spi signal: NPCS3 */
#define PIO_PA14A_SPCK       (1u << 14)  //!< Spi signal: SPCK */
/* ========== Pio definition for TC0 peripheral ========== */
#define PIO_PA4B_TCLK0       (1u << 4)   //!< Tc0 signal: TCLK0 */
#define PIO_PA28B_TCLK1      (1u << 28)  //!< Tc0 signal: TCLK1 */
#define PIO_PA29B_TCLK2      (1u << 29)  //!< Tc0 signal: TCLK2 */
#define PIO_PA0B_TIOA0       (1u << 0)   //!< Tc0 signal: TIOA0 */
#define PIO_PA15B_TIOA1      (1u << 15)  //!< Tc0 signal: TIOA1 */
#define PIO_PA26B_TIOA2      (1u << 26)  //!< Tc0 signal: TIOA2 */
#define PIO_PA1B_TIOB0       (1u << 1)   //!< Tc0 signal: TIOB0 */
#define PIO_PA16B_TIOB1      (1u << 16)  //!< Tc0 signal: TIOB1 */
#define PIO_PA27B_TIOB2      (1u << 27)  //!< Tc0 signal: TIOB2 */
/* ========== Pio definition for TC1 peripheral ========== */
#define PIO_PC25B_TCLK3      (1u << 25)  //!< Tc1 signal: TCLK3 */
#define PIO_PC28B_TCLK4      (1u << 28)  //!< Tc1 signal: TCLK4 */
#define PIO_PC31B_TCLK5      (1u << 31)  //!< Tc1 signal: TCLK5 */
#define PIO_PC23B_TIOA3      (1u << 23)  //!< Tc1 signal: TIOA3 */
#define PIO_PC26B_TIOA4      (1u << 26)  //!< Tc1 signal: TIOA4 */
#define PIO_PC29B_TIOA5      (1u << 29)  //!< Tc1 signal: TIOA5 */
#define PIO_PC24B_TIOB3      (1u << 24)  //!< Tc1 signal: TIOB3 */
#define PIO_PC27B_TIOB4      (1u << 27)  //!< Tc1 signal: TIOB4 */
#define PIO_PC30B_TIOB5      (1u << 30)  //!< Tc1 signal: TIOB5 */
/* ========== Pio definition for TWI0 peripheral ========== */
#define PIO_PA4A_TWCK0       (1u << 4)   //!< Twi0 signal: TWCK0 */
#define PIO_PA3A_TWD0        (1u << 3)   //!< Twi0 signal: TWD0 */
/* ========== Pio definition for TWI1 peripheral ========== */
#define PIO_PB5A_TWCK1       (1u << 5)   //!< Twi1 signal: TWCK1 */
#define PIO_PB4A_TWD1        (1u << 4)   //!< Twi1 signal: TWD1 */
/* ========== Pio definition for PWM peripheral ========== */
#define PIO_PA9C_PWMFI0      (1u << 9)   //!< Pwm signal: PWMFI0 */
#define PIO_PA0A_PWMH0       (1u << 0)   //!< Pwm signal: PWMH0 */
#define PIO_PA11B_PWMH0      (1u << 11)  //!< Pwm signal: PWMH0 */
#define PIO_PA23B_PWMH0      (1u << 23)  //!< Pwm signal: PWMH0 */
#define PIO_PB0A_PWMH0       (1u << 0)   //!< Pwm signal: PWMH0 */
#define PIO_PC18B_PWMH0      (1u << 18)  //!< Pwm signal: PWMH0 */
#define PIO_PA1A_PWMH1       (1u << 1)   //!< Pwm signal: PWMH1 */
#define PIO_PA12B_PWMH1      (1u << 12)  //!< Pwm signal: PWMH1 */
#define PIO_PA24B_PWMH1      (1u << 24)  //!< Pwm signal: PWMH1 */
#define PIO_PB1A_PWMH1       (1u << 1)   //!< Pwm signal: PWMH1 */
#define PIO_PC19B_PWMH1      (1u << 19)  //!< Pwm signal: PWMH1 */
#define PIO_PA2A_PWMH2       (1u << 2)   //!< Pwm signal: PWMH2 */
#define PIO_PA13B_PWMH2      (1u << 13)  //!< Pwm signal: PWMH2 */
#define PIO_PA25B_PWMH2      (1u << 25)  //!< Pwm signal: PWMH2 */
#define PIO_PB4B_PWMH2       (1u << 4)   //!< Pwm signal: PWMH2 */
#define PIO_PC20B_PWMH2      (1u << 20)  //!< Pwm signal: PWMH2 */
#define PIO_PA7B_PWMH3       (1u << 7)   //!< Pwm signal: PWMH3 */
#define PIO_PA14B_PWMH3      (1u << 14)  //!< Pwm signal: PWMH3 */
#define PIO_PA17C_PWMH3      (1u << 17)  //!< Pwm signal: PWMH3 */
#define PIO_PB14B_PWMH3      (1u << 14)  //!< Pwm signal: PWMH3 */
#define PIO_PC21B_PWMH3      (1u << 21)  //!< Pwm signal: PWMH3 */
#define PIO_PA19B_PWML0      (1u << 19)  //!< Pwm signal: PWML0 */
#define PIO_PB5B_PWML0       (1u << 5)   //!< Pwm signal: PWML0 */
#define PIO_PC0B_PWML0       (1u << 0)   //!< Pwm signal: PWML0 */
#define PIO_PC13B_PWML0      (1u << 13)  //!< Pwm signal: PWML0 */
#define PIO_PA20B_PWML1      (1u << 20)  //!< Pwm signal: PWML1 */
#define PIO_PB12A_PWML1      (1u << 12)  //!< Pwm signal: PWML1 */
#define PIO_PC1B_PWML1       (1u << 1)   //!< Pwm signal: PWML1 */
#define PIO_PC15B_PWML1      (1u << 15)  //!< Pwm signal: PWML1 */
#define PIO_PA16C_PWML2      (1u << 16)  //!< Pwm signal: PWML2 */
#define PIO_PA30A_PWML2      (1u << 30)  //!< Pwm signal: PWML2 */
#define PIO_PB13A_PWML2      (1u << 13)  //!< Pwm signal: PWML2 */
#define PIO_PC2B_PWML2       (1u << 2)   //!< Pwm signal: PWML2 */
#define PIO_PA15C_PWML3      (1u << 15)  //!< Pwm signal: PWML3 */
#define PIO_PC3B_PWML3       (1u << 3)   //!< Pwm signal: PWML3 */
#define PIO_PC22B_PWML3      (1u << 22)  //!< Pwm signal: PWML3 */
/* ========== Pio definition for USART0 peripheral ========== */
#define PIO_PA8A_CTS0        (1u << 8)   //!< Usart0 signal: CTS0 */
#define PIO_PA7A_RTS0        (1u << 7)   //!< Usart0 signal: RTS0 */
#define PIO_PA5A_RXD0        (1u << 5)   //!< Usart0 signal: RXD0 */
#define PIO_PA2B_SCK0        (1u << 2)   //!< Usart0 signal: SCK0 */
#define PIO_PA6A_TXD0        (1u << 6)   //!< Usart0 signal: TXD0 */
/* ========== Pio definition for USART1 peripheral ========== */
#define PIO_PA25A_CTS1       (1u << 25)  //!< Usart1 signal: CTS1 */
#define PIO_PA26A_DCD1       (1u << 26)  //!< Usart1 signal: DCD1 */
#define PIO_PA28A_DSR1       (1u << 28)  //!< Usart1 signal: DSR1 */
#define PIO_PA27A_DTR1       (1u << 27)  //!< Usart1 signal: DTR1 */
#define PIO_PA29A_RI1        (1u << 29)  //!< Usart1 signal: RI1 */
#define PIO_PA24A_RTS1       (1u << 24)  //!< Usart1 signal: RTS1 */
#define PIO_PA21A_RXD1       (1u << 21)  //!< Usart1 signal: RXD1 */
#define PIO_PA23A_SCK1       (1u << 23)  //!< Usart1 signal: SCK1 */
#define PIO_PA22A_TXD1       (1u << 22)  //!< Usart1 signal: TXD1 */
/* ========== Pio definition for ADC peripheral ========== */
#define PIO_PA17X1_AD0       (1u << 17)  //!< Adc signal: AD0 */
#define PIO_PA18X1_AD1       (1u << 18)  //!< Adc signal: AD1 */
#define PIO_PC13X1_AD10      (1u << 13)  //!< Adc signal: AD10 */
#define PIO_PC15X1_AD11      (1u << 15)  //!< Adc signal: AD11 */
#define PIO_PC12X1_AD12      (1u << 12)  //!< Adc signal: AD12 */
#define PIO_PC29X1_AD13      (1u << 29)  //!< Adc signal: AD13 */
#define PIO_PC30X1_AD14      (1u << 30)  //!< Adc signal: AD14 */
#define PIO_PA19X1_AD2_WKUP9 (1u << 19)  //!< Adc signal: AD2/WKUP9 */
#define PIO_PA20X1_AD3_WKUP10 (1u << 20) //!< Adc signal: AD3/WKUP10 */
#define PIO_PB0X1_AD4        (1u << 0)   //!< Adc signal: AD4 */
#define PIO_PB1X1_AD5        (1u << 1)   //!< Adc signal: AD5 */
#define PIO_PB2X1_AD6_WKUP12 (1u << 2)   //!< Adc signal: AD6/WKUP12 */
#define PIO_PB3X1_AD7        (1u << 3)   //!< Adc signal: AD7 */
#define PIO_PA21X1_AD8       (1u << 21)  //!< Adc signal: AD8 */
#define PIO_PA22X1_AD9       (1u << 22)  //!< Adc signal: AD9 */
#define PIO_PA8B_ADTRG       (1u << 8)   //!< Adc signal: ADTRG */
/* ========== Pio definition for DACC peripheral ========== */
#define PIO_PB13X1_DAC0      (1u << 13)  //!< Dacc signal: DAC0 */
#define PIO_PB14X1_DAC1      (1u << 14)  //!< Dacc signal: DAC1 */
#define PIO_PA2C_DATRG       (1u << 2)   //!< Dacc signal: DATRG */
/* ========== Pio definition for PMC peripheral ========== */
#define PIO_PA6B_PCK0        (1u << 6)   //!< Pmc signal: PCK0 */
#define PIO_PB13B_PCK0       (1u << 13)  //!< Pmc signal: PCK0 */
#define PIO_PA17B_PCK1       (1u << 17)  //!< Pmc signal: PCK1 */
#define PIO_PA21B_PCK1       (1u << 21)  //!< Pmc signal: PCK1 */
#define PIO_PA18B_PCK2       (1u << 18)  //!< Pmc signal: PCK2 */
#define PIO_PA31B_PCK2       (1u << 31)  //!< Pmc signal: PCK2 */
#define PIO_PB3B_PCK2        (1u << 3)   //!< Pmc signal: PCK2 */
/* ========== Pio definition for UART0 peripheral ========== */
#define PIO_PA9A_URXD0       (1u << 9)   //!< Uart0 signal: URXD0 */
#define PIO_PA10A_UTXD0      (1u << 10)  //!< Uart0 signal: UTXD0 */
/* ========== Pio definition for UART1 peripheral ========== */
#define PIO_PB2A_URXD1       (1u << 2)   //!< Uart1 signal: URXD1 */
#define PIO_PB3A_UTXD1       (1u << 3)   //!< Uart1 signal: UTXD1 */
/* ========== Pio definition for PIOA peripheral ========== */
#define PIO_PA24D_PIODC0     (1u << 24)  //!< Pioa signal: PIODC0 */
#define PIO_PA25D_PIODC1     (1u << 25)  //!< Pioa signal: PIODC1 */
#define PIO_PA26D_PIODC2     (1u << 26)  //!< Pioa signal: PIODC2 */
#define PIO_PA27D_PIODC3     (1u << 27)  //!< Pioa signal: PIODC3 */
#define PIO_PA28D_PIODC4     (1u << 28)  //!< Pioa signal: PIODC4 */
#define PIO_PA29D_PIODC5     (1u << 29)  //!< Pioa signal: PIODC5 */
#define PIO_PA30D_PIODC6     (1u << 30)  //!< Pioa signal: PIODC6 */
#define PIO_PA31D_PIODC7     (1u << 31)  //!< Pioa signal: PIODC7 */
#define PIO_PA23D_PIODCCLK   (1u << 23)  //!< Pioa signal: PIODCCLK */
#define PIO_PA15D_PIODCEN1   (1u << 15)  //!< Pioa signal: PIODCEN1 */
#define PIO_PA16D_PIODCEN2   (1u << 16)  //!< Pioa signal: PIODCEN2 */

//====================  USBD ================================
/** Number of USB endpoints */
#define CHIP_USB_NUMENDPOINTS 8


#endif /* SERIES_CPP_H_ */
/** @} ingroup hardware_sam3s */
