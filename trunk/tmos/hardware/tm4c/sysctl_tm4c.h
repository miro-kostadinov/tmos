/*
 * sysctl_tm4c.h
 *
 *  Created on: Aug 22, 2014
 *      Author: miro
 */

#ifndef SYSCTL_TM4C_H_
#define SYSCTL_TM4C_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  System Control  (offset 0x400F.E000)
 ******************************************************************************/
struct SYSCTL_TypeDef
{
  __IO uint32_t SYSCTL_DID0;		//!< (sysctl:0x000) Device Identification 0
  __IO uint32_t SYSCTL_DID1; 		//!< (sysctl:0x004) RO - Device Identification 1
  __IO uint32_t reserved1[12];		//!< (sysctl:0x008)
  __IO uint32_t SYSCTL_PTBOCTL; 	//!< (sysctl:0x038) Power-Temp Brown Out Control
  __IO uint32_t reserved2[5];		//!< (sysctl:0x03C)
  __IO uint32_t SYSCTL_RIS; 		//!< (sysctl:0x050) Raw Interrupt Status
  __IO uint32_t SYSCTL_IMC; 		//!< (sysctl:0x054) Interrupt Mask Control
  __IO uint32_t SYSCTL_MISC;		//!< (sysctl:0x058) Masked Interrupt Status and Clear
  __IO uint32_t SYSCTL_RESC;		//!< (sysctl:0x05C) Reset Cause
  __IO uint32_t SYSCTL_PWRTC;		//!< (sysctl:0x060) Power-Temperature Cause
  __IO uint32_t SYSCTL_NMIC;		//!< (sysctl:0x064) NMI Cause Register
  __IO uint32_t reserved3[5];		//!< (sysctl:0x068)
  __IO uint32_t SYSCTL_MOSCCTL;		//!< (sysctl:0x07C) Main Oscillator Control
  __IO uint32_t reserved4[12];		//!< (sysctl:0x080)
  __IO uint32_t SYSCTL_RSCLKCFG; 	//!< (sysctl:0x0B0) Run and Sleep Mode Configuration Register
  __IO uint32_t reserved5[3];		//!< (sysctl:0x0B4)
  __IO uint32_t SYSCTL_MEMTIM0;		//!< (sysctl:0x0C0) Memory Timing Parameter Register 0 for Main Flash and  EEPROM
  __IO uint32_t reserved6[1];		//!< (sysctl:0x0C4)
  __IO uint32_t SYSCTL_DSMEMTIM0;	//!< (sysctl:0x0C8) Deep Sleep Mode Memory Timing Register 0 for Main Flash and EEPROM
  __IO uint32_t reserved7[27];		//!< (sysctl:0x0CC)
  __IO uint32_t SYSCTL_ALTCLKCFG; 	//!< (sysctl:0x138) Alternate Clock Configuration
  __IO uint32_t reserved8[2];		//!< (sysctl:0x13C)
  __IO uint32_t SYSCTL_DSCLKCFG; 	//!< (sysctl:0x144) Deep Sleep Clock Configuration Register
  __IO uint32_t SYSCTL_DIVSCLK;		//!< (sysctl:0x148) Divisor and Source Clock Configuration
  __IO uint32_t SYSCTL_SYSPROP;		//!< (sysctl:0x14C) System Properties
  __IO uint32_t SYSCTL_PIOSCCAL;	//!< (sysctl:0x150) Precision Internal Oscillator Calibration
  __IO uint32_t SYSCTL_PIOSCSTAT;	//!< (sysctl:0x154) Precision Internal Oscillator Statistics
  __IO uint32_t reserved9[2];		//!< (sysctl:0x158)
  __IO uint32_t SYSCTL_PLLFREQ0;	//!< (sysctl:0x160) PLL Frequency 0
  __IO uint32_t SYSCTL_PLLFREQ1;	//!< (sysctl:0x164) PLL Frequency 1
  __IO uint32_t SYSCTL_PLLSTAT;		//!< (sysctl:0x168) PLL Status
  __IO uint32_t reserved10[7];		//!< (sysctl:0x16C)
  __IO uint32_t SYSCTL_SLPPWRCFG;	//!< (sysctl:0x188) Sleep Power Configuration
  __IO uint32_t SYSCTL_DSLPPWRCFG; 	//!< (sysctl:0x18C) Deep-Sleep Power Configuration
  __IO uint32_t reserved11[4];		//!< (sysctl:0x190)
  __IO uint32_t SYSCTL_NVMSTAT;		//!< (sysctl:0x1A0) Non-Volatile Memory Information
  __IO uint32_t reserved12[3];		//!< (sysctl:0x1A4)
  __IO uint32_t SYSCTL_LDOPCAL;		//!< (sysctl:0x1B0) LDO Power Calibration
  __IO uint32_t reserved13[1];		//!< (sysctl:0x1B4)
  __IO uint32_t SYSCTL_LDOSPCAL; 	//!< (sysctl:0x1B8) LDO Sleep Power Calibration
  __IO uint32_t SYSCTL_LDODPCTL; 	//!< (sysctl:0x1BC) LDO Deep-Sleep Power Control
  __IO uint32_t SYSCTL_LDODPCAL; 	//!< (sysctl:0x1C0) LDO Deep-Sleep Power Calibration
  __IO uint32_t reserved14[2];		//!< (sysctl:0x1C4)
  __IO uint32_t SYSCTL_SDPMST; 		//!< (sysctl:0x1CC) Sleep / Deep-Sleep Power Mode Status
  __IO uint32_t reserved15[2];		//!< (sysctl:0x1D0)
  __IO uint32_t SYSCTL_RESBEHAVCTL;	//!< (sysctl:0x1D8) Reset Behavior Control Register
  __IO uint32_t reserved16[6];		//!< (sysctl:0x1DC)
  __IO uint32_t SYSCTL_HSSR; 		//!< (sysctl:0x1F4) Hardware System Service Request
  __IO uint32_t reserved17[34];		//!< (sysctl:0x1F8)
  __IO uint32_t SYSCTL_USBPDS; 		//!< (sysctl:0x280) USB Power Domain Status
  __IO uint32_t SYSCTL_USBMPC; 		//!< (sysctl:0x284) USB Memory Power Control
  __IO uint32_t SYSCTL_EMACPDS;		//!< (sysctl:0x288) Ethernet MAC Power Domain Status
  __IO uint32_t SYSCTL_EMACMPC;		//!< (sysctl:0x28C) Ethernet MAC Memory Power Control
  __IO uint32_t reserved18[2];		//!< (sysctl:0x290)
  __IO uint32_t SYSCTL_CAN0PDS;		//!< (sysctl:0x298) CAN 0 Power Domain Status
  __IO uint32_t SYSCTL_CAN0MPC;		//!< (sysctl:0x29C) CAN 0 Memory Power Control
  __IO uint32_t SYSCTL_CAN1PDS;		//!< (sysctl:0x2A0) CAN 1 Power Domain Status
  __IO uint32_t SYSCTL_CAN1MPC;		//!< (sysctl:0x2A4) CAN 1 Memory Power Control
  __IO uint32_t reserved19[22];		//!< (sysctl:0x2A8)

