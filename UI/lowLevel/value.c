#include <math.h>
#include "value.h"







void Value_Init_As_Int(Value* this, uint16_t* value, char** units, float max, float min){
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_INT;
	this->max= roundf(max);
	if (min <0)min = 0;
	this->min= roundf(min);
}


void Value_Init_As_Float(Value* this, float* value, char** units, float max, float min){
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_INT;
	this->max= max;
	this->min= min;
}


void Value_Init_As_Bool(Value* this, bool* value, char** units){
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_BOOL;
	this->max= 1;
	this->min= 0;
}

void Value_Init_As_Enum(Value* this, uint8_t* value, char** units, float max, float min){
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_ENUM;
	if (min <0)min = 0;
	this->min= roundf(min);
	this->max = sizeof(units) + min;
}





