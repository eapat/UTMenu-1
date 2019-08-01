#include "menu_window.h"
#include <stdbool.h>

int MenuWindow_calculateChildsCount(MenuItem* menuItem);

/*
 * Инициализация MenuWindow
 * Canvas* canvas
 * Font* font
 * Layout layout
 */
void MenuWindow_init(MenuWindow* menuWindow,Canvas* canvas,Layout layout,Font* titleFont, Font* bodyFont){
	menuWindow->canvas=canvas;
	menuWindow->layout=layout;
	menuWindow->bodyFont=titleFont;
	menuWindow->bodyFont=bodyFont;

}

/*
 * Отрисовка MenuWindow
 * uint32_t curTime-текущее время в мс
 */
void MenuWindow_draw(MenuWindow* menuWindow,uint32_t curTime){
	uint8_t scroll_width = 0;

	uint8_t view_rows = menuWindow->layout.height/ menuWindow->bodyFont->height;
	if (view_rows > menuWindow->childsCount) view_rows = menuWindow->childsCount;


	int i=0;

	MenuItem* currentChild=(MenuItem*)menuWindow->rootItem->child;

	for (int k=0; k<menuWindow->childsCount; k++ )
	{
		if(i>=menuWindow->pos && i<menuWindow->pos + view_rows)
			Canvas_drawString(menuWindow->canvas,menuWindow->layout.x, menuWindow->layout.y + (i - menuWindow->pos) * menuWindow->bodyFont->height,currentChild->text,menuWindow->bodyFont);

		currentChild=currentChild->next;
		i++;
	}

	uint8_t x1 = 0;
	uint8_t y1 = (menuWindow->select - menuWindow->pos) * menuWindow->bodyFont->height;
	Canvas_drawPixel(menuWindow->canvas, x1,y1,true);

}

/*
 * Установка корневого элемента
 */
void MenuWindow_setRootItem(MenuWindow* menuWindow, MenuItem* rootItem){
	menuWindow->rootItem=rootItem;
	menuWindow->childsCount=MenuWindow_calculateChildsCount(rootItem);
	Stack_push(&menuWindow->stack,rootItem);
	menuWindow->pos=0;
	menuWindow->select=0;
	menuWindow->viewHeight=64;
}


/*
 * Обработка проваливания вглубь
 */
void MenuWindow_enter(MenuWindow* menuWindow){
	MenuItem* currentChild=(MenuItem*)menuWindow->rootItem->child;
	for(int i=0;i<menuWindow->childsCount;i++)
	{
		if(i==menuWindow->select)
		{
			MenuWindow_setRootItem(menuWindow,currentChild);
			break;
		}
		currentChild=(MenuItem*)currentChild->next;
	}
}

/*
 * Обработка выхода
 */
void MenuWindow_back(MenuWindow* menuWindow){
	if(menuWindow->stack.pos>1){
		Stack_pop(&menuWindow->stack);
		MenuWindow_setRootItem(menuWindow,Stack_top(&menuWindow->stack));
		Stack_pop(&menuWindow->stack);
	}
}

/*
 * Обработка инкрементирования позиции
 */
void MenuWindow_incPosition(MenuWindow* menuWindow){
	if (menuWindow->select < menuWindow->childsCount - 1) {
		menuWindow->select++;
			uint8_t view_rows = menuWindow->layout.height/ menuWindow->bodyFont->height;
			if (menuWindow->select - menuWindow->pos == view_rows) {
				menuWindow->pos++;
			}
		}
}

/*
 * Обработка декрементирования позиции
 */
void MenuWindow_decPosition(MenuWindow* menuWindow){
	if (menuWindow->select > 0) {
		menuWindow->select--;
			uint8_t view_rows = menuWindow->layout.height/ menuWindow->bodyFont->height;
			if (menuWindow->select < menuWindow->pos) {
				menuWindow->pos--;
			}
		}
}

/*
 * Подсчёт количества детей
 * MenuItem* menuItem-целевой элемент меню
 */
int MenuWindow_calculateChildsCount(MenuItem* menuItem){
	int childsCount=0;
	MenuItem* currentChild=(MenuItem*)menuItem->child;

	if(currentChild==&Null_Menu)
		return 0;

	while(true){
		childsCount++;
		currentChild=(MenuItem*)currentChild->next;
		if(currentChild==&Null_Menu)
			break;
	}

	return childsCount;
}


