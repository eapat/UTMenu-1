#ifndef __CANVAS_H
#define __CANVAS_H

#include "stm32f4xx_hal.h"
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

typedef struct {
	uint8_t  width;
	uint8_t  height;
	uint8_t* bitmap;
	Font*     font;
	uint8_t  update;
	char strBuff[20];
} Canvas;

void Canvas_init(Canvas* canvas,Font* font,uint8_t width, uint8_t height);
void Canvas_destroy(Canvas* canvas);
void Canvas_clear(Canvas* canvas);
void Canvas_drawPixel(Canvas* canvas, uint8_t x, uint8_t y, uint8_t color);
void Canvas_drawString(Canvas* canvas, uint8_t x, uint8_t y, char *s);


#endif
