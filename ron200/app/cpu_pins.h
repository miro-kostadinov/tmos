/*
 * cpu_pins.h
 *
 *  Created on: 03.05.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef CPU_PINS_H_
#define CPU_PINS_H_

#include <tmos.h>
#include <pio_drv.h>
#include <series_cpp.h>


//------------------------------------------------------------------------------
//	Power
//------------------------------------------------------------------------------
/** Power disable
 *  Disables +1.2V, +2.5V, +3.3V used for FPGA
 *  has external pull-up
 */
#define PWR_DIS_PIN		PIO_PA14

/** Enables FPGA banks 0,1 power
 *  has external pull-down
 */
#define F_VCCO_EN_PIN	PIO_PB12

/** Enables target power
 *  has external pull-down also can be controlled from the target
 */
#define J_VCC_EN_PIN	PIO_PA30

void power_fpga_core_on();
void power_fpga_core_off();
void power_fpga_interface_on();
void power_fpga_interface_off();


//------------------------------------------------------------------------------
// FPGA
//------------------------------------------------------------------------------

/** LB_WE used as address latch
 */
#define FPGA_WE_PIN	PIO_PA10

/** PROG_B used for programming only
 */
#define FPGA_PROG_PIN	PIO_PA8

/** during programming is input = INIT_B
 * has external pull-up
 * outputs 24MHz for the FPGA pll during normal operation
 */
#define FPGA_INIT_PIN	PIO_PA12

/** Read/Write control during programming and bus transfers
 */
#define FPGA_RDWR_PIN 	PIO_PA27

/** FPGA chip select during programming
 *  FPGA reset during normal operation
 */
#define FPGA_CS_PIN 	PIO_PA28

/** FPGA clock used during programming and bus transfers
 */
#define FPGA_CLK_PIN 	PIO_PA29


extern const GPIO_STRU FPGA_INIT;
extern const GPIO_STRU FPGA_PROG;
extern const GPIO_STRU FPGA_DATANCLK;
extern const GPIO_STRU FPGA_DATA;
extern const GPIO_STRU FPGA_RDWR;
extern const GPIO_STRU FPGA_CS;
extern const GPIO_STRU FPGA_ADRWE;
extern const GPIO_STRU FPGA_CLK;





#endif /* CPU_PINS_H_ */