  __IO uint32_t SYSCTL_PPWD; 		//!< (sysctl:0x300) Watchdog Timer Peripheral Present
  __IO uint32_t SYSCTL_PPTIMER;		//!< (sysctl:0x304) 16/32-Bit General-Purpose Timer Peripheral Present
  __IO uint32_t SYSCTL_PPGPIO; 		//!< (sysctl:0x308) General-Purpose Input/Output Peripheral Present
  __IO uint32_t SYSCTL_PPDMA; 		//!< (sysctl:0x30C) Micro Direct Memory Access Peripheral Present
  __IO uint32_t SYSCTL_PPEPI; 		//!< (sysctl:0x310) EPI Peripheral Present
  __IO uint32_t SYSCTL_PPHIB; 		//!< (sysctl:0x314) Hibernation Peripheral Present
  __IO uint32_t SYSCTL_PPUART; 		//!< (sysctl:0x318) Universal Asynchronous Receiver/Transmitter Peripheral Present
  __IO uint32_t SYSCTL_PPSSI; 		//!< (sysctl:0x31C) Synchronous Serial Interface Peripheral Present
  __IO uint32_t SYSCTL_PPI2C; 		//!< (sysctl:0x320) Inter-Integrated Circuit Peripheral Present
  __IO uint32_t reserved20[1];		//!< (sysctl:0x324)
  __IO uint32_t SYSCTL_PPUSB; 		//!< (sysctl:0x328) Universal Serial Bus Peripheral Present
  __IO uint32_t reserved21[1];		//!< (sysctl:0x32C)
  __IO uint32_t SYSCTL_PPEPHY; 		//!< (sysctl:0x330) Ethernet PHY Peripheral Present
  __IO uint32_t SYSCTL_PPCAN; 		//!< (sysctl:0x334) Controller Area Network Peripheral Present
  __IO uint32_t SYSCTL_PPADC; 		//!< (sysctl:0x338) Analog-to-Digital Converter Peripheral Present
  __IO uint32_t SYSCTL_PPACMP; 		//!< (sysctl:0x33C) Analog Comparator Peripheral Present
  __IO uint32_t SYSCTL_PPPWM; 		//!< (sysctl:0x340) Pulse Width Modulator Peripheral Present
  __IO uint32_t SYSCTL_PPQEI; 		//!< (sysctl:0x344) Quadrature Encoder Interface Peripheral Present
  __IO uint32_t SYSCTL_PPLPC; 		//!< (sysctl:0x348) Low Pin Count Interface Peripheral Present
  __IO uint32_t reserved22[1];		//!< (sysctl:0x34C)
  __IO uint32_t SYSCTL_PPPECI; 		//!< (sysctl:0x350) Platform Environment Control Interface Peripheral Present
  __IO uint32_t SYSCTL_PPFAN; 		//!< (sysctl:0x354) Fan Control Peripheral Present
  __IO uint32_t SYSCTL_PPEEPROM; 	//!< (sysctl:0x358) EEPROM Peripheral Present
  __IO uint32_t SYSCTL_PPWTIMER; 	//!< (sysctl:0x35C) 32/64-Bit Wide General-Purpose Timer Peripheral  Present
  __IO uint32_t reserved23[4];		//!< (sysctl:0x360)
  __IO uint32_t SYSCTL_PPRTS; 		//!< (sysctl:0x370) Remote Temperature Sensor Peripheral Present
  __IO uint32_t SYSCTL_PPCCM; 		//!< (sysctl:0x374) CRC and Cryptographic Modules Peripheral Present
  __IO uint32_t reserved24[6];		//!< (sysctl:0x378)
  __IO uint32_t SYSCTL_PPLCD; 		//!< (sysctl:0x390) LCD Peripheral Present
  __IO uint32_t reserved25[1];		//!< (sysctl:0x394)
  __IO uint32_t SYSCTL_PPOWIRE;		//!< (sysctl:0x398) 1-Wire Peripheral Present
  __IO uint32_t SYSCTL_PPEMAC; 		//!< (sysctl:0x39C) Ethernet MAC Peripheral Present
  __IO uint32_t SYSCTL_PPPRB; 		//!< (sysctl:0x3A0) Power Regulator Bus Peripheral Present
  __IO uint32_t SYSCTL_PPHIM; 		//!< (sysctl:0x3A4) Human Interface Master Peripheral Present
  __IO uint32_t reserved26[86];		//!< (sysctl:0x3A8)

  __IO uint32_t SYSCTL_SRWD; 		//!< (sysctl:0x500) Watchdog Timer Software Reset
  __IO uint32_t SYSCTL_SRTIMER;		//!< (sysctl:0x504) 16/32-Bit General-Purpose Timer Software Reset
  __IO uint32_t SYSCTL_SRGPIO; 		//!< (sysctl:0x508) General-Purpose Input/Output Software Reset
  __IO uint32_t SYSCTL_SRDMA; 		//!< (sysctl:0x50C) Micro Direct Memory Access Software Reset
  __IO uint32_t SYSCTL_SREPI; 		//!< (sysctl:0x510) EPI Software Reset
  __IO uint32_t SYSCTL_SRHIB; 		//!< (sysctl:0x514) Hibernation Software Reset
  __IO uint32_t SYSCTL_SRUART; 		//!< (sysctl:0x518) Universal Asynchronous Receiver/Transmitter Software Reset
  __IO uint32_t SYSCTL_SRSSI; 		//!< (sysctl:0x51C) Synchronous Serial Interface Software Reset
  __IO uint32_t SYSCTL_SRI2C; 		//!< (sysctl:0x520) Inter-Integrated Circuit Software Reset
  __IO uint32_t reserved50[1];		//!< (sysctl:0x524)
  __IO uint32_t SYSCTL_SRUSB; 		//!< (sysctl:0x528) Universal Serial Bus Software Reset
  __IO uint32_t reserved51[2];		//!< (sysctl:0x52C)
  __IO uint32_t SYSCTL_SRCAN; 		//!< (sysctl:0x534) Controller Area Network Software Reset
  __IO uint32_t SYSCTL_SRADC; 		//!< (sysctl:0x538) Analog-to-Digital Converter Software Reset
  __IO uint32_t SYSCTL_SRACMP; 		//!< (sysctl:0x53C) Analog Comparator Software Reset
  __IO uint32_t SYSCTL_SRPWM; 		//!< (sysctl:0x540) Pulse Width Modulator Software Reset
  __IO uint32_t SYSCTL_SRQEI; 		//!< (sysctl:0x544) Quadrature Encoder Interface Software Reset
  __IO uint32_t reserved52[4];		//!< (sysctl:0x548)
  __IO uint32_t SYSCTL_SREEPROM; 	//!< (sysctl:0x558) EEPROM Software Reset
  __IO uint32_t reserved53[6];		//!< (sysctl:0x55C)
  __IO uint32_t SYSCTL_SRCCM; 		//!< (sysctl:0x574) CRC and Cryptographic Modules Software Reset
  __IO uint32_t reserved54[9];		//!< (sysctl:0x578)
  __IO uint32_t SYSCTL_SREMAC; 		//!< (sysctl:0x59C) Ethernet MAC Software Reset
  __IO uint32_t reserved55[24];		//!< (sysctl:0x5A0)

