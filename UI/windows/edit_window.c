#include <stdbool.h>
#include "edit_window.h"
#include "value.h"


void EditWindow_init(EditWindow* this,Canvas* canvas,Font* headerFont, Font* bodyFont, Layout layout){
	this->canvas = canvas;
	this->headerFont = headerFont;
	this->bodyFont = bodyFont;
	this->mainLayout = layout;
	this->inProgress = false;

	this->headerLayout.height = headerFont->height + headerFont->height/2;
	this->headerLayout.width = this->mainLayout.width;
	this->headerLayout.x = this->mainLayout.x;
	this->headerLayout.y = this->mainLayout.y;
	this->headerStyle = (headerFont->inversion == FS_INVERT)? FRAME_WHITE: FRAME_BLACK;
	if ( this->mainLayout.height < (this->headerLayout.height + bodyFont->height + bodyFont->height/2) ){
	 while(1){
		 //Ошибка инициализации - недостаточная высота Layout при заданных шрифтах
	 }
	}

	this->bodyLayout.height = this->mainLayout.height - this->headerLayout.height;
	this->bodyLayout.width = this->mainLayout.width;
	this->bodyLayout.x = this->mainLayout.x;
	this->bodyLayout.y = this->mainLayout.y + this->headerLayout.height;
	this->bodyStyle = (bodyFont->inversion == FS_INVERT)? FRAME_WHITE: FRAME_BLACK;

}

void EditWindow_start(EditWindow* this, Value* val, char* header){
	this->vlPt = val;
	Value_copy(this->vlPt,&this->vlCopy);
	this->inProgress = true;
	this->headerText = header;
}


void EditWindow_draw(EditWindow* this){
	if (this->inProgress){
		Canvas_drawFrame(this->canvas, &this->mainLayout, FRAME_WHITE);
		Canvas_drawFrame(this->canvas, &this->bodyLayout, this->bodyStyle);
		Canvas_drawFrame(this->canvas, &this->headerLayout, this->headerStyle);
		Canvas_drawAlignedString(this->canvas,&this->headerLayout,this->headerFont,this->headerText,ALIGN_CENTER,0);
		Canvas_drawAlignedString(this->canvas,&this->bodyLayout,this->bodyFont,Value_toString(&this->vlCopy),ALIGN_CENTER,0);
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
