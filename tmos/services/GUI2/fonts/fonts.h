#ifndef FONTS_H
#define FONTS_H

#include <tmos.h>

/// By default use Fixed code page (cp1251)
#ifndef USE_FIXED_CPAGE
#error "Missing code page definitions!"
#endif

typedef struct
{
	const char *font_data;

    const char *icon_data;
	uint8_t	char_bytes;

	union
	{
		struct
		{
		    uint8_t	width;
		    uint8_t	height;
		    uint8_t	hspacing;
			uint8_t vspacing;
		};
		struct
		{
		    uint16_t char_size;
		    uint16_t spacing;
		};
		uint32_t size;
	};
	union
	{
		struct
		{
			uint8_t 	hdistance;
			uint8_t 	vdistance;
		};
		uint16_t distance;
	};
} RENDER_MODE;

extern const char icond5x7_1[];
extern const char icond7x9_1[];
extern const char icond10x21_1[];


extern const RENDER_MODE FNT10x21;
extern const RENDER_MODE FNT10x14;
extern const RENDER_MODE FNT10x12;
extern const RENDER_MODE FNT7x9;
extern const RENDER_MODE FNT5x7;

extern const char font5x7_cp1250[];
extern const char font7x9_cp1250[];
extern const char font10x12_cp1250[];
extern const char font10x14_cp1250[];
extern const char font10x21_cp1250[];

extern const char font5x7_cp1251[];
extern const char font7x9_cp1251[];
extern const char font10x12_cp1251[];
extern const char font10x14_cp1251[];
extern const char font10x21_cp1251[];

#endif


