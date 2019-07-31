#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "value.h"
#define VAL_BUF_SIZE 20



static char valStrBuf[VAL_BUF_SIZE];



//IncDec



void Value_addUnitsToString(Value* this);
void Value_intToString(Value* this);
void Value_floatToString(Value* this);
void Value_boolToString(Value* this);
void Value_enumToString(Value* this);

void Value_incDecFloat(Value* this, enum ValueAct act);
void Value_incDecEnumAndBool(Value* this, enum ValueAct act);
void Value_incDecInt(Value* this, enum ValueAct act);



int Value_initAsInt(Value* this, int* value, char** units, float max, float min){
	if (max<min) {
		while (1){}
	}
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_INT;
	this->max = roundf(max);
	this->min = (min <0)? 0 : roundf(min);
	this->digitsAfterDot = 0;
	return 0;

}

int Value_initAsFloat(Value* this, float* value, char** units, float max, float min, uint8_t digitsAfterDot){
	if (max<min) {
		while (1){}
	}
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_FLOAT;
	this->max= max;
	this->min= min;
	this->digitsAfterDot = digitsAfterDot;
	return 0;
}


int Value_initAsBool(Value* this, bool* value, char** units){
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_BOOL;
	this->max= 1;
	this->min= 0;
	this->digitsAfterDot = 0;
	return 0;
}

int Value_initAsEnum(Value* this, uint8_t* value, char** units,  float min){
	this->vl = (void*)value;
	this->units = units;
	this->type = VALUE_ENUM;
	this->min = (min <0)? 0 : roundf(min);
	this->max = sizeof(units) + min;
	this->digitsAfterDot = 0;
	return 0;
}


char* Value_toString(Value* this){
	switch (this->type){
	case VALUE_INT:
		Value_intToString(this);
		Value_addUnitsToString(this);
		break;
	case VALUE_FLOAT:
		Value_floatToString(this);
		Value_addUnitsToString(this);
		break;
	case VALUE_BOOL:
		Value_boolToString(this);
		break;
	case VALUE_ENUM:
		Value_enumToString(this);
		break;
	}
	return valStrBuf;
}

void Value_addUnitsToString(Value* this){
	int len = strlen(valStrBuf);
	strncat(valStrBuf, " ", VAL_BUF_SIZE-len-1);
	strncat(valStrBuf, this->units[0], VAL_BUF_SIZE-len-2);
	valStrBuf[VAL_BUF_SIZE-1] = '\0';
}

void Value_intToString(Value* this){
	sprintf(valStrBuf,"%d",*(int* )this->vl);
}

void Value_floatToString(Value* this){
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


void Value_boolToString(Value* this){
	uint8_t temp = *(uint8_t* )this->vl;
	if (temp>1) temp = 1;
	strncpy(valStrBuf, this->units[temp], VAL_BUF_SIZE-1);
	valStrBuf[VAL_BUF_SIZE-1] = '\0';
}

void Value_enumToString(Value* this){
	uint8_t temp = *(uint8_t* )this->vl;

	if(temp<this->min || temp>=this->max){
		strcpy(valStrBuf,"nan");
	}
	else{
		strncpy(valStrBuf, this->units[temp-(int)this->min], VAL_BUF_SIZE-1);
		valStrBuf[VAL_BUF_SIZE-1] = '\0';
	}
}


void Value_incDec(Value* this, enum ValueAct act){
	switch (this->type){
	case VALUE_INT:
		Value_incDecInt(this, act);
		break;
	case VALUE_FLOAT:
		Value_incDecFloat(this, act);
		break;
	case VALUE_BOOL:
		Value_incDecEnumAndBool(this, act);
		break;
	case VALUE_ENUM:
		Value_incDecEnumAndBool(this, act);
		break;
	}
}


void Value_inc(Value* this){
	Value_incDec(this, VALUE_INC);
}

void Value_dec(Value* this){
	Value_incDec(this, VALUE_DEC);
}


void Value_incDecInt(Value* this, enum ValueAct act){
	int temp = *(int* )this->vl;
	int min = (int)this->min;
	int max = (int)this->max;
	uint8_t delta = 1;


	temp = (act == VALUE_INC)? temp+delta: temp-delta;
	if (temp < min){
		temp =  min;
	} else if (temp > max){
		temp =  max;
	}

	*(int* )this->vl = temp;
}


void Value_incDecFloat(Value* this, enum ValueAct act){
	float temp = *(float* )this->vl;
	float delta;
	switch (this->digitsAfterDot){
	case 0:
		delta = 1;
		break;
	case 1:
		delta = 0.1;
		break;
	default:
		delta = 0.01;
		break;
	}

	temp = (act == VALUE_INC)? temp+delta: temp-delta;
	if (temp < this->min){
		temp =  this->min;
	} else if (temp > this->max){
		temp =  this->max;
	}
	*(float* )this->vl = temp;
}

void Value_incDecEnumAndBool(Value* this, enum ValueAct act){
	uint8_t temp = *(uint8_t* )this->vl;
	uint8_t delta = 1;
	int min = (int)this->min;
	int max = (int)this->max;

	temp = (act == VALUE_INC)? temp+delta: temp-delta;
	if (temp < min){
		temp =  min;
	} else if (temp > max){
		temp =  max;
	}

	*(uint8_t* )this->vl = temp;
}



