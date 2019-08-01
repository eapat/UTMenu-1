#ifndef __MENU_WINDOW_H
#define __MENU_WINDOW_H
#include "canvas.h"
#include "menu.h"
#include "stack.h"
#include "canvas.h"

#define TITLE_FONT_PADDING 1
#define BODY_PADDING 1
#define ITEM_FONT_PADDING 2
#define ITEM_TEXT_SPACE 0.7

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
}MenuWindow;

void MenuWindow_init(MenuWindow* menuWindow,Canvas* canvas,Layout layout,Font* titleFont, Font* bodyFont);
void MenuWindow_draw(MenuWindow* menuWindow,uint32_t curTime);
bool MenuWindow_setRootItem(MenuWindow* menuWindow, MenuItem* rootItem);
void MenuWindow_incPosition(MenuWindow* menuWindow);
void MenuWindow_decPosition(MenuWindow* menuWindow);
MenuItem* MenuWindow_enter(MenuWindow* menuWindow);
void MenuWindow_back(MenuWindow* menuWindow);

#endif