  __IO uint32_t SYSCTL_RCGCWD; 		//!< (sysctl:0x600) Watchdog Timer Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCTIMER; 	//!< (sysctl:0x604) 16/32-Bit General-Purpose Timer Run Mode Clock Gating  Control
  __IO uint32_t SYSCTL_RCGCGPIO; 	//!< (sysctl:0x608) General-Purpose Input/Output Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCDMA; 	//!< (sysctl:0x60C) Micro Direct Memory Access Run Mode Clock Gating  Control
  __IO uint32_t SYSCTL_RCGCEPI; 	//!< (sysctl:0x610) EPI Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCHIB; 	//!< (sysctl:0x614) Hibernation Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCUART; 	//!< (sysctl:0x618) Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCSSI; 	//!< (sysctl:0x61C) Synchronous Serial Interface Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCI2C; 	//!< (sysctl:0x620) Inter-Integrated Circuit Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCUSB; 	//!< (sysctl:0x628) Universal Serial Bus Run Mode Clock Gating Control
  __IO uint32_t reserved60[2];		//!< (sysctl:0x62C)
  __IO uint32_t SYSCTL_RCGCCAN; 	//!< (sysctl:0x634) Controller Area Network Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCADC; 	//!< (sysctl:0x638) Analog-to-Digital Converter Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCACMP; 	//!< (sysctl:0x63C) Analog Comparator Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCPWM; 	//!< (sysctl:0x640) Pulse Width Modulator Run Mode Clock Gating Control
  __IO uint32_t SYSCTL_RCGCQEI; 	//!< (sysctl:0x644) Quadrature Encoder Interface Run Mode Clock Gating Control
  __IO uint32_t reserved61[4];		//!< (sysctl:0x648)
  __IO uint32_t SYSCTL_RCGCEEPROM; 	//!< (sysctl:0x658) EEPROM Run Mode Clock Gating Control
  __IO uint32_t reserved62[6];		//!< (sysctl:0x65C)
  __IO uint32_t SYSCTL_RCGCCCM; 	//!< (sysctl:0x674) CRC and Cryptographic Modules Run Mode Clock Gating Control
  __IO uint32_t reserved63[9];		//!< (sysctl:0x678)
  __IO uint32_t SYSCTL_RCGCEMAC; 	//!< (sysctl:0x69C) Ethernet MAC Run Mode Clock Gating Control
  __IO uint32_t reserved64[24];		//!< (sysctl:0x6A0)

  __IO uint32_t SYSCTL_SCGCWD; 		//!< (sysctl:0x700) Watchdog Timer Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCTIMER; 	//!< (sysctl:0x704) 16/32-Bit General-Purpose Timer Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCGPIO; 	//!< (sysctl:0x708) General-Purpose Input/Output Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCDMA; 	//!< (sysctl:0x70C) Micro Direct Memory Access Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCEPI; 	//!< (sysctl:0x710) EPI Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCHIB; 	//!< (sysctl:0x714) Hibernation Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCUART; 	//!< (sysctl:0x718) Universal Asynchronous Receiver/Transmitter Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCSSI; 	//!< (sysctl:0x71C) Synchronous Serial Interface Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCI2C; 	//!< (sysctl:0x720) Inter-Integrated Circuit Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCUSB; 	//!< (sysctl:0x728) Universal Serial Bus Sleep Mode Clock Gating Control
  __IO uint32_t reserved70[2];		//!< (sysctl:0x72C)
  __IO uint32_t SYSCTL_SCGCCAN; 	//!< (sysctl:0x734) Controller Area Network Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCADC; 	//!< (sysctl:0x738) Analog-to-Digital Converter Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCACMP; 	//!< (sysctl:0x73C) Analog Comparator Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCPWM; 	//!< (sysctl:0x740) Pulse Width Modulator Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_SCGCQEI; 	//!< (sysctl:0x744) Quadrature Encoder Interface Sleep Mode Clock Gating Control
  __IO uint32_t reserved71[4];		//!< (sysctl:0x748)
  __IO uint32_t SYSCTL_SCGCEEPROM; 	//!< (sysctl:0x758) EEPROM Sleep Mode Clock Gating Control
  __IO uint32_t reserved72[6];		//!< (sysctl:0x75C)
  __IO uint32_t SYSCTL_SCGCCCM; 	//!< (sysctl:0x774) CRC and Cryptographic Modules Sleep Mode Clock Gating Control
  __IO uint32_t reserved73[9];		//!< (sysctl:0x778)
  __IO uint32_t SYSCTL_SCGCEMAC; 	//!< (sysctl:0x79C) Ethernet MAC Sleep Mode Clock Gating Control
  __IO uint32_t reserved74[24];		//!< (sysctl:0x7A0)

  __IO uint32_t SYSCTL_DCGCWD; 		//!< (sysctl:0x800) Watchdog Timer Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCTIMER; 	//!< (sysctl:0x804) 16/32-Bit General-Purpose Timer Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCGPIO; 	//!< (sysctl:0x808) General-Purpose Input/Output Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCDMA; 	//!< (sysctl:0x80C) Micro Direct Memory Access Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCEPI; 	//!< (sysctl:0x810) EPI Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCHIB; 	//!< (sysctl:0x814) Hibernation Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCUART; 	//!< (sysctl:0x818) Universal Asynchronous Receiver/Transmitter Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCSSI; 	//!< (sysctl:0x81C) Synchronous Serial Interface Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCI2C; 	//!< (sysctl:0x820) Inter-Integrated Circuit Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCUSB; 	//!< (sysctl:0x828) Universal Serial Bus Deep-Sleep Mode Clock Gating Control
  __IO uint32_t reserved80[2];		//!< (sysctl:0x82C)
  __IO uint32_t SYSCTL_DCGCCAN; 	//!< (sysctl:0x834) Controller Area Network Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCADC; 	//!< (sysctl:0x838) Analog-to-Digital Converter Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCACMP; 	//!< (sysctl:0x83C) Analog Comparator Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCPWM; 	//!< (sysctl:0x840) Pulse Width Modulator Deep-Sleep Mode Clock Gating Control
  __IO uint32_t SYSCTL_DCGCQEI; 	//!< (sysctl:0x844) Quadrature Encoder Interface Deep-Sleep Mode Clock Gating Control
  __IO uint32_t reserved81[4];		//!< (sysctl:0x848)
  __IO uint32_t SYSCTL_DCGCEEPROM; 	//!< (sysctl:0x858) EEPROM Deep-Sleep Mode Clock Gating Control
  __IO uint32_t reserved82[6];		//!< (sysctl:0x85C)
  __IO uint32_t SYSCTL_DCGCCCM; 	//!< (sysctl:0x874) CRC and Cryptographic Modules Deep-Sleep Mode Clock Gating Control
  __IO uint32_t reserved83[9];		//!< (sysctl:0x878)
  __IO uint32_t SYSCTL_DCGCEMAC; 	//!< (sysctl:0x89C) Ethernet MAC Deep-Sleep Mode Clock Gating Control
  __IO uint32_t reserved84[24];		//!< (sysctl:0x8A0)

