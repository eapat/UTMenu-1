#ifndef __LAYOUT_H
#define __LAYOUT_H
#include <stdint.h>
#include "canvas.h"



enum Layout_type{LAYOUT_CUSTOM,LAYOUT_FILL,LAYOUT_CENTER};

//Структура для хранения параметров контейнра
typedef struct {
	enum Layout_type type;
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
}Layout;

void Layout_Init(Layout* layout,Canvas* canvas,Layout* sourceLayout);


#endif
