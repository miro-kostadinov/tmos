/*
 * lcd_ST7565S_cmd.h
 *
 *  Created on: 03.09.2013
 *      Author: stanly
 */

#ifndef LCD_ST7565S_CMD_H_
#define LCD_ST7565S_CMD_H_

//LCD commands A0 = 0
#define CMD_DISPLAY_ON			0xAF		//!< Display On
#define CMD_DISPLAY_OFF			0xAE		//!< Display Off
#define CMD_START_LINE(x)		(0x40 +(x))	//!< Display Start Line Set(0..63)
#define CMD_PAGE_ADR(x)			(0xB0 +(x))	//!< Page Address Set(0..8)
#define CMD_COLUMN_ADR_HI(x)	(0x10 +(x))	//!< Column Address Set(0..131) 4 MSB
#define CMD_COLUMN_ADR_LO(x)	(0x00 +(x))	//!< Column Address Set(0..131) 4 LSB
#define CMD_ADC_NORMAL			0xA0		//!< Segment Driver Direction Select
#define CMD_ADC_REVERSE			0xA1		//!< Segment Driver Direction Select (column)
#define CMD_DISPLAY_NORMAL		0xA6		//!< Display Normal
#define CMD_DISPLAY_REVERSE		0xA7		//!< Display Inverse
#define CMD_DISPLAY_ALL_ON		0xA4		//!< Display All points=ON
#define CMD_DISPLAY_ALL_OFF		0xA5		//!< Display All points=OFF
#define CMD_BIAS1				0xA2		//!< LCD Bias Set (1/9 1/8 ..)
#define CMD_BIAS2				0xA3		//!< LCD Bias Set (1/7 1/6 ..)
#define CMD_RESET				0xE2		//!< Reset
#define CMD_COMMON_NORMAL		0xC0		//!< Common Output mode
#define CMD_COMMON_REVERSE		0xC8		//!< Common Output mode Inverse (lines)

#define POWER_VOLTAGE_FOLLOWER 	1
#define POWER_VOLTAGE_REGULATOR	2
#define POWER_BOOSTER			4
#define POWER_ALL				7
#define CMD_POWER(x)			(0x28 +(x))	//!< Page Address Set(0..8)

#define CMD_V5_REGULATOR(x)		(0x20 +(x))	//!< V5 voltage regulator internal resistor ratio(0..7)

#define CMD_ELECTRONIC_VOLUME	0x81		//!< The Electronic Volume (Double Byte Command) (0..3F)
#define CMD_INDICATOR_ON		0xAD		//!< Static Indicator (Double Byte Command) (0-3) OFF, 1s, 0.5s, on
#define CMD_INDICATOR_OFF		0xAC		//!< Static Indicator OFF

#define CMD_READ_WRITE_START	0xE0		//!< Read/Modify loop start
#define CMD_END					0xEE		//!< Read/Modify loop END

#define CMD_BOOSTER_RATIO		0xF8		//!< The Booster Ratio (Double Byte Command) (0-3)
#define CMD_NOP					0xE3		//!< NOP
#define CMD_TEST				0xFC		//!< Don't use

#define CMD_STATUS_READ						//!< Reads status

//LCD data A0 = 1
#define DATA_WRITE							//!< Write data when A0=1



#endif /* LCD_ST7565S_CMD_H_ */