  __IO uint32_t SYSCTL_PCWD; 		//!< (sysctl:0x900) Watchdog Timer Power Control
  __IO uint32_t SYSCTL_PCTIMER; 	//!< (sysctl:0x904) 16/32-Bit General-Purpose Timer Power Control
  __IO uint32_t SYSCTL_PCGPIO; 		//!< (sysctl:0x908) General-Purpose Input/Output Power Control
  __IO uint32_t SYSCTL_PCDMA; 		//!< (sysctl:0x90C) Micro Direct Memory Access Power Control
  __IO uint32_t SYSCTL_PCEPI; 		//!< (sysctl:0x910) External Peripheral Interface Power Control
  __IO uint32_t SYSCTL_PCHIB; 		//!< (sysctl:0x914) Hibernation Power Control
  __IO uint32_t SYSCTL_PCUART; 		//!< (sysctl:0x918) Universal Asynchronous Receiver/Transmitter Power Control
  __IO uint32_t SYSCTL_PCSSI; 		//!< (sysctl:0x91C) Synchronous Serial Interface Power Control
  __IO uint32_t SYSCTL_PCI2C; 		//!< (sysctl:0x920) Inter-Integrated Circuit Power Control
  __IO uint32_t SYSCTL_PCUSB; 		//!< (sysctl:0x928) Universal Serial Bus Power Control
  __IO uint32_t reserved90[2];		//!< (sysctl:0x92C)
  __IO uint32_t SYSCTL_PCCAN; 		//!< (sysctl:0x934) Controller Area Network Power Control
  __IO uint32_t SYSCTL_PCADC; 		//!< (sysctl:0x938) Analog-to-Digital Converter Power Control
  __IO uint32_t SYSCTL_PCACMP; 		//!< (sysctl:0x93C) Analog Comparator Power Control
  __IO uint32_t SYSCTL_PCPWM; 		//!< (sysctl:0x940) Pulse Width Modulator Power Control
  __IO uint32_t SYSCTL_PCQEI; 		//!< (sysctl:0x944) Quadrature Encoder Interface Power Control
  __IO uint32_t reserved91[4];		//!< (sysctl:0x948)
  __IO uint32_t SYSCTL_PCEEPROM; 	//!< (sysctl:0x958) EEPROM Power Control
  __IO uint32_t reserved92[6];		//!< (sysctl:0x95C)
  __IO uint32_t SYSCTL_PCCCM; 		//!< (sysctl:0x974) CRC and Cryptographic Modules Power Control
  __IO uint32_t reserved93[9];		//!< (sysctl:0x978)
  __IO uint32_t SYSCTL_PCEMAC; 		//!< (sysctl:0x99C) Ethernet MAC Power Control
  __IO uint32_t reserved94[24];		//!< (sysctl:0x9A0)

  __IO uint32_t SYSCTL_PRWD; 		//!< (sysctl:0xA00) Watchdog Timer Peripheral Ready
  __IO uint32_t SYSCTL_PRTIMER; 	//!< (sysctl:0xA04) 16/32-Bit General-Purpose Timer Peripheral Ready
  __IO uint32_t SYSCTL_PRGPIO; 		//!< (sysctl:0xA08) General-Purpose Input/Output Peripheral Ready
  __IO uint32_t SYSCTL_PRDMA; 		//!< (sysctl:0xA0C) Micro Direct Memory Access Peripheral Ready
  __IO uint32_t SYSCTL_PREPI; 		//!< (sysctl:0xA10) EPI Peripheral Ready
  __IO uint32_t SYSCTL_PRHIB; 		//!< (sysctl:0xA14) Hibernation Peripheral Ready
  __IO uint32_t SYSCTL_PRUART; 		//!< (sysctl:0xA18) Universal Asynchronous Receiver/Transmitter Peripheral Ready
  __IO uint32_t SYSCTL_PRSSI; 		//!< (sysctl:0xA1C) Synchronous Serial Interface Peripheral Ready
  __IO uint32_t SYSCTL_PRI2C; 		//!< (sysctl:0xA20) Inter-Integrated Circuit Peripheral Ready
  __IO uint32_t SYSCTL_PRUSB; 		//!< (sysctl:0xA28) Universal Serial Bus Peripheral Ready
  __IO uint32_t reservedA0[2];		//!< (sysctl:0xA2C)
  __IO uint32_t SYSCTL_PRCAN; 		//!< (sysctl:0xA34) Controller Area Network Peripheral Ready
  __IO uint32_t SYSCTL_PRADC; 		//!< (sysctl:0xA38) Analog-to-Digital Converter Peripheral Ready
  __IO uint32_t SYSCTL_PRACMP; 		//!< (sysctl:0xA3C) Analog Comparator Peripheral Ready
  __IO uint32_t SYSCTL_PRPWM; 		//!< (sysctl:0xA40) Pulse Width Modulator Peripheral Ready
  __IO uint32_t SYSCTL_PRQEI; 		//!< (sysctl:0xA44) Quadrature Encoder Interface Peripheral Ready
  __IO uint32_t reservedA1[4];		//!< (sysctl:0xA48)
  __IO uint32_t SYSCTL_PREEPROM; 	//!< (sysctl:0xA58) EEPROM Peripheral Ready
  __IO uint32_t reservedA2[6];		//!< (sysctl:0xA5C)
  __IO uint32_t SYSCTL_PRCCM; 		//!< (sysctl:0xA74) CRC and Cryptographic Modules Peripheral Ready
  __IO uint32_t reservedA3[9];		//!< (sysctl:0xA78)
  __IO uint32_t SYSCTL_PREMAC; 		//!< (sysctl:0xA9C) Ethernet MAC Peripheral Ready
};

/*******************************************************************************
 * @defgroup SYSCTL_regs_define
 * @{
 ******************************************************************************/

/** @defgroup SYSCTL_DID0:		(sysctl:0x000) Device Identification 0          */
#define SYSCTL_DID0_VER					0x70000000 		//!< DID0 Version
/** @} */

