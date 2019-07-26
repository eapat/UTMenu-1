#ifndef __FONTS_H
#define __FONTS_H

#include <stdint.h>

typedef struct {
	uint8_t type;
	uint8_t width;
	uint8_t height;
	uint8_t spacing;
	uint8_t transparent;
	uint8_t inversion;
} Font;

#define FONT_5x8   0
#define FONT_8x12B 1
#define FONT_8x12S 2

#define BG_TRANS 1
#define BG_FILL  0

#define FS_NORMAL 0
#define FS_INVERT 1

#define F_SPACING_0 0
#define F_SPACING_1 1
#define F_SPACING_2 2

extern const uint8_t Font_pattern5x8[][6];
extern const uint8_t Font_pattern8x12[][17];
extern const uint8_t Font_pattern8x12s[][17];

uint8_t AnsiToIndex(uint8_t ansi_code);

void Font_init(Font* font,uint8_t font_type, uint8_t spacing, uint8_t inversion, uint8_t transparent);
uint8_t Font_getWidth(Font* font, uint8_t ansi_code);

#endif
