#include <math.h>
#include "value.h"







int Value_Init_As_Int(Value* this, uint16_t* value, char** units, float max, float min, enum ValueFactor factor){
	if (max<min) return -1;
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_INT;
	this->factor = (factor == VALUE_FACT_MILLI)? VALUE_NO_FACT: factor;
	this->max = roundf(max);
	this->min = (min <0)? 0 : roundf(min);
	this->digitsAfterDot = 0;
	return 0;

}

int Value_Init_As_Float(Value* this, float* value, char** units, float max, float min, enum ValueFactor factor, uint8_t dAD){
	if (max<min) return -1;
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_FLOAT;
	this->factor = factor;
	this->max= max;
	this->min= min;
	this->digitsAfterDot = dAD;
	return 0;
}


int Value_Init_As_Bool(Value* this, bool* value, char** units){
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_BOOL;
	this->max= 1;
	this->min= 0;
	this->factor = VALUE_NO_FACT;
	this->digitsAfterDot = 0;
	return 0;
}

int Value_Init_As_Enum(Value* this, uint8_t* value, char** units, float max, float min){
	if (max<min) return -1;
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_ENUM;
	this->min = (min <0)? 0 : roundf(min);
	this->max = sizeof(units) + min;
	this->factor = VALUE_NO_FACT;
	this->digitsAfterDot = 0;
	return 0;
}


char* Value_to_string(void){

}



