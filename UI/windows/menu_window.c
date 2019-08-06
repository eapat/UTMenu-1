#include "menu_window.h"
#include <stdbool.h>
#include "time_utilities.h"
#include "value.h"
#include <stdio.h>

#define MW_SHIFT_TIME 300//Период сдвига текса
#define MW_SHIFT_PAUSE 1000//Время паузы

#define TITLE_FONT_PADDING 1//Отступы по вертикали у текста заголовка
#define BODY_PADDING 1//Отступ списка от заголовка
#define ITEM_FONT_PADDING 2//Отступы по вертикали у текста элемента меню

#define ITEM_TEXT_RATIO 0.5//Соотношение ширины текста элемента ко всей строке
#define ITEM_SPACE_PX 5//Отступ от текста до значения в пикселях
#define SCROLLBAR_WIDTH 3//Толщина scrollbar
#define SCROLLBAR_PADDING 1//Отступ scrollbar от значения

uint8_t MenuWindow_calculateChildsCount(MenuItem* menuItem);
void MenuWindow_resetShift(MenuWindow* menuWindow);

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
	menuWindow->isRunning=false;
}

/*
 * Старт окна
 */
void MenuWindow_start(MenuWindow* menuWindow){
	menuWindow->isRunning=true;
	menuWindow->lifeTime=0;
}

/*
 * Останов окна
 */
void MenuWindow_stop(MenuWindow* menuWindow){
	menuWindow->isRunning=false;
	menuWindow->lifeTime=0;
}

/*
 * Вернуть время жизни окна
 */
uint32_t MenuWindow_getLifeTime(MenuWindow* menuWindow){
	return menuWindow->lifeTime;
}

/*
 * true-если окно запущено
 */
bool MenuWindow_isRunning(MenuWindow* menuWindow){
	return menuWindow->isRunning;
}

/*
 * Отрисовка MenuWindow
 * uint32_t curTime-текущее время в мс
 */
void MenuWindow_draw(MenuWindow* mW,uint32_t curTime){

	if(!mW->isRunning)
		return;

	bool showScroll=true;
	//Отрисовываем заголовок

	Layout layout={mW->layout.x,mW->layout.y,mW->layout.width,mW->titleHeight-BODY_PADDING};

	Canvas_drawFrame(mW->canvas,&layout,FRAME_WHITE);
	Canvas_drawAlignedString(mW->canvas,&layout,mW->rootItem->text, mW->titleFont,ALIGN_CENTER,0);

	uint8_t view_rows=mW->viewRows;
	if (view_rows >=mW->childsCount){
		view_rows = mW->childsCount;
		showScroll=false;
	}

	int i=0;

	MenuItem* currentChild=(MenuItem*)mW->rootItem->child;

	for (int k=0; k<mW->childsCount; k++ )
	{
		//Отрисовываем видимые элементы
		if(i>=mW->pos && i<mW->pos +view_rows){
			//Большой контейнер для текста элемента
			Layout layout={mW->layout.x, (int)mW->layout.y+(i - mW->pos) * mW->itemHeight+mW->titleHeight, (int)mW->layout.width*ITEM_TEXT_RATIO,mW->itemHeight};

			//Если этот элемент выделен, то надо его шевелить и отрисовать рамку вокруг него
			if(i==mW->select){
				int delay = (mW->shStr.shiftFlag||mW->shStr.shift==0)? MW_SHIFT_PAUSE : MW_SHIFT_TIME;
				uint32_t delta=TimeUtilities_getDelta32(curTime,mW->shStr.prevTime);
				mW->lifeTime+=delta;
				if ( delta> delay){
					mW->shStr.prevTime=curTime;
					mW->shStr.shift=!mW->shStr.shiftFlag?mW->shStr.shift+1:0;
				}
				mW->shStr.shiftFlag=Canvas_drawAlignedString(mW->canvas,&layout,currentChild->text,mW->bodyFont,ALIGN_LEFT,mW->shStr.shift);
				if(showScroll)
					layout.width=mW->layout.width-SCROLLBAR_WIDTH-SCROLLBAR_PADDING;
				else
					layout.width=mW->layout.width;
				Canvas_drawFrame(mW->canvas,&layout,FRAME_TRANSPARENT);
			}
			else{
				Canvas_drawAlignedString(mW->canvas,&layout,currentChild->text,mW->bodyFont,ALIGN_LEFT,0);
			}

			//Если надо отобразить значение
			if(currentChild->value!=NULL){
				if(showScroll)
					layout.width=mW->layout.width*(1-ITEM_TEXT_RATIO)-ITEM_SPACE_PX-SCROLLBAR_WIDTH-SCROLLBAR_PADDING;
				else
					layout.width=mW->layout.width*(1-ITEM_TEXT_RATIO)-ITEM_SPACE_PX;

				layout.x=mW->layout.x+mW->layout.width*ITEM_TEXT_RATIO+ITEM_SPACE_PX;

				Canvas_drawAlignedString(mW->canvas,&layout,Value_toString(currentChild->value),mW->bodyFont,ALIGN_RIGHT,0);
			}
		}
		currentChild=currentChild->next;
		i++;
	}

	//Отрисовка scrallbar надо вынести этот код в MenuWindow_setRootItem и добавить поле в MenuWindow
	if(showScroll){
		layout.x=mW->layout.x+mW->layout.width-SCROLLBAR_WIDTH;
		layout.y=mW->layout.y+mW->titleHeight;
		layout.width=SCROLLBAR_WIDTH;
		layout.height=mW->layout.height-mW->titleHeight;

		Canvas_drawFrame(mW->canvas,&layout,FRAME_TRANSPARENT);

		layout.height=(int)((float)mW->viewRows/mW->childsCount)*(mW->layout.height-mW->titleHeight);
		layout.width=SCROLLBAR_WIDTH;
		layout.y+=((float)mW->pos/mW->childsCount)*(mW->layout.height-mW->titleHeight);
		Canvas_drawFrame(mW->canvas,&layout,FRAME_WHITE);
	}
}

