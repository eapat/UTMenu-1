#include "menu_window.h"
#include <stdbool.h>

uint8_t MenuWindow_calculateChildsCount(MenuItem* menuItem);

/*
 * Инициализация MenuWindow
 * Canvas* canvas
 * Font* font
 * Layout layout
 */
void MenuWindow_init(MenuWindow* menuWindow,Canvas* canvas,Layout layout,Font* titleFont, Font* bodyFont){
	menuWindow->canvas=canvas;
	menuWindow->layout=layout;
	menuWindow->titleFont=titleFont;
	menuWindow->bodyFont=bodyFont;

	menuWindow->titleHeight=menuWindow->titleFont->height+TITLE_FONT_PADDING*2+BODY_PADDING;
	menuWindow->itemHeight=menuWindow->bodyFont->height+2*ITEM_FONT_PADDING;
	menuWindow->viewRows = (menuWindow->layout.height-menuWindow->titleHeight)/ menuWindow->itemHeight;


}

/*
 * Отрисовка MenuWindow
 * uint32_t curTime-текущее время в мс
 */
void MenuWindow_draw(MenuWindow* mW,uint32_t curTime){
	uint8_t scroll_width = 0;

	//Отрисовываем заголовок

	Layout layout={mW->layout.x,mW->layout.y,mW->layout.width,mW->titleHeight-BODY_PADDING};

	Canvas_drawFrame(mW->canvas,&layout,FRAME_WHITE);
	Canvas_drawAlignedString(mW->canvas,&layout,mW->rootItem->text, mW->titleFont,ALIGN_CENTER,0);

	uint8_t view_rows=mW->viewRows;
	if (view_rows > mW->childsCount)
		view_rows = mW->childsCount;

	int i=0;

	MenuItem* currentChild=(MenuItem*)mW->rootItem->child;

	for (int k=0; k<mW->childsCount; k++ )
	{
		if(i>=mW->pos && i<mW->pos +view_rows){
			Layout layout={mW->layout.x,mW->layout.y+(i - mW->pos) * mW->itemHeight+mW->titleHeight,mW->layout.width*ITEM_TEXT_SPACE,mW->itemHeight};
			Canvas_drawAlignedString(mW->canvas,&layout,currentChild->text,mW->bodyFont,ALIGN_LEFT,0);
			if(i==mW->select){
				layout.width=mW->layout.width;
				Canvas_drawFrame(mW->canvas,&layout,FRAME_TRANSPARENT);
			}
		}
		currentChild=currentChild->next;
		i++;
	}

	//uint8_t x1 = 0;
	//uint8_t y1 = (mW->select - mW->pos) * mW->bodyFont->height;
	//Canvas_drawPixel(mW->canvas, x1,y1,true);

}

/*
 * Установка корневого элемента
 * return false-если элемента нет потомков
 */
bool MenuWindow_setRootItem(MenuWindow* menuWindow, MenuItem* rootItem){

	uint8_t childCount=MenuWindow_calculateChildsCount(rootItem);

	if(childCount==0)
		return false;

	menuWindow->rootItem=rootItem;
	menuWindow->childsCount=childCount;
	Stack_push(&menuWindow->stack,rootItem);
	menuWindow->pos=0;
	menuWindow->select=0;

	return true;
}


/*
 * Обработка проваливания вглубь
 * возвращает &Null_Menu если смог провалиться
 */
MenuItem* MenuWindow_enter(MenuWindow* menuWindow){
	MenuItem* currentChild=(MenuItem*)menuWindow->rootItem->child;
	for(int i=0;i<menuWindow->childsCount;i++)
	{
		if(i==menuWindow->select)
		{
			if(MenuWindow_setRootItem(menuWindow,currentChild))
				return &Null_Menu;
			else
				return currentChild;
		}
		currentChild=(MenuItem*)currentChild->next;
	}

	return &Null_Menu;
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
			if (menuWindow->select - menuWindow->pos == menuWindow->viewRows) {
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
			if (menuWindow->select < menuWindow->pos) {
				menuWindow->pos--;
			}
		}
}

/*
 * Подсчёт количества детей
 * MenuItem* menuItem-целевой элемент меню
 */
uint8_t MenuWindow_calculateChildsCount(MenuItem* menuItem){
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


