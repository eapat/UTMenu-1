#ifndef __CANVAS_H
#define __CANVAS_H

#include <stdint.h>
#include <stdbool.h>
#include "fonts.h"

#define CL_BLACK 0
#define CL_WHITE 1
#define CL_TRANSPARENT 2

#define H_ALIGN_LEFT   0
#define H_ALIGN_RIGHT  1
#define H_ALIGN_CENTER 2

#define V_ALIGN_TOP    0
#define V_ALIGN_BOTTOM 1
#define V_ALIGN_CENTER 2

#define SIZE(x) sizeof(x)/sizeof(x[0])

#define PS_CLEAR 0
#define PS_SOLID 1
#define PS_DOT   2

#define BS_CLEAR    0
#define BS_SOLID    1
#define BS_DOT_PTR1 2
#define BS_DOT_PTR2 3

//Структура "кисти"
typedef struct {
	uint8_t style;
	uint8_t color;
} Brush;

//Структура "карандаша"
typedef struct {
	uint8_t width;
	uint8_t style;
	uint8_t color;
} Pen;

enum Layout_type{LAYOUT_CUSTOM,LAYOUT_FILL,LAYOUT_CENTER};//Тип заполнения контейнера
enum String_align{ALIGN_LEFT,ALIGN_CENTER,ALIGN_RIGHT};//Тип выравнивания строки в контейнере
enum Frame_style{FRAME_TRANSPARENT,FRAME_BLACK,FRAME_WHITE};//Стиль рамки

//Структура для хранения параметров контейнра
typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
}Layout;


//Структура для хранения параметров холста
typedef struct {
	uint8_t  width;
	uint8_t  height;
	uint8_t* bitmap;
	uint8_t  update;
	char strBuff[20];
	Pen      pen;
	Brush    brush;
} Canvas;

void Canvas_init(Canvas* canvas,uint8_t width, uint8_t height);
void Canvas_destroy(Canvas* canvas);
void Canvas_clear(Canvas* canvas);
void Canvas_setStyle(Canvas* canvas,enum Frame_style style);
void Canvas_calculateLayout(Canvas* canvas,Layout* layout,enum Layout_type type);
void Canvas_drawPixel(Canvas* canvas, uint8_t x, uint8_t y, uint8_t color);
void Canvas_drawString(Canvas* canvas, uint8_t x, uint8_t y, char *s,Font* font);
void Canvas_drawLineH(Canvas* canvas, uint8_t x, uint8_t y, uint8_t len);
void Canvas_drawLineV(Canvas* canvas, uint8_t x, uint8_t y, uint8_t len);
void Canvas_drawFrame(Canvas* canvas, Layout* layout,enum Frame_style style);
bool Canvas_drawAlignedString(Canvas* canvas,Layout* layout,char* s,Font* font,enum String_align align,uint8_t n);


#endif
