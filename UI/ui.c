#include "ui.h"
#include "canvas.h"
#include "LCD12864.h"
#include "boolpin.h"

Canvas canvas;
Font fontRegular;

//Кнопки
Boolpin btnPrev;
Boolpin btnNext;
Boolpin btnBack;
Boolpin btnFunc;

//Инициализация пользовательского интерфейса
void UI_init(void){

	Boolpin_init(&btnPrev,BTN_PREV_PORT,BTN_PREV_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);
	Boolpin_init(&btnNext,BTN_NEXT_PORT,BTN_NEXT_PIN,BTN_DEBOUNCE_MS,BTN_LONG_DELAY_MS,BTN_LONG_PERIOD_MS);

	Font_init(&fontRegular,FONT_5x8, F_SPACING_0, FS_NORMAL, BG_TRANS);

	Canvas_init(&canvas,&fontRegular,128,64);

	LCD_init();




	LCD_draw(&canvas);

}

//Обработчик пользователского интерфейса, обновление дисплея, кнопок
void UI_handler(void){
	Canvas_clear(&canvas);
	static int i;

	if(Boolpin_update(&btnPrev))
			i--;
	if(Boolpin_update(&btnNext))
			i++;

	sprintf(canvas.strBuff,"%d",i);
	Canvas_drawString(&canvas,0,0,canvas.strBuff);
	LCD_draw(&canvas);

}