/** @defgroup SYSCTL_DID1: 		(sysctl:0x004) RO - Device Identification 1     */
/** @} */

/** @defgroup SYSCTL_PTBOCTL: 	(sysctl:0x038) Power-Temp Brown Out Control     */
/** @} */

/** @defgroup SYSCTL_RIS: 		(sysctl:0x050) Raw Interrupt Status             */
#define SYSCTL_RIS_MOSCPUPRIS			0x00000100 		//!< MOSC Power Up Raw Interrupt Status
#define SYSCTL_RIS_PLLLRIS				0x00000040 		//!< PLL Lock Raw Interrupt Status
#define SYSCTL_RIS_MOFRIS				0x00000008 		//!< Main Oscillator Failure Raw Interrupt Status
#define SYSCTL_RIS_BORRIS				0x00000002 		//!< Brown-Out Reset Raw Interrupt Status
/** @} */

/** @defgroup SYSCTL_IMC: 		(sysctl:0x054) Interrupt Mask Control           */
/** @} */

/** @defgroup SYSCTL_MISC:		(sysctl:0x058) Masked Interrupt Status and Clear*/
#define SYSCTL_MISC_MOSCPUPMIS			0x00000100 		//!< MOSC Power Up Masked Interrupt Status
#define SYSCTL_MISC_PLLLMIS				0x00000040 		//!< PLL Lock Masked Interrupt Status
#define SYSCTL_MISC_MOFMIS				0x00000008 		//!< Main Oscillator Failure Masked Interrupt Status
#define SYSCTL_MISC_BORMIS				0x00000002 		//!< BOR Masked Interrupt Status
/** @} */

/** @defgroup SYSCTL_RESC:		(sysctl:0x05C) Reset Cause                      */
/** @} */

/** @defgroup SYSCTL_PWRTC:		(sysctl:0x060) Power-Temperature Cause          */
/** @} */

/** @defgroup SYSCTL_NMIC:		(sysctl:0x064) NMI Cause Register               */
/** @} */

/** @defgroup SYSCTL_MOSCCTL:	(sysctl:0x07C) Main Oscillator Control          */
#define SYSCTL_MOSCCTL_OSCRNG			0x00000010 		//!< Oscillator Range
#define SYSCTL_MOSCCTL_PWRDN			0x00000008 		//!< Power Down
#define SYSCTL_MOSCCTL_NOXTAL			0x00000004 		//!< No MOSC/Crystal Connected
#define SYSCTL_MOSCCTL_MOSCIM			0x00000002 		//!< MOSC Failure Action
#define SYSCTL_MOSCCTL_CVAL				0x00000001 		//!< Clock Validation for MOSC
/** @} */

/** @defgroup SYSCTL_RSCLKCFG: 	(sysctl:0x0B0) Run and Sleep Mode Configuration Register */
#define SYSCTL_RSCLKCFG_MEMTIMU			0x80000000 		//!< Memory Timing Register Update
#define SYSCTL_RSCLKCFG_NEWFREQ			0x40000000 		//!< New PLLFREQ Accept
#define SYSCTL_RSCLKCFG_ACG				0x20000000 		//!< Auto Clock Gating
#define SYSCTL_RSCLKCFG_USEPLL			0x10000000 		//!< Use PLL
#define SYSCTL_RSCLKCFG_PLLSRC			0x0F000000 		//!< PLL Source
#define SYSCTL_RSCLKCFG_PLLSRC_PIOSC	0x00000000 		//!<  PIOSC is the PLL input clock source
#define SYSCTL_RSCLKCFG_PLLSRC_MOSC		0x03000000 		//!<  MOSC is the PLL input clock source
#define SYSCTL_RSCLKCFG_OSCSRC			0x00F00000 		//!< Oscillator Source
#define SYSCTL_RSCLKCFG_OSCSRC_PIOSC	0x00000000 		//!<  PIOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_LFIOSC	0x00200000 		//!<  LFIOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_MOSC		0x00300000 		//!<  MOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_RTCOSC	0x00400000 		//!<  Hibernation Module RTC Oscillator (RTCOSC)
#define SYSCTL_RSCLKCFG_OSYSDIV			0x000FFC00 		//!< Oscillator System Clock Divisor
#define SYSCTL_RSCLKCFG_PSYSDIV			0x000003FF 		//!< PLL System Clock Divisor
#define SYSCTL_RSCLKCFG_PSYSDIV_Set(x)	(x) 			//!<  set macro
/** @} */

/** @defgroup SYSCTL_MEMTIM0:	(sysctl:0x0C0) Memory Timing Parameter Register 0 for Main Flash and  EEPROM */
#define SYSCTL_MEMTIM0_EBCHT			0x03C00000 		//!< EEPROM Clock High Time
#define SYSCTL_MEMTIM0_EBCHT_0_5		0x00000000 		//!<  1/2 system clock period
#define SYSCTL_MEMTIM0_EBCHT_1_0		0x00400000 		//!<  1 system clock period
#define SYSCTL_MEMTIM0_EBCHT_1_5		0x00800000 		//!<  1.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_2_0		0x00C00000 		//!<  2 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_2_5		0x01000000 		//!<  2.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_3_0		0x01400000 		//!<  3 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_3_5		0x01800000 		//!<  3.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_4_0		0x01C00000 		//!<  4 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_4_5		0x02000000 		//!<  4.5 system clock periods
#define SYSCTL_MEMTIM0_EBCE				0x00200000 		//!< EEPROM Bank Clock Edge
#define SYSCTL_MEMTIM0_MB1      		0x00100010  	//!< Must be one
#define SYSCTL_MEMTIM0_EWS				0x000F0000 		//!< EEPROM Wait States
#define SYSCTL_MEMTIM0_EWS_0			0x00000000 		//!<  0 wait states
#define SYSCTL_MEMTIM0_EWS_1			0x00010000 		//!<  1 wait states
#define SYSCTL_MEMTIM0_EWS_2			0x00020000 		//!<  2 wait states
#define SYSCTL_MEMTIM0_EWS_3			0x00030000 		//!<  3 wait states
#define SYSCTL_MEMTIM0_EWS_4			0x00040000 		//!<  4 wait states
#define SYSCTL_MEMTIM0_EWS_5			0x00050000 		//!<  5 wait states
#define SYSCTL_MEMTIM0_EWS_6			0x00060000 		//!<  6 wait states
#define SYSCTL_MEMTIM0_EWS_7			0x00070000 		//!<  7 wait states
#define SYSCTL_MEMTIM0_FBCHT			0x000003C0 		//!< Flash Bank Clock High Time
#define SYSCTL_MEMTIM0_FBCHT_0_5		0x00000000 		//!<  1/2 system clock period
#define SYSCTL_MEMTIM0_FBCHT_1_0		0x00000040 		//!<  1 system clock period
#define SYSCTL_MEMTIM0_FBCHT_1_5		0x00000080 		//!<  1.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_2_0		0x000000C0 		//!<  2 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_2_5		0x00000100 		//!<  2.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_3_0		0x00000140 		//!<  3 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_3_5		0x00000180 		//!<  3.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_4_0		0x000001C0 		//!<  4 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_4_5		0x00000200 		//!<  4.5 system clock periods
#define SYSCTL_MEMTIM0_FBCE				0x00000020 		//!< EEPROM Bank Clock Edge
#define SYSCTL_MEMTIM0_FWS				0x0000000F 		//!< Flash Wait State
#define SYSCTL_MEMTIM0_FWS_0			0x00000000 		//!<  0 wait states
#define SYSCTL_MEMTIM0_FWS_1			0x00000001 		//!<  1 wait states
#define SYSCTL_MEMTIM0_FWS_2			0x00000002 		//!<  2 wait states
#define SYSCTL_MEMTIM0_FWS_3			0x00000003 		//!<  3 wait states
#define SYSCTL_MEMTIM0_FWS_4			0x00000004 		//!<  4 wait states
#define SYSCTL_MEMTIM0_FWS_5			0x00000005 		//!<  5 wait states
#define SYSCTL_MEMTIM0_FWS_6			0x00000006 		//!<  6 wait states
#define SYSCTL_MEMTIM0_FWS_7			0x00000007 		//!<  7 wait states
/** @} */

