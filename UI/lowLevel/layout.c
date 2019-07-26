#include "layout.h"


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


