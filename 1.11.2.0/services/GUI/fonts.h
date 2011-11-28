#ifndef FONTS_H
#define FONTS_H


typedef struct
{
    const unsigned char *font_data;
    unsigned char	spacing;
    unsigned char	width;
    unsigned char	hight;
	unsigned char 	distance;
	unsigned char 	vspacing;
	unsigned char	char_bytes;
} RENDER_MODE;

extern const RENDER_MODE FNT10x21;
extern const RENDER_MODE FNT7x9;

extern const unsigned char fontd10x21_1[];

#endif


