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
	Canvas_setStyle(canvas,FRAME_TRANSPARENT);
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

void Canvas_calculateLayout(Layout* layout,Canvas* canvas,enum Layout_type type){

	uint8_t x=layout->x;
	uint8_t y=layout->y;
	uint8_t w=layout->width;
	uint8_t h=layout->height;


	//Произвольный контейнер
	if(type==LAYOUT_CUSTOM){
		layout->x=x<canvas->width-1?x:0;
		layout->y=y<canvas->height-1?y:0;
		layout->width=(layout->x+w<=canvas->width)?w:(canvas->width-x);
		layout->height=(layout->y+h<=canvas->height)?h:(canvas->height-y);
	}
	//Заполнение холста
	else if(type==LAYOUT_FILL){
		layout->x=0;
		layout->y=0;
		layout->width=canvas->width;
		layout->height=canvas->height;
	}
	//Центр холста
	else if(type==LAYOUT_CENTER){
		if(w>canvas->width)
			w=canvas->width;
		layout->width=w;

		layout->width=w<=canvas->width?w:canvas->width;
		layout->height=h<=canvas->height?h:canvas->height;

		layout->x=(canvas->width-layout->width)/2;
		layout->y=(canvas->height-layout->height)/2;
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

void Canvas_setStyle(Canvas* canvas,enum Frame_style style){

	canvas->pen.color=1;
	canvas->pen.style=PS_SOLID;
	canvas->pen.width=1;

	if(style==FRAME_TRANSPARENT){
		canvas->brush.color=0;
		canvas->brush.style=BS_CLEAR;
	}
	else if(style==FRAME_BLACK){
		canvas->brush.color=0;
		canvas->brush.style=BS_SOLID;
	}
	else if (style==FRAME_WHITE){
		canvas->brush.color=1;
		canvas->brush.style=BS_SOLID;
	}
}


void Canvas_drawFrame(Canvas* canvas, Layout* layout,enum Frame_style style) {

	Canvas_setStyle(canvas,style);
	uint8_t x1=layout->x;
	uint8_t y1=layout->y;
	uint8_t x2=layout->x+layout->width-1;
	uint8_t y2=layout->y+layout->height-1;
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
			Canvas_drawLineH(canvas, x1, y1, x2 - x1+1);
			Canvas_drawLineH(canvas, x1, y2-penWidth+1, x2 - x1+1);
			Canvas_drawLineV(canvas, x1, y1, y2 - y1+1);
			Canvas_drawLineV(canvas, x2-penWidth+1, y1, y2 - y1+1);
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

/*
 * Отрисовка строки внутри layout
 * align-выравнивание
 * n-номер символа с которого выводить
 * возвращает true,если строка влезла в layout
 */
bool Canvas_drawAlignedString(Canvas* canvas,Layout* layout,Font* font,char* s,enum Layout_align align,uint8_t n){

	uint8_t strLenght=strlen(s);
	if(layout->height<font->height || n>strLenght)
	{
		Canvas_drawFrame(canvas,layout,FRAME_WHITE);
		if(n>strLenght)
			return true;
		return false;
	}

	//Количество символов и их длина, которую можем вывести
	uint8_t sNum=0;
	uint8_t sWidth=0;
	uint8_t tempWidth=0;


	if(canvas->s!=s)
		canvas->s=s;

	//Цикл по всем сиволам строки c n
	for(int i=n;i<(strLenght);i++)
	{
		tempWidth+=Font_getWidth(font, *(s+i))+1+font->spacing;
		if(tempWidth<layout->width)
		{
			sNum++;
			sWidth=tempWidth;
		}
		else
			break;
	}

	uint8_t x=0;



	//Выводим исходя из выравнивания
	if(align==ALIGN_LEFT)
		 x = layout->x;
	else if(align==ALIGN_CENTER)
		 x=layout->x+(layout->width-sWidth)/2;
	else if(align==ALIGN_RIGHT)
		 x=layout->x+layout->width-1-sWidth;

	uint8_t y=layout->y+(layout->height-font->height)/2;

	for(int i=n;i<n+sNum;i++)
	{
		Canvas_drawChar(canvas, &x, y, *(s+i),font);
	}

	if((n+sNum)==strLenght)
		return true;
	else
		return false;

	return false;

}