/** @defgroup SYSCTL_DSMEMTIM0:	(sysctl:0x0C8) Deep Sleep Mode Memory Timing Register 0 for Main Flash and EEPROM */
/** @} */

/** @defgroup SYSCTL_ALTCLKCFG: (sysctl:0x138) Alternate Clock Configuration	*/
/** @} */

/** @defgroup SYSCTL_DSCLKCFG: 	(sysctl:0x144) Deep Sleep Clock Configuration Register */
/** @} */

/** @defgroup SYSCTL_DIVSCLK:	(sysctl:0x148) Divisor and Source Clock Configuration */
/** @} */

/** @defgroup SYSCTL_SYSPROP:	(sysctl:0x14C) System Properties				*/
/** @} */

/** @defgroup SYSCTL_PIOSCCAL:	(sysctl:0x150) Precision Internal Oscillator Calibration */
/** @} */

/** @defgroup SYSCTL_PIOSCSTAT:	(sysctl:0x154) Precision Internal Oscillator Statistics*/
/** @} */

/** @defgroup SYSCTL_PLLFREQ0:	(sysctl:0x160) PLL Frequency 0					*/
#define SYSCTL_PLLFREQ0_PLLPWR			0x00800000 		//!< PLL Power
#define SYSCTL_PLLFREQ0_MFRAC			0x000FFC00 		//!< PLL M Fractional Value
#define SYSCTL_PLLFREQ0_MFRAC_Set(x)	((x)<<10)		//!<  set macro
#define SYSCTL_PLLFREQ0_MINT			0x000003FF 		//!< PLL M Integer Value
#define SYSCTL_PLLFREQ0_MINT_Set(x)		(x) 			//!<  set macro
/** @} */

/** @defgroup SYSCTL_PLLFREQ1:	(sysctl:0x164) PLL Frequency 1                  */
#define SYSCTL_PLLFREQ1_Q				0x00001F00 		//!< PLL Q Value
#define SYSCTL_PLLFREQ1_Q_Set(x)		((x)<<8) 		//!<  set macro
#define SYSCTL_PLLFREQ1_N				0x0000001F 		//!< PLL N Value
#define SYSCTL_PLLFREQ1_N_Set(x)		((x)-1)	 		//!<  set macro
/** @} */

/** @defgroup SYSCTL_PLLSTAT:	(sysctl:0x168) PLL Status                       */
#define SYSCTL_PLLSTAT_LOCK				0x00000001 		//!< PLL Lock
/** @} */

/** @defgroup SYSCTL_SLPPWRCFG:	(sysctl:0x188) Sleep Power Configuration        */
/** @} */

/** @defgroup SYSCTL_DSLPPWRCFG:(sysctl:0x18C) Deep-Sleep Power Configuration   */
/** @} */

/** @defgroup SYSCTL_NVMSTAT:	(sysctl:0x1A0) Non-Volatile Memory Information  */
/** @} */

/** @defgroup SYSCTL_LDOPCAL:	(sysctl:0x1B0) LDO Power Calibration            */
/** @} */

/** @defgroup SYSCTL_LDOSPCAL: 	(sysctl:0x1B8) LDO Sleep Power Calibration      */
/** @} */

/** @defgroup SYSCTL_LDODPCTL: 	(sysctl:0x1BC) LDO Deep-Sleep Power Control     */
/** @} */

/** @defgroup SYSCTL_LDODPCAL: 	(sysctl:0x1C0) LDO Deep-Sleep Power Calibration */
/** @} */

/** @defgroup SYSCTL_SDPMST: 	(sysctl:0x1CC) Sleep / Deep-Sleep Power Mode Status */
/** @} */

/** @defgroup SYSCTL_RESBEHAVCTL:(sysctl:0x1D8) Reset Behavior Control Register */
/** @} */

/** @defgroup SYSCTL_HSSR: 		(sysctl:0x1F4) Hardware System Service Request  */
/** @} */

/** @defgroup SYSCTL_USBPDS: 	(sysctl:0x280) USB Power Domain Status          */
/** @} */

/** @defgroup SYSCTL_USBMPC: 	(sysctl:0x284) USB Memory Power Control         */
/** @} */

/** @defgroup SYSCTL_EMACPDS:	(sysctl:0x288) Ethernet MAC Power Domain Status */
/** @} */

/** @defgroup SYSCTL_EMACMPC:	(sysctl:0x28C) Ethernet MAC Memory Power Control*/
/** @} */

/** @defgroup SYSCTL_CAN0PDS:	(sysctl:0x298) CAN 0 Power Domain Status        */
/** @} */

/** @defgroup SYSCTL_CAN0MPC:	(sysctl:0x29C) CAN 0 Memory Power Control       */
/** @} */

/** @defgroup SYSCTL_CAN1PDS:	(sysctl:0x2A0) CAN 1 Power Domain Status        */
/** @} */

/** @defgroup SYSCTL_CAN1MPC:	(sysctl:0x2A4) CAN 1 Memory Power Control       */
/** @} */

/** @defgroup SYSCTL_PPWD: 		(sysctl:0x300) Watchdog Timer Peripheral Present*/
/** @} */

/** @defgroup SYSCTL_PPTIMER:	(sysctl:0x304) 16/32-Bit General-Purpose Timer Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPGPIO: 	(sysctl:0x308) General-Purpose Input/Output Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPDMA: 	(sysctl:0x30C) Micro Direct Memory Access Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPEPI: 	(sysctl:0x310) EPI Peripheral Present			*/
/** @} */

