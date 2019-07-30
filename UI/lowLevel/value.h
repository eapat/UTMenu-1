#ifndef __VALUE_H
#define __VALUE_H

#include <stdint.h>
#include <stdbool.h>


#define MAKE_VALUE(Name, Value, Type, Units, max , min, factor, digitsAfterDot) \
		Value Name = {(void*)&Value, Type, Units, max , min, factor, digitsAfterDot}


enum ValueType {
	VALUE_INT,
	VALUE_FLOAT,
	VALUE_BOOL,
	VALUE_ENUM
};

enum ValueFactor {
	VALUE_NO_FACT,
	VALUE_FACT_KILO,
	VALUE_FACT_MEGA,
	VALUE_FACT_MILLI,
	VALUE_FACT_AUTO
};

typedef struct {
	void*				vl;
	enum ValueType		type;
	char**				units;
	float				max;
	float				min;
	enum ValueFactor	factor;
	uint8_t 			digitsAfterDot;	//количество знаков после запятой
} Value;





int Value_Init_As_Int(Value* this, uint16_t* value, char** units, float max, float min, enum ValueFactor factor);
int Value_Init_As_Float(Value* this, float* value, char** units, float max, float min, enum ValueFactor factor, uint8_t dAD);
int Value_Init_As_Bool(Value* this, bool* value, char** units);
int Value_Init_As_Enum(Value* this, uint8_t* value, char** units, float max, float min);
char* Value_to_string(Value* this);





#endif /* VALUE_H_ */
