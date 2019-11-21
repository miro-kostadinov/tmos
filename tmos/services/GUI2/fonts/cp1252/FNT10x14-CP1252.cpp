#include <tmos.h>
#include <fonts.h>

const char font10x14_cp1252[4032] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0C, 0x30, 0xE0, 0x81, 0x07, 0x1E, 0x78, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0x00, 0x00, 
	0x00, 0x0C, 0x30, 0x00, 0xCC, 0x30, 0xC3, 0x0C, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0x33, 0xFF, 0xFF, 0xCF, 0x0C, 0x33, 
	0xFF, 0xFF, 0xCF, 0x0C, 0x33, 0x00, 0x00, 0x00, 0x30, 0xC0, 0xC0, 0x0F, 0x3F, 0x33, 0xCC, 0xC0, 
	0x0F, 0x3F, 0x30, 0xC3, 0xCC, 0x0F, 0x3F, 0x30, 0xC0, 0x00, 0x0F, 0x3F, 0xFC, 0xF0, 0xC3, 0xC0, 
	0x00, 0x03, 0x03, 0x0C, 0x0C, 0x30, 0x30, 0xFC, 0xF0, 0xC3, 0x0F, 0x0F, 0x0C, 0x30, 0x30, 0xC3, 
	0x0C, 0x0C, 0x30, 0x30, 0xF3, 0xCC, 0xC3, 0x0C, 0x33, 0xCC, 0x30, 0x3C, 0xF3, 0x0C, 0x30, 0xC0, 
	0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xC0, 0x00, 0x03, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0xC0, 
	0x00, 0x03, 0x0C, 0x30, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 
	0x0C, 0x0C, 0x30, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x33, 0xCF, 0xCC, 0x0F, 0x3F, 0x33, 0xCF, 
	0x0C, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0xF0, 0xFF, 0xFF, 
	0x30, 0xC0, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x03, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xF0, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x03, 
	0x0C, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x03, 0x0C, 0x00, 
	0xFC, 0xF0, 0x33, 0xFC, 0xF0, 0x33, 0xCF, 0x3C, 0xF3, 0xCC, 0x33, 0xCF, 0xFC, 0xF0, 0xC3, 0xFC, 
	0xF0, 0x03, 0x30, 0xC0, 0xC0, 0x03, 0x0F, 0x33, 0xCC, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 
	0x0C, 0xFF, 0xFF, 0x0F, 0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x00, 0x03, 0x0C, 0x0C, 0x30, 0x30, 0xC0, 
	0xC0, 0x00, 0x03, 0xFF, 0xFF, 0x0F, 0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x00, 0x03, 0x0C, 0x0F, 0x3C, 
	0x00, 0x03, 0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 0xF0, 0xC0, 0xC3, 0x0C, 0x33, 0xCC, 0x30, 0x33, 
	0xCC, 0x30, 0xFF, 0xFF, 0x0F, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0xFF, 0xFF, 0x3F, 0xC0, 0x00, 0xFF, 
	0xFC, 0x03, 0x30, 0xC0, 0x00, 0x03, 0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 0xFC, 0xF0, 0x33, 0xF0, 
	0xC0, 0x03, 0x0C, 0xF0, 0xCF, 0x3F, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 0xFF, 0xFF, 
	0x0F, 0x30, 0xC0, 0xC0, 0x00, 0x03, 0x03, 0x0C, 0x30, 0xC0, 0xC0, 0x00, 0x03, 0x03, 0x0C, 0x00, 
	0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x03, 0x0F, 0xCC, 0x0F, 0x3F, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 
	0xF0, 0x03, 0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x03, 0x0F, 0xCC, 0x3F, 0xFF, 0x00, 0x03, 0x3C, 0xF0, 
	0xC0, 0xFC, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC0, 0x00, 0x03, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x30, 0xC0, 0xC0, 
	0x00, 0x03, 0x30, 0xC0, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0x0F, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 
	0x03, 0x30, 0xC0, 0x00, 0x0C, 0x30, 0x30, 0xC0, 0xC0, 0x00, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xF0, 
	0x33, 0xF0, 0xC0, 0x00, 0x03, 0x0C, 0x0C, 0x30, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x30, 0xC0, 0x00, 
	0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x00, 0x03, 0xCC, 0x33, 0xCF, 0x33, 0xCF, 0x3C, 0xF3, 0xCC, 0xFC, 
	0xF0, 0x03, 0x30, 0xC0, 0xC0, 0x0C, 0x33, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFF, 0xFF, 0x3F, 0xF0, 
	0xC0, 0x03, 0x0F, 0x0C, 0xFF, 0xFC, 0x33, 0xF0, 0xC0, 0x03, 0x0F, 0xFC, 0xCF, 0x3F, 0x03, 0x0F, 
	0x3C, 0xF0, 0xC0, 0xFF, 0xFC, 0x03, 0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x03, 0x0C, 0x30, 0xC0, 0x00, 
	0x03, 0x0C, 0x30, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 0xFF, 0xFC, 0x33, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 
	0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFF, 0xFC, 0x03, 0xFF, 0xFF, 0x3F, 0xC0, 0x00, 0x03, 
	0x0C, 0xF0, 0xFF, 0xFF, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0x3F, 0xC0, 
	0x00, 0x03, 0x0C, 0xF0, 0xFF, 0xFF, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x00, 0xFC, 0xF0, 
	0x33, 0xF0, 0xC0, 0x03, 0x0C, 0x30, 0xFF, 0xFC, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 
	0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0xFC, 0xFF, 0xFF, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 
	0x0F, 0x0C, 0xFC, 0xF0, 0x03, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 
	0x0C, 0xFC, 0xF0, 0x03, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 
	0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 0x03, 0x0F, 0x3C, 0xCC, 0x30, 0x33, 0xCC, 0xF0, 0xC0, 0x03, 
	0x33, 0xCC, 0x30, 0xCC, 0x30, 0x03, 0x0F, 0x0C, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 
	0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0xFF, 0xFF, 0x0F, 0x03, 0x0F, 0xFC, 0xFC, 0xF3, 0x33, 
	0xCF, 0x3C, 0xF3, 0xCC, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0x0C, 0x03, 0x0F, 0xFC, 0xF0, 
	0xC3, 0x33, 0xCF, 0x3C, 0xF3, 0xCC, 0xC3, 0x0F, 0x3F, 0xF0, 0xC0, 0x03, 0x0F, 0x0C, 0xFC, 0xF0, 
	0x33, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 
	0xFF, 0xFC, 0x33, 0xF0, 0xC0, 0x03, 0x0F, 0xFC, 0xCF, 0x3F, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 
	0x0C, 0x00, 0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x33, 0xCF, 0x3C, 0xCC, 
	0x30, 0x3C, 0xF3, 0x0C, 0xFF, 0xFC, 0x33, 0xF0, 0xC0, 0x03, 0x0F, 0xFC, 0xFF, 0xFF, 0x33, 0xCC, 
	0x30, 0xCC, 0x30, 0x03, 0x0F, 0x0C, 0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x03, 0x0C, 0xC0, 0x0F, 0x3F, 
	0x00, 0x03, 0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 0xFF, 0xFF, 0x0F, 0x03, 0x0C, 0x30, 0xC0, 0x00, 
	0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 
	0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 0x03, 0x0F, 0x3C, 0xF0, 
	0xC0, 0x03, 0x0F, 0xCC, 0x0C, 0x33, 0xCC, 0x30, 0xC3, 0x0C, 0x33, 0x30, 0xC0, 0x00, 0x03, 0x0F, 
	0x3C, 0xF0, 0xC0, 0x33, 0xCF, 0x3C, 0xF3, 0xCC, 0x33, 0xCF, 0xCC, 0x0C, 0x33, 0xCC, 0x30, 0x03, 
	0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xCC, 0x30, 0x03, 0x03, 0x0C, 0xCC, 0x30, 0x33, 0xF0, 0xC0, 0x03, 
	0x0F, 0x0C, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x87, 0x3B, 0xC7, 0x0F, 0x1E, 0x30, 0xC0, 0x00, 0x03, 
	0x0C, 0x30, 0xC0, 0x00, 0xFF, 0xFF, 0x0F, 0x30, 0xC0, 0xC0, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x30, 
	0x30, 0xC0, 0x00, 0xFF, 0xFF, 0x0F, 0xF0, 0xC0, 0x03, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 
	0x30, 0xC0, 0x00, 0x03, 0x0C, 0xF0, 0xC0, 0x03, 0x03, 0x0C, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0x00, 
	0x03, 0x0C, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0x00, 0x03, 0x0C, 0x3C, 0xF0, 0x00, 0x03, 0x0C, 0x30, 
	0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x3C, 0xF0, 0x00, 0x30, 0xC0, 0xC0, 0x0C, 
	0x33, 0x03, 0x0F, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 
	0x30, 0xC0, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xF8, 0x27, 0x30, 0xC0, 0xFC, 0xFB, 0x3F, 0xF0, 
	0xC0, 0xFE, 0xF3, 0x0D, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0xF3, 0xCC, 0xF3, 0xF0, 0xC3, 0x03, 0x0F, 
	0x3C, 0xF0, 0xC0, 0xFF, 0xFC, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xF8, 0x77, 0xF0, 0x80, 
	0x03, 0x0C, 0x30, 0xE0, 0xC1, 0xFE, 0xF1, 0x03, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x3C, 0xF3, 0x3C, 
	0xFC, 0xF0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 0xF3, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 
	0xF0, 0x33, 0xF0, 0xC0, 0xFF, 0xFF, 0x3F, 0xC0, 0x00, 0xFC, 0xF0, 0x03, 0xF0, 0xC0, 0xC3, 0x30, 
	0xC3, 0x0C, 0x30, 0xF0, 0xCF, 0x3F, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xFC, 0xF3, 0x3F, 0xC0, 0x00, 0xF3, 0xCF, 0x3F, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 
	0x03, 0x0C, 0x30, 0xC0, 0x00, 0xF3, 0xCC, 0xF3, 0xF0, 0xC3, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 
	0x0F, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x3C, 0xF0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 
	0x0C, 0xFF, 0xFF, 0x0F, 0xC0, 0x00, 0x03, 0x00, 0x00, 0xFC, 0xF3, 0x0F, 0x0C, 0x30, 0xC0, 0x00, 
	0x33, 0xCC, 0x30, 0x3C, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x0C, 0x33, 0xC3, 0x0C, 
	0x0F, 0x3C, 0x30, 0xC3, 0x0C, 0xC3, 0x0F, 0x0F, 0x3C, 0xF0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 
	0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCF, 
	0x3F, 0x3F, 0xF3, 0xCC, 0x33, 0xCF, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0x0C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xF3, 0xCC, 0xF3, 0xF0, 0xC3, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0x0C, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 0xF0, 0x03, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0x33, 0xF0, 0xC0, 0xFF, 0xFC, 0x33, 0xC0, 0x00, 0x03, 
	0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xF3, 0x3F, 0xF0, 0xC0, 0xC3, 0x0F, 0xCF, 0x33, 
	0xCF, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0xCF, 0xFF, 0xC0, 0x03, 0x03, 0x0C, 
	0x30, 0xC0, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xF3, 0x3F, 0xC0, 0x00, 
	0xFC, 0xF0, 0x03, 0x30, 0xC0, 0xFF, 0xFC, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x3F, 0xFC, 0xC0, 
	0x00, 0x03, 0x0C, 0x30, 0xC0, 0x30, 0xC3, 0xF0, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
	0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 0xF3, 0x0F, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0xCC, 0x0C, 0x33, 0x30, 0xC0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x0F, 0x3C, 0xF3, 0xCC, 0x33, 0xCF, 0x3C, 0xF3, 0xCC, 0xCF, 0x3F, 0x0F, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0xCC, 0x0C, 0x33, 0x30, 0xC0, 0xC0, 0x0C, 0x33, 0x03, 
	0x0F, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFC, 0xF3, 0x0F, 0x30, 
	0xC0, 0xFC, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x18, 0x30, 0x60, 0xE0, 
	0xC0, 0x80, 0x01, 0xFF, 0xFF, 0x0F, 0xF0, 0xC0, 0x03, 0x03, 0x0C, 0x30, 0xC0, 0xC0, 0x03, 0x0F, 
	0x30, 0xC0, 0x00, 0x03, 0x0C, 0xF0, 0xC0, 0x03, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 
	0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x3C, 0xF0, 0x00, 0x03, 0x0C, 0x30, 
	0xC0, 0x00, 0x0F, 0x3C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x3C, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0C, 0x78, 0x30, 0x63, 0xCC, 0xE0, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xF0, 
	0xC3, 0x0C, 0x33, 0xCC, 0x30, 0xC3, 0x0C, 0x33, 0xCC, 0x30, 0xC3, 0x0C, 0x33, 0xFC, 0xF0, 0x03, 
	0xF0, 0xE0, 0xC7, 0x30, 0x03, 0x3F, 0xFC, 0xC0, 0x00, 0x03, 0x3F, 0xFC, 0xC0, 0x00, 0xC3, 0xF8, 
	0xC1, 0x03, 0x80, 0x01, 0xF3, 0xFF, 0xFF, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 
	0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x80, 0x03, 0x0C, 0x18, 0x30, 0x00, 0xC0, 0x80, 0x07, 0x13, 0x0C, 0x30, 0xF0, 0xC3, 0x0F, 0x0C, 
	0x30, 0xC0, 0x00, 0x83, 0x0C, 0x1E, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x30, 0xC3, 0x0C, 0x33, 0x88, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xF3, 0xCC, 0x00, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x03, 
	0x0C, 0xFF, 0xFF, 0x0F, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x30, 0xC0, 
	0x00, 0xC3, 0xFF, 0xFF, 0xC3, 0x00, 0x03, 0x0C, 0x30, 0xFC, 0xFF, 0x3F, 0x0C, 0x30, 0xC0, 0x00, 
	0x30, 0xE0, 0xC1, 0x8C, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x06, 0x27, 0x96, 0x8C, 0x19, 0x30, 0x60, 0xC0, 0x80, 0x01, 0x03, 0x34, 0x26, 0xA5, 
	0x94, 0x8C, 0x01, 0x00, 0x48, 0xC0, 0xC0, 0x8F, 0x7F, 0x03, 0x0E, 0x30, 0x80, 0x3F, 0xFC, 0x01, 
	0x0C, 0x70, 0xC0, 0xFE, 0xF1, 0x03, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x00, 0x06, 
	0x30, 0x80, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFB, 0x6F, 0xC3, 0x0C, 0x33, 0xCC, 0xF0, 
	0xFF, 0xFF, 0x33, 0xCC, 0x30, 0x83, 0x0D, 0xFE, 0xF3, 0x0F, 0x1B, 0xCE, 0x3C, 0xD8, 0x30, 0x63, 
	0xCC, 0xB0, 0xC1, 0x03, 0x1F, 0xCC, 0x30, 0xC6, 0x30, 0x83, 0x0D, 0x0C, 0x48, 0xC0, 0xF0, 0xFF, 
	0xFF, 0x00, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x01, 0xFF, 0xFF, 0x0F, 0x86, 0x19, 
	0x66, 0x98, 0x61, 0x86, 0x19, 0x66, 0x98, 0x61, 0x86, 0xF9, 0xE7, 0x1F, 0x0C, 0x30, 0xC0, 0x00, 
	0x0C, 0x30, 0xF0, 0xC3, 0x0F, 0x0C, 0x30, 0xC0, 0x0C, 0x7B, 0x3C, 0x73, 0xCC, 0x30, 0xC3, 0xCC, 
	0x31, 0x03, 0x10, 0x20, 0x80, 0x03, 0x0E, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0xE0, 0x80, 0x03, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x08, 0xE1, 0x9C, 0x73, 0x84, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x38, 0xE7, 0x1C, 0x42, 0x84, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0xFC, 
	0xF0, 0xF3, 0xFF, 0xFF, 0xFC, 0xF0, 0x03, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x80, 0x07, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x40, 0x88, 0x33, 0x7B, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x2F, 0xBE, 0x6D, 0xBE, 0xF9, 0x66, 0x9B, 0x6D, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x88, 0x40, 0x01, 0x02, 0x00, 0xFC, 0xF9, 0x3F, 0xC0, 0x00, 0xFE, 0xF0, 
	0x07, 0x30, 0xC0, 0xFF, 0xF9, 0x03, 0x00, 0x00, 0x80, 0x01, 0x0C, 0x60, 0x00, 0x03, 0x18, 0x30, 
	0x60, 0xC0, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xFD, 0x3F, 
	0xE3, 0x8C, 0xF3, 0xCF, 0x37, 0xC3, 0x0C, 0xFF, 0x3B, 0x07, 0x00, 0x00, 0x00, 0x06, 0x0C, 0x83, 
	0x8D, 0x33, 0xC7, 0x0E, 0x0F, 0x3C, 0xB0, 0xC1, 0x1C, 0xE3, 0x0C, 0x06, 0x88, 0x40, 0x01, 0x02, 
	0x00, 0xFF, 0xFF, 0x0F, 0x18, 0x30, 0x70, 0x60, 0xC0, 0x80, 0x01, 0xFF, 0xFF, 0x0F, 0x4B, 0x2F, 
	0x3D, 0xF0, 0xC0, 0x87, 0x3B, 0xC7, 0x0F, 0x1E, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x00, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x1E, 0x78, 0xE0, 0x81, 
	0x07, 0x0C, 0x30, 0x00, 0x00, 0xC0, 0x00, 0x03, 0x3F, 0xFE, 0x1D, 0x3C, 0xE0, 0x00, 0x03, 0x0C, 
	0x78, 0xB0, 0x7F, 0xFC, 0xC0, 0x00, 0x00, 0xC0, 0x81, 0x0D, 0x23, 0x0C, 0x30, 0xE0, 0x83, 0x0F, 
	0x0C, 0x30, 0xC0, 0x00, 0x03, 0xFF, 0xFC, 0x03, 0x00, 0x00, 0x30, 0xF0, 0xC0, 0xFC, 0xF0, 0xC3, 
	0x0C, 0x33, 0xFC, 0xF0, 0x33, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x04, 0x38, 0xB0, 0x61, 0xCC, 
	0xE0, 0x01, 0x03, 0x3F, 0x30, 0xC0, 0xC0, 0x0F, 0x0C, 0x30, 0xC0, 0x00, 0x30, 0xC0, 0x00, 0x03, 
	0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0xFC, 0xF8, 
	0x37, 0xF0, 0x80, 0x3E, 0xF0, 0xC1, 0x0C, 0x33, 0xF8, 0xC0, 0x17, 0xE0, 0xC0, 0xFE, 0xF1, 0x03, 
	0xCC, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xF0, 0xE3, 0xDF, 0xC0, 0x73, 0x6F, 0xBC, 0xF1, 0xC6, 0x73, 0x0F, 0xEC, 0x1F, 
	0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x8F, 0x7F, 0x80, 0x03, 0xCC, 0xBF, 0xFF, 0x03, 0x0F, 
	0xEC, 0x3F, 0xFF, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x22, 0xCC, 0x98, 0x31, 0x63, 0x86, 0x19, 
	0xCC, 0x60, 0x06, 0x33, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x03, 
	0x0C, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xC0, 0x03, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE3, 0xDF, 
	0xC0, 0x3B, 0x2F, 0xBD, 0xF3, 0xCA, 0x4B, 0x0F, 0xEC, 0x1F, 0x3F, 0x00, 0x00, 0x00, 0xFF, 0xFC, 
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x30, 0xE0, 0xC1, 0x0C, 0x33, 0x78, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0xFC, 0xF0, 0x03, 0x03, 0x0C, 0x00, 0x00, 0xC0, 0x0F, 
	0x3F, 0x00, 0x00, 0x00, 0x1C, 0xD8, 0x00, 0x03, 0x06, 0x0C, 0xF8, 0xE0, 0x03, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x88, 0x00, 0x03, 0x07, 0x20, 0xC8, 0xC0, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 0xC0, 0x80, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 
	0x3C, 0xC3, 0x0C, 0x33, 0xCC, 0x30, 0xC3, 0x0F, 0x3F, 0x0C, 0x33, 0x0C, 0xF8, 0xFB, 0xFF, 0xF3, 
	0xCF, 0x3E, 0xF3, 0x0C, 0x33, 0xCC, 0x30, 0xC3, 0x0C, 0x33, 0xCC, 0x30, 0xC3, 0x0C, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x20, 0xF0, 
	0xC0, 0x01, 0x08, 0x30, 0xE0, 0x00, 0x03, 0x0C, 0x30, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x30, 0xE0, 0xC1, 0x0C, 0x33, 0x78, 0xC0, 0x00, 0x00, 0x00, 0xFC, 0xF0, 
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xC1, 0x0C, 0x66, 0x30, 0x83, 0x19, 0x66, 
	0xCC, 0x98, 0x31, 0x43, 0x04, 0x00, 0x00, 0x00, 0x04, 0x18, 0x70, 0x80, 0x41, 0x86, 0x18, 0xF1, 
	0x12, 0x64, 0xC8, 0x91, 0x25, 0x12, 0xF8, 0x80, 0x01, 0x0F, 0x04, 0x18, 0x70, 0x80, 0x41, 0x86, 
	0x18, 0xF1, 0x02, 0x34, 0xE8, 0x91, 0x2C, 0x30, 0x60, 0xC0, 0x80, 0x0F, 0x0E, 0x44, 0x80, 0x01, 
	0x03, 0x90, 0x64, 0xE1, 0x12, 0x64, 0xC8, 0x91, 0x25, 0x12, 0xF8, 0x80, 0x01, 0x0F, 0x30, 0xC0, 
	0x00, 0x00, 0x00, 0x30, 0xC0, 0x80, 0x01, 0x03, 0x06, 0x0C, 0x30, 0xE0, 0xC0, 0xFE, 0xF1, 0x03, 
	0x18, 0xC0, 0x00, 0x03, 0x1E, 0xCC, 0x18, 0x66, 0xD8, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0xC0, 0x03, 
	0x0F, 0x0C, 0x60, 0xC0, 0x00, 0x03, 0x1E, 0xCC, 0x18, 0x66, 0xD8, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 
	0xC0, 0x03, 0x0F, 0x0C, 0x30, 0x20, 0x01, 0x00, 0x1E, 0xCC, 0x18, 0x66, 0xD8, 0xC0, 0x03, 0xFF, 
	0xFF, 0xFF, 0xC0, 0x03, 0x0F, 0x0C, 0x0C, 0x49, 0x02, 0x06, 0x1E, 0xCC, 0x18, 0x66, 0xD8, 0xC0, 
	0x03, 0xFF, 0xFF, 0xFF, 0xC0, 0x03, 0x0F, 0x0C, 0xCC, 0x30, 0x03, 0x00, 0x1E, 0xCC, 0x18, 0x66, 
	0xD8, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0xC0, 0x03, 0x0F, 0x0C, 0x30, 0x20, 0x01, 0x03, 0x1E, 0xCC, 
	0x18, 0x66, 0xD8, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0xC0, 0x03, 0x0F, 0x0C, 0xF8, 0xF3, 0xEF, 0x86, 
	0x19, 0x63, 0x8C, 0x31, 0xFE, 0xF8, 0x7F, 0xFC, 0x31, 0xC6, 0x18, 0xE3, 0x8F, 0x0F, 0xFC, 0xF8, 
	0x77, 0xF0, 0x80, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x1E, 0xEC, 0x1F, 0x3F, 0x60, 0xC0, 0x01, 
	0x18, 0xC0, 0xF0, 0xFF, 0xFF, 0x03, 0x0C, 0x30, 0xC0, 0x7F, 0xFF, 0x0D, 0x30, 0xC0, 0x00, 0xFF, 
	0xFF, 0x0F, 0x60, 0xC0, 0xF0, 0xFF, 0xFF, 0x03, 0x0C, 0x30, 0xC0, 0x7F, 0xFF, 0x0D, 0x30, 0xC0, 
	0x00, 0xFF, 0xFF, 0x0F, 0x30, 0x20, 0xF1, 0xFF, 0xFF, 0x03, 0x0C, 0x30, 0xC0, 0x7F, 0xFF, 0x0D, 
	0x30, 0xC0, 0x00, 0xFF, 0xFF, 0x0F, 0xCC, 0x30, 0x03, 0xC0, 0xFF, 0xFF, 0x0F, 0x30, 0xC0, 0x00, 
	0xFF, 0x0D, 0x30, 0xC0, 0x00, 0xFF, 0xFF, 0x0F, 0x18, 0xC0, 0x00, 0x00, 0x3F, 0x30, 0xC0, 0x00, 
	0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xF0, 0x03, 0x60, 0xC0, 0x00, 0x00, 0x3F, 0x30, 
	0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xF0, 0x03, 0x30, 0x20, 0x01, 0x00, 
	0x3F, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xF0, 0x03, 0xCC, 0x30, 
	0x03, 0x00, 0x3F, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xF0, 0x03, 
	0xFE, 0xF8, 0x67, 0x98, 0xC1, 0x06, 0x1B, 0xFC, 0xF1, 0xC7, 0x06, 0x1B, 0x6C, 0x90, 0x61, 0xFE, 
	0xF9, 0x03, 0x0C, 0x49, 0x02, 0xC6, 0xC0, 0x07, 0x1F, 0xFC, 0xF0, 0xC6, 0x33, 0x8F, 0x3D, 0xFC, 
	0xE0, 0x83, 0x0F, 0x0C, 0x18, 0xC0, 0x00, 0x00, 0x3F, 0xFE, 0x1D, 0x3E, 0xF0, 0xC0, 0x03, 0x0F, 
	0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x60, 0xC0, 0x00, 0x00, 0x3F, 0xFE, 0x1D, 0x3E, 0xF0, 0xC0, 
	0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x30, 0x20, 0x01, 0x00, 0x3F, 0xFE, 0x1D, 0x3E, 
	0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x0C, 0x49, 0x02, 0x06, 0x3F, 0xFE, 
	0x1D, 0x3E, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0xCC, 0x30, 0x03, 0x00, 
	0x3F, 0xFE, 0x1D, 0x3E, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x00, 0x00, 
	0x00, 0x80, 0x61, 0xCE, 0x31, 0x03, 0x03, 0x0C, 0xCC, 0x38, 0x67, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0xFC, 0xF8, 0x77, 0xF8, 0xC0, 0x43, 0x8F, 0x3D, 0xF2, 0xC4, 0x1B, 0x2F, 0x3C, 0xF0, 0xE1, 0xFE, 
	0xF1, 0x03, 0x18, 0xC0, 0x00, 0xC0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 
	0xE1, 0xFE, 0xF1, 0x03, 0x60, 0xC0, 0x00, 0xC0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 
	0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x30, 0x20, 0x01, 0xC0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 
	0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0xCC, 0x30, 0x03, 0xC0, 0xC0, 0x03, 0x0F, 0x3C, 
	0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x63, 0xCF, 0x3C, 0xF0, 0xC0, 0x87, 
	0x3B, 0xC7, 0x0F, 0x1E, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0xF0, 0xCF, 
	0x7F, 0x83, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0xFE, 0xDF, 0x3F, 0x03, 0x0C, 0x00, 0x3C, 0xF8, 
	0x21, 0xCE, 0x30, 0x63, 0xCC, 0xB0, 0xC3, 0x3E, 0xE3, 0x0D, 0xFE, 0xF0, 0xE7, 0xFB, 0x8D, 0x03, 
	0x00, 0x60, 0x00, 0x03, 0x00, 0xFC, 0xF8, 0x27, 0x30, 0xC0, 0xFC, 0xFB, 0x3F, 0xF0, 0xC0, 0xFE, 
	0xF3, 0x0D, 0x00, 0x80, 0x01, 0x03, 0x00, 0xFC, 0xF8, 0x27, 0x30, 0xC0, 0xFC, 0xFB, 0x3F, 0xF0, 
	0xC0, 0xFE, 0xF3, 0x0D, 0x00, 0xC0, 0x80, 0x04, 0x00, 0xFC, 0xF8, 0x27, 0x30, 0xC0, 0xFC, 0xFB, 
	0x3F, 0xF0, 0xC0, 0xFE, 0xF3, 0x0D, 0x0C, 0x49, 0x02, 0x06, 0x00, 0xFC, 0xF8, 0x27, 0x30, 0xC0, 
	0xFC, 0xFB, 0x3F, 0xF0, 0xC0, 0xFE, 0xF3, 0x0D, 0x00, 0x30, 0xC3, 0x0C, 0x00, 0xFC, 0xF8, 0x27, 
	0x30, 0xC0, 0xFC, 0xFB, 0x3F, 0xF0, 0xC0, 0xFE, 0xF3, 0x0D, 0x30, 0x20, 0x01, 0x03, 0x00, 0xFC, 
	0xF8, 0x27, 0x30, 0xC0, 0xFC, 0xFB, 0x3F, 0xF0, 0xC0, 0xFE, 0xF3, 0x0D, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xEC, 0xF8, 0x27, 0x33, 0xCC, 0xFC, 0xFB, 0x30, 0xC3, 0x0C, 0xFE, 0x33, 0x07, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xFC, 0xF8, 0x37, 0xF0, 0x00, 0x03, 0x0C, 0xEC, 0x1F, 0x3F, 0x60, 0xC0, 0x01, 
	0x00, 0x60, 0x00, 0x03, 0x00, 0xFC, 0xF8, 0x37, 0xF0, 0xC0, 0xFF, 0xFF, 0x3F, 0xC0, 0x00, 0xFE, 
	0xF1, 0x03, 0x00, 0x80, 0x01, 0x03, 0x00, 0xFC, 0xF8, 0x37, 0xF0, 0xC0, 0xFF, 0xFF, 0x3F, 0xC0, 
	0x00, 0xFE, 0xF1, 0x03, 0x00, 0xC0, 0x80, 0x04, 0x00, 0xFC, 0xF8, 0x37, 0xF0, 0xC0, 0xFF, 0xFF, 
	0x3F, 0xC0, 0x00, 0xFE, 0xF1, 0x03, 0x00, 0x30, 0xC3, 0x0C, 0x00, 0xFC, 0xF8, 0x37, 0xF0, 0xC0, 
	0xFF, 0xFF, 0x3F, 0xC0, 0x00, 0xFE, 0xF1, 0x03, 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0xC0, 0x00, 
	0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x78, 0xE0, 0x01, 0x00, 0x80, 0x01, 0x03, 0x00, 0x00, 
	0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x78, 0xE0, 0x01, 0x00, 0xC0, 0x80, 0x04, 
	0x00, 0x00, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x78, 0xE0, 0x01, 0x00, 0x30, 
	0xC3, 0x0C, 0x00, 0x00, 0xC0, 0x00, 0x03, 0x0C, 0x30, 0xC0, 0x00, 0x03, 0x0C, 0x78, 0xE0, 0x01, 
	0x33, 0x38, 0xC0, 0xC3, 0x38, 0x80, 0xF1, 0xE7, 0xDF, 0xE1, 0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 
	0xF1, 0x03, 0x0C, 0x49, 0x02, 0x06, 0x00, 0xE3, 0xEC, 0xF7, 0xF0, 0xC1, 0x03, 0x0F, 0x3C, 0xF0, 
	0xC0, 0x03, 0x0F, 0x0C, 0x00, 0x60, 0x00, 0x03, 0x00, 0xFC, 0xF8, 0x77, 0xF8, 0xC0, 0x03, 0x0F, 
	0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x00, 0x80, 0x01, 0x03, 0x00, 0xFC, 0xF8, 0x77, 0xF8, 0xC0, 
	0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x00, 0xC0, 0x80, 0x04, 0x00, 0xFC, 0xF8, 0x77, 
	0xF8, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x0C, 0x49, 0x02, 0x06, 0x00, 0xFC, 
	0xF8, 0x77, 0xF8, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x00, 0x30, 0xC3, 0x0C, 
	0x00, 0xFC, 0xF8, 0x77, 0xF8, 0xC0, 0x03, 0x0F, 0x3C, 0xF0, 0xE1, 0xFE, 0xF1, 0x03, 0x00, 0x00, 
	0x00, 0x03, 0x0C, 0x00, 0x00, 0xE0, 0x9F, 0x7F, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFA, 0x77, 0xF8, 0xF0, 0x63, 0xCF, 0xBC, 0xF1, 0xE3, 0xFE, 
	0xF5, 0x03, 0x00, 0x60, 0x00, 0x03, 0x00, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF8, 
	0xF1, 0x7E, 0x73, 0x0C, 0x00, 0x80, 0x01, 0x03, 0x00, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x03, 0x0F, 
	0x3C, 0xF8, 0xF1, 0x7E, 0x73, 0x0C, 0x00, 0xC0, 0x80, 0x04, 0x00, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 
	0x03, 0x0F, 0x3C, 0xF8, 0xF1, 0x7E, 0x73, 0x0C, 0x00, 0x30, 0xC3, 0x0C, 0x00, 0x03, 0x0F, 0x3C, 
	0xF0, 0xC0, 0x03, 0x0F, 0x3C, 0xF8, 0xF1, 0x7E, 0x73, 0x0C, 0x00, 0x80, 0x01, 0x03, 0x00, 0x03, 
	0x0F, 0x3C, 0xF0, 0xC0, 0xFE, 0xF3, 0x0F, 0x30, 0xE0, 0xFC, 0xF1, 0x03, 0x00, 0x00, 0x30, 0xC0, 
	0x00, 0x03, 0x0C, 0xF0, 0xC3, 0x1F, 0xC3, 0x0C, 0xF3, 0xC7, 0x0F, 0x03, 0x0C, 0x00, 0x00, 0x30, 
	0xC3, 0x0C, 0x00, 0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xFE, 0xF3, 0x0F, 0x30, 0xE0, 0xFC, 0xF1, 0x03
};