/** @defgroup SYSCTL_PPHIB: 	(sysctl:0x314) Hibernation Peripheral Present   */
/** @} */

/** @defgroup SYSCTL_PPUART: 	(sysctl:0x318) Universal Asynchronous Receiver/Transmitter Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPSSI: 	(sysctl:0x31C) Synchronous Serial Interface Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPI2C: 	(sysctl:0x320) Inter-Integrated Circuit Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPUSB: 	(sysctl:0x328) Universal Serial Bus Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPEPHY: 	(sysctl:0x330) Ethernet PHY Peripheral Present	*/
/** @} */

/** @defgroup SYSCTL_PPCAN: 	(sysctl:0x334) Controller Area Network Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPADC: 	(sysctl:0x338) Analog-to-Digital Converter Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPACMP: 	(sysctl:0x33C) Analog Comparator Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPPWM: 	(sysctl:0x340) Pulse Width Modulator Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPQEI: 	(sysctl:0x344) Quadrature Encoder Interface Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPLPC: 	(sysctl:0x348) Low Pin Count Interface Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPPECI: 	(sysctl:0x350) Platform Environment Control Interface Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPFAN: 	(sysctl:0x354) Fan Control Peripheral Present	*/
/** @} */

/** @defgroup SYSCTL_PPEEPROM: 	(sysctl:0x358) EEPROM Peripheral Present        */
/** @} */

/** @defgroup SYSCTL_PPWTIMER: 	(sysctl:0x35C) 32/64-Bit Wide General-Purpose Timer Peripheral  Present */
/** @} */

/** @defgroup SYSCTL_PPRTS: 	(sysctl:0x370) Remote Temperature Sensor Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPCCM: 	(sysctl:0x374) CRC and Cryptographic Modules Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPLCD: 	(sysctl:0x390) LCD Peripheral Present			*/
/** @} */

/** @defgroup SYSCTL_PPOWIRE:	(sysctl:0x398) 1-Wire Peripheral Present        */
/** @} */

/** @defgroup SYSCTL_PPEMAC: 	(sysctl:0x39C) Ethernet MAC Peripheral Present  */
/** @} */

/** @defgroup SYSCTL_PPPRB: 	(sysctl:0x3A0) Power Regulator Bus Peripheral Present */
/** @} */

/** @defgroup SYSCTL_PPHIM: 	(sysctl:0x3A4) Human Interface Master Peripheral Present */
/** @} */

/** @defgroup SYSCTL_SRWD: 		(sysctl:0x500) Watchdog Timer Software Reset	*/
/** @} */

/** @defgroup SYSCTL_SRTIMER:	(sysctl:0x504) 16/32-Bit General-Purpose Timer Software Reset */
/** @} */

/** @defgroup SYSCTL_SRGPIO: 	(sysctl:0x508) General-Purpose Input/Output Software Reset */
/** @} */

/** @defgroup SYSCTL_SRDMA: 	(sysctl:0x50C) Micro Direct Memory Access Software Reset */
/** @} */

/** @defgroup SYSCTL_SREPI: 	(sysctl:0x510) EPI Software Reset				*/
/** @} */

/** @defgroup SYSCTL_SRHIB: 	(sysctl:0x514) Hibernation Software Reset       */
/** @} */

/** @defgroup SYSCTL_SRUART: 	(sysctl:0x518) Universal Asynchronous Receiver/Transmitter Software Reset */
/** @} */

/** @defgroup SYSCTL_SRSSI: 	(sysctl:0x51C) Synchronous Serial Interface Software Reset */
/** @} */

/** @defgroup SYSCTL_SRI2C: 	(sysctl:0x520) Inter-Integrated Circuit Software Reset */
/** @} */

/** @defgroup SYSCTL_SRUSB: 	(sysctl:0x528) Universal Serial Bus Software Reset */
/** @} */

/** @defgroup SYSCTL_SRCAN: 	(sysctl:0x534) Controller Area Network Software Reset */
/** @} */

/** @defgroup SYSCTL_SRADC: 	(sysctl:0x538) Analog-to-Digital Converter Software Reset */
/** @} */

/** @defgroup SYSCTL_SRACMP: 	(sysctl:0x53C) Analog Comparator Software Reset */
/** @} */

/** @defgroup SYSCTL_SRPWM: 	(sysctl:0x540) Pulse Width Modulator Software Reset */
/** @} */

/** @defgroup SYSCTL_SRQEI: 	(sysctl:0x544) Quadrature Encoder Interface Software Reset */
/** @} */

/** @defgroup SYSCTL_SREEPROM: 	(sysctl:0x558) EEPROM Software Reset 			*/
/** @} */

/** @defgroup SYSCTL_SRCCM: 	(sysctl:0x574) CRC and Cryptographic Modules Software Reset */
/** @} */

/** @defgroup SYSCTL_SREMAC: 	(sysctl:0x59C) Ethernet MAC Software Reset		*/
/** @} */

/** @defgroup SYSCTL_RCGCWD: 	(sysctl:0x600) Watchdog Timer Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCTIMER: (sysctl:0x604) 16/32-Bit General-Purpose Timer Run Mode Clock Gating  Control */
/** @} */

/** @defgroup SYSCTL_RCGCGPIO: 	(sysctl:0x608) General-Purpose Input/Output Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCDMA: 	(sysctl:0x60C) Micro Direct Memory Access Run Mode Clock Gating  Control */
/** @} */

/** @defgroup SYSCTL_RCGCEPI: 	(sysctl:0x610) EPI Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCHIB: 	(sysctl:0x614) Hibernation Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCUART: 	(sysctl:0x618) Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCSSI: 	(sysctl:0x61C) Synchronous Serial Interface Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCI2C: 	(sysctl:0x620) Inter-Integrated Circuit Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCUSB: 	(sysctl:0x628) Universal Serial Bus Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCCAN: 	(sysctl:0x634) Controller Area Network Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCADC: 	(sysctl:0x638) Analog-to-Digital Converter Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCACMP: 	(sysctl:0x63C) Analog Comparator Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCPWM: 	(sysctl:0x640) Pulse Width Modulator Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCQEI: 	(sysctl:0x644) Quadrature Encoder Interface Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCEEPROM:(sysctl:0x658) EEPROM Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCCCM: 	(sysctl:0x674) CRC and Cryptographic Modules Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_RCGCEMAC: 	(sysctl:0x69C) Ethernet MAC Run Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCWD: 	(sysctl:0x700) Watchdog Timer Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCTIMER: (sysctl:0x704) 16/32-Bit General-Purpose Timer Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCGPIO: 	(sysctl:0x708) General-Purpose Input/Output Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCDMA: 	(sysctl:0x70C) Micro Direct Memory Access Sleep Mode Clock Gating Control*/
/** @} */

