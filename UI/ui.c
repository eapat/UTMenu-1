#include "ui.h"
#include "canvas.h"
#include "LCD12864.h"
#include "boolpin.h"
#include "menu.h"
#include "menu_window.h"
#include "edit_window.h"

Canvas canvas;
Canvas canvas2;
Font fontRegular;
Font fontBold;
Font fontBoldS;

Font fontTitleIverted;

//Кнопки
Boolpin btnPrev;
Boolpin btnNext;
Boolpin btnBack;
Boolpin btnFunc;

MenuWindow menuWindow;
EditWindow editWindow;

bool testBool;
int testInt;
float testFloat;
int testEnum;




//Инициализация пользовательского интерфейса
void UI_init(void){

	Boolpin_init(&btnPrev,BTN_PREV_PORT,BTN_PREV_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnNext,BTN_NEXT_PORT,BTN_NEXT_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnBack,BTN_BACK_PORT,BTN_BACK_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnFunc,BTN_FUNC_PORT,BTN_FUNC_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);

	Font_init(&fontRegular,FONT_5x8, F_SPACING_0, FS_NORMAL, BG_TRANS);
	Font_init(&fontBold,FONT_8x12B, F_SPACING_0, FS_NORMAL, BG_TRANS);
	Font_init(&fontBoldS,FONT_8x12S, F_SPACING_0, FS_NORMAL, BG_FILL);
	Font_init(&fontTitleIverted,FONT_8x12B, F_SPACING_0, FS_INVERT, BG_FILL);

	Canvas_init(&canvas,128,64);
	LCD_init();
	Menu_init();

	Layout layout={0,0,70,30};

	Canvas_calculateLayout(&canvas,&layout,LAYOUT_FILL);
	MenuWindow_init(&menuWindow,&canvas,layout,&fontTitleIverted,&fontRegular);

	Layout layout2={0,0,70,50};
	Canvas_calculateLayout(&canvas,&layout2,LAYOUT_CENTER);
	EditWindow_init(&editWindow,&canvas,layout2,&fontTitleIverted,&fontBoldS);

	MenuWindow_setRootItem(&menuWindow,&rootItem);
}

//Обработчик пользователского интерфейса, обновление дисплея, кнопок
void UI_handler(void){
	uint32_t time=HAL_GetTick();
	Canvas_clear(&canvas);

	testInt++;
	testFloat=testInt/3251.0;

	if(testBool)
		testInt=0;

	if(Boolpin_update(&btnPrev,time))
	{
		if(EditWindow_isRuning(&editWindow))
			EditWindow_dec(&editWindow);
		else if(MenuWindow_isRunning(&menuWindow))
			MenuWindow_decPosition(&menuWindow);
	}
	if(Boolpin_update(&btnNext,time)){
		if(EditWindow_isRuning(&editWindow))
			EditWindow_inc(&editWindow);
		else if(MenuWindow_isRunning(&menuWindow))
			MenuWindow_incPosition(&menuWindow);
	}

	if(Boolpin_update(&btnFunc,time)){
		if(EditWindow_isRuning(&editWindow))
			EditWindow_enter(&editWindow);
		else if(MenuWindow_isRunning(&menuWindow))
		{
			MenuItem* item=MenuWindow_enter(&menuWindow);

			if(item!=&Null_Menu)
					if(item->function!=NULL)
						item->function();
					else if(item->value!=NULL)
						EditWindow_start(&editWindow,item->value,item->text);
		}
		else{
			MenuWindow_start(&menuWindow);
		}
	}

	if(Boolpin_update(&btnBack,time)){
		if(EditWindow_isRuning(&editWindow))
			EditWindow_back(&editWindow);
		else if(MenuWindow_isRunning(&menuWindow))
			MenuWindow_back(&menuWindow);
	}

	if (MenuWindow_isRunning(&menuWindow)){
			MenuWindow_draw(&menuWindow,time);
	}

	if(EditWindow_isRuning(&editWindow))
	{
		EditWindow_draw(&editWindow,time);
		if(EditWindow_getLifeTime(&editWindow)>2000){
			EditWindow_stop(&editWindow);
		}
	}

	else if(MenuWindow_isRunning(&menuWindow)){
		if(MenuWindow_getLifeTime(&menuWindow)>2000)
			MenuWindow_stop(&menuWindow);
	}


	LCD_draw(&canvas);
}
