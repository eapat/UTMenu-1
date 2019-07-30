#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "value.h"
#define VAL_BUF_SIZE 20

static char valStrBuf[VAL_BUF_SIZE];

//Убрать ValueFactor
//Добавить пробелы
//IncDec
//Стиль_дляМетодовТакойВот
//привести к int

void Value_add_units_to_string(Value* this);
void Value_int_to_string(Value* this);
void Value_float_to_string(Value* this);
void Value_bool_to_string(Value* this);
void Value_enum_to_string(Value* this);



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

int Value_Init_As_Enum(Value* this, uint8_t* value, char** units,  float min){
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_ENUM;
	this->min = (min <0)? 0 : roundf(min);
	this->max = sizeof(units) + min;
	this->factor = VALUE_NO_FACT;
	this->digitsAfterDot = 0;
	return 0;
}


char* Value_to_string(Value* this){
	switch (this->type){
	case VALUE_INT:
		Value_int_to_string(this);
		Value_add_units_to_string(this);
		break;
	case VALUE_FLOAT:
		Value_float_to_string(this);
		Value_add_units_to_string(this);
		break;
	case VALUE_BOOL:
		Value_bool_to_string(this);
		break;
	case VALUE_ENUM:
		Value_enum_to_string(this);
		break;
	}
	return valStrBuf;
}

void Value_add_units_to_string(Value* this){
	int len = strlen(valStrBuf);
	strncat(valStrBuf, this->units[0], VAL_BUF_SIZE-len-1);
	valStrBuf[VAL_BUF_SIZE-1] = '\0';
}

void Value_int_to_string(Value* this){
	sprintf(valStrBuf,"%d",*(uint16_t* )this->vl);
}

void Value_float_to_string(Value* this){
	switch (this->digitsAfterDot){
	case 0:
		sprintf(valStrBuf,"%.0f",*(float* )this->vl);
		break;
	case 1:
		sprintf(valStrBuf,"%.1f",*(float* )this->vl);
		break;
	default:
		sprintf(valStrBuf,"%.2f",*(float* )this->vl);
		break;
	}
}


void Value_bool_to_string(Value* this){
	uint8_t temp = *(uint8_t* )this->vl;
	if (temp>1) temp = 1;
	strncpy(valStrBuf, this->units[temp], VAL_BUF_SIZE-1);
	valStrBuf[VAL_BUF_SIZE-1] = '\0';
}

void Value_enum_to_string(Value* this){
	uint8_t temp = *(uint8_t* )this->vl;

	if(temp<this->min || temp>=this->max){
		strcpy(valStrBuf,"nan");
	}
	else{
		strncpy(valStrBuf, this->units[temp-(int)this->min], VAL_BUF_SIZE-1);
		valStrBuf[VAL_BUF_SIZE-1] = '\0';
	}
}


