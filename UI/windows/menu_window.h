#ifndef __MENU_WINDOW_H
#define __MENU_WINDOW_H
#include "canvas.h"
#include "menu.h"
#include "stack.h"
#include "canvas.h"





typedef struct {
	uint32_t prevTime; //время предыдощего прохода
	uint8_t shiftFlag; //флаг необходимости сдвига
	uint8_t shift;	   //количество сдвигаемых символов
}MWShiftString;

enum WindowState{CLOSED,OPEN,PAUSED};

//Структура MenuWindow
typedef struct {
	MenuItem* rootItem;//корневой элемент меню
	Canvas* canvas;//указатель на холст, в которм рисуем
	uint8_t childsCount;//количество потомков корневого элемента
	Font* titleFont;//шрифт заголовка
	Font* bodyFont;// шрифт содержимого
	Layout layout;//корневой контейнер
	Stack stack;//стэк вызовов

	uint8_t   pos;//откуда отрисовываем
	uint8_t   select;//выдленный элемент

	uint8_t titleHeight;//высота заголовка (с прозрачным промежутком снизу BODY_PADDING)
	uint8_t itemHeight;//высота одного элемента
	uint8_t viewRows;//количество строк, которые можем вывести
	MWShiftString shStr;	// Парамеры сдвига бегущей строки
	uint32_t lifeTime;//Время жизни окна в миллисекунда
	enum WindowState windowState;
}MenuWindow;

void MenuWindow_init(MenuWindow* menuWindow,Canvas* canvas,Layout layout,Font* titleFont, Font* bodyFont);
void MenuWindow_start(MenuWindow* menuWindow);
void MenuWindow_stop(MenuWindow* menuWindow);
uint32_t MenuWindow_getLifeTime(MenuWindow* menuWindow);
bool MenuWindow_isRunning(MenuWindow* menuWindow);

void MenuWindow_draw(MenuWindow* menuWindow,uint32_t curTime);
bool MenuWindow_setRootItem(MenuWindow* menuWindow, MenuItem* rootItem);
void MenuWindow_incPosition(MenuWindow* menuWindow);
void MenuWindow_decPosition(MenuWindow* menuWindow);
MenuItem* MenuWindow_enter(MenuWindow* menuWindow);
void MenuWindow_back(MenuWindow* menuWindow);
void MenuWindow_pause(MenuWindow* menuWindow);

#endif
