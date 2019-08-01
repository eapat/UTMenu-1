#ifndef __MENU_WINDOW_H
#define __MENU_WINDOW_H
#include "canvas.h"
#include "menu.h"
#include "stack.h"
#include "canvas.h"

//Структура MenuWindow
typedef struct {
	MenuItem* rootItem;//корневой элемент меню
	Canvas* canvas;//указатель на холст, в которм рисуем
	uint8_t childsCount;//количество потомков корневого элемента
	Font* titleFont;//шрифт заголовка
	Font* bodyFont;// шрифт содержимого
	Layout layout;//корневой контейнер

	uint8_t titleFontPadding;
	int viewHeight;
	Stack stack;

	uint8_t   pos;
	uint8_t   select;
}MenuWindow;

void MenuWindow_init(MenuWindow* menuWindow,Canvas* canvas,Layout layout,Font* titleFont, Font* bodyFont);
void MenuWindow_draw(MenuWindow* menuWindow,uint32_t curTime);
void MenuWindow_setRootItem(MenuWindow* menuWindow,MenuItem* rootItem);
void MenuWindow_incPosition(MenuWindow* menuWindow);
void MenuWindow_decPosition(MenuWindow* menuWindow);
void MenuWindow_enter(MenuWindow* menuWindow);
void MenuWindow_back(MenuWindow* menuWindow);

#endif
