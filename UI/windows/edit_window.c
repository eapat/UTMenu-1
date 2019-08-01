#include <stdbool.h>
#include "edit_window.h"
#include "value.h"
#define EW_SHIFT_TIME 300
#define EW_SHIFT_PAUSE 1000

void EditWindow_init(EditWindow* this,Canvas* canvas,Font* headerFont, Font* bodyFont, Layout layout){
	this->canvas = canvas;
	this->header.font = headerFont;
	this->body.font = bodyFont;
	this->mainLayout = layout;
	this->inProgress = false;

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

}

void EditWindow_start(EditWindow* this, Value* val, char* header){
	this->vlPt = val;
	Value_copy(this->vlPt,&this->vlCopy);
	this->inProgress = true;
	this->headerText = header;
}


void EditWindow_draw(EditWindow* this, uint32_t currentTime){
	if (this->inProgress){
		int delay = !this->shStr.shiftFlag? EW_SHIFT_PAUSE : EW_SHIFT_TIME;
		if ((currentTime-this->shStr.prevTime) > delay){
			this->shStr.prevTime=currentTime;
			this->shStr.shift=!this->shStr.shiftFlag?this->shStr.shiftFlag+1:0;
		}
		Canvas_drawFrame(this->canvas, &this->mainLayout, FRAME_WHITE);
		Canvas_drawFrame(this->canvas, &this->body.layout, this->body.style);
		Canvas_drawFrame(this->canvas, &this->header.layout, this->header.style);
		this->shStr.shiftFlag = Canvas_drawAlignedString(this->canvas,&this->header.layout,this->header.font,this->headerText,ALIGN_CENTER,this->shStr.shift);
		Canvas_drawAlignedString(this->canvas,&this->body.layout,this->body.font,Value_toString(&this->vlCopy),ALIGN_CENTER,0);
	}

}

void EditWindow_stop(EditWindow* this){
	this->inProgress = false;
}

void EditWindow_inc(EditWindow* this){
	if (this->inProgress){
		Value_inc(this);
	}
}

void EditWindow_dec(EditWindow* this){
	if (this->inProgress){
		Value_dec(this);
	}
}

void EditWindow_enter(EditWindow* this){
	if (this->inProgress){
		Value_copy(&this->vlCopy,this->vlPt);
		EditWindow_stop(this);
	}
}

void EditWindow_back(EditWindow* this){
	EditWindow_stop(this);
}

bool EditWindow_isRuning(EditWindow* this){
	return this->inProgress;
}
