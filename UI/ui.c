#include "ui.h"
#include "canvas.h"
#include "LCD12864.h"
#include "boolpin.h"
#include "menu.h"
#include "menu_window.h"

Canvas canvas;
Canvas canvas2;
Font fontRegular;
Font fontBold;

//Кнопки
Boolpin btnPrev;
Boolpin btnNext;
Boolpin btnBack;
Boolpin btnFunc;

MenuWindow menuWindow;

//Инициализация пользовательского интерфейса
void UI_init(void){

	Boolpin_init(&btnPrev,BTN_PREV_PORT,BTN_PREV_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnNext,BTN_NEXT_PORT,BTN_NEXT_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnBack,BTN_BACK_PORT,BTN_BACK_PIN,BTN_DEBOUNCE_MS,0,0);
	Boolpin_init(&btnFunc,BTN_FUNC_PORT,BTN_FUNC_PIN,BTN_DEBOUNCE_MS,0,0);

	Font_init(&fontRegular,FONT_5x8, F_SPACING_0, FS_NORMAL, BG_TRANS);
	Font_init(&fontBold,FONT_8x12B, F_SPACING_0, FS_NORMAL, BG_FILL);

	Canvas_init(&canvas,128,64);
	canvas.pen.color=1;

	canvas.pen.style=PS_DOT;

	canvas.brush.color=1;



	LCD_init();

	Layout layout={LAYOUT_FILL,0,0,70,40};

	MenuWindow_init(&menuWindow,&canvas,&fontBold,layout);
	MenuWindow_setMenuList(&menuWindow,&rootItem);
}

//Обработчик пользователского интерфейса, обновление дисплея, кнопок
void UI_handler(void){
	static uint8_t i=0;
	static bool up=true;
	Canvas_clear(&canvas);

	Layout layout={LAYOUT_FILL,0,0,128,64};

	canvas.pen.width=i;

	if(Boolpin_update(&btnPrev))
		{
			i--;
		}

	if(Boolpin_update(&btnNext)){
			i++;
		}

	if(Boolpin_update(&btnFunc))
	{
		canvas.brush.style++;
		if(canvas.brush.style>3)
			canvas.brush.style=0;
	}

	Canvas_drawFrame(&canvas,&layout);
	Canvas_drawString(&canvas,10,10,"Текст",&fontBold);
	//Canvas_drawLineV(&canvas,0,0,40);

	/*

	if(Boolpin_update(&btnPrev))
	{
		MenuWindow_decPosition(&menuWindow);
	}
	if(Boolpin_update(&btnNext)){
		MenuWindow_incPosition(&menuWindow);
	}

	if(Boolpin_update(&btnFunc))
		MenuWindow_enter(&menuWindow);
	if(Boolpin_update(&btnBack))
			MenuWindow_back(&menuWindow);

	MenuWindow_draw(&menuWindow);
	*/

	LCD_draw(&canvas);
}