/** @defgroup SYSCTL_SCGCEPI: 	(sysctl:0x710) EPI Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCHIB: 	(sysctl:0x714) Hibernation Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCUART: 	(sysctl:0x718) Universal Asynchronous Receiver/Transmitter Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCSSI: 	(sysctl:0x71C) Synchronous Serial Interface Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCI2C: 	(sysctl:0x720) Inter-Integrated Circuit Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCUSB: 	(sysctl:0x728) Universal Serial Bus Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCCAN: 	(sysctl:0x734) Controller Area Network Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCADC: 	(sysctl:0x738) Analog-to-Digital Converter Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCACMP: 	(sysctl:0x73C) Analog Comparator Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCPWM: 	(sysctl:0x740) Pulse Width Modulator Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCQEI: 	(sysctl:0x744) Quadrature Encoder Interface Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCEEPROM:(sysctl:0x758) EEPROM Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCCCM: 	(sysctl:0x774) CRC and Cryptographic Modules Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_SCGCEMAC: 	(sysctl:0x79C) Ethernet MAC Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCWD: 	(sysctl:0x800) Watchdog Timer Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCTIMER: (sysctl:0x804) 16/32-Bit General-Purpose Timer Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCGPIO: 	(sysctl:0x808) General-Purpose Input/Output Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCDMA: 	(sysctl:0x80C) Micro Direct Memory Access Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCEPI: 	(sysctl:0x810) EPI Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCHIB: 	(sysctl:0x814) Hibernation Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCUART: 	(sysctl:0x818) Universal Asynchronous Receiver/Transmitter Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCSSI: 	(sysctl:0x81C) Synchronous Serial Interface Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCI2C: 	(sysctl:0x820) Inter-Integrated Circuit Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCUSB: 	(sysctl:0x828) Universal Serial Bus Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCCAN: 	(sysctl:0x834) Controller Area Network Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCADC: 	(sysctl:0x838) Analog-to-Digital Converter Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCACMP: 	(sysctl:0x83C) Analog Comparator Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCPWM: 	(sysctl:0x840) Pulse Width Modulator Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCQEI: 	(sysctl:0x844) Quadrature Encoder Interface Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCEEPROM:(sysctl:0x858) EEPROM Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCCCM: 	(sysctl:0x874) CRC and Cryptographic Modules Deep-Sleep Mode Clock Gating Control */
/** @} */

/** @defgroup SYSCTL_DCGCEMAC: 	(sysctl:0x89C) Ethernet MAC Deep-Sleep Mode Clock Gating Control  */
/** @} */

/** @defgroup SYSCTL_PCWD: 		(sysctl:0x900) Watchdog Timer Power Control		 */
/** @} */

/** @defgroup SYSCTL_PCTIMER: 	(sysctl:0x904) 16/32-Bit General-Purpose Timer Power Control  */
/** @} */

/** @defgroup SYSCTL_PCGPIO: 	(sysctl:0x908) General-Purpose Input/Output Power Control */
/** @} */

/** @defgroup SYSCTL_PCDMA: 	(sysctl:0x90C) Micro Direct Memory Access Power Control */
/** @} */

/** @defgroup SYSCTL_PCEPI: 	(sysctl:0x910) External Peripheral Interface Power Control */
/** @} */

/** @defgroup SYSCTL_PCHIB: 	(sysctl:0x914) Hibernation Power Control */
/** @} */

/** @defgroup SYSCTL_PCUART: 	(sysctl:0x918) Universal Asynchronous Receiver/Transmitter Power Control */
/** @} */

/** @defgroup SYSCTL_PCSSI: 	(sysctl:0x91C) Synchronous Serial Interface Power Control */
/** @} */

/** @defgroup SYSCTL_PCI2C: 	(sysctl:0x920) Inter-Integrated Circuit Power Control */
/** @} */

/** @defgroup SYSCTL_PCUSB: 	(sysctl:0x928) Universal Serial Bus Power Control */
/** @} */

/** @defgroup SYSCTL_PCCAN: 	(sysctl:0x934) Controller Area Network Power Control */
/** @} */

/** @defgroup SYSCTL_PCADC: 	(sysctl:0x938) Analog-to-Digital Converter Power Control */
/** @} */

/** @defgroup SYSCTL_PCACMP: 	(sysctl:0x93C) Analog Comparator Power Control */
/** @} */

/** @defgroup SYSCTL_PCPWM: 	(sysctl:0x940) Pulse Width Modulator Power Control */
/** @} */

/** @defgroup SYSCTL_PCQEI: 	(sysctl:0x944) Quadrature Encoder Interface Power Control */
/** @} */

/** @defgroup SYSCTL_PCEEPROM: 	(sysctl:0x958) EEPROM Power Control			 */
/** @} */

/** @defgroup SYSCTL_PCCCM: 	(sysctl:0x974) CRC and Cryptographic Modules Power Control */
/** @} */

/** @defgroup SYSCTL_PCEMAC: 	(sysctl:0x99C) Ethernet MAC Power Control	 */
/** @} */

/** @defgroup SYSCTL_PRWD: 		(sysctl:0xA00) Watchdog Timer Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRTIMER: 	(sysctl:0xA04) 16/32-Bit General-Purpose Timer Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRGPIO: 	(sysctl:0xA08) General-Purpose Input/Output Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRDMA: 	(sysctl:0xA0C) Micro Direct Memory Access Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PREPI: 	(sysctl:0xA10) EPI Peripheral Ready				 */
/** @} */

/** @defgroup SYSCTL_PRHIB: 	(sysctl:0xA14) Hibernation Peripheral Ready		 */
/** @} */

/** @defgroup SYSCTL_PRUART: 	(sysctl:0xA18) Universal Asynchronous Receiver/Transmitter Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRSSI: 	(sysctl:0xA1C) Synchronous Serial Interface Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRI2C: 	(sysctl:0xA20) Inter-Integrated Circuit Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRUSB: 	(sysctl:0xA28) Universal Serial Bus Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRCAN: 	(sysctl:0xA34) Controller Area Network Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRADC: 	(sysctl:0xA38) Analog-to-Digital Converter Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRACMP: 	(sysctl:0xA3C) Analog Comparator Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRPWM: 	(sysctl:0xA40) Pulse Width Modulator Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PRQEI: 	(sysctl:0xA44) Quadrature Encoder Interface Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PREEPROM: 	(sysctl:0xA58) EEPROM Peripheral Ready			 */
/** @} */

/** @defgroup SYSCTL_PRCCM: 	(sysctl:0xA74) CRC and Cryptographic Modules Peripheral Ready */
/** @} */

/** @defgroup SYSCTL_PREMAC: 	(sysctl:0xA9C) Ethernet MAC Peripheral Ready	 */
/** @} */


/** @} */ // @defgroup SYSCTL_regs_define


struct TM4C_CLOCK_CFG
{
	uint32_t CFG_clk_freq;
	uint32_t CFG_xtal_freq;
	uint32_t CFG_RSCLKCFG;
	uint32_t CFG_PLLFREQ0;
	uint32_t CFG_PLLFREQ1;
};

void SysCtlClockSet(const TM4C_CLOCK_CFG* cfg);

#endif /* SYSCTL_TM4C_H_ */
