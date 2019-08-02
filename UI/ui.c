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
	Font_init(&fontBoldS,FONT_8x12S, F_SPACING_0, FS_INVERT, BG_FILL);
	Font_init(&fontTitleIverted,FONT_8x12B, F_SPACING_0, FS_INVERT, BG_FILL);

	Canvas_init(&canvas,128,64);

	//Value_Init_As_Int(&value,&fval,units,200,10,0);
	//Value_initAsInt(&value,&fval,units,-20,5);

	LCD_init();
	Menu_init();

	Layout layout={0,0,70,50};

	Canvas_calculateLayout(&canvas,&layout,LAYOUT_FILL);
	MenuWindow_init(&menuWindow,&canvas,layout,&fontTitleIverted,&fontRegular);

	Layout layout2={0,0,60,40};
	Canvas_calculateLayout(&canvas,&layout2,LAYOUT_CENTER);
	EditWindow_init(&editWindow,&canvas,layout2,&fontTitleIverted,&fontRegular);

	MenuWindow_setRootItem(&menuWindow,&rootItem);
}

//Обработчик пользователского интерфейса, обновление дисплея, кнопок
void UI_handler(void){
	Canvas_clear(&canvas);


	uint32_t time=HAL_GetTick();

	if(Boolpin_update(&btnPrev,time))
	{
		if(EditWindow_isRuning(&editWindow))
			EditWindow_dec(&editWindow);
		else
			MenuWindow_decPosition(&menuWindow);
	}
	if(Boolpin_update(&btnNext,time)){
		if(EditWindow_isRuning(&editWindow))
			EditWindow_inc(&editWindow);
		else
			MenuWindow_incPosition(&menuWindow);
	}

	if(Boolpin_update(&btnFunc,time)){
		if(EditWindow_isRuning(&editWindow))
			EditWindow_enter(&editWindow);
		else
		{
			MenuItem* item=MenuWindow_enter(&menuWindow);
			if(item!=&Null_Menu && item->value!=NULL)
				EditWindow_start(&editWindow,item->value,item->text);
		}
	}

	if(Boolpin_update(&btnBack,time)){
		if(EditWindow_isRuning(&editWindow))
			EditWindow_back(&editWindow);
		else
			MenuWindow_back(&menuWindow);
	}


	MenuWindow_draw(&menuWindow,time);
	if(EditWindow_isRuning(&editWindow))
		EditWindow_draw(&editWindow,time);
	LCD_draw(&canvas);


}
