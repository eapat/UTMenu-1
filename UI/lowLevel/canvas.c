#include "canvas.h"
#include <string.h>
#include <stdlib.h>


void Canvas_init(Canvas* canvas,uint8_t width, uint8_t height) {
	canvas->width = width;
	canvas->height = height;
	uint8_t rows = height / 8;
	if (height % 8 > 0) rows++;
	canvas->bitmap = malloc(rows*width);
	Canvas_clear(canvas);
}

void Canvas_destroy(Canvas* canvas) {
	if (canvas->bitmap != NULL) {
		free(canvas->bitmap);
		canvas->bitmap = NULL;
	}
}

void Canvas_clear(Canvas* canvas) {
	uint8_t rows = canvas->height / 8;
	if (canvas->height % 8 > 0) rows++;
	for (uint8_t j = 0; j < rows; j++) {
		for (uint8_t i = 0; i < canvas->width; i++) {
			canvas->bitmap[j*canvas->width + i] = 0;
		}
	}
}

void Canvas_drawPixel(Canvas* canvas, uint8_t x, uint8_t y, uint8_t color) {
	if ((x < canvas->width) & (y < canvas->height)) {
		if (color) canvas->bitmap[(y / 8)*canvas->width + x] |= 1 << y % 8;
		else canvas->bitmap[(y / 8)*canvas->width + x] &= ~(1 << y % 8);
	}
}



void Canvas_drawChar(Canvas* canvas, uint8_t* x, uint8_t y, char c,Font* font) {
	uint8_t width = Font_getWidth(font, c);
	uint8_t bs = font->height / 8;
	if (font->height % 8 > 0) bs++;
	uint8_t color = 0;
	for (uint8_t col = 0; col < width + 1 + font->spacing; col++) {
		color = font->inversion;
		if (font->transparent) {
			if (color) Canvas_drawPixel(canvas, *x + col, y, color);
		} else Canvas_drawPixel(canvas, *x + col, y, color);
	}
	for (uint8_t row = 0; row < font->height; row++) {
		color = font->inversion;
		if (font->transparent) {
			if (color) Canvas_drawPixel(canvas, *x, y + row + 1, color);
		} else Canvas_drawPixel(canvas, *x, y + row + 1, color);
	}
	uint8_t n = AnsiToIndex(c);
	for (uint8_t col = 0; col < width; col++) {
		for (uint8_t row = 0; row < font->height; row++) {
			if (row < 8) {
				if (font->type == FONT_5x8) color = ((Font_pattern5x8[n][col + 1] >> row) & 1) ^ font->inversion;
				if (font->type == FONT_8x12B) color = ((Font_pattern8x12[n][col * 2 + 1] >> row) & 1) ^ font->inversion;
				if (font->type == FONT_8x12S) color = ((Font_pattern8x12s[n][col * 2 + 1] >> row) & 1) ^ font->inversion;
				if (font->transparent) {
					if (color) Canvas_drawPixel(canvas, *x + col + 1, y + row + 1, color);
				} else Canvas_drawPixel(canvas, *x + col + 1, y + row + 1, color);
			} else {
				if (font->type == FONT_8x12B) color = ((Font_pattern8x12[n][col * 2 + 2] >> (row-8)) & 1) ^ font->inversion;
				if (font->type == FONT_8x12S) color = ((Font_pattern8x12s[n][col * 2 + 2] >> (row-8)) & 1) ^ font->inversion;
				if (font->transparent) {
					if (color) Canvas_drawPixel(canvas, *x + col + 1, y + row + 1, color);
				} else Canvas_drawPixel(canvas, *x + col + 1, y + row + 1, color);
			}
		}
	}
	for (uint8_t col = width; col < width + font->spacing; col++) {
		for (uint8_t row = 0; row < font->height; row++) {
			color = font->inversion;
			if (font->transparent) {
				if (color) Canvas_drawPixel(canvas, *x + col + 1, y + row + 1, color);
			} else Canvas_drawPixel(canvas, *x + col + 1, y + row + 1, color);
		}
	}
	*x += width + 1 + font->spacing;
}

void Canvas_drawString(Canvas* canvas, uint8_t x, uint8_t y, char *s,Font* font) {
	uint8_t i = x;
	while(*s != 0) {
		Canvas_drawChar(canvas, &i, y, *s,font);
		s++;
	}
}

