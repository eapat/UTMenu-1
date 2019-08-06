#include <stdbool.h>
#include "edit_window.h"
#include "value.h"
#include "time_utilities.h"

#define EW_SHIFT_TIME 300
#define EW_SHIFT_PAUSE 1000

/*
 * Инициализация окна редактирования
 * 	EditWindow* this - целевой объект
 *	Canvas* canvas - канвас с которым работаем
 *	Font* headerFont - шрифт заголовка
 *	Font* bodyFont - шрифт отрисовки значения
 *	Layout layout - область в которой должно быть отрисовано окно
 */

void EditWindow_init(EditWindow* this,Canvas* canvas,Layout layout, Font* headerFont, Font* bodyFont){
	this->canvas = canvas;
	this->header.font = headerFont;
	this->body.font = bodyFont;
	this->mainLayout = layout;
	this->inProgress = false;

	uint8_t dx = this->mainLayout.x>0? 1 : 0;
	uint8_t dy = this->mainLayout.y>0? 1 : 0;
	this->frameLayout.height = this->mainLayout.height + 1 +dy;
	this->frameLayout.width = this->mainLayout.width + 1 + dx;
	this->frameLayout.x = this->mainLayout.x - dx;
	this->frameLayout.y = this->mainLayout.y - dy;

	this->header.layout.height = headerFont->height + headerFont->height/2;
	this->header.layout.width = this->mainLayout.width;
	this->header.layout.x = this->mainLayout.x;
	this->header.layout.y = this->mainLayout.y;
	this->header.style = (headerFont->inversion == FS_INVERT)? FRAME_WHITE: FRAME_BLACK;
	if ( this->mainLayout.height < (this->header.layout.height + bodyFont->height + bodyFont->height/2) ){
	 while(1){
		 //Ошибка инициализации - недостаточная высота Layout при заданных шрифтах
	 }
	}

	this->body.layout.height = this->mainLayout.height - this->header.layout.height;
	this->body.layout.width = this->mainLayout.width;
	this->body.layout.x = this->mainLayout.x;
	this->body.layout.y = this->mainLayout.y + this->header.layout.height;
	this->body.style = (bodyFont->inversion == FS_INVERT)? FRAME_WHITE: FRAME_BLACK;
	this->shStr.shift = 0;
	this->shStr.shiftFlag = true;

}


/*
 * Запуск окна редактирования
 * 	EditWindow* this - целевой объект
 *	Value* val - редактируемое значение
 *	char* header - текст заголовка
 */

void EditWindow_start(EditWindow* this, Value* val, char* header){
	this->vlPt = val;
	Value_copy(this->vlPt,&this->vlCopy);
	this->vlLocal = *(float*)this->vlCopy.vl;
	this->vlCopy.vl = &this->vlLocal;
	this->shStr.shift = 0;
	this->shStr.shiftFlag = true;
	this->inProgress = true;
	this->headerText = header;
	this->lifeTime = 0;
	this->shStr.prevTime=0;
}


/*
 * Отрисовка окна редактирования
 * 	EditWindow* this - целевой объект
 *	uint32_t currentTime - текущее время в ms
 */

void EditWindow_draw(EditWindow* this, uint32_t currentTime){
	if (this->inProgress){
		int delay = (this->shStr.shiftFlag||this->shStr.shift==0)? EW_SHIFT_PAUSE : EW_SHIFT_TIME;

		if(this->shStr.prevTime==0)
			this->shStr.prevTime=currentTime;

		uint32_t delta = TimeUtilities_getDelta32(currentTime,this->shStr.prevTime);

		if (delta > delay){
			this->shStr.prevTime=currentTime;
			this->lifeTime+=delta;
			this->shStr.shift=!this->shStr.shiftFlag?this->shStr.shift+1:0;
		}

		Canvas_drawFrame(this->canvas, &this->frameLayout, FRAME_LAYOUT_MASK);
		Canvas_drawFrame(this->canvas, &this->mainLayout, FRAME_WHITE);
		Canvas_drawFrame(this->canvas, &this->body.layout, this->body.style);
		Canvas_drawFrame(this->canvas, &this->header.layout, this->header.style);
		this->shStr.shiftFlag = Canvas_drawAlignedString(this->canvas,&this->header.layout,this->headerText,this->header.font,ALIGN_CENTER,this->shStr.shift);
		Canvas_drawAlignedString(this->canvas,&this->body.layout,Value_toString(&this->vlCopy),this->body.font,ALIGN_CENTER,0);
	}

}


/*
 * Остановка окна редактирования
 * 	EditWindow* this - целевой объект
 */

void EditWindow_stop(EditWindow* this){
	this->inProgress = false;
}

/*
 * Инкрементирование значения в окне
 * 	EditWindow* this - целевой объект
 */

void EditWindow_inc(EditWindow* this){
	if (this->inProgress){
		Value_inc(&this->vlCopy);
		this->lifeTime=0;
	}
}

/*
 * Декрементирование значения в окне
 * 	EditWindow* this - целевой объект
 */

void EditWindow_dec(EditWindow* this){
	if (this->inProgress){
		Value_dec(&this->vlCopy);
		this->lifeTime=0;
	}
}

/*
 * Выход из окна с созранением изменений
 * 	EditWindow* this - целевой объект
 */
void EditWindow_enter(EditWindow* this){
	if (this->inProgress){
		switch (this->vlPt->type){
		case VALUE_BOOL:
			*(uint8_t*)this->vlPt->vl = *(uint8_t*)this->vlCopy.vl;
			break;
		case VALUE_ENUM:
			*(uint8_t*)this->vlPt->vl = *(uint8_t*)this->vlCopy.vl;
			break;
		case VALUE_FLOAT:
			*(float*)this->vlPt->vl = *(float*)this->vlCopy.vl;
			break;
		case VALUE_INT:
			*(int*)this->vlPt->vl = *(int*)this->vlCopy.vl;
			break;
		}
		EditWindow_stop(this);
	}
}

/*
 * Выход из окна без сохранения изменений
 * 	EditWindow* this - целевой объект
 */

void EditWindow_back(EditWindow* this){
	EditWindow_stop(this);
}

/*
 * Проверка состояния окна
 * 	EditWindow* this - целевой объект
 */
bool EditWindow_isRuning(EditWindow* this){
	return this->inProgress;
}



/*
 * Получить время существования окна
 * 	EditWindow* this - целевой объект
 */

uint32_t EditWindow_getLifeTime(EditWindow* this){
	return this->lifeTime;
}

