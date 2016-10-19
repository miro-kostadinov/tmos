#include <tmos.h>
#include <fonts.h>


const char font5x7_cp1250[1120] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x18, 0x03, 0x80, 0x01, 0x4A, 0x01, 0x00, 0x00, 0x00, 0x40, 
	0x7D, 0xF5, 0x15, 0x00, 0xC4, 0x17, 0x47, 0x1F, 0x01, 0x63, 0x60, 0x32, 0x30, 0x06, 0x26, 0x15, 
	0x51, 0x93, 0x05, 0x86, 0x00, 0x00, 0x00, 0x00, 0x88, 0x08, 0x21, 0x08, 0x02, 0x82, 0x20, 0x84, 
	0x88, 0x00, 0x80, 0x54, 0x57, 0x09, 0x00, 0x80, 0x90, 0x4F, 0x08, 0x00, 0x00, 0x00, 0x63, 0x88, 
	0x00, 0x00, 0x80, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0x01, 0x10, 0x21, 0x22, 0x44, 0x00, 
	0x2E, 0xE6, 0x3A, 0xA3, 0x03, 0xC4, 0x14, 0x42, 0xC8, 0x07, 0x2E, 0x42, 0x17, 0xC2, 0x07, 0x2E, 
	0x42, 0x06, 0xA3, 0x03, 0x88, 0xA9, 0xF4, 0x11, 0x02, 0x3F, 0x3C, 0x08, 0xA3, 0x03, 0x2E, 0x84, 
	0x17, 0xA3, 0x03, 0x1F, 0x22, 0x22, 0x84, 0x00, 0x2E, 0x46, 0x17, 0xA3, 0x03, 0x2E, 0x46, 0x0F, 
	0xA3, 0x03, 0x00, 0x18, 0x03, 0x8C, 0x01, 0x00, 0x18, 0x60, 0x88, 0x00, 0x88, 0x88, 0x20, 0x08, 
	0x02, 0x00, 0x7C, 0x00, 0x3E, 0x00, 0x82, 0x20, 0x88, 0x88, 0x00, 0x2E, 0x42, 0x44, 0x00, 0x01, 
	0x2E, 0x42, 0x5B, 0xAB, 0x03, 0x2E, 0xC6, 0xF8, 0x63, 0x04, 0x2F, 0xC6, 0x17, 0xE3, 0x03, 0x2E, 
	0x86, 0x10, 0xA2, 0x03, 0x2F, 0xC6, 0x18, 0xE3, 0x03, 0x3F, 0x84, 0x17, 0xC2, 0x07, 0x3F, 0x84, 
	0x17, 0x42, 0x00, 0x2E, 0x86, 0x1C, 0xA3, 0x03, 0x31, 0xC6, 0x1F, 0x63, 0x04, 0x8E, 0x10, 0x42, 
	0x88, 0x03, 0x1C, 0x21, 0x84, 0x92, 0x01, 0x31, 0x95, 0x51, 0x52, 0x04, 0x21, 0x84, 0x10, 0xC2, 
	0x07, 0x71, 0xFF, 0x1A, 0x63, 0x04, 0x71, 0xDE, 0x9E, 0x63, 0x04, 0x2E, 0xC6, 0x18, 0xA3, 0x03, 
	0x2F, 0xC6, 0x17, 0x42, 0x00, 0x2E, 0xC6, 0x58, 0x93, 0x05, 0x2F, 0xC6, 0x97, 0x62, 0x04, 0x2E, 
	0x06, 0x07, 0xA3, 0x03, 0x9F, 0x10, 0x42, 0x08, 0x01, 0x31, 0xC6, 0x18, 0xA3, 0x03, 0x31, 0x6E, 
	0xE5, 0x08, 0x01, 0x31, 0xD6, 0xFA, 0x95, 0x02, 0x31, 0x2A, 0xA2, 0x62, 0x04, 0x31, 0x46, 0x47, 
	0x08, 0x01, 0x1F, 0x22, 0x22, 0xC2, 0x07, 0x4E, 0x08, 0x21, 0x84, 0x03, 0x41, 0x08, 0x82, 0x10, 
	0x04, 0x0E, 0x21, 0x84, 0x90, 0x03, 0x44, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x07, 
	0x86, 0x10, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x1C, 0xB3, 0x05, 0x21, 0xB4, 0x19, 0x67, 0x03, 0x00, 
	0xB8, 0x18, 0xA2, 0x03, 0x10, 0xDA, 0x1C, 0xB3, 0x05, 0x00, 0xB8, 0xF8, 0x83, 0x07, 0x4C, 0x8A, 
	0x27, 0x84, 0x00, 0x00, 0xF8, 0xE8, 0xA1, 0x03, 0x21, 0xB4, 0x19, 0x63, 0x04, 0x80, 0x00, 0x43, 
	0x88, 0x03, 0x08, 0x30, 0x84, 0x14, 0x01, 0x21, 0xA4, 0x32, 0x4A, 0x02, 0x42, 0x08, 0x21, 0x04, 
	0x03, 0x00, 0xAC, 0x5A, 0x6B, 0x04, 0x00, 0xB4, 0x19, 0x63, 0x04, 0x00, 0xB8, 0x18, 0xA3, 0x03, 
	0x00, 0xBC, 0xF8, 0x42, 0x00, 0x00, 0xF8, 0xE8, 0x21, 0x04, 0x00, 0xB4, 0x11, 0x42, 0x00, 0x00, 
	0xF8, 0xE0, 0xE0, 0x03, 0x40, 0x88, 0x23, 0x14, 0x01, 0x00, 0xC4, 0x18, 0xB3, 0x05, 0x00, 0xC4, 
	0xA8, 0x14, 0x01, 0x00, 0xC4, 0x5A, 0x95, 0x02, 0x00, 0x44, 0x45, 0x54, 0x04, 0x00, 0xC4, 0xE8, 
	0xA1, 0x03, 0x00, 0x7C, 0x44, 0xC4, 0x07, 0x8C, 0x10, 0x43, 0x08, 0x03, 0x84, 0x10, 0x42, 0x08, 
	0x01, 0x86, 0x10, 0x46, 0x88, 0x01, 0x40, 0xD6, 0x04, 0x00, 0x00, 0x4E, 0x29, 0xA5, 0x94, 0x03, 
	0x5C, 0x3C, 0xF1, 0x04, 0x07, 0xE8, 0x87, 0x10, 0x42, 0x00, 0x00, 0x00, 0x20, 0x44, 0x00, 0x00, 
	0x90, 0x17, 0x42, 0x00, 0x00, 0x00, 0x20, 0x65, 0x02, 0x00, 0x00, 0x00, 0x40, 0x05, 0x84, 0x7C, 
	0x42, 0x08, 0x01, 0xE4, 0x13, 0x42, 0x3E, 0x01, 0x4C, 0x1E, 0x71, 0x24, 0x03, 0x11, 0x22, 0x22, 
	0x42, 0x05, 0xC9, 0xF8, 0xE0, 0xE0, 0x03, 0x00, 0x11, 0x41, 0x10, 0x00, 0x88, 0xF8, 0xE0, 0xE0, 
	0x03, 0x8A, 0x7C, 0x42, 0x08, 0x01, 0xC9, 0x7C, 0x44, 0xC4, 0x07, 0x88, 0x7C, 0x44, 0xC4, 0x07, 
	0xE2, 0x08, 0x65, 0xA5, 0x02, 0xC4, 0x18, 0x00, 0x00, 0x00, 0x8C, 0x21, 0x00, 0x00, 0x00, 0x49, 
	0x02, 0x00, 0x00, 0x00, 0x32, 0x01, 0x00, 0x00, 0x00, 0xC0, 0xBC, 0x67, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9F, 0x10, 0x10, 
	0x77, 0x05, 0x8A, 0xF8, 0xE0, 0xE0, 0x03, 0x40, 0x10, 0x44, 0x04, 0x00, 0x98, 0xF8, 0xE0, 0xE0, 
	0x03, 0x50, 0x89, 0x23, 0x14, 0x01, 0x8A, 0x7C, 0x44, 0xC4, 0x07, 0x98, 0x7C, 0x44, 0xC4, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x8A, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x99, 0x21, 0x84, 0x07, 0x20, 0x3A, 0xE5, 0x22, 0x00, 0x2E, 0xFE, 0x18, 0x11, 0x06, 0x84, 0x10, 
	0x40, 0x08, 0x01, 0x2E, 0x1A, 0xC5, 0xA2, 0x03, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8, 0x3A, 
	0xAB, 0x03, 0x3E, 0x38, 0xF8, 0x88, 0x01, 0x80, 0xAA, 0xA2, 0x28, 0x00, 0x00, 0x80, 0x87, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xB8, 0x7B, 0xB7, 0x03, 0x04, 0x7C, 0x44, 0xC4, 0x07, 
	0x44, 0x11, 0x00, 0x00, 0x00, 0x80, 0x38, 0x02, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x08, 0x03, 0x86, 
	0x50, 0x66, 0x08, 0x06, 0x8C, 0x11, 0x00, 0x00, 0x00, 0x00, 0x2C, 0xA5, 0x9C, 0x04, 0xFE, 0x5A, 
	0x4A, 0x29, 0x05, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x01, 0x0E, 0xFA, 0xE8, 
	0x11, 0x06, 0x3E, 0x38, 0xF8, 0x88, 0x01, 0xA0, 0x28, 0xAA, 0x0A, 0x00, 0xA5, 0x84, 0x10, 0xC2, 
	0x07, 0x32, 0x01, 0x00, 0x00, 0x00, 0x96, 0x12, 0x42, 0x88, 0x03, 0x80, 0x80, 0x47, 0xC4, 0x03, 
	0x44, 0x9C, 0x74, 0x52, 0x04, 0x88, 0x00, 0x17, 0x7F, 0x04, 0x44, 0x01, 0x17, 0x7F, 0x04, 0x8A, 
	0x00, 0x17, 0x7F, 0x04, 0x40, 0x01, 0x17, 0x7F, 0x04, 0xA9, 0x84, 0x10, 0xC2, 0x07, 0x88, 0xB8, 
	0x18, 0xA2, 0x03, 0x2E, 0x86, 0xE8, 0x88, 0x00, 0x8A, 0xB8, 0x18, 0xA2, 0x03, 0x88, 0xFC, 0xF0, 
	0xC2, 0x07, 0x3F, 0xBC, 0xF0, 0x11, 0x06, 0x0A, 0xFC, 0xF0, 0xC2, 0x07, 0x8A, 0xFC, 0xF0, 0xC2, 
	0x07, 0x88, 0x38, 0x42, 0x88, 0x03, 0x44, 0x39, 0x42, 0x88, 0x03, 0x8A, 0xBC, 0x18, 0xE3, 0x03, 
	0x4E, 0xCA, 0x2B, 0xA5, 0x03, 0x88, 0xC4, 0x59, 0x73, 0x04, 0x8A, 0xC4, 0x59, 0x73, 0x04, 0x88, 
	0xB8, 0x18, 0xA3, 0x03, 0x44, 0xB9, 0x18, 0xA3, 0x03, 0x32, 0xB9, 0x18, 0xA3, 0x03, 0x0A, 0xB8, 
	0x18, 0xA3, 0x03, 0x00, 0x28, 0xA2, 0x00, 0x00, 0x8A, 0xBC, 0xF8, 0x52, 0x04, 0x44, 0xD5, 0x18, 
	0xA3, 0x03, 0x88, 0xC4, 0x18, 0xA3, 0x03, 0x32, 0xC5, 0x18, 0xA3, 0x03, 0x0A, 0xC4, 0x18, 0xA3, 
	0x03, 0x88, 0x44, 0x45, 0x08, 0x01, 0x9F, 0x10, 0x42, 0x80, 0x01, 0x26, 0xA5, 0x16, 0x63, 0x03, 
	0x44, 0xB4, 0x11, 0x42, 0x00, 0x88, 0x00, 0x1F, 0xB3, 0x05, 0x44, 0x01, 0x1F, 0xB3, 0x05, 0x8A, 
	0x00, 0x1F, 0xB3, 0x05, 0x40, 0x01, 0x1F, 0xB3, 0x05, 0x88, 0x00, 0x42, 0x08, 0x03, 0x88, 0x00, 
	0x17, 0x82, 0x03, 0x2E, 0x04, 0x47, 0x0C, 0x00, 0x8A, 0x00, 0x17, 0x82, 0x03, 0x88, 0xB8, 0xF8, 
	0x83, 0x07, 0x2E, 0xFE, 0xE0, 0x09, 0x03, 0x0A, 0xB8, 0xF8, 0x83, 0x07, 0x8A, 0xB8, 0xF8, 0x83, 
	0x07, 0x88, 0x00, 0x43, 0x88, 0x03, 0x44, 0x01, 0x43, 0x88, 0x03, 0x10, 0x23, 0x97, 0x92, 0x03, 
	0x88, 0x23, 0x97, 0x92, 0x03, 0x88, 0xB4, 0x19, 0x63, 0x04, 0x8A, 0xB4, 0x19, 0x63, 0x04, 0x88, 
	0x00, 0x17, 0xA3, 0x03, 0x44, 0x01, 0x17, 0xA3, 0x03, 0x54, 0x01, 0x17, 0xA3, 0x03, 0x40, 0x01, 
	0x17, 0xA3, 0x03, 0x80, 0x80, 0x0F, 0x08, 0x00, 0x8A, 0x80, 0x36, 0x42, 0x00, 0x44, 0x91, 0x18, 
	0xB3, 0x05, 0x88, 0x80, 0x18, 0xB3, 0x05, 0x54, 0x81, 0x18, 0xB3, 0x05, 0x40, 0x81, 0x18, 0xB3, 
	0x05, 0x88, 0xC4, 0xE8, 0xA1, 0x03, 0xE2, 0x08, 0xE5, 0x88, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00
};

