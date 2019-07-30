#include "canvas.h"
#include <string.h>
#include <stdlib.h>

void Layout_Init(Layout* layout,Canvas* canvas,Layout* sourceLayout){

	uint8_t x=sourceLayout->x;
	uint8_t y=sourceLayout->y;
	uint8_t w=sourceLayout->width;
	uint8_t h=sourceLayout->height;


	//Произвольный контейнер
	if(sourceLayout->type==LAYOUT_CUSTOM){
		layout->x=x<canvas->width-1?x:0;
		layout->y=y<canvas->height-1?y:0;
		layout->width=(layout->x+w<=canvas->width)?w:(canvas->width-x);
		layout->height=(layout->y+h<=canvas->height)?h:(canvas->height-y);
	}
	//Заполнение холста
	else if(sourceLayout->type==LAYOUT_FILL){
		layout->x=0;
		layout->y=0;
		layout->width=canvas->width;
		layout->height=canvas->height;
	}
	//Центр холста
	else if(sourceLayout->type==LAYOUT_CENTER){
		if(w>canvas->width)
			w=canvas->width;
		layout->width=w;

		layout->width=w<=canvas->width?w:canvas->width;
		layout->height=h<=canvas->height?h:canvas->height;

		layout->x=(canvas->width-layout->width)/2;
		layout->y=(canvas->height-layout->height)/2;
	}
}

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

void Canvas_drawLineH(Canvas* canvas, uint8_t x, uint8_t y, uint8_t len) {
	for (uint8_t w = 0; w < canvas->pen.width; w++) {
		for (uint8_t i = x; i < x + len; i++) {
			if (canvas->pen.style == PS_SOLID) Canvas_drawPixel(canvas, i, y + w, canvas->pen.color);
			if (canvas->pen.style == PS_DOT) {
				if (i%2 == 0) Canvas_drawPixel(canvas, i, y + w, canvas->pen.color);
				else Canvas_drawPixel(canvas, i, y + w, !canvas->pen.color);
			}
		}
	}
}

void Canvas_drawLineV(Canvas* canvas, uint8_t x, uint8_t y, uint8_t len) {
	for (uint8_t w = 0; w < canvas->pen.width; w++) {
		for (uint8_t i = y; i < y + len; i++) {
			if (canvas->pen.style == PS_SOLID) Canvas_drawPixel(canvas, x + w, i, canvas->pen.color);
			if ((canvas->pen.style == PS_DOT)&(i%2 == 0)) Canvas_drawPixel(canvas, x + w, i, canvas->pen.color);
		}
	}
}

void Canvas_drawFrame(Canvas* canvas, Layout* layout) {

	uint8_t x1=layout->x;
	uint8_t y1=layout->y;
	uint8_t x2=layout->x+layout->width-1;
	uint8_t y2=layout->x+layout->height-1;
	uint8_t penWidth=canvas->pen.width;

	if ((x1 >= 0)&(x2 < canvas->width)&(y1 >= 0)&(y2 < canvas->height)&(x2 > x1)&(y2 > y1)) {
			// fill area

			if(canvas->brush.style!=BS_CLEAR)
			for (uint8_t x = x1+penWidth; x <= x2-penWidth; x++) {
				for (uint8_t y = y1+penWidth; y <= y2-penWidth; y++) {
					if(canvas->brush.style == BS_SOLID)
						Canvas_drawPixel(canvas, x, y, canvas->brush.color);
					else if(canvas->brush.style == BS_DOT_PTR1)
						if ((x + y) % 2 == 0) Canvas_drawPixel(canvas, x, y, canvas->pen.color);
						else Canvas_drawPixel(canvas, x, y, !canvas->pen.color);
					else if(canvas->brush.style == BS_DOT_PTR2)
						if (((x - x1) % 2 == 0) & ((y - y1) % 2 == 0)) Canvas_drawPixel(canvas, x, y, canvas->pen.color);
						else Canvas_drawPixel(canvas, x, y, !canvas->pen.color);
				}
			}


			// draw borders
			Canvas_drawLineH(canvas, x1, y1, x2 - x1);
			Canvas_drawLineH(canvas, x1, y2-penWidth, x2 - x1);
			Canvas_drawLineV(canvas, x1, y1, y2 - y1);
			Canvas_drawLineV(canvas, x2-penWidth, y1, y2 - y1);
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


