#ifndef __EDIT_WINDOW_H
#define __EDIT_WINDOW_H

#include <stdbool.h>
#include "canvas.h"
#include "value.h"

typedef struct {
	uint32_t prevTime; //время предыдощего прохода
	uint8_t shiftFlag; //флаг необходимости сдвига
	uint8_t shift;	   //количество сдвигаемых символов
}EWShiftString;

typedef struct {
	Font* font;				//Шрифт в данной области
	Layout layout;			//Размеры и координаты области
	enum Frame_style style; //Стиль области
}EWArea;


typedef struct {
	Canvas* canvas;			//canvas с которым работаем
	Layout mainLayout;		//Размеры и координаты окна
	Layout frameLayout;		//Размеры и координаты черной рамки
	EWArea header;			//Заголовочная область
	EWArea body;			//Область со значением
	bool inProgress;		//Флаг работы
	Value* vlPt;			//Указатель на "оригинальное" значение
	Value vlCopy;			//Копия значения
	float vlLocal;			//Переменная для хранения копии
	char* headerText;		// Текст заголовка
	EWShiftString shStr;	// Парамеры сдвига бегущей строки
}EditWindow;

void EditWindow_init(EditWindow* this,Canvas* canvas,Layout layout, Font* headerFont, Font* bodyFont);
void EditWindow_start(EditWindow* this, Value* val, char* header);
void EditWindow_stop(EditWindow* this);
void EditWindow_draw(EditWindow* this, uint32_t currentTime);
void EditWindow_inc(EditWindow* this);
void EditWindow_dec(EditWindow* this);
void EditWindow_enter(EditWindow* this);
void EditWindow_back(EditWindow* this);
bool EditWindow_isRuning(EditWindow* this);

#endif
