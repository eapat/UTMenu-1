#ifndef __VALUE_H
#define __VALUE_H

#include <stdint.h>
#include <stdbool.h>


#define MAKE_VALUE(Name, Value, Type, Units, max , min, digitsAfterDot) \
		Value Name = {(void*)&Value, Type, Units, max , min, digitsAfterDot}


enum ValueType {
	VALUE_INT,
	VALUE_FLOAT,
	VALUE_BOOL,
	VALUE_ENUM
};



typedef struct {
	void*				vl;
	enum ValueType		type;
	char**				units;
	float				max;
	float				min;
	uint8_t 			digitsAfterDot;	//количество знаков после запятой
} Value;





int Value_initAsInt(Value* this, int* value, char** units, float max, float min);
int Value_initAsFloat(Value* this, float* value, char** units, float max, float min, uint8_t digitsAfterDot);
int Value_initAsBool(Value* this, bool* value, char** units);
int Value_initAsEnum(Value* this, uint8_t* value, char** units,  float min);
char* Value_toString(Value* this);
void Value_inc(Value* this);
void Value_dec(Value* this);





#endif /* VALUE_H_ */
