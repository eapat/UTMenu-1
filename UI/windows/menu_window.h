#ifndef __MENU_WINDOW_H
#define __MENU_WINDOW_H
#include "canvas.h"
#include "menu.h"
#include "stack.h"
#include "canvas.h"


typedef struct {
	MenuItem* parentItem;
	Canvas* canvas;
	uint8_t childsCount;
	Font* font;
	Layout layout;

	uint8_t titleFontPadding;
	int viewHeight;
	Stack stack;

	uint8_t   pos;
	uint8_t   select;
}MenuWindow;

void MenuWindow_init(MenuWindow* menuWindow,Canvas* canvas,Font* font,Layout layout);
void MenuWindow_draw(MenuWindow* menuWindow);
void MenuWindow_setMenuList(MenuWindow* menuWindow,MenuItem* parentItem);
void MenuWindow_incPosition(MenuWindow* menuWindow);
void MenuWindow_decPosition(MenuWindow* menuWindow);
void MenuWindow_enter(MenuWindow* menuWindow);
void MenuWindow_back(MenuWindow* menuWindow);

#endif