/*
 * Установка корневого элемента
 * return false-если элемента нет потомков
 */
bool MenuWindow_setRootItem(MenuWindow* menuWindow, MenuItem* rootItem){

	if(!menuWindow->isRunning)
			return false;
	uint8_t childCount=MenuWindow_calculateChildsCount(rootItem);

	if(childCount==0)
		return false;

	menuWindow->rootItem=rootItem;
	menuWindow->childsCount=childCount;
	Stack_push(&menuWindow->stack,rootItem);
	menuWindow->pos=0;
	menuWindow->select=0;
	MenuWindow_resetShift(menuWindow);

	return true;
}


/*
 * Обработка проваливания вглубь
 * возвращает &Null_Menu если смог провалиться
 */
MenuItem* MenuWindow_enter(MenuWindow* menuWindow){
	if(!menuWindow->isRunning)
		return &Null_Menu;

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
 * Сброс сдвига в активной строке
 */
void MenuWindow_resetShift(MenuWindow* menuWindow){
	menuWindow->shStr.shift = 0;
	menuWindow->shStr.shiftFlag = true;
}

/*
 * Обработка выхода
 */
void MenuWindow_back(MenuWindow* menuWindow){
	if(menuWindow->isRunning && menuWindow->stack.pos>1){
		Stack_pop(&menuWindow->stack);
		MenuWindow_setRootItem(menuWindow,Stack_top(&menuWindow->stack));
		Stack_pop(&menuWindow->stack);
	}
}

/*
 * Обработка инкрементирования позиции
 */
void MenuWindow_incPosition(MenuWindow* menuWindow){
	if (menuWindow->isRunning && menuWindow->select < menuWindow->childsCount - 1) {
		menuWindow->select++;
			if (menuWindow->select - menuWindow->pos == menuWindow->viewRows) {
				menuWindow->pos++;
			}
			MenuWindow_resetShift(menuWindow);
		}
}

/*
 * Обработка декрементирования позиции
 */
void MenuWindow_decPosition(MenuWindow* menuWindow){
	if (menuWindow->isRunning && menuWindow->select > 0) {
		menuWindow->select--;
			if (menuWindow->select < menuWindow->pos) {
				menuWindow->pos--;
			}
			MenuWindow_resetShift(menuWindow);
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


