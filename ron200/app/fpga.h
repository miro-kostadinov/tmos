/*
 * fpga.h
 *
 *  Created on: 18.04.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef FPGA_H_
#define FPGA_H_


void fpga_write( HANDLE fpga_pio_hnd, uint8_t *data, uint32_t len );
int fpga_open( HANDLE fpga_pio_hnd );
int fpga_close( HANDLE fpga_pio_hnd );




#endif /* FPGA_H_ */
