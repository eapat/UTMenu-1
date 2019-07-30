#include <stdbool.h>
#include "edit_window.h"
#include "value.h"


void EditWindow_init(EditWindow* this,Canvas* canvas,Font* font,Layout layout){
	this->canvas = canvas;
	this->font = font;
	this->layout = layout;
	this->inProgress = false;
}

void EditWindow_start(EditWindow* this, Value* val){
	this->val = val;
	this->inProgress = true;
}

/*
void EditWindow_draw(EditWindow* this){
	char strBuff[20];
	switch(this->val->type){
	case VALUE_INT:
		sprintf(strBuff,"%d",*(uint16_t* )this->val->vl);
	}
}
*/
