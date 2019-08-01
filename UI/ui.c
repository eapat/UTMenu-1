#include "ui.h"
#include "canvas.h"
#include "LCD12864.h"
#include "boolpin.h"
#include "menu.h"
#include "menu_window.h"
#include "value.h"

Canvas canvas;
Canvas canvas2;
Font fontRegular;
Font fontBold;
Font fontBoldS;

//Кнопки
Boolpin btnPrev;
Boolpin btnNext;
Boolpin btnBack;
Boolpin btnFunc;

MenuWindow menuWindow;

Value value;
char * units []={"Вкл.","Откл.","C","BUS"};
int width=60;
int height=20;
Layout layout={0,0,20,60};

//Инициализация пользовательского интерфейса
void UI_init(void){

	Boolpin_init(&btnPrev,BTN_PREV_PORT,BTN_PREV_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnNext,BTN_NEXT_PORT,BTN_NEXT_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnBack,BTN_BACK_PORT,BTN_BACK_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnFunc,BTN_FUNC_PORT,BTN_FUNC_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);

	Font_init(&fontRegular,FONT_5x8, F_SPACING_0, FS_NORMAL, BG_TRANS);
	Font_init(&fontBold,FONT_8x12B, F_SPACING_0, FS_NORMAL, BG_TRANS);
	Font_init(&fontBoldS,FONT_8x12S, F_SPACING_0, FS_INVERT, BG_FILL);

	Canvas_init(&canvas,128,64);







	//Value_Init_As_Int(&value,&fval,units,200,10,0);
	//Value_initAsInt(&value,&fval,units,-20,5);





	LCD_init();

	Layout layout={0,0,70,40};

	Canvas_calculateLayout(&canvas,&layout,LAYOUT_FILL);
	MenuWindow_init(&menuWindow,&canvas,layout,&fontBold,&fontBold);
	MenuWindow_setRootItem(&menuWindow,&rootItem);
}

//Обработчик пользователского интерфейса, обновление дисплея, кнопок
void UI_handler(void){
	Canvas_clear(&canvas);
	static uint8_t n1=0;
	static uint8_t n1Flag=0;
	static uint32_t prevTime=0;
	static bool right=true;
	static bool down=true;
	static bool hitFlag=false;
	uint32_t time=HAL_GetTick();


	if(Boolpin_update(&btnPrev,time))
		width--;
	if(Boolpin_update(&btnNext,time))
		width++;
	if(Boolpin_update(&btnFunc,time))
		height++;
	if(Boolpin_update(&btnBack,time))
		height--;


	if(time-prevTime>300)
	{
		prevTime=time;
		n1=!n1Flag?n1+1:0;
	}

	layout.width=width;
	layout.height=height;

	if(right){
		if(layout.x+layout.width<canvas.width)
			layout.x++;
		else
		{
			right=false;
			hitFlag=true;
		}
	}
	else
	{
		if(layout.x>0)
			layout.x--;
		else
		{
			right=true;
			hitFlag=true;
		}
	}

	if(down){
		if(layout.y+layout.height<canvas.height)
			layout.y++;
		else
		{
			down=false;
			hitFlag=true;
		}
	}
	else
	{
		if(layout.y>0)
			layout.y--;
		else
		{
			down=true;
			hitFlag=true;
		}
	}

	if(hitFlag){
		hitFlag=false;

		Canvas_drawFrame(&canvas,&layout,FRAME_WHITE);
		LCD_draw(&canvas);
		HAL_Delay(30);
	}
	else
	{
		Canvas_drawFrame(&canvas,&layout,FRAME_TRANSPARENT);
		n1Flag=Canvas_drawAlignedString(&canvas,&layout,"Учтех-Профи",&fontBold,ALIGN_CENTER,n1);
		LCD_draw(&canvas);
	}








	/*
	if(up){
		fval++;
		if(fval>80)
			up=false;
	}
	else
	{
		fval--;
		if(fval<2)
			up=true;
	}
	*/


	//Canvas_drawString(&canvas,0,0,"11111",&fontBold);
	//Canvas_drawString(&canvas,0,20,"22222",&fontBold);
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


}
