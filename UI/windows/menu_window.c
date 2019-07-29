#include "menu_window.h"
#include <stdbool.h>

int calculateChildsCount(MenuItem* menuItem){
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


void MenuWindow_init(MenuWindow* menuWindow,Canvas* canvas,Font* font,Layout layout){
	menuWindow->canvas=canvas;
	menuWindow->font=font;
	Layout_Init(&menuWindow->layout,menuWindow->canvas,&layout);
}

void MenuWindow_draw(MenuWindow* menuWindow){
	uint8_t scroll_width = 0;

	uint8_t view_rows = menuWindow->layout.height/ menuWindow->font->height;
	if (view_rows > menuWindow->childsCount) view_rows = menuWindow->childsCount;


	int i=0;

	MenuItem* currentChild=(MenuItem*)menuWindow->parentItem->child;

	for (int k=0; k<menuWindow->childsCount; k++ )
	{
		if(i>=menuWindow->pos && i<menuWindow->pos + view_rows)
			Canvas_drawString(menuWindow->canvas,menuWindow->layout.x, menuWindow->layout.y + (i - menuWindow->pos) * menuWindow->font->height,currentChild->text,menuWindow->font);

		currentChild=currentChild->next;
		i++;
	}

	uint8_t x1 = 0;
	uint8_t y1 = (menuWindow->select - menuWindow->pos) * menuWindow->font->height;
	Canvas_drawPixel(menuWindow->canvas, x1,y1,true);

}

void MenuWindow_setMenuList(MenuWindow* menuWindow, MenuItem* parentItem){
	menuWindow->parentItem=parentItem;
	menuWindow->childsCount=calculateChildsCount(parentItem);
	Stack_push(&menuWindow->stack,parentItem);
	menuWindow->pos=0;
	menuWindow->select=0;
	menuWindow->viewHeight=64;
}



void MenuWindow_enter(MenuWindow* menuWindow){
	MenuItem* currentChild=(MenuItem*)menuWindow->parentItem->child;
	for(int i=0;i<menuWindow->childsCount;i++)
	{
		if(i==menuWindow->select)
		{
			MenuWindow_setMenuList(menuWindow,currentChild);
			break;
		}
		currentChild=(MenuItem*)currentChild->next;
	}
}

void MenuWindow_back(MenuWindow* menuWindow){
	if(menuWindow->stack.pos>1){
		Stack_pop(&menuWindow->stack);
		MenuWindow_setMenuList(menuWindow,Stack_top(&menuWindow->stack));
		Stack_pop(&menuWindow->stack);
	}
}

void MenuWindow_incPosition(MenuWindow* menuWindow){
	if (menuWindow->select < menuWindow->childsCount - 1) {
		menuWindow->select++;
			uint8_t view_rows = menuWindow->layout.height/ menuWindow->font->height;
			if (menuWindow->select - menuWindow->pos == view_rows) {
				menuWindow->pos++;
			}
		}
}

void MenuWindow_decPosition(MenuWindow* menuWindow){
	if (menuWindow->select > 0) {
		menuWindow->select--;
			uint8_t view_rows = menuWindow->layout.height/ menuWindow->font->height;
			if (menuWindow->select < menuWindow->pos) {
				menuWindow->pos--;
			}
		}
}

